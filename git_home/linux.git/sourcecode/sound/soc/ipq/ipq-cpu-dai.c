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

#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/bitops.h>
#include <linux/slab.h>
#include <linux/clk.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/soc.h>
#include <sound/apr_audio.h>
#include <sound/pcm_params.h>
#include <mach/clk.h>
#include <asm/io.h>
#include "ipq-pcm.h"
#include "ipq-lpaif.h"
#include "ipq806x.h"
#include "ipq-spdif.h"

static struct clk *lpaif_mi2s_bit_clk;
static struct clk *lpaif_mi2s_osr_clk;
static struct clk *spdif_bit_clk;
extern struct clk *lpaif_pcm_bit_clk;
struct mi2s_hw_params mi2s_params;
atomic_t mi2s_in_use[2] = {ATOMIC_INIT(0), ATOMIC_INIT(0)};

uint8_t ipq_mi2s_compare_hw_params(struct mi2s_hw_params *curr_params)
{
	if ((curr_params->bit_width == mi2s_params.bit_width) &&
		(curr_params->freq == mi2s_params.freq) &&
		(curr_params->channels == mi2s_params.channels))
		return 0;
	else
		return -EINVAL;
}

static int ipq_lpass_spdif_hw_params(struct snd_pcm_substream *substream,
					struct snd_pcm_hw_params *params,
					struct snd_soc_dai *dai)
{
	uint32_t ret;
	uint32_t bit_width = params_format(params);
	uint32_t freq = params_rate(params);
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct ipq_lpass_runtime_data_t *prtd =
	(struct ipq_lpass_runtime_data_t *)runtime->private_data;

	ret = ipq_spdif_cfg_bit_width(bit_width);
	if (ret) {
		dev_err(dai->dev, "%s:%d\n", __func__, __LINE__);
		return -EINVAL;
	}

	switch (freq) {
	case F_22_05_KHZ:
	case F_32_KHZ:
	case F_44_1_KHZ:
	case F_48_KHZ:
	case F_88_2_KHZ:
	case F_96_KHZ:
	case F_176_4_KHZ:
	case F_192_KHZ:
		ret = clk_set_rate(spdif_bit_clk, freq);
		if (IS_ERR_VALUE(ret)) {
			dev_err(dai->dev,
			"%s: error in setting spdif bit clk \n", __func__);
			return ret;
		}
		ret = clk_prepare_enable(spdif_bit_clk);
		if (IS_ERR_VALUE(ret)) {
			dev_err(dai->dev,
			"%s: error in enabling spdif bit clk \n", __func__);
			return ret;
		}
		prtd->lpaif_clk.is_bit_clk_enabled = 1;
		break;
	default:
		dev_err(dai->dev, "%s:%d Format not supported\n",
						 __func__, __LINE__);
		return -EINVAL;

	}

	return 0;
}

static int ipq_lpass_spdif_prepare(struct snd_pcm_substream *substream,
					struct snd_soc_dai *dai)
{
	dev_dbg(dai->dev, "%s:%d\n", __func__, __LINE__);
	return 0;
}

static int ipq_lpass_spdif_startup(struct snd_pcm_substream *substream,
					struct snd_soc_dai *dai)
{

	spdif_bit_clk = clk_get(dai->dev, "spdif_bit_clk");
	if (IS_ERR(spdif_bit_clk)) {
		dev_err(dai->dev,
			"%s: Error in getting spdif_bit_clk\n", __func__);
		return PTR_ERR(spdif_bit_clk);
	}

	dev_dbg(dai->dev, "%s:%d\n", __func__, __LINE__);
	ipq_spdif_onetime_cfg();
	return 0;
}

