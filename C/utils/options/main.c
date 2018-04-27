#include <stdio.h>

#include "options.h"

int main(int argc, char **argv)
{
	struct options_t opts;

	opts_def(&opts);
	opts_parse(&opts, argc, argv);
	opts_dump(&opts);

	printf("--> device = %s, loop = %d\n", opts.device, opts.loop);

	return 0;
}
