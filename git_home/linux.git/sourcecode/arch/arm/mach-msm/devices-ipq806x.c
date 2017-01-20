/* * Copyright (c) 2013, 2015 The Linux Foundation. All rights reserved.* */
/* Copyright (c) 2011-2012, The Linux Foundation. All rights reserved.
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

#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <linux/clkdev.h>
#include <linux/dma-mapping.h>
#include <linux/coresight.h>
#include <linux/mdio-gpio.h>
#include <mach/irqs-ipq806x.h>
#include <mach/board.h>
#include <mach/msm_iomap.h>
#include <mach/usbdiag.h>
#include <mach/msm_sps.h>
#include <mach/dma.h>
#include <mach/msm_dsps.h>
#include <mach/clk-provider.h>
#include <sound/msm-dai-q6.h>
#include <sound/apr_audio.h>
#include <mach/msm_tsif.h>
#include <mach/msm_tspp.h>
#include <mach/msm_bus_board.h>
#include <mach/rpm.h>
#include <mach/mdm2.h>
#include <mach/msm_smd.h>
#include <mach/msm_dcvs.h>
#include <mach/msm_rtb.h>
#include <asm/mach/flash.h>
#include <linux/msm_ion.h>
#include "clock.h"
#include "devices.h"
#include "footswitch.h"
#include "msm_watchdog.h"
#include "rpm_stats.h"
#include "rpm_log.h"
#include "pcie.h"
#include <mach/mpm.h>
#include <mach/iommu_domains.h>
#include <mach/msm_cache_dump.h>
#include <mach/msm_memtypes.h>
#include <mach/msm_nss_gmac.h>
#include <mach/msm_nss_macsec.h>
#include <mach/msm_nss.h>
#include <mach/msm_nss_crypto.h>
#include <linux/ar8216_platform.h>
#include <linux/aq_phy.h>
#include <linux/qca_85xx_sw.h>
#include <mach/msm_usb30.h>
#include <mach/cache_erp.h>

/* Address of GSBI blocks */
#define MSM_GSBI1_PHYS		0x12440000
#define MSM_GSBI2_PHYS		0x12480000
#define MSM_GSBI3_PHYS		0x16200000
#define MSM_GSBI4_PHYS		0x16300000
#define MSM_GSBI5_PHYS		0x1A200000
#define MSM_GSBI6_PHYS		0x16500000
#define MSM_GSBI7_PHYS		0x16600000

/* GSBI UART devices */
#define MSM_UART1DM_PHYS	(MSM_GSBI1_PHYS + 0x10000)
#define MSM_UART2DM_PHYS	(MSM_GSBI2_PHYS + 0x10000)
#define MSM_UART3DM_PHYS	(MSM_GSBI3_PHYS + 0x40000)
#define MSM_UART4DM_PHYS	(MSM_GSBI4_PHYS + 0x40000)
#define MSM_UART6DM_PHYS	(MSM_GSBI6_PHYS + 0x40000)
#define MSM_UART7DM_PHYS	(MSM_GSBI7_PHYS + 0x40000)

/* GSBI QUP devices */
#define MSM_GSBI1_QUP_PHYS	(MSM_GSBI1_PHYS + 0x20000)
#define MSM_GSBI2_QUP_PHYS	(MSM_GSBI2_PHYS + 0x20000)
#define MSM_GSBI3_QUP_PHYS	(MSM_GSBI3_PHYS + 0x80000)
#define MSM_GSBI4_QUP_PHYS	(MSM_GSBI4_PHYS + 0x80000)
#define MSM_GSBI5_QUP_PHYS	(MSM_GSBI5_PHYS + 0x80000)
#define MSM_GSBI6_QUP_PHYS	(MSM_GSBI6_PHYS + 0x80000)
#define MSM_GSBI7_QUP_PHYS	(MSM_GSBI7_PHYS + 0x80000)
#define MSM_QUP_SIZE		SZ_4K

/* Address of SSBI CMD */
#define MSM_PMIC1_SSBI_CMD_PHYS	0x00500000
#define MSM_PMIC2_SSBI_CMD_PHYS	0x00C00000
#define MSM_PMIC_SSBI_SIZE	SZ_4K

/* Address of HS USBOTG1 */
#define MSM_HSUSB1_PHYS		0x12500000
#define MSM_HSUSB1_SIZE		SZ_4K

/* Address of HS USB3 */
#define MSM_HSUSB3_PHYS		0x12520000
#define MSM_HSUSB3_SIZE		SZ_4K

/* Address of HS USB4 */
#define MSM_HSUSB4_PHYS		0x12530000
#define MSM_HSUSB4_SIZE		SZ_4K

/* Address of SS USB1 */
#define MSM_SSUSB1_PHYS		0x11000000
#define MSM_SSUSB1_SIZE		SZ_16M

/* Address of SS USB2 */
#define MSM_SSUSB2_PHYS		0x10000000
#define MSM_SSUSB2_SIZE		SZ_16M

/* Address of PCIE20 PARF */
#define PCIE20_0_PARF_PHYS 0x1b600000
#define PCIE20_1_PARF_PHYS 0x1b800000
#define PCIE20_2_PARF_PHYS 0x1ba00000
#define PCIE20_PARF_SIZE   SZ_128

/* Address of PCIE20 ELBI */
#define PCIE20_0_ELBI_PHYS 0x1b502000
#define PCIE20_1_ELBI_PHYS 0x1b702000
#define PCIE20_2_ELBI_PHYS 0x1b902000

#define PCIE20_ELBI_SIZE   SZ_256

/* Address of PCIE20 */
#define PCIE20_0_PHYS 0x1b500000
#define PCIE20_1_PHYS 0x1b700000
#define PCIE20_2_PHYS 0x1b900000
#define PCIE20_SIZE   SZ_4K

#define IPQ806X_RPM_MASTER_STATS_BASE	0x10BB00
#define IPQ806X_PC_CNTR_PHYS	(IPQ806X_IMEM_PHYS + 0x664)
#define IPQ806X_PC_CNTR_SIZE		0x40

/* LPASS Addresses */
#define IPQ_DML_START	0x28008040
#define IPQ_DML_END	0x28008060

#define IPQ_LPAIF_SPDIF_PHYS	0x28080000
#define IPQ_LPAIF_SPDIF_END	0x28081ffc

#define IPQ_LPAIF_PHYS	0x28100000
#define IPQ_LPAIF_END	0x2810ffff

/* NSS Firmware default load addresses */
#define NSS_CORE0_LOAD_ADDR 0x40000000
#define NSS_CORE1_LOAD_ADDR 0x40800000

static struct resource ipq806x_resources_pccntr[] = {
	{
		.start	= IPQ806X_PC_CNTR_PHYS,
		.end	= IPQ806X_PC_CNTR_PHYS + IPQ806X_PC_CNTR_SIZE,
		.flags	= IORESOURCE_MEM,
	},
};

struct platform_device ipq806x_pc_cntr = {
	.name		= "pc-cntr",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(ipq806x_resources_pccntr),
	.resource	= ipq806x_resources_pccntr,
};

static struct msm_watchdog_pdata msm_watchdog_pdata = {
	.pet_time = 5000,
	.bark_time = 10000,
	.has_secure = true,
	.needs_expired_enable = true,
	.base = MSM_TMR0_BASE + WDT0_OFFSET,
};

static struct resource msm_watchdog_resources[] = {
	{
		.start	= WDT0_ACCSCSSNBARK_INT,
		.end	= WDT0_ACCSCSSNBARK_INT,
		.flags	= IORESOURCE_IRQ,
	},
};

struct platform_device ipq806x_device_watchdog = {
	.name = "msm_watchdog",
	.id = -1,
	.dev = {
		.platform_data = &msm_watchdog_pdata,
	},
	.num_resources	= ARRAY_SIZE(msm_watchdog_resources),
	.resource	= msm_watchdog_resources,
};

static struct resource qcom_wdt_resource[] =  {
	{
		.start = 0x0208a038,
		.end = 0x0208a078,
		.flags = IORESOURCE_MEM,
	},
	{
		.start	= WDT0_ACCSCSSNBARK_INT,
		.end	= WDT0_ACCSCSSNBARK_INT,
		.flags	= IORESOURCE_IRQ,
	},
};

struct platform_device qcom_wdt_device = {
	.name		= "qcom_wdt",
	.id		= -1,
	.resource	= qcom_wdt_resource,
	.num_resources	= ARRAY_SIZE(qcom_wdt_resource),
};

static struct resource msm_dmov_resource[] = {
	{
		.start = ADM_0_SCSS_0_IRQ,
		.flags = IORESOURCE_IRQ,
	},
	{
		.start = 0x18300000,
		.end = 0x18300000 + SZ_1M - 1,
		.flags = IORESOURCE_MEM,
	},
};

static struct msm_dmov_pdata msm_dmov_pdata = {
	.sd = 0,
	.sd_size = 0x800,
};

struct platform_device ipq806x_device_dmov = {
	.name	= "msm_dmov",
	.id	= -1,
	.resource = msm_dmov_resource,
	.num_resources = ARRAY_SIZE(msm_dmov_resource),
	.dev = {
		.platform_data = &msm_dmov_pdata,
	},
};

#define CORESIGHT_PHYS_BASE		0x01A00000
#define CORESIGHT_TPIU_PHYS_BASE	(CORESIGHT_PHYS_BASE + 0x3000)
#define CORESIGHT_ETB_PHYS_BASE		(CORESIGHT_PHYS_BASE + 0x1000)
#define CORESIGHT_FUNNEL_PHYS_BASE	(CORESIGHT_PHYS_BASE + 0x4000)
#define CORESIGHT_STM_PHYS_BASE		(CORESIGHT_PHYS_BASE + 0x6000)
#define CORESIGHT_ETM0_PHYS_BASE	(CORESIGHT_PHYS_BASE + 0x1C000)
#define CORESIGHT_ETM1_PHYS_BASE	(CORESIGHT_PHYS_BASE + 0x1D000)

#define CORESIGHT_STM_CHANNEL_PHYS_BASE	(0x14000000 + 0x280000)

static struct resource coresight_tpiu_resources[] = {
	{
		.start = CORESIGHT_TPIU_PHYS_BASE,
		.end   = CORESIGHT_TPIU_PHYS_BASE + SZ_4K - 1,
		.flags = IORESOURCE_MEM,
	},
};

static struct coresight_platform_data coresight_tpiu_pdata = {
	.id		= 0,
	.name		= "coresight-tpiu",
	.nr_inports	= 1,
	.nr_outports	= 0,
};

struct platform_device coresight_tpiu_device = {
	.name          = "coresight-tpiu",
	.id            = 0,
	.num_resources = ARRAY_SIZE(coresight_tpiu_resources),
	.resource      = coresight_tpiu_resources,
	.dev = {
		.platform_data = &coresight_tpiu_pdata,
	},
};

static struct resource coresight_etb_resources[] = {
	{
		.start = CORESIGHT_ETB_PHYS_BASE,
		.end   = CORESIGHT_ETB_PHYS_BASE + SZ_4K - 1,
		.flags = IORESOURCE_MEM,
	},
};

static struct coresight_platform_data coresight_etb_pdata = {
	.id		= 1,
	.name		= "coresight-etb",
	.nr_inports	= 1,
	.nr_outports	= 0,
	.default_sink	= true,
};

struct platform_device coresight_etb_device = {
	.name          = "coresight-etb",
	.id            = 0,
	.num_resources = ARRAY_SIZE(coresight_etb_resources),
	.resource      = coresight_etb_resources,
	.dev = {
		.platform_data = &coresight_etb_pdata,
	},
};

static struct resource coresight_funnel_resources[] = {
	{
		.start = CORESIGHT_FUNNEL_PHYS_BASE,
		.end   = CORESIGHT_FUNNEL_PHYS_BASE + SZ_4K - 1,
		.flags = IORESOURCE_MEM,
	},
};

static const int coresight_funnel_outports[] = { 0, 1 };
static const int coresight_funnel_child_ids[] = { 0, 1 };
static const int coresight_funnel_child_ports[] = { 0, 0 };

static struct coresight_platform_data coresight_funnel_pdata = {
	.id		= 2,
	.name		= "coresight-funnel",
	.nr_inports	= 4,
	.outports	= coresight_funnel_outports,
	.child_ids	= coresight_funnel_child_ids,
	.child_ports	= coresight_funnel_child_ports,
	.nr_outports	= ARRAY_SIZE(coresight_funnel_outports),
};

struct platform_device ipq806x_coresight_funnel_device = {
	.name          = "coresight-funnel",
	.id            = 0,
	.num_resources = ARRAY_SIZE(coresight_funnel_resources),
	.resource      = coresight_funnel_resources,
	.dev = {
		.platform_data = &coresight_funnel_pdata,
	},
};

static struct resource coresight_stm_resources[] = {
	{
		.start = CORESIGHT_STM_PHYS_BASE,
		.end   = CORESIGHT_STM_PHYS_BASE + SZ_4K - 1,
		.flags = IORESOURCE_MEM,
	},
	{
		.start = CORESIGHT_STM_CHANNEL_PHYS_BASE,
		.end   = CORESIGHT_STM_CHANNEL_PHYS_BASE + SZ_1M + SZ_512K - 1,
		.flags = IORESOURCE_MEM,
	},
};

static const int coresight_stm_outports[] = { 0 };
static const int coresight_stm_child_ids[] = { 2 };
static const int coresight_stm_child_ports[] = { 2 };

static struct coresight_platform_data coresight_stm_pdata = {
	.id		= 3,
	.name		= "coresight-stm",
	.nr_inports	= 0,
	.outports	= coresight_stm_outports,
	.child_ids	= coresight_stm_child_ids,
	.child_ports	= coresight_stm_child_ports,
	.nr_outports	= ARRAY_SIZE(coresight_stm_outports),
};

struct platform_device coresight_stm_device = {
	.name          = "coresight-stm",
	.id            = 0,
	.num_resources = ARRAY_SIZE(coresight_stm_resources),
	.resource      = coresight_stm_resources,
	.dev = {
		.platform_data = &coresight_stm_pdata,
	},
};

static struct resource coresight_etm0_resources[] = {
	{
		.start = CORESIGHT_ETM0_PHYS_BASE,
		.end   = CORESIGHT_ETM0_PHYS_BASE + SZ_4K - 1,
		.flags = IORESOURCE_MEM,
	},
};

static const int coresight_etm0_outports[] = { 0 };
static const int coresight_etm0_child_ids[] = { 2 };
static const int coresight_etm0_child_ports[] = { 0 };

static struct coresight_platform_data coresight_etm0_pdata = {
	.id		= 4,
	.name		= "coresight-etm0",
	.nr_inports	= 0,
	.outports	= coresight_etm0_outports,
	.child_ids	= coresight_etm0_child_ids,
	.child_ports	= coresight_etm0_child_ports,
	.nr_outports	= ARRAY_SIZE(coresight_etm0_outports),
};

