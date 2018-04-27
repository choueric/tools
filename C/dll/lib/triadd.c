#include <stdio.h>

#include <add.h>

#include "dynlink_loader.h"

typedef int (tAddFunc)(int x,int y);

typedef struct VendorFuncs {
	void *lib;
	tAddFunc *add;
}VendorFuncs;


static int init_lib(struct VendorFuncs **ppFuncs)
{
	GENERIC_LOAD_FUNC_PREAMBLE(VendorFuncs, ppFuncs, "/tmp/libvendor.so");

	LOAD_SYMBOL(add, tAddFunc, "add");

	GENERIC_LOAD_FUNC_FINALE(ppFuncs);
}

int triadd(int x, int y, int z)
{
	VendorFuncs *vFuncs;

	int ret = init_lib(&vFuncs);
	if (ret != 0) {
		printf("!! load symbol add failed: %d\n", ret);
		return 0;
	}

	return vFuncs->add(vFuncs->add(x, y), z);
}
