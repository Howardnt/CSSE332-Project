#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {

  printf("SPOON TEST:\n");
  uint64 p = 0xdeadbeef;
  spoon((void*)p);

  printf("THREAD_CREATE TEST:\n");
  uint64 t = 0xabcdefab;
  uint64 a = 0xbafedcba;
  thread_create((void*)t, (void*)p, (void*)a, (void*)malloc(4096));

  printf("THREAD_COMBINE TEST:\n");
  thread_combine((void*)t);

  exit(0);
}