struct platform_device coresight_etm0_device = {
	.name          = "coresight-etm",
	.id            = 0,
	.num_resources = ARRAY_SIZE(coresight_etm0_resources),
	.resource      = coresight_etm0_resources,
	.dev = {
		.platform_data = &coresight_etm0_pdata,
	},
};

static struct resource coresight_etm1_resources[] = {
	{
		.start = CORESIGHT_ETM1_PHYS_BASE,
		.end   = CORESIGHT_ETM1_PHYS_BASE + SZ_4K - 1,
		.flags = IORESOURCE_MEM,
	},
};

static const int coresight_etm1_outports[] = { 0 };
static const int coresight_etm1_child_ids[] = { 2 };
static const int coresight_etm1_child_ports[] = { 1 };

static struct coresight_platform_data coresight_etm1_pdata = {
	.id		= 5,
	.name		= "coresight-etm1",
	.nr_inports	= 0,
	.outports	= coresight_etm1_outports,
	.child_ids	= coresight_etm1_child_ids,
	.child_ports	= coresight_etm1_child_ports,
	.nr_outports	= ARRAY_SIZE(coresight_etm1_outports),
};

struct platform_device coresight_etm1_device = {
	.name          = "coresight-etm",
	.id            = 1,
	.num_resources = ARRAY_SIZE(coresight_etm1_resources),
	.resource      = coresight_etm1_resources,
	.dev = {
		.platform_data = &coresight_etm1_pdata,
	},
};

static struct resource msm_ebi1_ch0_erp_resources[] = {
	{
		.start = HSDDRX_EBI1CH0_IRQ,
		.flags = IORESOURCE_IRQ,
	},
	{
		.start = 0x00A40000,
		.end   = 0x00A40000 + SZ_4K - 1,
		.flags = IORESOURCE_MEM,
	},
};

struct platform_device ipq806x_device_ebi1_ch0_erp = {
	.name		= "msm_ebi_erp",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(msm_ebi1_ch0_erp_resources),
	.resource	= msm_ebi1_ch0_erp_resources,
};

static struct resource resources_uart_gsbi4[] = {
	{
		.start	= IPQ806X_GSBI4_UARTDM_IRQ,
		.end	= IPQ806X_GSBI4_UARTDM_IRQ,
		.flags	= IORESOURCE_IRQ,
	},
	{
		.start	= MSM_UART4DM_PHYS,
		.end	= MSM_UART4DM_PHYS + PAGE_SIZE - 1,
		.name	= "uartdm_resource",
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= MSM_GSBI4_PHYS,
		.end	= MSM_GSBI4_PHYS + PAGE_SIZE - 1,
		.name	= "gsbi_resource",
		.flags	= IORESOURCE_MEM,
	},
};

struct platform_device ipq806x_device_uart_gsbi4 = {
	.name	= "msm_serial_hsl",
	.id	= 1,
	.num_resources	= ARRAY_SIZE(resources_uart_gsbi4),
	.resource	= resources_uart_gsbi4,
};

static struct resource resources_uart_gsbi2[] = {
	{
		.start	= IPQ806X_GSBI2_UARTDM_IRQ,
		.end	= IPQ806X_GSBI2_UARTDM_IRQ,
		.flags	= IORESOURCE_IRQ,
	},
	{
		.start	= MSM_UART2DM_PHYS,
		.end	= MSM_UART2DM_PHYS + PAGE_SIZE - 1,
		.name	= "uartdm_resource",
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= MSM_GSBI2_PHYS,
		.end	= MSM_GSBI2_PHYS + PAGE_SIZE - 1,
		.name	= "gsbi_resource",
		.flags	= IORESOURCE_MEM,
	},
};

struct platform_device ipq806x_device_uart_gsbi2 = {
	.name	= "msm_serial_hsl",
	.id	= 1,
	.num_resources	= ARRAY_SIZE(resources_uart_gsbi2),
	.resource	= resources_uart_gsbi2,
};

static struct resource msm_uart_dm2_resources[] = {
	{
		.start  = MSM_UART2DM_PHYS,
		.end    = MSM_UART2DM_PHYS + PAGE_SIZE - 1,
		.name   = "uartdm_resource",
		.flags  = IORESOURCE_MEM,
	},
	{
		.start	= IPQ806X_GSBI2_UARTDM_IRQ,
		.end	= IPQ806X_GSBI2_UARTDM_IRQ,
		.flags  = IORESOURCE_IRQ,
	},
	{
		.start  = MSM_GSBI2_PHYS,
		.end    = MSM_GSBI2_PHYS + PAGE_SIZE - 1,
		.name   = "gsbi_resource",
		.flags  = IORESOURCE_MEM,
	},
	{
		.start  = DMOV_HSUART_GSBI2_TX_CHAN,
		.end    = DMOV_HSUART_GSBI2_RX_CHAN,
		.name   = "uartdm_channels",
		.flags  = IORESOURCE_DMA,
	},
	{
		.start  = DMOV_HSUART_GSBI2_TX_CRCI,
		.end    = DMOV_HSUART_GSBI2_RX_CRCI,
		.name   = "uartdm_crci",
		.flags  = IORESOURCE_DMA,
	},
};
static u64 msm_uart_dm2_dma_mask = DMA_BIT_MASK(32);
struct platform_device ipq806x_device_uartdm_gsbi2 = {
	.name   = "msm_serial_hs",
	.id     = 0,
	.num_resources  = ARRAY_SIZE(msm_uart_dm2_resources),
	.resource       = msm_uart_dm2_resources,
	.dev    = {
		.dma_mask		= &msm_uart_dm2_dma_mask,
		.coherent_dma_mask	= DMA_BIT_MASK(32),
	},
};

static struct resource resources_uart_gsbi3[] = {
	{
		.start	= GSBI3_UARTDM_IRQ,
		.end	= GSBI3_UARTDM_IRQ,
		.flags	= IORESOURCE_IRQ,
	},
	{
		.start	= MSM_UART3DM_PHYS,
		.end	= MSM_UART3DM_PHYS + PAGE_SIZE - 1,
		.name	= "uartdm_resource",
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= MSM_GSBI3_PHYS,
		.end	= MSM_GSBI3_PHYS + PAGE_SIZE - 1,
		.name	= "gsbi_resource",
		.flags	= IORESOURCE_MEM,
	},
};

struct platform_device ipq806x_device_uart_gsbi3 = {
	.name	= "msm_serial_hsl",
	.id	= 0,
	.num_resources	= ARRAY_SIZE(resources_uart_gsbi3),
	.resource	= resources_uart_gsbi3,
};

static struct resource resources_qup_i2c_gsbi1[] = {
	{
		.name	= "gsbi_qup_i2c_addr",
		.start	= MSM_GSBI1_PHYS,
		.end	= MSM_GSBI1_PHYS + 4 - 1,
		.flags	= IORESOURCE_MEM,
	},
	{
		.name	= "qup_phys_addr",
		.start	= MSM_GSBI1_QUP_PHYS,
		.end	= MSM_GSBI1_QUP_PHYS + MSM_QUP_SIZE - 1,
		.flags	= IORESOURCE_MEM,
	},
	{
		.name	= "qup_err_intr",
		.start	= IPQ806X_GSBI1_QUP_IRQ,
		.end	= IPQ806X_GSBI1_QUP_IRQ,
		.flags	= IORESOURCE_IRQ,
	},
	{
		.name	= "i2c_clk",
		.start	= 54,
		.end	= 54,
		.flags	= IORESOURCE_IO,
	},
	{
		.name	= "i2c_sda",
		.start	= 53,
		.end	= 53,
		.flags	= IORESOURCE_IO,
	},
};

static struct resource resources_qup_i2c_gsbi2[] = {
	{
		.name	= "gsbi_qup_i2c_addr",
		.start	= MSM_GSBI2_PHYS,
		.end	= MSM_GSBI2_PHYS + 4 - 1,
		.flags	= IORESOURCE_MEM,
	},
	{
		.name	= "qup_phys_addr",
		.start	= MSM_GSBI2_QUP_PHYS,
		.end	= MSM_GSBI2_QUP_PHYS + MSM_QUP_SIZE - 1,
		.flags	= IORESOURCE_MEM,
	},
	{
		.name	= "qup_err_intr",
		.start	= IPQ806X_GSBI2_QUP_IRQ,
		.end	= IPQ806X_GSBI2_QUP_IRQ,
		.flags	= IORESOURCE_IRQ,
	},
	{
		.name	= "i2c_clk",
		.start	= 25,
		.end	= 25,
		.flags	= IORESOURCE_IO,
	},
	{
		.name	= "i2c_sda",
		.start	= 24,
		.end	= 24,
		.flags	= IORESOURCE_IO,
	},
};

struct platform_device ipq806x_device_qup_i2c_gsbi1 = {
	.name		= "qup_i2c",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(resources_qup_i2c_gsbi1),
	.resource	= resources_qup_i2c_gsbi1,
};

struct platform_device ipq806x_device_qup_i2c_gsbi2 = {
	.name		= "qup_i2c",
	.id		= 1,
	.num_resources	= ARRAY_SIZE(resources_qup_i2c_gsbi2),
	.resource	= resources_qup_i2c_gsbi2,
};

static struct resource resources_qup_i2c_gsbi4[] = {
	{
		.name	= "gsbi_qup_i2c_addr",
		.start	= MSM_GSBI4_PHYS,
		.end	= MSM_GSBI4_PHYS + 4 - 1,
		.flags	= IORESOURCE_MEM,
	},
	{
		.name	= "qup_phys_addr",
		.start	= MSM_GSBI4_QUP_PHYS,
		.end	= MSM_GSBI4_QUP_PHYS + MSM_QUP_SIZE - 1,
		.flags	= IORESOURCE_MEM,
	},
	{
		.name	= "qup_err_intr",
		.start	= GSBI4_QUP_IRQ,
		.end	= GSBI4_QUP_IRQ,
		.flags	= IORESOURCE_IRQ,
	},
	{
		.name	= "i2c_clk",
		.start	= 13,
		.end	= 13,
		.flags	= IORESOURCE_IO,
	},
	{
		.name	= "i2c_sda",
		.start	= 12,
		.end	= 12,
		.flags	= IORESOURCE_IO,
	},
};

struct platform_device ipq806x_device_qup_i2c_gsbi4 = {
	.name		= "qup_i2c",
	.id		= 3,
	.num_resources	= ARRAY_SIZE(resources_qup_i2c_gsbi4),
	.resource	= resources_qup_i2c_gsbi4,
};

/* GSBI2 used into SPI Mode */
static struct resource resources_qup_spi_gsbi2[] = {
	{
		.name   = "spi_base",
		.start  = MSM_GSBI2_QUP_PHYS,
		.end    = MSM_GSBI2_QUP_PHYS + MSM_QUP_SIZE - 1,
		.flags  = IORESOURCE_MEM,
	},
	{
		.name   = "gsbi_base",
		.start  = MSM_GSBI2_PHYS,
		.end    = MSM_GSBI2_PHYS + 4 - 1,
		.flags  = IORESOURCE_MEM,
	},
	{
		.name   = "spi_irq_in",
		.start  = IPQ806X_GSBI2_QUP_IRQ,
		.end    = IPQ806X_GSBI2_QUP_IRQ,
		.flags  = IORESOURCE_IRQ,
	},
};

struct platform_device ipq806x_device_qup_spi_gsbi2 = {
	.name           = "spi_qsd",
	.id             = 2,
	.num_resources  = ARRAY_SIZE(resources_qup_spi_gsbi2),
	.resource       = resources_qup_spi_gsbi2,
};
static struct resource resources_qup_spi_gsbi5[] = {
	{
		.name   = "spi_base",
		.start  = MSM_GSBI5_QUP_PHYS,
		.end    = MSM_GSBI5_QUP_PHYS + SZ_4K - 1,
		.flags  = IORESOURCE_MEM,
	},
	{
		.name   = "gsbi_base",
		.start  = MSM_GSBI5_PHYS,
		.end    = MSM_GSBI5_PHYS + 4 - 1,
		.flags  = IORESOURCE_MEM,
	},
	{
		.name   = "spi_irq_in",
		.start  = GSBI5_QUP_IRQ,
		.end    = GSBI5_QUP_IRQ,
		.flags  = IORESOURCE_IRQ,
	},
	{
		.name   = "spidm_channels",
		.start  = DMOV_SPI_GSBI5_RX_CHAN,
		.end    = DMOV_SPI_GSBI5_TX_CHAN,
		.flags  = IORESOURCE_DMA,
	},
	{
		.name   = "spidm_crci",
		.start  = DMOV_SPI_GSBI5_RX_CRCI,
		.end    = DMOV_SPI_GSBI5_TX_CRCI,
		.flags  = IORESOURCE_DMA,
	},
};

struct platform_device ipq806x_device_qup_spi_gsbi5 = {
	.name		= "spi_qsd",
	.id		= 5,
	.num_resources	= ARRAY_SIZE(resources_qup_spi_gsbi5),
	.resource	= resources_qup_spi_gsbi5,
};

static struct resource resources_qup_i2c_gsbi5[] = {
	{
		.name	= "gsbi_qup_i2c_addr",
		.start	= MSM_GSBI5_PHYS,
		.end	= MSM_GSBI5_PHYS + 4 - 1,
		.flags	= IORESOURCE_MEM,
	},
	{
		.name	= "qup_phys_addr",
		.start	= MSM_GSBI5_QUP_PHYS,
		.end	= MSM_GSBI5_QUP_PHYS + MSM_QUP_SIZE - 1,
		.flags	= IORESOURCE_MEM,
	},
	{
		.name	= "qup_err_intr",
		.start	= GSBI5_QUP_IRQ,
		.end	= GSBI5_QUP_IRQ,
		.flags	= IORESOURCE_IRQ,
	},
	{
		.name	= "i2c_clk",
		.start	= 21,
		.end	= 21,
		.flags	= IORESOURCE_IO,
	},
	{
		.name	= "i2c_sda",
		.start	= 20,
		.end	= 20,
		.flags	= IORESOURCE_IO,
	},
};

struct platform_device ipq806x_device_qup_i2c_gsbi5 = {
	.name		= "qup_i2c",
	.id		= 4,
	.num_resources	= ARRAY_SIZE(resources_qup_i2c_gsbi5),
	.resource	= resources_qup_i2c_gsbi5,
};

