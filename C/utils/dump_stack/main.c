#include <stdio.h>
#include <execinfo.h>
#include <stdlib.h>

void dump_stack()
{
	void* callstack[128];
	int i, frames = backtrace(callstack, 128);
	char** strs = backtrace_symbols(callstack, frames);
	for (i = 0; i < frames; ++i) {
		printf("== %s\n", strs[i]);
	}
	free(strs);
}

static void print_stack(void)
{
#define SIZE 100
	int j, nptrs;
	void *buffer[SIZE];
	char **strings;

	nptrs = backtrace(buffer, SIZE);
	printf("backtrace() returned %d addresses\n", nptrs);

	strings = backtrace_symbols(buffer, nptrs);
	if (strings == NULL) {
		perror("backtrace_symbols");
		return;
	}

	for (j = 0; j < nptrs; j++)
		printf("== %s\n", strings[j]);

	free(strings);
#undef SIZE
}

static int func_a()
{
	printf("func a\n");
	print_stack();
	return 0;
}

static int func_b()
{
	printf("func b\n");
	func_a();

	return 0;
}

int main()
{
	printf("main()\n");
	func_b();
    return 0;
}

