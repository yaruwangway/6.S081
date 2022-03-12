#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  int pid, n;
  char buf[100];
  int parent2child_fds[2];
  int child2parent_fds[2];

  pipe(parent2child_fds);
  pipe(child2parent_fds);

  pid = fork();
  if (pid == 0) {
    n = read(parent2child_fds[0], buf, sizeof(buf));
    pid = getpid();
    fprintf(1, "%d: received ping\n", pid);

    write(child2parent_fds[1], buf, n);
  } else {
    write(parent2child_fds[1], "p", 1);

    n = read(child2parent_fds[0], buf, sizeof(buf));
    pid = getpid();
    fprintf(1, "%d: received pong\n", pid);
  }

  exit(0);
}
