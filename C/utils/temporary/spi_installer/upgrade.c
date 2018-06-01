#include "flash.h"
#include "spidev.h"
#include "upgrade.h"
#include "crc.h"
#include "bridge.h"
#include "ui.h"

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

static int GetFileSize(char *name)
{
	if (NULL == name)
		return -1;

	struct stat st;
	if (stat(name, &st) < 0) {
		perror("stat failed");
		return -1;
	}

	return st.st_size;
}

static uint8_t *load_check_firmware(char *path, int *psize, int range_len)
{
    uint8_t *buf = NULL;
    int size = 0;
    int fd = 0;

    size = GetFileSize(path);
    if (size == -1) {
        perror("get file size failed");
        return NULL;
    }

    if (size > range_len || size == 0) {
        printf(">>> size of firmware [%d] is larger than range [%d]\n",
                size, range_len);
        return NULL;
    }

    *psize = size;
    buf = malloc(size);
    if (buf == NULL) {
        perror("malloc failed");
        return NULL;
    }
    memset(buf, 0, size);
    
    fd = open(path, O_RDONLY);
    if (fd < 0) {
        perror("open failed");
        free(buf);
        return NULL;
    }

    size = read(fd, buf, size);
    if (size != *psize) {
        perror("read firmware failed");
        free(buf);
        close(fd);
        return NULL;
    }

    close(fd);

    return buf;
}

static int send_upgrade_cmd(int fd)
{
    int dep = 0;
    int ret = 0;
    unsigned char *buf = NULL;
    unsigned char *pr = NULL;
    uint8_t cmd[] = {0xfb, 0x81, 0x00, 0x00, 0x04, 0x22, // L1 router
        0x50, 0xff, 0xff, 0x7b}; // upgrade cmd
    uint8_t reply[] = {0x51, 0xff, 0xff, 0x3d};

    read_tx_dep(fd);

    send_data(fd, cmd, sizeof(cmd));

    dep = read_rx_dep(fd);
    if (dep < 0) {
        printf("read rx dep failed\n");
        return -1;
    }

    buf = malloc(dep);
    if (buf == NULL) {
        perror("malloc failed");
        return -2;
    }
    memset(buf, 0, dep);

    ret = recv_data(fd, buf, dep);
    if (ret < 0) {
        printf("recv data failed\n");
        free(buf);
        return -3;
    }

    pr = find_vir_dev_reply(buf, dep);
    if (pr == NULL) {
        printf("can't find vritual device's reply\n");
        free(buf);
        return -4;
    }

    if (memcmp(pr, reply, sizeof(reply))) {
        printf("replay isn't match\n");
        printf("%x %x %x %x\n", pr[0], pr[1], pr[2], pr[3]);
        free(buf);
        return -5;
    }

    free(buf);
    return 0;
}

int read_spi_version(int fd)
{
    int dep = 0;
    int ret = 0;
    unsigned char *buf = NULL;
    unsigned char *pr = NULL;
    uint8_t cmd[] = {0xfb, 0x81, 0x00, 0x00, 0x04, 0x22,
        0x00, 0xff, 0xff, 0x2d};

    dep = read_tx_dep(fd);

    send_data(fd, cmd, sizeof(cmd));

    dep = read_rx_dep(fd);
    if (dep < 0) {
        printf("read rx dep failed\n");
        return -1;
    }

    buf = malloc(dep);
    if (buf == NULL) {
        perror("malloc failed");
        return -2;
    }
    memset(buf, 0, dep);

    ret = recv_data(fd, buf, dep);
    if (ret < 0) {
        printf("recv data failed\n");
        free(buf);
        return -3;
    }

    pr = find_vir_dev_reply(buf, dep);
    if (pr == NULL) {
        printf("can't find vritual device's reply\n");
        free(buf);
        return -4;
    }

    printf("version: %02x %02x\n", pr[1], pr[2]);

    free(buf);
    return 0;
}

static int is_switch_ok(int fd)
{
    int wel = 0;
    uint8_t status = 0;

    printf("Flash signature: 0x%x\n", flash_read_sign(fd));
    printf("JEDEC: 0x%x\n", flash_read_jedec(fd));

    status = flash_read_status(fd);
    wel = status & STS_WEL;
    usleep(50 * 1000);
    wel ? flash_write_disable(fd): flash_write_enable(fd);
    status = flash_read_status(fd);
    if (wel == (status & STS_WEL)) {
        printf("test WEL failed.\n");
        return -1;
    }

    wel ? flash_write_enable(fd): flash_write_disable(fd);
    usleep(50 * 1000);
    status = flash_read_status(fd);
    if (wel != (status & STS_WEL)) {
        printf("test WEL failed 2.\n");
        return -1;
    }

    return 0;
}

