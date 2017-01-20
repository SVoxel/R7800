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
#include <linux/delay.h>

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
#include "ipq-spdif.h"

extern int ipq_dml_audio_trigger(uint32_t src_addr, size_t count,
						uint8_t dma_ch);
extern void ipq_spdif_intr_enable(void);
extern void ipq_lpaif_spdif_port_en(uint8_t en);
extern struct ipq_lpaif_spdif_baseinfo ipq_spdif_info;

extern void ipq_spdif_register_handler(
	irqreturn_t (*callback) (int intrsrc, void *private_data),
	void *private_data);


static struct snd_pcm_hardware ipq_pcm_hardware_playback = {
	.info =			SNDRV_PCM_INFO_MMAP |
				SNDRV_PCM_INFO_BLOCK_TRANSFER |
				SNDRV_PCM_INFO_MMAP_VALID |
				SNDRV_PCM_INFO_INTERLEAVED |
				SNDRV_PCM_INFO_PAUSE |
				SNDRV_PCM_INFO_RESUME,
	.formats =		SNDRV_PCM_FMTBIT_S16 |
				SNDRV_PCM_FMTBIT_S24,
	.rates = 		SNDRV_PCM_RATE_22050 |
				SNDRV_PCM_RATE_32000 |
				SNDRV_PCM_RATE_44100 |
				SNDRV_PCM_RATE_48000 |
				SNDRV_PCM_RATE_88200 |
				SNDRV_PCM_RATE_96000 |
				SNDRV_PCM_RATE_176400 |
				SNDRV_PCM_RATE_192000,
	.rate_min =		22050,
	.rate_max =		192000,
	.channels_min =		2,
	.channels_max =		2, /* spdif supports only stereo */
	.buffer_bytes_max =	SPDIF_BUF_MAX_BYTES,
	.period_bytes_min =	760,
	.period_bytes_max =	SPDIF_BUF_MAX_BYTES / 2,
	.periods_min =		SPDIF_PERIODS_MIN,
	.periods_max =		SPDIF_PERIODS_MAX,
	.fifo_size =		0,
};

struct ipq_spdif_buf_info spdif_buf_info;

static irqreturn_t ipq_pcm_spdif(int src, void *data)
{
	struct snd_pcm_substream *substream = data;
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct ipq_lpass_runtime_data_t *prtd =
		(struct ipq_lpass_runtime_data_t *)runtime->private_data;

	if (spdif_buf_info.curr_buf >=
		(spdif_buf_info.start_buf + spdif_buf_info.size_buf)
				- (prtd->pcm_stream_info.spdif_frame_size * 4))
		spdif_buf_info.curr_buf = spdif_buf_info.start_buf;
	else
		spdif_buf_info.curr_buf +=
			(prtd->pcm_stream_info.spdif_frame_size * 4);

	if (src & 0x1) {
		writel(spdif_buf_info.curr_buf,
			ipq_spdif_info.base + LPA_IF_SPDIF_BUF_PNTR);
		writel(0, ipq_spdif_info.base + LPA_IF_SPDIF_TX_SUBBUF_FIFO);
	}

	if (src & 0x2) {
		writel(spdif_buf_info.curr_buf,
			ipq_spdif_info.base + LPA_IF_SPDIF_BUF_PNTR);
		writel(0, ipq_spdif_info.base + LPA_IF_SPDIF_TX_SUBBUF_FIFO);

		if (spdif_buf_info.curr_buf >=
			(spdif_buf_info.start_buf + spdif_buf_info.size_buf)
				- (prtd->pcm_stream_info.spdif_frame_size * 4))
			spdif_buf_info.curr_buf = spdif_buf_info.start_buf;
		else
			spdif_buf_info.curr_buf +=
				(prtd->pcm_stream_info.spdif_frame_size * 4);

		writel(spdif_buf_info.curr_buf,
			ipq_spdif_info.base + LPA_IF_SPDIF_BUF_PNTR);
		writel(0, ipq_spdif_info.base + LPA_IF_SPDIF_TX_SUBBUF_FIFO);
	}

	prtd->pcm_stream_info.blocks_done_per_period ++;

	/*
	 * for Linear PCM, inform ALSA only of all blocks in a period are done
	 */
	if (prtd->pcm_stream_info.blocks_done_per_period !=
		prtd->pcm_stream_info.blocks_per_period) {
		/* period not done yet, don't inform upper layers */
		return IRQ_HANDLED;
	}

	if (++prtd->pcm_stream_info.period_index >= runtime->periods)
		prtd->pcm_stream_info.period_index = 0;

	snd_pcm_period_elapsed(substream);
	prtd->pcm_stream_info.blocks_done_per_period = 0;

	return IRQ_HANDLED;
}

