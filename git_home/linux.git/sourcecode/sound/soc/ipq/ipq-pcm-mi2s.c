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
#include <linux/err.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/time.h>
#include <linux/wait.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/dma-mapping.h>
#include <linux/memory_alloc.h>
#include <asm/dma.h>
#include <sound/core.h>
#include <sound/soc.h>
#include <sound/soc-dapm.h>
#include <sound/pcm.h>
#include <sound/initval.h>
#include <sound/control.h>
#include <sound/dai.h>
#include <sound/pcm_params.h>
#include "ipq-lpaif.h"
#include "ipq-dmlite.h"
#include "ipq-pcm.h"
#include "ipq806x.h"

extern struct lpass_dml_baseinfo dml_info;
extern struct ipq_lpass_runtime_data_t *gprtd;

static struct snd_pcm_hardware ipq_pcm_hardware_playback = {
	.info			=	SNDRV_PCM_INFO_MMAP |
					SNDRV_PCM_INFO_BLOCK_TRANSFER |
					SNDRV_PCM_INFO_MMAP_VALID |
					SNDRV_PCM_INFO_INTERLEAVED |
					SNDRV_PCM_INFO_PAUSE |
					SNDRV_PCM_INFO_RESUME,
	.formats		=	SNDRV_PCM_FMTBIT_S16 |
					SNDRV_PCM_FMTBIT_S24 |
					SNDRV_PCM_FMTBIT_S32,
	.rates			=	SNDRV_PCM_RATE_8000_192000,
	.rate_min		=	FREQ_8000,
	.rate_max		=	FREQ_192000,
	.channels_min		=	LPASS_STEREO,
	.channels_max		=	LPASS_7_1,
	.buffer_bytes_max	=	LPASS_MI2S_BUFF_SIZE,
	.period_bytes_max	=	(LPASS_MI2S_BUFF_SIZE) / 2,
	.period_bytes_min	=	LPASS_MI2S_PERIOD_BYTES_MIN,
	.periods_min		=	LPASS_MI2S_NO_OF_PERIODS,
	.periods_max		=	LPASS_MI2S_NO_OF_PERIODS,
	.fifo_size		=	0,
};

static struct snd_pcm_hardware ipq_pcm_hardware_capture = {
	.info			=	SNDRV_PCM_INFO_MMAP |
					SNDRV_PCM_INFO_BLOCK_TRANSFER |
					SNDRV_PCM_INFO_MMAP_VALID |
					SNDRV_PCM_INFO_INTERLEAVED,
	.formats		=	SNDRV_PCM_FMTBIT_S16 |
					SNDRV_PCM_FMTBIT_S24,
	.rates			=	SNDRV_PCM_RATE_8000_192000,
	.rate_min		=	FREQ_8000,
	.rate_max		=	FREQ_192000,
	.channels_min		=	LPASS_STEREO,
	.channels_max		=	LPASS_STEREO,
	.buffer_bytes_max	=	LPASS_MI2S_CAPTURE_BUFF_SIZE,
	.period_bytes_max	=	(LPASS_MI2S_CAPTURE_BUFF_SIZE) / 2,
	.period_bytes_min	=	LPASS_MI2S_PERIOD_BYTES_MIN,
	.periods_min		=	LPASS_MI2S_NO_OF_PERIODS,
	.periods_max		=	LPASS_MI2S_NO_OF_PERIODS,
	.fifo_size		=	0,
};

static int ipq_pcm_preallocate_dma_buffer(struct snd_pcm *pcm,
						int stream)
{
	struct snd_pcm_substream *substream = pcm->streams[stream].substream;
	struct snd_dma_buffer *buf = &substream->dma_buffer;
	size_t size;

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
		size = ipq_pcm_hardware_playback.buffer_bytes_max;
	else if (substream->stream == SNDRV_PCM_STREAM_CAPTURE)
		size = ipq_pcm_hardware_capture.buffer_bytes_max;
	else
		return -EINVAL;

	buf->dev.type = SNDRV_DMA_TYPE_DEV;
	buf->dev.dev = pcm->card->dev;
	buf->private_data = NULL;
	buf->area = dma_alloc_coherent(pcm->card->dev, size,
					&buf->addr, GFP_KERNEL);
	if (!buf->area)
		return -ENOMEM;
	buf->bytes = size;

	return 0;
}

