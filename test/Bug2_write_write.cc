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
	bool io_pending;
	atomic_int* p;
};
struct Alpha q;

static void a(void *obj)
{
	// x->lock();
	
	q.io_pending = true;
	int count = 10000;
	while(q.io_pending)
	{
		// printf("inside the while loop\n");
		count--;
		if(count == 0)
		{
			printf("force leaving an infinite loop\n");
			break;
		}
	}
	// x->unlock();
}

static void b(void *obj)
{
	// x->lock();
	while(q.io_pending == false)
	{
		printf("inside the second while loop\n");
	}
	int count = 1000;
	while(count > 0)
	{
		count--;
	}

	q.io_pending = false;
	// x->unlock();
}

int user_main(int argc, char **argv)
{
	thrd_t t1, t2;
	// q.a = 1;
	// atomic_init(&(q.a), 1);
	// int k = 2;
	// atomic_init((q.p), 1);
	// atomic_store_explicit((atomic_long*)(&(q.p)), 2, memory_order_seq_cst);
	q.io_pending = false;
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
