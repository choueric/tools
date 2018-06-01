#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "counter.h"

//#define DEBUG

#define MAX_BASE (~(uint32_t)0)
#define DEF_BASE MAX_BASE
#define DEF_NUM 1

struct counter {
    uint32_t *val;
    uint32_t num;
    uint32_t base;
};

struct counter *alloc_counter(uint32_t base, uint32_t num)
{
    struct counter *c = NULL;

    printf("%u\n", base);

    if (base == 0)
        base = DEF_BASE;
    if (num == 0)
        num = DEF_NUM;

    c = malloc(sizeof(*c));
    if (c == NULL) {
        perror("malloc failed");
        return NULL;
    }
    c->base = base;
    c->num = num;
    c->val = calloc(num, sizeof(uint32_t));
    if (c->val == NULL) {
        perror("malloc failed");
        free(c);
        return NULL;
    }

    printf("-> allocate counter:\n-> base = %u, num = %u, MAX_BASE = %u\n", 
            c->base, c->num, MAX_BASE);

    return c;
}


void free_counter(struct counter *c)
{
    if (c != NULL && c->val != NULL)
        free(c->val);
    if (c != NULL)
        free(c);
}

int inc_counter(struct counter *c)
{
    uint32_t v;
    uint32_t i;
    int delta;

    if (c == NULL || c->val == NULL)
        return -1;

    for (i = 0, delta = 1; i < c->num; i++) {
        if (delta == 0)
            break;
        v = c->val[i] + delta;
        if (v == c->base) {
            delta = 1;
            c->val[i] = 0;
        } else {
            delta = 0;
            c->val[i]++;
        }
    }

    return delta;
}

int dec_counter(struct counter *c)
{
    uint32_t v;
    uint32_t i;
    int delta;

    if (c == NULL || c->val == NULL)
        return -1;

    for (i = 0, v = 0; i < c->num; i++)
        if (c->val[i] == 0)
            v++;
    if (v == c->num) {
        fprintf(stderr, "counter is already zero\n");
        return 1;
    }

    for (i = 0, delta = -1; i < c->num; i++) {
        if (delta == 0)
            break;
        if (c->val[i] == 0) {
            delta = -1;
            c->val[i] = c->base - 1;
        } else {
            delta = 0;
            c->val[i]--;
        }
    }

    return delta;
}

uint32_t *get_counter_cap(struct counter *c, uint32_t *base, uint32_t *num)
{
    if (c == NULL || base == NULL || num == NULL)
        return NULL;

    *base = c->base;
    *num = c->num;
    return c->val;
}

struct counter *realloc_counter(struct counter *c, uint32_t base, uint32_t num)
{
    uint32_t *v = NULL;
    uint32_t n = 0;

    if (c == NULL || c->val == NULL)
        return NULL;

    if (base == 0)
        base = DEF_BASE;
    if (num == 0)
        num = DEF_NUM;

    v = calloc(num, sizeof(uint32_t));
    if (v == NULL) {
        perror("malloc failed");
        return NULL;
    }

    n = c->num > num ? num: c->num;
    memcpy(v, c->val, n * sizeof(uint32_t));

    c->base = base;
    c->num = num;
    free(c->val);
    c->val = v;

    printf("rallocate counter:\n  base = %u, num = %u, MAX_BASE = %u\n", 
            c->base, c->num, MAX_BASE);

    return c;
}


/*
 * base = xxx
 * a3 * base^3 + a2 * base^2 + a1 * base^1 + a0 * base^0
 */
void print_counter(struct counter *c, FILE *stream)
{
    uint32_t i = 0;
    uint32_t n = 0;
    uint32_t j = 0;
    int s = 1;

    if (c == NULL || c->val == NULL)
        return;

    for (i = c->num, n = 0; i > 0; i--) {
        j = i - 1;
        if (c->val[j] == 0) {
            n++;
            continue;
        }

        if (s != 1 && i != c->num) {
            fprintf(stream, " + %u * %u^%u", c->val[j], c->base, j);
        } else {
            fprintf(stream, "%u * %u^%u", c->val[j], c->base, j);
            if (s == 1)
                s = 0;
        }
    }
    if (n == c->num)
        fprintf(stream, "0 * %u^0", c->base);
    fprintf(stream, "\n");

#ifdef DEBUG
    for (i = c->num; i > 0; i--)
        printf("%u ", c->val[i - 1]);
    printf("\n");
#endif
}