/* GSBI 6 used into UARTDM Mode */
static struct resource msm_uart_dm6_resources[] = {
	{
		.start  = MSM_UART6DM_PHYS,
		.end    = MSM_UART6DM_PHYS + PAGE_SIZE - 1,
		.name   = "uartdm_resource",
		.flags  = IORESOURCE_MEM,
	},
	{
		.start  = GSBI6_UARTDM_IRQ,
		.end    = GSBI6_UARTDM_IRQ,
		.flags  = IORESOURCE_IRQ,
	},
	{
		.start  = MSM_GSBI6_PHYS,
		.end    = MSM_GSBI6_PHYS + 4 - 1,
		.name   = "gsbi_resource",
		.flags  = IORESOURCE_MEM,
	},
	{
		.start  = DMOV_IPQ806X_HSUART_GSBI6_TX_CHAN,
		.end    = DMOV_IPQ806X_HSUART_GSBI6_RX_CHAN,
		.name   = "uartdm_channels",
		.flags  = IORESOURCE_DMA,
	},
	{
		.start  = DMOV_IPQ806X_HSUART_GSBI6_TX_CRCI,
		.end    = DMOV_IPQ806X_HSUART_GSBI6_RX_CRCI,
		.name   = "uartdm_crci",
		.flags  = IORESOURCE_DMA,
	},
};
static u64 msm_uart_dm6_dma_mask = DMA_BIT_MASK(32);
struct platform_device ipq806x_device_uartdm_gsbi6 = {
	.name   = "msm_serial_hs",
	.id     = 0,
	.num_resources  = ARRAY_SIZE(msm_uart_dm6_resources),
	.resource       = msm_uart_dm6_resources,
	.dev    = {
		.dma_mask		= &msm_uart_dm6_dma_mask,
		.coherent_dma_mask	= DMA_BIT_MASK(32),
	},
};

/* GSBI6 used into SPI Mode for PCM */
static struct resource resources_qup_spi_pcm_gsbi6[] = {
	{
		.name   = "spi_base",
		.start  = MSM_GSBI6_QUP_PHYS,
		.end    = MSM_GSBI6_QUP_PHYS + MSM_QUP_SIZE - 1,
		.flags  = IORESOURCE_MEM,
	},
	{
		.name   = "gsbi_base",
		.start  = MSM_GSBI6_PHYS,
		.end    = MSM_GSBI6_PHYS + 4 - 1,
		.flags  = IORESOURCE_MEM,
	},
	{
		.name   = "spi_irq_in",
		.start  = GSBI6_QUP_IRQ,
		.end    = GSBI6_QUP_IRQ,
		.flags  = IORESOURCE_IRQ,
	},
};

struct platform_device ipq806x_device_qup_spi_gsbi6 = {
	.name           = "spi_qsd",
	.id             = 6,
	.num_resources  = ARRAY_SIZE(resources_qup_spi_pcm_gsbi6),
	.resource       = resources_qup_spi_pcm_gsbi6,
};

static struct resource resources_uart_gsbi7[] = {
	{
		.start	= GSBI7_UARTDM_IRQ,
		.end	= GSBI7_UARTDM_IRQ,
		.flags	= IORESOURCE_IRQ,
	},
	{
		.start	= MSM_UART7DM_PHYS,
		.end	= MSM_UART7DM_PHYS + PAGE_SIZE - 1,
		.name	= "uartdm_resource",
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= MSM_GSBI7_PHYS,
		.end	= MSM_GSBI7_PHYS + PAGE_SIZE - 1,
		.name	= "gsbi_resource",
		.flags	= IORESOURCE_MEM,
	},
};

struct platform_device ipq806x_device_uart_gsbi7 = {
	.name	= "msm_serial_hsl",
	.id	= 0,
	.num_resources	= ARRAY_SIZE(resources_uart_gsbi7),
	.resource	= resources_uart_gsbi7,
};

static struct resource resources_ssbi_pmic1[] = {
	{
		.start  = MSM_PMIC1_SSBI_CMD_PHYS,
		.end    = MSM_PMIC1_SSBI_CMD_PHYS + MSM_PMIC_SSBI_SIZE - 1,
		.flags  = IORESOURCE_MEM,
	},
};

struct platform_device ipq806x_device_ssbi_pmic1 = {
	.name           = "msm_ssbi",
	.id             = 0,
	.resource       = resources_ssbi_pmic1,
	.num_resources  = ARRAY_SIZE(resources_ssbi_pmic1),
};

static struct resource resources_hsusb[] = {
	{
		.start	= MSM_HSUSB1_PHYS,
		.end	= MSM_HSUSB1_PHYS + MSM_HSUSB1_SIZE - 1,
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= USB1_HS_IRQ,
		.end	= USB1_HS_IRQ,
		.flags	= IORESOURCE_IRQ,
	},
};

struct platform_device ipq806x_device_gadget_peripheral = {
	.name		= "msm_hsusb",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(resources_hsusb),
	.resource	= resources_hsusb,
	.dev		= {
		.coherent_dma_mask	= 0xffffffff,
	},
};

static struct resource resources_hsusb_host[] = {
	{
		.start  = MSM_HSUSB1_PHYS,
		.end    = MSM_HSUSB1_PHYS + MSM_HSUSB1_SIZE - 1,
		.flags  = IORESOURCE_MEM,
	},
	{
		.start  = USB1_HS_IRQ,
		.end    = USB1_HS_IRQ,
		.flags  = IORESOURCE_IRQ,
	},
};

static struct resource resources_hsic_host[] = {
	{
		.start	= 0x12510000,
		.end	= 0x12510000 + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= USB2_HSIC_IRQ,
		.end	= USB2_HSIC_IRQ,
		.flags	= IORESOURCE_IRQ,
	},
	{
		.start	= MSM_GPIO_TO_INT(49),
		.end	= MSM_GPIO_TO_INT(49),
		.name	= "peripheral_status_irq",
		.flags	= IORESOURCE_IRQ,
	},
	{
		.start	= 47,
		.end	= 47,
		.name	= "wakeup",
		.flags	= IORESOURCE_IO,
	},
};

static u64 dma_mask = DMA_BIT_MASK(32);
struct platform_device ipq806x_device_hsusb_host = {
	.name           = "msm_hsusb_host",
	.id             = -1,
	.num_resources  = ARRAY_SIZE(resources_hsusb_host),
	.resource       = resources_hsusb_host,
	.dev            = {
		.dma_mask               = &dma_mask,
		.coherent_dma_mask      = 0xffffffff,
	},
};

struct platform_device ipq806x_device_hsic_host = {
	.name		= "msm_hsic_host",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(resources_hsic_host),
	.resource	= resources_hsic_host,
	.dev		= {
		.dma_mask		= &dma_mask,
		.coherent_dma_mask	= DMA_BIT_MASK(32),
	},
};

static struct resource resources_dwc3_host1[] = {
	{
		.start  = MSM_SSUSB1_PHYS,
		.end    = MSM_SSUSB1_PHYS + MSM_SSUSB1_SIZE - 1,
		.flags  = IORESOURCE_MEM,
	},
	{
		.start  = USB30_EE1_IRQ_1,
		.end    = USB30_EE1_IRQ_1,
		.flags  = IORESOURCE_IRQ,
	},
};

static struct resource resources_dwc3_host2[] = {
	{
		.start  = MSM_SSUSB2_PHYS,
		.end    = MSM_SSUSB2_PHYS + MSM_SSUSB2_SIZE - 1,
		.flags  = IORESOURCE_MEM,
	},
	{
		.start  = USB30_EE1_IRQ,
		.end    = USB30_EE1_IRQ,
		.flags  = IORESOURCE_IRQ,
	},
};

static struct dwc3_platform_data dwc3_pdata_host1 = {
	.usb_mode	= USB30_MODE_HOST,
	.dwc3_core_size = DWC3_CORE_SIZE,
	.pwr_en		= 1,
	.pwr_en_gpio1	= 51,
	.pwr_en_gpio2	= 52,
};

static struct dwc3_platform_data dwc3_pdata_host2 = {
	.usb_mode	= USB30_MODE_HOST,
	.dwc3_core_size = DWC3_CORE_SIZE,
	.pwr_en		= 0,
};

struct platform_device ipq806x_device_dwc3_host1 = {
	.name		= "ipq-dwc3",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(resources_dwc3_host1),
	.resource	= resources_dwc3_host1,
	.dev		= {
		.dma_mask		= &dma_mask,
		.coherent_dma_mask	= 0xffffffff,
		.platform_data		= &dwc3_pdata_host1,
	},
};

struct platform_device ipq806x_device_dwc3_host2 = {
	.name		= "ipq-dwc3",
	.id		= 1,
	.num_resources	= ARRAY_SIZE(resources_dwc3_host2),
	.resource	= resources_dwc3_host2,
	.dev		= {
		.dma_mask		= &dma_mask,
		.coherent_dma_mask	= 0xffffffff,
		.platform_data          = &dwc3_pdata_host2,
	},
};

static struct nss_platform_data nss0_pdata = {
	.num_irq		= 2,
	.irq[0]			= NSS_UBI32_CORE0_IRQ_0,
	.irq[1]			= NSS_UBI32_CORE0_IRQ_1,
	.nmap			= (uint32_t)MSM_UBI32_0_CSM_BASE,
	.vmap			= (uint32_t)MSM_NSS_TCM_BASE,
	.nphys			= IPQ806X_UBI32_0_CSM_PHYS,
	.vphys			= IPQ806X_NSS_TCM_PHYS,
	.rst_addr		= 0x40000000,
	.load_addr		= NSS_CORE0_LOAD_ADDR,
	.turbo_frequency	= NSS_FEATURE_NOT_ENABLED,
	.ipv4_enabled		= NSS_FEATURE_ENABLED,
	.ipv4_reasm_enabled	= NSS_FEATURE_ENABLED,
	.ipv6_enabled		= NSS_FEATURE_ENABLED,
	.ipv6_reasm_enabled	= NSS_FEATURE_ENABLED,
	.l2switch_enabled	= NSS_FEATURE_ENABLED,
	.crypto_enabled		= NSS_FEATURE_NOT_ENABLED,
	.ipsec_enabled		= NSS_FEATURE_NOT_ENABLED,
	.wlanredirect_enabled	= NSS_FEATURE_ENABLED,
	.tun6rd_enabled		= NSS_FEATURE_ENABLED,
	.l2tpv2_enabled		= NSS_FEATURE_ENABLED,
	.tunipip6_enabled	= NSS_FEATURE_ENABLED,
	.gre_redir_enabled	= NSS_FEATURE_ENABLED,
	.shaping_enabled	= NSS_FEATURE_ENABLED,
	.gmac_enabled[0]	= NSS_FEATURE_ENABLED,
	.gmac_enabled[1]	= NSS_FEATURE_ENABLED,
	.gmac_enabled[2]	= NSS_FEATURE_ENABLED,
	.gmac_enabled[3]	= NSS_FEATURE_ENABLED,
	.wifioffload_enabled	= NSS_FEATURE_ENABLED,
};

struct platform_device ipq806x_device_nss0 = {
	.name	= "qca-nss",
	.id	= 0,
	.dev	= {
		.platform_data = &nss0_pdata,
	},
};

static int __init parse_qca_nss_load0(char *p)
{
	nss0_pdata.load_addr = memparse(p, NULL);
	return 0;
}
early_param("qca-nss-drv.load0", parse_qca_nss_load0);

static struct nss_platform_data nss1_pdata = {
	.num_irq		= 2,
	.irq[0]			= NSS_UBI32_CORE1_IRQ_0,
	.irq[1]			= NSS_UBI32_CORE1_IRQ_1,
	.nmap			= (uint32_t)MSM_UBI32_1_CSM_BASE,
	.vmap			= (uint32_t)(MSM_NSS_TCM_BASE + SZ_64K),
	.vphys			= IPQ806X_NSS_TCM_PHYS + SZ_64K,
	.nphys			= IPQ806X_UBI32_1_CSM_PHYS,
	.rst_addr		= 0x40800000,
	.load_addr		= NSS_CORE1_LOAD_ADDR,
	.turbo_frequency	= NSS_FEATURE_NOT_ENABLED,
	.ipv4_enabled		= NSS_FEATURE_NOT_ENABLED,
	.ipv4_reasm_enabled	= NSS_FEATURE_NOT_ENABLED,
	.ipv6_enabled		= NSS_FEATURE_NOT_ENABLED,
	.ipv6_reasm_enabled	= NSS_FEATURE_NOT_ENABLED,
	.l2switch_enabled	= NSS_FEATURE_NOT_ENABLED,
	.crypto_enabled		= NSS_FEATURE_ENABLED,
	.ipsec_enabled		= NSS_FEATURE_ENABLED,
	.wlanredirect_enabled	= NSS_FEATURE_NOT_ENABLED,
	.tun6rd_enabled		= NSS_FEATURE_NOT_ENABLED,
	.l2tpv2_enabled		= NSS_FEATURE_NOT_ENABLED,
	.tunipip6_enabled	= NSS_FEATURE_NOT_ENABLED,
	.gre_redir_enabled	= NSS_FEATURE_NOT_ENABLED,
	.shaping_enabled	= NSS_FEATURE_NOT_ENABLED,
	.gmac_enabled[0]	= NSS_FEATURE_NOT_ENABLED,
	.gmac_enabled[1]	= NSS_FEATURE_NOT_ENABLED,
	.gmac_enabled[2]	= NSS_FEATURE_NOT_ENABLED,
	.gmac_enabled[3]	= NSS_FEATURE_NOT_ENABLED,
	.wifioffload_enabled	= NSS_FEATURE_NOT_ENABLED,
};

struct platform_device ipq806x_device_nss1 = {
	.name	= "qca-nss",
	.id	= 1,
	.dev	= {
		.platform_data = &nss1_pdata,
	},
};

static int __init parse_qca_nss_load1(char *p)
{
	nss1_pdata.load_addr = memparse(p, NULL);
	return 0;
}
early_param("qca-nss-drv.load1", parse_qca_nss_load1);

/* Resources for GMAC0 */
static struct resource nss_gmac_0_res[] = {
	[0] = {
		.start	= NSS_GMAC0_BASE,
		.end	= (NSS_GMAC0_BASE + NSS_GMAC_REG_LEN - 1),
		.name	= "registers",
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= NSS_GMAC0_IRQ0,
		.end	= NSS_GMAC0_IRQ0,
		.name	= "irq",
		.flags	= IORESOURCE_IRQ,
	},
};

static struct msm_nss_gmac_platform_data ipq806x_gmac0_platform_data;
struct platform_device nss_gmac_0 = {
	.name		= "nss-gmac",		/* This should be same as used
						   by driver for all GMACs 	*/
	.id		= 0,			/* Increment this sequentially 	*/
	.dev		= {
		.dma_mask		= (u64 *)~0,
		.coherent_dma_mask	= 0xffffffff,
		.platform_data		= (void *)&ipq806x_gmac0_platform_data,
	},
	.num_resources	= ARRAY_SIZE(nss_gmac_0_res),
	.resource	= nss_gmac_0_res,
};

/* Resources for GMAC1 */
static struct resource nss_gmac_1_res[] = {
	[0] = {
		.start	= NSS_GMAC1_BASE,
		.end	= (NSS_GMAC1_BASE + NSS_GMAC_REG_LEN - 1),
		.name	= "registers",
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= NSS_GMAC1_IRQ0,
		.end	= NSS_GMAC1_IRQ0,
		.name	= "irq",
		.flags	= IORESOURCE_IRQ,
	},
};

