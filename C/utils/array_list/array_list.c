#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <stdlib.h>

#include "array_list.h"

// ajust size of protocol list if necessary
static int list_ajust(list_t *list)
{
	if (list->len < list->cap)
		return 0;

	list->cap = (list->cap == 0 ? 4: list->cap * 2);
	size_t size = sizeof(type_t *) * list->cap;
	list->data = (type_t **)realloc(list->data, size);
	if (list->data == NULL)
		return -ENOMEM;

	return 0;
}

list_t *list_create(size_t size)
{
	list_t *list = (list_t *)malloc(sizeof(*list));
	if (list == NULL) {
		return NULL;
	}

	if (size == 0) {
		list->data = NULL;
	} else {
		list->data = (type_t **)malloc(sizeof(type_t *) * size);
		if (list->data == NULL) {
			free(list);
			return NULL;
		}
	}
	list->len = list->cap = size;
	return list;
}

void list_destroy(list_t *list)
{
	free(list->data);
	free(list);
}

int list_add(list_t *list, type_t *member)
{
	assert(NULL != member);

	int ret;
	if ( (ret = list_ajust(list)) != 0)
		return ret;

	list->data[list->len] = member;
	list->len++;

	return 0;
}

int list_del(list_t *list, type_t *member)
{
	assert(NULL != member);

	for (int i = 0; i < list->len; i++) {
		type_t *p = list->data[i];
		if (!strcmp(member->name, p->name)) {
			printf("delete [%d] %s\n", i, p->name);
			for (int j = i; j < list->len - 1; j++) {
				printf("replace %d with %d\n", j, j+1);
				list->data[j] = list->data[j+1];
			}
			list->data[list->len - 1] = NULL;
			list->len--;
			return 0;
		}
	}

	return -EEXIST;
}

void list_show(list_t *list)
{
	printf("len: %zd, cap: %zd\n", list->len, list->cap);
	for (int i = 0; i < list->len; i++)
		printf("  [%d]: %s\n", i, list->data[i]->name);
}
