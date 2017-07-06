static int autotest = 0;
static char *cmd = "read";
static char *rtcdev = "/dev/rtc1";

static char *commands[] = {
	"read",
	"write",
	"vl",
	"update",
	"alarm",
	"periodic",
	NULL,
};

static const char *def(int val)
{
	return val ? "enabled": "disabled";
}

static void print_usage(char *p)
{
	int i = 0;
	fprintf(stderr, "Usage: %s [-d /dev/rtc0] <-t | -c [cmd]> [-h]\n", p);
	fprintf(stderr, "    -c: manual test mode with command. Default [%s]\n", cmd);
	for (i = 0; commands[i] != NULL; i++)
		fprintf(stderr, "        %s\n", commands[i]);
	fprintf(stderr, "    -t: auto test mode. Default [%s]\n", def(autotest));
	fprintf(stderr, "    -d: rtc device. Default [%s]\n", rtcdev);
	fprintf(stderr, "    -h: print this message.\n");
}

static void parse_args(int argc, char **argv)
{
	int opt;
	
	while ( (opt = getopt(argc, argv, "d:tc:h")) != -1) {
		switch (opt) {
			case 'd': rtcdev = optarg; break;
			case 't': autotest = 1; break;
			case 'c': cmd = optarg; break;
			case 'h':
			default: 
				print_usage(argv[0]);
				exit(0);
		}
	}
}


static void print_options()
{
	printf("\n=========================================================\n");
	printf("  device: %s\n", rtcdev);
	printf("  auto test: %s\n", def(autotest));
	printf("  manual cmd: %s\n", cmd);
	printf("=========================================================\n");
}