static struct msm_nss_gmac_platform_data ipq806x_gmac1_platform_data;
struct platform_device nss_gmac_1 = {
	.name		= "nss-gmac",
	.id		= 1,
	.dev		= {
		.dma_mask		= (u64 *)~0,
		.coherent_dma_mask	= 0xffffffff,
		.platform_data		= (void *)&ipq806x_gmac1_platform_data,
	},
	.num_resources			= ARRAY_SIZE(nss_gmac_1_res),
	.resource			= nss_gmac_1_res,
};

/* Resources for GMAC2 */
static struct resource nss_gmac_2_res[] = {
	[0] = {
		.start	= NSS_GMAC2_BASE,
		.end	= (NSS_GMAC2_BASE + NSS_GMAC_REG_LEN - 1),
		.name	= "registers",
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= NSS_GMAC2_IRQ0,
		.end	= NSS_GMAC2_IRQ0,
		.name	= "irq",
		.flags	= IORESOURCE_IRQ,
	},
};

static struct msm_nss_gmac_platform_data ipq806x_gmac2_platform_data;
struct platform_device nss_gmac_2 = {
	.name		= "nss-gmac",
	.id		= 2,
	.dev		= {
		.dma_mask		= (u64 *)~0,
		.coherent_dma_mask	= 0xffffffff,
		.platform_data		= (void *)&ipq806x_gmac2_platform_data,
	},
	.num_resources			= ARRAY_SIZE(nss_gmac_2_res),
	.resource			= nss_gmac_2_res,
};

/* Resources for GMAC3 */
static struct resource nss_gmac_3_res[] = {
	[0] = {
		.start	= NSS_GMAC3_BASE,
		.end	= (NSS_GMAC3_BASE + NSS_GMAC_REG_LEN - 1),
		.name	= "registers",
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= NSS_GMAC3_IRQ0,
		.end	= NSS_GMAC3_IRQ0,
		.name	= "irq",
		.flags	= IORESOURCE_IRQ,
	},
};

static struct msm_nss_gmac_platform_data ipq806x_gmac3_platform_data;
struct platform_device nss_gmac_3 = {
	.name		= "nss-gmac",
	.id		= 3,
	.dev		= {
		.dma_mask		= (u64 *)~0,
		.coherent_dma_mask	= 0xffffffff,
		.platform_data		= (void *)&ipq806x_gmac3_platform_data,
	},
	.num_resources			= ARRAY_SIZE(nss_gmac_3_res),
	.resource			= nss_gmac_3_res,
};

struct platform_device *nss_gmac[] = {	&nss_gmac_0, &nss_gmac_1,
					&nss_gmac_2, &nss_gmac_3,
					NULL	/* Terminator */ };

/* Board specific data for Aquantia PHY */
struct aq_phy_platform_data ap160_2xx_aq_phy_platform_data = {
        .phy_addr = 0x10,
        .mdio_bus_id = 0,
        .mdio_bus_name = "mdio-gpio",
};

/* Populate the platform device data for Aquantia PHY driver */
struct platform_device ap160_2xx_aq_phy = {
	.name		= "aq-phy",
	.id		= 0,
	.dev		= {
		.platform_data		= (void *)&ap160_2xx_aq_phy_platform_data,
	},
};

/* Resources for MACSEC1 */
static struct resource nss_macsec1_res[] = {
	[0] = {
		.start	= NSS_MACSEC1_BASE,
		.end	= (NSS_MACSEC1_BASE + NSS_MACSEC_REG_LEN-1),
		.name	= "registers",
		.flags	= IORESOURCE_MEM,
	},
};

struct platform_device nss_macsec1 = {
	.name		= "nss-macsec",
	.id		= 0,
	.dev		= {
		.dma_mask		= (u64 *)~0,
		.coherent_dma_mask	= 0xffffffff,
	},
	.num_resources	= ARRAY_SIZE(nss_macsec1_res),
	.resource	= nss_macsec1_res,
};

/* Resources for MACSEC2 */
static struct resource nss_macsec2_res[] = {
	[0] = {
		.start	= NSS_MACSEC2_BASE,
		.end	= (NSS_MACSEC2_BASE + NSS_MACSEC_REG_LEN - 1),
		.name	= "registers",
		.flags	= IORESOURCE_MEM,
	},
};

struct platform_device nss_macsec2 = {
	.name		= "nss-macsec",
	.id		= 1,
	.dev		= {
		.dma_mask		= (u64 *)~0,
		.coherent_dma_mask	= 0xffffffff,
	},
	.num_resources	= ARRAY_SIZE(nss_macsec2_res),
	.resource	= nss_macsec2_res,
};

/* Resources for MACSEC3 */
static struct resource nss_macsec3_res[] = {
	[0] = {
		.start	= NSS_MACSEC3_BASE,
		.end	= (NSS_MACSEC3_BASE + NSS_MACSEC_REG_LEN - 1),
		.name	= "registers",
		.flags	= IORESOURCE_MEM,
	},
};

struct platform_device nss_macsec3 = {
	.name		= "nss-macsec",
	.id		= 2,
	.dev		= {
		.dma_mask		= (u64 *)~0,
		.coherent_dma_mask	= 0xffffffff,
	},
	.num_resources	= ARRAY_SIZE(nss_macsec3_res),
	.resource	= nss_macsec3_res,
};

static struct mdio_gpio_platform_data ipq806x_mdio_platform_data;
struct platform_device ipq806x_mdio_device = {
	.name   = IPQ806X_MDIO_BUS_NAME,
	.id     = IPQ806X_MDIO_BUS_NUM,
	.dev    = {
		.platform_data = &ipq806x_mdio_platform_data,
	},
};

static struct mdio_gpio_platform_data ipq806x_mdio_platform_data2;
struct platform_device ipq806x_mdio_device2 = {
	.name   = IPQ806X_MDIO_BUS_NAME,
	.id     = IPQ806X_MDIO_BUS_NUM + 1,
	.dev    = {
		.platform_data = &ipq806x_mdio_platform_data2,
	},
};

static struct qca_85xx_sw_platform_data ap160_2_8511_sw_data = {
	.mdio_bus_name = IPQ806X_MDIO_BUS_NAME,
	.sgmii_plus_if_mdio_bus_name = IPQ806X_MDIO_BUS_NAME,
	.mdio_bus_id = IPQ806X_MDIO_BUS_NUM + 1,
	.sgmii_plus_if_phy_mdio_bus_id = IPQ806X_MDIO_BUS_NUM,
	.sgmii_plus_if_phy_addr = 0x10,
	.chip_id = QCA_85XX_SW_ID_QCA8511,

	.qsgmii_cfg = {
		.port_mode = QCA_85XX_SW_PORT_MODE_QSGMII,
		.port_base = QCA_85XX_SW_PORT_1,
		.is_speed_forced = true,
		.forced_speed = SPEED_1000,
		.forced_duplex = DUPLEX_FULL,
	},

	.port_24_cfg = {
		.port_mode = QCA_85XX_SW_PORT_MODE_NOT_CONFIGURED,
	},

	.port_25_cfg = {
		.port_mode = QCA_85XX_SW_PORT_MODE_NOT_CONFIGURED,
	},

	.port_26_cfg = {
		.port_mode = QCA_85XX_SW_PORT_MODE_SGMII,
		.is_speed_forced = false,
	},

	.port_27_cfg = {
		.port_mode = QCA_85XX_SW_PORT_MODE_SGMII_PLUS,
		.is_speed_forced = true,
		.forced_speed = SPEED_2500,
		.forced_duplex = DUPLEX_FULL,
	},

	.port_28_cfg = {
		.port_mode = QCA_85XX_SW_PORT_MODE_NOT_CONFIGURED,
	},

	.port_29_cfg = {
		.port_mode = QCA_85XX_SW_PORT_MODE_NOT_CONFIGURED,
	},

	.trunk_cfg = {
		.is_trunk_enabled = true,
		.trunk_id = 0,
		.trunk_hash_policy = (TRUNK_DST_IP_INCL
				     | TRUNK_SRC_IP_INCL
				     | TRUNK_MAC_DA_INCL
				     | TRUNK_MAC_SA_INCL),
		.trunk_ports_bit_map = 0x1c,
	}
};

struct platform_device ap160_2_qca_8511_sw = {
	.name   = "qca_85xx_sw",
	.id     = 0,
	.dev    = {
		.platform_data = (void *)&ap160_2_8511_sw_data,
	},
};

static struct ar8327_pad_cfg ipq806x_db149_ar8337_pad0_cfg = {
	.mode = AR8327_PAD_MAC_RGMII,
	.txclk_delay_en = true,
	.rxclk_delay_en = true,
	.txclk_delay_sel = AR8327_CLK_DELAY_SEL1,
	.rxclk_delay_sel = AR8327_CLK_DELAY_SEL2,
	.rgmii_1_8v = false,
};

static struct ar8327_pad_cfg ipq806x_db147_ar8337_pad0_cfg = {
	.mode = AR8327_PAD_MAC_RGMII,
	.txclk_delay_en = true,
	.rxclk_delay_en = true,
	.txclk_delay_sel = AR8327_CLK_DELAY_SEL1,
	.rxclk_delay_sel = AR8327_CLK_DELAY_SEL2,
	.rgmii_1_8v = true,
};

static struct ar8327_sgmii_cfg ipq806x_db147_ar8337_sgmii_cfg = {
	.sgmii_mode = AR8327_SGMII_PHY,
	.serdes_aen = false,
};

static struct ar8327_sgmii_cfg ipq806x_db149_ar8337_sgmii_cfg = {
	.sgmii_mode = AR8327_SGMII_PHY,
	.serdes_aen = false,
};

static struct ar8327_pad_cfg ipq806x_db149_ar8337_pad6_cfg = {
	.mode = AR8327_PAD_MAC_SGMII,
	.sgmii_txclk_phase_sel = AR8327_SGMII_CLK_PHASE_RISE,
	.sgmii_rxclk_phase_sel = AR8327_SGMII_CLK_PHASE_FALL,
};

static struct ar8327_pad_cfg ipq806x_db147_ar8337_pad6_cfg = {
	.mode = AR8327_PAD_MAC_SGMII,
	.sgmii_txclk_phase_sel = AR8327_SGMII_CLK_PHASE_RISE,
	.sgmii_rxclk_phase_sel = AR8327_SGMII_CLK_PHASE_FALL,
};

static struct ar8327_platform_data ipq806x_db149_ar8337_data = {
	.pad0_cfg = &ipq806x_db149_ar8337_pad0_cfg,
	.pad6_cfg = &ipq806x_db149_ar8337_pad6_cfg,
	.cpuport_cfg = {
		.force_link = 1,
		.speed = AR8327_PORT_SPEED_1000,
		.duplex = 1,
		.txpause = 0,
		.rxpause = 0,
	},
	.port6_cfg = {
		.force_link = 1,
		.speed = AR8327_PORT_SPEED_1000,
		.duplex = 1,
		.txpause = 0,
		.rxpause = 0,
	},
	.sgmii_cfg = &ipq806x_db149_ar8337_sgmii_cfg
};

static struct ar8327_platform_data ipq806x_db147_ar8337_data = {
	.pad0_cfg = &ipq806x_db147_ar8337_pad0_cfg,
	.pad6_cfg = &ipq806x_db147_ar8337_pad6_cfg,
	.cpuport_cfg = {
		.force_link = 1,
		.speed = AR8327_PORT_SPEED_1000,
		.duplex = 1,
		.txpause = 0,
		.rxpause = 0,
	},
	.port6_cfg = {
		.force_link = 1,
		.speed = AR8327_PORT_SPEED_1000,
		.duplex = 1,
		.txpause = 0,
		.rxpause = 0,
	},
	.sgmii_cfg = &ipq806x_db147_ar8337_sgmii_cfg
};

static struct ar8327_pad_cfg ipq806x_ap148_ar8337_pad0_cfg = {
	.mode = AR8327_PAD_MAC_RGMII,
	.txclk_delay_en = true,
	.rxclk_delay_en = true,
	.txclk_delay_sel = AR8327_CLK_DELAY_SEL1,
	.rxclk_delay_sel = AR8327_CLK_DELAY_SEL2,
	.rgmii_1_8v = true,
};

static struct ar8327_pad_cfg ipq806x_ap148_ar8337_pad6_cfg = {
	.mode = AR8327_PAD_MAC_SGMII,
	.sgmii_txclk_phase_sel = AR8327_SGMII_CLK_PHASE_RISE,
	.sgmii_rxclk_phase_sel = AR8327_SGMII_CLK_PHASE_FALL,
};

static struct ar8327_sgmii_cfg ipq806x_ap148_ar8337_sgmii_cfg = {
	.sgmii_mode = AR8327_SGMII_PHY,
	.serdes_aen = false,
};

static struct ar8327_platform_data ipq806x_ap148_ar8337_data = {
	.pad0_cfg = &ipq806x_ap148_ar8337_pad0_cfg,
	.pad6_cfg = &ipq806x_ap148_ar8337_pad6_cfg,
	.cpuport_cfg = {
		.force_link = 1,
		.speed = AR8327_PORT_SPEED_1000,
		.duplex = 1,
		.txpause = 0,
		.rxpause = 0,
	},
	.port6_cfg = {
		.force_link = 1,
		.speed = AR8327_PORT_SPEED_1000,
		.duplex = 1,
		.txpause = 0,
		.rxpause = 0,
	},
	.sgmii_cfg = &ipq806x_ap148_ar8337_sgmii_cfg
};

static struct ar8327_pad_cfg ipq806x_ap160_ar8337_pad0_cfg = {
	.mode = AR8327_PAD_MAC_RGMII,
	.txclk_delay_en = true,
	.rxclk_delay_en = true,
	.txclk_delay_sel = AR8327_CLK_DELAY_SEL1,
	.rxclk_delay_sel = AR8327_CLK_DELAY_SEL2,
	.rgmii_1_8v = true,
};

static struct ar8327_pad_cfg ipq806x_ap160_ar8337_pad5_cfg = {
	.mode = AR8327_PAD_PHY_RGMII,
	.txclk_delay_en = true,
	.rxclk_delay_en = true,
	.txclk_delay_sel = AR8327_CLK_DELAY_SEL1,
	.rxclk_delay_sel = AR8327_CLK_DELAY_SEL2,
	.rgmii_1_8v = false,
};

static struct ar8327_pad_cfg ipq806x_ap160_ar8337_pad6_cfg = {
	.mode = AR8327_PAD_MAC_SGMII,
	.sgmii_txclk_phase_sel = AR8327_SGMII_CLK_PHASE_RISE,
	.sgmii_rxclk_phase_sel = AR8327_SGMII_CLK_PHASE_FALL,
};

static struct ar8327_sgmii_cfg ipq806x_ap160_ar8337_sgmii_cfg = {
	.sgmii_mode = AR8327_SGMII_PHY,
	.serdes_aen = false,
};

