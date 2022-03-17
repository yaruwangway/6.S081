#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "date.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;


  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}


#ifdef LAB_PGTBL
#define MAX_NPAGES_TO_SCAN 512
#ifndef CHAR_BIT
#define CHAR_BIT 8
#endif
int
sys_pgaccess(void)
{
  // lab pgtbl: your code here.
  uint64 va;
  int npages;
  uint64 bufp;
  if (argaddr(0, &va) < 0 ||
      argint(1, &npages) < 0 ||
      argaddr(2, &bufp) < 0 ||
      npages < 0 ||
      npages > MAX_NPAGES_TO_SCAN)
    return -1;

  int npages_roundup = (npages+CHAR_BIT-1) & ~(CHAR_BIT-1);
  char temp[npages_roundup / CHAR_BIT];
  memset(temp, 0, sizeof(temp));
  pagetable_t pagetable = myproc()->pagetable;
  pte_t *pte;
  for (int i = 0; i < npages; va += PGSIZE, i++) {
    pte = walk(pagetable, va, 0);
    if (pte == 0)
      continue;
    if (*pte & PTE_A) {
      temp[i/CHAR_BIT] |= (1 << (i % CHAR_BIT));
    }
    *pte &= ~PTE_A;
  }

  return copyout(pagetable, bufp, temp, sizeof(temp));
}
#endif

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
