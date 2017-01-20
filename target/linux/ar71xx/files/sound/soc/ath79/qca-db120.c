/*
 * qca-db120.c -- ALSA machine code for DB12x board ref design (and relatives)
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

static struct platform_device *db120_snd_device;

static struct snd_soc_dai_link db120_dai = {
	.name = "DB12x audio",
	.stream_name = "DB12x audio",
	.cpu_dai_name = "ath79-i2s",
	.codec_dai_name = "ath79-hifi",
	.platform_name = "ath79-pcm-audio",
	.codec_name = "ath79-internal-codec",
	/* use ops to check startup state */
};

static struct snd_soc_card snd_soc_db120 = {
	.name = "QCA DB12x",
	.long_name = "QCA DB12x - ath79-pcm/ath79-i2s/ath79-int-codec",
	.dai_link = &db120_dai,
	.num_links = 1,
};

static int __init db120_init(void)
{
	int ret;

	db120_snd_device = platform_device_alloc("soc-audio", -1);
	if(!db120_snd_device)
		return -ENOMEM;

	platform_set_drvdata(db120_snd_device, &snd_soc_db120);
	ret = platform_device_add(db120_snd_device);

	if (ret) {
		platform_device_put(db120_snd_device);
	}

	return ret;
}

static void __exit db120_exit(void)
{
	platform_device_unregister(db120_snd_device);
}

module_init(db120_init);
module_exit(db120_exit);

MODULE_AUTHOR("Qualcomm-Atheros Inc.");
MODULE_AUTHOR("Mathieu Olivari <mathieu@qca.qualcomm.com>");
MODULE_DESCRIPTION("QCA Audio Machine module");
MODULE_LICENSE("Dual BSD/GPL");