static struct ar8327_platform_data ipq806x_ap160_ar8337_data = {
	.pad0_cfg = &ipq806x_ap160_ar8337_pad0_cfg,
	.pad5_cfg = &ipq806x_ap160_ar8337_pad5_cfg,
	.pad6_cfg = &ipq806x_ap160_ar8337_pad6_cfg,
	.cpuport_cfg = {
		.force_link = 1,
		.speed = AR8327_PORT_SPEED_1000,
		.duplex = 1,
		.txpause = 0,
		.rxpause = 0,
	},
	.port5_cfg = {
		.force_link = 1,
		.speed = AR8327_PORT_SPEED_1000,
		.duplex = 1,
		.txpause = 0,
		.rxpause = 0,
	},
	.port6_cfg = {
		.force_link = 1,
		.speed = AR8327_PORT_SPEED_1000,
		.duplex = 1,
		.txpause = 0,
		.rxpause = 0,
	},
	.sgmii_cfg = &ipq806x_ap160_ar8337_sgmii_cfg
};

struct mdio_board_info ipq806x_db149_mdio_info[IPQ806X_MDIO_BUS_MAX] = {
	{
		.bus_id = "gpio-0",
		.phy_addr = 0,
		.platform_data = &ipq806x_db149_ar8337_data,
	}
};

struct mdio_board_info ipq806x_db147_mdio_info[IPQ806X_MDIO_BUS_MAX] = {
	{
		.bus_id = "gpio-0",
		.phy_addr = 0,
		.platform_data = &ipq806x_db147_ar8337_data,
	}
};

struct mdio_board_info ipq806x_ap148_mdio_info[IPQ806X_MDIO_BUS_MAX] = {
	{
		.bus_id = "gpio-0",
		.phy_addr = 0,
		.platform_data = &ipq806x_ap148_ar8337_data,
	}
};

struct mdio_board_info ipq806x_ap160_mdio_info[IPQ806X_MDIO_BUS_MAX] = {
	{
		.bus_id = "gpio-0",
		.phy_addr = 0,
		.platform_data = &ipq806x_ap160_ar8337_data,
	}
};

struct platform_device ipq806x_device_acpuclk = {
	.name		= "acpuclk-ipq806x",
	.id		= -1,
};

#define SHARED_IMEM_TZ_BASE 0x2a03f720
static struct resource tzlog_resources[] = {
	{
		.start = SHARED_IMEM_TZ_BASE,
		.end = SHARED_IMEM_TZ_BASE + SZ_4K - 1,
		.flags = IORESOURCE_MEM,
	},
};

struct platform_device ipq_device_tz_log = {
	.name		= "tz_log",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(tzlog_resources),
	.resource	= tzlog_resources,
};

#define MSM_SDC1_BASE         0x12400000
#define MSM_SDC1_DML_BASE     (MSM_SDC1_BASE + 0x800)
#define MSM_SDC1_BAM_BASE     (MSM_SDC1_BASE + 0x2000)
#define MSM_SDC3_BASE         0x12180000
#define MSM_SDC3_DML_BASE     (MSM_SDC3_BASE + 0x800)
#define MSM_SDC3_BAM_BASE     (MSM_SDC3_BASE + 0x2000)

static struct resource resources_sdc1[] = {
	{
		.name	= "core_mem",
		.flags	= IORESOURCE_MEM,
		.start	= MSM_SDC1_BASE,
		.end	= MSM_SDC1_DML_BASE - 1,
	},
	{
		.name	= "core_irq",
		.flags	= IORESOURCE_IRQ,
		.start	= SDC1_IRQ_0,
		.end	= SDC1_IRQ_0
	},
#ifdef CONFIG_MMC_MSM_SPS_SUPPORT
	{
		.name   = "dml_mem",
		.start	= MSM_SDC1_DML_BASE,
		.end	= MSM_SDC1_BAM_BASE - 1,
		.flags	= IORESOURCE_MEM,
	},
	{
		.name   = "bam_mem",
		.start	= MSM_SDC1_BAM_BASE,
		.end	= MSM_SDC1_BAM_BASE + (2 * SZ_4K) - 1,
		.flags	= IORESOURCE_MEM,
	},
	{
		.name   = "bam_irq",
		.start	= SDC1_BAM_IRQ,
		.end	= SDC1_BAM_IRQ,
		.flags	= IORESOURCE_IRQ,
	},
#endif
};

static struct resource resources_sdc3[] = {
	{
		.name	= "core_mem",
		.flags	= IORESOURCE_MEM,
		.start	= MSM_SDC3_BASE,
		.end	= MSM_SDC3_DML_BASE - 1,
	},
	{
		.name	= "core_irq",
		.flags	= IORESOURCE_IRQ,
		.start	= SDC3_IRQ_0,
		.end	= SDC3_IRQ_0
	},
#ifdef CONFIG_MMC_MSM_SPS_SUPPORT
	{
		.name   = "dml_mem",
		.start	= MSM_SDC3_DML_BASE,
		.end	= MSM_SDC3_BAM_BASE - 1,
		.flags	= IORESOURCE_MEM,
	},
	{
		.name   = "bam_mem",
		.start	= MSM_SDC3_BAM_BASE,
		.end	= MSM_SDC3_BAM_BASE + (2 * SZ_4K) - 1,
		.flags	= IORESOURCE_MEM,
	},
	{
		.name   = "bam_irq",
		.start	= SDC3_BAM_IRQ,
		.end	= SDC3_BAM_IRQ,
		.flags	= IORESOURCE_IRQ,
	},
#endif
};

struct platform_device ipq806x_device_sdc1 = {
	.name		= "msm_sdcc",
	.id		= 1,
	.num_resources	= ARRAY_SIZE(resources_sdc1),
	.resource	= resources_sdc1,
	.dev		= {
		.coherent_dma_mask	= 0xffffffff,
	},
};

struct platform_device ipq806x_device_sdc3 = {
	.name		= "msm_sdcc",
	.id		= 3,
	.num_resources	= ARRAY_SIZE(resources_sdc3),
	.resource	= resources_sdc3,
	.dev		= {
		.coherent_dma_mask	= 0xffffffff,
	},
};

static struct platform_device *ipq806x_sdcc_devices[] __initdata = {
	&ipq806x_device_sdc1,
	&ipq806x_device_sdc3,
};

int __init ipq806x_add_sdcc(unsigned int controller,
				struct mmc_platform_data *plat)
{
	struct platform_device	*pdev;

	if (!plat)
		return 0;
	if (controller < 1 || controller > 2)
		return -EINVAL;

	pdev = ipq806x_sdcc_devices[controller-1];
	pdev->dev.platform_data = plat;
	return platform_device_register(pdev);
}

#define MSM_SATA_AHCI_BASE      0x29000000
#define MSM_SATA_AHCI_REGS_SZ   0x180
#define MSM_SATA_PHY_BASE       0x1B400000
#define MSM_SATA_PHY_REGS_SZ    0x200

static struct resource resources_sata[] = {
	{
		.name   = "ahci_mem",
		.flags  = IORESOURCE_MEM,
		.start  = MSM_SATA_AHCI_BASE,
		.end    = MSM_SATA_AHCI_BASE + MSM_SATA_AHCI_REGS_SZ - 1,
	},
	{
		.name   = "ahci_irq",
		.flags  = IORESOURCE_IRQ,
		.start  = SATA_CONTROLLER_IRQ,
		.end    = SATA_CONTROLLER_IRQ,
	},
	{
		.name   = "phy_mem",
		.flags  = IORESOURCE_MEM,
		.start  = MSM_SATA_PHY_BASE,
		.end    = MSM_SATA_PHY_BASE + MSM_SATA_PHY_REGS_SZ - 1,
	},
};

static u64 sata_dma_mask = DMA_BIT_MASK(32);
struct platform_device ipq806x_device_sata = {
	.name           = "msm_sata",
	.id             = 0,
	.num_resources  = ARRAY_SIZE(resources_sata),
	.resource       = resources_sata,
	.dev            = {
		.dma_mask               = &sata_dma_mask,
		.coherent_dma_mask      = DMA_BIT_MASK(32),
	},
};

static struct resource resources_sps[] = {
	{
		.name	= "pipe_mem",
		.start	= 0x12800000,
		.end	= 0x12800000 + 0x4000 - 1,
		.flags	= IORESOURCE_MEM,
	},
	{
		.name	= "bamdma_dma",
		.start	= 0x12240000,
		.end	= 0x12240000 + 0x1000 - 1,
		.flags	= IORESOURCE_MEM,
	},
	{
		.name	= "bamdma_bam",
		.start	= 0x12244000,
		.end	= 0x12244000 + 0x4000 - 1,
		.flags	= IORESOURCE_MEM,
	},
	{
		.name	= "bamdma_irq",
		.start	= SPS_BAM_DMA_IRQ,
		.end	= SPS_BAM_DMA_IRQ,
		.flags	= IORESOURCE_IRQ,
	},
};

struct platform_device msm_bus_ipq806x_sys_fabric = {
	.name  = "msm_bus_fabric",
	.id    =  MSM_BUS_FAB_SYSTEM,
};
struct platform_device msm_bus_ipq806x_apps_fabric = {
	.name  = "msm_bus_fabric",
	.id    = MSM_BUS_FAB_APPSS,
};
struct platform_device msm_bus_ipq806x_nss_fabric_0 = {
	.name  = "msm_bus_fabric",
	.id    = MSM_BUS_FAB_NSS_0,
};
struct platform_device msm_bus_ipq806x_nss_fabric_1 = {
	.name  = "msm_bus_fabric",
	.id    = MSM_BUS_FAB_NSS_1,
};
struct platform_device msm_bus_ipq806x_sys_fpb = {
	.name  = "msm_bus_fabric",
	.id    = MSM_BUS_FAB_SYSTEM_FPB,
};
struct platform_device msm_bus_ipq806x_cpss_fpb = {
	.name  = "msm_bus_fabric",
	.id    = MSM_BUS_FAB_CPSS_FPB,
};

static struct msm_sps_platform_data msm_sps_pdata = {
	.bamdma_restricted_pipes = 0x06,
};

struct platform_device msm_device_sps_ipq806x = {
	.name		= "msm_sps",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(resources_sps),
	.resource	= resources_sps,
	.dev.platform_data = &msm_sps_pdata,
};

#define MSM_NAND_PHYS		0x1AC00000
static struct resource resources_nand[] = {
	[0] = {
		.name   = "msm_nand_dmac",
		.start	= DMOV_NAND_CHAN,
		.end	= DMOV_NAND_CHAN,
		.flags	= IORESOURCE_DMA,
	},
	[1] = {
		.name   = "msm_nand_phys",
		.start  = MSM_NAND_PHYS,
		.end    = MSM_NAND_PHYS + 0x7FF,
		.flags  = IORESOURCE_MEM,
	},
};

struct flash_platform_data msm_nand_data = {
	.version = VERSION_2,
};

struct platform_device msm_device_nand = {
	.name		= "msm_nand",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(resources_nand),
	.resource	= resources_nand,
	.dev		= {
		.platform_data	= &msm_nand_data,
	},
};

struct platform_device msm_device_bam_dmux = {
	.name		= "BAM_RMNT",
	.id		= -1,
};

static struct resource smd_resource[] = {
	{
		.name   = "a9_m2a_0",
		.start  = INT_A9_M2A_0,
		.flags  = IORESOURCE_IRQ,
	},
	{
		.name   = "a9_m2a_5",
		.start  = INT_A9_M2A_5,
		.flags  = IORESOURCE_IRQ,
	},
	{
		.name   = "adsp_a11",
		.start  = INT_ADSP_A11,
		.flags  = IORESOURCE_IRQ,
	},
	{
		.name   = "adsp_a11_smsm",
		.start  = INT_ADSP_A11_SMSM,
		.flags  = IORESOURCE_IRQ,
	},
	{
		.name   = "dsps_a11",
		.start  = INT_DSPS_A11,
		.flags  = IORESOURCE_IRQ,
	},
	{
		.name   = "dsps_a11_smsm",
		.start  = INT_DSPS_A11_SMSM,
		.flags  = IORESOURCE_IRQ,
	},
};

static struct smd_subsystem_config smd_config_list[] = {
	{
		.irq_config_id = SMD_MODEM,
		.subsys_name = "gss",
		.edge = SMD_APPS_MODEM,

		.smd_int.irq_name = "a9_m2a_0",
		.smd_int.flags = IRQF_TRIGGER_RISING,
		.smd_int.irq_id = -1,
		.smd_int.device_name = "smd_dev",
		.smd_int.dev_id = 0,
		.smd_int.out_bit_pos =  1 << 3,
		.smd_int.out_base = (void __iomem *)MSM_APCS_GCC_BASE,
		.smd_int.out_offset = 0x8,

		.smsm_int.irq_name = "a9_m2a_5",
		.smsm_int.flags = IRQF_TRIGGER_RISING,
		.smsm_int.irq_id = -1,
		.smsm_int.device_name = "smd_smsm",
		.smsm_int.dev_id = 0,
		.smsm_int.out_bit_pos =  1 << 4,
		.smsm_int.out_base = (void __iomem *)MSM_APCS_GCC_BASE,
		.smsm_int.out_offset = 0x8,
	},
	{
		.irq_config_id = SMD_Q6,
		.subsys_name = "q6",
		.edge = SMD_APPS_QDSP,

		.smd_int.irq_name = "adsp_a11",
		.smd_int.flags = IRQF_TRIGGER_RISING,
		.smd_int.irq_id = -1,
		.smd_int.device_name = "smd_dev",
		.smd_int.dev_id = 0,
		.smd_int.out_bit_pos =  1 << 15,
		.smd_int.out_base = (void __iomem *)MSM_APCS_GCC_BASE,
		.smd_int.out_offset = 0x8,

		.smsm_int.irq_name = "adsp_a11_smsm",
		.smsm_int.flags = IRQF_TRIGGER_RISING,
		.smsm_int.irq_id = -1,
		.smsm_int.device_name = "smd_smsm",
		.smsm_int.dev_id = 0,
		.smsm_int.out_bit_pos =  1 << 14,
		.smsm_int.out_base = (void __iomem *)MSM_APCS_GCC_BASE,
		.smsm_int.out_offset = 0x8,
	},
	{
		.irq_config_id = SMD_DSPS,
		.subsys_name = "dsps",
		.edge = SMD_APPS_DSPS,

		.smd_int.irq_name = "dsps_a11",
		.smd_int.flags = IRQF_TRIGGER_RISING,
		.smd_int.irq_id = -1,
		.smd_int.device_name = "smd_dev",
		.smd_int.dev_id = 0,
		.smd_int.out_bit_pos =  1,
		.smd_int.out_base = (void __iomem *)MSM_SIC_NON_SECURE_BASE,
		.smd_int.out_offset = 0x4080,

		.smsm_int.irq_name = "dsps_a11_smsm",
		.smsm_int.flags = IRQF_TRIGGER_RISING,
		.smsm_int.irq_id = -1,
		.smsm_int.device_name = "smd_smsm",
		.smsm_int.dev_id = 0,
		.smsm_int.out_bit_pos =  1,
		.smsm_int.out_base = (void __iomem *)MSM_SIC_NON_SECURE_BASE,
		.smsm_int.out_offset = 0x4094,
	},
	{
		.irq_config_id = SMD_WCNSS,
		.subsys_name = "wcnss",
		.edge = SMD_APPS_WCNSS,

		.smd_int.irq_name = "wcnss_a11",
		.smd_int.flags = IRQF_TRIGGER_RISING,
		.smd_int.irq_id = -1,
		.smd_int.device_name = "smd_dev",
		.smd_int.dev_id = 0,
		.smd_int.out_bit_pos =  1 << 25,
		.smd_int.out_base = (void __iomem *)MSM_APCS_GCC_BASE,
		.smd_int.out_offset = 0x8,

		.smsm_int.irq_name = "wcnss_a11_smsm",
		.smsm_int.flags = IRQF_TRIGGER_RISING,
		.smsm_int.irq_id = -1,
		.smsm_int.device_name = "smd_smsm",
		.smsm_int.dev_id = 0,
		.smsm_int.out_bit_pos =  1 << 23,
		.smsm_int.out_base = (void __iomem *)MSM_APCS_GCC_BASE,
		.smsm_int.out_offset = 0x8,
	},
};

