
static char *device = "/dev/ttyS0";
static int loop = 0;

static const char *def(int val)
{
	return val ? "enabled": "disabled";
}

static void print_usage(char *p)
{
	fprintf(stderr, "Usage: %s [-d device] [-l]\n", p);
	fprintf(stderr, "    -d: tty device. Default is [%s]\n", device);
	fprintf(stderr, "    -l: loop test. Default is [%s]\n", def(loop));
}

static void parse_args(int argc, char **argv)
{
	int opt;
	
	while ( (opt = getopt(argc, argv, "d:l")) != -1) {
		switch (opt) {
			case 'd': device = optarg; break;
			case 'l': loop = 1; break;
			default:
				print_usage(argv[0]);
				exit(0);
		}
	}
}

static void print_options()
{
	printf("\n=========================================================\n");
	printf("  device: %s\n", device);
	printf("  loop test: %s\n", def(loop));
	printf("=========================================================\n");
}
