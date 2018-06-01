/*
 * gpio_common.h
 *
 *  Created on: 2014-7-1
 *      Author: 50215238
 */

#ifndef GPIO_COMMON_H_
#define GPIO_COMMON_H_

#define GPIO_NAME_LEN 15

enum GPIO_BANK_WIDTH
{
	GPIO_BANK_WIDTH_16 = 16,
	GPIO_BANK_WIDTH_32 = 32,
};

enum GPIO_DIR {
	DIR_OUT = 0,
	DIR_IN,
	DIR_ERR,
};

enum gpio_type {
	GPIO_TYPE_DEVFS,
	GPIO_TYPE_SYSFS,
    GPIO_TYPE_SPIBRG,
	GPIO_TYPE_INVALID,
};

struct gpio_dev {
	struct gpio_ops *ops;
	char name[GPIO_NAME_LEN + 1];
	const char *path;  // path of dev node or sys node
	enum gpio_type type;
	GPIO_BANK_WIDTH bank_width;
};

/*
 * TODO: the callback should have a parameter which is struct gpio_ops *,
 * but there is only on instance, so ...
 */
struct gpio_ops {
	int (* init)(struct gpio_dev *dev, int pin);
	int (* set_dir)(struct gpio_dev *dev, int pin, enum GPIO_DIR dir);
	enum GPIO_DIR (* get_dir)(struct gpio_dev *dev, int pin);
	/* @return, 1 is high; 0 is low; < 0 is error */
	int (* get_val)(struct gpio_dev *dev, int pin);
	int (* set_val)(struct gpio_dev *dev, int pin, int val);
	int (* setup)(struct gpio_dev *dev);
	int (* release)(struct gpio_dev *dev);

	enum gpio_type type;
};

void InitGpioAdapters();
struct gpio_dev *GpioSetup(const char *name, const char *path, enum gpio_type type, GPIO_BANK_WIDTH bank_width);
int GpioRelease(struct gpio_dev *dev);

////////////////////////////////////////////////////////////////////////////////

struct gpio_dev *GetGpioDev();
void SetGpioDev(struct gpio_dev *dev);


/**************************************************
 * add new type of gpio operatons here
 *************************************************/
/* devfs type operations */
struct gpio_ops *Get_gpio_devfs_ops();
void Init_gpio_devfs_ops();

/* sysfs type operations */
void Init_gpio_sysfs_ops();
struct gpio_ops *Get_gpio_sysfs_ops();

/* spi bridge type operations */
void Init_gpio_spibrg_ops();
struct gpio_ops *Get_gpio_spibrg_ops();

#endif /* GPIO_COMMON_H_ */
