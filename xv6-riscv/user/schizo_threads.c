#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

typedef int sthread_t;
typedef void(*sthread_fn_in_t)(void *);




int sthread_create(sthread_t *thread, sthread_fn_in_t fn, void *args) {
  int err = thread_create(thread, fn, args); // (syscall) 
  // printf("here\n");
  if (err != 0) {
    printf("error: Encountered error calling thread create %d %d\n", err, *((int *)args));
    return err;
  }
  return 0; // TODO
}

int sthread_join(sthread_t *thread) {
  // TODO check status so we can add retvals as feature
  int err = thread_combine(thread); // (syscall)
  if (err != 0){
    printf("error: Encountered error calling thread combine %d\n", err);
    return err;
  }
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
  exit(0);
}

int test2() {
  sthread_t t1;
  sthread_t t2;
  int id1 = 3;
  int id2 = 5;
  sthread_create(&t1, test_fn_2, &id1);
  sthread_create(&t2, test_fn_2, &id2);
  return 0;
}

void test_fn_3(void *arg) {
  sleep(2 * *((int *)arg));
  printf("(chilled) thread_num: %d pid: %d\n", *((int *)arg), getpid());
  exit(0);
}


#define TEST_3_CNT 10
int test3() {
  sthread_t ts[TEST_3_CNT];
  int ids[TEST_3_CNT];
  for (int i = 0; i < TEST_3_CNT; i++) {
    ids[i] = i+1;
    sthread_create(&ts[i], test_fn_3, &ids[i]);
    printf("(parrot) thread_num %d, sthread_t %d (should match that threads pid)\n", ids[i], (int)ts[i]);
  }
  sleep(100);
  return 0;
}

// --------- t4
int global = 0;
void test_fn_4(void *arg) {
  int add_to_global = *(int *)arg;
  sleep(add_to_global);
  printf("%d\n", global);
  global += add_to_global;
  exit(0);
}

int test4() {
  sthread_t t1;
  sthread_t t2;
  int add1 = 13;
  int add2 = 23;
  sthread_create(&t1, test_fn_4, &add1);
  sthread_create(&t2, test_fn_4, &add2);
  sleep(100);
  printf("%d\n", global);
  return 0;
}

void test_fn_5(void *arg){
//    printf("started\n");
    sleep(40);
//    printf("ended\n");
    exit(1);
}

int test5() {
    sthread_t t1;
    sthread_create(&t1, test_fn_5, 0);
    printf("got here\n");
    while(1);
    sthread_join(&t1);
    printf("done\n");
    return 0;
}

int main() {
  test3();
  return 0; // dummy main
}
