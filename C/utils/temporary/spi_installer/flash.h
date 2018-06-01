#ifndef _FLASH_H_
#define _FLASH_H_

#include <stdint.h>
#include <sys/types.h>

#define STS_WIP (1 << 0)  // RO
#define STS_WEL (1 << 1)  // RO
#define STS_BP0 (1 << 2)  // RW
#define STS_BP1 (1 << 3)  // RW
#define STS_BP2 (1 << 4)  // RW
#define STS_SRWD (1 << 7) // RW

struct flash_info {
	int blocks_per_chip;
	int sectors_per_block;
	int pages_per_sector;
	int page_size;  // Byte
};


uint8_t flash_read_sign(int fd);
int flash_read_jedec(int fd);

int flash_write_disable(int fd);
int flash_write_enable(int fd);
uint8_t flash_read_status(int fd);
int flash_page_program(int fd, int addr, uint8_t *buf, int len);
int flash_sector_erase(int fd, int addr);
int flash_bulk_erase(int fd);
int flash_write_status_reg(int fd, uint8_t val);
int flash_read_data(int fd, int addr, uint8_t *buf, int len);
int flash_fast_read(int fd, int addr, uint8_t *buf, int len);

#endif
