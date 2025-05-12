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

void test_fn_1(void *arg) {
  sleep(1);
  printf("thread_called w/ %d\n", *(int *)arg);
  exit(1);
//  return;
}

int test1() {
  printf("parrot alive\n");
  sthread_t t1;
  int local = 1234;
  sthread_create(&t1, test_fn_1, &local);
  printf("parrot alive\n");
  return 0;
}

void test_fn_2(void *arg) {
  int a = 10*(*(int*)arg);
  sleep(a);
  printf("thread %d ended\n", a);
  sleep(100-a);
  exit(0);
}

int test2() {
  sthread_t t1;
  sthread_t t2;
  int id1 = 3;
  int id2 = 5;
  sthread_create(&t1, test_fn_2, &id1);
  sleep(0.5);
  sthread_create(&t2, test_fn_2, &id2);
  return 0;
}

int main() {
  test2();
  return 0; // dummy main
}
