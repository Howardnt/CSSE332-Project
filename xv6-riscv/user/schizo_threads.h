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


void *sthread_join(sthread_t *thread);


int sthread_exit();

#endif // __SCHIZO_THREADS_H
