/*
 * drivers/misc/max1749.c
 *
 * Driver for MAX1749, my_test motor driver.
 *
 * Copyright (c) 2011-2013 NVIDIA Corporation, All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <linux/module.h>
#include <linux/regulator/consumer.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/err.h>
#include <linux/hrtimer.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/gpio.h>

static int irq_type = 1;
static int gpio = 2;
static int irq = 0;

static irqreturn_t irq_handler(int irq, void *dev_id)
{
	printk("zhs: irq hannpens\n");
	return IRQ_HANDLED;
}

static int my_test_probe(struct platform_device *pdev)
{
	int ret;
	int irq_flag;
	int irq_gpio = 178;

	if (gpio == 2) {
		irq_gpio = 178; // GPIO PW2
	} else if (gpio == 3) {
		irq_gpio = 179; // GPIO PW3
	}
	printk("irq gpio choose is %d\n", irq_gpio);
	irq = gpio_to_irq(irq_gpio);

	switch (irq_type) {
		case 1:
			printk("choose low trigger\n");
			irq_flag = IRQF_ONESHOT | IRQF_TRIGGER_LOW;
			break;
		case 2:
			printk("choose high trigger\n");
			irq_flag = IRQF_ONESHOT | IRQF_TRIGGER_HIGH;
			break;
		case 3:
			printk("choose falling trigger\n");
			irq_flag = IRQF_ONESHOT | IRQF_TRIGGER_FALLING;
			break;
		case 4:
			printk("choose rising trigger\n");
			irq_flag = IRQF_ONESHOT | IRQF_TRIGGER_RISING;
			break;
		default:
			printk("invalid irq type\n");
			return -EINVAL;
	}

	ret = request_irq(irq, irq_handler, irq_flag, "my test", NULL);
	if (ret) {
		printk("Failed to get host_wake irq\n");
		return ret;
	}
	printk("probe my_test\n");

	return 0;
}

static int my_test_remove(struct platform_device *pdev)
{
	if (irq)
		free_irq(irq, NULL);
	printk("remove my_test\n");

	return 0;
}

static struct platform_driver my_test_driver = {
	.probe = my_test_probe,
	.remove = my_test_remove,
	.driver = {
		.name = "my-test",
		.owner = THIS_MODULE,
	},
};

static int __init my_test_init(void)
{
	return platform_driver_register(&my_test_driver);
}

static void __exit my_test_exit(void)
{
	platform_driver_unregister(&my_test_driver);
}

MODULE_DESCRIPTION("my_test device");
MODULE_AUTHOR("zhs");
MODULE_LICENSE("GPL");

module_init(my_test_init);
module_exit(my_test_exit);
module_param(gpio, int, 0644);
module_param(irq_type, int, 0644);
