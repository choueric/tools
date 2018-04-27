#include<stdio.h>
#include<stdlib.h>
#include<sys/queue.h>

#include "queue.h"

/*
 * see 'man queue'
 */

struct element{
    int num;
    /*LIST_ENTRY 创建了一个匿名结构体，这个结构体有两个成员，
      分别为struct element* le_next;
           struct element** le_prev;
      即，elements的类型就是这个匿名的结构体，
      使得element成为双向链表中的节点
     */
    LIST_ENTRY(element) elements;
};

struct elementq{
    int num;
    TAILQ_ENTRY(elementq) elements;
};

static struct element *newElement(int num)
{
	struct element *p = (struct element*)malloc(sizeof(struct element));
    p->num = num;
	return p;
}

static struct elementq *newElementq(int num)
{
	struct elementq *p = (struct elementq*)malloc(sizeof(struct elementq));
    p->num = num;
	return p;
}

void test_list()
{
    struct element *n1, *np;

    LIST_HEAD(listhead, element) head; 
	struct listhead *pHead = &head;

    LIST_INIT(pHead);

	n1 = newElement(1);
    LIST_INSERT_HEAD(pHead, n1, elements);

	n1 = newElement(2);
    LIST_INSERT_AFTER(LIST_FIRST(pHead), n1, elements);    
    
	n1 = newElement(3);
    LIST_INSERT_BEFORE(LIST_FIRST(pHead), n1, elements);
    
    //遍历链表
    LIST_FOREACH(np, pHead, elements){
        printf("%d ",np->num);
    }
	printf("\n");
    
    //删除，释放链表
	LIST_FOREACH(np, pHead, elements) {
        printf("remove %d\n",np->num);
		LIST_REMOVE(np, elements);
		free(np);
	}
}

void test_tailq()
{
	TAILQ_HEAD(tailqHead, elementq) head;
	struct tailqHead *pHead = &head;

	TAILQ_INIT(pHead);

	struct elementq *pn = newElementq(1);
	TAILQ_INSERT_HEAD(pHead, pn, elements);

	pn = newElementq(2);
	TAILQ_INSERT_TAIL(pHead, pn, elements);

	pn = newElementq(3);
	TAILQ_INSERT_TAIL(pHead, pn, elements);

	TAILQ_FOREACH(pn, pHead, elements) {
		printf("%d ", pn->num);
	}
	printf("\n");

	TAILQ_FOREACH(pn, pHead, elements) {
		printf("remove %d\n", pn->num);
		TAILQ_REMOVE(pHead, pn, elements);
		free(pn);
	}
}

static void qprint(void *data)
{
	printf("%p ", data);
}

void test_queue()
{
	Queue *q = NewQueue();

	QueuePushBack(q, (void *)1);
	QueuePushBack(q, (void *)2);
	QueuePushBack(q, (void *)3);
	QueuePrint(q, qprint);

	void *data = QueueFront(q);
	printf("%p\n", data);

	QueuePopFront(q);
	QueuePrint(q, qprint);

	data = QueueFront(q);
	printf("%p\n", data);

	QueuePopFront(q);
	QueuePopFront(q);
	QueuePrint(q, qprint);

	QueuePopFront(q);
	QueuePrint(q, qprint);

	DestroyQueue(q);
}

int main(int argc, char **argv)
{
	test_queue();
    return 0;
}
