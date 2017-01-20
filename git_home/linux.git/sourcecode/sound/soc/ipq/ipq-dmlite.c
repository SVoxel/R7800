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
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>
#include <linux/debugfs.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/spinlock.h>
#include <linux/slab.h>
#include <linux/msm_audio.h>
#include <linux/clk.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/soc.h>
#include <mach/msm_iomap-8x60.h>
#include <sound/dai.h>
#include "ipq-pcm.h"
#include "ipq-dmlite.h"
#include "ipq-lpaif.h"

struct ipq_lpass_runtime_data_t *gprtd;
EXPORT_SYMBOL_GPL(gprtd);

struct lpass_dml_baseinfo dml_info;
EXPORT_SYMBOL_GPL(dml_info);

static irqreturn_t ipq_dml_intr_handler(int irq, void *dev_id)
{
	uint32_t status;
	uint32_t ret = IRQ_NONE;

	status = readl(dml_info.base + DML_STATUS);

	if (status & DML_STATUS_ERR) {
		pr_debug("%s: %d: dml dma error\n", __func__, __LINE__);
		ret = IRQ_HANDLED;
	}

	if (status & DML_STATUS_DONE) {
		/* Stop DML DMA transfer */
		writel(0x0, dml_info.base + DML_CTL);
		/* Clear the DML interrupt status */
		writel(0x1, dml_info.base + DML_STATUS);

		if (gprtd->lpaif_info.lpa_if_dma_start != 0) {
			gprtd->dml_info.dml_src_addr +=
				gprtd->dml_info.lpm_period_size;
			gprtd->dml_info.dml_dst_addr +=
				gprtd->dml_info.lpm_period_size;
		} else {
			/*
			 * This indicates the start of the ALSA
			 * ring buffer. Enable LPA_IF data transfer
			 */
			ipq_cfg_i2s_spkr(1, 0, LPA_IF_MI2S);
			gprtd->dml_info.dml_src_addr +=
				(gprtd->dml_info.lpm_period_size * 2);
			gprtd->dml_info.dml_dst_addr +=
				(gprtd->dml_info.lpm_period_size * 2);
			gprtd->lpaif_info.lpa_if_dma_start = 1;
		}

		if (gprtd->dml_info.dml_src_addr >=
		(gprtd->dml_info.dml_start_addr +
		(gprtd->dml_info.lpm_period_size *
			gprtd->dml_info.lpm_periods)))
			gprtd->dml_info.dml_src_addr =
			gprtd->dml_info.dml_start_addr;

		if ((gprtd->dml_info.dml_dst_addr) >=
		(IPQ_LPM_START + (gprtd->dml_info.lpm_period_size * 2)))
			gprtd->dml_info.dml_dst_addr = IPQ_LPM_START;

		ret = IRQ_HANDLED;
	}

	return ret;
}

void ipq_dml_trigger(unsigned long data)
{
	struct snd_pcm_substream *substream = (struct snd_pcm_substream *)data;
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct ipq_lpass_runtime_data_t *prtd =
		(struct ipq_lpass_runtime_data_t *)runtime->private_data;

	if (!prtd->dml_info.dml_dma_started) {
		prtd->dml_info.dml_start_addr = runtime->dma_addr;
		prtd->dml_info.dml_src_addr = runtime->dma_addr;
		prtd->dml_info.dml_dst_addr = IPQ_LPM_START;
		prtd->dml_info.dml_transfer_size =
			(prtd->dml_info.lpm_period_size * 2);

		prtd->dml_info.dml_dma_started = 1;
	}

	writel(prtd->dml_info.dml_src_addr, (dml_info.base + DML_SRC));
	writel(prtd->dml_info.dml_dst_addr, (dml_info.base + DML_DST));
	writel(((prtd->dml_info.dml_transfer_size - 1)),
		(dml_info.base + DML_TRAN));

	/* Clearing the DML Status */
	writel(0x1, dml_info.base + DML_STATUS);
	/* Initiating DML DMA transfer */
	writel(0x1, (dml_info.base + DML_CTL));
	prtd->dml_info.dml_transfer_size = prtd->dml_info.lpm_period_size;
}
EXPORT_SYMBOL_GPL(ipq_dml_trigger);

static struct resource *dml_res;
static int __devinit ipq_dmlite_probe(struct platform_device *pdev)
{
	int rc = 0;
	dml_res = platform_get_resource_byname(
		pdev, IORESOURCE_MEM, "ipq-dmlite");
	if (!dml_res) {
		pr_err("%s: %d: failed to get mem resource\n",
					__func__, __LINE__);
		return -ENODEV;
	}

	dml_info.base = ioremap(dml_res->start,
		(dml_res->end - dml_res->start) + 1);

	if (!dml_info.base) {
		dev_err(&pdev->dev, "ioremap failed\n");
		return -ENOMEM;
	}

	dml_res = platform_get_resource_byname(
		pdev, IORESOURCE_IRQ, "ipq-dmlite-irq");

	if (!dml_res) {
		pr_err("%s: %d: irq resource\n", __func__, __LINE__);
		rc = -ENODEV;
		goto error;
	}

	rc = request_irq(dml_res->start, ipq_dml_intr_handler,
			IRQF_TRIGGER_RISING,
			"lpass-dml", (void *)gprtd);

	if (rc < 0) {
		rc = -ENODEV;
		goto error;
	}

	return 0;

error:
	iounmap(dml_info.base);
	return rc;
}

static int ipq_dmlite_remove(struct platform_device *pdev)
{
	iounmap(dml_info.base);
	return 0;
}

static struct platform_driver ipq_dmlite_driver = {
	.probe  = ipq_dmlite_probe,
	.remove = ipq_dmlite_remove,
	.driver = {
		.name = "ipq-dmlite",
		.owner = THIS_MODULE,
	},
};

static int __init ipq_dmlite_init(void)
{
	int ret;
	ret = platform_driver_register(&ipq_dmlite_driver);
	if (ret)
		pr_err("%s: %d: Registration failed\n", __func__, __LINE__);

	return ret;
}

static void __exit ipq_dmlite_exit(void)
{
	free_irq(dml_res->start, NULL);
	platform_driver_unregister(&ipq_dmlite_driver);
}

module_init(ipq_dmlite_init);
module_exit(ipq_dmlite_exit);

MODULE_DESCRIPTION("IPQ DM-Lite Driver");
MODULE_LICENSE("GPL v2");
