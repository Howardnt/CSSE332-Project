#include "user/schizo_threads.h"

int global = 0;
#define TEST_3_NUM 5
int sthread_create(sthread_t *thread, sthread_fn_in_t fn, void *args, void *stack) {
  int err = thread_create(thread, fn, args, stack); // (syscall) 
  // printf("here\n");
  if (err != 0) {
    printf("error: Encountered error calling thread create %d %d\n", err, *((int *)args));
    return err;
  }
  return 0; // TODO
}

uint64 sthread_join(sthread_t *thread) {
  // TODO check status so we can add retvals as feature
  uint64 result = thread_combine(thread); // (syscall)
  return result;
}

int sthread_exit(void* retval){
    thread_exit(retval);
    return 0;
}


void test_fn_1(void *arg) {
  sleep(1);
  printf("thread_called w/ %d\n", *(int *)arg);
  sthread_exit(0);
  return;
}

int test1() {
  printf("parrot alive\n");
  sthread_t t1;
  int local = 1234;
  void* ptr = malloc(4096);
  sthread_create(&t1, test_fn_1, &local, ptr);
  printf("parrot alive\n");
  sthread_join(&t1);
  free(ptr);

  return 0;
}


void test_fn_3(void *arg) {
  sleep(2 * *((int *)arg));
  printf("(chilled) thread_num: %d pid: %d\n", *((int *)arg), getpid());
  sthread_exit(0);
  exit(0);
  return;
}




// --------- t4
void test_fn_4(void *arg) {
  int add_to_global = *(int *)arg;
  sleep(add_to_global);
  printf("%d\n", global);
  global += add_to_global;
  sthread_exit(0);
  return;
}

int test4() {
  sthread_t t1;
  sthread_t t2;
  int add1 = 13;
  int add2 = 23;
  void* ptr1 = malloc(4096);
  void* ptr2 = malloc(4096);
  sthread_create(&t1, test_fn_4, &add1, ptr1);
  sthread_create(&t2, test_fn_4, &add2, ptr2);
  sthread_join(&t1);
  free(ptr1);
  sthread_join(&t2);
  free(ptr2);
  printf("%d\n", global);
  return 0;
}

int *p = (int *)0xdeadbeef;

void test_fn_5(void *arg) {
    p = (int*)sbrk(4096);

    p[0] = 3;
    p[1] = 2;
    
    printf("thread 1 sees: %p %d %d\n", p, p[0], p[1]);
    sthread_exit(0);
    return;
}

void test_fn_5_2(void *arg){
    if (p == (int*)0xdeadbeef){
	printf("Failed a\n");
	sthread_exit(0);
	return;
    }

    if (p[0] == 3 && p[1] == 2){
	printf("Success\n");
    } else {
	printf("Failed | thread 2 sees: %p %d %d\n", p, p[0], p[1]);
    }

    sthread_exit(0);
    return;
}

int test5(){
    sthread_t t1;
    sthread_t t2;
    
    void* ptr = malloc(4096);
    void* ptr2 = malloc(4096);
    sthread_create(&t1, test_fn_5, 0, ptr);
    sthread_join(&t1);
    free(ptr);
    printf("%p %d %d\n", p, p[0], p[1]);
    sthread_create(&t2, test_fn_5_2, 0, ptr2);
    sthread_join(&t2);
    free(ptr2);
    return 0;
}