// write firmware to flash in a sector page by page
static int write_sector(int fd, int start, uint8_t *buf, int count, int page_size)
{
	int left = 0;
	int offset = 0;
	int len = 0;
	int next_page_offset = 0;
    int ret = 0;
    int i = 0;
    uint8_t *tbuf = NULL;

	left = count;
	offset = start;
	next_page_offset = (offset / page_size + 1) * page_size;
	while (left > 0) {
		len = left > page_size ? next_page_offset - offset: left;

        printf("       [%d] page program page from 0x%x, size 0x%x    \r", i++, offset, len);
        fflush(stdout);
#if TEST_S_PAGE
        if (len < page_size) {
            int s = page_size;
            tbuf = malloc(s);
            memset(tbuf, 0x55, s);
            memcpy(tbuf, buf, len);
            ret = flash_page_program(fd, offset, tbuf, s);
            free(tbuf);
        } else
#endif
            ret = flash_page_program(fd, offset, buf, len);
		if (ret) {
			printf("page program failed\n");
			return -1;
		}

        buf += len;
		offset += len;
		left -= len;
		next_page_offset += page_size;
	}
    printf("\n");

    return 0;
}

static int read_sector(int fd, int start, uint8_t *buf, int count, int spidev_size)
{
    int left = 0;
    int offset = 0;
    int len = 0;
    int next_offset = 0;
    int ret = 0;
    int i = 0;

    left = count;
    offset = start;
    next_offset = (offset / spidev_size + 1) * spidev_size;
    while (left > 0) {
		len = left > spidev_size ? next_offset - offset : left;

        printf("       [%d] read from 0x%x, size = 0x%x to %p   \r", i++, offset, len, buf);
        fflush(stdout);
        ret = flash_fast_read(fd, offset, buf, len);
        if (ret < 0) {
            printf("flash_fast_read failed.\n");
            return -1;
        }

        buf += len;
        offset += len;
        left -= len;
        next_offset += spidev_size;
    }
    printf("\n");

    return 0;
}


static int check_sector(int fd, int start, uint8_t *buf, int count, int spidev_size)
{
    int ret = 0;
    uint8_t crc1, crc2;
    uint8_t *fbuf = NULL;

    fbuf = malloc(count);
    if (fbuf == NULL) {
        perror("malloc failed");
        return -1;
    }
    memset(fbuf, 0, count);

    ret = read_sector(fd, start, fbuf, count, spidev_size);
    if (ret < 0) {
        printf("fast read sector failed.\n");
        free(fbuf);
        return -1;
    }
    crc1 = getCRC8(buf, count);
    crc2 = getCRC8(fbuf, count);

    if (crc1 != crc2) {
        printf("crc check failed. 0x%x / 0x%x\n", crc1, crc2);
        free(fbuf);
        return -1;
    }

    free(fbuf);
    return 0;
}

// flash firmware to flash sector by sector
static int write_firmware(int fd, int start, uint8_t *buf, int count, int sector_size, int page_size)
{
    int left = 0;
    int offset = 0;
    int len = 0;
    int next_sector_offset = 0;
    int ret = 0;
    int i = 0;

    left = count;
    offset = start;
    next_sector_offset = (offset / sector_size + 1) * sector_size;
    // sector by sector
    while (left > 0) {
		len = left > sector_size ? next_sector_offset - offset : left;

        printf("   [%d] handle sector from 0x%x, len = 0x%x\n", i++, offset, len);
        ret = flash_sector_erase(fd, offset);
        if (ret) {
            printf("erase sector failed.\n");
            return -1;
        }

        ret = write_sector(fd, offset, buf, len, page_size);
        if (ret) {
            printf("write secotr failed.\n");
            return -1;
        }

        ret = check_sector(fd, offset, buf, len, SPIDEV_DRIVER_BUF_SIZE);
        if (ret) {
            printf("check sector failed.\n");
            return -1;
        }

        buf += len;
        offset += len;
        left -= len;
        next_sector_offset += sector_size;
    }

    return 0;
}

