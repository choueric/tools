#include <stdio.h>
#include <stdlib.h>

#include <triadd.h>

int main(int argc, char **argv)
{
	int a = 10;
	int b = 3;
	int c = 5;

	if (argc == 4) {
		a = atoi(argv[1]);
		b = atoi(argv[2]);
		c = atoi(argv[3]);
	}

	printf("%d+%d+%d=%d\n", a, b, c, triadd(a, b, c));
	return 0;
}
