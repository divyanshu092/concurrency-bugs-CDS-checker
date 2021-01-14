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
	atomic_bool io_pending;
};
struct Alpha q;

static void a(void *obj)
{
	// q.io_pending = true;
	// x->lock();
	atomic_int taken;
	std::atomic_init(&taken, 1);
	printf("the value of io_pending1 is %d \n", atomic_load_explicit(&(q.io_pending), memory_order_normal));
	atomic_store_explicit(&(q.io_pending), true, memory_order_normal);
	int count = 10;
	while(atomic_load_explicit(&(q.io_pending), memory_order_normal))
	{
		// cv->notify_all();
		// x->unlock();
		// // atomic_store_explicit(&taken, 0, memory_order_normal);
		// x->lock();
		// if (atomic_load_explicit(&taken, memory_order_normal))
		// {
		// 	// cv->notify_all();
		// 	// x->unlock();
		// 	// atomic_store_explicit(&taken, 0, memory_order_normal);
		// 	// x->lock();
		// }
		count--;
		if(count == 0)
		{
			printf("force leaving an infinite loop\n");
			break;
		}
	}

	printf("the value of io_pending2 is %d \n", atomic_load_explicit(&(q.io_pending), memory_order_normal));
	// x->unlock();
}

static void b(void *obj)
{
	// x->lock();
	printf("the value of io_pending3 is %d \n", atomic_load_explicit(&(q.io_pending), memory_order_normal));
	
	// while(atomic_load_explicit(&(q.io_pending), memory_order_normal) == false)
	// {
	
	// }
	// q.io_pending = false;
	// x->lock();
	// if(atomic_load_explicit(&(q.io_pending), memory_order_normal) == false)
	// {
	// 	cv->wait(*x);
	// 	// ASSERT(atomic_load_explicit(&(q.io_pending), memory_order_normal) == true);
	// }
	// x->unlock();
	// x->lock();
	atomic_store_explicit(&(q.io_pending), false, memory_order_normal);
	// x->unlock();
	// printf("the value of io_pending4 is %d \n", atomic_load_explicit(&(q.io_pending), memory_order_normal));
	// x->unlock();
}

int user_main(int argc, char **argv)
{
	thrd_t t1, t2;
	// q.a = 1;
	atomic_init(&(q.a), 1);
	atomic_init(&(q.io_pending), false);
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
