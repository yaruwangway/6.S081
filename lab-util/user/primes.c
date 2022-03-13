#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define INT_SIZE sizeof(int)
#define PRIME_END 35

void sieve(int *);

int main(int argc, char *argv[]) {
  int i;
  int pid;
  int pipeline[2];

  pipe(pipeline);

  pid = fork();
  if (pid == 0) {
    sieve(pipeline);
  } else {
    for (i = 2; i <= PRIME_END; i++) {
      write(pipeline[1], &i, INT_SIZE);
    }
    close(pipeline[1]);
    wait((int *)0);
  }

  exit(0);
}

void sieve(int *from_pipeline) {
  int p, n, k;
  int pid;
  int to_pipeline[2];

  /**
   * IMPORTANT!!!
   *
   * Quota from book-riscv-rev2, page 16:
   * > After fork, both parent and child have file descriptors referring to the pipe.
   *
   * > If no data is available, a read on a pipe waits for either data to be written
   * > or for all file descriptors referring to the write end to be closed;
   * > in the latter case, read will return 0, just as if the end of a data file had been reached.
   * > The fact that read blocks until it is impossible for new data to arrive is one reason
   * > that it’s important for the child to close the write end of the pipe before executing wc above:
   * > if one of wc ’s file descriptors referred to the write end of the pipe, wc would never see end-of-file.
   */
  close(from_pipeline[1]);

  if ((k = read(from_pipeline[0], &p, INT_SIZE)) == 0) {
    close(from_pipeline[0]);
    exit(0);
  }

  fprintf(1, "prime %d\n", p);

  pipe(to_pipeline);
  pid = fork();
  if (pid == 0) {
    sieve(to_pipeline);
  } else {
    while ((k = read(from_pipeline[0], &n, INT_SIZE)) != 0) {
      if (n % p) {
        write(to_pipeline[1], &n, INT_SIZE);
      }
    }
    close(from_pipeline[0]);
    close(to_pipeline[1]);
    wait((int *)0);
  }
  exit(0);
}
