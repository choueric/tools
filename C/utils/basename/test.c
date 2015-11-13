#include <stdio.h>
#include "basename.h"

#define INPUT_1 "/home/zhs/test"
#define INPUT_2 "/home/zhs/test/main.c/"
#define INPUT_3 "/home/zhs/test/main.c"

void test(char *input, int size)
{
	char output[100] = {0};

	basename_r(input, output, sizeof(output));

	printf("-----------------------------------------\n");
	printf("input is: [%s]\noutput is : [%s]\n", input, output);
}

int main(int argc, char **argv)
{
	test(INPUT_1, sizeof(INPUT_1));
	test(INPUT_2, sizeof(INPUT_2));
	test(INPUT_3, sizeof(INPUT_3));

	return 0;
}