static int ipq_pcm_preallocate_dma_buffer(struct snd_pcm *pcm,
						int stream)
{
	struct snd_pcm_substream *substream = pcm->streams[stream].substream;
	struct snd_dma_buffer *buf = &substream->dma_buffer;
	size_t size;

	size = ipq_pcm_hardware_playback.buffer_bytes_max;

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

static snd_pcm_uframes_t ipq_pcm_spdif_pointer(
			struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct ipq_lpass_runtime_data_t *prtd =
		(struct ipq_lpass_runtime_data_t *)runtime->private_data;
	snd_pcm_uframes_t offset;

	offset = prtd->pcm_stream_info.period_index * runtime->period_size;

	return offset >= (runtime->buffer_size) ? 0 : offset;
}

int ipq_pcm_spdif_mmap(struct snd_pcm_substream *substream,
				struct vm_area_struct *vma)
{
	struct snd_pcm_runtime *runtime = substream->runtime;

	pr_debug("%s\n", __func__);

	return dma_mmap_coherent(substream->pcm->card->dev, vma,
					runtime->dma_area,
					runtime->dma_addr,
					runtime->dma_bytes);
}

static int ipq_pcm_spdif_prepare(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct ipq_lpass_runtime_data_t *prtd =
		(struct ipq_lpass_runtime_data_t *)runtime->private_data;

	if (prtd->pcm_stream_info.pcm_prepare_start)
		return 0;

	/* make sure the period size is multiple of 192 for PCM */
	if (prtd->pcm_stream_info.compr_mode ==
		LPA_IF_SPDIF_TX_DATA_TYPE_LINEAR) {
		if (runtime->period_size % SPDIF_FRAMESIZE) {
			prtd->pcm_stream_info.spdif_frame_size =
			((frames_to_bytes(runtime, runtime->period_size)) / 4);
			prtd->pcm_stream_info.blocks_per_period = 1;
		} else {
			prtd->pcm_stream_info.blocks_per_period =
			((frames_to_bytes(runtime, runtime->period_size)) /
						(SPDIF_FRAMESIZE * 4));
		}
	} else {
		/* for non linear PCM, one block is one period */
		prtd->pcm_stream_info.blocks_per_period =
			(runtime->period_size /
				prtd->pcm_stream_info.spdif_frame_size);
	}
	ipq_spdif_intr_enable();
	ipq_spdif_register_handler(ipq_pcm_spdif, substream);
	ipq_spdif_cfg_compr_mode(prtd->pcm_stream_info.compr_mode,
				prtd->pcm_stream_info.spdif_frame_size);

	spdif_buf_info.curr_buf = ipq_spdif_set_params(runtime->dma_addr,
				prtd->pcm_stream_info.spdif_frame_size);
	spdif_buf_info.start_buf = runtime->dma_addr;
	spdif_buf_info.size_buf =
		frames_to_bytes(runtime, runtime->buffer_size);

	prtd->pcm_stream_info.pcm_prepare_start = 1;
	return 0;
}

static int ipq_pcm_spdif_close(struct snd_pcm_substream *substream)
{

	struct snd_pcm_runtime *runtime = substream->runtime;
	struct ipq_lpass_runtime_data_t *prtd =
		(struct ipq_lpass_runtime_data_t *)runtime->private_data;

	/*
	 * Disable SPDIF interrupts
	 */
	writel(0x0, ipq_spdif_info.base + LPA_IF_SPDIF_TX_INT_CNTL);

	pr_debug("%s\n", __func__);
	if (prtd)
		kfree(prtd);
	return 0;
}

static int ipq_pcm_spdif_trigger(struct snd_pcm_substream *substream, int cmd)
{
	int ret = 0;
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct ipq_lpass_runtime_data_t *prtd =
		(struct ipq_lpass_runtime_data_t *)runtime->private_data;

	switch (cmd) {
	case SNDRV_PCM_TRIGGER_START:
	case SNDRV_PCM_TRIGGER_RESUME:
	case SNDRV_PCM_TRIGGER_PAUSE_RELEASE:
		ipq_lpaif_spdif_port_en(1);
		break;
	case SNDRV_PCM_TRIGGER_STOP:
	case SNDRV_PCM_TRIGGER_SUSPEND:
	case SNDRV_PCM_TRIGGER_PAUSE_PUSH:
		ipq_lpaif_spdif_port_en(0);
		break;
	default:
		ret = -EINVAL;
	}

	if (prtd->pcm_stream_info.pcm_prepare_start == 1) {
		if (prtd->pcm_stream_info.compr_mode ==
				LPA_IF_SPDIF_TX_DATA_TYPE_LINEAR)
			mdelay(700);

		prtd->pcm_stream_info.pcm_prepare_start++;
	}
	return ret;
}

static int ipq_pcm_spdif_hw_params(struct snd_pcm_substream *substream,
					struct snd_pcm_hw_params *params)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct ipq_lpass_runtime_data_t *prtd =
		(struct ipq_lpass_runtime_data_t *)runtime->private_data;

	pr_debug("%s\n", __func__);

	snd_pcm_set_runtime_buffer(substream, &substream->dma_buffer);

	switch (params->reserved[63]) {
	case LPA_IF_SPDIF_TX_DATA_TYPE_AC3:
	case LPA_IF_SPDIF_TX_DATA_TYPE_MPEG_1_L1:
	case LPA_IF_SPDIF_TX_DATA_TYPE_MPEG_1_L2:
	case LPA_IF_SPDIF_TX_DATA_TYPE_MPEG_2_w_ext:
	case LPA_IF_SPDIF_TX_DATA_TYPE_MPEG_2_AAC:
	case LPA_IF_SPDIF_TX_DATA_TYPE_MPEG_2_L1:
	case LPA_IF_SPDIF_TX_DATA_TYPE_MPEG_2_L2:
	case LPA_IF_SPDIF_TX_DATA_TYPE_MPEG_2_L3:
	case LPA_IF_SPDIF_TX_DATA_TYPE_DTS_T1:
	case LPA_IF_SPDIF_TX_DATA_TYPE_DTS_T2:
	case LPA_IF_SPDIF_TX_DATA_TYPE_DTS_T3:
	case LPA_IF_SPDIF_TX_DATA_TYPE_ATRAC:
	case LPA_IF_SPDIF_TX_DATA_TYPE_ATRAC2_3:
		prtd->pcm_stream_info.compr_mode = params->reserved[63];
		prtd->pcm_stream_info.spdif_frame_size =
			 (params->reserved[62] | (params->reserved[61] << 8));
		break;
	default:
		prtd->pcm_stream_info.compr_mode =
			LPA_IF_SPDIF_TX_DATA_TYPE_LINEAR;
		prtd->pcm_stream_info.spdif_frame_size = SPDIF_FRAMESIZE;
		break;

	}
	prtd->pcm_stream_info.pcm_prepare_start = 0;
	prtd->pcm_stream_info.period_index = 0;
	prtd->pcm_stream_info.blocks_done_per_period = 0;
	prtd->pcm_stream_info.blocks_per_period = 0;
	return 0;
}

