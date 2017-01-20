/* * Copyright (c) 2012 The Linux Foundation. All rights reserved.* */
/* Copyright (c) 2012, Code Aurora Forum. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */


#ifndef __REGULATOR_IPQ_H
#define __REGULATOR_IPQ_H

struct regulator_init_data;

/**
 * struct ipq_reg_config - ipq_reg_config structure
 * @supply_name:	Name of the regulator supply
 * @microvolts:		Output voltage of regulator
 * @gpio:		GPIO to use for enable control
 * 			set to -EINVAL if not used
 * @startup_delay:	Start-up time in microseconds
 * @enabled_at_boot:	Whether regulator has been enabled at
 * 			boot or not. 1 = Yes, 0 = No
 * 			This is used to keep the regulator at
 * 			the default state
 * @init_data:		regulator_init_data
 *
 * This structure contains dummy regulator configuration
 * information that must be passed by platform code to the
 * dummy regulator driver.
 */
struct ipq_reg_config {
	const char *supply_name;
	int microvolts;
	unsigned startup_delay;
	unsigned enabled_at_boot:1;
	struct regulator_init_data *init_data;
};

struct regulator_consumer_supply;

struct platform_device *regulator_register_ipq_dummy(int id,
		struct regulator_consumer_supply *supplies, int num_supplies, int min_uV, int max_uV);

#endif
