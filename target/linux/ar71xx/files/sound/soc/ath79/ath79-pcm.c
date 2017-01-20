/*
 * ath-pcm.c -- ALSA PCM interface for the QCA Wasp based audio interface
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

#include <linux/dma-mapping.h>
#include <linux/types.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <asm/mach-ath79/ar71xx_regs.h>
#include <asm/mach-ath79/ath79.h>

#include <linux/mm.h>

#include <sound/core.h>
#include <sound/soc.h>
#include <sound/pcm_params.h>

#include "ath79-pcm.h"

#define  DRV_NAME	"ath79-pcm-audio"


#define BUFFER_BYTES_MAX 16 * 4095 * 16
#define PERIOD_BYTES_MIN 64

static struct snd_pcm_hardware ath79_pcm_hardware = {
	.info = SNDRV_PCM_INFO_MMAP |
		SNDRV_PCM_INFO_MMAP_VALID |
		SNDRV_PCM_INFO_INTERLEAVED |
		SNDRV_PCM_INFO_NO_PERIOD_WAKEUP,
	.formats = SNDRV_PCM_FMTBIT_S8 |
			SNDRV_PCM_FMTBIT_S16_BE | SNDRV_PCM_FMTBIT_S16_LE |
			SNDRV_PCM_FMTBIT_S24_BE | SNDRV_PCM_FMTBIT_S24_LE |
			SNDRV_PCM_FMTBIT_S32_BE | SNDRV_PCM_FMTBIT_S32_LE,
	.rates = SNDRV_PCM_RATE_22050 |
			SNDRV_PCM_RATE_32000 |
			SNDRV_PCM_RATE_44100 |
			SNDRV_PCM_RATE_48000 |
			SNDRV_PCM_RATE_88200 |
			SNDRV_PCM_RATE_96000,
	.rate_min = 22050,
	.rate_max = 96000,
	.channels_min = 2,
	.channels_max = 2,
	/* These numbers are empirical. As the DMA engine is descriptor base
	 * the only real limitation we have is the amount of RAM.
	 * Ideally, we'd need to find the best tradeoff between number of descs
	 * and CPU load */

	.buffer_bytes_max = BUFFER_BYTES_MAX,
	.period_bytes_min = PERIOD_BYTES_MIN,
	.period_bytes_max = 4095,
	.periods_min = 16,
	.periods_max = 256,
	.fifo_size = 0,
};

static irqreturn_t ath79_pcm_interrupt(int irq, void *dev_id)
{
	uint32_t status;
	struct ath79_pcm_pltfm_priv *prdata = dev_id;
	struct ath79_pcm_rt_priv *rtpriv;
	unsigned int period_bytes;

	status = ath79_dma_rr(AR934X_DMA_REG_MBOX_INT_STATUS);

	if(status & AR934X_DMA_MBOX_INT_STATUS_RX_DMA_COMPLETE) {
		unsigned int played_size;
		rtpriv = prdata->playback->runtime->private_data;
		/* Store the last played buffer in the runtime priv struct */
		rtpriv->last_played = ath79_pcm_get_last_played(rtpriv);
		period_bytes = snd_pcm_lib_period_bytes(prdata->playback);


		played_size = ath79_pcm_set_own_bits(rtpriv);
		if(played_size > period_bytes)
			snd_printd("Played more than one period  bytes played: %d\n",played_size);
		rtpriv->elapsed_size += played_size;
		ath79_mbox_interrupt_ack(AR934X_DMA_MBOX_INT_STATUS_RX_DMA_COMPLETE);
		if(rtpriv->elapsed_size >= period_bytes)
		{
			rtpriv->elapsed_size %= period_bytes;
			snd_pcm_period_elapsed(prdata->playback);
		}

		if (rtpriv->last_played == NULL) {
			snd_printd("BUG: ISR called but no played buf found\n");
			goto ack;
		}

	}
	if(status & AR934X_DMA_MBOX_INT_STATUS_TX_DMA_COMPLETE) {
		rtpriv = prdata->capture->runtime->private_data;
		/* Store the last played buffer in the runtime priv struct */
		rtpriv->last_played = ath79_pcm_get_last_played(rtpriv);
		ath79_pcm_set_own_bits(rtpriv);
		ath79_mbox_interrupt_ack(AR934X_DMA_MBOX_INT_STATUS_TX_DMA_COMPLETE);

		if (rtpriv->last_played == NULL) {
			snd_printd("BUG: ISR called but no rec buf found\n");
			goto ack;
		}
		snd_pcm_period_elapsed(prdata->capture);
	}

ack:
	return IRQ_HANDLED;
}

