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
#include <sound/core.h>
#include <sound/soc.h>
#include <sound/soc-dapm.h>
#include <sound/pcm.h>
#include <sound/initval.h>
#include <sound/control.h>
#include <asm/dma.h>
#include <linux/memory_alloc.h>
#include <sound/dai.h>
#include <linux/dma-mapping.h>
#include <linux/slab.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <sound/dai.h>
#include "ipq-lpaif.h"
#include "ipq-pcm.h"

static struct snd_pcm_hardware ipq_pcm_hardware_playback = {
	.info		=	SNDRV_PCM_INFO_MMAP |
				SNDRV_PCM_INFO_BLOCK_TRANSFER |
				SNDRV_PCM_INFO_MMAP_VALID |
				SNDRV_PCM_INFO_INTERLEAVED |
				SNDRV_PCM_INFO_PAUSE |
				SNDRV_PCM_INFO_RESUME,
	.formats	=	SNDRV_PCM_FMTBIT_S16_LE |
				SNDRV_PCM_FORMAT_U16 |
				SNDRV_PCM_FMTBIT_U8 |
				SNDRV_PCM_FMTBIT_S8,
	.rates		=	SNDRV_PCM_RATE_8000 |
				SNDRV_PCM_RATE_16000,
	.rate_min	=	8000,
	.rate_max	=	16000,
	.channels_min	=	1,
	.channels_max	=	2,
	.buffer_bytes_max =	32768,
	.period_bytes_min =	128,
	.period_bytes_max =	32768 / 2,
	.periods_min	=	4,
	.periods_max	=	512,
	.fifo_size	=	0,
};

static struct snd_pcm_hardware ipq_pcm_hardware_capture = {
	.info		=	SNDRV_PCM_INFO_MMAP |
				SNDRV_PCM_INFO_BLOCK_TRANSFER |
				SNDRV_PCM_INFO_MMAP_VALID |
				SNDRV_PCM_INFO_INTERLEAVED |
				SNDRV_PCM_INFO_PAUSE |
				SNDRV_PCM_INFO_RESUME,
        .formats	=	SNDRV_PCM_FMTBIT_S16_LE |
				SNDRV_PCM_FORMAT_U16 |
				SNDRV_PCM_FMTBIT_U8 |
				SNDRV_PCM_FMTBIT_S8,
        .rates		=	SNDRV_PCM_RATE_8000 |
				SNDRV_PCM_RATE_16000,
        .rate_min	=	8000,
        .rate_max	=	16000,
        .channels_min	=	1,
        .channels_max	=	2,
        .buffer_bytes_max	=	32768,
        .period_bytes_min	=	128,
        .period_bytes_max	=	32768 / 2,
        .periods_min	=	4,
        .periods_max	=	512,
        .fifo_size	=	0,
};

