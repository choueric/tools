#include <unistd.h>

#include "statistic.h"

int main(int argc, char **argv)
{
	struct statistic stat;

	init_statistic(&stat, "test", 5000000);

	while (1) {
		sleep(1);
		print_statistic(&stat);
	}
	return 0;
}
