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

static struct snd_soc_dai_driver spdif_codec_dai = {
	.name = "ipq-spdif-codec-dai",
	.playback = {
		.stream_name = "lpass-spdif-playback",
		.channels_min = 2,
		.channels_max = 8,
		.rates = SNDRV_PCM_RATE_22050 |
			SNDRV_PCM_RATE_32000 |
			SNDRV_PCM_RATE_44100 |
			SNDRV_PCM_RATE_48000 |
			SNDRV_PCM_RATE_88200 |
			SNDRV_PCM_RATE_96000 |
			SNDRV_PCM_RATE_176400 |
			SNDRV_PCM_RATE_192000,
		.formats = SNDRV_PCM_FMTBIT_S16 |
			SNDRV_PCM_FMTBIT_S24_3LE |
			SNDRV_PCM_FMTBIT_S24,
	},
};

static int spdif_info(struct snd_kcontrol *kcontrol,
			struct snd_ctl_elem_info *uinfo)
{
	return -ENOTSUPP;
}

static const struct snd_kcontrol_new spdif_vol_ctrl  = {
	.iface = SNDRV_CTL_ELEM_IFACE_MIXER,
	.name = "Spdif playback volume",
	.access = (SNDRV_CTL_ELEM_ACCESS_TLV_READ |
		SNDRV_CTL_ELEM_ACCESS_READWRITE),
	.info = spdif_info,
};

static const struct snd_soc_codec_driver spdif_codec = {
	.num_controls = 0,
};

static int spdif_codec_probe(struct platform_device *pdev)
{
	return snd_soc_register_codec(&pdev->dev,
		&spdif_codec, &spdif_codec_dai, 1);
}

static int spdif_codec_remove(struct platform_device *pdev)
{
	snd_soc_unregister_codec(&pdev->dev);
	return 0;
}

struct platform_driver spdif_codec_driver = {
	.probe	= spdif_codec_probe,
	.remove = spdif_codec_remove,
	.driver = {
		.name = "ipq-spdif-codec",
		.owner = THIS_MODULE,
	},
};

