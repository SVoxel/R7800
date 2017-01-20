 /* * Copyright (c) 2013 The Linux Foundation. All rights reserved.* */
/* Copyright (c) 2012, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>
#include <linux/pm_runtime.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/delay.h>
#include <linux/of.h>
#include <linux/list.h>
#include <linux/debugfs.h>
#include <linux/uaccess.h>
#include <linux/usb/ch9.h>
#include <linux/usb/gadget.h>
#include <linux/usb/msm_hsusb.h>
#include <linux/regulator/consumer.h>

#include <mach/rpm-regulator.h>
#include <mach/msm_bus.h>
#include <mach/msm_usb30.h>
#include "dwc3_otg.h"
#include "core.h"
#include "gadget.h"


/* USB3.0 default values provided by VI */

#define RX_TERM_VALUE		0   /* RTUNE_DEBUG register value */
#define RX_EQ_VALUE		4   /* Override value for rx_eq */
#define AMPLITUDE_VALUE		110 /* Override value for transmit amplitude */
#define TX_DEEMPH_3_5DB		23 /* Override value for transmit preemphasis */

/**
 *  USB DBM Hardware registers.
 *
 */
#define DBM_BASE		0x000F8000
#define DBM_EP_CFG(n)		(DBM_BASE + (0x00 + 4 * (n)))
#define DBM_DATA_FIFO(n)	(DBM_BASE + (0x10 + 4 * (n)))
#define DBM_DATA_FIFO_SIZE(n)	(DBM_BASE + (0x20 + 4 * (n)))
#define DBM_DATA_FIFO_EN	(DBM_BASE + (0x30))
#define DBM_GEVNTADR		(DBM_BASE + (0x34))
#define DBM_GEVNTSIZ		(DBM_BASE + (0x38))
#define DBM_DBG_CNFG		(DBM_BASE + (0x3C))
#define DBM_HW_TRB0_EP(n)	(DBM_BASE + (0x40 + 4 * (n)))
#define DBM_HW_TRB1_EP(n)	(DBM_BASE + (0x50 + 4 * (n)))
#define DBM_HW_TRB2_EP(n)	(DBM_BASE + (0x60 + 4 * (n)))
#define DBM_HW_TRB3_EP(n)	(DBM_BASE + (0x70 + 4 * (n)))
#define DBM_PIPE_CFG		(DBM_BASE + (0x80))
#define DBM_SOFT_RESET		(DBM_BASE + (0x84))
#define DBM_GEN_CFG		(DBM_BASE + (0x88))

/**
 *  USB DBM  Hardware registers bitmask.
 *
 */
/* DBM_EP_CFG */
#define DBM_EN_EP		0x00000001
#define USB3_EPNUM		0x0000003E
#define DBM_BAM_PIPE_NUM	0x000000C0
#define DBM_PRODUCER		0x00000100
#define DBM_DISABLE_WB		0x00000200
#define DBM_INT_RAM_ACC		0x00000400

/* DBM_DATA_FIFO_SIZE */
#define DBM_DATA_FIFO_SIZE_MASK	0x0000ffff

/* DBM_GEVNTSIZ */
#define DBM_GEVNTSIZ_MASK	0x0000ffff

/* DBM_DBG_CNFG */
#define DBM_ENABLE_IOC_MASK	0x0000000f

/* DBM_SOFT_RESET */
#define DBM_SFT_RST_EP0		0x00000001
#define DBM_SFT_RST_EP1		0x00000002
#define DBM_SFT_RST_EP2		0x00000004
#define DBM_SFT_RST_EP3		0x00000008
#define DBM_SFT_RST_EPS_MASK	0x0000000F
#define DBM_SFT_RST_MASK	0x80000000
#define DBM_EN_MASK		0x00000002

#define DBM_MAX_EPS		4

/* DBM TRB configurations */
#define DBM_TRB_BIT		0x80000000
#define DBM_TRB_DATA_SRC	0x40000000
#define DBM_TRB_DMA		0x20000000
#define DBM_TRB_EP_NUM(ep)	(ep<<24)

struct dwc3_ipq_req_complete {
	struct list_head list_item;
	struct usb_request *req;
	void (*orig_complete)(struct usb_ep *ep,
			      struct usb_request *req);
};

struct dwc3_ipq {
	struct platform_device *dwc3;
	struct device *dev;
	void __iomem *base;
	u32 resource_size;
	int dbm_num_eps;
	u8 ep_num_mapping[DBM_MAX_EPS];
	const struct usb_ep_ops *original_ep_ops[DWC3_ENDPOINTS_NUM];
	struct list_head req_complete_list;
	struct clk		*core_clk;
	struct clk		*iface0_clk;
	struct clk              *iface1_clk;
	struct clk		*sleep_clk;
	struct clk		*utmi_clk;
	struct clk 		*utmi_b0_clk;
	struct clk              *utmi_b1_clk;
	struct dwc3_ext_xceiv	ext_xceiv;
	bool			resume_pending;
	atomic_t                pm_suspended;
	atomic_t		in_lpm;
	struct delayed_work	resume_work;
	struct wake_lock	wlock;
	struct dwc3_charger	charger;
	struct usb_phy		*otg_xceiv;
	struct delayed_work	chg_work;
	enum usb_chg_state	chg_state;
	u8			dcd_retries;
	u8			mode;
	u32			bus_perf_client;
	struct msm_bus_scale_pdata	*bus_scale_table;
};

static struct dwc3_ipq *context;
static u64 dwc3_ipq_dma_mask = DMA_BIT_MASK(64);

static u16 dwc3_ipq_ssusb_read_phy_reg(unsigned int  addr, void __iomem *ipq_base)
{
	u16 tmp_phy[3], i;
	do {
		for (i = 0; i < 3; i++) {
			writel(addr, ipq_base +
				IPQ_SSUSB_REG_QSCRATCH_SS_CR_PROTOCOL_DATA_IN);
			writel(0x1, ipq_base +
				IPQ_SSUSB_REG_QSCRATCH_SS_CR_PROTOCOL_CAP_ADDR);
			while (0 != readl(ipq_base +
				IPQ_SSUSB_REG_QSCRATCH_SS_CR_PROTOCOL_CAP_ADDR));
			writel(0x1, ipq_base +
				IPQ_SSUSB_REG_QSCRATCH_SS_CR_PROTOCOL_READ);
			while (0 != readl(ipq_base +
				IPQ_SSUSB_REG_QSCRATCH_SS_CR_PROTOCOL_READ));
			tmp_phy[i] = (u16)readl(ipq_base +
				IPQ_SSUSB_REG_QSCRATCH_SS_CR_PROTOCOL_DATA_OUT);
		}
	} while (tmp_phy[1] != tmp_phy[2]);
	return tmp_phy[2];
}

static void dwc3_ipq_ssusb_write_phy_reg(u32 addr, u16 data, void __iomem *ipq_base)
{
	writel(addr, ipq_base + IPQ_SSUSB_REG_QSCRATCH_SS_CR_PROTOCOL_DATA_IN);
	writel(0x1, ipq_base + IPQ_SSUSB_REG_QSCRATCH_SS_CR_PROTOCOL_CAP_ADDR);
	while (0 != readl(ipq_base +
		IPQ_SSUSB_REG_QSCRATCH_SS_CR_PROTOCOL_CAP_ADDR));
	writel(data, ipq_base + IPQ_SSUSB_REG_QSCRATCH_SS_CR_PROTOCOL_DATA_IN);
	writel(0x1, ipq_base + IPQ_SSUSB_REG_QSCRATCH_SS_CR_PROTOCOL_CAP_DATA);
	while (0 != readl(ipq_base +
		IPQ_SSUSB_REG_QSCRATCH_SS_CR_PROTOCOL_CAP_DATA));
	writel(0x1, ipq_base + IPQ_SSUSB_REG_QSCRATCH_SS_CR_PROTOCOL_WRITE);
	while (0 != readl(ipq_base +
		IPQ_SSUSB_REG_QSCRATCH_SS_CR_PROTOCOL_WRITE));
}

static void ipq_ssusb_clear_and_set_bits32(u32 offset, u32 clear_bits, u32 set_bits, void __iomem *ipq_base)
{
	u32 data;
	data = readl(ipq_base + offset);
	data = (data & ~clear_bits) | set_bits;
	writel(data, ipq_base + offset);
}

static void ipq_ssusb_clear_bits32(u32 offset, u32 bits, void __iomem *ipq_base)
{
	u32 data;
	data = readl(ipq_base+offset);
	data = data & ~bits;
	writel(data, ipq_base + offset);
}

static void partial_rx_reset_init(void __iomem *ipq_base)
{
        u32 addr = DWC3_SSUSB_PHY_TX_ALT_BLOCK_REG;
        u16 data = dwc3_ipq_ssusb_read_phy_reg(addr, ipq_base);
        data |= DWC3_SSUSB_PHY_TX_ALT_BLOCK_EN_ALT_BUS;
        dwc3_ipq_ssusb_write_phy_reg(addr, data, ipq_base);
        return;
}


