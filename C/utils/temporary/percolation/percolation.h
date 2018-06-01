#ifndef _PERCOLATION_H
#define _PERCOLATION_H

/* should be same as enum rect_type_t in xui.h */
/* BLOCK and open(FULL or EMPTY) */
enum rect_state_t {
	STATE_BLOCK,
	STATE_FULL,
	STATE_EMPTY,
};

struct rect_t {
	enum rect_state_t state;
};

struct rect_table_t {
	struct rect_t *prect;
	int num;
	int maxnum;
};

struct rect_table_t *PercolationInit(int n);
int TickTack(void *data);
int PercolationClose(struct rect_table_t *ptable);

#endif
