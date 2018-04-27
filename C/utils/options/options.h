#pragma once

#ifndef false
#define false 0
#endif

#ifndef true
#define true 1
#endif

typedef int bool;

// TODO: add options here
struct options_t {
	char *device;
	bool loop;
};

void opts_def(struct options_t *opts);
void opts_usage(struct options_t *opts, char *p);
void opts_parse(struct options_t *opts, int argc, char **argv);
void opts_dump(struct options_t *opts);
