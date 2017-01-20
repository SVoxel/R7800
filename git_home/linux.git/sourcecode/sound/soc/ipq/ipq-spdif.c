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
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/soc.h>
#include <mach/msm_iomap-8x60.h>
#include <mach/audio_dma_msm8k.h>
#include <asm/io.h>
#include <sound/dai.h>
#include <linux/slab.h>
#include "ipq806x.h"
#include "ipq-pcm.h"
#include "ipq-spdif.h"

/*
 * SPDIFTX driver has two specific component -
 * 1. SPDIC PCM driver
 * 2. SPDIC CPU DAI driver
 * PCM driver has to access SPDIF TX register
 * space which is defined in CPU DAI driver.
 * This is enabled by exporting the structure.
 */
struct ipq_lpaif_spdif_baseinfo ipq_spdif_info;
EXPORT_SYMBOL_GPL(ipq_spdif_info);

struct spdif_intr {
	irqreturn_t (*callback) (int intrsrc, void *private_data);
	void *private_data;
};

struct spdif_intr *spdif_handler;

static const uint32_t burst_ctl[] = {
	0, /* not used */
	(LPA_IF_SPDIF_TX_DATA_TYPE_AC3 |
		LPA_IF_SPDIF_TX_REF_POINT(1) |
		LPA_IF_SPDIF_TX_REP_PERIOD_DATA_BURST(0x600) |
		LPA_IF_SPDIF_TX_REP_PERIOD_PAUSE(1)),
	0, /* not used */
	0, /* not used */
	(LPA_IF_SPDIF_TX_DATA_TYPE_MPEG_1_L1 |
		LPA_IF_SPDIF_TX_REF_POINT(0) |
		LPA_IF_SPDIF_TX_REP_PERIOD_DATA_BURST(0x180) |
		LPA_IF_SPDIF_TX_REP_PERIOD_PAUSE(3)),
	(LPA_IF_SPDIF_TX_DATA_TYPE_MPEG_1_L2 |
		LPA_IF_SPDIF_TX_REF_POINT(0) |
		LPA_IF_SPDIF_TX_REP_PERIOD_DATA_BURST(0x480) |
		LPA_IF_SPDIF_TX_REP_PERIOD_PAUSE(3)),
	(LPA_IF_SPDIF_TX_DATA_TYPE_MPEG_2_w_ext |
		LPA_IF_SPDIF_TX_REF_POINT(0) |
		LPA_IF_SPDIF_TX_REP_PERIOD_DATA_BURST(0x480) |
		LPA_IF_SPDIF_TX_REP_PERIOD_PAUSE(3)),
	(LPA_IF_SPDIF_TX_DATA_TYPE_MPEG_2_AAC |
		LPA_IF_SPDIF_TX_REF_POINT(0) |
		LPA_IF_SPDIF_TX_REP_PERIOD_DATA_BURST(0x400) |
		LPA_IF_SPDIF_TX_REP_PERIOD_PAUSE(3)),
	(LPA_IF_SPDIF_TX_DATA_TYPE_MPEG_2_L1 |
		LPA_IF_SPDIF_TX_REF_POINT(0) |
		LPA_IF_SPDIF_TX_REP_PERIOD_DATA_BURST(0x300) |
		LPA_IF_SPDIF_TX_REP_PERIOD_PAUSE(4)),
	(LPA_IF_SPDIF_TX_DATA_TYPE_MPEG_2_L2 |
		LPA_IF_SPDIF_TX_REF_POINT(0) |
		LPA_IF_SPDIF_TX_REP_PERIOD_DATA_BURST(0x900) |
		LPA_IF_SPDIF_TX_REP_PERIOD_PAUSE(4)),
	(LPA_IF_SPDIF_TX_DATA_TYPE_MPEG_2_L3 |
		LPA_IF_SPDIF_TX_REF_POINT(0) |
		LPA_IF_SPDIF_TX_REP_PERIOD_DATA_BURST(0x480) |
		LPA_IF_SPDIF_TX_REP_PERIOD_PAUSE(4)),
	(LPA_IF_SPDIF_TX_DATA_TYPE_DTS_T1 |
		LPA_IF_SPDIF_TX_REF_POINT(0) |
		LPA_IF_SPDIF_TX_REP_PERIOD_DATA_BURST(0x200) |
		LPA_IF_SPDIF_TX_REP_PERIOD_PAUSE(1)),
	(LPA_IF_SPDIF_TX_DATA_TYPE_DTS_T2 |
		LPA_IF_SPDIF_TX_REF_POINT(1) |
		LPA_IF_SPDIF_TX_REP_PERIOD_DATA_BURST(0x400) |
		LPA_IF_SPDIF_TX_REP_PERIOD_PAUSE(2)),
	(LPA_IF_SPDIF_TX_DATA_TYPE_DTS_T3 |
		LPA_IF_SPDIF_TX_REF_POINT(2) |
		LPA_IF_SPDIF_TX_REP_PERIOD_DATA_BURST(0x800) |
		LPA_IF_SPDIF_TX_REP_PERIOD_PAUSE(3)),
	(LPA_IF_SPDIF_TX_DATA_TYPE_ATRAC |
		LPA_IF_SPDIF_TX_REF_POINT(0) |
		LPA_IF_SPDIF_TX_REP_PERIOD_DATA_BURST(0x200) |
		LPA_IF_SPDIF_TX_REP_PERIOD_PAUSE(3)),
	(LPA_IF_SPDIF_TX_DATA_TYPE_ATRAC2_3 |
		LPA_IF_SPDIF_TX_REF_POINT(0) |
		LPA_IF_SPDIF_TX_REP_PERIOD_DATA_BURST(0x400) |
		LPA_IF_SPDIF_TX_REP_PERIOD_PAUSE(3)) };

