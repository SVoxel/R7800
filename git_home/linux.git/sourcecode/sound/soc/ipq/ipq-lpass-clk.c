/* Copyright (c) 2013 Qualcomm Atheros, Inc.
 *
 * Copyright (c) 2010-2011, The Linux Foundation. All rights reserved.
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

#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/platform_device.h>
#include <linux/of_platform.h>
#include <linux/gpio.h>
#include <linux/slab.h>
#include <sound/core.h>
#include <sound/soc.h>
#include <sound/soc-dapm.h>
#include <sound/pcm.h>
#include <sound/jack.h>
#include <asm/io.h>
#include "ipq806x.h"
#include "ipq-lpaif.h"

static struct clk *lpass_ahbex_clk;
static struct clk *lpass_ahbix_clk;

struct lpass_clk_baseinfo lpass_clk_base;
EXPORT_SYMBOL_GPL(lpass_clk_base);

void ipq_cfg_pcm_reset(uint8_t reset)
{
	uint32_t cfg;
	cfg = readl(lpass_clk_base.base + LCC_PCM_NS);

	if (reset)
		cfg = cfg | LCC_PCM_NS_ARES;
	else
		cfg = cfg & ~LCC_PCM_NS_ARES;

	writel(cfg, lpass_clk_base.base + LCC_PCM_NS);
}
EXPORT_SYMBOL_GPL(ipq_cfg_pcm_reset);

static int ipq_lpass_clk_probe(struct platform_device *pdev)
{
	uint32_t ret;
	struct resource *clk_res;

	clk_res = platform_get_resource_byname(pdev,
			IORESOURCE_MEM, "ipq-lpass-clk");

	if (!clk_res) {
		dev_err(&pdev->dev, "%s: %d: mem resource for lpass failed\n",
				__func__, __LINE__);
		return -ENOMEM;
	}

	lpass_clk_base.base = ioremap(
			clk_res->start, (clk_res->end - clk_res->start) + 1);

	if (!lpass_clk_base.base) {
		dev_err(&pdev->dev, "%s: %d: Error in lpass clk mem remap\n",
				__func__, __LINE__);
		return -ENOMEM;
	}

	lpass_ahbex_clk = clk_get(&pdev->dev, "ahbex_clk");
	if (IS_ERR(lpass_ahbex_clk)) {
		dev_err(&pdev->dev, "%s: %d: Error in getting ahbex_clk\n",
				__func__, __LINE__);
		ret = PTR_ERR(lpass_ahbex_clk);
		goto err;
	}
	ret = clk_prepare_enable(lpass_ahbex_clk);
	if (IS_ERR_VALUE(ret)) {
		dev_err(&pdev->dev, "%s: %d: Error in enabling ahbex_clk\n",
				__func__, __LINE__);
		goto err;
	}

	lpass_ahbix_clk = clk_get(&pdev->dev, "ahbix_clk");
	if (IS_ERR(lpass_ahbix_clk)) {
		dev_err(&pdev->dev, "%s: %d: Error in getting ahbix_clk\n",
				__func__, __LINE__);
		ret = PTR_ERR(lpass_ahbix_clk);
		goto err;
	}
	clk_set_rate(lpass_ahbix_clk, 131072);
	ret = clk_prepare_enable(lpass_ahbix_clk);
	if (IS_ERR_VALUE(ret)) {
		dev_err(&pdev->dev, "%s: %d: Error in enabling ahbix_clk\n",
				__func__, __LINE__);
		goto err;
	}

	return 0;
err:
	iounmap(lpass_clk_base.base);
	return ret;

}

static int ipq_lpass_clk_remove(struct platform_device *pdev)
{
	pr_debug("%s %d\n", __func__, __LINE__);
	if (lpass_ahbex_clk) {
		clk_disable_unprepare(lpass_ahbex_clk);
		clk_put(lpass_ahbex_clk);
		lpass_ahbex_clk = NULL;
	}

	if (lpass_ahbix_clk) {
		clk_disable_unprepare(lpass_ahbix_clk);
		clk_put(lpass_ahbix_clk);
		lpass_ahbix_clk = NULL;
	}
	iounmap(lpass_clk_base.base);
	return 0;
}

static struct platform_driver ipq_lpass_clk = {
	.driver = {
		.name   = "ipq-lpass-clk",
		.owner  = THIS_MODULE,
	},
	.probe  = ipq_lpass_clk_probe,
	.remove = ipq_lpass_clk_remove,
};

static int __init ipq806x_lpass_clock_init(void)
{
	int ret;
	ret = platform_driver_register(&ipq_lpass_clk);
	if (ret) {
		pr_err("%s: %d: clk registration failed\n",
				__func__, __LINE__);
		goto err_reg;
	}

	return ret;

err_reg:
	platform_driver_unregister(&ipq_lpass_clk);
	return ret;
}

static void __exit ipq806x_lpass_clock_exit(void)
{
	platform_driver_unregister(&ipq_lpass_clk);
}


module_init(ipq806x_lpass_clock_init);
module_exit(ipq806x_lpass_clock_exit);

MODULE_DESCRIPTION("IPQ AHB Clock Initialization Driver");
MODULE_LICENSE("GPL v2");
