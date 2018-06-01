#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <linux/types.h>
#include <errno.h>
#include <poll.h>
#include <linux/soundcard.h>
#include <linux/input.h>
#include <linux/spi/spidev.h>
#include <termios.h>
#include <linux/serial.h>
#include <linux/i2c.h>
#include <stdarg.h>
#include <signal.h>
#include <sys/types.h>
#include <ctype.h>
#include <sys/uio.h>
#include <dirent.h>
#include <pthread.h>
#include <limits.h>
#include <sys/vfs.h>
#include <sys/mount.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <net/ethernet.h>
#include <net/route.h>
#include <arpa/inet.h>
#include <linux/sockios.h>
#include <linux/tcp.h>
#include <linux/ethtool.h>
#include <signal.h>
#include <bits/signum.h>
#include <execinfo.h>
#include <linux/reboot.h>
#include <sys/reboot.h>

#define FALSE 0
#define TRUE 1
#define BOOL int

/*************************************************************************
 * ddc接口读取显示器edid并解析，用于显示器热插拔检测
 *************************************************************************/

/*
 * 只解析了需要的部分，完整的解析可以参考ddc_test.cpp
 */

#define DEBUG

#ifdef DEBUG
#define debug_print printf
#else
#define debug_print
#endif

#define EDID_LEN                  128
#define EDID_HEADER               0x00
#define EDID_HEADER_END           0x07
#define ID_MANUFACTURER_NAME      0x08
#define ID_MANUFACTURER_NAME_END  0x09
#define ID_MODEL                  0x0a
#define ID_SERIAL_NUMBER          0x0c
#define MANUFACTURE_WEEK          0x10
#define MANUFACTURE_YEAR          0x11
#define EDID_STRUCT_VERSION       0x12
#define EDID_STRUCT_REVISION      0x13
#define EDID_STRUCT_DISPLAY       0x14

#define DPMS_FLAGS                0x18
#define ESTABLISHED_TIMING_1      0x23
#define ESTABLISHED_TIMING_2      0x24
#define MANUFACTURERS_TIMINGS     0x25

#define DETAILED_TIMING_DESCRIPTIONS_START      0x36
#define DETAILED_TIMING_DESCRIPTION_1           0x36
#define DETAILED_TIMING_DESCRIPTION_2           0x48
#define DETAILED_TIMING_DESCRIPTION_3           0x5a
#define DETAILED_TIMING_DESCRIPTION_4           0x6c

#define DETAILED_TIMING_DESCRIPTION_SIZE        18
#define NO_DETAILED_TIMING_DESCRIPTIONS         4

#define MONITOR_NAME            0xfc
#define MONITOR_LIMITS          0xfd
#define UNKNOWN_DESCRIPTOR      -1
#define DETAILED_TIMING_BLOCK   -2

#define DESCRIPTOR_DATA         5


#define I2C_RETRIES	0x0701
#define I2C_TIMEOUT	0x0702
#define I2C_SLAVE	0x0703
#define I2C_RDWR	0x0707

#define COMBINE_HI_8LO(hi, lo) ((((unsigned)hi) << 8) | (unsigned)lo)


#define DDC_DEV "/dev/i2c-2"
#define DDC_ADDR 0x6e

#define VENDOR_LEN 4
#define PRODUCT_LEN 14

#define RATIO_UNKNOWN 0
#define RATIO_SIZE 1
#define RATIO_HOR 2
#define RATIO_VER 3

#define ERR_OK 0
#define ERR_HEADER 1
#define ERR_CHECKSUM 2


struct i2c_rdwr_ioctl_data {
	struct i2c_msg *msgs;	/*i2c_msg[]指针*/
	int nmsgs;		/*i2c_msg数量*/
};

struct edid_info {
	unsigned char version;			/* EDID version... */
	unsigned char revision;			/* ...and revision */

	unsigned char manufacturer[4];	/* Manufacturer */
	unsigned char product[14];		/* Monitor String */

