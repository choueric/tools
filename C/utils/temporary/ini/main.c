#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#include "ini.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

static int parse_func(char *key, char *val, void *arg)
{
	printf("[%s] = [%s]\n", key, val);
		
	return 0;
}


int main(int argc, char *argv[])
{
	ParseIni("./cfg.ini", parse_func, NULL);
	return 0;
}
