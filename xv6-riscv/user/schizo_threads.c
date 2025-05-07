#include "user/user.h"

typedef struct {

} sthread_t;

typedef void * (*generic_fn_ptr_t)(void *);


int sthread_create(sthread_t *thread, generic_fn_ptr_t fn, void *arg) {
  return 1; // TODO
}

int sthread_join(sthread_t *thread) {
  return 0;
}

int main() {
  return 0; // dummy main
}
