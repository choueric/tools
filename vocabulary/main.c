/*
 * [abolition/ ad, ajsdk, jsakd/ adf, ksjdk, kdsk/ ksadk, kasdjf/ ]
 * 1. 本词 word
 * 2. 同根词 cognate
 * 3. 形近、音近词 similar
 * 4. 同义、反义词 synonym
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "item.h"

#ifdef DEBUG
static int debug = 1;
#else
static int debug = 0;
#endif

#define LEN 1024

static int input_words(FILE *fp, char *word, char *cog, char *sim, char *syn)
{
    int i, read;
    size_t n;
    char *str[4] = {word, cog, sim, syn};

    printf("please input 4 elements:\n");
    for (n = LEN, i = 0; i < 4; i++, n = LEN) {
        printf("%d: ", i);
        fflush(stdout);
        read = getline(&str[i], &n, fp);
        str[i][read-1] = '\0';
        if (read == -1)
            return 1;
        if (!strcmp(word, "end"))
            return 1;
    }
    printf("--------------------------------------------------\n");
    return 0;
}

static int output_words(FILE *fp, struct head *phead)
{
    struct item *p = NULL;

    p = phead->head;
    while (p != NULL) {
        fprintf(fp, "%s%s%s%s%s%s%s%s\n",
            p->word, DELIM, p->cog, DELIM, p->sim, DELIM, p->syn, DELIM);
        p = p->next;
    }

    return 0;
}

static void print_list(struct head *phead)
{
    struct item *p;
    int i = 0;

    if (!debug)
        return;

    printf("--- %d ---\n", phead->num);
    p = phead->head;
    while (p != NULL) {
        printf("%d: ", ++i);
        print_item(p);
        p = p->next;
    }
}

static struct head *parse_file(char *file)
{
    FILE *fp;
    struct head *phead = NULL;
    struct item *pitem = NULL;
    char line[LEN];
    char *pline = line;
    size_t n = LEN;
    int read;

    fp = fopen(file, "r+");
    if (fp == NULL) {
        perror("open file failed");
        return NULL;
    }

    phead = malloc(sizeof(struct head));
    if (phead == NULL) {
        perror("malloc failed");
        fclose(fp);
        return NULL;
    }

    phead->head = NULL;
    phead->tail = NULL;
    phead->num = 0;

    while ((read = getline(&pline, &n, fp)) != -1) {
        line[read-1] = '\0';
        if (debug)
            printf("read [%s]\n", line);
        pitem = parse_item(line);
        if (pitem == NULL)
            break;
        add_item(pitem, phead);
    }
    fclose(fp);

    return phead;
}

static void bakeup()
{
    system("cp ./output.txt ./output.txt.bak");
}

int main(int argc, char **argv)
{
    char word[LEN];
    char cognate[LEN];
    char similar[LEN];
    char synonym[LEN];

    FILE *fp1, *fp2;

    char *filename = "./output.txt";

    struct head *phead = NULL;
    struct item *pitem = NULL;

    bakeup();

    fp1 = stdin;

    phead = parse_file(filename);
    if (phead == NULL) {
        printf("parse_file failed\n");
        return 1;
    }
    print_list(phead);

    while (input_words(fp1, word, cognate, similar, synonym) == 0) {
        pitem = make_item(word, cognate, similar, synonym);
        if (pitem == NULL) {
            printf("make item failed\n");
            return 1;
        }
        insert_item(phead, pitem, phead->head, phead->tail);
        if (debug)
            printf("---> insert item %s\n", pitem->word);
        print_list(phead);
    }

    printf("---> after input, now output to file\n");
    fp2 = fopen("./output.txt", "w+");
    if (fp2 == NULL) {
        perror("open failed");
        return 1;
    }
    output_words(fp2, phead);
    fclose(fp2);

    return 0;
}
