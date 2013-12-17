#ifndef ITEM_H
#define ITEM_H

#define DELIM "/ "
#define DELIMNUM 4

struct head {
    struct item *head;
    struct item *tail;
    int num;
};

struct item {
    char *word;
    char *cog;
    char *sim;
    char *syn;
    struct item *pre;
    struct item *next;
};

struct item *make_item(char *word, char *cog, char *sim, char *syn);
struct item *parse_item(char *line);

int insert_item(struct head *phead, struct item *pitem,
        struct item *head, struct item *tail);


int add_item(struct item *pitem, struct head *phead);

void print_item(struct item *p);

#endif