static int ath79_pcm_open(struct snd_pcm_substream *ss)
{
	struct snd_soc_pcm_runtime *runtime = ss->private_data;
	struct snd_soc_platform *platform = runtime->platform;
	struct ath79_pcm_pltfm_priv *prdata = snd_soc_platform_get_drvdata(platform);
	struct ath79_pcm_rt_priv *rtpriv;
	int err;

	if (prdata == NULL) {
		prdata = kzalloc(sizeof(struct ath79_pcm_pltfm_priv), GFP_KERNEL);
		if (prdata == NULL)
			return -ENOMEM;

		err = request_irq(ATH79_MISC_IRQ_DMA, ath79_pcm_interrupt, 0,
				  "ath79-pcm", prdata);
		if (err) {
			kfree(prdata);
			return -EBUSY;
		}

		snd_soc_platform_set_drvdata(platform, prdata);
	}

	if (ss->stream == SNDRV_PCM_STREAM_PLAYBACK) {
		prdata->playback = ss;
	} else {
		prdata->capture = ss;
	}

	/* Allocate/Initialize the buffer linked list head */
	rtpriv = kmalloc(sizeof(*rtpriv), GFP_KERNEL);
	if (!rtpriv) {
		return -ENOMEM;
	}
	snd_printd("%s: 0x%xB allocated at 0x%08x\n",
	       __FUNCTION__, sizeof(*rtpriv), (u32) rtpriv);

	ss->runtime->private_data = rtpriv;
	rtpriv->last_played = NULL;
	INIT_LIST_HEAD(&rtpriv->dma_head);
	if(ss->stream == SNDRV_PCM_STREAM_PLAYBACK)
		rtpriv->direction = SNDRV_PCM_STREAM_PLAYBACK;
	else
		rtpriv->direction = SNDRV_PCM_STREAM_CAPTURE;

	snd_soc_set_runtime_hwparams(ss, &ath79_pcm_hardware);

	return 0;
}

static int ath79_pcm_close(struct snd_pcm_substream *ss)
{
	struct snd_soc_pcm_runtime *runtime = ss->private_data;
	struct snd_soc_platform *platform = runtime->platform;
	struct ath79_pcm_pltfm_priv *prdata = snd_soc_platform_get_drvdata(platform);
	struct ath79_pcm_rt_priv *rtpriv;

	if (!prdata)
		return 0;

	if (ss->stream == SNDRV_PCM_STREAM_PLAYBACK) {
		prdata->playback = NULL;
	} else {
		prdata->capture = NULL;
	}

	if (!prdata->playback && !prdata->capture) {
		free_irq(ATH79_MISC_IRQ_DMA, prdata);
		kfree(prdata);
		snd_soc_platform_set_drvdata(platform, NULL);
	}
	rtpriv = ss->runtime->private_data;
	kfree(rtpriv);

	return 0;
}

static int ath79_pcm_hw_params(struct snd_pcm_substream *ss,
			      struct snd_pcm_hw_params *hw_params)
{
	struct snd_pcm_runtime *runtime = ss->runtime;
	struct ath79_pcm_rt_priv *rtpriv;
	int ret;
	unsigned int period_size, sample_size, sample_rate, frames, channels;

	// Does this routine need to handle new clock changes in the hw_params?
	rtpriv = runtime->private_data;