static void ipq_lpass_spdif_shutdown(struct snd_pcm_substream *substream,
					struct snd_soc_dai *dai)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct ipq_lpass_runtime_data_t *prtd =
	(struct ipq_lpass_runtime_data_t *)runtime->private_data;
	uint32_t cfg;

	dev_dbg(dai->dev, "%s:%d\n", __func__, __LINE__);
	if (spdif_bit_clk) {
		if (prtd->lpaif_clk.is_bit_clk_enabled)
			clk_disable_unprepare(spdif_bit_clk);

		clk_put(spdif_bit_clk);
		spdif_bit_clk = NULL;
	}

	/* put spdiftx block in reset */
	cfg = readl(lpass_clk_base.base + LCC_AHBEX_BRANCH_CTL);
	cfg |= HWIO_LCC_AHBEX_BRANCH_CTL_SLIMBUS_ARES_RESET;
	writel(cfg, lpass_clk_base.base + LCC_AHBEX_BRANCH_CTL);
}

static int ipq_lpass_spdif_set_fmt(struct snd_soc_dai *dai, unsigned int fmt)
{
	dev_dbg(dai->dev, "%s:%d\n", __func__, __LINE__);
	return 0;
}

static uint32_t ipq_lpass_get_act_bit_width(uint32_t bit_width)
{
	switch (bit_width) {
	case SNDRV_PCM_FORMAT_S8:
	case SNDRV_PCM_FORMAT_U8:
		return __BIT_8;
	case SNDRV_PCM_FORMAT_S16_LE:
	case SNDRV_PCM_FORMAT_S16_BE:
	case SNDRV_PCM_FORMAT_U16_LE:
	case SNDRV_PCM_FORMAT_U16_BE:
		return __BIT_16;
	case SNDRV_PCM_FORMAT_S24_LE:
	case SNDRV_PCM_FORMAT_S24_3LE:
	case SNDRV_PCM_FORMAT_S24_BE:
	case SNDRV_PCM_FORMAT_U24_LE:
	case SNDRV_PCM_FORMAT_U24_BE:
		return __BIT_24;
	case SNDRV_PCM_FORMAT_S32_LE:
	case SNDRV_PCM_FORMAT_S32_BE:
	case SNDRV_PCM_FORMAT_U32_LE:
	case SNDRV_PCM_FORMAT_U32_BE:
		return __BIT_32;
	default:
		return __BIT_INVAL;
	}
}

static uint32_t ipq_lpass_get_bit_div(uint32_t samp_freq, uint32_t bit_width,
							uint32_t channels)
{
	if (IPQ_CHANNELS_8 == channels) {
		if (__BIT_24 == bit_width &&
			((FREQ_176400 != samp_freq) &&
			(FREQ_192000 != samp_freq)))
			return __BIT_DIV_2;
		return __BIT_DIV_1;
	} else if (IPQ_CHANNELS_6 == channels) {
		if ((__BIT_32 == bit_width &&
			((FREQ_176400 == samp_freq) ||
			(FREQ_192000 == samp_freq))) ||
			(__BIT_24 == bit_width &&
			((FREQ_88200 == samp_freq) ||
			(FREQ_192000 == samp_freq))))
			return __BIT_DIV_1;
		else
			return __BIT_DIV_2;
	}

	switch (samp_freq) {
	case FREQ_8000:
	case FREQ_11025:
	case FREQ_16000:
	case FREQ_22050:
	case FREQ_32000:
	case FREQ_44100:
		if (__BIT_16 == bit_width )
			return __BIT_DIV_4;
	case FREQ_48000:
		if (__BIT_24 == bit_width )
			return __BIT_DIV_4;
	case FREQ_64000:
	case FREQ_88200:
		if (__BIT_16 == bit_width )
			return __BIT_DIV_4;
	case FREQ_96000:
	case FREQ_192000:
		return __BIT_DIV_4;
	case FREQ_176400:
		return __BIT_DIV_2;
	default:
		return __BIT_DIV_INVAL;
	}
}

static int ipq_lpass_mi2s_hw_params(struct snd_pcm_substream *substream,
					struct snd_pcm_hw_params *params,
					struct snd_soc_dai *dai)
{
	uint32_t ret = 0;
	uint32_t bit_act;
	uint16_t bit_div;
	uint32_t bit_width = params_format(params);
	uint32_t channels = params_channels(params);
	uint32_t rate = params_rate(params);
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct ipq_lpass_runtime_data_t *prtd =
	(struct ipq_lpass_runtime_data_t *)runtime->private_data;
	struct mi2s_hw_params curr_params;

