#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "semaphore_cond.h"

static int count;

void *consumer(void *data)
{
	SemaphoreHandle sem = (SemaphoreHandle)data;

	printf("consumer: enter\n");
	while (count != 0) {
		printf("consumer: wait for sem ... \n");
		SemaphoreWait(sem);
		printf("consumer: wait end, count = %d\n\n", count);
	}
	printf("consumer: leave\n");

	return NULL;
}

int main(int argc, char **argv)
{
	pthread_t consumer_tid;
	SemaphoreHandle sem;

	SemaphoreCreate(&sem, 0);
	count = 4;

	pthread_create(&consumer_tid, NULL, consumer, sem);

	while (count) {
		sleep(count);
		count--;
		SemaphoreSignal(sem);
		if (count == 1)
			SemaphoreSignal(sem);
	}

	pthread_join(consumer_tid, NULL);

    return 0;
}
