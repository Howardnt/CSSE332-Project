#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

typedef int sthread_t;
typedef void(*sthread_fn_in_t)(void *);




int sthread_create(sthread_t *thread, sthread_fn_in_t fn, void *args) {
  int err = thread_create(thread, fn, args); // (syscall) 
  if (err != 0) {
    printf("error: Encountered error calling thread create %d\n", err);
    return err;
  }
  return 1; // TODO
}

int sthread_join(sthread_t *thread) {
// TODO note for next time
// waitpid needed for joining
//
// need to check status for errors to return

//  waitpid(*(thread->ts), );
  return 0;
}

void test_fn_1(void *no_arg) {
  sleep(1);
  printf("thread_called\n");
  exit(1);
//  return;
}

int test1() {
  printf("parrot alive\n");
  printf("child should exec at %p\n", test_fn_1);
  sthread_t t1;
  sthread_create(&t1, test_fn_1, 0);
  printf("parrot alive\n");
  return 0;
}

int main() {
  test1();
  return 0; // dummy main
}
