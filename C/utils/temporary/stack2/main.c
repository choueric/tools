#include <stdio.h>

static int entry_aa()
{
	char str[] = "entry_aa";

	void *fp, *sp;
	__asm("mov %0,r11": "=r"(fp));
	__asm("mov %0,sp": "=r"(sp));
	printf("%s:fp\t[%p]\t%p\n", __FUNCTION__, &fp, fp);
	printf("%s:sp\t[%p]\t%p\n", __FUNCTION__, &sp, sp);

	while (1) {
	}

	return 0;
}

static void *entry_a(void *arg)
{
	char str[] = "entry_a";

	void *fp, *sp;
	__asm("mov %0,r11": "=r"(fp));
	__asm("mov %0,sp": "=r"(sp));
	printf("%s:fp\t[%p]\t%p\n", __FUNCTION__, &fp, fp);
	printf("%s:sp\t[%p]\t%p\n", __FUNCTION__, &sp, sp);

	entry_aa();

	return NULL;
}

int test1()
{
	int a[10];
	int i;

	for (i = 0; i < 10; i++)
		a[i] = i;
}

int test2()
{
	int i;
	int b[10];
	int *p = b;
	for (i = 0; i < 10; i++)
		*p++ = i;

	return 0;
}

int main(int argc, char **argv)
{
	test1();
	test2();
	entry_a(NULL);

	return 0;
}
