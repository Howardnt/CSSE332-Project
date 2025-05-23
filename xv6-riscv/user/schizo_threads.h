#ifndef __SCHIZO_THREADS_H
#define __SCHIZO_THREADS_H

#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


typedef int sthread_t;
typedef void(*sthread_fn_in_t)(void *);

int sthread_create(
		sthread_t *thread, 
		sthread_fn_in_t fn,
		void *args, 
		void *stack);

<<<<<<< HEAD
int sthread_create(sthread_t *thread, sthread_fn_in_t fn, void *args, void *stack);
uint64 sthread_join(sthread_t *thread);
int sthread_exit(void* arg);
=======

void *sthread_join(sthread_t *thread);


int sthread_exit();
>>>>>>> ce1c0818ef16896487c77ff85b0d1720a2f750d7

#endif // __SCHIZO_THREADS_H
