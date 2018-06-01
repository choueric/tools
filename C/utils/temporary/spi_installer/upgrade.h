#ifndef _UPGRADE_H
#define _UPGRADE_H_

/*
 * @fd: fd for spi controller
 * @path: firmware's path
 * @start: start of range in flash for firmware
 * @range_len: length of range in flash for firmware
 * @page_size: page size of flash
 * @sector_size: sector size of flash.
 */
int upgrade_firmware(int fd, char *path, int start, int range_len, int page_size, int sector_size);

int read_spi_version(int fd);
int bridge_switch(int fd);
int upgrade_firmware_flash_only(int fd, char *path, int start, int range_len, int page_size, int sector_size);
int flash_read_to_file(int fd, char *name);
#endif
