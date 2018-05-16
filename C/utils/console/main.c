#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "console.h"

static inline void dec_ratio(int *p)
{
	int v = *p;
	if (v == 0)
		return;
	else
		*p = v-1;
}

static inline void inc_ratio(int *p)
{
	int v = *p;
	if (v == 12)
		return;
	else
		*p = v+1;
}

int main()
{
	char c = 0;
	int ratio = 0;
	console_t con;

	printf("Press <j> to decrease, <k> to increase, <enter> quit\n");

	console_enter_raw(&con);
	while ( ((c = fgetc(stdin)) != EOF) && c != '\n') {
		switch (c) {
		case 'j': dec_ratio(&ratio); break;
		case 'k': inc_ratio(&ratio); break;
		default: break;
		}
		printf("ratio = %d\n", ratio);
	}
	console_leave_raw(&con);
	return 0;
}

