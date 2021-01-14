#include <stdio.h>
#include <threads.h>
#include <mutex>
#include <stdatomic.h>

#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <stdlib.h>
#include <condition_variable>

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
std::condition_variable *cv;

struct Alpha 
{
	atomic_int a;
	int io_pending;
};
struct Alpha q;

static void a(void *obj)
{
	// q.io_pending = true;
	x->lock();
	// int taken = 1;
	printf("the value of io_pending1 is %d \n", q.io_pending);
	store_32(&q.io_pending, 1);
	int count = 5;
	while(q.io_pending)
	{
		cv->notify_all();
		x->unlock();
		// if (taken)
		// {
		// 	cv->notify_all();
		// 	x->unlock();
		// }
		// taken = 0;
		count--;
		if(count == 0)
		{
			printf("force leaving an infinite loop\n");
			break;
		}
	}

	printf("the value of io_pending2 is %d \n", q.io_pending);
	x->unlock();
}

static void b(void *obj)
{
	// x->lock();
	printf("the value of io_pending3 is %d \n", q.io_pending);
	
	// while(atomic_load_explicit(&(q.io_pending), memory_order_relaxed) == false)
	// {
	
	// }

	// q.io_pending = false;
	x->lock();
	if(load_32(&q.io_pending) == 0)
	{
		cv->wait(*x);
		// ASSERT(load_32(&q.io_pending) == 1);		
	}
	int budget = 0;
	while(load_32(&q.io_pending) == 0)
	{
		if(budget == 0)
		{
			printf("the time budget expired\n");
			ASSERT(load_32(&q.io_pending) == 1);
			break;
		}
		budget--;

		cv->wait(*x);
		
	}
	x->unlock();
	// atomic_store_explicit(&(q.io_pending), false, memory_order_relaxed);
	x->lock();
	store_32(&q.io_pending, 0);
	x->unlock();
	printf("the value of io_pending4 is %d \n", q.io_pending);
	
}

int user_main(int argc, char **argv)
{
	thrd_t t1, t2;
	// q.a = 1;
	atomic_init(&(q.a), 1);
	// atomic_init(&(q.io_pending), false);
	q.io_pending = 0;
	x = new std::mutex();
	y = new std::mutex();
	cv = new std::condition_variable();

	printf("Main thread: creating 2 threads\n");
	thrd_create(&t1, (thrd_start_t)&a, NULL);
	thrd_create(&t2, (thrd_start_t)&b, NULL);

	thrd_join(t1);
	thrd_join(t2);
	printf("Main thread is finished\n");

	return 0;
}
