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

struct {
  struct spinlock lock;
  struct run *freelist;
  uint8 references[NREFERENCE];
} kmem;

void
kinit()
{
  initlock(&kmem.lock, "kmem");
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

  acquire(&kmem.lock);
  if (kmem.references[PA2REFERENCE_INDEX((uint64)pa)]) {
    release(&kmem.lock);
    return;
  }
  release(&kmem.lock);

  // Fill with junk to catch dangling refs.
  memset(pa, 1, PGSIZE);

  r = (struct run*)pa;

  acquire(&kmem.lock);
  r->next = kmem.freelist;
  kmem.freelist = r;
  release(&kmem.lock);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *
kalloc(void)
{
  struct run *r;

  acquire(&kmem.lock);
  r = kmem.freelist;
  if(r) {
    if (kmem.references[PA2REFERENCE_INDEX((uint64)r)])
      panic("kalloc: physical page reference count is non-zero");
    kmem.freelist = r->next;
  }
  release(&kmem.lock);

  if(r)
    memset((char*)r, 5, PGSIZE); // fill with junk
  return (void*)r;
}

void
kincr_refcount(uint64 pa)
{
  if ((char*)pa < end || pa >= PHYSTOP)
    return;
  acquire(&kmem.lock);
  kmem.references[PA2REFERENCE_INDEX(pa)]++;
  release(&kmem.lock);
}

void
kdecr_refcount(uint64 pa)
{
  if ((char*)pa < end || pa >= PHYSTOP)
    return;
  acquire(&kmem.lock);
  kmem.references[PA2REFERENCE_INDEX(pa)]--;
  release(&kmem.lock);
}

uint8
kget_refcount(uint64 pa)
{
  if ((char*)pa < end || pa >= PHYSTOP)
    return 100;
  acquire(&kmem.lock);
  uint8 cnt = kmem.references[PA2REFERENCE_INDEX(pa)];
  release(&kmem.lock);
  return cnt;
}

#include "proc.h"
extern struct proc proc[];

void
walkcount(pagetable_t pagetable, uint8 refs[], uint64 trapframe)
{
  for (int i = 0; i < 512; i++) {
    pte_t pte = pagetable[i];
    uint64 pa = PTE2PA(pte);
    if (pte & PTE_V) {
      if ((pte & (PTE_R | PTE_W | PTE_X)) == 0) {
        walkcount((pagetable_t)pa, refs, trapframe);
      } else {
        if ((char*)pa < end || pa >= PHYSTOP || pa == TRAMPOLINE || pa == trapframe)
          continue;
        refs[PA2REFERENCE_INDEX(pa)]++;
      }
    }
  }
}

uint8 refs[NREFERENCE];
void
kcheck_invariant()
{
  acquire(&kmem.lock);
  // all free page should 0 ref count
  struct run *r = kmem.freelist;
  while (r) {
    if (kmem.references[PA2REFERENCE_INDEX((uint64)r)]) {
      int idx = PA2REFERENCE_INDEX((uint64)r);
      printf("r: %p, idx: %d, refcount: %d\n", (uint64)r, idx, kmem.references[idx]);
      panic("check invariant: ref count not zero");
    }
    r = r->next;
  }

  // check reference count from all process's pagetable
  // it should be same as kmem.references
  struct proc *p;
  pagetable_t pagetable;
  for (int i = 0; i < NREFERENCE; i++)
    refs[i] = 0;
  for (p = proc; p < &proc[NPROC]; p++) {
    pagetable = p->pagetable;
    if (pagetable != 0)
      walkcount(pagetable, refs, (uint64)p->trapframe);
  }
  for (int i = 0; i < NREFERENCE; i++) {
    if (refs[i] != kmem.references[i]) {
      uint64 ppa = i*PGSIZE+KERNBASE;
      printf("pa: %p, idx: %d, refcount: %d, kmem.refcount: %d\n", ppa, i, refs[i], kmem.references[i]);
      panic("check invariant: ref count not same");
    }
  }
  release(&kmem.lock);
}