static void ipq_pcm_free_dma_buffer(struct snd_pcm *pcm, int stream)
{
	struct snd_pcm_substream *substream;
	struct snd_dma_buffer *buf;

	substream = pcm->streams[stream].substream;
	buf = &substream->dma_buffer;
	if (buf->area) {
		dma_free_coherent(pcm->card->dev, buf->bytes,
					buf->area, buf->addr);
	}
	buf->area = NULL;
}

static irqreturn_t ipq_mi2s_irq(int intrsrc, void *data)
{
	int dma_ch;
	uint32_t ret = IRQ_NONE;
	uint32_t has_xrun, pending;

	struct snd_pcm_substream *substream = data;
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct ipq_lpass_runtime_data_t *prtd =
		(struct ipq_lpass_runtime_data_t *)runtime->private_data;

	if (prtd)
		dma_ch = prtd->lpaif_info.dma_ch;
	else
		return IRQ_NONE;

	pending = (intrsrc
		& (UNDER_CH(dma_ch) | PER_CH(dma_ch) | ERR_CH(dma_ch)));

	has_xrun = (pending & UNDER_CH(dma_ch));

	if (unlikely(has_xrun) && substream->runtime &&
			snd_pcm_running(substream)) {
		pr_debug("%s %d: xrun warning\n", __func__, __LINE__);
		snd_pcm_stop(substream, SNDRV_PCM_STATE_XRUN);
		pending &= ~UNDER_CH(dma_ch);
		ret = IRQ_HANDLED;
	}

	if (pending & PER_CH(dma_ch)) {
		if (++prtd->pcm_stream_info.period_index >= runtime->periods)
			prtd->pcm_stream_info.period_index = 0;
		snd_pcm_period_elapsed(substream);
		pending &= ~PER_CH(dma_ch);
		ret = IRQ_HANDLED;
	}

	if (pending & UNDER_CH(dma_ch)) {
		snd_pcm_period_elapsed(substream);
		pr_debug("%s %d: xrun warning\n", __func__, __LINE__);
		ret = IRQ_HANDLED;
	}

	if (pending & ERR_CH(dma_ch)) {
		pr_debug("%s %d: Bus access warning\n", __func__, __LINE__);
		ret = IRQ_HANDLED;
	}

	return ret;
}

static snd_pcm_uframes_t ipq_pcm_mi2s_pointer(
				struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct ipq_lpass_runtime_data_t *prtd =
		(struct ipq_lpass_runtime_data_t *)runtime->private_data;
	snd_pcm_uframes_t offset;

	offset = prtd->pcm_stream_info.period_index * runtime->period_size;

	return offset >= (runtime->buffer_size) ? 0 : offset;
}

static int ipq_pcm_mi2s_mmap(struct snd_pcm_substream *substream,
				struct vm_area_struct *vma)
{
	struct snd_pcm_runtime *runtime = substream->runtime;

	return dma_mmap_coherent(substream->pcm->card->dev, vma,
		runtime->dma_area, runtime->dma_addr, runtime->dma_bytes);
}

static int ipq_pcm_mi2s_prepare(struct snd_pcm_substream *substream)
{
	int ret;
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct dai_dma_params dma_params;
	struct ipq_lpass_runtime_data_t *prtd =
		(struct ipq_lpass_runtime_data_t *)runtime->private_data;

	if (!prtd) {
		pr_err("%s %d:Error in getting runtime data\n",
						__func__, __LINE__);
		return -EINVAL;
	}

	/*
	 * This is the case for under\over-run, we have already
	 * configured the DMA registers for this stream
	 */
	if (prtd->pcm_stream_info.pcm_prepare_start)
		return 0;

	ipq_lpaif_dma_stop(prtd->lpaif_info.dma_ch);
	prtd->pcm_stream_info.pcm_prepare_start = 1;
	prtd->lpaif_info.lpa_if_dma_start = 0;

	memset(&dma_params, 0, sizeof(dma_params));
	dma_params.src_start = runtime->dma_addr;
	dma_params.buffer_size = snd_pcm_lib_buffer_bytes(substream);
	dma_params.period_size = snd_pcm_lib_period_bytes(substream);
	dma_params.channels = runtime->channels;
	ret = ipq_lpaif_cfg_dma(prtd->lpaif_info.dma_ch, &dma_params,
					prtd->pcm_stream_info.bit_width,
					1 /*enable intr*/);
	if (ret)
		return -EINVAL;

	ipq_lpaif_register_dma_irq_handler(prtd->lpaif_info.dma_ch,
					ipq_mi2s_irq, (void *)substream);

	return 0;
}

