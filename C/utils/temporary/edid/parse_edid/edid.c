#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/types.h>
#include <errno.h>
#include "edid.h"

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

#define COMBINE_HI_8LO(hi, lo) ((((unsigned)hi) << 8) | (unsigned)lo)

#define VENDOR_LEN 4
#define PRODUCT_LEN 14

#define RATIO_UNKNOWN 0
#define RATIO_SIZE 1
#define RATIO_HOR 2
#define RATIO_VER 3

#define ERR_OK 0
#define ERR_HEADER 1
#define ERR_CHECKSUM 2

const unsigned char edid_v1_header[] = {0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00};

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

/* return 0 if ok */
static int edid_check_header(unsigned char *edid)
{
    int ret = 0;
	int i = 0;
	const unsigned char *p = NULL;
	const unsigned char *q = NULL;

	for (i = 0; i <= EDID_HEADER_END; i++) {
		p = edid + EDID_HEADER + i;
		q = edid_v1_header + i;
		if (*p != *q) {
			ret = 1;
			break;
		}
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

int parse_ddc_edid(unsigned char *edid, struct edid_info *pinfo)
{
    unsigned char *block;
    int i;

    if (edid == NULL || pinfo == NULL)
        return -1;

    // some monitor can not pass this test, so ignore it
    if (!(edid_checksum(edid))) {
    	debug_print("EDID checksum failed.\n");
    }

    if (edid_check_header(edid)) {
    	debug_print("Don't match EDID version 1 header\n");
        pinfo->err = ERR_HEADER;
        return -1;
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

    return 0;
}

void dump_edidinfo(struct edid_info *pinfo)
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
