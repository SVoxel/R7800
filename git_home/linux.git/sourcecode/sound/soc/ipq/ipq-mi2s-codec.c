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

#include <linux/module.h>
#include <sound/soc.h>
#include <sound/pcm.h>
#include <sound/soc.h>
#include <sound/soc-dai.h>
#include <sound/control.h>
#include <linux/platform_device.h>
#include <linux/spi/spi.h>
#include "ipq-mi2s-codec.h"

static struct snd_soc_dai_driver mi2s_codec_dai = {
		.name = "ipq-mi2s-codec-dai",
		.playback = {
			.stream_name = "lpass-mi2s-playback",
			.channels_min = 2,
			.channels_max = 8,
			.rates = SNDRV_PCM_RATE_8000_192000,
			.formats = SNDRV_PCM_FMTBIT_S16 |
				SNDRV_PCM_FMTBIT_S24 |
				SNDRV_PCM_FMTBIT_S32,
		},
		.capture = {
			.stream_name = "lpass-mi2s-capture",
			.channels_min = 2,
			.channels_max = 2,
			.rates = SNDRV_PCM_RATE_8000_192000,
			.formats = SNDRV_PCM_FMTBIT_S16 |
				SNDRV_PCM_FMTBIT_S24,
		},
};

static int mi2s_info(struct snd_kcontrol *kcontrol,
			struct snd_ctl_elem_info *uinfo)
{
	return -ENOTSUPP;
}


static const struct snd_kcontrol_new mi2s_vol_ctrl  = {
	.iface = SNDRV_CTL_ELEM_IFACE_MIXER,
	.name = "Mi2s playback volume",
	.access = (SNDRV_CTL_ELEM_ACCESS_TLV_READ |
		SNDRV_CTL_ELEM_ACCESS_READWRITE),
	.info = mi2s_info,
};

static const struct snd_soc_codec_driver mi2s_codec = {
	.num_controls = 0,
};

static int mi2s_codec_probe(struct platform_device *pdev)
{
	return snd_soc_register_codec(&pdev->dev,
		&mi2s_codec, &mi2s_codec_dai, 1);
}

static int mi2s_codec_remove(struct platform_device *pdev)
{
	snd_soc_unregister_codec(&pdev->dev);
	return 0;
}

struct platform_driver mi2s_codec_driver = {
	.probe	= mi2s_codec_probe,
	.remove = mi2s_codec_remove,
	.driver = {
		.name = "ipq-mi2s-codec",
		.owner = THIS_MODULE,
	},
};

