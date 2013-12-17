#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

#include "item.h"

#ifdef DEBUG
static int debug = 1;
#else
static int debug = 0;
#endif


void print_item(struct item *p)
{
    if (!debug)
        return;
    printf("%s | %s | %s | %s\n", p->word, p->cog, p->sim, p->syn);
}

static int cmp_item(struct item *p1, struct item *p2)
{
    assert(p1 != NULL && p2 != NULL);

    return strcmp(p1->word, p2->word);
}


struct item *do_make_item(char *word, char *cog, char *sim, char *syn)
{
    struct item *pitem = NULL;

    pitem = malloc(sizeof(struct item));
    if (pitem == NULL) {
        perror("malloc failed");
        return NULL;
    }
    pitem->word = strdup(word);
    pitem->cog = strdup(cog);
    pitem->sim = strdup(sim);
    pitem->syn = strdup(syn);

    return pitem;
}

struct item *make_item(char *word, char *cog, char *sim, char *syn)
{
    return do_make_item(word, cog, sim, syn);
}

struct item *parse_item(char *line)
{
    char *p = NULL;
    char *s = line;
    struct item *pitem = NULL;
    char *word = NULL, *cog = NULL, *sim = NULL, *syn = NULL;
    int i;

    assert(line != NULL);

    for (i = 0, s = line; i < 4; i++) {
        p = strstr(s, DELIM);
        if (p == NULL) {
            return NULL;
        }
        *p = '\0';
        if (i == 0)
            word = s;
        else if (i == 1)
            cog = s;
        else if (i == 2)
            sim = s;
        else if (i == 3)
            syn = s;
        s = p + 2;
    }
    pitem = do_make_item(word, cog, sim, syn);
    print_item(pitem);

    return pitem;
}

static int prepend(struct item *p, struct item *head, struct head *phead)
{
    struct item *tmp;

    if (phead->head == head) {
        phead->head = p;
        p->next = head;
        p->pre = NULL;
        head->pre = p;
    } else {
        tmp = head->pre;
        tmp->next = p;
        p->next = head;
        p->pre = tmp;
        head->pre = p;
    }
    phead->num++;
    return phead->num;
}

static int append(struct item *p, struct item *tail, struct head *phead)
{
    struct item *tmp;

    if (phead->tail == tail) {
        phead->tail = p;
        p->pre = tail;
        p->next = NULL;
        tail->next = p;
    } else {
        tmp = tail->next;
        tmp->pre = p;
        tail->next = p;
        p->pre = tail;
        p->next = tmp;
    }
    phead->num++;
    return phead->num;
}

static struct item *get_middle(struct item *head, struct item *tail)
{
    int i, n;
    struct item *p = NULL;

    for (i = 0, p = head; p != tail; p = p->next)
        i++;

    n = i / 2;
    for (i = 0, p = head; i < n; p = p->next)
        i++;

    return p;
}

int insert_item(struct head *phead, struct item *pitem,
        struct item *head, struct item *tail)
{
    struct item *middle;

    if (head == NULL) {
        return add_item(pitem, phead);
    }

    if (cmp_item(pitem, head) < 0) {
        if (debug)
            printf("insert 1\n");
        prepend(pitem, head, phead);
    } else if (cmp_item(pitem, tail) > 0) {
        if (debug)
            printf("insert 2\n");
        append(pitem, tail, phead);
    } else {
        middle = get_middle(head, tail);
        if (debug)
            printf("insert 3\n");
        if (middle == head) {
            if (debug)
                printf("insert 3.1\n");
            append(pitem, head, phead);
        } else if (cmp_item(pitem, middle) < 0) {
            if (debug)
                printf("insert 3.2\n");
            insert_item(phead, pitem, head, middle);
        } else {
            if (debug)
                printf("insert 3.3\n");
            insert_item(phead, pitem, middle, tail);
        }
    }

    return 0;
}

int add_item(struct item *pitem, struct head *phead)
{
    assert(pitem != NULL && phead != NULL);

    if (phead->tail == NULL) {
        phead->tail = pitem;
        phead->head = pitem;
        pitem->pre = NULL;
        pitem->next = NULL;
    } else {
        pitem->pre = phead->tail;
        phead->tail = pitem;
        pitem->next = NULL;
        pitem->pre->next = pitem;
    }
    phead->num++;
    return phead->num;
}
