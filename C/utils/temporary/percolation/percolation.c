#include "percolation.h"
#include "xui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define is_in_table(x, y, n) ((x) >= 0 && (x) < (n) && (y) >= 0 && (y) < (n))
#define xy_to_index(x, y, n) ((y) * (n) + (x))

static int get_rand(int maxnum);
inline static void index_to_xy(int index, int *px, int *py, int n);
static int is_rect_opened(struct rect_t *prect, int index);
static int is_percolated(struct rect_table_t *ptable);
static int open_rect(struct rect_t *prect, int num, int index);
static int check_and_set_full(struct rect_t *prect, int num, int index);
static int is_rect_full(struct rect_t *prect, int num, int x, int y);
static int set_full(struct rect_t *prect, int num, int x, int y);
static float calculate(struct rect_table_t *ptable);

/* this may be not so random */
static int get_rand(int maxnum)
{
	struct timeval tv;
	unsigned int seed;

	gettimeofday(&tv, NULL);
	//seed = (tv.tv_sec % 100000) * 1000000 + tv.tv_usec;
	seed = tv.tv_sec * 1000000 + tv.tv_usec;
	srand(seed);
	return (rand() % maxnum);
}

/* calculate the final result */
static float calculate(struct rect_table_t *ptable)
{
	int i;
	int nonblock = 0;
	int maxnum = ptable->maxnum;
	float ret;

	for (i = 0; i < maxnum; i++)
		if (ptable->prect[i].state != STATE_BLOCK)
			nonblock++;

	ret = (float)nonblock / (float)maxnum;
	printf("%d/%d = %f\n", nonblock, maxnum, ret);

	return ret;
}

/*
 *  +------> x
 *  |
 *  |
 *  ? y
 */
inline static void index_to_xy(int index, int *px, int *py, int n)
{
	*px = index % n;
	*py = index / n;
}

static int is_rect_opened(struct rect_t *prect, int index)
{
	return (prect[index].state != STATE_BLOCK ? 1: 0);
}

static int is_percolated(struct rect_table_t *ptable)
{
	int i;
	int ret = 0;
	struct rect_t *pr;

	for (i = 0; i < ptable->num; i++) {
		pr = ptable->prect + ptable->maxnum - 1 - i;
		if (pr->state == STATE_FULL) {
			ret = 1;
			break;
		}
	}
	return ret;
}

static int open_rect(struct rect_t *prect, int num, int index)
{
	int x ,y;

	index_to_xy(index, &x, &y, num);
	prect[index].state = STATE_EMPTY;
	XuiDrawRect(x, y, TYPE_EMPTY);    // ui update

	return 0;
}

/* check if rect state is full */
static int is_rect_full(struct rect_t *prect, int num, int x, int y)
{
	int index;

	if (!is_in_table(x, y, num))
		return 0;

	index = xy_to_index(x, y, num);

	return (prect[index].state == STATE_FULL);
}

/* check if the rect could turn to be full.
 * if so, then set itself and surrounding rects to be full
 */
static int check_and_set_full(struct rect_t *prect, int num, int index)
{
	int x, y;

	index_to_xy(index, &x, &y, num);
	if (y == 0 || is_rect_full(prect, num, x - 1, y)
			|| is_rect_full(prect, num, x + 1, y)
			|| is_rect_full(prect, num, x, y - 1)
			|| is_rect_full(prect, num, x, y + 1)) {
		set_full(prect, num, x, y);
	}

	return 0;
}

/* set rect itself and its surrounding rects to be full.
 * this function is recursive.
 */
static int set_full(struct rect_t *prect, int num, int x, int y)
{
	int index = 0;
	
	if (!is_in_table(x, y, num))
		return 0;

	index = xy_to_index(x, y, num);
	if (prect[index].state != STATE_EMPTY)
		return 0;

	prect[index].state = STATE_FULL;
	XuiDrawRect(x, y, TYPE_FULL);     // update ui

	set_full(prect, num, x - 1, y);
	set_full(prect, num, x + 1, y);
	set_full(prect, num, x, y - 1);
	set_full(prect, num, x, y + 1);

	return 0;
}

/**********************************************************************/

struct rect_table_t *PercolationInit(int n)
{
	struct rect_t *prect = NULL;
	struct rect_table_t *ptable = NULL;

	ptable = malloc(sizeof(struct rect_table_t));
	if (ptable == NULL) {
		printf("malloc failed\n");
		exit(1);
	}
	ptable->num = n;
	ptable->maxnum = n * n;

	prect = malloc(ptable->maxnum * sizeof(struct rect_t));
	if (prect == NULL) {
		printf("malloc failed\n");
		exit(1);
	}
	memset(prect, 0, ptable->maxnum * sizeof(struct rect_t));
	ptable->prect = prect;

	return ptable;
}

int TickTack(void *data)
{
	int index;
	struct rect_table_t *ptable = (struct rect_table_t *)data;

	while (1) {
		index = get_rand(ptable->maxnum);
		if (is_rect_opened(ptable->prect, index))
			continue;
		else
			break;
	}

	open_rect(ptable->prect, ptable->num, index);
	check_and_set_full(ptable->prect, ptable->num, index);

	if (is_percolated(ptable)) {
		printf("----> perlocated <-----------\n");
		calculate(ptable);
		return 1;
	}

	return 0;
}

int PercolationClose(struct rect_table_t *ptable)
{
	free(ptable->prect);
	free(ptable);

	return 0;
}