static irqreturn_t ipq_lpaif_spdif_irq_handler(int irq, void *dev_id)
{
	uint32_t status;
	int ret = IRQ_NONE;
	status = readl(ipq_spdif_info.base + LPA_IF_SPDIF_TX_INT_STAT);

	if (((status & 0x1) == 0x1) || ((status & 0x2) == 0x2)) {
		/* Ack the interrupts */
		writel(0x7f, ipq_spdif_info.base + LPA_IF_SPDIF_TX_INT_STAT);
		if (spdif_handler->callback && spdif_handler->private_data)
			ret = spdif_handler->callback(status,
					spdif_handler->private_data);
	}

	return ret;
}

void ipq_spdif_register_handler(irqreturn_t (*callback) (int intrsrc,
		void *private_data), void *private_data)
{
	spdif_handler->callback = callback;
	spdif_handler->private_data = private_data;
}
EXPORT_SYMBOL_GPL(ipq_spdif_register_handler);

uint32_t ipq_spdif_cfg_bit_width(uint32_t bit_width)
{
	uint32_t cfg, fifo_ctl;

	cfg = readl(ipq_spdif_info.base + LPA_IF_SPDIF_TX_PORT_CFG);
	cfg &= ~(LPA_IF_SPDIF_TX_PORT_CFG_BIT_MASK);
	fifo_ctl = readl(ipq_spdif_info.base + LPA_IF_SPDIF_FIFO_CNTL);
	fifo_ctl &= ~(LPA_IF_SPDIF_FIFO_DWD_WD_SWAP);

	switch (bit_width) {
	case SNDRV_PCM_FORMAT_S16:
		cfg |= LPA_IF_SPDIF_TX_PORT_CFG_L16;
		fifo_ctl |= LPA_IF_SPDIF_FIFO_DWD_WD_SWAP;
		break;
	case SNDRV_PCM_FORMAT_S24:
	case SNDRV_PCM_FORMAT_S24_3LE:
		cfg |= LPA_IF_SPDIF_TX_PORT_CFG_L24;
		break;
	default:
		return -EINVAL;
	}
	writel(cfg, ipq_spdif_info.base + LPA_IF_SPDIF_TX_PORT_CFG);
	writel(fifo_ctl, (ipq_spdif_info.base + LPA_IF_SPDIF_FIFO_CNTL));

	return 0;
}
EXPORT_SYMBOL_GPL(ipq_spdif_cfg_bit_width);

