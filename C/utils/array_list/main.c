#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "array_list.h"

int main()
{
	list_t *list = list_create(0);
	type_t a = {.name = "alice",};
	type_t b = {.name = "bob",};
	type_t c = {.name = "cavin",};
	type_t d = {.name = "denis",};
	type_t e = {.name = "emerson",};
	type_t f = {.name = "frank",};

	list_add(list, &a);
	list_add(list, &b);
	list_add(list, &c);
	list_add(list, &d);
	list_add(list, &e);
	list_add(list, &f);
	list_show(list);

	list_del(list, &d);
	list_show(list);

	return 0;
}

