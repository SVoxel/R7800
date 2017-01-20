/* Copyright (c) 2013 The Linux Foundation. All rights reserved.
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

static struct snd_soc_dai_link ipq_snd_dai[] = {
	/* Front end DAI Links */
	{
		.name		= "IPQ806x Media1",
		.stream_name	= "MultiMedia1",
		/* Front End DAI Name */
		.cpu_dai_name	= "ipq-mi2s-dai",
		/* Platform Driver Name */
		.platform_name	= "ipq-pcm-mi2s",
		/* Codec DAI Name */
		.codec_dai_name	= "ipq-mi2s-codec-dai",
		/*Codec Driver Name */
		.codec_name	= "ipq-mi2s-codec",
	},
	{
		.name		= "IPQ806x Media2",
		.stream_name	= "MultiMedia2",
		.cpu_dai_name	= "ipq-spdif-dai",
		.platform_name	= "ipq-pcm-spdif",
		.codec_dai_name	= "ipq-spdif-codec-dai",
		.codec_name	= "ipq-spdif-codec",
	},
	{
		.name		= "IPQ806x VoIP",
		.stream_name	= "CS-Voice",
		.cpu_dai_name	= "ipq-pcm-dai",
		.platform_name	= "ipq-pcm-voip",
		.codec_dai_name	= "ipq-pcm-codec-dai",
		.codec_name	= "ipq-pcm-codec",
	},
};

static struct snd_soc_card snd_soc_card_ipq = {
	.name		= "ipq806x_snd_card",
	.dai_link	= ipq_snd_dai,
	.num_links	= ARRAY_SIZE(ipq_snd_dai),
};

static struct platform_device *ipq_snd_device;
static int __init ipq806x_lpass_init(void)
{
	int ret;

	ipq_snd_device = platform_device_alloc("soc-audio", -1);
	if (!ipq_snd_device) {
		pr_err("%s: snd device allocation failed\n", __func__);
		return -ENOMEM;
	}

	platform_set_drvdata(ipq_snd_device, &snd_soc_card_ipq);
	ret = platform_device_add(ipq_snd_device);
	if (ret) {
		pr_err("%s: snd device add failed\n", __func__);
		goto err_add;
	}

	return ret;
err_add:
	platform_device_del(ipq_snd_device);
	platform_device_put(ipq_snd_device);
	return ret;
}

static void __exit ipq806x_lpass_exit(void)
{
	platform_device_unregister(ipq_snd_device);
}


module_init(ipq806x_lpass_init);
module_exit(ipq806x_lpass_exit);

MODULE_DESCRIPTION("ALSA SoC IPQ806x Machine Driver");
MODULE_LICENSE("GPL v2");
