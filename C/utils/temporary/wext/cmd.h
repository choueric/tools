#ifndef _CMD_H 
#define _CMD_H

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/wireless.h>

#define dp(fmt, args...) printf("%s(): " fmt, __FUNCTION__, ##args)

#define IOCTL_SET_ACTIVE_SCAN (SIOCIWFIRSTPRIV + 1)
#define IOCTL_GET_RSSI (SIOCIWFIRSTPRIV + 3)
#define IOCTL_SET_PASSIVE_SCAN (SIOCIWFIRSTPRIV + 5)

#define IOCTL_SET_SCAN_PERIOD (SIOCIWFIRSTPRIV + 14)
#define IOCTL_GET_SCAN_PERIOD (SIOCIWFIRSTPRIV + 15)

#define IOCTL_SET_FULLSCAN_PERIOD (SIOCIWFIRSTPRIV + 16)
#define IOCTL_GET_FULLSCAN_PERIOD (SIOCIWFIRSTPRIV + 17)

#define IOCTL_SET_DELTA (SIOCIWFIRSTPRIV + 18)
#define IOCTL_GET_DELTA (SIOCIWFIRSTPRIV + 19)

#define IOCTL_SET_TRIGGER (SIOCIWFIRSTPRIV + 20)
#define IOCTL_GET_TRIGGER (SIOCIWFIRSTPRIV + 21)

struct ioctl_args;
struct ioctl_entry;

typedef int (*entry_func)(struct ioctl_args *, struct ioctl_entry *);

struct ioctl_args {
	int fd;
	char *ifname;
	int argc;
	char **argv;
};

struct ioctl_entry {
	char *cmd;
	entry_func entry;
	int ioctl_cmd;
};

int set_scan_period(struct ioctl_args *args, struct ioctl_entry *entry);
int get_scan_period(struct ioctl_args *args, struct ioctl_entry *entry);

int set_fullscan_period(struct ioctl_args *args, struct ioctl_entry *entry);
int get_fullscan_period(struct ioctl_args *args, struct ioctl_entry *entry);

int set_trigger(struct ioctl_args *args, struct ioctl_entry *entry);
int get_trigger(struct ioctl_args *args, struct ioctl_entry *entry);

int set_delta(struct ioctl_args *args, struct ioctl_entry *entry);
int get_delta(struct ioctl_args *args, struct ioctl_entry *entry);



int set_freq(struct ioctl_args *args, struct ioctl_entry *entry);
int get_freq(struct ioctl_args *args, struct ioctl_entry *entry);
int get_rssi(struct ioctl_args *args, struct ioctl_entry *entry);
int set_active_scan(struct ioctl_args *args, struct ioctl_entry *entry);
int set_passive_scan(struct ioctl_args *args, struct ioctl_entry *entry);
#endif
