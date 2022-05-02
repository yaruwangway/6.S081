## Progress

- [x] LEC 1: [Introduction](https://pdos.csail.mit.edu/6.S081/2021/slides/6s081-lec-intro.pdf)
      and [examples](https://pdos.csail.mit.edu/6.S081/2021/lec/l-overview/)
      (handouts: [xv6 book](https://pdos.csail.mit.edu/6.S081/2021/xv6/book-riscv-rev2.pdf); 
      2020: [notes](https://pdos.csail.mit.edu/6.S081/2021/lec/l-overview.txt), 
      [video](https://youtu.be/L6YqHxYHa7A))
  - [x] Preparation: [Read chapter 1](https://pdos.csail.mit.edu/6.S081/2021/xv6/book-riscv-rev2.pdf)
        (for your amusement: [Unix](https://www.youtube.com/watch?v=tc4ROCJYbm0))
  - [x] Assignment: [Lab util: Unix utilities](https://pdos.csail.mit.edu/6.S081/2021/labs/util.html)

- [x] LEC 2: [C](https://pdos.csail.mit.edu/6.S081/2021/lec/6S081-Intro-to-C-Fa21.pdf) and [gdb](https://pdos.csail.mit.edu/6.S081/2021/lec/gdb_slides.pdf)
  - [x] Preparation: 2.9 (Bitwise operators) and 
        5.1 (Pointers and addresses) through 5.6 (Pointer arrays) and 
        6.4 (pointers to structures) by Kernighan and Ritchie (K&R)

- [x] LEC 3: [OS organization and system calls](https://pdos.csail.mit.edu/6.S081/2021/slides/6s081-lec-osorg.pdf) 
      (2020: [notes](https://pdos.csail.mit.edu/6.S081/2021/lec/l-os.txt), 
      [boards](https://pdos.csail.mit.edu/6.S081/2021/lec/l-os-boards.pdf), 
      [video](https://youtu.be/o44d---Dk4o))
  - [x] Preparation: [Read chapter 2](https://pdos.csail.mit.edu/6.S081/2021/xv6/book-riscv-rev2.pdf) 
        and xv6 code: 
        [kernel/proc.h](https://github.com/mit-pdos/xv6-riscv/blob/riscv/kernel/proc.h), 
        [kernel/defs.h](https://github.com/mit-pdos/xv6-riscv/blob/riscv/kernel/defs.h), 
        [kernel/entry.S](https://github.com/mit-pdos/xv6-riscv/blob/riscv/kernel/entry.S), 
        [kernel/main.c](https://github.com/mit-pdos/xv6-riscv/blob/riscv/kernel/main.c), 
        [user/initcode.S](https://github.com/mit-pdos/xv6-riscv/blob/riscv/user/initcode.S), 
        [user/init.c](https://github.com/mit-pdos/xv6-riscv/blob/riscv/user/init.c), 
        and skim [kernel/proc.c](https://github.com/mit-pdos/xv6-riscv/blob/riscv/kernel/proc.c) and 
        [kernel/exec.c](https://github.com/mit-pdos/xv6-riscv/blob/riscv/kernel/exec.c)
  - [x] Assignment: [Lab syscall: System calls](https://pdos.csail.mit.edu/6.S081/2021/labs/syscall.html)

- [x] LEC 4: [Page tables](https://pdos.csail.mit.edu/6.S081/2021/slides/6s081-lec-vm.pdf) 
      (2020: [notes](https://pdos.csail.mit.edu/6.S081/2021/lec/l-vm.txt), 
      [boards](https://pdos.csail.mit.edu/6.S081/2021/lec/l-vm-boards.pdf), 
      [video](https://youtu.be/f1Hpjty3TT8))
  - [x] Preparation: Read [Chapter 3](https://pdos.csail.mit.edu/6.S081/2021/xv6/book-riscv-rev2.pdf) and 
        [kernel/memlayout.h](https://github.com/mit-pdos/xv6-riscv/blob/riscv/kernel/memlayout.h), 
        [kernel/vm.c](https://github.com/mit-pdos/xv6-riscv/blob/riscv/kernel/vm.c), 
        [kernel/kalloc.c](https://github.com/mit-pdos/xv6-riscv/blob/riscv/kernel/kalloc.c), 
        [kernel/riscv.h](https://github.com/mit-pdos/xv6-riscv/blob/riscv/kernel/riscv.h), and 
        [kernel/exec.c](https://github.com/mit-pdos/xv6-riscv/blob/riscv/kernel/exec.c)

- [x] LEC 5: [GDB, calling conventions and stack frames RISC-V](https://pdos.csail.mit.edu/6.S081/2021/lec/gdb_slides.pdf) 
      (2020: [notes](https://pdos.csail.mit.edu/6.S081/2021/lec/l-riscv.txt), 
      [boards](https://pdos.csail.mit.edu/6.S081/2021/lec/l-riscv-slides.pdf), 
      [video](https://youtu.be/s-Z5t_yTyTM))
  - [x] Preparation: Read [Calling Convention](https://pdos.csail.mit.edu/6.S081/2021/readings/riscv-calling.pdf)
  - [x] Assignment: [Lab pgtbl: Page tables](https://pdos.csail.mit.edu/6.S081/2021/labs/pgtbl.html)

- [x] LEC 6: [Isolation & system call entry/exit](https://pdos.csail.mit.edu/6.S081/2021/slides/6s081-lec-syscall.pdf) 
      (2020: [notes](https://pdos.csail.mit.edu/6.S081/2021/lec/l-internal.txt), 
      [video](https://youtu.be/T26UuauaxWA)) 
  - [x] Preparation: Read [Chapter 4, except 4.6](https://pdos.csail.mit.edu/6.S081/2021/xv6/book-riscv-rev2.pdf) and 
        [kernel/riscv.h](https://github.com/mit-pdos/xv6-riscv/blob/riscv/kernel/riscv.h), 
        [kernel/trampoline.S](https://github.com/mit-pdos/xv6-riscv/blob/riscv/kernel/trampoline.S), and 
        [kernel/trap.c](https://github.com/mit-pdos/xv6-riscv/blob/riscv/kernel/trap.c) 

- [x] LEC 7: [Page faults](https://pdos.csail.mit.edu/6.S081/2021/slides/6s081-lec-usingvm.pdf) 
      (2020: [notes](https://pdos.csail.mit.edu/6.S081/2021/lec/l-pgfaults.txt), 
      [boards](https://pdos.csail.mit.edu/6.S081/2021/lec/l-pgfaults.pdf), 
      [video](https://youtu.be/KSYO-gTZo0A)) 
  - [x] Preparation: Read [Section 4.6](https://pdos.csail.mit.edu/6.S081/2021/xv6/book-riscv-rev2.pdf) 
  - [x] Assignment: [Lab traps: Traps](https://pdos.csail.mit.edu/6.S081/2021/labs/traps.html)

- [x] LEC 8: [Q&A labs](https://pdos.csail.mit.edu/6.S081/2021/slides/6s081-lec-qa1.pdf) 

- [x] LEC 9: [Interrupts](https://pdos.csail.mit.edu/6.S081/2021/slides/6s081-lec-interrupts.pdf) 
      (2020: [notes](https://pdos.csail.mit.edu/6.S081/2021/lec/l-interrupt.txt), 
      [boards](https://pdos.csail.mit.edu/6.S081/2021/lec/l-interrupt.pdf), 
      [video](https://youtu.be/zRnGNndcVEA)) 
  - [x] Preparation: Read [Chapter 5](https://pdos.csail.mit.edu/6.S081/2021/xv6/book-riscv-rev2.pdf) and 
        [kernel/kernelvec.S](https://github.com/mit-pdos/xv6-riscv/blob/riscv/kernel/kernelvec.S), 
        [kernel/plic.c](https://github.com/mit-pdos/xv6-riscv/blob/riscv/kernel/plic.c), 
        [kernel/console.c](https://github.com/mit-pdos/xv6-riscv/blob/riscv/kernel/console.c), 
        [kernel/uart.c](https://github.com/mit-pdos/xv6-riscv/blob/riscv/kernel/uart.c), 
        [kernel/printf.c](https://github.com/mit-pdos/xv6-riscv/blob/riscv/kernel/printf.c) 
  - [x] Assignment: [Lab cow: Copy-on-write fork](https://pdos.csail.mit.edu/6.S081/2021/labs/cow.html)

- [x] LEC 10: [Multiprocessors and locking](https://pdos.csail.mit.edu/6.S081/2021/slides/6s081-lec-locks.pdf) 
      (2020: [notes](https://pdos.csail.mit.edu/6.S081/2021/lec/l-lockv2.txt), 
      [boards](https://pdos.csail.mit.edu/6.S081/2021/lec/l-lock.pdf), 
      [video](https://youtu.be/NGXu3vN7yAk)) 
  - [x] Preparation: Read ["Locking"](https://pdos.csail.mit.edu/6.S081/2021/xv6/book-riscv-rev2.pdf) with 
        [kernel/spinlock.h](https://github.com/mit-pdos/xv6-riscv/blob/riscv/kernel/spinlock.h) and 
        [kernel/spinlock.c](https://github.com/mit-pdos/xv6-riscv/blob/riscv/kernel/spinlock.c) 

- [x] LEC 11: [Scheduling 1](https://pdos.csail.mit.edu/6.S081/2021/slides/6s081-lec-threads.pdf) 
      (2020: [notes](https://pdos.csail.mit.edu/6.S081/2021/lec/l-threads.txt), 
      [video](https://youtu.be/vsgrTHY5tkg)) 
  - [x] Preparation: Read ["Scheduling"](https://pdos.csail.mit.edu/6.S081/2021/xv6/book-riscv-rev2.pdf) through Section 7.4, and 
        [kernel/proc.c](https://github.com/mit-pdos/xv6-riscv/blob/riscv/kernel/proc.c), 
        [kernel/swtch.S](https://github.com/mit-pdos/xv6-riscv/blob/riscv/kernel/swtch.S) 

- [x] LEC 12: [Scheduling 2](https://pdos.csail.mit.edu/6.S081/2021/slides/6s081-lec-threads2.pdf) 
      (2020: [notes](https://pdos.csail.mit.edu/6.S081/2021/lec/l-coordination.txt), 
      [code](https://pdos.csail.mit.edu/6.S081/2021/lec/l-coordination.c), 
      [video](https://youtu.be/gP67sJ4PTnc)) 
  - [x] Preparation: Read remainder of ["Scheduling"](https://pdos.csail.mit.edu/6.S081/2021/xv6/book-riscv-rev2.pdf), and corresponding parts of 
        [kernel/proc.c](https://github.com/mit-pdos/xv6-riscv/blob/riscv/kernel/proc.c), 
        [kernel/sleeplock.c](https://github.com/mit-pdos/xv6-riscv/blob/riscv/kernel/sleeplock.c) 
  - [x] Assignment: [Lab thread: Multithreading](https://pdos.csail.mit.edu/6.S081/2021/labs/thread.html)

- [x] LEC 13: [Q&A labs](https://pdos.csail.mit.edu/6.S081/2021/slides/6s081-lec-qa2.pdf) 

- [x] LEC 14: File systems 
      (2020: [notes](https://pdos.csail.mit.edu/6.S081/2021/lec/l-fs.txt), 
      [boards](https://pdos.csail.mit.edu/6.S081/2021/lec/l-fs.pdf), 
      [video](https://youtu.be/ADzLv1nRtR8)) 
  - [x] Preparation: Read [kernel/bio.c](https://github.com/mit-pdos/xv6-riscv/blob/riscv/kernel/bio.c), 
        [kernel/fs.c](https://github.com/mit-pdos/xv6-riscv/blob/riscv/kernel/fs.c), 
        [kernel/sysfile.c](https://github.com/mit-pdos/xv6-riscv/blob/riscv/kernel/sysfile.c), 
        [kernel/file.c](https://github.com/mit-pdos/xv6-riscv/blob/riscv/kernel/file.c) and 
        ["File system" (except for the logging sections)](https://pdos.csail.mit.edu/6.S081/2021/xv6-book-riscv-rev2.pdf) 
  - [x] Assignment: [Lab net: Network driver](https://pdos.csail.mit.edu/6.S081/2021/labs/net.html)

- [x] LEC 15: Crash recovery 
      (2020: [notes](https://pdos.csail.mit.edu/6.S081/2021/lec/l-crash.txt), 
      [boards](https://pdos.csail.mit.edu/6.S081/2021/lec/l-crash.pdf), 
      [video](https://youtu.be/7Hk2dIorDkk)) 
  - [x] Preparation: Read [kernel/log.c](https://github.com/mit-pdos/xv6-riscv/blob/riscv/kernel/log.c) and the logging sections of "File system" 

- [x] LEC 16: File system performance and fast crash recovery 
      (2020: [notes](https://pdos.csail.mit.edu/6.S081/2021/lec/l-journal.txt), 
      [video](https://youtu.be/CmDcf6rjFb4)) 
  - [x] Preparation: Read [Journaling the Linux ext2fs Filesystem (1998)](https://pdos.csail.mit.edu/6.S081/2021/readings/journal-ext2fs.html) 

- [x] LEC 17: Virtual memory for applications 
      (2020: [notes](https://pdos.csail.mit.edu/6.S081/2021/lec/l-uservm.txt), 
      [boards](https://pdos.csail.mit.edu/6.S081/2021/lec/l-uservm.pdf), 
      [video](https://youtu.be/YNQghIvk0jc)) 
      ([baker example](https://pdos.csail.mit.edu/6.S081/2021/lec/baker.c)) 
  - [x] Preparation: Read [Virtual Memory Primitives for User Programs (1991)](https://pdos.csail.mit.edu/6.S081/2021/readings/appel-li.pdf) 

- [ ] LEC 18: OS Organization 
      (2020: [notes](https://pdos.csail.mit.edu/6.S081/2021/lec/l-organization.txt), 
      [video](https://youtu.be/dM9PLdaTpnA)) 
  - [x] Preparation: Read [The Performance of micro-Kernel-Based Systems (1997)](https://pdos.csail.mit.edu/6.S081/2021/readings/microkernel.pdf) 
  - [x] Assignment: [Lab lock: Parallelism/locking](https://pdos.csail.mit.edu/6.S081/2021/labs/lock.html)

- [ ] LEC 19: Virtual Machines 
      (2020: [notes](https://pdos.csail.mit.edu/6.S081/2021/lec/l-vmm.txt), 
      [video](https://youtu.be/R8obXHAIPY0)) 
  - [ ] Preparation: Read [Dune: Safe User-level Access to Privileged CPU Features (2012)](https://pdos.csail.mit.edu/6.S081/2021/readings/belay-dune.pdf) 

- [ ] LEC 20: Kernels and HLL 
      (2020: [notes](https://pdos.csail.mit.edu/6.S081/2021/lec/l-biscuit.txt), 
      [slides](https://pdos.csail.mit.edu/6.S081/2021/lec/l-biscuit-slides.pdf), 
      [video](https://youtu.be/AAtXWGwxI9k)) 
  - [ ] Preparation: Read [the Biscuit paper (2018)](https://pdos.csail.mit.edu/6.S081/2021/readings/biscuit.pdf), 
        [FAQ](https://pdos.csail.mit.edu/6.S081/2021/lec/faq-biscuit.txt) 
  - [ ] Assignment: [Lab fs: File system](https://pdos.csail.mit.edu/6.S081/2021/labs/fs.html)

- [ ] LEC 21: Networking 
      (2020: [notes](https://pdos.csail.mit.edu/6.S081/2021/lec/l-net.txt), 
      [video](https://youtu.be/Fcjychg4Tvk)) 
  - [ ] Preparation: Read [Receive Livelock (1996)](https://pdos.csail.mit.edu/6.S081/2021/readings/mogul96usenix.pdf) 
  - [ ] Assignment: [Lab mmap: Mmap](https://pdos.csail.mit.edu/6.S081/2021/labs/mmap.html)

- [ ] LEC 22: Meltdown 
      (2020: [notes](https://pdos.csail.mit.edu/6.S081/2021/lec/l-meltdown.txt), 
      [video](https://youtu.be/WpKVr3p5rjE)) 
  - [ ] Preparation: Read [Meltdown (2018)](https://pdos.csail.mit.edu/6.S081/2021/readings/meltdown.pdf) 

- [ ] LEC 23: Multi-Core scalability and RCU 
      (2020: [notes](https://pdos.csail.mit.edu/6.S081/2021/lec/l-rcu.txt), 
      [video](https://youtu.be/KUwyCGMTeq8)) 
  - [ ] Preparation: Read [RCU paper (2013)](https://pdos.csail.mit.edu/6.S081/2021/readings/rcu-decade-later.pdf), 
        [FAQ](https://pdos.csail.mit.edu/6.S081/2021/lec/rcu-faq.txt) 

- [ ] LEC 24: Current Research: [Radiation Tolerance](https://pdos.csail.mit.edu/6.S081/2021/lec/CelSkeggsRadiationTolerance.pdf) 
  - [ ] Preparation: Read [NASA CALIPSO paper (2006)](https://pdos.csail.mit.edu/6.S081/2021/readings/nasa-calipso-radiation.pdf) for background 