	bit_act = ipq_lpass_get_act_bit_width(bit_width);
	if (bit_act == __BIT_INVAL)
		return -EINVAL;

	prtd->pcm_stream_info.bit_width = bit_act;
	curr_params.freq = rate;
	curr_params.channels = channels;
	curr_params.bit_width = bit_act;

        /*
         * Since CLKS are shared by all I2S channels, Rx and Tx when used
         * simulatneoulsy will have to use the same channel, sampling
         * frequency and bit widths. So compare the settings and then
         * enable the clocks. Any MIC register change will take
         * effect only when MIC is enabled in trigger.
         */

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) {
		atomic_set(&mi2s_in_use[SNDRV_PCM_STREAM_PLAYBACK], 1);
		/* disable SPKR to make sure it will start in sane state */
		ipq_cfg_i2s_spkr(0, 0, LPA_IF_MI2S);

		/*
		 * Set channel info, it will take effect only if SPKR is
		 * enabled
		 */
		ret = ipq_cfg_mi2s_playback_hwparams_channels(channels,
						LPA_IF_MI2S, bit_act);
		if (atomic_read(&mi2s_in_use[SNDRV_PCM_STREAM_CAPTURE])) {
			if (ipq_mi2s_compare_hw_params(&curr_params))
				/* Playback and capture settings do not match */
				return -EINVAL;
			else
				/* Settings match, CLK is already enabled */
				return 0;
		}
	} else if (substream->stream == SNDRV_PCM_STREAM_CAPTURE) {
		atomic_set(&mi2s_in_use[SNDRV_PCM_STREAM_CAPTURE], 1);
		/* disable MIC to make sure it will start in sane state */
		ipq_cfg_i2s_mic(0, LPA_IF_MI2S);

		/*
		 * Set channel info, it will take effect only if MIC is
		 * enabled
		 */
		ret = ipq_cfg_mi2s_capture_hwparams_channels(channels,
						LPA_IF_MI2S, bit_act);
		if (atomic_read(&mi2s_in_use[SNDRV_PCM_STREAM_PLAYBACK])) {
			if (ipq_mi2s_compare_hw_params(&curr_params))
				/* Playback and capture settings do not match */
				return -EINVAL;
			else
				/* Settings match, CLK is already enabled */
				return 0;
		}
	} else {
		return -EINVAL;
	}

	/* Make sure our channel setting was success */
	if (ret)
		/* shutdown would be called where we restore stuff */
		return -EINVAL;

	mi2s_params.freq = rate;
	mi2s_params.channels = channels;
	mi2s_params.bit_width = bit_act;

	ret = ipq_cfg_mi2s_hwparams_bit_width(bit_width, LPA_IF_MI2S);
	if (ret)
		return -EINVAL;

	bit_div = ipq_lpass_get_bit_div(rate, bit_act, channels);
	if (bit_div == __BIT_DIV_INVAL)
		return -EINVAL;

	ret = clk_set_rate(lpaif_mi2s_osr_clk,
		(rate * bit_act * channels * bit_div));
	if (IS_ERR_VALUE(ret)) {
		dev_err(dai->dev,
		"%s: error in setting mi2s osr clk \n", __func__);
		return ret;
	}
	ret = clk_prepare_enable(lpaif_mi2s_osr_clk);
	if (IS_ERR_VALUE(ret)) {
		dev_err(dai->dev,
		"%s: error in enabling mi2s osr clk \n", __func__);
		return ret;
	}
	prtd->lpaif_clk.is_osr_clk_enabled = 1;

	ret = clk_set_rate(lpaif_mi2s_bit_clk, ((channels / 2) * bit_div));
	if (IS_ERR_VALUE(ret)) {
		dev_err(dai->dev,
		"%s: error in setting mi2s bit clk \n", __func__);
		return ret;
	}
	ret = clk_prepare_enable(lpaif_mi2s_bit_clk);
	if (IS_ERR_VALUE(ret)) {
		dev_err(dai->dev,
		"%s: error in enabling mi2s bit clk \n", __func__);
		return ret;
	}
	prtd->lpaif_clk.is_bit_clk_enabled = 1;
	return 0;
}