static void  uw_ssusb_pre_init(void __iomem *ipq_base,
	struct dwc3_platform_data *dwc3)
{
	u32  set_bits, tmp;
	/* GCTL Reset ON */
	writel(0x800, ipq_base + DWC3_SSUSB_REG_GCTL);
	/* Config SS PHY CTRL */
	set_bits = 0;
	writel(0x80, ipq_base + IPQ_SS_PHY_CTRL_REG);
	udelay(5);
	ipq_ssusb_clear_bits32(IPQ_SS_PHY_CTRL_REG, 0x80, ipq_base);
	udelay(5);
	/* REF_USE_PAD */
	set_bits = 0x0000000;  /* USE Internal clock */
	set_bits |= IPQ_SSUSB_QSCRATCH_SS_PHY_CTRL_LANE0_PWR_PRESENT;
	set_bits |= IPQ_SSUSB_QSCRATCH_SS_PHY_CTRL_REF_SS_PHY_EN;
	writel(set_bits, ipq_base + IPQ_SS_PHY_CTRL_REG);
	/* Config HS PHY CTRL */
	set_bits = IPQ_SSUSB_REG_QSCRATCH_HS_PHY_CTRL_UTMI_OTG_VBUS_VALID;
	/*
	 * COMMONONN forces xo, bias and pll to stay on during suspend;
	 * Allowing suspend (writing 1) kills Aragorn V1
	 */
	set_bits |= IPQ_SSUSB_REG_QSCRATCH_HS_PHY_CTRL_COMMONONN;
	set_bits |= IPQ_SSUSB_REG_QSCRATCH_HS_PHY_CTRL_USE_CLKCORE;
	set_bits |= IPQ_SSUSB_REG_QSCRATCH_HS_PHY_CTRL_FSEL_VAL;
	/*
	 * If the configuration of clocks is not bypassed in Host mode,
	 * HS PHY suspend needs to be prohibited, otherwise - SS connection fails
	 */
	ipq_ssusb_clear_and_set_bits32(IPQ_SSUSB_REG_QSCRATCH_HS_PHY_CTRL, 0,
					set_bits, ipq_base);
	/* USB2 PHY Reset ON */
	writel(DWC3_SSUSB_REG_GUSB2PHYCFG_PHYSOFTRST, ipq_base +
		DWC3_SSUSB_REG_GUSB2PHYCFG(0));
	/* USB3 PHY Reset ON */
	writel(DWC3_SSUSB_REG_GUSB3PIPECTL_PHYSOFTRST, ipq_base +
		DWC3_SSUSB_REG_GUSB3PIPECTL(0));
	udelay(5);
	/* USB3 PHY Reset OFF */
	ipq_ssusb_clear_bits32(DWC3_SSUSB_REG_GUSB3PIPECTL(0),
				DWC3_SSUSB_REG_GUSB3PIPECTL_PHYSOFTRST, ipq_base);
	ipq_ssusb_clear_bits32(DWC3_SSUSB_REG_GUSB2PHYCFG(0),
				DWC3_GUSB2PHYCFG_PHYSOFTRST, ipq_base);
	udelay(5);
	/* GCTL Reset OFF */
	ipq_ssusb_clear_bits32(DWC3_SSUSB_REG_GCTL, DWC3_GCTL_CORESOFTRESET,
				ipq_base);
	udelay(5);
	if (RX_TERM_VALUE) {
		dwc3_ipq_ssusb_write_phy_reg(DWC3_SSUSB_PHY_RTUNE_RTUNE_CTRL_REG,
						0, ipq_base);
		dwc3_ipq_ssusb_write_phy_reg(DWC3_SSUSB_PHY_RTUNE_DEBUG_REG,
						0x0448, ipq_base);
		dwc3_ipq_ssusb_write_phy_reg(DWC3_SSUSB_PHY_RTUNE_DEBUG_REG,
						RX_TERM_VALUE, ipq_base);
	}
	if (0 != RX_EQ_VALUE) { /* Values from 1 to 7 */
		tmp =0;
		/*
		 * 1. Fixed EQ setting. This can be achieved as follows:
		 * LANE0.RX_OVRD_IN_HI. RX_EQ_EN set to 0 - address 1006 bit 6
		 * LANE0.RX_OVRD_IN_HI.RX_EQ_EN_OVRD set to 1 0- address 1006 bit 7
		 * LANE0.RX_OVRD_IN_HI.RX_EQ set to 4 (also try setting 3 if possible) -
		 * address 1006 bits 10:8 - please make this a variable, if unchanged the section is not executed
		 * LANE0.RX_OVRD_IN_HI.RX_EQ_OVRD set to 1 - address 1006 bit 11
		 */
		tmp = dwc3_ipq_ssusb_read_phy_reg(DWC3_SSUSB_PHY_RX_OVRD_IN_HI_REG,
							ipq_base);
		tmp &= ~((u16)1 << DWC3_SSUSB_PHY_RX_OVRD_IN_HI_RX_EQ_EN);
		tmp |= ((u16)1 << DWC3_SSUSB_PHY_RX_OVRD_IN_HI_RX_EQ_EN_OVRD);
		tmp &= ~((u16) DWC3_SSUSB_PHY_RX_OVRD_IN_HI_RX_EQ_MASK <<
			DWC3_SSUSB_PHY_RX_OVRD_IN_HI_RX_EQ);
		tmp |= ((dwc3->rx_eq) <<
			(DWC3_SSUSB_PHY_RX_OVRD_IN_HI_RX_EQ));

		tmp |= 1 << DWC3_SSUSB_PHY_RX_OVRD_IN_HI_RX_EQ_OVRD;
		dwc3_ipq_ssusb_write_phy_reg(DWC3_SSUSB_PHY_RX_OVRD_IN_HI_REG,
						tmp, ipq_base);
	}
	if ((113 != AMPLITUDE_VALUE) || (21 != TX_DEEMPH_3_5DB)) {
		tmp = dwc3_ipq_ssusb_read_phy_reg(DWC3_SSUSB_PHY_TX_OVRD_DRV_LO_REG,
			ipq_base);
		tmp &= ~DWC3_SSUSB_PHY_TX_DEEMPH_MASK;
		tmp |= ((dwc3->tx_deamp_3_5db) <<
			(DWC3_SSUSB_PHY_TX_DEEMPH_SHIFT));
		tmp &= ~DWC3_SSUSB_PHY_AMP_MASK;
		tmp |= AMPLITUDE_VALUE;
		tmp |= DWC3_SSUSB_PHY_AMP_EN;
		dwc3_ipq_ssusb_write_phy_reg(DWC3_SSUSB_PHY_TX_OVRD_DRV_LO_REG,
						tmp, ipq_base);
	}
	if (dwc3->mpll)
		dwc3_ipq_ssusb_write_phy_reg(DWC3_SSUSB_PHY_MPLL, dwc3->mpll, ipq_base);
	ipq_ssusb_clear_and_set_bits32(IPQ_SS_PHY_PARAM_CTRL_1_REG,
					0x7, 0x5, ipq_base);
	/* XHCI REV */
	writel((1 << 2), ipq_base + IPQ_QSCRATCH_GENERAL_CFG);
	writel(0x0c80c010, ipq_base + DWC3_SSUSB_REG_GUCTL);
	partial_rx_reset_init(ipq_base);
	set_bits = 0;
	/* Test  U2EXIT_LFPS */
	set_bits |= IPQ_SSUSB_REG_GCTL_U2EXIT_LFPS;
	ipq_ssusb_clear_and_set_bits32(DWC3_SSUSB_REG_GCTL, 0,
					set_bits, ipq_base);
	set_bits = 0;
	set_bits |= IPQ_SSUSB_REG_GCTL_U2RSTECN;
	set_bits |= IPQ_SSUSB_REG_GCTL_U2EXIT_LFPS;
	ipq_ssusb_clear_and_set_bits32(DWC3_SSUSB_REG_GCTL, 0,
					set_bits, ipq_base);
	writel(DWC3_GCTL_U2EXIT_LFPS | DWC3_GCTL_SOFITPSYNC |
		DWC3_GCTL_PRTCAPDIR(1) |
		DWC3_GCTL_U2RSTECN | DWC3_GCTL_PWRDNSCALE(2),
		ipq_base + DWC3_GCTL);
	writel((IPQ_SSUSB_QSCRATCH_SS_PHY_CTRL_MPLL_MULTI(0x19) |
		IPQ_SSUSB_QSCRATCH_SS_PHY_CTRL_REF_SS_PHY_EN |
		IPQ_SSUSB_QSCRATCH_SS_PHY_CTRL_LANE0_PWR_PRESENT),
		ipq_base + IPQ_SS_PHY_CTRL_REG);
	writel((DWC3_SSUSB_REG_GUSB2PHYCFG_SUSPENDUSB20 |
		DWC3_SSUSB_REG_GUSB2PHYCFG_ENBLSLPM |
		DWC3_SSUSB_REG_GUSB2PHYCFG_USBTRDTIM(9)),
		ipq_base + DWC3_SSUSB_REG_GUSB2PHYCFG(0));
	writel(DWC3_SSUSB_REG_GUSB3PIPECTL_ELASTIC_BUFFER_MODE |
		DWC3_SSUSB_REG_GUSB3PIPECTL_TX_DE_EPPHASIS(1) |
		DWC3_SSUSB_REG_GUSB3PIPECTL_TX_MARGIN(0)|
		DWC3_SSUSB_REG_GUSB3PIPECTL_DELAYP1TRANS |
		DWC3_SSUSB_REG_GUSB3PIPECTL_DELAYP1P2P3(1) |
		DWC3_SSUSB_REG_GUSB3PIPECTL_U1U2EXITFAIL_TO_RECOV |
		DWC3_SSUSB_REG_GUSB3PIPECTL_REQUEST_P1P2P3,
		ipq_base + DWC3_SSUSB_REG_GUSB3PIPECTL(0));
	writel(
	IPQ_SSUSB_REG_QSCRATCH_SS_PHY_PARAM_CTRL_1_LOS_LEVEL(0x9) |
	IPQ_SSUSB_REG_QSCRATCH_SS_PHY_PARAM_CTRL_1_TX_DEEMPH_3_5DB(
						dwc3->tx_deamp_3_5db) |
	IPQ_SSUSB_REG_QSCRATCH_SS_PHY_PARAM_CTRL_1_TX_DEEMPH_6DB(
						dwc3->tx_deamp_6_db) |
	IPQ_SSUSB_REG_QSCRATCH_SS_PHY_PARAM_CTRL_1_TX_SWING_FULL(0x6E),
	ipq_base + IPQ_SS_PHY_PARAM_CTRL_1_REG);
	writel(IPQ_SSUSB_REG_QSCRATCH_GENERAL_CFG_XHCI_REV(DWC3_SSUSB_XHCI_REV_10),
		ipq_base + IPQ_QSCRATCH_GENERAL_CFG);
}

