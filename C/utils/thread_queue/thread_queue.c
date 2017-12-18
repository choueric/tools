#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#include "thread_queue.h"
#include "../queue_demo/queue.h"
#include "../semaphore_cond/semaphore_cond.h"

#define error(fmt, args...) \
	printf("\033[31m!!!\033[0m %s %s() %d: " fmt "\n", \
			__FILE__, __func__, __LINE__, ##args)

struct _ThreadQueue {
	Queue *queue;
	SemaphoreHandle sem;
};

ThreadQueue *NewThreadQueue()
{
	ThreadQueue *q = (ThreadQueue *)malloc(sizeof(ThreadQueue));
	if (q == NULL) {
		error("malloc failed");
		return NULL;
	}

	SemaphoreCreate(&q->sem, 0);

	q->queue = NewQueue();

	return q;
}

void DeleteThreadQueue(ThreadQueue *q)
{
	if (q->sem) {
		SemaphoreSignal(q->sem);
		SemaphoreDestroy(q->sem);
		q->sem = NULL;
	}
	DestroyQueue(q->queue);
	free(q);
}

void PushThreadQueue(ThreadQueue *q, void *p)
{
	QueuePushBack(q->queue, p);
	SemaphoreSignal(q->sem);
}

void *PullThreadQueue(ThreadQueue *q, uint32_t timeout)
{
	int err = SemaphoreWaitTimeout(q->sem, timeout);
	if (err == 0) {
		void *p = QueueFront(q->queue);
		QueuePopFront(q->queue);
		return p;
	}
	if (err != -ETIMEDOUT)
		error("failed: %d.", err);
	return NULL;
}