static struct smd_subsystem_restart_config smd_ssr_config = {
	.disable_smsm_reset_handshake = 1,
};

static struct smd_platform smd_platform_data = {
	.num_ss_configs = ARRAY_SIZE(smd_config_list),
	.smd_ss_configs = smd_config_list,
	.smd_ssr_config = &smd_ssr_config,
};

struct platform_device msm_device_smd_ipq806x = {
	.name		= "msm_smd",
	.id		= -1,
	.resource = smd_resource,
	.num_resources = ARRAY_SIZE(smd_resource),
	.dev = {
		.platform_data = &smd_platform_data,
	},
};

#define MSM_PCIE_RES(n, id, type, f)		\
{						\
	.name   = n,				\
	.start  = PCIE20_##id####type##PHYS,	\
	.end    = PCIE20_##id####type##PHYS +	\
			PCIE20##type##SIZE - 1,	\
	.flags  = f,				\
}

static struct resource resources_msm_pcie[][MSM_PCIE_MAX_PLATFORM_RES] = {
	{
		MSM_PCIE_RES("pcie_parf", 0, _PARF_, IORESOURCE_MEM),
		MSM_PCIE_RES("pcie_elbi", 0, _ELBI_, IORESOURCE_MEM),
		MSM_PCIE_RES("pcie20", 0, _, IORESOURCE_MEM),
	},
	{
		MSM_PCIE_RES("pcie_parf", 1, _PARF_, IORESOURCE_MEM),
		MSM_PCIE_RES("pcie_elbi", 1, _ELBI_, IORESOURCE_MEM),
		MSM_PCIE_RES("pcie20", 1, _, IORESOURCE_MEM),
	},
	{
		MSM_PCIE_RES("pcie_parf", 2, _PARF_, IORESOURCE_MEM),
		MSM_PCIE_RES("pcie_elbi", 2, _ELBI_, IORESOURCE_MEM),
		MSM_PCIE_RES("pcie20", 2, _, IORESOURCE_MEM),
	},
};

#define MSM_DEVICE_PCIE(i, n)					\
{								\
	.name           = n,					\
	.id             = i,					\
	.num_resources  = ARRAY_SIZE(resources_msm_pcie[i]),	\
	.resource       = resources_msm_pcie[i],		\
}

struct platform_device msm_device_pcie[] = {
	MSM_DEVICE_PCIE(0, "msm_pcie"),
	MSM_DEVICE_PCIE(1, "msm_pcie"),
	MSM_DEVICE_PCIE(2, "msm_pcie"),
};

int __init ipq806x_add_pcie(unsigned int controller,
			 struct mmc_platform_data *plat)
{
	struct platform_device  *pdev;

	if (!plat)
		return 0;
	if (controller > CONFIG_MSM_NUM_PCIE)
		return -EINVAL;
	pdev = &msm_device_pcie[controller];
	pdev->dev.platform_data = plat;
	return platform_device_register(pdev);
}

#ifdef CONFIG_HW_RANDOM_MSM
/* PRNG device */
#define MSM_PRNG_PHYS           0x1A500000
static struct resource rng_resources = {
	.flags = IORESOURCE_MEM,
	.start = MSM_PRNG_PHYS,
	.end   = MSM_PRNG_PHYS + SZ_512 - 1,
};

struct platform_device ipq806x_device_rng = {
	.name          = "msm_rng",
	.id            = 0,
	.num_resources = 1,
	.resource      = &rng_resources,
};
#endif

struct msm_rpm_platform_data ipq806x_rpm_data __initdata = {
	.reg_base_addrs = {
		[MSM_RPM_PAGE_STATUS] = MSM_RPM_BASE,
		[MSM_RPM_PAGE_CTRL] = MSM_RPM_BASE + 0x400,
		[MSM_RPM_PAGE_REQ] = MSM_RPM_BASE + 0x600,
		[MSM_RPM_PAGE_ACK] = MSM_RPM_BASE + 0xa00,
	},
	.irq_ack = RPM_APCC_CPU0_GP_HIGH_IRQ,
	.irq_err = RPM_APCC_CPU0_GP_LOW_IRQ,
	.irq_wakeup = RPM_APCC_CPU0_WAKE_UP_IRQ,
	.ipc_rpm_reg = MSM_APCS_GCC_BASE + 0x008,
	.ipc_rpm_val = 4,
	.target_id =  {
		MSM_RPM_MAP(IPQ806X, NOTIFICATION_CONFIGURED_0, NOTIFICATION, 4, "notification_configured"),
		MSM_RPM_MAP(IPQ806X, NOTIFICATION_REGISTERED_0, NOTIFICATION, 4, "notification_registered"),
		MSM_RPM_MAP(IPQ806X, INVALIDATE_0, INVALIDATE, 8, "invalidate"),
		MSM_RPM_MAP(IPQ806X, TRIGGER_TIMED_TO, TRIGGER_TIMED, 1, "trigger_timed_to"),
		MSM_RPM_MAP(IPQ806X, TRIGGER_TIMED_SCLK_COUNT, TRIGGER_TIMED, 1, "trigger_timed_sclk_count"),
		MSM_RPM_MAP(IPQ806X, RPM_CTL, RPM_CTL, 1, "rpm_ctl"),
		MSM_RPM_MAP(IPQ806X, CXO_CLK, CXO_CLK, 1, "cxo_clk"),
		MSM_RPM_MAP(IPQ806X, PXO_CLK, PXO_CLK, 1, "pxo_clk"),
		MSM_RPM_MAP(IPQ806X, APPS_FABRIC_CLK, APPS_FABRIC_CLK, 1, "apps_fabric_clk"),
		MSM_RPM_MAP(IPQ806X, SYSTEM_FABRIC_CLK, SYSTEM_FABRIC_CLK, 1, "system_fabric_clk"),
		MSM_RPM_MAP(IPQ806X, DAYTONA_FABRIC_CLK, DAYTONA_FABRIC_CLK, 1, "daytona_fabric_clk"),
		MSM_RPM_MAP(IPQ806X, SFPB_CLK, SFPB_CLK, 1, "sfpb_clk"),
		MSM_RPM_MAP(IPQ806X, CFPB_CLK, CFPB_CLK, 1, "cfpb_clk"),
		MSM_RPM_MAP(IPQ806X, EBI1_CLK, EBI1_CLK, 1, "ebi1_clk"),
		MSM_RPM_MAP(IPQ806X, APPS_FABRIC_CFG_HALT_0,
				APPS_FABRIC_CFG_HALT, 2, "apps_fabric_cfg_halt"),
		MSM_RPM_MAP(IPQ806X, APPS_FABRIC_CFG_CLKMOD_0,
				APPS_FABRIC_CFG_CLKMOD, 3, "apps_fabric_cfg_clkmod" ),
		MSM_RPM_MAP(IPQ806X, APPS_FABRIC_CFG_IOCTL,
				APPS_FABRIC_CFG_IOCTL, 1, "apps_fabric_cfg_ioctl" ),
		MSM_RPM_MAP(IPQ806X, APPS_FABRIC_ARB_0, APPS_FABRIC_ARB, 12, "apps_fabric_arb"),
		MSM_RPM_MAP(IPQ806X, SYS_FABRIC_CFG_HALT_0,
				SYS_FABRIC_CFG_HALT, 2, "sys_fabric_cfg_halt" ),
		MSM_RPM_MAP(IPQ806X, SYS_FABRIC_CFG_CLKMOD_0,
				SYS_FABRIC_CFG_CLKMOD, 3, "sys_fabric_cfg_clkmod" ),
		MSM_RPM_MAP(IPQ806X, SYS_FABRIC_CFG_IOCTL,
				SYS_FABRIC_CFG_IOCTL, 1, "sys_fabric_cfg_ioctl" ),
		MSM_RPM_MAP(IPQ806X, SYSTEM_FABRIC_ARB_0, SYSTEM_FABRIC_ARB, 30, "system_fabric_arb"),
		MSM_RPM_MAP(IPQ806X, MMSS_FABRIC_CFG_HALT_0,
				MMSS_FABRIC_CFG_HALT, 2, "mmss_fabric_cfg_halt" ),
		MSM_RPM_MAP(IPQ806X, MMSS_FABRIC_CFG_CLKMOD_0,
				MMSS_FABRIC_CFG_CLKMOD, 3, "mmss_fabric_cfg_clkmod" ),
		MSM_RPM_MAP(IPQ806X, MMSS_FABRIC_CFG_IOCTL,
				MMSS_FABRIC_CFG_IOCTL, 1, "mmss_fabric_cfg_ioctl" ),
		MSM_RPM_MAP(IPQ806X, MM_FABRIC_ARB_0, MM_FABRIC_ARB, 2, "mm_fabric_arb"),

		MSM_RPM_MAP(IPQ806X, NCP_0, NCP, 2, "ncp"),
		MSM_RPM_MAP(IPQ806X, CXO_BUFFERS, CXO_BUFFERS, 1, "cxo_buffers"),
		MSM_RPM_MAP(IPQ806X, USB_OTG_SWITCH, USB_OTG_SWITCH, 1, "usb_otg_switch"),
		MSM_RPM_MAP(IPQ806X, HDMI_SWITCH, HDMI_SWITCH, 1, "hdmi_switch"),
		MSM_RPM_MAP(IPQ806X, DDR_DMM_0, DDR_DMM, 2, "ddr_dmm"),
		MSM_RPM_MAP(IPQ806X, QDSS_CLK, QDSS_CLK, 1, "qdss_clk"),
		MSM_RPM_MAP(IPQ806X, VDDMIN_GPIO, VDDMIN_GPIO, 1, "vddmin_gpio"),

		MSM_RPM_MAP(IPQ806X, SMB208_S1a_0, SMB208_S1a, 2, "smb208_s1a"),
		MSM_RPM_MAP(IPQ806X, SMB208_S1b_0, SMB208_S1b, 2, "smb208_s1b"),
		MSM_RPM_MAP(IPQ806X, SMB208_S2a_0, SMB208_S2a, 2, "smb208_s2a"),
		MSM_RPM_MAP(IPQ806X, SMB208_S2b_0, SMB208_S2b, 2, "smb208_s2b"),

		MSM_RPM_MAP(IPQ806X, DDR_SELF_REFRESH, DDR_SELF_REFRESH, 1, "ddr_self_refresh"),
		MSM_RPM_MAP(IPQ806X, ENTER_IDLE, ENTER_IDLE, 1, "enter_idle"),
		MSM_RPM_MAP(IPQ806X, NSS_FABRIC_0_CLK, NSS_FABRIC_0_CLK, 1, "nss_fabric_0_clk" ),
		MSM_RPM_MAP(IPQ806X, NSS_FABRIC_1_CLK, NSS_FABRIC_1_CLK, 1, "nss_fabric_1_clk" ),
	},
	.target_status = {
		MSM_RPM_STATUS_ID_MAP(IPQ806X, VERSION_MAJOR),
		MSM_RPM_STATUS_ID_MAP(IPQ806X, VERSION_MINOR),
		MSM_RPM_STATUS_ID_MAP(IPQ806X, VERSION_BUILD),
		MSM_RPM_STATUS_ID_MAP(IPQ806X, SUPPORTED_RESOURCES_0),
		MSM_RPM_STATUS_ID_MAP(IPQ806X, SUPPORTED_RESOURCES_1),
		MSM_RPM_STATUS_ID_MAP(IPQ806X, SUPPORTED_RESOURCES_2),
		MSM_RPM_STATUS_ID_MAP(IPQ806X, RESERVED_SUPPORTED_RESOURCES_0),
		MSM_RPM_STATUS_ID_MAP(IPQ806X, SEQUENCE),
		MSM_RPM_STATUS_ID_MAP(IPQ806X, RPM_CTL),
		MSM_RPM_STATUS_ID_MAP(IPQ806X, CXO_CLK),
		MSM_RPM_STATUS_ID_MAP(IPQ806X, PXO_CLK),
		MSM_RPM_STATUS_ID_MAP(IPQ806X, APPS_FABRIC_CLK),
		MSM_RPM_STATUS_ID_MAP(IPQ806X, SYSTEM_FABRIC_CLK),
		MSM_RPM_STATUS_ID_MAP(IPQ806X, DAYTONA_FABRIC_CLK),
		MSM_RPM_STATUS_ID_MAP(IPQ806X, SFPB_CLK),
		MSM_RPM_STATUS_ID_MAP(IPQ806X, CFPB_CLK),
		MSM_RPM_STATUS_ID_MAP(IPQ806X, EBI1_CLK),
		MSM_RPM_STATUS_ID_MAP(IPQ806X, APPS_FABRIC_CFG_HALT),
		MSM_RPM_STATUS_ID_MAP(IPQ806X, APPS_FABRIC_CFG_CLKMOD),
		MSM_RPM_STATUS_ID_MAP(IPQ806X, APPS_FABRIC_CFG_IOCTL),
		MSM_RPM_STATUS_ID_MAP(IPQ806X, APPS_FABRIC_ARB),
		MSM_RPM_STATUS_ID_MAP(IPQ806X, SYS_FABRIC_CFG_HALT),
		MSM_RPM_STATUS_ID_MAP(IPQ806X, SYS_FABRIC_CFG_CLKMOD),
		MSM_RPM_STATUS_ID_MAP(IPQ806X, SYS_FABRIC_CFG_IOCTL),
		MSM_RPM_STATUS_ID_MAP(IPQ806X, SYSTEM_FABRIC_ARB),
		MSM_RPM_STATUS_ID_MAP(IPQ806X, MMSS_FABRIC_CFG_HALT),
		MSM_RPM_STATUS_ID_MAP(IPQ806X, MMSS_FABRIC_CFG_CLKMOD),
		MSM_RPM_STATUS_ID_MAP(IPQ806X, MMSS_FABRIC_CFG_IOCTL),
		MSM_RPM_STATUS_ID_MAP(IPQ806X, MM_FABRIC_ARB),

		MSM_RPM_STATUS_ID_MAP(IPQ806X, NCP_0),
		MSM_RPM_STATUS_ID_MAP(IPQ806X, NCP_1),
		MSM_RPM_STATUS_ID_MAP(IPQ806X, CXO_BUFFERS),
		MSM_RPM_STATUS_ID_MAP(IPQ806X, USB_OTG_SWITCH),
		MSM_RPM_STATUS_ID_MAP(IPQ806X, HDMI_SWITCH),
		MSM_RPM_STATUS_ID_MAP(IPQ806X, DDR_DMM_0),
		MSM_RPM_STATUS_ID_MAP(IPQ806X, DDR_DMM_1),
		MSM_RPM_STATUS_ID_MAP(IPQ806X, EBI1_CH0_RANGE),
		MSM_RPM_STATUS_ID_MAP(IPQ806X, EBI1_CH1_RANGE),
		MSM_RPM_STATUS_ID_MAP(IPQ806X, VDDMIN_GPIO),

		MSM_RPM_STATUS_ID_MAP(IPQ806X, SMB208_S1a_0),
		MSM_RPM_STATUS_ID_MAP(IPQ806X, SMB208_S1a_1),
		MSM_RPM_STATUS_ID_MAP(IPQ806X, SMB208_S1b_0),
		MSM_RPM_STATUS_ID_MAP(IPQ806X, SMB208_S1b_1),
		MSM_RPM_STATUS_ID_MAP(IPQ806X, SMB208_S2a_0),
		MSM_RPM_STATUS_ID_MAP(IPQ806X, SMB208_S2a_1),
		MSM_RPM_STATUS_ID_MAP(IPQ806X, SMB208_S2b_0),
		MSM_RPM_STATUS_ID_MAP(IPQ806X, SMB208_S2b_1),

		MSM_RPM_STATUS_ID_MAP(IPQ806X, DDR_SELF_REFRESH),
		MSM_RPM_STATUS_ID_MAP(IPQ806X, ENTER_IDLE),
		MSM_RPM_STATUS_ID_MAP(IPQ806X, NSS_FABRIC_0_CLK),
		MSM_RPM_STATUS_ID_MAP(IPQ806X, NSS_FABRIC_1_CLK),
		MSM_RPM_STATUS_ID_MAP(IPQ806X, QDSS_CLK),
	},
	.target_ctrl_id = {
		MSM_RPM_CTRL_MAP(IPQ806X, VERSION_MAJOR),
		MSM_RPM_CTRL_MAP(IPQ806X, VERSION_MINOR),
		MSM_RPM_CTRL_MAP(IPQ806X, VERSION_BUILD),
		MSM_RPM_CTRL_MAP(IPQ806X, REQ_CTX_0),
		MSM_RPM_CTRL_MAP(IPQ806X, REQ_SEL_0),
		MSM_RPM_CTRL_MAP(IPQ806X, ACK_CTX_0),
		MSM_RPM_CTRL_MAP(IPQ806X, ACK_SEL_0),
	},
	.sel_invalidate = MSM_RPM_IPQ806X_SEL_INVALIDATE,
	.sel_notification = MSM_RPM_IPQ806X_SEL_NOTIFICATION,
	.sel_last = MSM_RPM_IPQ806X_SEL_LAST,
	.ver = {3, 0, 0},
};

