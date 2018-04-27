#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "thread_queue.h"

static void *feed_thread(void *data)
{
	ThreadQueue *q = (ThreadQueue *)data;
	uint64_t count = 5;

	while (count--) {
		PushThreadQueue(q, (void *)count);
		sleep(1);
	}
	return NULL;
}

int main(int argc, char **argv)
{
	ThreadQueue *q = NewThreadQueue();
	pthread_t t;

	pthread_create(&t, NULL, feed_thread, (void *)q);
	while (1) {
		void *ret = PullThreadQueue(q, 5*1000); // 5 sec
		if (ret == NULL) {
			printf("End\n");
			break;
		}
		printf("pull %lu\n", (uint64_t)ret);
	}

	pthread_join(t, NULL);
	printf("end of test\n");
    return 0;
}