static void usb30_common_pre_init(int id, void __iomem *ipq_base)
{
	void __iomem *reg;
	if (id == 0)
		reg = USB30_RESET;
	else
		reg = USB30_1_RESET;

	writel(IPQ_USB30_RESET_PHY_ASYNC_RESET, reg);
	writel(IPQ_USB30_RESET_POWERON_ASYNC_RESET |
		IPQ_USB30_RESET_PHY_ASYNC_RESET, reg);
	writel(IPQ_USB30_RESET_MOC_UTMI_ASYNC_RESET |
		IPQ_USB30_RESET_POWERON_ASYNC_RESET |
		IPQ_USB30_RESET_PHY_ASYNC_RESET, reg);
	writel(IPQ_USB30_RESET_SLEEP_ASYNC_RESET |
		IPQ_USB30_RESET_MOC_UTMI_ASYNC_RESET |
		IPQ_USB30_RESET_POWERON_ASYNC_RESET |
		IPQ_USB30_RESET_PHY_ASYNC_RESET, reg);
	writel(IPQ_USB30_RESET_MASTER_ASYNC_RESET |
		IPQ_USB30_RESET_SLEEP_ASYNC_RESET |
		IPQ_USB30_RESET_MOC_UTMI_ASYNC_RESET |
		IPQ_USB30_RESET_POWERON_ASYNC_RESET |
		IPQ_USB30_RESET_PHY_ASYNC_RESET, reg);
	if (id == 0) {
		writel(IPQ_USB30_RESET_PORT2_HS_PHY_ASYNC_RESET |
			IPQ_USB30_RESET_MASTER_ASYNC_RESET |
			IPQ_USB30_RESET_SLEEP_ASYNC_RESET |
			IPQ_USB30_RESET_MOC_UTMI_ASYNC_RESET |
			IPQ_USB30_RESET_POWERON_ASYNC_RESET |
			IPQ_USB30_RESET_PHY_ASYNC_RESET, reg);
	}
	udelay(5);
	writel(IPQ_USB30_RESET_MASK & ~(IPQ_USB30_RESET_PHY_ASYNC_RESET), reg);
	writel(IPQ_USB30_RESET_MASK & ~(IPQ_USB30_RESET_POWERON_ASYNC_RESET |
		IPQ_USB30_RESET_PHY_ASYNC_RESET), reg);
	writel(IPQ_USB30_RESET_MASK & ~(IPQ_USB30_RESET_MOC_UTMI_ASYNC_RESET |
		IPQ_USB30_RESET_POWERON_ASYNC_RESET |
		IPQ_USB30_RESET_PHY_ASYNC_RESET), reg);
	writel(IPQ_USB30_RESET_MASK & ~(IPQ_USB30_RESET_SLEEP_ASYNC_RESET |
		IPQ_USB30_RESET_MOC_UTMI_ASYNC_RESET |
		IPQ_USB30_RESET_POWERON_ASYNC_RESET |
		IPQ_USB30_RESET_PHY_ASYNC_RESET), reg);
	writel(IPQ_USB30_RESET_MASK & ~(IPQ_USB30_RESET_MASTER_ASYNC_RESET |
		IPQ_USB30_RESET_SLEEP_ASYNC_RESET|
		IPQ_USB30_RESET_MOC_UTMI_ASYNC_RESET |
		IPQ_USB30_RESET_POWERON_ASYNC_RESET |
		IPQ_USB30_RESET_PHY_ASYNC_RESET), reg);
	if (id == 0) {
		writel(IPQ_USB30_RESET_MASK &
			~(IPQ_USB30_RESET_PORT2_HS_PHY_ASYNC_RESET |
			IPQ_USB30_RESET_MASTER_ASYNC_RESET |
			IPQ_USB30_RESET_SLEEP_ASYNC_RESET |
			IPQ_USB30_RESET_MOC_UTMI_ASYNC_RESET |
			IPQ_USB30_RESET_POWERON_ASYNC_RESET |
			IPQ_USB30_RESET_PHY_ASYNC_RESET), reg);
/*
 * TCSR configuration cannot be done from HLOS drivers once XPUs are enabled.
 * Configuration of IPQ_TCSR_USB_CONTROLLER_TYPE_SEL moved to TZ init.
 */
	}
	writel((IPQ_SSUSB_REG_QSCRATCH_CGCTL_RAM1112_EN |
		IPQ_SSUSB_REG_QSCRATCH_CGCTL_RAM13_EN),
		ipq_base + IPQ_SSUSB_REG_QSCRATCH_CGCTL);
	writel((IPQ_SSUSB_REG_QSCRATCH_RAM1_RAM13_EN |
		IPQ_SSUSB_REG_QSCRATCH_RAM1RAM12_EN |
		IPQ_SSUSB_REG_QSCRATCH_RAM1_RAM11_EN),
		ipq_base + IPQ_SSUSB_REG_QSCRATCH_RAM1);
}


/**
 *
 * Read register with debug info.
 *
 * @base - DWC3 base virtual address.
 * @offset - register offset.
 *
 * @return u32
 */
static inline u32 dwc3_ipq_read_reg(void *base, u32 offset)
{
	u32 val = ioread32(base + offset);
	return val;
}

/**
 * Read register masked field with debug info.
 *
 * @base - DWC3 base virtual address.
 * @offset - register offset.
 * @mask - register bitmask.
 *
 * @return u32
 */
static inline u32 dwc3_ipq_read_reg_field(void *base,
					  u32 offset,
					  const u32 mask)
{
	u32 shift = find_first_bit((void *)&mask, 32);
	u32 val = ioread32(base + offset);
	val &= mask;		/* clear other bits */
	val >>= shift;
	return val;
}

/**
 *
 * Write register with debug info.
 *
 * @base - DWC3 base virtual address.
 * @offset - register offset.
 * @val - value to write.
 *
 */
static inline void dwc3_ipq_write_reg(void *base, u32 offset, u32 val)
{
	iowrite32(val, base + offset);
}

/**
 * Write register masked field with debug info.
 *
 * @base - DWC3 base virtual address.
 * @offset - register offset.
 * @mask - register bitmask.
 * @val - value to write.
 *
 */
static inline void dwc3_ipq_write_reg_field(void *base, u32 offset,
					    const u32 mask, u32 val)
{
	u32 shift = find_first_bit((void *)&mask, 32);
	u32 tmp = ioread32(base + offset);

	tmp &= ~mask;		/* clear written bits */
	val = tmp | (val << shift);
	iowrite32(val, base + offset);
}

/**
 * Write register and read back masked value to confirm it is written
 *
 * @base - DWC3 base virtual address.
 * @offset - register offset.
 * @mask - register bitmask specifying what should be updated
 * @val - value to write.
 *
 */
static inline void dwc3_ipq_write_readback(void *base, u32 offset,
					    const u32 mask, u32 val)
{
	u32 write_val, tmp = ioread32(base + offset);

	tmp &= ~mask;		/* retain other bits */
	write_val = tmp | val;

	iowrite32(write_val, base + offset);

	/* Read back to see if val was written */
	tmp = ioread32(base + offset);
	tmp &= mask;		/* clear other bits */

	if (tmp != val)
		dev_err(context->dev, "%s: write: %x to QSCRATCH: %x FAILED\n",
						__func__, val, offset);
}

/**
 * Return DBM EP number according to usb endpoint number.
 *
 */
static int dwc3_ipq_find_matching_dbm_ep(u8 usb_ep)
{
	int i;

	for (i = 0; i < context->dbm_num_eps; i++)
		if (context->ep_num_mapping[i] == usb_ep)
			return i;

	return -ENODEV; /* Not found */
}

/**
 * Return number of configured DBM endpoints.
 *
 */
static int dwc3_ipq_configured_dbm_ep_num(void)
{
	int i;
	int count = 0;

	for (i = 0; i < context->dbm_num_eps; i++)
		if (context->ep_num_mapping[i])
			count++;

	return count;
}

/**
 * Configure the DBM with the USB3 core event buffer.
 * This function is called by the SNPS UDC upon initialization.
 *
 * @addr - address of the event buffer.
 * @size - size of the event buffer.
 *
 */