	ret = ath79_mbox_dma_map(rtpriv, ss->dma_buffer.addr,
		params_period_bytes(hw_params), params_buffer_bytes(hw_params));

	if(ret < 0)
		return ret;

	period_size = params_period_bytes(hw_params);
	sample_size = snd_pcm_format_size(params_format(hw_params), 1);
	sample_rate = params_rate(hw_params);
	channels = params_channels(hw_params);
	frames = period_size / (sample_size * channels);

/* 	When we disbale the DMA engine, it could be just at the start of a descriptor.
	Hence calculate the longest time the DMA engine could be grabbing bytes for to
	Make sure we do not unmap the memory before the DMA is complete.
	Add 10 mSec of margin. This value will be used in ath79_mbox_dma_stop */

	rtpriv->delay_time = (frames * 1000)/sample_rate + 10;


	snd_pcm_set_runtime_buffer(ss, &ss->dma_buffer);
	runtime->dma_bytes = params_buffer_bytes(hw_params);

	return 1;
}

static int ath79_pcm_hw_free(struct snd_pcm_substream *ss)
{
	struct snd_pcm_runtime *runtime = ss->runtime;
	struct ath79_pcm_rt_priv *rtpriv;

	rtpriv = runtime->private_data;
	ath79_mbox_dma_unmap(rtpriv);
	snd_pcm_set_runtime_buffer(ss, NULL);
	return 0;
}

static int ath79_pcm_prepare(struct snd_pcm_substream *ss)
{
	struct snd_pcm_runtime *runtime = ss->runtime;
	struct snd_soc_pcm_runtime *rtd = ss->private_data;
	struct snd_soc_dai *cpu_dai;
	struct ath79_pcm_rt_priv *rtpriv;

	rtpriv = runtime->private_data;
	cpu_dai = rtd->cpu_dai;
	/* When setup the first stream should reset the DMA MBOX controller */
	if(cpu_dai->active == 1)
		ath79_mbox_dma_reset();

	ath79_mbox_dma_prepare(rtpriv);

	ath79_pcm_set_own_bits(rtpriv);
	rtpriv->last_played = NULL;

	return 0;
}