static int ipq_pcm_spdif_open(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;

	int ret;
	struct ipq_lpass_runtime_data_t *prtd;

	pr_debug("%s\n", __func__);

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
		runtime->dma_bytes = ipq_pcm_hardware_playback.buffer_bytes_max;

	ret = snd_pcm_hw_constraint_integer(runtime,
				SNDRV_PCM_HW_PARAM_PERIODS);
	if (ret < 0) {
		pr_err("%s snd_pcm_hw_constraint_integer failed\n", __func__);
		return -EINVAL;
	}

	snd_soc_set_runtime_hwparams(substream, &ipq_pcm_hardware_playback);

	prtd = kzalloc(sizeof(struct ipq_lpass_runtime_data_t), GFP_KERNEL);
	if (prtd == NULL) {
		pr_err("Error allocating prtd\n");
		return -ENOMEM;
	}

	prtd->pcm_stream_info.pcm_prepare_start = 0;
	prtd->lpaif_clk.is_bit_clk_enabled = 0;
	prtd->pcm_stream_info.substream = substream;
	prtd->pcm_stream_info.compr_mode = LPA_IF_SPDIF_TX_DATA_TYPE_LINEAR;
	prtd->pcm_stream_info.spdif_frame_size = SPDIF_FRAMESIZE;
	runtime->private_data = prtd;

	return 0;

}

