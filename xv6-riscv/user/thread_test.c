#include "user/schizo_threads.c"
#include "user/user.h"

#define SUCCESS 1
#define FAILURE 0

//#define EXPECT(run, result)

void thread_creation(void *arg){
  sleep(10);
  printf("(thread %d) I have been created with argument: %d\n", getpid(), *((int *)arg));
  thread_exit(0);
  printf("do I get here?\n");
  return;
}

int thread_creation_test(){
  void* ptr = malloc(4096);
  sthread_t ts;
  uint64 arg = 100;

  printf("(parent) Thread is being created with argument: %d\n", arg);
  sthread_create(&ts, thread_creation, &arg, ptr);
  sleep(11);
  printf("(parent) Thread %d is being joined\n", ts);
  sthread_join(&ts);
  free(ptr);
  return SUCCESS;
}

void multi_thread_creation(void *arg) {
  sleep(2);
  printf("(thread %d) has been created with argument: %d\n", getpid(), *((int *)arg));
  sthread_exit(0);
  return;
}


int m_thread_creation_test() {
  sthread_t ts[TEST_3_NUM];
  int ids[TEST_3_NUM];
  void* ptrs[TEST_3_NUM];

  for (int i = 0; i < TEST_3_NUM; i++) {
    ids[i] = i+1;
    ptrs[i] = malloc(4096);
    sthread_create(&ts[i], multi_thread_creation, &ids[i], ptrs[i]);
    printf("(parent) thread_num %d, sthread_t pid %d\n", ids[i], (int)ts[i]);
    sleep(5);
  }

  for (int i = 0; i < TEST_3_NUM; i++) {
    printf("(parent) joining thread %d\n", (int)ts[i]);
    sthread_join(&ts[i]);
    free(ptrs[i]);
  }
  return SUCCESS;
}

void shared_memory(void *arg) {
  int add_to_global = *(int *)arg;
  sleep(2);
  printf("(thread %d) adding %d to global (currently %d)\n", getpid(), add_to_global, global);
  global += add_to_global;
  sthread_exit(0);
  return;
}

int shared_memory_test() {
  sthread_t t1;
  sthread_t t2;
  int add1 = 10;
  int add2 = 20;
  void* ptr1 = malloc(4096);
  void* ptr2 = malloc(4096);
  printf("(parent) global is starting at: %d\n", global);
  sthread_create(&t1, shared_memory, &add1, ptr1);
  sleep(5);
  sthread_create(&t2, shared_memory, &add2, ptr2);
  sleep(5);
  sthread_join(&t1);
  free(ptr1);
  sthread_join(&t2);
  free(ptr2);
  printf("(parent) global is now: %d\n", global);
  return SUCCESS;
}

void propagation_update(void *arg) {
    printf("(thread %d) allocating p using sbrk\n", getpid());
    p = (int*)sbrk(4096);

    printf("(thread %d) setting p[0] to 3\n", getpid());
    p[0] = 3;
    printf("(thread %d) setting p[1] to 2\n", getpid());
    p[1] = 2;
    
    printf("(thread %d) sees p:%p p[0]:%d p[1]:%d\n", getpid(), p, p[0], p[1]);
    sthread_exit(0);
    return;
}

void propagation_update_2(void *arg){
    if (p == (int*)0xdeadbeef){
	printf("(thread %d) Failed\n", getpid());
	sthread_exit(0);
	return;
    }

    printf("(thread %d) sees p:%p p[0]:%d p[1]:%d\n", getpid(), p, p[0], p[1]);
    sthread_exit(0);
    return;
}

int propagation_update_test(){
    sthread_t t1;
    sthread_t t2;
    
    void* ptr = malloc(4096);
    void* ptr2 = malloc(4096);
    sthread_create(&t1, propagation_update, 0, ptr);
    sthread_join(&t1);
    free(ptr);
    sthread_create(&t2, propagation_update_2, 0, ptr2);
    sthread_join(&t2);
    free(ptr2);
    return SUCCESS;
}

void factorial(void *arg) {
  uint64 num = *(uint64 *)arg;
  printf("thread_num: %d arg: %d\n", getpid(), num);
  if (num == 0) {
    thread_exit((void *)1);
    return;
  } else {
    uint64 smaller = num - 1;
    sthread_t child;
    void *stack = malloc(4096);
    sthread_create(&child, factorial, &smaller, stack);
    uint64 got = (uint64)sthread_join(&child);
    free(stack);
    uint64 res = (got*num);
    printf("%d! = %d\n", num, res);
    thread_exit((void *)res);
    return;
  }
}

int factorial_test() {
  sthread_t top;
  void *stack = malloc(4096);
  uint64 arg = 7;
  sthread_create(&top, factorial, &arg, stack);
  int got = sthread_join(&top);
  free(stack);
  return got == 5040;
}

typedef int (*test_fn_t)();

test_fn_t tests[] = {thread_creation_test, m_thread_creation_test, shared_memory_test, propagation_update_test, factorial_test};
int num_tests = sizeof(tests)/sizeof(test_fn_t);

char *line = "----------------------------\n";

void run_test(int n) {
  printf(line);
  printf("Running Test %d\n", n);
  printf(line);
  int result = tests[n]();
  printf("Test %d %s\n", n, result ? "Succeeded" : "Failed");
}

int main(char argc, char **argv) {
  if (argc < 2) {
    printf("Running All Tests\n");
    for (int i = 0; i < num_tests; i++) {
      run_test(i);
    }
  } else {
    printf("Running Test: %s\n", argv[1]);
    int test_num = atoi(argv[1]);
    if (test_num < 0 || test_num >= num_tests) {
      printf("Error test %d out of range [%d %d)\n", test_num, 0, num_tests);
      return 1;
    }
    run_test(test_num);
  }
  return 0;
}



