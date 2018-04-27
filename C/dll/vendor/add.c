#include "add.h"
#include <lowlevel.h>

int add(int x, int y)
{
	return lowLevel_add(x, y);
}
