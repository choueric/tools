#include <stdio.h>
#include <string.h>
#include "dir_ops.h"

#define INPUT_1 "/home/zhs/test"
#define INPUT_2 "/home/zhs/test/main.c/"
#define INPUT_3 "/home/zhs/test/main.c"

// choose *.c files
static int filter(const struct dirent *entry)
{
	if (strstr(entry->d_name, ".c"))
		return 1;
	else
		return 0;
}


int main(int argc, char **argv)
{
#if 0
	delete_files("./dd");
	delete_appendix("./aa", ".c");
#endif
	int n = scan_dir(".", filter);
	printf("count of *.c files: %d\n", n);
	return 0;
}
