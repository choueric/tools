#include <stdio.h>
#include <unistd.h>
#include "change_priority.h"

int main(int argc, char **argv)
{
	get_priority();

	set_priority(66);

	get_priority();

	sleep(100);

	return 0;
}