static int ipq_lpass_mi2s_prepare(struct snd_pcm_substream *substream,
					struct snd_soc_dai *dai)
{
	dev_dbg(dai->dev, "%s:%d\n", __func__, __LINE__);
	return 0;
}
static int ipq_lpass_mi2s_startup(struct snd_pcm_substream *substream,
					struct snd_soc_dai *dai)
{

	lpaif_mi2s_osr_clk = clk_get(dai->dev, "mi2s_osr_clk");
	if (IS_ERR(lpaif_mi2s_osr_clk)) {
		dev_err(dai->dev,
			"%s: Error in getting lpaif_mi2s_osr_clk\n", __func__);
		return PTR_ERR(lpaif_mi2s_osr_clk);
	}

	lpaif_mi2s_bit_clk = clk_get(dai->dev, "mi2s_bit_clk");
	if (IS_ERR(lpaif_mi2s_bit_clk)) {
		dev_err(dai->dev,
			"%s: Error in getting lpaif_mi2s_bit_clk\n", __func__);
		return PTR_ERR(lpaif_mi2s_bit_clk);
	}

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
		ipq_cfg_i2s_spkr(0, 0, LPA_IF_MI2S);
	else if (substream->stream == SNDRV_PCM_STREAM_CAPTURE)
		ipq_cfg_i2s_mic(0, LPA_IF_MI2S);
	else
		return -EINVAL;

	return 0;
}

static void ipq_lpass_mi2s_shutdown(struct snd_pcm_substream *substream,
					struct snd_soc_dai *dai)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct ipq_lpass_runtime_data_t *prtd =
	(struct ipq_lpass_runtime_data_t *)runtime->private_data;

	atomic_set(&mi2s_in_use[substream->stream], 0);

	/*
	 * Since CLKS are shared by all I2S channels, we need to shutdown
	 * the clock only when both streams have been closed.
	*/

	if (!(atomic_read(&mi2s_in_use[SNDRV_PCM_STREAM_PLAYBACK]) ||
		atomic_read(&mi2s_in_use[SNDRV_PCM_STREAM_CAPTURE]))) {
		if (lpaif_mi2s_osr_clk) {
			if (prtd->lpaif_clk.is_osr_clk_enabled)
				clk_disable_unprepare(lpaif_mi2s_osr_clk);

			clk_put(lpaif_mi2s_osr_clk);
			lpaif_mi2s_osr_clk = NULL;
		}

		if (lpaif_mi2s_bit_clk) {
			if (prtd->lpaif_clk.is_bit_clk_enabled)
				clk_disable_unprepare(lpaif_mi2s_bit_clk);

			clk_put(lpaif_mi2s_bit_clk);
			lpaif_mi2s_bit_clk = NULL;
		}
	}
}

static int ipq_lpass_mi2s_set_fmt(struct snd_soc_dai *dai, unsigned int fmt)
{
	dev_dbg(dai->dev, "%s:%d\n", __func__, __LINE__);
	return 0;
}

static int ipq_lpass_pcm_hw_params(struct snd_pcm_substream *substream,
					struct snd_pcm_hw_params *params,
					struct snd_soc_dai *dai)
{
	uint32_t ret;
	uint32_t stream;
	uint32_t bit_act;
	uint32_t bit_width = params_format(params);
	uint32_t freq = params_rate(params);

	struct snd_pcm_runtime *runtime = substream->runtime;
	struct ipq_lpass_runtime_data_t *prtd =
	(struct ipq_lpass_runtime_data_t *)runtime->private_data;

	bit_act = ipq_lpass_get_act_bit_width(bit_width);
	if (bit_act == __BIT_INVAL)
		return -EINVAL;

	prtd->pcm_stream_info.bit_width = bit_act;

	ret = clk_set_rate(lpaif_pcm_bit_clk, (freq * bit_act));
	if (IS_ERR_VALUE(ret)) {
		dev_err(dai->dev,
		"%s: error in setting pcm bit clk \n", __func__);
		return ret;
	}
	ret = clk_prepare_enable(lpaif_pcm_bit_clk);
	if (IS_ERR_VALUE(ret)) {
		dev_err(dai->dev,
		"%s: error in enabling pcm bit clk \n", __func__);
		return ret;
	}
	prtd->lpaif_clk.is_bit_clk_enabled = 1;