static int dwc3_ipq_event_buffer_config(u32 addr, u16 size)
{
	dev_dbg(context->dev, "%s\n", __func__);

	dwc3_ipq_write_reg(context->base, DBM_GEVNTADR, addr);
	dwc3_ipq_write_reg_field(context->base, DBM_GEVNTSIZ,
		DBM_GEVNTSIZ_MASK, size);

	return 0;
}

/**
 * Reset the DBM registers upon initialization.
 *
 */
static int dwc3_ipq_dbm_soft_reset(int enter_reset)
{
	dev_dbg(context->dev, "%s\n", __func__);
	if (enter_reset) {
		dev_dbg(context->dev, "enter DBM reset\n");
		dwc3_ipq_write_reg_field(context->base, DBM_SOFT_RESET,
			DBM_SFT_RST_MASK, 1);
	} else {
		dev_dbg(context->dev, "exit DBM reset\n");
		dwc3_ipq_write_reg_field(context->base, DBM_SOFT_RESET,
			DBM_SFT_RST_MASK, 0);
		/*enable DBM*/
		dwc3_ipq_write_reg_field(context->base, IPQ_QSCRATCH_GENERAL_CFG,
			DBM_EN_MASK, 0x1);
	}

	return 0;
}

/**
 * Soft reset specific DBM ep.
 * This function is called by the function driver upon events
 * such as transfer aborting, USB re-enumeration and USB
 * disconnection.
 *
 * @dbm_ep - DBM ep number.
 * @enter_reset - should we enter a reset state or get out of it.
 *
 */
static int dwc3_ipq_dbm_ep_soft_reset(u8 dbm_ep, bool enter_reset)
{
	dev_dbg(context->dev, "%s\n", __func__);

	if (dbm_ep >= context->dbm_num_eps) {
		dev_err(context->dev,
				"%s: Invalid DBM ep index\n", __func__);
		return -ENODEV;
	}

	if (enter_reset) {
		dwc3_ipq_write_reg_field(context->base, DBM_SOFT_RESET,
			DBM_SFT_RST_EPS_MASK & 1 << dbm_ep, 1);
	} else {
		dwc3_ipq_write_reg_field(context->base, DBM_SOFT_RESET,
			DBM_SFT_RST_EPS_MASK & 1 << dbm_ep, 0);
	}

	return 0;
}

/**
 * Configure a USB DBM ep to work in BAM mode.
 *
 *
 * @usb_ep - USB physical EP number.
 * @producer - producer/consumer.
 * @disable_wb - disable write back to system memory.
 * @internal_mem - use internal USB memory for data fifo.
 * @ioc - enable interrupt on completion.
 *
 * @return int - DBM ep number.
 */
static int dwc3_ipq_dbm_ep_config(u8 usb_ep, u8 bam_pipe,
				  bool producer, bool disable_wb,
				  bool internal_mem, bool ioc)
{
	s8 dbm_ep;
	u32 ep_cfg;

	dev_dbg(context->dev, "%s\n", __func__);

	dbm_ep = dwc3_ipq_find_matching_dbm_ep(usb_ep);

	if (dbm_ep < 0) {
		dev_err(context->dev,
				"%s: Invalid usb ep index\n", __func__);
		return -ENODEV;
	}
	/* First, reset the dbm endpoint */
	dwc3_ipq_dbm_ep_soft_reset(dbm_ep, 0);

	/* Set ioc bit for dbm_ep if needed */
	dwc3_ipq_write_reg_field(context->base, DBM_DBG_CNFG,
		DBM_ENABLE_IOC_MASK & 1 << dbm_ep, ioc ? 1 : 0);

	ep_cfg = (producer ? DBM_PRODUCER : 0) |
		(disable_wb ? DBM_DISABLE_WB : 0) |
		(internal_mem ? DBM_INT_RAM_ACC : 0);

	dwc3_ipq_write_reg_field(context->base, DBM_EP_CFG(dbm_ep),
		DBM_PRODUCER | DBM_DISABLE_WB | DBM_INT_RAM_ACC, ep_cfg >> 8);

	dwc3_ipq_write_reg_field(context->base, DBM_EP_CFG(dbm_ep), USB3_EPNUM,
		usb_ep);
	dwc3_ipq_write_reg_field(context->base, DBM_EP_CFG(dbm_ep),
		DBM_BAM_PIPE_NUM, bam_pipe);
	dwc3_ipq_write_reg_field(context->base, DBM_PIPE_CFG, 0x000000ff,
		0xe4);
	dwc3_ipq_write_reg_field(context->base, DBM_EP_CFG(dbm_ep), DBM_EN_EP,
		1);

	return dbm_ep;
}

/**
 * Configure a USB DBM ep to work in normal mode.
 *
 * @usb_ep - USB ep number.
 *
 */
static int dwc3_ipq_dbm_ep_unconfig(u8 usb_ep)
{
	s8 dbm_ep;

	dev_dbg(context->dev, "%s\n", __func__);

	dbm_ep = dwc3_ipq_find_matching_dbm_ep(usb_ep);

	if (dbm_ep < 0) {
		dev_err(context->dev,
				"%s: Invalid usb ep index\n", __func__);
		return -ENODEV;
	}

	context->ep_num_mapping[dbm_ep] = 0;

	dwc3_ipq_write_reg(context->base, DBM_EP_CFG(dbm_ep), 0);

	/* Reset the dbm endpoint */
	dwc3_ipq_dbm_ep_soft_reset(dbm_ep, true);

	return 0;
}

/**
 * Configure the DBM with the BAM's data fifo.
 * This function is called by the USB BAM Driver
 * upon initialization.
 *
 * @ep - pointer to usb endpoint.
 * @addr - address of data fifo.
 * @size - size of data fifo.
 *
 */
int ipq_data_fifo_config(struct usb_ep *ep, u32 addr, u32 size, u8 dst_pipe_idx)
{
	u8 dbm_ep;
	struct dwc3_ep *dep = to_dwc3_ep(ep);
	u8 bam_pipe = dst_pipe_idx;

	dev_dbg(context->dev, "%s\n", __func__);

	dbm_ep = bam_pipe;
	context->ep_num_mapping[dbm_ep] = dep->number;

	dwc3_ipq_write_reg(context->base, DBM_DATA_FIFO(dbm_ep), addr);
	dwc3_ipq_write_reg_field(context->base, DBM_DATA_FIFO_SIZE(dbm_ep),
		DBM_DATA_FIFO_SIZE_MASK, size);

	return 0;
}

/**
 * Cleanups for ipq endpoint on request complete.
 *
 * Also call original request complete.
 *
 * @usb_ep - pointer to usb_ep instance.
 * @request - pointer to usb_request instance.
 *
 * @return int - 0 on success, negetive on error.
 */
static void dwc3_ipq_req_complete_func(struct usb_ep *ep,
				       struct usb_request *request)
{
	struct dwc3_request *req = to_dwc3_request(request);
	struct dwc3_ep *dep = to_dwc3_ep(ep);
	struct dwc3_ipq_req_complete *req_complete = NULL;

	/* Find original request complete function and remove it from list */
	list_for_each_entry(req_complete,
				&context->req_complete_list,
				list_item) {
		if (req_complete->req == request)
			break;
	}
	if (!req_complete || req_complete->req != request) {
		dev_err(dep->dwc->dev, "%s: could not find the request\n",
					__func__);
		return;
	}
	list_del(&req_complete->list_item);

	/*
	 * Release another one TRB to the pool since DBM queue took 2 TRBs
	 * (normal and link), and the dwc3/gadget.c :: dwc3_gadget_giveback
	 * released only one.
	 */
	if (req->queued)
		dep->busy_slot++;

	/* Unconfigure dbm ep */
	dwc3_ipq_dbm_ep_unconfig(dep->number);

	/*
	 * If this is the last endpoint we unconfigured, than reset also
	 * the event buffers.
	 */
	if (0 == dwc3_ipq_configured_dbm_ep_num())
		dwc3_ipq_event_buffer_config(0, 0);

	/*
	 * Call original complete function, notice that dwc->lock is already
	 * taken by the caller of this function (dwc3_gadget_giveback()).
	 */
	request->complete = req_complete->orig_complete;
	if (request->complete)
		request->complete(ep, request);

	kfree(req_complete);
}

/**
 * Helper function.
 * See the header of the dwc3_ipq_ep_queue function.
 *
 * @dwc3_ep - pointer to dwc3_ep instance.
 * @req - pointer to dwc3_request instance.
 *
 * @return int - 0 on success, negetive on error.
 */
