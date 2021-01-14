#include <threads.h>

#ifdef WILDCARD
#include "hashmap_wildcard.h"
#else
#include "hashmap.h"
#endif

HashMap *table;

void printKey(Key *key) {
	if (key)
		printf("pos = (%d, %d, %d)\n", key->x, key->y, key->z);
	else
		printf("pos = NULL\n");
}

void printValue(Value *value) {
	if (value)
		printf("velocity = (%d, %d, %d)\n", value->vX, value->vY, value->vZ);
	else
		printf("velocity = NULL\n");
}

// Key(3, 2, 6) & Key(1, 3, 3) are hashed to the same slot -> 4
// Key(1, 1, 1) & Key(3, 2, 2) are hashed to the same slot -> 0
// Key(2, 4, 1) & Key(3, 4, 2) are hashed to the same slot -> 3
// Key(3, 4, 5) & Key(1, 4, 3) are hashed to the same slot -> 5


void threadA(void *arg) {
	Key *k1 = new Key(3, 2, 6);
	Key *k2 = new Key(1, 1, 1);
	Value *v1 = new Value(10, 10, 10);
	Value *r1 = table->put(k1, v1);
	//printValue(r1);
	Value *r2 = table->get(k2);
	//printf("Thrd A:\n");
	printValue(r2);
}

void threadB(void *arg) {
	Key *k1 = new Key(3, 2, 6);
	Key *k2 = new Key(1, 1, 1);
	Value *v2 = new Value(30, 40, 50);
	Value *r3 = table->put(k2, v2);
	//printValue(r3);
	Value *r4 = table->get(k1);
	printf("Thrd B:\n");
	printValue(r4);
}

int user_main(int argc, char *argv[]) {
	thrd_t t1, t2;
	table = new HashMap;

	thrd_create(&t1, threadA, NULL);
	thrd_create(&t2, threadB, NULL);
	thrd_join(t1);
	thrd_join(t2);
	
	return 0;
}


