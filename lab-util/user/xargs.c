#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

#define MAX_LINE_LEN 512

int readline(char *buf) {
  char *p;
  int n;

  p = buf;
  while ((n = read(0, p, 1)) != 0) {
    if (*p == '\n') {
      *p = 0;
      return 0;
    }
    p++;
    if (p >= buf + MAX_LINE_LEN) {
      fprintf(2, "line too long\n");
      exit(1);
    }
  }

  *p = 0;
  return 1;
}

int main(int argc, char *argv[]) {
  char line[MAX_LINE_LEN];
  int read_end;
  int pid;
  char *args[MAXARG];
  int i;

  for (i = 1; i < argc; i++) {
    args[i-1] = argv[i];
  }

  do {
    read_end = readline(line);
    if (line[0] == 0) {
      continue;
    }

    pid = fork();
    if (pid == 0) {
      args[argc-1] = line;
      exec(argv[1], args);
      fprintf(2, "exec: %s failed\n", argv[1]);
      exit(1);
    } else {
      wait(0);
    }
  } while (read_end != 1);

  exit(0);
}
