#include "cmd.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static int do_set_ioctl_int(struct ioctl_args *args, struct ioctl_entry *entry)
{
	int val;
	int ret;
	struct iwreq iwr;

	if (args->argc != 1) {
		dp("%s: argc not match\n", entry->cmd);
		return -1;
	}

	val = atoi(args->argv[0]);
	memset(&iwr, 0, sizeof(iwr));
	strncpy(iwr.ifr_name, args->ifname, strlen(args->ifname));
	iwr.u.data.pointer = &val;

	ret = ioctl(args->fd, entry->ioctl_cmd, &iwr);
	if (ret < 0) {
		perror("ioctl failed");
		return -2;
	}

	return 0;
}

static int do_get_ioctl_int(struct ioctl_args *args, struct ioctl_entry *entry)
{
	int val;
	int ret;
	struct iwreq iwr;

	if (args->argc != 0) {
		dp("%s: argc not match\n", entry->cmd);
		return -1;
	}

	memset(&iwr, 0, sizeof(iwr));
	strncpy(iwr.ifr_name, args->ifname, strlen(args->ifname));
	iwr.u.data.pointer = &val;

	ret = ioctl(args->fd, entry->ioctl_cmd, &iwr);
	if (ret < 0) {
		perror("ioctl failed");
		return -2;
	}

	return val;
}



int set_scan_period(struct ioctl_args *args, struct ioctl_entry *entry)
{
	return do_set_ioctl_int(args, entry);
}

int get_scan_period(struct ioctl_args *args, struct ioctl_entry *entry)
{
	return do_get_ioctl_int(args, entry);
}

int set_fullscan_period(struct ioctl_args *args, struct ioctl_entry *entry)
{
	return do_set_ioctl_int(args, entry);
}

int get_fullscan_period(struct ioctl_args *args, struct ioctl_entry *entry)
{
	return do_get_ioctl_int(args, entry);
}

int set_trigger(struct ioctl_args *args, struct ioctl_entry *entry)
{
	return do_set_ioctl_int(args, entry);
}

int get_trigger(struct ioctl_args *args, struct ioctl_entry *entry)
{
	return do_get_ioctl_int(args, entry);
}

int set_delta(struct ioctl_args *args, struct ioctl_entry *entry)
{
	return do_set_ioctl_int(args, entry);
}

int get_delta(struct ioctl_args *args, struct ioctl_entry *entry)
{
	return do_get_ioctl_int(args, entry);
}

int set_freq(struct ioctl_args *args, struct ioctl_entry *entry)
{
	int val;
	int ret;
	struct iwreq iwr;

	if (args->argc != 1) {
		dp("%s: argc not match\n", entry->cmd);
		return -1;
	}

	val = atoi(args->argv[0]);
	memset(&iwr, 0, sizeof(iwr));
	strncpy(iwr.ifr_name, args->ifname, strlen(args->ifname));
	iwr.u.freq.m = val;  // ch
	iwr.u.freq.e = 0;
	printf("set freq %d %d\n", iwr.u.freq.m, iwr.u.freq.e);

	ret = ioctl(args->fd, entry->ioctl_cmd, &iwr);
	if (ret < 0) {
		perror("ioctl failed");
		return -2;
	}

	return 0;
}

int get_freq(struct ioctl_args *args, struct ioctl_entry *entry)
{
	struct iwreq iwr;
	int ret;

	if (args->argc != 0) {
		dp("%s: argc not match\n", entry->cmd);
		return -1;
	}

	memset(&iwr, 0, sizeof(iwr));
	strncpy(iwr.ifr_name, args->ifname, strlen(args->ifname));

	ret = ioctl(args->fd, entry->ioctl_cmd, &iwr);
	if (ret < 0) {
		perror("ioctl failed");
		return -2;
	}
	printf("get freq: %d %d\n", iwr.u.freq.m, iwr.u.freq.e);

	return 0;
}

int get_rssi(struct ioctl_args *args, struct ioctl_entry *entry)
{
	struct iwreq iwr;
	int ret;
	char buf[100];

	if (args->argc != 0) {
		dp("%s: argc not match\n", entry->cmd);
		return -1;
	}

	memset(&iwr, 0, sizeof(iwr));
	strncpy(iwr.ifr_name, args->ifname, strlen(args->ifname));
	iwr.u.data.pointer = buf;
	iwr.u.data.length = 100;

	ret = ioctl(args->fd, entry->ioctl_cmd, &iwr);
	if (ret < 0) {
		perror("ioctl failed");
		return -2;
	}
	printf("get rssi: %s\n", buf);

	return 0;
}


int set_active_scan(struct ioctl_args *args, struct ioctl_entry *entry)
{
	struct iwreq iwr;
	int ret;
	char buf[100];

	memset(&iwr, 0, sizeof(iwr));
	strncpy(iwr.ifr_name, args->ifname, strlen(args->ifname));
	iwr.u.data.pointer = buf;
	iwr.u.data.length = 100;
	
	ret = ioctl(args->fd, entry->ioctl_cmd, &iwr);
	if (ret < 0) {
		perror("ioctl failed");
		return -2;
	}
	printf("get : %s\n", buf);

	return 0;
}

int set_passive_scan(struct ioctl_args *args, struct ioctl_entry *entry)
{
	struct iwreq iwr;
	int ret;
	char buf[100];

	memset(&iwr, 0, sizeof(iwr));
	strncpy(iwr.ifr_name, args->ifname, strlen(args->ifname));
	iwr.u.data.pointer = buf;
	iwr.u.data.length = 100;
	
	ret = ioctl(args->fd, entry->ioctl_cmd, &iwr);
	if (ret < 0) {
		perror("ioctl failed");
		return -2;
	}
	printf("get : %s\n", buf);

	return 0;
}
