#pragma once

/*
 * `list_t` is a list based on array, which can insert and remove pointer of
 * `type_t` from or out of it.
 */

typedef struct {
	char name[10];
} type_t;

typedef struct {
	type_t **data;
	size_t len;
	size_t cap;
} list_t;

list_t *list_create(size_t size);
void list_destroy(list_t *list);

int list_add(list_t *list, type_t *member);
int list_del(list_t *list, type_t *member);

void list_show(list_t *list);
