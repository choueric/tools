#ifndef _COUNTER_H
#define _COUNTER_H

#include <stdint.h>

struct counter;

struct counter *alloc_counter(uint32_t base, uint32_t num);
void free_counter(struct counter *c);

int inc_counter(struct counter *c);
int dec_counter(struct counter *c);

uint32_t *get_counter_cap(struct counter *c, uint32_t *base, uint32_t *num);
struct counter *realloc_counter(struct counter *c, uint32_t base, uint32_t num);

void print_counter(struct counter *c, FILE *stream);

#endif
