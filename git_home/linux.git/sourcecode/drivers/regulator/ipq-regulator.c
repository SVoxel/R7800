/*
 * Copyright (c) 2010-2013, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/err.h>
#include <linux/mutex.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/regulator/driver.h>
#include <linux/regulator/ipq-regulator.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/regulator/machine.h>

struct ipq_regulator_data {
	struct ipq_reg_config cfg;
	struct regulator_init_data init_data;
	struct platform_device pdev;
};

static void regulator_ipq_release(struct device *dev)
{
	struct ipq_regulator_data *data = container_of(dev,
			struct ipq_regulator_data, pdev.dev);
	kfree(data);
}

struct ipq_drv_data {
	struct regulator_desc desc;
	struct regulator_dev *dev;
	int min_uV;
	int max_uV;
	unsigned startup_delay;
	bool is_enabled;
};

static int ipq_fixed_is_enabled(struct regulator_dev *dev)
{
	struct ipq_drv_data *data = rdev_get_drvdata(dev);

	return data->is_enabled;
}

static int ipq_fixed_enable(struct regulator_dev *dev)
{
	struct ipq_drv_data *data = rdev_get_drvdata(dev);
	data->is_enabled = true;

	return 0;
}

static int ipq_fixed_disable(struct regulator_dev *dev)
{
	struct ipq_drv_data *data = rdev_get_drvdata(dev);
	data->is_enabled = false;

	return 0;
}

static int ipq_fixed_enable_time(struct regulator_dev *dev)
{
	struct ipq_drv_data *data = rdev_get_drvdata(dev);

	return data->startup_delay;
}

static int ipq_fixed_get_voltage(struct regulator_dev *dev)
{
	struct ipq_drv_data *data = rdev_get_drvdata(dev);
	return data->min_uV;
}

static int ipq_fixed_set_voltage(struct regulator_dev *dev,
					int min_uV, int max_uV,
					unsigned *selector)
{
	struct ipq_drv_data *data = rdev_get_drvdata(dev);
	data->min_uV = min_uV;
	data->max_uV = max_uV;
	return 0;
}

static int ipq_fixed_list_voltage(struct regulator_dev *dev,
				unsigned selector)
{
	struct ipq_drv_data *data = rdev_get_drvdata(dev);

	if (selector != 0)
		return -EINVAL;

	return data->min_uV;
}

static struct regulator_ops ipq_fixed_ops = {
	.is_enabled = ipq_fixed_is_enabled,
	.enable = ipq_fixed_enable,
	.disable = ipq_fixed_disable,
	.enable_time = ipq_fixed_enable_time,
	.get_voltage = ipq_fixed_get_voltage,
	.list_voltage = ipq_fixed_list_voltage,
	.set_voltage = ipq_fixed_set_voltage,
};

static int __devinit reg_ipq_fixed_probe(struct platform_device *pdev)
{
	struct ipq_reg_config *config;
	struct ipq_drv_data *drvdata;
	int ret;

	config = pdev->dev.platform_data;

	if (!config)
		return -ENOMEM;

	drvdata = kzalloc(sizeof(struct ipq_drv_data), GFP_KERNEL);
	if (drvdata == NULL) {
		dev_err(&pdev->dev, "Failed to allocate device data\n");
		ret = -ENOMEM;
		goto err;
	}

	drvdata->desc.name = kstrdup(config->supply_name, GFP_KERNEL);
	if (drvdata->desc.name == NULL) {
		dev_err(&pdev->dev, "Failed to allocate supply name\n");
		ret = -ENOMEM;
		goto err;
	}
	drvdata->desc.type = REGULATOR_VOLTAGE;
	drvdata->desc.owner = THIS_MODULE;
	drvdata->desc.ops = &ipq_fixed_ops;

	if (config->microvolts)
		drvdata->desc.n_voltages = 1;

	drvdata->startup_delay = config->startup_delay;

	drvdata->is_enabled = true;

	drvdata->dev = regulator_register(&drvdata->desc, &pdev->dev,
					config->init_data, drvdata,
					pdev->dev.of_node);
	if (IS_ERR(drvdata->dev)) {
		ret = PTR_ERR(drvdata->dev);
		dev_err(&pdev->dev, "Failed to register regulator: %d\n", ret);
		goto err;
	}

	platform_set_drvdata(pdev, drvdata);

	return 0;

err_name:
	kfree(drvdata->desc.name);
err:
	kfree(drvdata);
	return ret;
}

struct platform_device *regulator_register_ipq_dummy(int id,
		struct regulator_consumer_supply *supplies, int num_supplies, int min_uV, int max_uV)
{
	struct ipq_regulator_data *data;

	data = kzalloc(sizeof(*data), GFP_KERNEL);
	if (!data)
		return NULL;

	data->cfg.supply_name = supplies->supply;

	data->cfg.enabled_at_boot = 1;
	data->cfg.init_data = &data->init_data;

	data->init_data.constraints.always_on = 1;
	data->init_data.consumer_supplies = supplies;
	data->init_data.num_consumer_supplies = num_supplies;

	data->init_data.constraints.min_uV = min_uV;
	data->init_data.constraints.max_uV = max_uV;

	data->init_data.constraints.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE | REGULATOR_CHANGE_STATUS |
		REGULATOR_CHANGE_MODE | REGULATOR_CHANGE_DRMS;

	data->pdev.name = "reg-ipq-dummy";
	data->pdev.id = id;
	data->pdev.dev.platform_data = &data->cfg;
	data->pdev.dev.release = regulator_ipq_release;

	platform_device_register(&data->pdev);

	return &data->pdev;
}

static int __devexit reg_ipq_fixed_remove(struct platform_device *pdev)
{
	struct ipq_drv_data *drvdata = platform_get_drvdata(pdev);

	regulator_unregister(drvdata->dev);

	kfree(drvdata->desc.name);
	kfree(drvdata);

	return 0;
}

static struct platform_driver regulator_ipq_fixed_driver = {
	.probe		= reg_ipq_fixed_probe,
	.remove		= __devexit_p(reg_ipq_fixed_remove),
	.driver		= {
		.name		= "reg-ipq-dummy",
		.owner		= THIS_MODULE,
		.of_match_table = NULL,
	},
};

static int __init regulator_ipq_fixed_init(void)
{
	return platform_driver_register(&regulator_ipq_fixed_driver);
}

arch_initcall(regulator_ipq_fixed_init);

static void __exit regulator_ipq_fixed_exit(void)
{
	platform_driver_unregister(&regulator_ipq_fixed_driver);
}
module_exit(regulator_ipq_fixed_exit);

MODULE_DESCRIPTION("IPQ regulator");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:reg-ipq-dummy");