struct platform_device ipq806x_rpm_device = {
	.name   = "msm_rpm",
	.id     = -1,
};

static struct msm_rpmstats_platform_data msm_rpm_stat_pdata = {
	.phys_addr_base = 0x0010DD04,
	.phys_size = SZ_256,
};

struct platform_device ipq806x_rpm_stat_device = {
	.name = "msm_rpm_stat",
	.id = -1,
	.dev = {
		.platform_data = &msm_rpm_stat_pdata,
	},
};

static struct resource resources_rpm_master_stats[] = {
	{
		.start	= IPQ806X_RPM_MASTER_STATS_BASE,
		.end	= IPQ806X_RPM_MASTER_STATS_BASE + SZ_256,
		.flags	= IORESOURCE_MEM,
	},
};

static char *master_names[] = {
	"KPSS",
	"MPSS",
	"LPASS",
	"RIVA",
	"DSPS",
};

static struct msm_rpm_master_stats_platform_data msm_rpm_master_stat_pdata = {
	.masters = master_names,
	.nomasters = ARRAY_SIZE(master_names),
};

struct platform_device ipq806x_rpm_master_stat_device = {
	.name = "msm_rpm_master_stat",
	.id = -1,
	.num_resources	= ARRAY_SIZE(resources_rpm_master_stats),
	.resource	= resources_rpm_master_stats,
	.dev = {
		.platform_data = &msm_rpm_master_stat_pdata,
	},
};

#define RPM_LOG_BUF_LEN		(1536 * sizeof(uint32_t)) /* in bytes */

static struct msm_rpm_log_platform_data msm_rpm_log_pdata = {
	.phys_addr_base = 0x0010C0C8,
	.reg_offsets = {
		[MSM_RPM_LOG_PAGE_INDICES] = 0x00000080,
		[MSM_RPM_LOG_PAGE_BUFFER]  = 0x000000A0,
	},
	.phys_size = SZ_8K,
	.log_len = RPM_LOG_BUF_LEN,
};

struct platform_device ipq806x_rpm_log_device = {
	.name	= "msm_rpm_log",
	.id	= -1,
	.dev	= {
		.platform_data = &msm_rpm_log_pdata,
	},
};

#ifdef CONFIG_MSM_MPM
static uint16_t msm_mpm_irqs_m2a[MSM_MPM_NR_MPM_IRQS] __initdata = {
	[1] = MSM_GPIO_TO_INT(26),
	[2] = MSM_GPIO_TO_INT(88),
	[4] = MSM_GPIO_TO_INT(73),
	[5] = MSM_GPIO_TO_INT(74),
	[6] = MSM_GPIO_TO_INT(75),
	[7] = MSM_GPIO_TO_INT(76),
	[8] = MSM_GPIO_TO_INT(77),
	[9] = MSM_GPIO_TO_INT(36),
	[10] = MSM_GPIO_TO_INT(84),
	[11] = MSM_GPIO_TO_INT(7),
	[12] = MSM_GPIO_TO_INT(11),
	[13] = MSM_GPIO_TO_INT(52),
	[14] = MSM_GPIO_TO_INT(15),
	[15] = MSM_GPIO_TO_INT(83),
	[16] = USB3_HS_IRQ,
	[19] = MSM_GPIO_TO_INT(61),
	[20] = MSM_GPIO_TO_INT(58),
	[23] = MSM_GPIO_TO_INT(65),
	[24] = MSM_GPIO_TO_INT(63),
	[25] = USB1_HS_IRQ,
	[29] = MSM_GPIO_TO_INT(22),
	[30] = MSM_GPIO_TO_INT(72),
	[31] = USB4_HS_IRQ,
	[33] = MSM_GPIO_TO_INT(44),
	[34] = MSM_GPIO_TO_INT(39),
	[35] = MSM_GPIO_TO_INT(19),
	[36] = MSM_GPIO_TO_INT(23),
	[37] = MSM_GPIO_TO_INT(41),
	[38] = MSM_GPIO_TO_INT(30),
	[41] = MSM_GPIO_TO_INT(42),
	[42] = MSM_GPIO_TO_INT(56),
	[43] = MSM_GPIO_TO_INT(55),
	[44] = MSM_GPIO_TO_INT(50),
	[45] = MSM_GPIO_TO_INT(49),
	[46] = MSM_GPIO_TO_INT(47),
	[47] = MSM_GPIO_TO_INT(45),
	[48] = MSM_GPIO_TO_INT(38),
	[49] = MSM_GPIO_TO_INT(34),
	[50] = MSM_GPIO_TO_INT(32),
	[51] = MSM_GPIO_TO_INT(29),
	[52] = MSM_GPIO_TO_INT(18),
	[53] = MSM_GPIO_TO_INT(10),
	[54] = MSM_GPIO_TO_INT(81),
	[55] = MSM_GPIO_TO_INT(6),
};

static uint16_t msm_mpm_bypassed_apps_irqs[] __initdata = {
	TLMM_MSM_SUMMARY_IRQ,
	RPM_APCC_CPU0_GP_HIGH_IRQ,
	RPM_APCC_CPU0_GP_MEDIUM_IRQ,
	RPM_APCC_CPU0_GP_LOW_IRQ,
	RPM_APCC_CPU0_WAKE_UP_IRQ,
	RPM_APCC_CPU1_GP_HIGH_IRQ,
	RPM_APCC_CPU1_GP_MEDIUM_IRQ,
	RPM_APCC_CPU1_GP_LOW_IRQ,
	RPM_APCC_CPU1_WAKE_UP_IRQ,
	LPASS_DML_IRQ,
	LPASS_SPDIFTX_IRQ,
	LPASS_SCSS_GP_HIGH_IRQ,
	SPS_MTI_30,
	SPS_MTI_31,
	PM8821_SEC_IRQ_N,
};

struct msm_mpm_device_data ipq806x_mpm_dev_data __initdata = {
	.irqs_m2a = msm_mpm_irqs_m2a,
	.irqs_m2a_size = ARRAY_SIZE(msm_mpm_irqs_m2a),
	.bypassed_apps_irqs = msm_mpm_bypassed_apps_irqs,
	.bypassed_apps_irqs_size = ARRAY_SIZE(msm_mpm_bypassed_apps_irqs),
	.mpm_request_reg_base = MSM_RPM_BASE + 0x9d8,
	.mpm_status_reg_base = MSM_RPM_BASE + 0xdf8,
	.mpm_apps_ipc_reg = MSM_APCS_GCC_BASE + 0x008,
	.mpm_apps_ipc_val =  BIT(1),
	.mpm_ipc_irq = RPM_APCC_CPU0_GP_MEDIUM_IRQ,

};
#endif

/* AP2MDM_SOFT_RESET is implemented by the PON_RESET_N gpio */
#define MDM2AP_ERRFATAL			19
#define AP2MDM_ERRFATAL			18
#define MDM2AP_STATUS			49
#define AP2MDM_STATUS			48
#define AP2MDM_SOFT_RESET		27
#define I2S_AP2MDM_SOFT_RESET		0
#define AP2MDM_WAKEUP			35
#define I2S_AP2MDM_WAKEUP		44
#define MDM2AP_PBLRDY			46
#define I2S_MDM2AP_PBLRDY		81

static struct resource mdm_resources[] = {
	{
		.start	= MDM2AP_ERRFATAL,
		.end	= MDM2AP_ERRFATAL,
		.name	= "MDM2AP_ERRFATAL",
		.flags	= IORESOURCE_IO,
	},
	{
		.start	= AP2MDM_ERRFATAL,
		.end	= AP2MDM_ERRFATAL,
		.name	= "AP2MDM_ERRFATAL",
		.flags	= IORESOURCE_IO,
	},
	{
		.start	= MDM2AP_STATUS,
		.end	= MDM2AP_STATUS,
		.name	= "MDM2AP_STATUS",
		.flags	= IORESOURCE_IO,
	},
	{
		.start	= AP2MDM_STATUS,
		.end	= AP2MDM_STATUS,
		.name	= "AP2MDM_STATUS",
		.flags	= IORESOURCE_IO,
	},
	{
		.start	= AP2MDM_SOFT_RESET,
		.end	= AP2MDM_SOFT_RESET,
		.name	= "AP2MDM_SOFT_RESET",
		.flags	= IORESOURCE_IO,
	},
	{
		.start	= AP2MDM_WAKEUP,
		.end	= AP2MDM_WAKEUP,
		.name	= "AP2MDM_WAKEUP",
		.flags	= IORESOURCE_IO,
	},
	{
		.start	= MDM2AP_PBLRDY,
		.end	= MDM2AP_PBLRDY,
		.name	= "MDM2AP_PBLRDY",
		.flags	= IORESOURCE_IO,
	},
};

static struct resource i2s_mdm_resources[] = {
	{
		.start	= MDM2AP_ERRFATAL,
		.end	= MDM2AP_ERRFATAL,
		.name	= "MDM2AP_ERRFATAL",
		.flags	= IORESOURCE_IO,
	},
	{
		.start	= AP2MDM_ERRFATAL,
		.end	= AP2MDM_ERRFATAL,
		.name	= "AP2MDM_ERRFATAL",
		.flags	= IORESOURCE_IO,
	},
	{
		.start	= MDM2AP_STATUS,
		.end	= MDM2AP_STATUS,
		.name	= "MDM2AP_STATUS",
		.flags	= IORESOURCE_IO,
	},
	{
		.start	= AP2MDM_STATUS,
		.end	= AP2MDM_STATUS,
		.name	= "AP2MDM_STATUS",
		.flags	= IORESOURCE_IO,
	},
	{
		.start	= I2S_AP2MDM_SOFT_RESET,
		.end	= I2S_AP2MDM_SOFT_RESET,
		.name	= "AP2MDM_SOFT_RESET",
		.flags	= IORESOURCE_IO,
	},
	{
		.start	= I2S_AP2MDM_WAKEUP,
		.end	= I2S_AP2MDM_WAKEUP,
		.name	= "AP2MDM_WAKEUP",
		.flags	= IORESOURCE_IO,
	},
	{
		.start	= I2S_MDM2AP_PBLRDY,
		.end	= I2S_MDM2AP_PBLRDY,
		.name	= "MDM2AP_PBLRDY",
		.flags	= IORESOURCE_IO,
	},
};

struct platform_device mdm_ipq806x_device = {
	.name		= "mdm2_modem",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(mdm_resources),
	.resource	= mdm_resources,
};

struct platform_device i2s_mdm_ipq806x_device = {
	.name		= "mdm2_modem",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(i2s_mdm_resources),
	.resource	= i2s_mdm_resources,
};

static struct msm_dcvs_core_info ipq806x_core_info = {
	.num_cores		= 4,
	.sensors		= (int[]){7, 8, 9, 10},
	.thermal_poll_ms	= 60000,
	.core_param		= {
		.core_type	= MSM_DCVS_CORE_TYPE_CPU,
	},
	.algo_param		= {
		.disable_pc_threshold		= 1458000,
		.em_win_size_min_us		= 100000,
		.em_win_size_max_us		= 300000,
		.em_max_util_pct		= 97,
		.group_id			= 1,
		.max_freq_chg_time_us		= 100000,
		.slack_mode_dynamic		= 0,
		.slack_weight_thresh_pct	= 3,
		.slack_time_min_us		= 45000,
		.slack_time_max_us		= 45000,
		.ss_no_corr_below_freq		= 0,
		.ss_win_size_min_us		= 1000000,
		.ss_win_size_max_us		= 1000000,
		.ss_util_pct			= 95,
	},
	.energy_coeffs		= {
		.active_coeff_a		= 336,
		.active_coeff_b		= 0,
		.active_coeff_c		= 0,

		.leakage_coeff_a	= -17720,
		.leakage_coeff_b	= 37,
		.leakage_coeff_c	= 3329,
		.leakage_coeff_d	= -277,
	},
	.power_param		= {
		.current_temp	= 25,
		.num_freq	= 0,
	}
};

