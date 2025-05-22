#include "user/schizo_threads.c"
#include "user/user.h"

#define SUCCESS 1
#define FAILURE 0

//#define EXPECT(run, result)

void thread_creation(void *arg){
  sleep(10);
  printf("thread %d: I have been created with argument: %d\n", getpid(), *((int *)arg));
  thread_exit(0);
  printf("do I get here?\n");
  return;
}

int thread_creation_test(){
  void* ptr = malloc(4096);
  sthread_t ts;
  uint64 arg = 4;

  printf("Thread is being created with argument: %d\n", arg);
  sthread_create(&ts, thread_creation, &arg, ptr);
  printf("Thread %d is being joined\n", ts);
  sthread_join(&ts);
  free(ptr);
  return SUCCESS;
}

void factorial(void *arg) {
  uint64 num = (uint64)arg;
  num++;
  return;
}

int factorial_test() {
  return 0;
}

typedef int (*test_fn_t)();

test_fn_t tests[] = {thread_creation_test};
int num_tests = sizeof(tests)/sizeof(test_fn_t);

char *line = "----------------------------\n";

void run_test(int n) {
  printf(line);
  printf("Running Test %d\n", n);
  printf(line);

  int result = tests[n]();
  printf("Test %d %s\n", n, result ? "Succeded" : "Failed");
}

int main(char argc, char **argv) {
  if (argc < 2) {
    printf("running all tests: %s\n", argv[1]);
    for (int i = 0; i < num_tests; i++) {
      run_test(i);
    }
  } else {
    printf("running test: %s\n", argv[1]);
    int test_num = atoi(argv[1]);
    if (test_num < 0 || test_num >= num_tests) {
      printf("Error test %d out of range [%d %d]\n", 0, num_tests);
      return 1;
    }
    run_test(test_num);
  }
  return 0;
}