void ipq_spdif_onetime_cfg(void)
{
	uint32_t cfg;

	/* Take SPIDFTX block out of reset */
	cfg = readl(lpass_clk_base.base + LCC_AHBEX_BRANCH_CTL);
	mdelay(500);
	cfg &= ~(HWIO_LCC_AHBEX_BRANCH_CTL_SLIMBUS_ARES_RESET);
	writel(cfg, lpass_clk_base.base + LCC_AHBEX_BRANCH_CTL);

	writel(LPA_IF_SPDIF_TX_CMD_ABRT,
		(ipq_spdif_info.base + LPA_IF_SPDIF_TX_CMD));
	/* port select */
	writel(0x0,  (ipq_spdif_info.base + LPA_IF_SPDIF_TXP_SEL));
	writel(0x0, (ipq_spdif_info.base + LPA_IF_SPDIF_TX_PORT_CFG));
	writel(SPDIF_FIFO_CTL, (ipq_spdif_info.base + LPA_IF_SPDIF_FIFO_CNTL));
	writel(SPDIF_TX_CH_ST, (ipq_spdif_info.base + LPA_IF_SPDIF_TX_CH_STAT));

	writel(0x0, (ipq_spdif_info.base + LPA_IF_SPDIF_TX_BURST_MISC));
	writel(SPDIF_TX_DBG_CRC_CNT,
		(ipq_spdif_info.base + LPA_IF_SPDIF_TX_DEBUG_CRC_COUNT));
	writel(0x0, (ipq_spdif_info.base + LPA_IF_SPDIF_TX_DEBUG_CRC_CNTL));
	writel(SPDIF_TX_INTR, (ipq_spdif_info.base + LPA_IF_SPDIF_TX_INT_STAT));
}
EXPORT_SYMBOL_GPL(ipq_spdif_onetime_cfg);

int ipq_spdif_cfg_compr_mode(uint32_t compr_mode, uint32_t spdif_frame_size)
{
	uint32_t fifo_ctl;
	uint32_t cfg;

	/* Frame Size in cfg*/
	writel((spdif_frame_size - 1),
		(ipq_spdif_info.base + LPA_IF_SPDIF_TX_CFG));

	cfg = readl(ipq_spdif_info.base + LPA_IF_SPDIF_TX_BURST_CNTL);
	cfg &= ~(LPA_IF_SPDIF_TX_BURST_CNTL_MASK);
	writel(cfg, (ipq_spdif_info.base + LPA_IF_SPDIF_TX_BURST_CNTL));

	switch (compr_mode) {
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
		cfg |= burst_ctl[compr_mode];
		writel(cfg,
			(ipq_spdif_info.base + LPA_IF_SPDIF_TX_BURST_CNTL));

		/* Disable DWORD SWAP for compressed mode */
		fifo_ctl = readl(ipq_spdif_info.base + LPA_IF_SPDIF_FIFO_CNTL);
		fifo_ctl &= ~(LPA_IF_SPDIF_FIFO_DWD_WD_SWAP);
		writel(fifo_ctl, (ipq_spdif_info.base +
				LPA_IF_SPDIF_FIFO_CNTL));

		/* Values to be set for compressed mode */
		writel(LPA_IF_SPDIF_TX_CHA_STAT_BUF0_VAL,
			(ipq_spdif_info.base + LPA_IF_SPDIF_TX_CHA_STAT_BUF0));
		writel(LPA_IF_SPDIF_TX_CHA_STAT_BUF1_VAL,
			(ipq_spdif_info.base + LPA_IF_SPDIF_TX_CHA_STAT_BUF1));
		writel(LPA_IF_SPDIF_TX_CHA_STAT_BUF2_VAL,
			(ipq_spdif_info.base + LPA_IF_SPDIF_TX_CHA_STAT_BUF2));
		writel(LPA_IF_SPDIF_TX_CHA_STAT_BUF3_VAL,
			(ipq_spdif_info.base + LPA_IF_SPDIF_TX_CHA_STAT_BUF3));
		writel(LPA_IF_SPDIF_TX_CHA_STAT_BUF4_VAL,
			(ipq_spdif_info.base + LPA_IF_SPDIF_TX_CHA_STAT_BUF4));
		writel(LPA_IF_SPDIF_TX_CHA_STAT_BUF5_VAL,
			(ipq_spdif_info.base + LPA_IF_SPDIF_TX_CHA_STAT_BUF5));
		writel(LPA_IF_SPDIF_TX_CHB_STAT_BUF0_VAL,
			(ipq_spdif_info.base + LPA_IF_SPDIF_TX_CHB_STAT_BUF0));
		writel(LPA_IF_SPDIF_TX_CHB_STAT_BUF1_VAL,
			(ipq_spdif_info.base + LPA_IF_SPDIF_TX_CHB_STAT_BUF1));
		writel(LPA_IF_SPDIF_TX_CHB_STAT_BUF2_VAL,
			(ipq_spdif_info.base + LPA_IF_SPDIF_TX_CHB_STAT_BUF2));
		writel(LPA_IF_SPDIF_TX_CHB_STAT_BUF3_VAL,
			(ipq_spdif_info.base + LPA_IF_SPDIF_TX_CHB_STAT_BUF3));
		writel(LPA_IF_SPDIF_TX_CHB_STAT_BUF4_VAL,
			(ipq_spdif_info.base + LPA_IF_SPDIF_TX_CHB_STAT_BUF4));
		writel(LPA_IF_SPDIF_TX_CHB_STAT_BUF5_VAL,
			(ipq_spdif_info.base + LPA_IF_SPDIF_TX_CHB_STAT_BUF5));
		break;
	case LPA_IF_SPDIF_TX_DATA_TYPE_LINEAR:
		/* Burst ctrl */
		writel(SPDIF_TX_BURST_CTL_LINEAR,
			(ipq_spdif_info.base + LPA_IF_SPDIF_TX_BURST_CNTL));
		writel(0x0,
			(ipq_spdif_info.base + LPA_IF_SPDIF_TX_CHA_STAT_BUF0));
		writel(0x0,
			(ipq_spdif_info.base + LPA_IF_SPDIF_TX_CHA_STAT_BUF1));
		writel(0x0,
			(ipq_spdif_info.base + LPA_IF_SPDIF_TX_CHA_STAT_BUF2));
		writel(0x0,
			(ipq_spdif_info.base + LPA_IF_SPDIF_TX_CHA_STAT_BUF3));
		writel(0x0,
			(ipq_spdif_info.base + LPA_IF_SPDIF_TX_CHA_STAT_BUF4));
		writel(0x0,
			(ipq_spdif_info.base + LPA_IF_SPDIF_TX_CHA_STAT_BUF5));
		writel(0x0,
			(ipq_spdif_info.base + LPA_IF_SPDIF_TX_CHB_STAT_BUF0));
		writel(0x0,
			(ipq_spdif_info.base + LPA_IF_SPDIF_TX_CHB_STAT_BUF1));
		writel(0x0,
			(ipq_spdif_info.base + LPA_IF_SPDIF_TX_CHB_STAT_BUF2));
		writel(0x0,
			(ipq_spdif_info.base + LPA_IF_SPDIF_TX_CHB_STAT_BUF3));
		writel(0x0,
			(ipq_spdif_info.base + LPA_IF_SPDIF_TX_CHB_STAT_BUF4));
		writel(0x0,
			(ipq_spdif_info.base + LPA_IF_SPDIF_TX_CHB_STAT_BUF5));
		break;

	}

	return 0;
}
EXPORT_SYMBOL_GPL(ipq_spdif_cfg_compr_mode);

