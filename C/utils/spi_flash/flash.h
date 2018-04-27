#ifndef _FLASH_H_
#define _FLASH_H_

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

uint8_t flash_read_status(int fd);
uint8_t flash_read_sign(int fd);
int flash_read_jedec(int fd);
int flash_page_program(int fd, int addr, uint8_t *buf, int len);
int flash_sector_erase(int fd, int addr);
int flash_bulk_erase(int fd);
int flash_write_status_reg(int fd, uint8_t val);
int flash_read_data(int fd, int addr, uint8_t *buf, int len);
int flash_fast_read(int fd, int addr, uint8_t *buf, int len);
int flash_w25q16_init_write(int fd);

#endif