	unsigned char serial_no[14];	/* Serial Number */
	unsigned char ascii[14];		/* ? */
	unsigned int model;			    /* Monitor Model */
	unsigned int serial;			/* Serial Number - Integer */
	unsigned int year;		    	/* Year manufactured */
	unsigned int week;		    	/* Week Manufactured */

    int   ratio_type;
	unsigned char hsize;           /* Maximum horizontal size (cm) */
	unsigned char vsize;           /* Maximum vertical size (cm) */
    float landscape_ratio;
    float portait_ratio;

    int err;                     /* parse errno */
};

const char edid_v1_header[] = {0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00};

static struct edid_info EdidInfo = {0};
static unsigned char EdidBuf[EDID_LEN] = {0};
static int Ddcfd = -1;

static BOOL read_ddc(int fd, unsigned char *buf, size_t len, unsigned char addr);
static BOOL probe_ddc_edid(int fd, unsigned char *buf, size_t len);
static BOOL parse_ddc_edid(unsigned char *edid, struct edid_info *pinfo);
#ifdef DEBUG
static void dump_buf(const unsigned char *buf, int len);
#endif

BOOL DDC_Init(const char *devpath)
{
	if ((Ddcfd > 0) || (NULL == devpath)) {
		return FALSE;
	}

    Ddcfd = open(devpath, O_RDWR);
	if (Ddcfd < 0) {
		return FALSE;
	}

	return TRUE;
}

BOOL DDC_Deinit()
{
	if (Ddcfd < 0)
		return FALSE;

    close(Ddcfd);
	Ddcfd = -1;

	return TRUE;
}

static BOOL test_ddcci(int fd)
{
	struct i2c_rdwr_ioctl_data msg_rdwr;
	struct i2c_msg msgs[2];
	int ret = 0;
    unsigned char addr = 0x6e;

    unsigned char send[] = {0x51, 0x81, 0xb1, 0x0f};
    unsigned char receive[6] = {0};

	msgs[0].addr  = addr;
	msgs[0].flags = 0;
	msgs[0].len	  = sizeof(send);
	msgs[0].buf	  = send;

	msgs[1].addr  = addr;
	msgs[1].flags = I2C_M_RD;
	msgs[1].len	  = 6;
	msgs[1].buf	  = receive;

	msg_rdwr.msgs = msgs;
	msg_rdwr.nmsgs = 2;

	ret = ioctl(fd, I2C_RDWR, &msg_rdwr);
	printf("ret = %d\n", ret);
    dump_buf(receive, 6);

	return TRUE;
}

struct edid_info *DDC_ProbeEdid()
{
	unsigned char *buf = EdidBuf;
	int fd = Ddcfd;

	if (fd < 0)
		return NULL;

	memset(buf, 0, EDID_LEN);

#if 1
    test_ddcci(fd);
#else
	if (!probe_ddc_edid(fd, buf, EDID_LEN))
		return NULL;
#endif

	return NULL;
}

static BOOL read_ddc(int fd, unsigned char *buf, size_t len, unsigned char addr)
{
	struct i2c_rdwr_ioctl_data msg_rdwr;
	struct i2c_msg msgs[1];
	int ret = 0;

    printf("len = %d\n", len);

	msgs[0].addr  = addr;
	msgs[0].flags = 0;
	msgs[0].len	  = len;
	msgs[0].buf	  = buf;

	msg_rdwr.msgs = msgs;
	msg_rdwr.nmsgs = 1;

	if ( (ret = ioctl(fd, I2C_RDWR, &msg_rdwr)) != 1) {
#ifdef DEBUG
		perror("i2c_read ioctl()");
		debug_print("errno=%d, ret = %d\n", errno, ret);
#endif
		return FALSE;
	}

	return TRUE;
}