static int ath79_pcm_trigger(struct snd_pcm_substream *ss, int cmd)
{
	struct ath79_pcm_rt_priv *rtpriv = ss->runtime->private_data;

	switch (cmd) {
	case SNDRV_PCM_TRIGGER_START:
		ath79_mbox_dma_start(rtpriv);
		break;
	case SNDRV_PCM_TRIGGER_STOP:
		ath79_mbox_dma_stop(rtpriv);
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

static snd_pcm_uframes_t ath79_pcm_pointer(struct snd_pcm_substream *ss)
{
	struct snd_pcm_runtime *runtime = ss->runtime;
	struct ath79_pcm_rt_priv *rtpriv;
	snd_pcm_uframes_t ret = 0;

	rtpriv = runtime->private_data;

	if(rtpriv->last_played == NULL)
		ret = 0;
	else
		ret = rtpriv->last_played->BufPtr - runtime->dma_addr;

	ret = bytes_to_frames(runtime, ret);
	return ret;
}

static int ath79_pcm_mmap(struct snd_pcm_substream *ss, struct vm_area_struct *vma)
{
	return remap_pfn_range(vma, vma->vm_start,
			ss->dma_buffer.addr >> PAGE_SHIFT,
			vma->vm_end - vma->vm_start, vma->vm_page_prot);
}

static struct snd_pcm_ops ath79_pcm_ops = {
	.open		= ath79_pcm_open,
	.close		= ath79_pcm_close,
	.ioctl		= snd_pcm_lib_ioctl,
	.hw_params	= ath79_pcm_hw_params,
	.hw_free	= ath79_pcm_hw_free,
	.prepare	= ath79_pcm_prepare,
	.trigger	= ath79_pcm_trigger,
	.pointer	= ath79_pcm_pointer,
	.mmap		= ath79_pcm_mmap,
};

static void ath79_pcm_free_dma_buffers(struct snd_pcm *pcm)
{
	struct snd_pcm_substream *ss;
	struct snd_dma_buffer *buf;
	int stream;

	for (stream = 0; stream < 2; stream++) {
		ss = pcm->streams[stream].substream;
		if (!ss)
			continue;
		buf = &ss->dma_buffer;
		if (!buf->area)
			continue;
		dma_free_coherent(NULL, buf->bytes,
				      buf->area, buf->addr);
		buf->area = NULL;
	}

	ath79_mbox_dma_exit();
}

static int ath79_pcm_preallocate_dma_buffer(struct snd_pcm *pcm, int stream)
{
	struct snd_pcm_substream *ss = pcm->streams[stream].substream;
	struct snd_dma_buffer *buf = &ss->dma_buffer;

	printk(KERN_NOTICE "%s: allocate %8s stream\n", __FUNCTION__,
		stream == SNDRV_PCM_STREAM_CAPTURE ? "capture" : "playback" );

	buf->dev.type = SNDRV_DMA_TYPE_DEV;
	buf->dev.dev = pcm->card->dev;
	buf->private_data = NULL;
	buf->bytes = ath79_pcm_hardware.buffer_bytes_max;

	buf->area = dma_alloc_coherent(NULL, buf->bytes,
					   &buf->addr, GFP_DMA);
	if (!buf->area)
		return -ENOMEM;

	printk(KERN_NOTICE "%s: 0x%xB allocated at 0x%08x\n",
		__FUNCTION__, buf->bytes, (u32) buf->area);

	return 0;
}

static u64 ath79_pcm_dmamask = 0xffffffff;

static int ath79_soc_pcm_new(struct snd_soc_pcm_runtime *rtd)
{
	struct snd_card *card = rtd->card->snd_card;
	struct snd_pcm *pcm = rtd->pcm;
	int ret = 0;

	if (!card->dev->dma_mask)
		card->dev->dma_mask = &ath79_pcm_dmamask;
	if (!card->dev->coherent_dma_mask)
		card->dev->coherent_dma_mask = DMA_BIT_MASK(32);

	if (pcm->streams[SNDRV_PCM_STREAM_PLAYBACK].substream) {
		ret = ath79_pcm_preallocate_dma_buffer(pcm,
			SNDRV_PCM_STREAM_PLAYBACK);
		if (ret)
			goto out;
	}

	if (pcm->streams[SNDRV_PCM_STREAM_CAPTURE].substream) {
		ret = ath79_pcm_preallocate_dma_buffer(pcm,
			SNDRV_PCM_STREAM_CAPTURE);
		if (ret)
			goto out;
	}

	ath79_mbox_dma_init(rtd->platform->dev);

out:
	return ret;
}

struct snd_soc_platform_driver ath79_soc_platform = {
	.ops		= &ath79_pcm_ops,
	.pcm_new	= ath79_soc_pcm_new,
	.pcm_free	= ath79_pcm_free_dma_buffers,
};
EXPORT_SYMBOL_GPL(ath79_soc_platform);

static int __devinit ath79_soc_platform_probe(struct platform_device *pdev)
{
	return snd_soc_register_platform(&pdev->dev, &ath79_soc_platform);
}

static int __devexit ath79_soc_platform_remove(struct platform_device *pdev)
{
	snd_soc_unregister_platform(&pdev->dev);
	return 0;
}

static struct platform_driver ath79_pcm_driver = {
	.driver = {
			.name = DRV_NAME,
			.owner = THIS_MODULE,
	},

	.probe = ath79_soc_platform_probe,
	.remove = __devexit_p(ath79_soc_platform_remove),
};

module_platform_driver(ath79_pcm_driver);

MODULE_AUTHOR("Qualcomm-Atheros Inc.");
MODULE_AUTHOR("Mathieu Olivari <mathieu@qca.qualcomm.com>");
MODULE_DESCRIPTION("QCA Audio PCM DMA module");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_ALIAS("platform:" DRV_NAME);
