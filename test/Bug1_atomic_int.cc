#include <stdio.h>
#include <threads.h>
#include <mutex>
#include <stdatomic.h>

#include "librace.h"

std::mutex *x;
std::mutex *y;
uint32_t shared = 0;

struct Alpha 
{
	atomic_int a;
	int* p;
};
struct Alpha q;

static void a(void *obj)
{
	// int count = 200;
	// while(count != 0)
	// {
	// 	printf("Thread 1\n");
	// 	count--;
	// }
	printf("Thread 1 entering\n");
	printf("the value of a1 is %d\n", atomic_load_explicit(&(q.a), memory_order_relaxed));
	// x->lock();
	if(q.p)
	{
		// int count = 50;
		// while(count != 0)
		// {
		// 	printf("Thread 1 ");
		// 	count--;
		// }
		printf("the value of a2 is %d\n", atomic_load_explicit(&(q.a), memory_order_relaxed));
	}
	// x->unlock();
	printf("Thread 1 leaving\n");
}

static void b(void *obj)
{
	printf("Thread 2 entering\n");
	// int count = 50;
	// while(count != 0)
	// {
	// 	printf("Thread 2 ");
	// 	count--;
	// }
	// x->lock();
	q.p = NULL;
	atomic_store_explicit(&(q.a), 2, memory_order_relaxed);
	printf("the value of a3 is %d\n", atomic_load_explicit(&(q.a), memory_order_relaxed));
	// x->unlock();
	printf("Thread 2 leaving\n");
}

int user_main(int argc, char **argv)
{
	thrd_t t1, t2;
	// q.a = 1;
	atomic_init(&(q.a), 1);
	int k = 2;
	q.p = &k;
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