	stream = (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) ? 1 : 0;

	if (bit_width == SNDRV_PCM_FORMAT_S8 ||
	bit_width == SNDRV_PCM_FORMAT_U8 ||
	bit_width == SNDRV_PCM_FORMAT_U16 ||
	bit_width == SNDRV_PCM_FORMAT_S16)
		ipq_cfg_pcm_width(bit_width, stream);
	else
		dev_dbg(dai->dev, "Format Not supported %s: %d\n",
						__func__, __LINE__);

	ipq_cfg_pcm_rate(params_rate(params));
	return 0;
}

static int ipq_lpass_pcm_prepare(struct snd_pcm_substream *substream,
					struct snd_soc_dai *dai)
{
	dev_dbg(dai->dev, "%s:%d\n", __func__, __LINE__);
	return 0;
}

static int ipq_lpass_pcm_startup(struct snd_pcm_substream *substream,
					struct snd_soc_dai *dai)
{
	clk_reset(lpaif_pcm_bit_clk, LPAIF_PCM_ASSERT);
	/*
	 * Put the PCM instance to Reset
	 */
	ipq_cfg_pcm_reset(1);
	/*
	 * Sync source external
	 */
	ipq_cfg_pcm_sync_src(1);
	/*
	 * Configuring Slot0 for Tx and Rx
	 */
	ipq_cfg_pcm_slot(0, 0);
	ipq_cfg_pcm_slot(0, 1);

	return 0;
}

static void ipq_lpass_pcm_shutdown(struct snd_pcm_substream *substream,
				struct snd_soc_dai *dai)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct ipq_lpass_runtime_data_t *prtd =
	(struct ipq_lpass_runtime_data_t *)runtime->private_data;

	if (lpaif_pcm_bit_clk) {
		if (prtd->lpaif_clk.is_bit_clk_enabled)
			clk_disable_unprepare(lpaif_pcm_bit_clk);
	}
	dev_dbg(dai->dev, "%s:%d\n", __func__, __LINE__);
}

static int ipq_lpass_pcm_set_fmt(struct snd_soc_dai *dai, unsigned int fmt)
{
	dev_dbg(dai->dev, "%s:%d\n", __func__, __LINE__);
	return 0;
}

static struct snd_soc_dai_ops ipq_lpass_pcm_ops = {
	.startup	= ipq_lpass_pcm_startup,
	.prepare	= ipq_lpass_pcm_prepare,
	.hw_params	= ipq_lpass_pcm_hw_params,
	.shutdown	= ipq_lpass_pcm_shutdown,
	.set_fmt	= ipq_lpass_pcm_set_fmt,
};

static struct snd_soc_dai_ops ipq_lpass_mi2s_ops = {
	.startup	= ipq_lpass_mi2s_startup,
	.prepare	= ipq_lpass_mi2s_prepare,
	.hw_params	= ipq_lpass_mi2s_hw_params,
	.shutdown	= ipq_lpass_mi2s_shutdown,
	.set_fmt	= ipq_lpass_mi2s_set_fmt,
};

static struct snd_soc_dai_ops ipq_lpass_spdif_ops = {
	.startup	= ipq_lpass_spdif_startup,
	.prepare	= ipq_lpass_spdif_prepare,
	.hw_params	= ipq_lpass_spdif_hw_params,
	.shutdown	= ipq_lpass_spdif_shutdown,
	.set_fmt	= ipq_lpass_spdif_set_fmt,
};

