#include "lib.h"
#include <stdio.h>

int main(int argc, char **argv)
{
	int x = 10;
	int y = 20;

	printf("%d + %d = %d\n", x, y, add(x, y));
	return 0;
}
