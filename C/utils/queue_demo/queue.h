#pragma once

#include<sys/queue.h>

struct _element {
	void *data;
	TAILQ_ENTRY(_element) _elements;
};

TAILQ_HEAD(tailqHead, _element);
typedef struct tailqHead Queue;

Queue *NewQueue();
void DestroyQueue(Queue *q);

void QueuePushBack(Queue *q, void *data);
void *QueueFront(Queue *q);
void QueuePopFront(Queue *q);

void QueuePrint(Queue *q, void (*print)(void *data));
