/*
 * ath-pcm.c -- ALSA PCM interface for the QCA Wasp based audio interface
 *
 * Copyright (c) 2012 The Linux Foundation. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <linux/module.h>
#include <sound/soc.h>
#include <sound/pcm.h>

#include <asm/mach-ath79/ar71xx_regs.h>
#include <asm/mach-ath79/ath79.h>

#define DRV_NAME		"ath79-internal-codec"

static struct snd_soc_dai_driver ath79_codec_dai = {
	.name = "ath79-hifi",
	.playback = {
		.stream_name = "Playback",
		.channels_min = 2,
		.channels_max = 2,
		.rates = SNDRV_PCM_RATE_22050 |
				SNDRV_PCM_RATE_32000 |
				SNDRV_PCM_RATE_44100 |
				SNDRV_PCM_RATE_48000 |
				SNDRV_PCM_RATE_88200 |
				SNDRV_PCM_RATE_96000,
		.formats = SNDRV_PCM_FMTBIT_S8 |
				SNDRV_PCM_FMTBIT_S16 |
				SNDRV_PCM_FMTBIT_S24 |
				SNDRV_PCM_FMTBIT_S32,
		},
};

static int ath79_volume_ctrl_info(struct snd_kcontrol *kcontrol,
					struct snd_ctl_elem_info *uinfo)
{
	uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
	uinfo->count = 2;
	uinfo->value.integer.min = 0;
	uinfo->value.integer.max = 21;
	return 0;
}

/* If value in reg has bit4 set, it's a negative. See Datasheet for details */
#define reg_to_int(t) (t >= 0x10 ? 14 - (t & 0xf) : t + 14)
#define int_to_reg(t) (t >= 14 ? (t - 14) : (14 - t) | 0x10)

static int ath79_volume_ctrl_get(struct snd_kcontrol *kcontrol,
					struct snd_ctl_elem_value *ucontrol)
{
	u32 t, lvol, rvol;

	t = ath79_stereo_rr(AR934X_STEREO_REG_VOLUME);
	lvol = (t >> AR934X_STEREO_VOLUME_CH0) & AR934X_STEREO_VOLUME_MASK;
	rvol = (t >> AR934X_STEREO_VOLUME_CH1) & AR934X_STEREO_VOLUME_MASK;
	ucontrol->value.integer.value[0] = reg_to_int(lvol);
	ucontrol->value.integer.value[1] = reg_to_int(rvol);
	t = reg_to_int(t);

	return 0;
}

static int ath79_volume_ctrl_put(struct snd_kcontrol *kcontrol,
					struct snd_ctl_elem_value *ucontrol)
{
	u32 t;

	t = ath79_stereo_rr(AR934X_STEREO_REG_VOLUME);
	if ((reg_to_int(t) == ucontrol->value.integer.value[0]) &&
	    (reg_to_int(t) == ucontrol->value.integer.value[1])) {
		return 0;
	}

	t &= ~(AR934X_STEREO_VOLUME_MASK << AR934X_STEREO_VOLUME_CH0);
	t &= ~(AR934X_STEREO_VOLUME_MASK << AR934X_STEREO_VOLUME_CH1);
	t |= int_to_reg(ucontrol->value.integer.value[0])
		<< AR934X_STEREO_VOLUME_CH0;
	t |= int_to_reg(ucontrol->value.integer.value[1])
		 << AR934X_STEREO_VOLUME_CH1;
	ath79_stereo_wr(AR934X_STEREO_REG_VOLUME, t);
	return 1;
}

static const struct snd_kcontrol_new ath79_volume_ctrl __devinitdata = {
	.iface = SNDRV_CTL_ELEM_IFACE_MIXER,
	.name = "Master Playback Volume",
	.access = SNDRV_CTL_ELEM_ACCESS_TLV_READ |
			SNDRV_CTL_ELEM_ACCESS_READWRITE,
	.info = ath79_volume_ctrl_info,
	.get = ath79_volume_ctrl_get,
	.put = ath79_volume_ctrl_put,
};

static const struct snd_soc_codec_driver soc_codec_ath79 = {
	.controls = &ath79_volume_ctrl,
	.num_controls = 1,
};

static int ath79_codec_probe(struct platform_device *pdev)
{
	return snd_soc_register_codec(&pdev->dev, &soc_codec_ath79,
			&ath79_codec_dai, 1);
}

static int ath79_codec_remove(struct platform_device *pdev)
{
	snd_soc_unregister_codec(&pdev->dev);
	return 0;
}

static struct platform_driver ath79_codec_driver = {
	.probe		= ath79_codec_probe,
	.remove		= ath79_codec_remove,
	.driver		= {
		.name	= DRV_NAME,
		.owner	= THIS_MODULE,
	},
};

module_platform_driver(ath79_codec_driver);

MODULE_AUTHOR("Qualcomm-Atheros Inc.");
MODULE_AUTHOR("Mathieu Olivari <mathieu@qca.qualcomm.com>");
MODULE_DESCRIPTION("ATH79 integrated codec driver");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_ALIAS("platform:" DRV_NAME);
