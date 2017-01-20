/*
 *  Atheros AR71XX/AR724X/AR913X I2C controller device
 *
 *  Copyright (c) 2013 The Linux Foundation. All rights reserved.
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 */
#ifndef _ATH79_DEV_I2C_H_
#define _ATH79_DEV_I2C_H_

void ath79_register_i2c(struct i2c_gpio_platform_data *pdata,
			struct i2c_board_info const *info,
			unsigned int n);

#endif /* _ATH79_DEV_I2C_H_ */