static int __dwc3_ipq_ep_queue(struct dwc3_ep *dep, struct dwc3_request *req)
{
	struct dwc3_trb *trb;
	struct dwc3_trb *trb_link;
	struct dwc3_gadget_ep_cmd_params params;
	u32 cmd;
	int ret = 0;

	/* We push the request to the dep->req_queued list to indicate that
	 * this request is issued with start transfer. The request will be out
	 * from this list in 2 cases. The first is that the transfer will be
	 * completed (not if the transfer is endless using a circular TRBs with
	 * with link TRB). The second case is an option to do stop stransfer,
	 * this can be initiated by the function driver when calling dequeue.
	 */
	req->queued = true;
	list_add_tail(&req->list, &dep->req_queued);

	/* First, prepare a normal TRB, point to the fake buffer */
	trb = &dep->trb_pool[dep->free_slot & DWC3_TRB_MASK];
	dep->free_slot++;
	memset(trb, 0, sizeof(*trb));

	req->trb = trb;
	trb->bph = DBM_TRB_BIT | DBM_TRB_DMA | DBM_TRB_EP_NUM(dep->number);
	trb->size = DWC3_TRB_SIZE_LENGTH(req->request.length);
	trb->ctrl = DWC3_TRBCTL_NORMAL | DWC3_TRB_CTRL_HWO | DWC3_TRB_CTRL_CHN;
	req->trb_dma = dwc3_trb_dma_offset(dep, trb);

	/* Second, prepare a Link TRB that points to the first TRB*/
	trb_link = &dep->trb_pool[dep->free_slot & DWC3_TRB_MASK];
	dep->free_slot++;
	memset(trb_link, 0, sizeof *trb_link);

	trb_link->bpl = lower_32_bits(req->trb_dma);
	trb_link->bph = DBM_TRB_BIT |
			DBM_TRB_DMA | DBM_TRB_EP_NUM(dep->number);
	trb_link->size = 0;
	trb_link->ctrl = DWC3_TRBCTL_LINK_TRB | DWC3_TRB_CTRL_HWO;

	/*
	 * Now start the transfer
	 */
	memset(&params, 0, sizeof(params));
	params.param0 = 0; /* TDAddr High */
	params.param1 = lower_32_bits(req->trb_dma); /* DAddr Low */

	cmd = DWC3_DEPCMD_STARTTRANSFER;
	ret = dwc3_send_gadget_ep_cmd(dep->dwc, dep->number, cmd, &params);
	if (ret < 0) {
		dev_dbg(dep->dwc->dev,
			"%s: failed to send STARTTRANSFER command\n",
			__func__);

		list_del(&req->list);
		return ret;
	}

	return ret;
}

/**
 * Queue a usb request to the DBM endpoint.
 * This function should be called after the endpoint
 * was enabled by the ep_enable.
 *
 * This function prepares special structure of TRBs which
 * is familier with the DBM HW, so it will possible to use
 * this endpoint in DBM mode.
 *
 * The TRBs prepared by this function, is one normal TRB
 * which point to a fake buffer, followed by a link TRB
 * that points to the first TRB.
 *
 * The API of this function follow the regular API of
 * usb_ep_queue (see usb_ep_ops in include/linuk/usb/gadget.h).
 *
 * @usb_ep - pointer to usb_ep instance.
 * @request - pointer to usb_request instance.
 * @gfp_flags - possible flags.
 *
 * @return int - 0 on success, negetive on error.
 */
static int dwc3_ipq_ep_queue(struct usb_ep *ep,
			     struct usb_request *request, gfp_t gfp_flags)
{
	struct dwc3_request *req = to_dwc3_request(request);
	struct dwc3_ep *dep = to_dwc3_ep(ep);
	struct dwc3 *dwc = dep->dwc;
	struct dwc3_ipq_req_complete *req_complete;
	unsigned long flags;
	int ret = 0;
	u8 bam_pipe;
	bool producer;
	bool disable_wb;
	bool internal_mem;
	bool ioc;
	u8 speed;

	if (!(request->udc_priv & MSM_SPS_MODE)) {
		/* Not SPS mode, call original queue */
		dev_vdbg(dwc->dev, "%s: not sps mode, use regular queue\n",
					__func__);

		return (context->original_ep_ops[dep->number])->queue(ep,
								request,
								gfp_flags);
	}

	if (!dep->endpoint.desc) {
		dev_err(dwc->dev,
			"%s: trying to queue request %p to disabled ep %s\n",
			__func__, request, ep->name);
		return -EPERM;
	}

	if (dep->number == 0 || dep->number == 1) {
		dev_err(dwc->dev,
			"%s: trying to queue dbm request %p to control ep %s\n",
			__func__, request, ep->name);
		return -EPERM;
	}

	if (dep->free_slot > 0 || dep->busy_slot > 0 ||
		!list_empty(&dep->request_list) ||
		!list_empty(&dep->req_queued)) {

		dev_err(dwc->dev,
			"%s: trying to queue dbm request %p tp ep %s\n",
			__func__, request, ep->name);
		return -EPERM;
	}

	/*
	 * Override req->complete function, but before doing that,
	 * store it's original pointer in the req_complete_list.
	 */
	req_complete = kzalloc(sizeof(*req_complete), GFP_KERNEL);
	if (!req_complete) {
		dev_err(dep->dwc->dev, "%s: not enough memory\n", __func__);
		return -ENOMEM;
	}
	req_complete->req = request;
	req_complete->orig_complete = request->complete;
	list_add_tail(&req_complete->list_item, &context->req_complete_list);
	request->complete = dwc3_ipq_req_complete_func;

	/*
	 * Configure the DBM endpoint
	 */
	bam_pipe = request->udc_priv & MSM_PIPE_ID_MASK;
	producer = ((request->udc_priv & MSM_PRODUCER) ? true : false);
	disable_wb = ((request->udc_priv & MSM_DISABLE_WB) ? true : false);
	internal_mem = ((request->udc_priv & MSM_INTERNAL_MEM) ? true : false);
	ioc = ((request->udc_priv & MSM_ETD_IOC) ? true : false);

	ret = dwc3_ipq_dbm_ep_config(dep->number,
					bam_pipe, producer,
					disable_wb, internal_mem, ioc);
	if (ret < 0) {
		dev_err(context->dev,
			"error %d after calling dwc3_ipq_dbm_ep_config\n",
			ret);
		return ret;
	}

	dev_vdbg(dwc->dev, "%s: queing request %p to ep %s length %d\n",
			__func__, request, ep->name, request->length);

	/*
	 * We must obtain the lock of the dwc3 core driver,
	 * including disabling interrupts, so we will be sure
	 * that we are the only ones that configure the HW device
	 * core and ensure that we queuing the request will finish
	 * as soon as possible so we will release back the lock.
	 */
	spin_lock_irqsave(&dwc->lock, flags);
	ret = __dwc3_ipq_ep_queue(dep, req);
	spin_unlock_irqrestore(&dwc->lock, flags);
	if (ret < 0) {
		dev_err(context->dev,
			"error %d after calling __dwc3_ipq_ep_queue\n", ret);
		return ret;
	}

	speed = dwc3_readl(dwc->regs, DWC3_DSTS) & DWC3_DSTS_CONNECTSPD;
	dwc3_ipq_write_reg(context->base, DBM_GEN_CFG, speed >> 2);

	return 0;
}

/**
 * Configure MSM endpoint.
 * This function do specific configurations
 * to an endpoint which need specific implementaion
 * in the MSM architecture.
 *
 * This function should be called by usb function/class
 * layer which need a support from the specific MSM HW
 * which wrap the USB3 core. (like DBM specific endpoints)
 *
 * @ep - a pointer to some usb_ep instance
 *
 * @return int - 0 on success, negetive on error.
 */
int msm_ep_config(struct usb_ep *ep)
{
	struct dwc3_ep *dep = to_dwc3_ep(ep);
	struct usb_ep_ops *new_ep_ops;

	/* Save original ep ops for future restore*/
	if (context->original_ep_ops[dep->number]) {
		dev_err(context->dev,
			"ep [%s,%d] already configured as msm endpoint\n",
			ep->name, dep->number);
		return -EPERM;
	}
	context->original_ep_ops[dep->number] = ep->ops;

	/* Set new usb ops as we like */
	new_ep_ops = kzalloc(sizeof(struct usb_ep_ops), GFP_KERNEL);
	if (!new_ep_ops) {
		dev_err(context->dev,
			"%s: unable to allocate mem for new usb ep ops\n",
			__func__);
		return -ENOMEM;
	}
	(*new_ep_ops) = (*ep->ops);
	new_ep_ops->queue = dwc3_ipq_ep_queue;
	ep->ops = new_ep_ops;

	/*
	 * Do HERE more usb endpoint configurations
	 * which are specific to MSM.
	 */

	return 0;
}
EXPORT_SYMBOL(msm_ep_config);

/**
 * Un-configure MSM endpoint.
 * Tear down configurations done in the
 * dwc3_msm_ep_config function.
 *
 * @ep - a pointer to some usb_ep instance
 *
 * @return int - 0 on success, negetive on error.
 */
int msm_ep_unconfig(struct usb_ep *ep)
{
	struct dwc3_ep *dep = to_dwc3_ep(ep);
	struct usb_ep_ops *old_ep_ops;

	/* Restore original ep ops */
	if (!context->original_ep_ops[dep->number]) {
		dev_err(context->dev,
			"ep [%s,%d] was not configured as msm endpoint\n",
			ep->name, dep->number);
		return -EINVAL;
	}
	old_ep_ops = (struct usb_ep_ops	*)ep->ops;
	ep->ops = context->original_ep_ops[dep->number];
	context->original_ep_ops[dep->number] = NULL;
	kfree(old_ep_ops);

	/*
	 * Do HERE more usb endpoint un-configurations
	 * which are specific to MSM.
	 */

	return 0;
}
EXPORT_SYMBOL(msm_ep_unconfig);

