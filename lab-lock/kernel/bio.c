// Buffer cache.
//
// The buffer cache is a linked list of buf structures holding
// cached copies of disk block contents.  Caching disk blocks
// in memory reduces the number of disk reads and also provides
// a synchronization point for disk blocks used by multiple processes.
//
// Interface:
// * To get a buffer for a particular disk block, call bread.
// * After changing buffer data, call bwrite to write it to disk.
// * When done with the buffer, call brelse.
// * Do not use the buffer after calling brelse.
// * Only one process at a time can use a buffer,
//     so do not keep them longer than necessary.


#include "types.h"
#include "param.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "riscv.h"
#include "defs.h"
#include "fs.h"
#include "buf.h"

#define NBUCKET 13
#define hash(dev, blockno) (((dev+1) * (blockno+1)) % NBUCKET)

extern uint ticks;

struct bbucket {
  struct spinlock lock; // lock protects bucket list and buf(dev, blockno, refcnt, ts) in the list
  struct buf head; // each bucket is a double-linked cyclic linked list
};

struct {
  struct buf buf[NBUF];

  struct spinlock lock; // lock protects buf[NBUF] resource pool and synchronize access when eviction
  struct bbucket buckets[NBUCKET];
} bcache;

/**
 * install buf at bucket head
 */
void
install_buf_into_bucket(struct bbucket *bucket, struct buf *buf)
{
  buf->next = bucket->head.next;
  buf->prev = &bucket->head;
  bucket->head.next->prev = buf;
  bucket->head.next = buf;
}

void
binit(void)
{
  initlock(&bcache.lock, "bcache");

  for(struct buf *b = bcache.buf; b < bcache.buf+NBUF; b++){
    initsleeplock(&b->lock, "buffer");
  }
  // init each bucket lock and bucket list
  for (int i = 0; i < NBUCKET; i++) {
    initlock(&bcache.buckets[i].lock, "bcache.bucket");
    bcache.buckets[i].head.next = bcache.buckets[i].head.prev = &bcache.buckets[i].head;
  }
  // evenly distribute empty bufs into all buckets
  for (int i = 0; i < NBUF; i++) {
    install_buf_into_bucket(&bcache.buckets[i%NBUCKET], &bcache.buf[i]);
  }
}

// Look through buffer cache for block on device dev.
// If not found, allocate a buffer.
// In either case, return locked buffer.
static struct buf*
bget(uint dev, uint blockno)
{
  struct buf *b;
  uint bucket_idx = hash(dev, blockno);
  struct buf *victim = 0;
  uint min_ts = ticks+1;

  acquire(&bcache.buckets[bucket_idx].lock);
  for(b = bcache.buckets[bucket_idx].head.next; b != &bcache.buckets[bucket_idx].head; b = b->next){
    // Is the block already cached in its bucket list?
    if(b->dev == dev && b->blockno == blockno){
      b->refcnt++;
      release(&bcache.buckets[bucket_idx].lock);
      acquiresleep(&b->lock);
      return b;
    }
    // or select a min-timestamp victim in this bucket list if any
    if (b->refcnt == 0 && b->ts < min_ts) {
      min_ts = b->ts;
      victim = b;
    }
  }
  // found a victim in **THIS SAME** bucket, do nothing but set block metadata
  if ((b=victim)) {
    // set new block
    b->dev = dev;
    b->blockno = blockno;
    b->valid = 0;
    b->refcnt = 1;
    release(&bcache.buckets[bucket_idx].lock);
    acquiresleep(&b->lock);
    return b;
  }
  release(&bcache.buckets[bucket_idx].lock);

  // Not cached, **steal** from other bucket

  acquire(&bcache.lock);
  /**
   * select same dev and blockno buf if any
   *
   * **THERE IS A CHANCE** that two concurrent process both access same block,
   * but both found **NOT CACHED**, so they both need to acquire bcache.lock
   * to alloc a new buf and set new data into the buf.
   * BUT only one process can get lock and do this,
   * at the time the second process get the lock, the buf it needed is already available,
   * so the second process only need to scan buf[NBUF] resource pool
   *
   * bcache.lock protects this buf[NBUF] resource pool
   */
  for (b = bcache.buf; b < bcache.buf+NBUF; b++) {
    if (b->dev == dev && b->blockno == blockno) { // bcache.lock protects dev and blockno
      b->refcnt++;
      release(&bcache.lock);
      acquiresleep(&b->lock);
      return b;
    }
  }

  // steal one unused buf from other bucket
  struct bbucket *bucket;
  for (int i = 1; i < NBUCKET; i++) {
    bucket = &bcache.buckets[(bucket_idx+i)%NBUCKET]; // start from next bucket
    min_ts = ticks+1;
    acquire(&bucket->lock);
    for(b = bucket->head.next; b != &bucket->head; b = b->next){
      if (b->refcnt == 0 && b->ts < min_ts) {
        min_ts = b->ts;
        victim = b;
      }
    }
    if (victim) {
      // hold bucket lock if victim found in the bucket
      break;
    } else {
      release(&bucket->lock);
    }
  }

  if (!(b=victim))
    // at this time, there should be at least one unused buf available as victim
    panic("bget: no buffers");

  // remove from the bucket
  b->next->prev = b->prev;
  b->prev->next = b->next;
  release(&bucket->lock);

  // set new block
  b->dev = dev;
  b->blockno = blockno;
  b->valid = 0;
  b->refcnt = 1;
  release(&bcache.lock); // bcache.lock protects dev and blockno, its protection is finished here

  // install item in new bucket
  acquire(&bcache.buckets[bucket_idx].lock);
  install_buf_into_bucket(&bcache.buckets[bucket_idx], b);
  release(&bcache.buckets[bucket_idx].lock);

  acquiresleep(&b->lock);
  return b;
}

// Return a locked buf with the contents of the indicated block.
struct buf*
bread(uint dev, uint blockno)
{
  struct buf *b;

  b = bget(dev, blockno);
  if(!b->valid) {
    virtio_disk_rw(b, 0);
    b->valid = 1;
  }
  return b;
}

// Write b's contents to disk.  Must be locked.
void
bwrite(struct buf *b)
{
  if(!holdingsleep(&b->lock))
    panic("bwrite");
  virtio_disk_rw(b, 1);
}

// Release a locked buffer.
// Move to the head of the most-recently-used list.
void
brelse(struct buf *b)
{
  if(!holdingsleep(&b->lock))
    panic("brelse");

  releasesleep(&b->lock);

  uint bucket_idx = hash(b->dev, b->blockno);

  acquire(&bcache.buckets[bucket_idx].lock);
  b->refcnt--;
  if (b->refcnt == 0) {
    // no one is waiting for it.
    // record access time-stamp
    // but not remove from current bucket until evicted
    b->ts = ticks;
  }
  release(&bcache.buckets[bucket_idx].lock);
}

void
bpin(struct buf *b) {
  acquire(&bcache.buckets[hash(b->dev, b->blockno)].lock);
  b->refcnt++;
  release(&bcache.buckets[hash(b->dev, b->blockno)].lock);
}

void
bunpin(struct buf *b) {
  acquire(&bcache.buckets[hash(b->dev, b->blockno)].lock);
  b->refcnt--;
  release(&bcache.buckets[hash(b->dev, b->blockno)].lock);
}