#define IPQ806X_LPM_LATENCY  1000 /* >100 usec for WFI */

static struct msm_gov_platform_data gov_platform_data = {
	.info = &ipq806x_core_info,
	.latency = IPQ806X_LPM_LATENCY,
};

struct platform_device ipq806x_msm_gov_device = {
	.name = "msm_dcvs_gov",
	.id = -1,
	.dev = {
		.platform_data = &gov_platform_data,
	},
};

static struct msm_mpd_algo_param ipq806x_mpd_algo_param = {
	.em_win_size_min_us	= 10000,
	.em_win_size_max_us	= 100000,
	.em_max_util_pct	= 90,
	.online_util_pct_min	= 60,
	.slack_time_min_us	= 50000,
	.slack_time_max_us	= 100000,
};

struct platform_device ipq806x_msm_mpd_device = {
	.name	= "msm_mpdecision",
	.id	= -1,
	.dev	= {
		.platform_data = &ipq806x_mpd_algo_param,
	},
};

static struct resource msm_cache_erp_resources[] = {
	{
		.name = "l1_irq",
		.start = SC_SICCPUXEXTFAULTIRPTREQ,
		.flags = IORESOURCE_IRQ,
	},
	{
		.name = "l2_irq",
		.start = APCC_QGICL2IRPTREQ,
		.flags = IORESOURCE_IRQ,
	}
};

#ifdef CONFIG_DEBUG_IPQ806X_CACHE_ERP_REGS

#define ipq_err_reg(_base, _reg, _off)		\
{						\
	.name = #_base ": " #_reg,		\
	.addr = ((MSM_ ##_base##_BASE) + _off)	\
}

struct ipq_reg ipq806x_cache_erp_regs[] = {
	ipq_err_reg(SFAB, ID_REVISION_REG0,		0x0000),
	ipq_err_reg(SFAB, ID_REVISION_REG1,		0x0004),
	ipq_err_reg(SFAB, CONFIGURATION_REG,		0x0008),
	ipq_err_reg(SFAB, ERROR_STATUS_REG_0,		0x3504),
	ipq_err_reg(SFAB, ERROR_STATUS_REG_1,		0x3508),
	ipq_err_reg(SFAB, ERROR_STATUS_REG_2,		0x350C),
	ipq_err_reg(SFAB, ERROR_STATUS_REG_3,		0x3510),
	ipq_err_reg(SFAB, ERROR_UPPER_ADDR_REG,		0x3514),
	ipq_err_reg(SFAB, ERROR_LOWER_ADDR_REG,		0x3518),

	ipq_err_reg(AFAB, ID_REVISION_REG0,		0x0000),
	ipq_err_reg(AFAB, ID_REVISION_REG1,		0x0004),
	ipq_err_reg(AFAB, CONFIGURATION_REG,		0x0008),
	ipq_err_reg(AFAB, ERROR_STATUS_REG_0,		0x3504),
	ipq_err_reg(AFAB, ERROR_STATUS_REG_1,		0x3508),
	ipq_err_reg(AFAB, ERROR_STATUS_REG_2,		0x350C),
	ipq_err_reg(AFAB, ERROR_STATUS_REG_3,		0x3510),
	ipq_err_reg(AFAB, ERROR_UPPER_ADDR_REG,		0x3514),
	ipq_err_reg(AFAB, ERROR_LOWER_ADDR_REG,		0x3518),

	ipq_err_reg(NSSFAB_0, ID_REVISION_REG0,		0x0000),
	ipq_err_reg(NSSFAB_0, ID_REVISION_REG1,		0x0004),
	ipq_err_reg(NSSFAB_0, CONFIGURATION_REG,	0x0008),
	ipq_err_reg(NSSFAB_0, ERROR_STATUS_REG_0,	0x3504),
	ipq_err_reg(NSSFAB_0, ERROR_STATUS_REG_1,	0x3508),
	ipq_err_reg(NSSFAB_0, ERROR_STATUS_REG_2,	0x350C),
	ipq_err_reg(NSSFAB_0, ERROR_STATUS_REG_3,	0x3510),
	ipq_err_reg(NSSFAB_0, ERROR_UPPER_ADDR_REG,	0x3514),
	ipq_err_reg(NSSFAB_0, ERROR_LOWER_ADDR_REG,	0x3518),

	ipq_err_reg(NSSFAB_1, ID_REVISION_REG0,		0x0000),
	ipq_err_reg(NSSFAB_1, ID_REVISION_REG1,		0x0004),
	ipq_err_reg(NSSFAB_1, CONFIGURATION_REG,	0x0008),
	ipq_err_reg(NSSFAB_1, ERROR_STATUS_REG_0,	0x3504),
	ipq_err_reg(NSSFAB_1, ERROR_STATUS_REG_1,	0x3508),
	ipq_err_reg(NSSFAB_1, ERROR_STATUS_REG_2,	0x350C),
	ipq_err_reg(NSSFAB_1, ERROR_STATUS_REG_3,	0x3510),
	ipq_err_reg(NSSFAB_1, ERROR_UPPER_ADDR_REG,	0x3514),
	ipq_err_reg(NSSFAB_1, ERROR_LOWER_ADDR_REG,	0x3518),

	ipq_err_reg(DAY_CFG, ID_REVISION_REG0,		0x0000),
	ipq_err_reg(DAY_CFG, ID_REVISION_REG1,		0x0004),
	ipq_err_reg(DAY_CFG, CONFIGURATION_REG,		0x0008),
	ipq_err_reg(DAY_CFG, ERROR_STATUS_REG_0,	0x3504),
	ipq_err_reg(DAY_CFG, ERROR_STATUS_REG_1,	0x3508),
	ipq_err_reg(DAY_CFG, ERROR_STATUS_REG_2,	0x350C),
	ipq_err_reg(DAY_CFG, ERROR_STATUS_REG_3,	0x3510),
	ipq_err_reg(DAY_CFG, ERROR_UPPER_ADDR_REG,	0x3514),
	ipq_err_reg(DAY_CFG, ERROR_LOWER_ADDR_REG,	0x3518),
};

struct ipq_error_status_reg ipq806x_cache_erp_pdata = {
	.reg = ipq806x_cache_erp_regs,
	.num = ARRAY_SIZE(ipq806x_cache_erp_regs),
};
#endif

struct platform_device ipq806x_device_cache_erp = {
	.name		= "msm_cache_erp",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(msm_cache_erp_resources),
	.resource	= msm_cache_erp_resources,
#ifdef CONFIG_DEBUG_IPQ806X_CACHE_ERP_REGS
	.dev		= {
		.platform_data = &ipq806x_cache_erp_pdata,
	},
#endif
};

struct resource ipq_dmlite_resource[] = {
	{
		.name   = "ipq-dmlite",
		.start  = IPQ_DML_START,
		.end    = IPQ_DML_END,
		.flags  = IORESOURCE_MEM,
	},
	{
		.name	= "ipq-dmlite-irq",
		.start	= LPASS_DML_IRQ,
		.end	= LPASS_DML_IRQ,
		.flags	= IORESOURCE_IRQ,
	},
};

struct resource ipq_spdif_resources[] = {
	{
		.start = IPQ_LPAIF_SPDIF_PHYS,
		.end   = IPQ_LPAIF_SPDIF_END,
		.flags = IORESOURCE_MEM,
		.name  = "ipq-spdif",
	},
	{
		.name	= "ipq-spdiftx-irq",
		.start	= LPASS_SPDIFTX_IRQ,
		.end	= LPASS_SPDIFTX_IRQ,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct resource ipq_lpaif_resources[] = {
        {
		.start = IPQ_LPAIF_PHYS,
		.end   = IPQ_LPAIF_END,
		.flags = IORESOURCE_MEM,
		.name  = "ipq-dai",
        },
	{
		.name	= "ipq-dai-irq",
		.start	= LPASS_AUDIO_IF_OUT0_IRQ,
		.end	= LPASS_AUDIO_IF_OUT0_IRQ,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct resource ipq_lpass_clk[] = {
        {
		.start = IPQ806X_LPASS_CLK_CTL_PHYS,
		.end   = IPQ806X_LPASS_CLK_CTL_PHYS + SZ_4K,
		.flags = IORESOURCE_MEM,
		.name  = "ipq-lpass-clk",
        },
};

static uint32_t ipq_chip_version = 0;

struct platform_device ipq806x_lpass_lpaif = {
	.name = "ipq-lpaif",
	.id = -1,
	.num_resources = ARRAY_SIZE(ipq_lpaif_resources),
	.resource = ipq_lpaif_resources,
	.dev	= {
		.platform_data = &ipq_chip_version,
	},
};

struct  platform_device ipq806x_lpass_dmlite = {
	.name = "ipq-dmlite",
	.id = -1,
	.num_resources = ARRAY_SIZE(ipq_dmlite_resource),
	.resource = ipq_dmlite_resource,
};

struct platform_device ipq806x_lpass_cpudai = {
	.name = "ipq-cpu-dai",
	.id = -1,
};

struct platform_device ipq806x_lpass_clock = {
	.name = "ipq-lpass-clk",
	.id = -1,
	.num_resources = ARRAY_SIZE(ipq_lpass_clk),
	.resource = ipq_lpass_clk,
};

struct platform_device ipq806x_lpass_spdif = {
	.name = "ipq-spdif",
	.id = -1,
	.num_resources = ARRAY_SIZE(ipq_spdif_resources),
	.resource = ipq_spdif_resources,
};

struct platform_device ipq806x_lpass_pcm_mi2s = {
	.name = "ipq-pcm-mi2s",
	.id = -1,
};

struct platform_device ipq806x_lpass_pcm_voip = {
	.name = "ipq-pcm-voip",
	.id = -1,
};

struct platform_device ipq806x_lpass_pcm_spdif = {
	.name = "ipq-pcm-spdif",
	.id = -1,
};

struct platform_device ipq806x_mi2s_codec = {
	.name = "ipq-mi2s-codec",
	.id = -1,
};

struct platform_device ipq806x_spdif_codec = {
	.name = "ipq-spdif-codec",
	.id = -1,
};

struct platform_device ipq806x_pcm_codec = {
	.name = "ipq-pcm-codec",
	.id = -1,
};

struct platform_device ipq806x_lpass_pcm_raw = {
	.name = "ipq-pcm-raw",
	.id = -1,
};

struct msm_rtb_platform_data ipq806x_rtb_pdata = {
	.size = SZ_1M,
};

static int __init msm_rtb_set_buffer_size(char *p)
{
	int s;

	s = memparse(p, NULL);
	ipq806x_rtb_pdata.size = ALIGN(s, SZ_4K);
	return 0;
}
early_param("msm_rtb_size", msm_rtb_set_buffer_size);

struct platform_device ipq806x_rtb_device = {
	.name           = "msm_rtb",
	.id             = -1,
	.dev            = {
		.platform_data = &ipq806x_rtb_pdata,
	},
};

#define IPQ806X_L1_SIZE  SZ_1M
/*
 * The actual L2 size is smaller but we need a larger buffer
 * size to store other dump information
 */
#define IPQ806X_L2_SIZE  SZ_2M

struct msm_cache_dump_platform_data ipq806x_cache_dump_pdata = {
	.l2_size = IPQ806X_L2_SIZE,
	.l1_size = IPQ806X_L1_SIZE,
};

struct platform_device ipq806x_cache_dump_device = {
	.name           = "msm_cache_dump",
	.id             = -1,
	.dev            = {
		.platform_data = &ipq806x_cache_dump_pdata,
	},
};

struct platform_device msm_gpio_device = {
	.name = "msmgpio",
	.id = -1,
};

/*
 * NSS crypto for IPQ806x (version 1.0)
 */
#define NSS_CRYPTO_PBASE_SZ	0x20000 /* Crypto Register space size */
#define NSS_CRYPTO_BAM_PBASE_SZ	0x22000 /* Crypto BAM Register space size */
#define NSS_CRYPTO_PBASE_OFST	0x400000 /* Crypto Register offset for each engine */

#define NSS_CRYPTO_PBASE_ENG(engine)		(0x38000000 + (NSS_CRYPTO_PBASE_OFST * engine))
#define NSS_CRYPTO_BAM_PBASE_ENG(engine)	(NSS_CRYPTO_PBASE_ENG(engine) + 0x4000)

#define NSS_CRYPTO_INIT_PLATFORM_DATA_ENG(engine)	{	\
	.crypto_pbase = NSS_CRYPTO_PBASE_ENG(engine),	\
	.crypto_pbase_sz = NSS_CRYPTO_PBASE_SZ,	\
	.bam_pbase = NSS_CRYPTO_BAM_PBASE_ENG(engine),	\
	.bam_pbase_sz = NSS_CRYPTO_BAM_PBASE_SZ,	\
	.bam_ee = 0,	\
}

/*
 * instantiate platform data for nss crypto
 */
static struct nss_crypto_platform_data nss_crypto_eng[] = {
	NSS_CRYPTO_INIT_PLATFORM_DATA_ENG(0),/* Engine - 0 */
	NSS_CRYPTO_INIT_PLATFORM_DATA_ENG(1),/* Engine - 1 */
	NSS_CRYPTO_INIT_PLATFORM_DATA_ENG(2),/* Engine - 2 */
	NSS_CRYPTO_INIT_PLATFORM_DATA_ENG(3),/* Engine - 3 */
};

static u64 nss_crypto_dma_mask = DMA_BIT_MASK(32);
#define NSS_CRYPTO_INIT_PLATFORM_DEVICE_ENG(engine)	{	\
	.name = "nss-crypto",	\
	.id = (engine),	\
	.dev = {	\
		.dma_mask		= &nss_crypto_dma_mask,	\
		.coherent_dma_mask	= DMA_BIT_MASK(32),	\
		.platform_data = &nss_crypto_eng[(engine)],	\
	}	\
}

/*
 * instantiate platform devices for nss crypto
 */
struct platform_device ipq806x_device_nss_crypto[] = {
	NSS_CRYPTO_INIT_PLATFORM_DEVICE_ENG(0),/* Engine - 0 */
	NSS_CRYPTO_INIT_PLATFORM_DEVICE_ENG(1),/* Engine - 1 */
	NSS_CRYPTO_INIT_PLATFORM_DEVICE_ENG(2),/* Engine - 2 */
	NSS_CRYPTO_INIT_PLATFORM_DEVICE_ENG(3),/* Engine - 3 */
};