static void dwc3_chg_enable_secondary_det(struct dwc3_ipq *mdwc)
{
	u32 chg_ctrl;

	/* Turn off VDP_SRC */
	dwc3_ipq_write_reg(mdwc->base, IPQ_CHARGING_DET_CTRL_REG, 0x0);
	msleep(20);

	/* Before proceeding make sure VDP_SRC is OFF */
	chg_ctrl = dwc3_ipq_read_reg(mdwc->base, IPQ_CHARGING_DET_CTRL_REG);
	if (chg_ctrl & 0x3F)
		dev_err(mdwc->dev, "%s Unable to reset chg_det block: %x\n",
						 __func__, chg_ctrl);
	/*
	 * Configure DM as current source, DP as current sink
	 * and enable battery charging comparators.
	 */
	dwc3_ipq_write_readback(mdwc->base, IPQ_CHARGING_DET_CTRL_REG, 0x3F, 0x34);
}

static bool dwc3_chg_det_check_output(struct dwc3_ipq *mdwc)
{
	u32 chg_det;
	bool ret = false;

	chg_det = dwc3_ipq_read_reg(mdwc->base, IPQ_CHARGING_DET_OUTPUT_REG);
	ret = chg_det & 1;

	return ret;
}

static void dwc3_chg_enable_primary_det(struct dwc3_ipq *mdwc)
{
	/*
	 * Configure DP as current source, DM as current sink
	 * and enable battery charging comparators.
	 */
	dwc3_ipq_write_readback(mdwc->base, IPQ_CHARGING_DET_CTRL_REG, 0x3F, 0x30);
}

static inline bool dwc3_chg_check_dcd(struct dwc3_ipq *mdwc)
{
	u32 chg_state;
	bool ret = false;

	chg_state = dwc3_ipq_read_reg(mdwc->base, IPQ_CHARGING_DET_OUTPUT_REG);
	ret = chg_state & 2;

	return ret;
}

static inline void dwc3_chg_disable_dcd(struct dwc3_ipq *mdwc)
{
	dwc3_ipq_write_readback(mdwc->base, IPQ_CHARGING_DET_CTRL_REG, 0x3F, 0x0);
}

static inline void dwc3_chg_enable_dcd(struct dwc3_ipq *mdwc)
{
	/* Data contact detection enable, DCDENB */
	dwc3_ipq_write_readback(mdwc->base, IPQ_CHARGING_DET_CTRL_REG, 0x3F, 0x2);
}

static void dwc3_chg_block_reset(struct dwc3_ipq *mdwc)
{
	u32 chg_ctrl;

	/* Clear charger detecting control bits */
	dwc3_ipq_write_reg(mdwc->base, IPQ_CHARGING_DET_CTRL_REG, 0x0);

	/* Clear alt interrupt latch and enable bits */
	dwc3_ipq_write_reg(mdwc->base, IPQ_HS_PHY_IRQ_STAT_REG, 0xFFF);
	dwc3_ipq_write_reg(mdwc->base, IPQ_ALT_INTERRUPT_EN_REG, 0x0);

	udelay(100);

	/* Before proceeding make sure charger block is RESET */
	chg_ctrl = dwc3_ipq_read_reg(mdwc->base, IPQ_CHARGING_DET_CTRL_REG);
	if (chg_ctrl & 0x3F)
		dev_err(mdwc->dev, "%s Unable to reset chg_det block: %x\n",
						 __func__, chg_ctrl);
}

static const char *chg_to_string(enum dwc3_chg_type chg_type)
{
	switch (chg_type) {
	case USB_SDP_CHARGER:		return "USB_SDP_CHARGER";
	case USB_DCP_CHARGER:		return "USB_DCP_CHARGER";
	case USB_CDP_CHARGER:		return "USB_CDP_CHARGER";
	default:			return "INVALID_CHARGER";
	}
}

#define DWC3_CHG_DCD_POLL_TIME		(100 * HZ/1000) /* 100 msec */
#define DWC3_CHG_DCD_MAX_RETRIES	6 /* Tdcd_tmout = 6 * 100 msec */
#define DWC3_CHG_PRIMARY_DET_TIME	(50 * HZ/1000) /* TVDPSRC_ON */
#define DWC3_CHG_SECONDARY_DET_TIME	(50 * HZ/1000) /* TVDMSRC_ON */

static void dwc3_chg_detect_work(struct work_struct *w)
{
	struct dwc3_ipq *mdwc = container_of(w, struct dwc3_ipq, chg_work.work);
	bool is_dcd = false, tmout, vout;
	unsigned long delay;

	dev_dbg(mdwc->dev, "chg detection work\n");
	switch (mdwc->chg_state) {
	case USB_CHG_STATE_UNDEFINED:
		dwc3_chg_block_reset(mdwc);
		dwc3_chg_enable_dcd(mdwc);
		mdwc->chg_state = USB_CHG_STATE_WAIT_FOR_DCD;
		mdwc->dcd_retries = 0;
		delay = DWC3_CHG_DCD_POLL_TIME;
		break;
	case USB_CHG_STATE_WAIT_FOR_DCD:
		is_dcd = dwc3_chg_check_dcd(mdwc);
		tmout = ++mdwc->dcd_retries == DWC3_CHG_DCD_MAX_RETRIES;
		if (is_dcd || tmout) {
			dwc3_chg_disable_dcd(mdwc);
			dwc3_chg_enable_primary_det(mdwc);
			delay = DWC3_CHG_PRIMARY_DET_TIME;
			mdwc->chg_state = USB_CHG_STATE_DCD_DONE;
		} else {
			delay = DWC3_CHG_DCD_POLL_TIME;
		}
		break;
	case USB_CHG_STATE_DCD_DONE:
		vout = dwc3_chg_det_check_output(mdwc);
		if (vout) {
			dwc3_chg_enable_secondary_det(mdwc);
			delay = DWC3_CHG_SECONDARY_DET_TIME;
			mdwc->chg_state = USB_CHG_STATE_PRIMARY_DONE;
		} else {
			mdwc->charger.chg_type = USB_SDP_CHARGER;
			mdwc->chg_state = USB_CHG_STATE_DETECTED;
			delay = 0;
		}
		break;
	case USB_CHG_STATE_PRIMARY_DONE:
		vout = dwc3_chg_det_check_output(mdwc);
		if (vout)
			mdwc->charger.chg_type = USB_DCP_CHARGER;
		else
			mdwc->charger.chg_type = USB_CDP_CHARGER;
		mdwc->chg_state = USB_CHG_STATE_SECONDARY_DONE;
		/* fall through */
	case USB_CHG_STATE_SECONDARY_DONE:
		mdwc->chg_state = USB_CHG_STATE_DETECTED;
		/* fall through */
	case USB_CHG_STATE_DETECTED:
		dwc3_chg_block_reset(mdwc);
		dev_dbg(mdwc->dev, "chg_type = %s\n",
			chg_to_string(mdwc->charger.chg_type));
		mdwc->charger.notify_detection_complete(mdwc->otg_xceiv->otg,
								&mdwc->charger);
		return;
	default:
		return;
	}

	queue_delayed_work(system_nrt_wq, &mdwc->chg_work, delay);
}

static void dwc3_start_chg_det(struct dwc3_charger *charger, bool start)
{
	struct dwc3_ipq *mdwc = context;

	if (start == false) {
		cancel_delayed_work_sync(&mdwc->chg_work);
		mdwc->chg_state = USB_CHG_STATE_UNDEFINED;
		charger->chg_type = DWC3_INVALID_CHARGER;
		return;
	}

	mdwc->chg_state = USB_CHG_STATE_UNDEFINED;
	charger->chg_type = DWC3_INVALID_CHARGER;
	queue_delayed_work(system_nrt_wq, &mdwc->chg_work, 0);
}

static int dwc3_ipq_suspend(struct dwc3_ipq *mdwc)
{
	dev_dbg(mdwc->dev, "%s: entering lpm\n", __func__);

	if (atomic_read(&mdwc->in_lpm)) {
		dev_dbg(mdwc->dev, "%s: Already suspended\n", __func__);
		return 0;
	}

	clk_disable_unprepare(mdwc->utmi_b0_clk);
	clk_disable_unprepare(mdwc->utmi_clk);
	clk_disable_unprepare(mdwc->iface1_clk);
	clk_disable_unprepare(mdwc->iface0_clk);
	clk_disable_unprepare(mdwc->core_clk);
	clk_disable_unprepare(mdwc->utmi_b1_clk);
	wake_unlock(&mdwc->wlock);

	if (mdwc->bus_perf_client) {
		int ret;
		ret = msm_bus_scale_client_update_request(
						mdwc->bus_perf_client, 0);
		if (ret)
			dev_err(mdwc->dev, "Failed to reset bus bw vote\n");
	}

	atomic_set(&mdwc->in_lpm, 1);
	dev_info(mdwc->dev, "DWC3 in low power mode\n");

	return 0;
}

