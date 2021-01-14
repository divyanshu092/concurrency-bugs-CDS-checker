#include <stdio.h>
#include <threads.h>
#include <mutex>
#include <stdatomic.h>

#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <stdlib.h>

#include "model.h"
#include "action.h"
#include "clockvector.h"
#include "common.h"
#include "threads-model.h"
#include "nodestack.h"
#include "wildcard.h"

#include "librace.h"

std::mutex *x;
std::mutex *y;
uint32_t shared = 0;

struct Alpha 
{
	atomic_int a;
	atomic_int* p;
};
struct Alpha q;

static void a(void *obj)
{
	printf("the value of p1 is %ld \n", atomic_load_explicit((atomic_long*)(&(q.p)), memory_order_relaxed));
	x->lock();
	if(atomic_load_explicit((atomic_long*)(&(q.p)), memory_order_relaxed))
	{
		// int count = 50;
		// while(count != 0)
		// {
		// 	printf("Thread 1 ");
		// 	count--;
		// }
		// ASSERT((long long)(*(atomic_long*)(&(q.p))) != 0);
		ASSERT(atomic_load_explicit((atomic_long*)(&(q.p)), memory_order_relaxed) != 0);
		printf("the value of p2 is %ld \n", atomic_load_explicit((atomic_long*)(&(q.p)), memory_order_relaxed));
	}
	x->unlock();
}

static void b(void *obj)
{
	// int count = 50;
	// while(count != 0)
	// {
	// 	printf("Thread 2 ");
	// 	count--;
	// }
	x->lock();
	// q.p = NULL;
	atomic_store_explicit((atomic_long*)(&(q.p)), 0, memory_order_relaxed);
	printf("the value of p3 is %ld \n", atomic_load_explicit((atomic_long*)(&(q.p)), memory_order_relaxed));
	x->unlock();
}

int user_main(int argc, char **argv)
{
	thrd_t t1, t2;
	// q.a = 1;
	atomic_init(&(q.a), 1);
	int k = 2;
	atomic_init((q.p), 1);
	atomic_store_explicit((atomic_long*)(&(q.p)), 2, memory_order_seq_cst);
	// printf("the value of p is %l \n", atomic_load_explicit((atomic_long*)(&(q.p)), memory_order_seq_cst));
	// q.p = &k;
	// atomic_init((q.p), k);
	x = new std::mutex();
	y = new std::mutex();

	printf("Main thread: creating 2 threads\n");
	thrd_create(&t1, (thrd_start_t)&a, NULL);
	thrd_create(&t2, (thrd_start_t)&b, NULL);

	thrd_join(t1);
	thrd_join(t2);
	printf("Main thread is finished\n");

	return 0;
}
