#include <stdio.h>
#include <string.h>
#include <fcntl.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/wireless.h>

#include "cmd.h"

#define IFNAME "eth1"

struct ioctl_entry entrys[] = {
	{
		"set_scan_period",
		set_scan_period,
		IOCTL_SET_SCAN_PERIOD,
	},
	{
		"get_scan_period",
		get_scan_period,
		IOCTL_GET_SCAN_PERIOD,
	},
	{
		"set_fullscan_period",
		set_fullscan_period,
		IOCTL_SET_FULLSCAN_PERIOD,
	},
	{
		"get_fullscan_period",
		get_fullscan_period,
		IOCTL_GET_FULLSCAN_PERIOD,
	},
	{
		"set_trigger",
		set_trigger,
		IOCTL_SET_TRIGGER,
	},
	{
		"get_trigger",
		get_trigger,
		IOCTL_GET_TRIGGER,
	},
	{
		"set_delta",
		set_delta,
		IOCTL_SET_DELTA,
	},
	{
		"get_delta",
		get_delta,
		IOCTL_GET_DELTA,
	},
	{
		"set_freq",
		set_freq,
		SIOCSIWFREQ,
	},
	{
		"get_freq",
		get_freq,
		SIOCGIWFREQ,
	},
	{
		"get_rssi",
		get_rssi,
		IOCTL_GET_RSSI,
	},
	{
		"set_active_scan",
		set_active_scan,
		IOCTL_SET_ACTIVE_SCAN,
	},
	{
		"set_passive_scan",
		set_passive_scan,
		IOCTL_SET_PASSIVE_SCAN,
	},
};


int iw_open()
{
	int sock;

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {
		perror("socket() failed");
		return -1;
	}

	return sock;
}

int iw_close(int fd)
{
	return close(fd);
}

void print_usage()
{
	int i, n;
	struct ioctl_entry *pentry = NULL;

	n = sizeof(entrys) / sizeof(entrys[0]);
	printf("options do not match: \n");
	for (i = 0; i < n; i++) {
		pentry = entrys + i;
		printf("    %s\n", pentry->cmd);
	}
}

int main(int argc, char **argv)
{
	int fd;
	int ret;
	int i;
	int n;
	struct ioctl_entry *pentry = NULL;
	struct ioctl_args args;
	char *cmd;

	if (argc < 2) {
		print_usage();
		return 1;

	}

	fd = iw_open();
	if (fd < 0) 
		return 1;

	cmd = argv[1];
	n = sizeof(entrys) / sizeof(entrys[0]);
	for (i = 0; i < n; i++) {
		pentry = entrys + i;
		if (!strcmp(cmd, pentry->cmd)) {
			args.fd = fd;
			args.ifname = IFNAME;
			args.argc = argc - 2;
			args.argv = argv + 2;
			ret = pentry->entry(&args, pentry);
			break;
		}

	}
	if (i == n) {
		print_usage();
	} else {
		printf("ret = %d\n", ret);
	}

	iw_close(fd);

	return 0;
}