static int ipq_pcm_mi2s_close(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct ipq_lpass_runtime_data_t *prtd =
		(struct ipq_lpass_runtime_data_t *)runtime->private_data;

	if (prtd) {
		ipq_lpaif_dai_stop(prtd->lpaif_info.dma_ch);
		ipq_lpaif_unregister_dma_irq_handler(prtd->lpaif_info.dma_ch);
		kfree(prtd);
	}

	return 0;
}

static int ipq_pcm_mi2s_trigger(struct snd_pcm_substream *substream, int cmd)
{
	int ret = 0;

	switch (cmd) {
	case SNDRV_PCM_TRIGGER_START:
	case SNDRV_PCM_TRIGGER_RESUME:
	case SNDRV_PCM_TRIGGER_PAUSE_RELEASE:
		if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
			ipq_cfg_i2s_spkr(1, 0, LPA_IF_MI2S);
		else if (substream->stream == SNDRV_PCM_STREAM_CAPTURE)
			ipq_cfg_i2s_mic(1, LPA_IF_MI2S);
		else
			ret = -EINVAL;
		break;
	case SNDRV_PCM_TRIGGER_STOP:
	case SNDRV_PCM_TRIGGER_SUSPEND:
	case SNDRV_PCM_TRIGGER_PAUSE_PUSH:
		if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
			ipq_cfg_i2s_spkr(0, 0, LPA_IF_MI2S);
		else if (substream->stream == SNDRV_PCM_STREAM_CAPTURE)
			ipq_cfg_i2s_mic(0, LPA_IF_MI2S);
		else
			ret = -EINVAL;
		break;
	default:
		ret = -EINVAL;
		break;
	}
	return ret;
}

static int ipq_pcm_mi2s_hw_params(struct snd_pcm_substream *substream,
					struct snd_pcm_hw_params *params)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct ipq_lpass_runtime_data_t *prtd =
		(struct ipq_lpass_runtime_data_t *)runtime->private_data;

	pr_debug("%s %d\n", __func__, __LINE__);
	snd_pcm_set_runtime_buffer(substream, &substream->dma_buffer);
	prtd->pcm_stream_info.pcm_prepare_start = 0;
	prtd->pcm_stream_info.period_index = 0;
	return 0;
}

static int ipq_pcm_mi2s_open(struct snd_pcm_substream *substream)
{
	int ret;
	struct ipq_lpass_runtime_data_t *prtd;
	struct snd_pcm_runtime *runtime = substream->runtime;

	pr_debug("%s %d\n", __func__, __LINE__);

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) {
		runtime->dma_bytes = ipq_pcm_hardware_playback.buffer_bytes_max;
		snd_soc_set_runtime_hwparams(substream,
				&ipq_pcm_hardware_playback);
	} else if (substream->stream == SNDRV_PCM_STREAM_CAPTURE) {
		runtime->dma_bytes = ipq_pcm_hardware_capture.buffer_bytes_max;
		snd_soc_set_runtime_hwparams(substream,
					&ipq_pcm_hardware_capture);
	} else {
		pr_err("%s: Invalid stream\n", __func__);
		return -EINVAL;
	}

	ret = snd_pcm_hw_constraint_integer(runtime,
			SNDRV_PCM_HW_PARAM_PERIODS);
	if (ret < 0) {
		pr_err("%s: snd_pcm_hw_constraint_integer failed\n", __func__);
		return -EINVAL;
	}

	prtd = kzalloc(sizeof(struct ipq_lpass_runtime_data_t), GFP_KERNEL);
	if (prtd == NULL) {
		pr_err("%s: %d: Error allocating prtd\n", __func__, __LINE__);
		return -ENOMEM;
	}

	prtd->pcm_stream_info.pcm_prepare_start = 0;
	prtd->lpaif_clk.is_bit_clk_enabled = 0;
	prtd->lpaif_clk.is_osr_clk_enabled = 0;
	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
		prtd->lpaif_info.dma_ch = MI2S_DMA_RD_CH;
	else
		prtd->lpaif_info.dma_ch = MI2S_DMA_WR_CH;

	prtd->pcm_stream_info.substream = substream;
	runtime->private_data = prtd;
	gprtd = prtd;

	return 0;
}

