#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>

#include "options.h"

static const char *bool_str(bool val)
{
	return val ? "enabled": "disabled";
}

void opts_def(struct options_t *opts)
{
	opts->device = "/dev/ttyS0";
	opts->loop = false;
}

void opts_usage(struct options_t *opts, char *p)
{
	fprintf(stderr, "Usage: %s [-d device] [-l]\n", p);
	fprintf(stderr, "  -d: tty device. Default is [%s]\n", opts->device);
	fprintf(stderr, "  -l: loop test. Default is [%s]\n", bool_str(opts->loop));
}

void opts_parse(struct options_t *opts, int argc, char **argv)
{
	int opt;
	
	while ( (opt = getopt(argc, argv, "d:l")) != -1) {
		switch (opt) {
			case 'd': opts->device = optarg; break;
			case 'l': opts->loop = true; break;
			default:
				opts_usage(opts, argv[0]);
				exit(0);
		}
	}
}

void opts_dump(struct options_t *opts)
{
	printf("\n=========================================================\n");
	printf("  device:\t%s\n", opts->device);
	printf("  loop test:\t%s\n", bool_str(opts->loop));
	printf("=========================================================\n");
}
