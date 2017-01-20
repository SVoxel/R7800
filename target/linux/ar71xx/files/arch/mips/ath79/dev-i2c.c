/*
 *  Atheros AR71XX/AR724X/AR913X I2C controller device
 *
 *  Copyright (c) 2013 The Linux Foundation. All rights reserved.
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 */

#include <linux/i2c.h>
#include <linux/i2c-gpio.h>
#include <linux/platform_device.h>

static struct platform_device ath79_i2c_gpio_device = {
	.name		= "i2c-gpio",
	.id		= 0,
};

void __init ath79_register_i2c(struct i2c_gpio_platform_data *pdata,
				struct i2c_board_info const *info,
				unsigned int n)
{
	i2c_register_board_info(0, info, n);
	ath79_i2c_gpio_device.dev.platform_data = pdata;
	platform_device_register(&ath79_i2c_gpio_device);
}