static int dwc3_ipq_resume(struct dwc3_ipq *mdwc)
{
	dev_dbg(mdwc->dev, "%s: exiting lpm\n", __func__);

	if (!atomic_read(&mdwc->in_lpm)) {
		dev_dbg(mdwc->dev, "%s: Already resumed\n", __func__);
		return 0;
	}

	if (mdwc->bus_perf_client) {
		int ret;
		ret = msm_bus_scale_client_update_request(
						mdwc->bus_perf_client, 1);
		if (ret)
			dev_err(mdwc->dev, "Failed to vote for bus scaling\n");
	}

	wake_lock(&mdwc->wlock);
	clk_prepare_enable(mdwc->core_clk);
	clk_prepare_enable(mdwc->iface0_clk);
	clk_prepare_enable(mdwc->iface1_clk);
	clk_prepare_enable(mdwc->utmi_clk);
	clk_prepare_enable(mdwc->utmi_b0_clk);
	clk_prepare_enable(mdwc->utmi_b1_clk);
	atomic_set(&mdwc->in_lpm, 0);
	dev_info(mdwc->dev, "DWC3 exited from low power mode\n");

	return 0;
}

static void dwc3_resume_work(struct work_struct *w)
{
	struct dwc3_ipq *mdwc = container_of(w, struct dwc3_ipq,
							resume_work.work);

	dev_dbg(mdwc->dev, "%s: dwc3 resume work\n", __func__);
	/* handle any event that was queued while work was already running */
	if (!atomic_read(&mdwc->in_lpm)) {
		dev_dbg(mdwc->dev, "%s: notifying xceiv event\n", __func__);
		if (mdwc->otg_xceiv)
			mdwc->ext_xceiv.notify_ext_events(mdwc->otg_xceiv->otg,
							DWC3_EVENT_XCEIV_STATE);
		return;
	}

	/* bail out if system resume in process, else initiate RESUME */
	if (atomic_read(&mdwc->pm_suspended)) {
		mdwc->resume_pending = true;
	} else {
		pm_runtime_get_sync(mdwc->dev);
		if (mdwc->otg_xceiv)
			mdwc->ext_xceiv.notify_ext_events(mdwc->otg_xceiv->otg,
							DWC3_EVENT_PHY_RESUME);
		pm_runtime_put_sync(mdwc->dev);
	}
}

static bool debug_id, debug_bsv, debug_connect;

static int dwc3_connect_show(struct seq_file *s, void *unused)
{
	if (debug_connect)
		seq_printf(s, "true\n");
	else
		seq_printf(s, "false\n");

	return 0;
}

static int dwc3_connect_open(struct inode *inode, struct file *file)
{
	return single_open(file, dwc3_connect_show, inode->i_private);
}

static ssize_t dwc3_connect_write(struct file *file, const char __user *ubuf,
				size_t count, loff_t *ppos)
{
	struct seq_file *s = file->private_data;
	struct dwc3_ipq *mdwc = s->private;
	char buf[8];

	memset(buf, 0x00, sizeof(buf));

	if (copy_from_user(&buf, ubuf, min_t(size_t, sizeof(buf) - 1, count)))
		return -EFAULT;

	if (!strncmp(buf, "enable", 6) || !strncmp(buf, "true", 4)) {
		debug_connect = true;
	} else {
		debug_connect = debug_bsv = false;
		debug_id = true;
	}

	mdwc->ext_xceiv.bsv = debug_bsv;
	mdwc->ext_xceiv.id = debug_id ? DWC3_ID_FLOAT : DWC3_ID_GROUND;

	if (atomic_read(&mdwc->in_lpm)) {
		dev_dbg(mdwc->dev, "%s: calling resume_work\n", __func__);
		dwc3_resume_work(&mdwc->resume_work.work);
	} else {
		dev_dbg(mdwc->dev, "%s: notifying xceiv event\n", __func__);
		if (mdwc->otg_xceiv)
			mdwc->ext_xceiv.notify_ext_events(mdwc->otg_xceiv->otg,
							DWC3_EVENT_XCEIV_STATE);
	}

	return count;
}

const struct file_operations dwc3_connect_fops = {
	.open = dwc3_connect_open,
	.read = seq_read,
	.write = dwc3_connect_write,
	.llseek = seq_lseek,
	.release = single_release,
};

static struct dentry *dwc3_debugfs_root;

static void dwc3_debugfs_init(struct dwc3_ipq *mdwc)
{
	dwc3_debugfs_root = debugfs_create_dir("ipq_dwc3", NULL);

	if (!dwc3_debugfs_root || IS_ERR(dwc3_debugfs_root))
		return;

	if (!debugfs_create_bool("id", S_IRUGO | S_IWUSR, dwc3_debugfs_root,
				 (u32 *)&debug_id))
		goto error;

	if (!debugfs_create_bool("bsv", S_IRUGO | S_IWUSR, dwc3_debugfs_root,
				 (u32 *)&debug_bsv))
		goto error;

	if (!debugfs_create_file("connect", S_IRUGO | S_IWUSR,
				dwc3_debugfs_root, mdwc, &dwc3_connect_fops))
		goto error;

	return;

error:
	debugfs_remove_recursive(dwc3_debugfs_root);
}

static int dwc3_ipq_power_down(struct dwc3_ipq *mdwc)
{
	int data, reg;

	/*Setting HCRST bit USBCMD register*/
	data = readl(mdwc->base + 0x0020);
	data |= 0x2;
	writel(data, mdwc->base + 0x0020);
	/*Global USB3 PIPE Control Register, USB3 PHY Soft Reset bit 31*/
	data = readl(mdwc->base + DWC3_SSUSB_REG_GUSB3PIPECTL(0));
	data |= 0x80000000;
	writel(data, mdwc->base + DWC3_SSUSB_REG_GUSB3PIPECTL(0));
	/*Global USB2 PHY Configuration Register, UTMI PHY Soft Reset bit 31*/
	data = readl(mdwc->base + DWC3_SSUSB_REG_GUSB2PHYCFG(0));
	data |= 0x80000000;
	writel(data, mdwc->base + DWC3_SSUSB_REG_GUSB2PHYCFG(0));
	/*Global Control Register, Core Soft Reset bit 11*/
	data = readl(mdwc->base + DWC3_SSUSB_REG_GCTL);
	data |= 0x804;
	writel(data, mdwc->base + DWC3_SSUSB_REG_GCTL);
	/*USB30_QSCRATCH_SS_PHY_CTRL,  SS_PHY_RESET bit 7 */
	data = readl(mdwc->base + IPQ_SS_PHY_CTRL_REG);
	data |= 0x80;
	writel(data, mdwc->base + IPQ_SS_PHY_CTRL_REG);
	/*USB30 RESET*/
	reg = USB30_RESET;
	writel(0x3F, reg);

	return 0;
}