static struct snd_soc_dai_driver ipq_cpu_dais[] = {
	{
		.playback = {
			.rates		= SNDRV_PCM_RATE_8000 |
					SNDRV_PCM_RATE_16000,
			.formats	= SNDRV_PCM_FMTBIT_S8 |
					SNDRV_PCM_FMTBIT_U8 |
					SNDRV_PCM_FORMAT_U16 |
					SNDRV_PCM_FMTBIT_S16_LE,
			.channels_min	= 1,
			.channels_max	= 2,
			.rate_min	= 8000,
			.rate_max	= 16000,
		},
		.capture = {
			.rates		= SNDRV_PCM_RATE_8000 |
					SNDRV_PCM_RATE_16000,
			.formats	= SNDRV_PCM_FMTBIT_S8 |
					SNDRV_PCM_FMTBIT_U8 |
					SNDRV_PCM_FORMAT_U16 |
					SNDRV_PCM_FMTBIT_S16_LE,
			.channels_min	= 1,
			.channels_max	= 2,
			.rate_min	= 8000,
			.rate_max	= 16000,
		},
		.ops = &ipq_lpass_pcm_ops,
		.name = "ipq-pcm-dai"
	},
	{
		.playback = {
			.rates		= SNDRV_PCM_RATE_8000_192000,
			.formats	= SNDRV_PCM_FMTBIT_S16 |
					SNDRV_PCM_FMTBIT_S24 |
					SNDRV_PCM_FMTBIT_S32,
			.channels_min	= LPASS_STEREO,
			.channels_max	= LPASS_7_1,
			.rate_min	= FREQ_8000,
			.rate_max	= FREQ_192000,
		},
		.capture = {
			.rates		= SNDRV_PCM_RATE_8000_192000,
			.formats	= SNDRV_PCM_FMTBIT_S16 |
					SNDRV_PCM_FMTBIT_S24,
			.channels_min	= LPASS_STEREO,
			.channels_max	= LPASS_STEREO,
			.rate_min	= FREQ_8000,
			.rate_max	= FREQ_192000,
		},
		.ops    = &ipq_lpass_mi2s_ops,
		.name = "ipq-mi2s-dai"
	},
	{
		.playback = {
			.rates		= SNDRV_PCM_RATE_22050 |
					SNDRV_PCM_RATE_32000 |
					SNDRV_PCM_RATE_44100 |
					SNDRV_PCM_RATE_48000 |
					SNDRV_PCM_RATE_88200 |
					SNDRV_PCM_RATE_96000 |
					SNDRV_PCM_RATE_176400 |
					SNDRV_PCM_RATE_192000,
			.formats	= SNDRV_PCM_FMTBIT_S16 |
					SNDRV_PCM_FMTBIT_S24_3LE |
					SNDRV_PCM_FMTBIT_S24,
			.channels_min	= LPASS_STEREO,
			.channels_max	= LPASS_7_1,
			.rate_min	= FREQ_22050,
			.rate_max	= FREQ_192000,
		},
		.ops    = &ipq_lpass_spdif_ops,
		.name = "ipq-spdif-dai"
	},
};

static __devinit int ipq_lpass_dai_probe(struct platform_device *pdev)
{
	int ret;
	ret = snd_soc_register_dais(&pdev->dev, ipq_cpu_dais,
					ARRAY_SIZE(ipq_cpu_dais));
	if (ret)
		dev_err(&pdev->dev,
			"%s: error registering soc dais\n", __func__);

	return ret;
}

static __devexit int ipq_lpass_dai_remove(struct platform_device *pdev)
{
	snd_soc_unregister_dais(&pdev->dev, ARRAY_SIZE(ipq_cpu_dais));
	return 0;
}

static struct platform_driver ipq_lpass_dai_driver = {
	.probe	= ipq_lpass_dai_probe,
	.remove	= ipq_lpass_dai_remove,
	.driver	= {
		.name	= "ipq-cpu-dai",
		.owner	= THIS_MODULE,
	},
};

static int __init ipq_lpass_dai_init(void)
{
	int ret;
	ret = platform_driver_register(&ipq_lpass_dai_driver);

	if (ret)
		pr_err("%s: %d:cpu dai registration failed\n",
					__func__, __LINE__);

	return ret;
}

static void __exit ipq_lpass_dai_exit(void)
{
	platform_driver_unregister(&ipq_lpass_dai_driver);
}

module_init(ipq_lpass_dai_init);
module_exit(ipq_lpass_dai_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("IPQ806x CPU DAI DRIVER");
