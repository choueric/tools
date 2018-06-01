/*
 * flash_common.h
 *
 *  Created on: 2014-7-9
 *      Author: 50215243
 */

#ifndef FLASH_COMMON_H_
#define FLASH_COMMON_H_
#include "FlashAccess.h"

#define FLASH_NAME_LEN 15
#define FOUR_BYTES_MODE 4
#define THREE_BYTES_MODE 3

enum flash_type
{
	FLASH_TYPE_SPI,
	FLASH_TYPE_MTD,
	FLASH_TYPE_INVALID,
};

struct flash_dev
{
	struct flash_ops *ops;
	char name[FLASH_NAME_LEN + 1];
	const char *path;
	int fd;
	int sector_size;
	int sectors;
	int pagesize;
	int addr_mode;
	enum flash_type type;
};

struct flash_ops
{
	BOOL (*init )(struct flash_dev* dev);
	BOOL (*release)(struct flash_dev* dev);
	BOOL (*getinfo)(struct flash_dev* dev, SPIFLASHNIFO* dev_info);
	UINT32 (*read)(struct flash_dev* dev, UINT32 start_addr, UCHAR *buffer, UINT32 size);
	UINT32 (*write)(struct flash_dev* dev, UINT32 start_addr, UCHAR *buffer, UINT32 size);
	BOOL (*erase)(struct flash_dev* dev, UINT32 addr, UINT32 len);
	UINT32 (*block_read)(struct flash_dev* dev, UINT32 block_index, UINT32 offset, UINT32 size, UCHAR *buffer);
	UINT32 (*block_write)(struct flash_dev* dev, UINT32 block_index, UINT32 offset,  UINT32 size, UCHAR *buffer);
	BOOL (*block_erase)(struct flash_dev* dev, UINT32 block_index);
};

VOID InitFlashAdapters();
VOID Init_spi_flash_ops();
struct flash_ops *Get_spi_flash_ops();
VOID Init_flash_mtd_ops();
struct flash_ops *Get_flash_mtd_ops();

struct flash_dev *Flash_Setup(const char *name, const char *path, int sector_size, int sectors, int page_size, int addr_mode, enum flash_type type);

#endif /* FLASH_COMMON_H_ */
