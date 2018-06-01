#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "xui.h"
#include "percolation.h"

static void print_usage()
{
	printf("usage: percolation num [-auto]\n"
		   " this will create a [num * num] matrix.\n"
		   " clicking mouse button will start and step one step.\n"
		   " when specify -auto option, it will go head without clicking mouse\n"
		   " pressing keyboard will exit.\n");
}

static int parse_opt(int argc, char **argv, int *pnum, int *pauto)
{
	*pauto = 0;

	if (argc == 2) {
		*pnum = atoi(argv[1]);
		if (*pnum <= 0) {
			printf("invalid argument %s\n", argv[1]);
			return 2;
		}
	} else if (argc == 3) {
		*pnum = atoi(argv[1]);
		if (*pnum <= 0) {
			printf("invalid argument %s\n", argv[1]);
			print_usage();
			return 2;
		}
		if (!strcmp(argv[2], "-auto"))
			*pauto = 1;
		else {
			printf("invalid argument %s\n", argv[2]);
			print_usage();
			return 3;
		}
	} else {
		print_usage();
		return 1;
	}

	return 0;
}

int main(int argc, char **argv)
{
	int n = 10;
	int w = 20;
	int b = 1;
	int auto_click = 0;
	struct rect_table_t *ptable = NULL;

	if (parse_opt(argc, argv, &n, &auto_click))
		return 1;

	ptable = PercolationInit(n);
	XuiInit(n, w, b, auto_click);
	XuiMainLoop(TickTack, (void *)ptable);
	XuiClose();
	PercolationClose(ptable);

	return 0;
}
