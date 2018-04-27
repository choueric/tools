#include <stdlib.h>
#include <stdio.h>

#include "queue.h"


static struct _element *newElement(void *data)
{
	struct _element *elm = (struct _element*)malloc(sizeof(struct _element));
    elm->data = data;
	return elm;
}

Queue *NewQueue()
{
	Queue *q = malloc(sizeof(Queue));
	TAILQ_INIT(q);
	return q;
}

void DestroyQueue(Queue *q)
{
	struct tailqHead *pHead = q;
	struct _element *elm = NULL;

	TAILQ_FOREACH(elm, pHead, _elements) {
		TAILQ_REMOVE(pHead, elm, _elements);
		free(elm);
	}
	free(q);
}

void QueuePushBack(Queue *q, void *data)
{
	struct _element *elm = newElement(data);
    TAILQ_INSERT_TAIL(q, elm, _elements);    
}

void *QueueFront(Queue *q)
{
	struct _element *elm = TAILQ_FIRST(q);
	if (elm == NULL)
		return NULL;
	return elm->data;
}

void QueuePopFront(Queue *q)
{
	struct _element *elm = TAILQ_FIRST(q);
	if (elm == NULL)
		return;

	TAILQ_REMOVE(q, elm, _elements);
	free(elm);
}

void QueuePrint(Queue *q, void (*print)(void *data))
{
	struct _element *elm = NULL;
	printf("queue: ");
	TAILQ_FOREACH(elm, q, _elements) {
		print(elm->data);
	}
	printf("\n");
}