static int ipq_pcm_preallocate_dma_buffer(struct snd_pcm *pcm,
						int stream)
{
	struct snd_pcm_substream *substream = pcm->streams[stream].substream;
	struct snd_dma_buffer *buf = &substream->dma_buffer;
	size_t size;

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
		size = ipq_pcm_hardware_playback.buffer_bytes_max;
	else
		size = ipq_pcm_hardware_capture.buffer_bytes_max;

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

static irqreturn_t ipq_pcm_irq(int intrsrc, void *data)
{
	struct snd_pcm_substream *substream = data;
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct ipq_lpass_runtime_data_t *prtd =
		(struct ipq_lpass_runtime_data_t *)runtime->private_data;
	int dma_ch;
	unsigned int has_xrun, pending;
	unsigned int ret = IRQ_NONE;

	if (prtd)
		dma_ch = prtd->lpaif_info.dma_ch;
	else
		return ret;

	pending = (intrsrc
		& (UNDER_CH(dma_ch) | PER_CH(dma_ch) | ERR_CH(dma_ch)));

	has_xrun = (pending & UNDER_CH(dma_ch));

	if (unlikely(has_xrun) && substream->runtime &&
		snd_pcm_running(substream)) {
		pr_debug("xrun\n");
		snd_pcm_stop(substream, SNDRV_PCM_STATE_XRUN);
		pending &= ~UNDER_CH(dma_ch);
		ret = IRQ_HANDLED;
	}

	if (pending & PER_CH(dma_ch)) {
		if (++prtd->pcm_stream_info.period_index >= runtime->periods)
			prtd->pcm_stream_info.period_index = 0;

		snd_pcm_period_elapsed(substream);
		pr_debug("period elapsed\n");
		pending &= ~PER_CH(dma_ch);
		ret = IRQ_HANDLED;
	}

	if (pending & UNDER_CH(dma_ch)) {
		pr_err("under run\n");
		ret = IRQ_HANDLED;
	}

	if (pending & ERR_CH(dma_ch)) {
		pr_err("DMA %x Access Error\n", dma_ch);
		ret = IRQ_HANDLED;
	}

	return ret;
}

static snd_pcm_uframes_t ipq_pcm_pointer(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct ipq_lpass_runtime_data_t *prtd =
		(struct ipq_lpass_runtime_data_t *)runtime->private_data;
	snd_pcm_uframes_t offset;

	offset = prtd->pcm_stream_info.period_index * runtime->period_size;

	return offset >= (runtime->buffer_size) ? 0 : offset;
}

int ipq_pcm_mmap(struct snd_pcm_substream *substream,
				struct vm_area_struct *vma)
{
	struct snd_pcm_runtime *runtime = substream->runtime;

	pr_debug("%s\n", __func__);

	snd_pcm_set_runtime_buffer(substream, &substream->dma_buffer);
	return dma_mmap_coherent(substream->pcm->card->dev, vma,
		runtime->dma_area, runtime->dma_addr, runtime->dma_bytes);
}

static int ipq_pcm_prepare(struct snd_pcm_substream *substream)
{
	int dma_ch;
	int ret;
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct ipq_lpass_runtime_data_t *prtd =
		(struct ipq_lpass_runtime_data_t *)runtime->private_data;
	struct dai_dma_params dma_params;

	if (prtd)
		dma_ch = prtd->lpaif_info.dma_ch;
	else
		return -EINVAL;

	if (prtd->pcm_stream_info.pcm_prepare_start)
		return 0;

	dma_params.src_start = runtime->dma_addr;
	dma_params.buffer = (u8 *)runtime->dma_area;
	dma_params.buffer_size = snd_pcm_lib_buffer_bytes(substream);
	dma_params.period_size = snd_pcm_lib_period_bytes(substream);
	dma_params.channels = runtime->channels;

	ret = ipq_lpaif_cfg_dma(dma_ch, &dma_params,
			prtd->pcm_stream_info.bit_width,
			1 /* enable intr */);
	if (ret)
		return -EINVAL;

	ipq_lpaif_register_dma_irq_handler(dma_ch, ipq_pcm_irq,
						(void *)substream);

	prtd->pcm_stream_info.pcm_prepare_start = 1;
	return 0;
}

static int ipq_pcm_close(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct ipq_lpass_runtime_data_t *prtd =
		(struct ipq_lpass_runtime_data_t *)runtime->private_data;
	uint32_t dma_ch;

	pr_debug("%s\n", __func__);
	if (prtd) {
		dma_ch = prtd->lpaif_info.dma_ch;
		ipq_lpaif_unregister_dma_irq_handler(prtd->lpaif_info.dma_ch);
		kfree(prtd);
		ipq_lpaif_pcm_stop(dma_ch);
	}

	return 0;
}

static int ipq_pcm_trigger(struct snd_pcm_substream *substream, int cmd)
{
	int ret = 0;

	switch (cmd) {
	case SNDRV_PCM_TRIGGER_START:
	case SNDRV_PCM_TRIGGER_RESUME:
	case SNDRV_PCM_TRIGGER_PAUSE_RELEASE:
		ipq_pcm_start();
		break;
	case SNDRV_PCM_TRIGGER_STOP:
	case SNDRV_PCM_TRIGGER_SUSPEND:
	case SNDRV_PCM_TRIGGER_PAUSE_PUSH:
		ipq_pcm_stop();
		break;
	default:
		ret = -EINVAL;
	}
	return ret;
}

static int ipq_pcm_hw_params(struct snd_pcm_substream *substream,
				struct snd_pcm_hw_params *params)
{
	pr_debug("%s\n", __func__);
	snd_pcm_set_runtime_buffer(substream, &substream->dma_buffer);
	return 0;
}

static int ipq_pcm_open(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;

	struct ipq_lpass_runtime_data_t *prtd;
	int ret;

	pr_debug("%s\n", __func__);

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) {
		snd_soc_set_runtime_hwparams(substream,
				&ipq_pcm_hardware_playback);
		runtime->dma_bytes =
				ipq_pcm_hardware_playback.buffer_bytes_max;
	} else if (substream->stream == SNDRV_PCM_STREAM_CAPTURE) {
		snd_soc_set_runtime_hwparams(substream,
				&ipq_pcm_hardware_capture);
		runtime->dma_bytes = ipq_pcm_hardware_capture.buffer_bytes_max;
	}

	ret = snd_pcm_hw_constraint_integer(runtime,
				SNDRV_PCM_HW_PARAM_PERIODS);

	prtd = kzalloc(sizeof(struct ipq_lpass_runtime_data_t), GFP_KERNEL);

	if (prtd == NULL) {
		pr_err("%s: Error allocating prtd\n", __func__);
		ret = -ENOMEM;
		goto err;
	}

	prtd->pcm_stream_info.pcm_prepare_start = 0;
	prtd->lpaif_clk.is_bit_clk_enabled = 0;

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
		prtd->lpaif_info.dma_ch = PCM0_DMA_RD_CH;
	else
		prtd->lpaif_info.dma_ch = PCM0_DMA_WR_CH;

	prtd->pcm_stream_info.substream = substream;
	runtime->private_data = prtd;

err:
	return ret;
}