void ipq_lpaif_spdif_port_en(uint8_t en)
{
	uint32_t cfg;

	cfg = readl(ipq_spdif_info.base + LPA_IF_SPDIF_TXP_SEL);
	if (en == 1)
		cfg |= LPA_IF_SPDIF_TXP_PEN;
	else
		cfg &= ~(LPA_IF_SPDIF_TXP_PEN);

	writel(cfg, ipq_spdif_info.base + LPA_IF_SPDIF_TXP_SEL);
}
EXPORT_SYMBOL_GPL(ipq_lpaif_spdif_port_en);

void ipq_spdif_intr_enable(void)
{
	uint32_t cfg;
	/* SPDIFTX_INT_CNTL register*/
	cfg = readl(ipq_spdif_info.base + LPA_IF_SPDIF_TX_INT_CNTL);
	cfg |= (LPA_IF_SPDIF_TX_BLKDONE_INT |
		LPA_IF_SPDIF_TX_FIFO_UNDERFLOW_INT |
		LPA_IF_SPDIF_TX_IDLE_INT |
		LPA_IF_SPDIF_TX_PAUSE_INT |
		LPA_IF_SPDIF_TX_DROPLB_INT |
		LPA_IF_SPDIF_TX_TS_DIFF_INT |
		LPA_IF_SPDIF_TX_MC_ERR |
		LPA_IF_SPDIF_TX_INT_MASK);
	writel(0x0, ipq_spdif_info.base + LPA_IF_SPDIF_TX_INT_CNTL);
	writel(SPDIF_INTR_MASK, ipq_spdif_info.base + LPA_IF_SPDIF_TX_INT_CNTL);
}
EXPORT_SYMBOL_GPL(ipq_spdif_intr_enable);

