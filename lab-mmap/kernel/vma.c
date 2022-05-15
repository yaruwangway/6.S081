#include "param.h"
#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "fcntl.h"
#include "spinlock.h"
#include "proc.h"
#include "fs.h"
#include "sleeplock.h"
#include "file.h"
#include "vma.h"

struct {
  struct spinlock lock;
  struct vma vma[NVMA];
} vmatable;

void
vmainit()
{
  initlock(&vmatable.lock, "vmatable");
  for (int i = 0; i < NVMA; i++) {
    vmatable.vma[i].addr = -1;
  }
}

/**
 * alloc a vma from global vma resource pool
 */
struct vma*
vmaalloc(uint64 addr)
{
  struct vma *v = 0;
  acquire(&vmatable.lock);
  for (int i = 0; i < NVMA; i++) {
    if (vmatable.vma[i].addr == -1) {
      v = &vmatable.vma[i];
      v->addr = addr;
      break;
    }
  }
  release(&vmatable.lock);

  return v;
}

/**
 * find an unused region in the process's address space,
 * to which vma maps to
 */
uint64
vmaaddr(uint64 addr, uint64 length)
{
  if (addr != 0)
    return -1; // only support addr: NULL

  struct proc *p = myproc();
  /**
   * vma addr start from page below trapframe in user virtual address
   * and grow to low address
   */
  addr = (uint64)p->trapframe;
  for (struct vma *v = p->vmas; v; v = v->next) {
    // find lowest used vma address
    if (v->addr < addr) {
      addr = v->addr;
    }
  }

  return PGROUNDDOWN(addr - length); // PGSIZE aligned
}

/**
 * copy src vma list to dest
 */
int
vmacopy(struct vma *src, struct vma **dest)
{
  struct vma head;
  head.next = 0;
  struct vma *p = &head;
  for (struct vma *v = src; v; v = v->next) {
    p->next = vmaalloc(v->addr);
    if (!p->next) // no more vma available
      goto bad;
    p = p->next;

    // copy from v
    p->length = v->length;
    p->prot = v->prot;
    p->flags = v->flags;
    p->f = filedup(v->f);
    p->next = 0; // BUT dont copy v->next
  }

  *dest = head.next;
  return 0;

bad:
  // return all alloced vmas back to recourse pool
  acquire(&vmatable.lock);
  for (struct vma *v = head.next; v; v = v->next)
    v->addr = -1;
  release(&vmatable.lock);
  return -1;
}

/**
 * - find an unused region in the process's address space in which to map the file
 * - add a VMA to the process's table of mapped regions
 */
uint64
mmap(uint64 addr, uint64 length, int prot, int flags, struct file* f)
{
  addr = vmaaddr(addr, length);
  if (addr == -1)
    return -1;

  struct vma *v = vmaalloc(addr);
  if (!v)
    return -1;

  // initialize this vma
  v->length = length;
  v->prot = prot;
  v->flags = flags;
  v->f = filedup(f);

  // install vma into process vma list
  struct proc *p = myproc();
  v->next = p->vmas;
  p->vmas = v;

  return addr;
}

/**
 * - find the VMA for the address range
 * - unmap the specified pages
 * - If munmap removes all pages of a previous mmap,
 * -- it should decrement the reference count of the corresponding struct file
 * - If an unmapped page has been modified and the file is mapped MAP_SHARED,
 * -- write the page back to the file.
 *
 * addr is PGSIZE aligned
 */
uint64
munmap(uint64 addr, uint64 length)
{
  struct vma *v;
  if ((v = in_vma(addr)) == 0)
    return -1;

  /**
   * munmap call might cover only a portion of an mmap-ed region,
   * but you can assume that it will either unmap at the start,
   * or at the end, or the whole region
   * (but not punch a hole in the middle of a region).
   */
  if (v->addr != addr && (v->addr+v->length) != addr+length)
    return -1;

  struct proc *p = myproc();
  // write page back to file, if shared and dirty
  if (v->flags & MAP_SHARED) {
    pte_t *pte;
    for (uint64 va = addr; va - addr < length; va += PGSIZE) {
      pte = walk(p->pagetable, va, 0);
      if (pte && (*pte & PTE_U) && (*pte & PTE_V) && (*pte & PTE_D)) {
        begin_op();
        ilock(v->f->ip);
        writei(v->f->ip, 1, va, va - addr, PGSIZE);
        iunlock(v->f->ip);
        end_op();
      }
    }
  }

  // unmap from page table
  int npages = (PGROUNDDOWN(addr+length) - addr) / PGSIZE;
  uvmunmap(p->pagetable, addr, npages, UNMAP_FLAG_DO_FREE | UNMAP_FLAG_INGORE_INVALID);

  /**
   * unmap a whole vma
   * - close its file to decrement fie ref count, and clean file if necessary
   * - remove from proccess's vma list
   */
  if (addr == v->addr && length == v->length) {
    fileclose(v->f);

    struct vma *vp = p->vmas, *prev = 0;
    for (; vp && vp != v; prev = vp, vp = vp->next)
      ;
    if (!vp)
      panic("munmap: remove from process's vma list");

    // remove from process's vma list
    if (prev)
      prev->next = vp->next;
    else
      p->vmas = vp->next;

    // return vma back to global vma resource pool
    acquire(&vmatable.lock);
    vp->addr = -1;
    release(&vmatable.lock);
  }
  // resize vma
  else {
    v->length -= length;
    if (addr == v->addr)
      v->addr += length;
  }

  return 0;
}

/**
 * check addr is in proc's vma mmap-ed region
 * if in, return the vma that contains this addr
 * else, return 0
 */
struct vma*
in_vma(uint64 addr)
{
  struct proc *p = myproc();
  for (struct vma *v = p->vmas; v; v = v->next) {
    if (addr >= v->addr && addr < v->addr + v->length)
      return v;
  }
  return 0;
}

/**
 * - allocate a page of physical memory
 * - read 4096 bytes of the relevant file into that page
 * - map it into the user address space
 */
int
alloc_vma(uint64 addr, struct vma *v)
{
  if (addr < v->addr || addr >= v->addr + v->length)
    panic("alloc vma: addr check");

  // alloc physical memory
  char *mem = kalloc();
  if (mem == 0)
    return -1;

  // read file content
  ilock(v->f->ip);
  int tot = readi(v->f->ip, 0, (uint64)mem, addr - v->addr, PGSIZE);
  iunlock(v->f->ip);

  // zero out remaining memory if file content cannot fulfill
  if (tot < PGSIZE) {
    memset(mem + tot, 0, PGSIZE - tot);
  }

  // map this physical page into page table
  if (mappages(
          myproc()->pagetable, addr, PGSIZE, (uint64)mem,
          PTE_V | PTE_U |
              (v->prot & PROT_READ ? PTE_R : 0) |
              (v->prot & PROT_WRITE ? PTE_W : 0) |
              (v->prot & PROT_EXEC ? PTE_X : 0)) != 0)
    panic("alloc vma: mappages");

  return 0;
}