static struct snd_pcm_ops ipq_asoc_pcm_ops = {
	.open		= ipq_pcm_open,
	.hw_params	= ipq_pcm_hw_params,
	.trigger	= ipq_pcm_trigger,
	.ioctl		= snd_pcm_lib_ioctl,
	.close		= ipq_pcm_close,
	.prepare	= ipq_pcm_prepare,
	.mmap		= ipq_pcm_mmap,
	.pointer	= ipq_pcm_pointer,
};

static void ipq_asoc_pcm_free(struct snd_pcm *pcm)
{
	struct snd_pcm_substream *substream;
	struct snd_dma_buffer *buf;
	int stream;

	for (stream = 0; stream < 2; stream++) {
		substream = pcm->streams[stream].substream;
		if (!stream)
			continue;

		buf = &substream->dma_buffer;
		if (!buf->area)
			continue;

		dma_free_coherent(pcm->card->dev, buf->bytes,
					buf->area, buf->addr);
		buf->area = NULL;
	}
}

static int ipq_asoc_pcm_new(struct snd_soc_pcm_runtime *rtd)
{
	struct snd_card *card = rtd->card->snd_card;
	struct snd_pcm *pcm = rtd->pcm;
	int ret = 0;

	if (!card->dev->coherent_dma_mask)
		card->dev->coherent_dma_mask = DMA_BIT_MASK(32);

	if (!card->dev->dma_mask)
		card->dev->dma_mask = &card->dev->coherent_dma_mask;

	if (pcm->streams[SNDRV_PCM_STREAM_PLAYBACK].substream) {
		ret = ipq_pcm_preallocate_dma_buffer(pcm,
			SNDRV_PCM_STREAM_PLAYBACK);
		if (ret)
			return ret;
	}
	if (pcm->streams[SNDRV_PCM_STREAM_CAPTURE].substream) {
		ret = ipq_pcm_preallocate_dma_buffer(pcm,
			SNDRV_PCM_STREAM_CAPTURE);
		if (ret)
			return ret;
	}
	return ret;
}

static struct snd_soc_platform_driver ipq_asoc_pcm_platform = {
	.ops		= &ipq_asoc_pcm_ops,
	.pcm_new	= ipq_asoc_pcm_new,
	.pcm_free	= ipq_asoc_pcm_free,
};

static __devinit int ipq_pcm_driver_probe(struct platform_device *pdev)
{
	int ret;
	ret = snd_soc_register_platform(&pdev->dev, &ipq_asoc_pcm_platform);
	if (ret)
		dev_err(&pdev->dev,
		"%s: Failed to register snd platform driver\n", __func__);

	return ret;
}

static __devexit int ipq_pcm_driver_remove(struct platform_device *pdev)
{
	snd_soc_unregister_platform(&pdev->dev);
	return 0;
}

static struct platform_driver ipq_pcm_driver = {
	.driver	= {
		.name	= "ipq-pcm-voip",
		.owner	= THIS_MODULE,
	},
	.probe	= ipq_pcm_driver_probe,
	.remove	= ipq_pcm_driver_remove,
};

static int __init ipq_lpass_pcm_init(void)
{
	int ret;
	ret = platform_driver_register(&ipq_pcm_driver);
	if (ret)
		pr_err("%s: Failed to register VoIP platform driver\n",
								__func__);

	return ret;
}

static void __exit ipq_lpass_pcm_exit(void)
{
	platform_driver_unregister(&ipq_pcm_driver);
}

module_init(ipq_lpass_pcm_init);
module_exit(ipq_lpass_pcm_exit);

MODULE_DESCRIPTION("IPQ PCM VoIP Platform Driver");
MODULE_LICENSE("GPL v2");
