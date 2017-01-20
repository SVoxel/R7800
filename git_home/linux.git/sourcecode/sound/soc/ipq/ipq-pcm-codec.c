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

static struct snd_soc_dai_driver pcm_codec_dai = {
	.name = "ipq-pcm-codec-dai",
	.capture = {
		.stream_name = "lpass-pcm-capture",
		.rate_max = 16000,
		.rate_min = 8000,
		.rates = SNDRV_PCM_RATE_8000 |
			SNDRV_PCM_RATE_16000,
		.formats = SNDRV_PCM_FMTBIT_S16_LE |
			SNDRV_PCM_FMTBIT_U8 |
			SNDRV_PCM_FORMAT_U16 |
			SNDRV_PCM_FMTBIT_S8,
		.channels_min = 1,
		.channels_max = 2,
	},
	.playback = {
		.stream_name = "lpass-pcm-playback",
		.channels_min = 1,
		.channels_max = 2,
		.rates = SNDRV_PCM_RATE_8000 |
			SNDRV_PCM_RATE_16000,
		.formats = SNDRV_PCM_FMTBIT_S8 |
			SNDRV_PCM_FMTBIT_U8 |
			SNDRV_PCM_FORMAT_U16 |
			SNDRV_PCM_FMTBIT_S16_LE,
	},
};

static int pcm_info(struct snd_kcontrol *kcontrol,
			struct snd_ctl_elem_info *uinfo)
{
	return -ENOTSUPP;
}

static const struct snd_kcontrol_new pcm_vol_ctrl  = {
	.iface = SNDRV_CTL_ELEM_IFACE_MIXER,
	.name = "Pcm playback volume",
	.access = (SNDRV_CTL_ELEM_ACCESS_TLV_READ |
		SNDRV_CTL_ELEM_ACCESS_READWRITE),
	.info = pcm_info,
};

static const struct snd_soc_codec_driver pcm_codec = {
	.num_controls = 0,
};

static int pcm_codec_probe(struct platform_device *pdev)
{
	return snd_soc_register_codec(&pdev->dev,
		&pcm_codec, &pcm_codec_dai, 1);
}

static int pcm_codec_remove(struct platform_device *pdev)
{
	snd_soc_unregister_codec(&pdev->dev);
	return 0;
}

struct platform_driver pcm_codec_driver = {
	.probe	= pcm_codec_probe,
	.remove = pcm_codec_remove,
	.driver = {
		.name = "ipq-pcm-codec",
		.owner = THIS_MODULE,
	},
};

