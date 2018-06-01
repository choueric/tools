#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <linux/types.h>
#include <stdarg.h>

#include "flash.h"
#include "upgrade.h"
#include "crc.h"
#include "spidev.h"

#define VERSION "0.3"

/*
 * TODO:
 * - if length of last data is less than page size, it cannot be writen into flash
 * - add timeout to flash.c, in order not to loop forever when get WIP/WEL.
 */

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

struct cmd_entry;
struct cmd_args;
typedef int (*entry_func) (struct cmd_args *, struct cmd_entry *);

struct cmd_entry {
	char *cmd;
	entry_func func;
};

struct cmd_args {
	int fd;
	int argc;
	char **argv;
};

struct range {
	int start;
	int size;
};


static struct flash_info m25p40 = {
	.page_size = 256,
	.pages_per_sector = 256,
	.sectors_per_block = 8,
	.blocks_per_chip = 1,
};

static struct spi_cfg cfg_info = {
	.device = "/dev/spidev0.0",
	.mode = 0,
	.bits = 8,
	.speed = 1000000,  // 1MHz
	.delay = 0,
};

// NOTE: range of firmware in flash
static struct range upgrade_range = {
    .start = 0x0,
    .size = 0x70000,
};

/**********************************************************/
static struct range *get_upgrade_range()
{
    return &upgrade_range;
}

static struct flash_info *get_flash_info()
{
    return &m25p40;
}

static void do_log(const char *format, ...)
{
	char str[50] = {0};
	char *substr = NULL;
	va_list args;
	struct timeval tv;

	va_start(args, format);
	gettimeofday(&tv, NULL);
	snprintf(str, 50, "[%ld.%06ld] ", tv.tv_sec, tv.tv_usec);
	substr = str + strlen(str);
	vsnprintf(substr, 50, format, args);
	va_end(args);
	printf("%s", str);
}

static void pabort(const char *s)
{
	perror(s);
	abort();
}

/*----------------------------------------------------------------------*/

static int cmd_read_sign(struct cmd_args *args, struct cmd_entry *entry)
{
	uint8_t val = 0;

	val = flash_read_sign(args->fd);
	printf("%s: sign = %02x\n", entry->cmd, val);

	return 0;
}

static int cmd_read_jedec(struct cmd_args *args, struct cmd_entry *entry)
{
	int id = 0;

	id = flash_read_jedec(args->fd);
	printf("%s: jedec id = %x\n", entry->cmd, id);

	return 0;
}

/*
 * write the firmware binary file to flash
 */
static int cmd_upgrade(struct cmd_args *args, struct cmd_entry *entry)
{
    struct range *r = get_upgrade_range();
    struct flash_info *f = get_flash_info();
    do_log("enter %s \n", entry->cmd);

    if (args->argc != 1) {
        printf("usage: %s <firmware>\n", entry->cmd);
        return -1;
    }

    return upgrade_firmware(args->fd, args->argv[0], r->start, r->size, 
            f->page_size, f->page_size * f->pages_per_sector);

    return 0;
}

static int cmd_flash(struct cmd_args *args, struct cmd_entry *entry)
{
    struct range *r = get_upgrade_range();
    struct flash_info *f = get_flash_info();
    do_log("enter %s \n", entry->cmd);

    if (args->argc != 1) {
        printf("usage: %s <firmware>\n", entry->cmd);
        return -1;
    }

    return upgrade_firmware_flash_only(args->fd, args->argv[0], r->start, r->size, 
            f->page_size, f->page_size * f->pages_per_sector);

    return 0;
}

static int cmd_switch(struct cmd_args *args, struct cmd_entry *entry)
{
    do_log("enter %s \n", entry->cmd);

    return bridge_switch(args->fd);
}

static int cmd_read_flash(struct cmd_args *args, struct cmd_entry *entry)
{
    do_log("enter %s \n", entry->cmd);

    return flash_read_to_file(args->fd, "read.bin");
}

static int cmd_read_firmware_version(struct cmd_args *args, struct cmd_entry *entry)
{
    return read_spi_version(args->fd);
}

static struct cmd_entry entrys[] = {
	{ "sign", cmd_read_sign, },
	{ "jedec", cmd_read_jedec, },
	{ "upgrade", cmd_upgrade, },
	{ "flash", cmd_flash, },
    { "switch", cmd_switch,},
    { "read", cmd_read_flash,},
    { "version", cmd_read_firmware_version,},
};

static void print_usage()
{
	int i, n;
	struct cmd_entry *p = NULL;

	n = ARRAY_SIZE(entrys);
	printf("usage: \n");
	for (i = 0; i < n; i++) {
		p = entrys + i;
		printf("    %s\n", p->cmd);
	}
}


////////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
	struct spi_cfg *p = &cfg_info;
	struct cmd_args args;
	struct cmd_entry *entry = NULL;
	char *cmd = NULL;
	int ret = 0;
	int n = 0, i = 0;

    printf("Version = %s\n", VERSION);

	if (argc < 2) {
		print_usage();
		goto L_END;
	}

	args.fd = open(p->device, O_RDWR);
	if (args.fd < 0)
		pabort("can't open device");

    /* change to 1MHz */
	spi_init_attr(args.fd, p);
	spi_print_cfg(p);

	cmd = argv[1];
	n = ARRAY_SIZE(entrys);
	for (i = 0; i < n; i++) {
		entry = entrys + i;
		if (!strcmp(cmd, entry->cmd)) {
			args.argc = argc - 2;
			args.argv = argv + 2;
			ret = entry->func(&args, entry);
			break;
		}
	}
	if (i == n)
		print_usage();
	else
		printf("ret = %d\n", ret);

    do_log("--- end ---\n");
	close(args.fd);

L_END:
	printf("\n\n");

	return ret;
}
