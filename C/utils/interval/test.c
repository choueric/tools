#include <stdio.h>
#include <unistd.h>
#include "interval.h"

#define INPUT_1 "/home/zhs/test\n"
#define INPUT_2 "/home/zhs/test/main.c/"
#define INPUT_3 "/home/zhs/test/main.c"

static struct timeval tv1 = {0}, tv2 = {0};

int count = 0;

void test(char *input, int size)
{
	printf("-------------- test %d --------------------\n", count++);

	debugoutput(input, STDOUT_FILENO);

	printf("\n\n");
}

int main(int argc, char **argv)
{
	do_edvlog(" do test %d\n", count);
	GET_TIME("1");
	test(INPUT_1, sizeof(INPUT_1));

	do_edvlog(" do test %d\n", count);
	GET_TIME("2");
	test(INPUT_2, sizeof(INPUT_2));

	do_edvlog(" do test %d\n", count);
	GET_TIME("3");
	test(INPUT_3, sizeof(INPUT_3));


	return 0;
}