static BOOL probe_ddc_edid(int fd, unsigned char *buf, size_t len)
{
	struct i2c_rdwr_ioctl_data msg_rdwr;
	unsigned char start = 0x0;
	struct i2c_msg msgs[2];
	int ret = 0;

	msgs[0].addr  = DDC_ADDR;
	msgs[0].flags = 0;
	msgs[0].len	  = 1;
	msgs[0].buf	  = &start;

	msgs[1].addr  = DDC_ADDR;
	msgs[1].flags = I2C_M_RD;
	msgs[1].len	  = len;
	msgs[1].buf	  = buf;

	msg_rdwr.msgs = msgs;
	msg_rdwr.nmsgs = 2;

	if ( (ret = ioctl(fd, I2C_RDWR, &msg_rdwr)) != 2) {
#ifdef DEBUG
		perror("i2c_read ioctl()");
		debug_print("errno=%d, ret = %d\n", errno, ret);
#endif
		return FALSE;
	}

	return TRUE;
}

static int edid_checksum(unsigned char *edid)
{
	unsigned char csum = 0;
	int i, ok = 0;

	for (i = 0; i < EDID_LEN; i++)
		csum += edid[i];
    debug_print("checksum = 0x%02x\n", csum);

	/* checksum passed, everything's good */
	if (csum == 0x00)
		ok = 1;

	return ok;
}

static int edid_check_header(unsigned char *edid)
{
    int ret = 0;

    if (!strncmp((const char *)(edid + EDID_HEADER), (const char *)edid_v1_header, EDID_HEADER_END + 1)) {
        ret = 1;
    }

    return ret;
}

static void parse_vendor_block(unsigned char *block, struct edid_info *pinfo)
{
	pinfo->manufacturer[0] = ((block[0] & 0x7c) >> 2) + '@';
	pinfo->manufacturer[1] = ((block[0] & 0x03) << 3) +
		((block[1] & 0xe0) >> 5) + '@';
	pinfo->manufacturer[2] = (block[1] & 0x1f) + '@';
	pinfo->manufacturer[3] = 0;
	pinfo->model = block[2] + (block[3] << 8);
	pinfo->serial = block[4] + (block[5] << 8) +
	       (block[6] << 16) + (block[7] << 24);
	pinfo->year = block[9] + 1990;
	pinfo->week = block[8];
}

static int edid_is_serial_block(unsigned char *block)
{
	if ((block[0] == 0x00) && (block[1] == 0x00) &&
	    (block[2] == 0x00) && (block[3] == 0xff) &&
	    (block[4] == 0x00))
		return 1;
	else
		return 0;
}

static int edid_is_ascii_block(unsigned char *block)
{
	if ((block[0] == 0x00) && (block[1] == 0x00) &&
	    (block[2] == 0x00) && (block[3] == 0xfe) &&
	    (block[4] == 0x00))
		return 1;
	else
		return 0;
}

static int edid_is_monitor_block(unsigned char *block)
{
	if ((block[0] == 0x00) && (block[1] == 0x00) &&
	    (block[2] == 0x00) && (block[3] == 0xfc) &&
	    (block[4] == 0x00))
		return 1;
	else
		return 0;
}

static void copy_string(unsigned char *c, unsigned char *s)
{
    int i;

    c = c + 5;
    for (i = 0; (i < 13 && *c != 0x0A); i++)
        *(s++) = *(c++);
    *s = 0;
    while (i-- && (*--s == 0x20)) *s = 0;
}

/*
 * 解析屏幕尺寸、分辨率
 */
static void get_monspecs(unsigned char *edid, struct edid_info *pinfo)
{
	unsigned char *block = NULL;

	block = edid + EDID_STRUCT_DISPLAY;

	pinfo->hsize = block[1];
	pinfo->vsize = block[2];
    if (pinfo->hsize == 0 && pinfo->vsize == 0) {
        pinfo->ratio_type = RATIO_UNKNOWN;
    } else if (pinfo->hsize && pinfo->vsize) {
        pinfo->ratio_type = 1;
    } else if (pinfo->hsize) {
        pinfo->ratio_type = 2;
        pinfo->landscape_ratio = pinfo->hsize * ((3.54 - 1) / 255) + 1;
        pinfo->hsize = 0;
    } else if (pinfo->vsize) {
        pinfo->ratio_type = 3;
        pinfo->portait_ratio = pinfo->vsize * ((0.99 - 0.28) / 255) + 0.28;
        pinfo->vsize = 0;
    }
}

