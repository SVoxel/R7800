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

extern struct platform_driver mi2s_codec_driver;
extern struct platform_driver spdif_codec_driver;
extern struct platform_driver pcm_codec_driver;

static int __init codec_init(void)
{
	platform_driver_register(&mi2s_codec_driver);
	platform_driver_register(&spdif_codec_driver);
	platform_driver_register(&pcm_codec_driver);
	return 0;
}

static void __exit codec_exit(void)
{
	platform_driver_unregister(&mi2s_codec_driver);
	platform_driver_unregister(&spdif_codec_driver);
	platform_driver_unregister(&pcm_codec_driver);
}

module_init(codec_init);
module_exit(codec_exit);
MODULE_DESCRIPTION("IPQ LPASS Codec Driver");
MODULE_LICENSE("GPL v2");
