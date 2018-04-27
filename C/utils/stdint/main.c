#include <stdio.h>

#include <stdint.h>
#include <inttypes.h>
#include <stddef.h>

#define PRINT_SIZE(type) printf("size of " #type " is %zu\n", sizeof(type))

int main()
{
	PRINT_SIZE(int);
	PRINT_SIZE(uint32_t);
	PRINT_SIZE(intptr_t);
	PRINT_SIZE(void *);
	PRINT_SIZE(size_t);

	int a = 4;
	int b = 5;

	intptr_t pa = (intptr_t)&a;
	intptr_t pb = (intptr_t)&b;

	printf("&a = %p, &b = %p, pa = %" PRIdPTR ", pb = %" PRIdPTR "\n", &a, &b, pa, pb);

	ptrdiff_t diff = pb - pa;
	printf("diff = %td\n", diff);

#ifdef __USE_GNU
	printf("USE GNU\n");
#else
	printf("NOT USE GNU\n");
#endif

#if defined __USE_MISC
	printf("USE MISC\n");
#else
	printf("NOT USE MISC\n");
#endif

	printf("a = %d, r = %d\n", 1, 1 != 0);
	printf("a = %d, r = %d\n", 0, 0 != 0);

    return 0;
}