static struct snd_pcm_ops ipq_asoc_pcm_mi2s_ops = {
	.open		= ipq_pcm_mi2s_open,
	.hw_params	= ipq_pcm_mi2s_hw_params,
	.trigger	= ipq_pcm_mi2s_trigger,
	.ioctl		= snd_pcm_lib_ioctl,
	.close		= ipq_pcm_mi2s_close,
	.prepare	= ipq_pcm_mi2s_prepare,
	.mmap		= ipq_pcm_mi2s_mmap,
	.pointer	= ipq_pcm_mi2s_pointer,
};

static void ipq_asoc_pcm_mi2s_free(struct snd_pcm *pcm)
{
	ipq_pcm_free_dma_buffer(pcm, SNDRV_PCM_STREAM_PLAYBACK);
	ipq_pcm_free_dma_buffer(pcm, SNDRV_PCM_STREAM_CAPTURE);
}

static int ipq_asoc_pcm_mi2s_new(struct snd_soc_pcm_runtime *prtd)
{
	struct snd_card *card = prtd->card->snd_card;
	struct snd_pcm *pcm = prtd->pcm;
	int ret = 0;

	if (!card->dev->coherent_dma_mask)
		card->dev->coherent_dma_mask = DMA_BIT_MASK(32);

	if (!card->dev->dma_mask)
		card->dev->dma_mask = &card->dev->coherent_dma_mask;

	if (pcm->streams[SNDRV_PCM_STREAM_PLAYBACK].substream) {
		ret = ipq_pcm_preallocate_dma_buffer(pcm,
				SNDRV_PCM_STREAM_PLAYBACK);
		if (ret) {
			pr_err("%s: %d: Error allocating dma buf\n",
						__func__, __LINE__);
			return -ENOMEM;
		}
	}

	if (pcm->streams[SNDRV_PCM_STREAM_CAPTURE].substream) {
		ret = ipq_pcm_preallocate_dma_buffer(pcm,
				SNDRV_PCM_STREAM_CAPTURE);
		if (ret) {
			pr_err("%s: %d: Error allocating dma buf\n",
						__func__, __LINE__);
			ipq_pcm_free_dma_buffer(pcm, SNDRV_PCM_STREAM_PLAYBACK);
			return -ENOMEM;
		}
	}
	return ret;
}

static struct snd_soc_platform_driver ipq_asoc_pcm_mi2s_platform = {
	.ops		= &ipq_asoc_pcm_mi2s_ops,
	.pcm_new	= ipq_asoc_pcm_mi2s_new,
	.pcm_free	= ipq_asoc_pcm_mi2s_free,
};

static __devinit int ipq_pcm_mi2s_driver_probe(struct platform_device *pdev)
{
	int ret;
	pr_debug("%s %d\n", __func__, __LINE__);
	ret = snd_soc_register_platform(&pdev->dev,
			&ipq_asoc_pcm_mi2s_platform);
	if (ret)
		dev_err(&pdev->dev, "%s: Failed to register mi2s pcm device\n",
								__func__);

	return ret;
}

static __devexit int ipq_pcm_mi2s_driver_remove(struct platform_device *pdev)
{
	pr_debug("%s %d\n", __func__, __LINE__);
	snd_soc_unregister_platform(&pdev->dev);

	return 0;
}

static struct platform_driver ipq_pcm_mi2s_driver = {
	.driver	= {
		.name	= "ipq-pcm-mi2s",
		.owner	= THIS_MODULE,
	},
	.probe	= ipq_pcm_mi2s_driver_probe,
	.remove	= ipq_pcm_mi2s_driver_remove,
};

static int __init ipq_lpass_pcm_mi2s_init(void)
{
	int ret;
	pr_debug("%s %d\n", __func__, __LINE__);
	ret = platform_driver_register(&ipq_pcm_mi2s_driver);
	if (ret)
		pr_err("%s: Failed to register mi2s driver\n", __func__);

	return ret;
}

static void __exit ipq_lpass_pcm_mi2s_exit(void)
{
	pr_debug("%s %d\n", __func__, __LINE__);
	platform_driver_unregister(&ipq_pcm_mi2s_driver);
}

module_init(ipq_lpass_pcm_mi2s_init);
module_exit(ipq_lpass_pcm_mi2s_exit);

MODULE_DESCRIPTION("IPQ PCM MI2S Platform Driver");
MODULE_LICENSE("GPL v2");
