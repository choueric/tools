#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "cmd_entry.h"

static int led_on_test(struct cmd_args *args, struct cmd_entry *entry)
{
	printf("execute: %s\n", entry->cmd);
	return 0;
}

////////////////////////////////////////////////////////////////////////////////

static struct cmd_entry cmd_entries[] = {
	{"o", led_on_test, "turn led ON"},
	{NULL, NULL, NULL},
};

int main(int argc, char **argv)
{
	int fd = 0, ret;
    char cmd[64];

#if 0
	fd = open("/dev/prpctrl", O_RDWR);
	if (fd < 0) {
		printf("open failed: %m\n");
		return -1;
	}
#endif

	print_usage(cmd_entries);

	while (1) {
		printf("\ninput command: ");
		fflush(stdout);
		ret = get_cmd(stdin, cmd, sizeof(cmd));
		ret = run_cmd_entry(fd, cmd, cmd_entries,
				argc, argv);
		if (ret)
			break;
	}

#if 0
	close(fd);
#endif
    return 0;
}
