// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"

void freerange(void *pa_start, void *pa_end);

extern char end[]; // first address after kernel.
                   // defined by kernel.ld.

struct run {
  struct run *next;
};

struct kmem {
  struct spinlock lock;
  struct run *freelist;
  uint32 nfree; // record # of free page in freelist
};

struct kmem kmems[NCPU];

void
kinit()
{
  for (int i = 0; i < NCPU; i++) {
    initlock(&kmems[i].lock, "kmem");
    kmems[i].nfree = 0;
  }
  freerange(end, (void*)PHYSTOP);
}

void
freerange(void *pa_start, void *pa_end)
{
  char *p;
  p = (char*)PGROUNDUP((uint64)pa_start);
  for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE)
    kfree(p);
}

// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(void *pa)
{
  struct run *r;

  if(((uint64)pa % PGSIZE) != 0 || (char*)pa < end || (uint64)pa >= PHYSTOP)
    panic("kfree");

  // Fill with junk to catch dangling refs.
  memset(pa, 1, PGSIZE);

  r = (struct run*)pa;

  push_off();
  int cid = cpuid();
  pop_off();

  acquire(&kmems[cid].lock);
  r->next = kmems[cid].freelist;
  kmems[cid].freelist = r;
  kmems[cid].nfree++;
  release(&kmems[cid].lock);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *
kalloc(void)
{
  struct run *r;

  push_off();
  int cid = cpuid();
  pop_off();

  acquire(&kmems[cid].lock);
  r = kmems[cid].freelist;
  if(r) {
    kmems[cid].freelist = r->next;
    kmems[cid].nfree--;
  }
  release(&kmems[cid].lock);

  // steal
  if (!r) {
    int stolen_nfree = 0;
    // cycle around all but this CPUs' kmem
    for (int i = 0; i < NCPU-1; i++) {
      int id = (cid + i + 1) % NCPU;
      acquire(&kmems[id].lock);
      if (kmems[id].nfree > 1) { // if only one free page available, too poor, not steal
        // steal !!right half!! of available free pages
        int passed = 0;
        struct run *prev;
        r = kmems[id].freelist;
        while (passed++ < kmems[id].nfree / 2) {
          prev = r;
          r = r->next;
        }

        prev->next = 0; // terminate stolen free list !!left half!!
        stolen_nfree = kmems[id].nfree - passed; // stolen # of free page
        kmems[id].nfree = passed; // reset # of free page
      }
      release(&kmems[id].lock);
      if (r)
        break;
    }

    if (r) {
      // install stolen free list to this CPU's kmem
      acquire(&kmems[cid].lock);
      kmems[cid].freelist = r->next;
      kmems[cid].nfree = stolen_nfree - 1;
      release(&kmems[cid].lock);
    }
  }

  if(r)
    memset((char*)r, 5, PGSIZE); // fill with junk
  return (void*)r;
}
