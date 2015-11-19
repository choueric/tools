#include <stdio.h>
#include "delete.h"

#define INPUT_1 "/home/zhs/test"
#define INPUT_2 "/home/zhs/test/main.c/"
#define INPUT_3 "/home/zhs/test/main.c"

int main(int argc, char **argv)
{
	delete_files("./dd");
	delete_appendix("./aa", ".c");
	return 0;
}