static int __devinit dwc3_ipq_probe(struct platform_device *pdev)
{
	struct device_node *node = pdev->dev.of_node;
	struct platform_device *dwc3;
	struct dwc3_ipq *ipq;
	struct resource *res;
	void __iomem *tcsr;
	int ret = 0;
	struct dwc3_platform_data *pd = pdev->dev.platform_data;

	ipq = devm_kzalloc(&pdev->dev, sizeof(*ipq), GFP_KERNEL);
	if (!ipq) {
		dev_err(&pdev->dev, "not enough memory\n");
		return -ENOMEM;
	}

	platform_set_drvdata(pdev, ipq);
	context = ipq;
	ipq->dev = &pdev->dev;
	INIT_LIST_HEAD(&ipq->req_complete_list);
	INIT_DELAYED_WORK(&ipq->chg_work, dwc3_chg_detect_work);
	INIT_DELAYED_WORK(&ipq->resume_work, dwc3_resume_work);

	/* Initialize clocks */
	if (pdev->id == 0) {
		ipq->core_clk = devm_clk_get(&pdev->dev, "core_clk");
		if (IS_ERR(ipq->core_clk)) {
			dev_err(&pdev->dev, "failed to get core_clk\n");
			return PTR_ERR(ipq->core_clk);
		}
		clk_set_rate(ipq->core_clk, 125000000);
		clk_prepare_enable(ipq->core_clk);

		ipq->iface0_clk = devm_clk_get(&pdev->dev, "iface0_clk");
		if (IS_ERR(ipq->iface0_clk)) {
			dev_err(&pdev->dev, "failed to get iface0_clk\n");
			ret = PTR_ERR(ipq->iface0_clk);
			goto disable_core_clk;
		}
		clk_prepare_enable(ipq->iface0_clk);

		ipq->iface1_clk = devm_clk_get(&pdev->dev, "iface1_clk");
		if (IS_ERR(ipq->iface1_clk)) {
			dev_err(&pdev->dev, "failed to get iface1_clk\n");
			ret = PTR_ERR(ipq->iface1_clk);
			goto disable_iface0_clk;
		}
		clk_prepare_enable(ipq->iface1_clk);

		ipq->utmi_clk = devm_clk_get(&pdev->dev, "utmi_clk");
		if (IS_ERR(ipq->utmi_clk)) {
			dev_err(&pdev->dev, "failed to get utmi_clk\n");
			goto disable_iface1_clk;
		}
		clk_set_rate(ipq->utmi_clk, 60000000);
		clk_prepare_enable(ipq->utmi_clk);

		ipq->utmi_b0_clk = devm_clk_get(&pdev->dev, "utmi_b0_clk");
		if (IS_ERR(ipq->utmi_b0_clk)) {
			dev_err(&pdev->dev, "failed to get utmi_b0_clk \n");
			ret = PTR_ERR(ipq->utmi_b0_clk);
			goto disable_utmi_clk;
		}
		clk_prepare_enable(ipq->utmi_b0_clk);

		ipq->utmi_b1_clk = devm_clk_get(&pdev->dev, "utmi_b1_clk");
		if (IS_ERR(ipq->utmi_b1_clk)) {
			dev_err(&pdev->dev, "failed to get utmi_b0_clk\n");
			ret = PTR_ERR(ipq->utmi_b1_clk);
			goto disable_utmi_b0__clk;
		}
		clk_prepare_enable(ipq->utmi_b1_clk);
	}

	res = platform_get_resource(pdev, IORESOURCE_MEM, 1);
	if (!res) {
		dev_dbg(&pdev->dev, "missing TCSR memory resource\n");
	} else {
		tcsr = devm_ioremap_nocache(&pdev->dev, res->start,
			resource_size(res));
		if (!tcsr) {
			dev_err(&pdev->dev, "tcsr ioremap failed\n");
			ret = -ENODEV;
			return ret;
		} else {
			/* Enable USB3 on the primary USB port. */
			writel_relaxed(0x1, tcsr);
			/*
			 * Ensure that TCSR write is completed before
			 * USB registers initialization.
			 */
			mb();
		}
	}

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(&pdev->dev, "missing memory base resource\n");
		ret = -ENODEV;
		return ret;
	}

	ipq->base = devm_ioremap_nocache(&pdev->dev, res->start,
		resource_size(res));
	if (!ipq->base) {
		dev_err(&pdev->dev, "ioremap failed\n");
		ret = -ENODEV;
		return ret;
	}

	dwc3 = platform_device_alloc("dwc3", pdev->id);
	if (!dwc3) {
		dev_err(&pdev->dev, "couldn't allocate dwc3 device\n");
		ret = -ENODEV;
		return ret;
	}

	dwc3->dev.parent = &pdev->dev;
	dwc3->dev.coherent_dma_mask = DMA_BIT_MASK(32);
	dwc3->dev.dma_mask = &dwc3_ipq_dma_mask;
	dwc3->dev.dma_parms = pdev->dev.dma_parms;
	dwc3->dev.id = pdev->id;
	ipq->resource_size = resource_size(res);
	ipq->dwc3 = dwc3;
	ipq->mode = pd->usb_mode;
	if(pdev->id == 0)
		usb30_common_pre_init(pdev->id, ipq->base);
	uw_ssusb_pre_init(ipq->base, pd);
	if (pd->pwr_en) {
		writel((0 << IPQ_GPIO_IN_OUTn_GPIO_OUT_SHFT),
			(IPQ_GPIO_IN_OUTn(pd->pwr_en_gpio1)));
		writel((0 << IPQ_GPIO_IN_OUTn_GPIO_OUT_SHFT),
			(IPQ_GPIO_IN_OUTn(pd->pwr_en_gpio2)));
	}
	pm_runtime_set_active(ipq->dev);

	if (of_property_read_u32(node, "qcom,dwc-usb3-ipq-dbm-eps",
				 &ipq->dbm_num_eps)) {
		dev_err(&pdev->dev,
			"unable to read platform data num of dbm eps\n");
		ipq->dbm_num_eps = DBM_MAX_EPS;
	}

	if (ipq->dbm_num_eps > DBM_MAX_EPS) {
		dev_err(&pdev->dev,
			"Driver doesn't support number of DBM EPs. "
			"max: %d, dbm_num_eps: %d\n",
			DBM_MAX_EPS, ipq->dbm_num_eps);
		ret = -ENODEV;
		goto put_pdev;
	}

	ret = platform_device_add_resources(dwc3, pdev->resource,
		pdev->num_resources);
	if (ret) {
		dev_err(&pdev->dev, "couldn't add resources to dwc3 device\n");
		goto put_pdev;
	}

	ret = platform_device_add_data(dwc3, &ipq->mode, sizeof(ipq->mode));
	if (ret) {
		dev_err(&pdev->dev, "failed to add platform_data\n");
		goto put_pdev;
	}

	/*
	 * dwc3 does not need full memory range as dwc3-ipq, so we
	 * will trim the region here.
	 */
	res = platform_get_resource(dwc3, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(&pdev->dev, "missing memory base resource\n");
		ret = -ENODEV;
		return ret;
	}
	res->end = res->start + pd->dwc3_core_size;

	ret = platform_device_add(dwc3);
	if (ret) {
		dev_err(&pdev->dev, "failed to register dwc3 device\n");
		goto put_pdev;
	}

	/* Reset the DBM */
	dwc3_ipq_dbm_soft_reset(1);
	usleep_range(1000, 1200);
	dwc3_ipq_dbm_soft_reset(0);
	dwc3_ipq_event_buffer_config(dwc3_readl(ipq->base, DWC3_GEVNTADRLO(0)),
		dwc3_readl(ipq->base, DWC3_GEVNTSIZ(0)));
	wake_lock_init(&ipq->wlock, WAKE_LOCK_SUSPEND, "ipq_dwc3");
	wake_lock(&ipq->wlock);
	dwc3_debugfs_init(ipq);

	return 0;

put_pdev:
	platform_device_put(dwc3);
disable_utmi_b0__clk:
	clk_disable_unprepare(ipq->utmi_b0_clk);
disable_utmi_clk:
	clk_disable_unprepare(ipq->utmi_clk);
disable_iface1_clk:
	clk_disable_unprepare(ipq->iface1_clk);
disable_iface0_clk:
	clk_disable_unprepare(ipq->iface0_clk);
disable_core_clk:
	clk_disable_unprepare(ipq->core_clk);
	return ret;
}

static int __devexit dwc3_ipq_remove(struct platform_device *pdev)
{
	struct dwc3_ipq	*ipq = platform_get_drvdata(pdev);

	if (dwc3_debugfs_root)
		debugfs_remove_recursive(dwc3_debugfs_root);
	if (ipq->otg_xceiv) {
		dwc3_start_chg_det(&ipq->charger, false);
		usb_put_transceiver(ipq->otg_xceiv);
	}
	pm_runtime_disable(ipq->dev);
	platform_device_unregister(ipq->dwc3);
	dwc3_ipq_power_down(ipq);
	devm_iounmap(&pdev->dev, ipq->base);
	devm_kfree(&pdev->dev, ipq);
	wake_lock_destroy(&ipq->wlock);

	return 0;
}

static int dwc3_ipq_pm_suspend(struct device *dev)
{
	int ret = 0;
	struct dwc3_ipq *mdwc = dev_get_drvdata(dev);

	dev_dbg(dev, "dwc3-ipq PM suspend\n");

	ret = dwc3_ipq_suspend(mdwc);
	if (!ret)
		atomic_set(&mdwc->pm_suspended, 1);

	return ret;
}

static int dwc3_ipq_pm_resume(struct device *dev)
{
	int ret = 0;
	struct dwc3_ipq *mdwc = dev_get_drvdata(dev);

	dev_dbg(dev, "dwc3-ipq PM resume\n");

	atomic_set(&mdwc->pm_suspended, 0);
	if (mdwc->resume_pending) {
		mdwc->resume_pending = false;

		ret = dwc3_ipq_resume(mdwc);
		/* Update runtime PM status */
		pm_runtime_disable(dev);
		pm_runtime_set_active(dev);
		pm_runtime_enable(dev);

		/* Let OTG know about resume event and update pm_count */
		if (mdwc->otg_xceiv)
			mdwc->ext_xceiv.notify_ext_events(mdwc->otg_xceiv->otg,
							DWC3_EVENT_PHY_RESUME);
	}

	return ret;
}

static int dwc3_ipq_runtime_idle(struct device *dev)
{
	dev_dbg(dev, "DWC3-ipq runtime idle\n");

	return 0;
}

static int dwc3_ipq_runtime_suspend(struct device *dev)
{
	struct dwc3_ipq *mdwc = dev_get_drvdata(dev);

	dev_dbg(dev, "DWC3-ipq runtime suspend\n");

	return dwc3_ipq_suspend(mdwc);
}

static int dwc3_ipq_runtime_resume(struct device *dev)
{
	struct dwc3_ipq *mdwc = dev_get_drvdata(dev);

	dev_dbg(dev, "DWC3-ipq runtime resume\n");

	return dwc3_ipq_resume(mdwc);
}

static const struct dev_pm_ops dwc3_ipq_dev_pm_ops = {
	SET_SYSTEM_SLEEP_PM_OPS(dwc3_ipq_pm_suspend, dwc3_ipq_pm_resume)
	SET_RUNTIME_PM_OPS(dwc3_ipq_runtime_suspend, dwc3_ipq_runtime_resume,
				dwc3_ipq_runtime_idle)
};

static const struct of_device_id of_dwc3_matach[] = {
	{
		.compatible = "qcom,dwc-usb3-ipq",
	},
	{ },
};
MODULE_DEVICE_TABLE(of, of_dwc3_matach);

static struct platform_driver dwc3_ipq_driver = {
	.probe		= dwc3_ipq_probe,
	.remove		= __devexit_p(dwc3_ipq_remove),
	.driver		= {
		.name	= "ipq-dwc3",
		.pm	= &dwc3_ipq_dev_pm_ops,
		.of_match_table	= of_dwc3_matach,
	},
};

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("DesignWare USB3 IPQ Glue Layer");

static int __devinit dwc3_ipq_init(void)
{
	return platform_driver_register(&dwc3_ipq_driver);
}
module_init(dwc3_ipq_init);

static void __exit dwc3_ipq_exit(void)
{
	platform_driver_unregister(&dwc3_ipq_driver);
}

module_exit(dwc3_ipq_exit);
