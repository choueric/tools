/*
 * eeprom_common.h
 *
 *  Created on: 2014-11-26
 *      Author: 50215238
 */

#ifndef EEPROM_COMMON_H_
#define EEPROM_COMMON_H_

#define EEPROM_DEV_NAME_LEN 15

enum eeprom_type {
	EEPROM_TYPE_CAT24C,
};

struct eeprom_dev {
	enum eeprom_type type;
	char name[EEPROM_DEV_NAME_LEN + 1];
	struct eeprom_ops *ops;
	const char *path;
	uint16 addr;
	unsigned int page_size;
	unsigned int chip_size;
};

struct eeprom_ops {
    int (* setup)(struct eeprom_dev *dev);
    int (* release)(struct eeprom_dev *dev);

	int (* read)(struct eeprom_dev *dev, int offset, uint8 *buf, int count);
	int (* write)(struct eeprom_dev *dev, int offset, uint8 *buf, int count);

	enum eeprom_type type;
};

void InitEepromAdapters();

struct eeprom_dev *EepromSetup(const char *name, const char *path, uint16 addr, enum eeprom_type type);
int EepromRelease(struct eeprom_dev *dev);

////////////////////////////////////////////////////////////////////////////////

void Init_eeprom_cat24c_ops();
struct eeprom_ops *Get_eeprom_cat24c_ops();

#endif /* EEPROM_COMMON_H_ */