dma_addr_t ipq_spdif_set_params(dma_addr_t paddr, uint32_t spdif_frame_size)
{
	uint32_t cfg, i;

	/* SPDIFTX_CMD register */
	writel(0x0, ipq_spdif_info.base + LPA_IF_SPDIF_LB_SHAPER_CFG);
	writel(SPDIF_TXP_SEL, ipq_spdif_info.base + LPA_IF_SPDIF_TXP_SEL);

	for (i = 0; i < SPDIF_FIFO_DEPTH; i++) {
		writel(paddr, ipq_spdif_info.base + LPA_IF_SPDIF_BUF_PNTR);
		writel(0, ipq_spdif_info.base + LPA_IF_SPDIF_TX_SUBBUF_FIFO);
		paddr += (spdif_frame_size * SPDIF_DWORD_SZ);
	}
	mb();

	/* SPDIFTX_SUBBUF_FIFO register*/
	cfg = readl(ipq_spdif_info.base + LPA_IF_SPDIF_TX_SUBBUF_FIFO);
	cfg |= (~(LPA_IF_SPDIF_TX_SUBBUF_FIFO_WTTS) |
		 LPA_IF_SPDIF_TX_SUBBUF_FIFO_FR_SIZ_ADJUST(0));

	return paddr;

}
EXPORT_SYMBOL_GPL(ipq_spdif_set_params);

struct resource *spdif_intr_tx;

static int __devinit ipq_lpaif_spdif_probe(struct platform_device *pdev)
{
	int rc;
	int ret;
	struct resource *spdif_res;

	spdif_res = platform_get_resource_byname(pdev,
					IORESOURCE_MEM, "ipq-spdif");
	if (!spdif_res) {
		dev_err(&pdev->dev, "%s: %d: Error getting spdif resource\n",
						__func__, __LINE__);
		return -ENODEV;
	}
	ipq_spdif_info.base = ioremap(spdif_res->start,
				(spdif_res->end - spdif_res->start) + 1);

	if (!ipq_spdif_info.base) {
		dev_err(&pdev->dev, "%s: %d: Error ioremap for spdif\n",
						__func__, __LINE__);
		return -ENOMEM;
	}

	/*
	 * Register intr handler
	 */
	spdif_intr_tx = platform_get_resource_byname(
		pdev, IORESOURCE_IRQ, "ipq-spdiftx-irq");

	if (!spdif_intr_tx) {
		dev_err(&pdev->dev, "%s: %d: Failed to get irq resource\n",
						__func__, __LINE__);
		rc = -ENODEV;
		goto error;
	}

	ret = request_irq(spdif_intr_tx->start, ipq_lpaif_spdif_irq_handler,
			IRQF_TRIGGER_RISING, "ipq-spdiftx-intr", NULL);
	if (ret < 0) {
		dev_err(&pdev->dev, "%s: %dRequest Irq Failed.\n",
						__func__, __LINE__);
		rc = -ENXIO;
		goto error;
	}

	spdif_handler = kmalloc(sizeof(struct spdif_intr), GFP_KERNEL);

	if (!spdif_handler) {
		free_irq(spdif_intr_tx->start, NULL);
		rc = -ENOMEM;
		goto error;
	}

	return 0;
error:
	iounmap(ipq_spdif_info.base);
	return rc;
}

static int ipq_lpaif_spdif_remove(struct platform_device *pdev)
{
	iounmap(ipq_spdif_info.base);
	free_irq(spdif_intr_tx->start, NULL);
	return 0;
}

static struct platform_driver spdif_driver = {
	.probe = ipq_lpaif_spdif_probe,
	.remove = ipq_lpaif_spdif_remove,
	.driver = {
		.name = "ipq-spdif",
		.owner = THIS_MODULE
	},
};

static int __init spdif_init(void)
{
	int ret;
	ret = platform_driver_register(&spdif_driver);
	if (ret)
		pr_debug("Error in SPDIF driver register %s\n", __func__);
	return ret;
}

static void __exit spdif_exit(void)
{
	platform_driver_unregister(&spdif_driver);
}

module_init(spdif_init);
module_exit(spdif_exit);

MODULE_DESCRIPTION("IPQ SPDIF Driver");
MODULE_LICENSE("GPL v2");
