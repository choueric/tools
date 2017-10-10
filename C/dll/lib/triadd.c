#include <stdio.h>
#include <dlfcn.h>

#include <add.h>

static void *pHandle = NULL;
static int (*addFunc)(int x,int y) = NULL;

static void init_lib()
{
	if (pHandle)
		return;

	pHandle = dlopen("/tmp/libvendor.so", RTLD_NOW);
	if (pHandle == NULL) {
		printf("++ dlopen failed: %s\n", dlerror());
		return;
	}

	addFunc = dlsym(pHandle, "add");
	if (addFunc == NULL) {
		printf("++ dlsym failed: %s\n", dlerror());
		return;
	}
}

int triadd(int x, int y, int z)
{
	init_lib();
	if (addFunc != NULL)
		return addFunc(addFunc(x, y), z);
	else
		printf("++ add is NULL\n");

	return 0;
}