static struct snd_pcm_ops ipq_asoc_pcm_spdif_ops = {
	.open		= ipq_pcm_spdif_open,
	.hw_params	= ipq_pcm_spdif_hw_params,
	.trigger	= ipq_pcm_spdif_trigger,
	.ioctl		= snd_pcm_lib_ioctl,
	.close		= ipq_pcm_spdif_close,
	.prepare	= ipq_pcm_spdif_prepare,
	.mmap		= ipq_pcm_spdif_mmap,
	.pointer	= ipq_pcm_spdif_pointer,
};

static void ipq_asoc_pcm_spdif_free(struct snd_pcm *pcm)
{
	struct snd_pcm_substream *substream;
	struct snd_dma_buffer *buf;
	int stream = 0;

	substream = pcm->streams[stream].substream;

	buf = &substream->dma_buffer;
	if (buf->area)
		dma_free_coherent(pcm->card->dev, buf->bytes,
					buf->area, buf->addr);

	buf->area = NULL;
}

static int ipq_asoc_pcm_spdif_new(struct snd_soc_pcm_runtime *rtd)
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
		if (ret) {
			pr_err("%s Error allocating dma buffer\n", __func__);
			return -ENOMEM;
		}
	}
	return ret;
}

static struct snd_soc_platform_driver ipq_asoc_pcm_spdif_platform = {
	.ops		= &ipq_asoc_pcm_spdif_ops,
	.pcm_new	= ipq_asoc_pcm_spdif_new,
	.pcm_free	= ipq_asoc_pcm_spdif_free,
};

static __devinit int ipq_pcm_spdif_driver_probe(struct platform_device *pdev)
{
	int ret;
	ret = snd_soc_register_platform(&pdev->dev,
			&ipq_asoc_pcm_spdif_platform);
	if (ret)
		dev_err(&pdev->dev, "%s Failed to register pcm device\n",
								__func__);

	return ret;
}

static __devexit int ipq_pcm_spdif_driver_remove(struct platform_device *pdev)
{
	snd_soc_unregister_platform(&pdev->dev);
	return 0;
}

static struct platform_driver ipq_pcm_spdif_driver = {
	.driver	= {
		.name	= "ipq-pcm-spdif",
		.owner	= THIS_MODULE,
	},
	.probe	= ipq_pcm_spdif_driver_probe,
	.remove	= ipq_pcm_spdif_driver_remove,
};

static int __init ipq_lpass_pcm_spdif_init(void)
{
	int ret;
	pr_debug("%s\n", __func__);
	ret = platform_driver_register(&ipq_pcm_spdif_driver);
	if (ret)
		pr_err("%s Failed to register pcm platform driver\n",
								__func__);
	return ret;
}

static void __exit ipq_lpass_pcm_spdif_exit(void)
{
	pr_debug("%s\n", __func__);
	platform_driver_unregister(&ipq_pcm_spdif_driver);
}

module_init(ipq_lpass_pcm_spdif_init);
module_exit(ipq_lpass_pcm_spdif_exit);


MODULE_DESCRIPTION("IPQ PCM SPDIF Platform Driver");
MODULE_LICENSE("GPL v2");