static BOOL parse_ddc_edid(unsigned char *edid, struct edid_info *pinfo)
{
    unsigned char *block;
    int i;

    if (edid == NULL || pinfo == NULL)
        return FALSE;

    // some monitor can not pass this test, so ignore it
    if (!(edid_checksum(edid))) {
    	debug_print("EDID checksum failed.\n");
    }

    if (!(edid_check_header(edid))) {
    	debug_print("Don't match EDID version 1 header\n");
        pinfo->err = ERR_HEADER;
        return FALSE;
    }

    memset(pinfo, 0, sizeof(struct edid_info));

    pinfo->version = edid[EDID_STRUCT_VERSION];
    pinfo->revision = edid[EDID_STRUCT_REVISION];

    parse_vendor_block(edid + ID_MANUFACTURER_NAME, pinfo);

    block = edid + DETAILED_TIMING_DESCRIPTIONS_START;
    for (i = 0; i < 4; i++, block += DETAILED_TIMING_DESCRIPTION_SIZE) {
        if (edid_is_serial_block(block)) {
            copy_string(block, pinfo->serial_no);
        } else if (edid_is_ascii_block(block)) {
            copy_string(block, pinfo->ascii);
        } else if (edid_is_monitor_block(block)) {
            copy_string(block, pinfo->product);
        }
    }

    get_monspecs(edid, pinfo);
    pinfo->err = ERR_OK;

    return TRUE;
}

#ifdef DEBUG
static void dump_buf(const unsigned char *buf, int len)
{
    int i = 0;

	printf("--------------------------------");
	for (i = 0; i < len; i++) {
		if (i % 16 == 0)
			printf("\n");
		printf("0x%02x ", buf[i]);
	}
	printf("\n--------------------------------\n");
}
#endif

static void dump_edidinfo(struct edid_info *pinfo)
{
    printf("========================================\n");
    printf("Display Information (EDID)\n");
    printf("========================================\n");
    printf("   EDID Version %d.%d\n", (int)pinfo->version,
            (int)pinfo->revision);

	printf("   Manufacturer: %s\n", pinfo->manufacturer);
    printf("   Product Name: %s\n", pinfo->product);

	printf("   Model: %x\n", pinfo->model);
	printf("   Serial#: %u\n", pinfo->serial);
	printf("   Year: %u Week: %u\n", pinfo->year, pinfo->week);

    printf("   Serial Number: %s\n", pinfo->serial_no);
    printf("   ASCII Block: %s\n", pinfo->ascii);

    switch (pinfo->ratio_type) {
        case RATIO_SIZE:
            debug_print("   H-size in cm: %d\n", pinfo->hsize);
            debug_print("   V-size in cm: %d\n", pinfo->vsize);
            break;
        case RATIO_HOR:
            debug_print("   Landscape Ratio = %f\n", pinfo->landscape_ratio);
            break;
            break;
        case RATIO_VER:
            debug_print("   Portrait Ratio = %f\n", pinfo->portait_ratio);
            break;
        case RATIO_UNKNOWN:
            debug_print("   Screen size and ratio are unkonwn\n");
            break;
    }
    debug_print("========================================\n");
}

static void print_usage()
{
	printf(" usage:\n"
	 " ./fb_test probe\n"
	 " ./fb_test probe 5\n"
			);
}

int main(int argc, char *argv[])
{
    struct edid_info *pinfo = NULL;
    int interval = 0;
    int ret = 0;

	char a[] = {0x00, 0x02};
	char b[] = {0x00, 0x02};

	//int i = strncmp(a, b, 2);
	int i = memcmp(a, b, 2);
	printf("ret = %d\n", i);
	return 0;

    if (argc == 1) {
        DDC_Init(DDC_DEV);
        DDC_ProbeEdid();
        DDC_Deinit();
        return 0;
    } else
        print_usage();

    return 0;
}