/*
 * @fd: fd for spi controller
 * @path: firmware's path
 * @start: start of range in flash for firmware
 * @range_len: length of range in flash for firmware
 * @page_size: page size of flash
 * @sector_size: sector size of flash.
 */
int upgrade_firmware(int fd, char *path, int start, int range_len, int page_size, int sector_size)
{
    int ret = 0;
    uint8_t *buf = NULL;
    int file_size = 0;

    /* 1. load fireware */
    buf = load_check_firmware(path, &file_size, range_len);
    if (buf == NULL) {
        errlog("\n>>> load firmware failed\n\n");
        return -1;
    }
    goodlog("\n>>> 1. load firmware [%s] success.\n\n", path);

    /* 2. send upgrade command to bridge device */
    ret = send_upgrade_cmd(fd);
    if (ret) {
        errlog("\n>>> send_upgrade_cmd failed\n\n");
        ret = -2;
        goto L_END;
    }
    goodlog("\n>>> 2. send upgrade command to bridge success.\n\n");

    /* 3. check if switch success */
    ret = is_switch_ok(fd);
    if (ret) {
        errlog("\n>>> switch check failed.\n\n");
        ret = -3;
        goto L_END;
    }
    goodlog("\n>>> 3. switch check success\n\n");

    /* 4. erase and write firmware into flash */
    ret = write_firmware(fd, start, buf, file_size, sector_size, page_size);
    if (ret) {
        errlog("\n>>> write_firmware failed.\n\n");
        ret = -4;
        goto L_END;
    }
    goodlog("\n>>> 4. write firmware to flash success.\n\n");

L_END:
    free(buf);

    return ret;
}

int upgrade_firmware_flash_only(int fd, char *path, int start, int range_len, int page_size, int sector_size)
{
    int ret = 0;
    uint8_t *buf = NULL;
    int file_size = 0;

    /* 1. load fireware */
    buf = load_check_firmware(path, &file_size, range_len);
    if (buf == NULL) {
        errlog("\n>>> load firmware failed\n\n");
        return -1;
    }
    goodlog("\n>>> 1. load firmware [%s] success.\n\n", path);

    /* 4. check if switch success */
    ret = is_switch_ok(fd);
    if (ret) {
        errlog("\n>>> switch check failed.\n\n");
        ret = -3;
        goto L_END;
    }
    goodlog("\n>>> 3. switch check success\n\n");

    /* 5. erase and write firmware into flash */
    ret = write_firmware(fd, start, buf, file_size, sector_size, page_size);
    if (ret) {
        errlog("\n>>> write_firmware failed.\n\n");
        ret = -4;
        goto L_END;
    }
    goodlog("\n>>> 4. write firmware to flash success.\n\n");

L_END:
    free(buf);

    return ret;
}

int bridge_switch(int fd)
{
    int ret = 0;

    ret = send_upgrade_cmd(fd);
    if (ret) {
        errlog("\n>>> send_upgrade_cmd failed\n\n");
        return -2;
    }
    goodlog("\n>>> 2. send upgrade command to bridge success.\n\n");

    ret = is_switch_ok(fd);
    if (ret) {
        errlog("\n>>> switch check failed.\n\n");
        return -3;
    }
    goodlog("\n>>> 3. switch check success\n\n");

    return ret;
}

static int read_whole_flash(int fd, int start, uint8_t *buf, int count)
{
    int i = 0, j = 0;
    int sector_size =  256 * 256;
    int page_size = 256;
    uint8_t *p = NULL;
    int offset = 0;

    for (i = 0; i < 8; i++) {
        p = buf + sector_size * i;
        offset = sector_size * i;
        for (j = 0; j < 256; j++) {
            flash_read_data(fd, offset + page_size * j, p + page_size * j, page_size);
        }
    }

    return 0;
}

int flash_read_to_file(int fd, char *name)
{
    uint8_t *buf = NULL;
    int size = 512 * 1024;
    int binfd;

    printf("read whole flash to %s\n", name);
    buf = malloc(size);
    if (buf == NULL) {
        perror("malloc failed");
        return -1;
    }
    memset(buf, 0, size);

    binfd = open(name, O_CREAT | O_TRUNC | O_RDWR);
    if (binfd < 0) {
        perror("open failed.");
        free(buf);
        return -1;
    }
    read_whole_flash(fd, 0, buf, size);
    write(binfd, buf, size);
    close(binfd);
    
    free(buf);
    return 0;
}


