/*
 * qca-cus227.c -- ALSA machine code for CUS227 board ref design (and relatives)
 *
 * Copyright (c) 2013 The Linux Foundation. All rights reserved.
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

#include <asm/delay.h>
#include <linux/types.h>
#include <sound/core.h>
#include <sound/soc.h>
#include <linux/module.h>

/* Driver include */
#include <asm/mach-ath79/ar71xx_regs.h>
#include <asm/mach-ath79/ath79.h>
#include "ath79-i2s.h"
#include "ath79-pcm.h"
#include "../codecs/wm8988.h"

static struct platform_device *cus227_snd_device;

static int cus227_hifi_hw_params(struct snd_pcm_substream *ss,
	struct snd_pcm_hw_params *params)
{
	struct snd_soc_pcm_runtime *rtd = ss->private_data;
	struct snd_soc_dai *codec_dai = rtd->codec_dai;
	struct snd_soc_codec *codec = rtd->codec;
	unsigned int clk = 0;
	int ret;
	u16 vol_reg;
	switch (params_rate(params)) {
	case 8000:
	case 16000:
	case 32000:
	case 48000:
	case 96000:
		clk = 12288000;
		break;
	case 11025:
	case 22050:
	case 44100:
	case 88200:
		clk = 11289600;
		break;
	}

	/* set the codec system clock for DAC and ADC */
	ret = snd_soc_dai_set_sysclk(codec_dai, WM8988_SYSCLK, clk,
				     SND_SOC_CLOCK_OUT);
	if (ret < 0)
		return ret;

	if (ss->stream == SNDRV_PCM_STREAM_CAPTURE) {
		/*Disable Left&Right Channel Input Analogue Mute,
		 * and wm8988 datasheet says we should also set bit8
		 * for WM8988_LINVOL&WM8988_RINVOL concurrently.*/
		vol_reg = snd_soc_read(codec, WM8988_LINVOL) & 0x7f;
		snd_soc_write(codec, WM8988_LINVOL, 0x100 | vol_reg);
		vol_reg = snd_soc_read(codec, WM8988_RINVOL) & 0x7f;
		snd_soc_write(codec, WM8988_RINVOL, 0x100 | vol_reg);
	}
	return 0;
}

static int cus227_late_probe(struct snd_soc_card *card)
{
	struct snd_soc_dai *codec_dai = card->rtd->codec_dai;
	int ret;

	ret = snd_soc_dai_set_sysclk(codec_dai, WM8988_SYSCLK,
				     11289600, SND_SOC_CLOCK_OUT);
	if (ret < 0)
		return ret;

	return 0;
}

static struct snd_soc_ops cus227_dai_ops = {
	.hw_params = cus227_hifi_hw_params,
};

static struct snd_soc_dai_link cus227_dai = {
	.name = "CUS227 audio",
	.stream_name = "CUS227 audio",
	.cpu_dai_name = "ath79-i2s",
	.codec_dai_name = "wm8988-hifi",
	.platform_name = "ath79-pcm-audio",
	.codec_name = "spi0.1",
	/* use ops to check startup state */
	.ops = &cus227_dai_ops,
};

static struct snd_soc_card snd_soc_cus227 = {
	.name = "QCA CUS227",
	.long_name = "CUS227 - ath79-pcm/ath79-i2s/wm8988",
	.dai_link = &cus227_dai,
	.num_links = 1,

	.late_probe = cus227_late_probe,
};

static int __init cus227_init(void)
{
	int ret;

	cus227_snd_device = platform_device_alloc("soc-audio", -1);
	if(!cus227_snd_device)
		return -ENOMEM;

	platform_set_drvdata(cus227_snd_device, &snd_soc_cus227);
	ret = platform_device_add(cus227_snd_device);

	if (ret) {
		platform_device_put(cus227_snd_device);
	}

	return ret;
}

static void __exit cus227_exit(void)
{
	platform_device_unregister(cus227_snd_device);
}

module_init(cus227_init);
module_exit(cus227_exit);

MODULE_AUTHOR("Qualcomm-Atheros Inc.");
MODULE_AUTHOR("Mathieu Olivari <mathieu@qca.qualcomm.com>");
MODULE_DESCRIPTION("QCA Audio Machine module");
MODULE_LICENSE("Dual BSD/GPL");
