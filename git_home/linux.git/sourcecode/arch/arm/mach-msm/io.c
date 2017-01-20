/* * Copyright (c) 2012 The Linux Foundation. All rights reserved.* */
/* arch/arm/mach-msm/io.c
 *
 * MSM7K, QSD io support
 *
 * Copyright (C) 2007 Google, Inc.
 * Copyright (c) 2008-2012, The Linux Foundation. All rights reserved.
 * Author: Brian Swetland <swetland@google.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/export.h>

#include <mach/hardware.h>
#include <asm/page.h>
#include <mach/msm_iomap.h>
#include <asm/mach/map.h>
#include <linux/dma-mapping.h>

#include <mach/board.h>

#define MSM_CHIP_DEVICE(name, chip) { \
		.virtual = (unsigned long) MSM_##name##_BASE, \
		.pfn = __phys_to_pfn(chip##_##name##_PHYS), \
		.length = chip##_##name##_SIZE, \
		.type = MT_DEVICE, \
	 }

#define MSM_DEVICE(name) MSM_CHIP_DEVICE(name, MSM)

/* msm_shared_ram_phys default value of 0x00100000 is the most common value
 * and should work as-is for any target without stacked memory.
 */
unsigned int msm_shared_ram_phys = 0x00100000;

static void __init msm_map_io(struct map_desc *io_desc, int size)
{
	int i;

	BUG_ON(!size);
	for (i = 0; i < size; i++)
		if (io_desc[i].virtual == (unsigned long)MSM_SHARED_RAM_BASE)
			io_desc[i].pfn = __phys_to_pfn(msm_shared_ram_phys);

	iotable_init(io_desc, size);
}

#if defined(CONFIG_ARCH_MSM7X01A) || defined(CONFIG_ARCH_MSM7X27) \
	|| defined(CONFIG_ARCH_MSM7X25)
static struct map_desc msm_io_desc[] __initdata = {
	MSM_CHIP_DEVICE(VIC, MSM7XXX),
	MSM_CHIP_DEVICE(CSR, MSM7XXX),
	MSM_CHIP_DEVICE(TMR, MSM7XXX),
	MSM_CHIP_DEVICE(GPIO1, MSM7XXX),
	MSM_CHIP_DEVICE(GPIO2, MSM7XXX),
	MSM_CHIP_DEVICE(CLK_CTL, MSM7XXX),
	MSM_CHIP_DEVICE(AD5, MSM7XXX),
	MSM_CHIP_DEVICE(MDC, MSM7XXX),
#if defined(CONFIG_DEBUG_MSM_UART1) || defined(CONFIG_DEBUG_MSM_UART2) || \
	defined(CONFIG_DEBUG_MSM_UART3)
	MSM_DEVICE(DEBUG_UART),
#endif
#ifdef CONFIG_CACHE_L2X0
	{
		.virtual =  (unsigned long) MSM_L2CC_BASE,
		.pfn =      __phys_to_pfn(MSM7XXX_L2CC_PHYS),
		.length =   MSM7XXX_L2CC_SIZE,
		.type =     MT_DEVICE,
	},
#endif
	{
		.virtual =  (unsigned long) MSM_SHARED_RAM_BASE,
		.length =   MSM_SHARED_RAM_SIZE,
		.type =     MT_DEVICE,
	},
};

void __init msm_map_common_io(void)
{
	/*Peripheral port memory remap, nothing looks to be there for
	 * cortex a5.
	 */
#ifndef CONFIG_ARCH_MSM_CORTEX_A5
	/* Make sure the peripheral register window is closed, since
	 * we will use PTE flags (TEX[1]=1,B=0,C=1) to determine which
	 * pages are peripheral interface or not.
	 */
	asm("mcr p15, 0, %0, c15, c2, 4" : : "r" (0));
#endif
	msm_map_io(msm_io_desc, ARRAY_SIZE(msm_io_desc));
}
#endif

#ifdef CONFIG_ARCH_QSD8X50
static struct map_desc qsd8x50_io_desc[] __initdata = {
	MSM_DEVICE(VIC),
	MSM_DEVICE(CSR),
	MSM_DEVICE(TMR),
	MSM_DEVICE(GPIO1),
	MSM_DEVICE(GPIO2),
	MSM_DEVICE(CLK_CTL),
	MSM_DEVICE(SIRC),
	MSM_DEVICE(SCPLL),
	MSM_DEVICE(AD5),
	MSM_DEVICE(MDC),
	MSM_DEVICE(TCSR),
#if defined(CONFIG_DEBUG_MSM_UART1) || defined(CONFIG_DEBUG_MSM_UART2) || \
	defined(CONFIG_DEBUG_MSM_UART3)
	MSM_DEVICE(DEBUG_UART),
#endif
	{
		.virtual =  (unsigned long) MSM_SHARED_RAM_BASE,
		.length =   MSM_SHARED_RAM_SIZE,
		.type =     MT_DEVICE,
	},
};

void __init msm_map_qsd8x50_io(void)
{
	msm_map_io(qsd8x50_io_desc, ARRAY_SIZE(qsd8x50_io_desc));
}
#endif /* CONFIG_ARCH_QSD8X50 */

#ifdef CONFIG_ARCH_MSM8X60
static struct map_desc msm8x60_io_desc[] __initdata = {
	MSM_DEVICE(QGIC_DIST),
	MSM_DEVICE(QGIC_CPU),
	MSM_DEVICE(TMR),
	MSM_DEVICE(TMR0),
	MSM_DEVICE(RPM_MPM),
	MSM_DEVICE(ACC),
	MSM_DEVICE(ACC0),
	MSM_DEVICE(ACC1),
	MSM_DEVICE(SAW0),
	MSM_DEVICE(SAW1),
	MSM_DEVICE(GCC),
	MSM_DEVICE(TLMM),
	MSM_DEVICE(SCPLL),
	MSM_DEVICE(RPM),
	MSM_DEVICE(CLK_CTL),
	MSM_DEVICE(MMSS_CLK_CTL),
	MSM_DEVICE(LPASS_CLK_CTL),
	MSM_DEVICE(TCSR),
	MSM_DEVICE(IMEM),
	MSM_DEVICE(HDMI),
#ifdef CONFIG_DEBUG_MSM8660_UART
	MSM_DEVICE(DEBUG_UART),
#endif
	MSM_DEVICE(SIC_NON_SECURE),
	{
		.virtual =  (unsigned long) MSM_SHARED_RAM_BASE,
		.length =   MSM_SHARED_RAM_SIZE,
		.type =     MT_DEVICE,
	},
	MSM_DEVICE(QFPROM),
};

void __init msm_map_msm8x60_io(void)
{
	msm_map_io(msm8x60_io_desc, ARRAY_SIZE(msm8x60_io_desc));
	init_consistent_dma_size(14*SZ_1M);
}
#endif /* CONFIG_ARCH_MSM8X60 */

#ifdef CONFIG_ARCH_MSM8960
static struct map_desc msm8960_io_desc[] __initdata = {
	MSM_CHIP_DEVICE(QGIC_DIST, MSM8960),
	MSM_CHIP_DEVICE(QGIC_CPU, MSM8960),
	MSM_CHIP_DEVICE(ACC0, MSM8960),
	MSM_CHIP_DEVICE(ACC1, MSM8960),
	MSM_CHIP_DEVICE(TMR, MSM8960),
	MSM_CHIP_DEVICE(TMR0, MSM8960),
	MSM_CHIP_DEVICE(RPM_MPM, MSM8960),
	MSM_CHIP_DEVICE(CLK_CTL, MSM8960),
	MSM_CHIP_DEVICE(MMSS_CLK_CTL, MSM8960),
	MSM_CHIP_DEVICE(LPASS_CLK_CTL, MSM8960),
	MSM_CHIP_DEVICE(RPM, MSM8960),
	MSM_CHIP_DEVICE(TLMM, MSM8960),
	MSM_CHIP_DEVICE(HFPLL, MSM8960),
	MSM_CHIP_DEVICE(SAW0, MSM8960),
	MSM_CHIP_DEVICE(SAW1, MSM8960),
	MSM_CHIP_DEVICE(SAW_L2, MSM8960),
	MSM_CHIP_DEVICE(SIC_NON_SECURE, MSM8960),
	MSM_CHIP_DEVICE(APCS_GCC, MSM8960),
	MSM_CHIP_DEVICE(IMEM, MSM8960),
	MSM_CHIP_DEVICE(HDMI, MSM8960),
	{
		.virtual =  (unsigned long) MSM_SHARED_RAM_BASE,
		.length =   MSM_SHARED_RAM_SIZE,
		.type =     MT_DEVICE,
	},
#ifdef CONFIG_DEBUG_MSM8960_UART
	MSM_DEVICE(DEBUG_UART),
#endif
	MSM_CHIP_DEVICE(QFPROM, MSM8960),
};

void __init msm_map_msm8960_io(void)
{
	msm_map_io(msm8960_io_desc, ARRAY_SIZE(msm8960_io_desc));
}
#endif /* CONFIG_ARCH_MSM8960 */

#ifdef CONFIG_ARCH_APQ8064
static struct map_desc apq8064_io_desc[] __initdata = {
	MSM_CHIP_DEVICE(QGIC_DIST, APQ8064),
	MSM_CHIP_DEVICE(QGIC_CPU, APQ8064),
	MSM_CHIP_DEVICE(TMR, APQ8064),
	MSM_CHIP_DEVICE(TMR0, APQ8064),
	MSM_CHIP_DEVICE(TLMM, APQ8064),
	MSM_CHIP_DEVICE(ACC0, APQ8064),
	MSM_CHIP_DEVICE(ACC1, APQ8064),
	MSM_CHIP_DEVICE(ACC2, APQ8064),
	MSM_CHIP_DEVICE(ACC3, APQ8064),
	MSM_CHIP_DEVICE(HFPLL, APQ8064),
	MSM_CHIP_DEVICE(CLK_CTL, APQ8064),
	MSM_CHIP_DEVICE(MMSS_CLK_CTL, APQ8064),
	MSM_CHIP_DEVICE(LPASS_CLK_CTL, APQ8064),
	MSM_CHIP_DEVICE(APCS_GCC, APQ8064),
	MSM_CHIP_DEVICE(RPM, APQ8064),
	MSM_CHIP_DEVICE(RPM_MPM, APQ8064),
	MSM_CHIP_DEVICE(SAW0, APQ8064),
	MSM_CHIP_DEVICE(SAW1, APQ8064),
	MSM_CHIP_DEVICE(SAW2, APQ8064),
	MSM_CHIP_DEVICE(SAW3, APQ8064),
	MSM_CHIP_DEVICE(SAW_L2, APQ8064),
	MSM_CHIP_DEVICE(IMEM, APQ8064),
	MSM_CHIP_DEVICE(HDMI, APQ8064),
	{
		.virtual =  (unsigned long) MSM_SHARED_RAM_BASE,
		.length =   MSM_SHARED_RAM_SIZE,
		.type =     MT_DEVICE,
	},
	MSM_CHIP_DEVICE(QFPROM, APQ8064),
	MSM_CHIP_DEVICE(SIC_NON_SECURE, APQ8064),
#ifdef CONFIG_DEBUG_APQ8064_UART
	MSM_DEVICE(DEBUG_UART),
#endif
};

void __init msm_map_apq8064_io(void)
{
	msm_map_io(apq8064_io_desc, ARRAY_SIZE(apq8064_io_desc));
}
#endif /* CONFIG_ARCH_APQ8064 */

#ifdef CONFIG_ARCH_IPQ806X
static struct map_desc ipq806x_io_desc[] __initdata = {
	MSM_CHIP_DEVICE(QGIC_DIST, IPQ806X),
	MSM_CHIP_DEVICE(QGIC_CPU, IPQ806X),
	MSM_CHIP_DEVICE(TMR, IPQ806X),
	MSM_CHIP_DEVICE(TMR0, IPQ806X),
	MSM_CHIP_DEVICE(TLMM, IPQ806X),
	MSM_CHIP_DEVICE(ACC0, IPQ806X),
	MSM_CHIP_DEVICE(ACC1, IPQ806X),
	MSM_CHIP_DEVICE(HFPLL, IPQ806X),
	MSM_CHIP_DEVICE(CLK_CTL, IPQ806X),
	MSM_CHIP_DEVICE(APCS_GCC, IPQ806X),
	MSM_CHIP_DEVICE(RPM, IPQ806X),
	MSM_CHIP_DEVICE(RPM_MPM, IPQ806X),
	MSM_CHIP_DEVICE(RPM_TIMERS, IPQ806X),
	MSM_CHIP_DEVICE(SAW0, IPQ806X),
	MSM_CHIP_DEVICE(SAW1, IPQ806X),
	MSM_CHIP_DEVICE(SAW_L2, IPQ806X),
	MSM_CHIP_DEVICE(IMEM, IPQ806X),
#ifdef CONFIG_DEBUG_IPQ806X_CACHE_ERP_REGS
	MSM_CHIP_DEVICE(SFAB, IPQ806X),
	MSM_CHIP_DEVICE(AFAB, IPQ806X),
	MSM_CHIP_DEVICE(NSSFAB_0, IPQ806X),
	MSM_CHIP_DEVICE(NSSFAB_1, IPQ806X),
	MSM_CHIP_DEVICE(DAY_CFG, IPQ806X),
#endif
	{
		.virtual =  (unsigned long) MSM_SHARED_RAM_BASE,
		.length =   MSM_SHARED_RAM_SIZE,
		.type =     MT_DEVICE,
	},
	MSM_CHIP_DEVICE(QFPROM, IPQ806X),
	MSM_CHIP_DEVICE(SIC_NON_SECURE, IPQ806X),
#ifdef CONFIG_DEBUG_IPQ806X_UART
	MSM_DEVICE(DEBUG_UART),
#endif
	MSM_CHIP_DEVICE(NSS_TCM, IPQ806X),
	MSM_CHIP_DEVICE(NSS_FPB, IPQ806X),
	MSM_CHIP_DEVICE(UBI32_0_CSM, IPQ806X),
	MSM_CHIP_DEVICE(UBI32_1_CSM, IPQ806X),
	MSM_CHIP_DEVICE(LPASS_CLK_CTL, IPQ806X),
};

void __init msm_map_ipq806x_io(void)
{
       msm_map_io(ipq806x_io_desc, ARRAY_SIZE(ipq806x_io_desc));
}
#endif /* CONFIG_ARCH_IPQ806X */

#ifdef CONFIG_ARCH_MSM7X30
static struct map_desc msm7x30_io_desc[] __initdata = {
	MSM_CHIP_DEVICE(VIC, MSM7X30),
	MSM_CHIP_DEVICE(CSR, MSM7X30),
	MSM_CHIP_DEVICE(TMR, MSM7X30),
	MSM_CHIP_DEVICE(GPIO1, MSM7X30),
	MSM_CHIP_DEVICE(GPIO2, MSM7X30),
	MSM_CHIP_DEVICE(CLK_CTL, MSM7X30),
	MSM_CHIP_DEVICE(CLK_CTL_SH2, MSM7X30),
	MSM_CHIP_DEVICE(AD5, MSM7X30),
	MSM_CHIP_DEVICE(MDC, MSM7X30),
	MSM_CHIP_DEVICE(ACC0, MSM7X30),
	MSM_CHIP_DEVICE(SAW0, MSM7X30),
	MSM_CHIP_DEVICE(APCS_GCC, MSM7X30),
	MSM_CHIP_DEVICE(TCSR, MSM7X30),
#if defined(CONFIG_DEBUG_MSM_UART1) || defined(CONFIG_DEBUG_MSM_UART2) || \
	defined(CONFIG_DEBUG_MSM_UART3)
	MSM_DEVICE(DEBUG_UART),
#endif
	{
		.virtual =  (unsigned long) MSM_SHARED_RAM_BASE,
		.length =   MSM_SHARED_RAM_SIZE,
		.type =     MT_DEVICE,
	},
};

void __init msm_map_msm7x30_io(void)
{
	msm_map_io(msm7x30_io_desc, ARRAY_SIZE(msm7x30_io_desc));
}
#endif /* CONFIG_ARCH_MSM7X30 */

#ifdef CONFIG_ARCH_FSM9XXX
static struct map_desc fsm9xxx_io_desc[] __initdata = {
	MSM_DEVICE(VIC),
	MSM_DEVICE(SIRC),
	MSM_DEVICE(CSR),
	MSM_DEVICE(TLMM),
	MSM_DEVICE(TCSR),
	MSM_DEVICE(CLK_CTL),
	MSM_DEVICE(ACC),
	MSM_DEVICE(SAW),
	MSM_DEVICE(GCC),
	MSM_DEVICE(GRFC),
	MSM_DEVICE(QFP_FUSE),
	MSM_DEVICE(HH),
#if defined(CONFIG_DEBUG_MSM_UART1) || defined(CONFIG_DEBUG_MSM_UART2) || \
	defined(CONFIG_DEBUG_MSM_UART3)
	MSM_DEVICE(DEBUG_UART),
#endif
	{
		.virtual =  (unsigned long) MSM_SHARED_RAM_BASE,
		.length =   MSM_SHARED_RAM_SIZE,
		.type =     MT_DEVICE,
	},
};

void __init msm_map_fsm9xxx_io(void)
{
	msm_map_io(fsm9xxx_io_desc, ARRAY_SIZE(fsm9xxx_io_desc));
}
#endif /* CONFIG_ARCH_FSM9XXX */

#ifdef CONFIG_ARCH_MSM8625
static struct map_desc msm8625_io_desc[] __initdata = {
	MSM_CHIP_DEVICE(CSR, MSM7XXX),
	MSM_CHIP_DEVICE(GPIO1, MSM7XXX),
	MSM_CHIP_DEVICE(GPIO2, MSM7XXX),
	MSM_CHIP_DEVICE(QGIC_DIST, MSM8625),
	MSM_CHIP_DEVICE(QGIC_CPU, MSM8625),
	MSM_CHIP_DEVICE(TMR, MSM8625),
	MSM_CHIP_DEVICE(TMR0, MSM8625),
	MSM_CHIP_DEVICE(SCU, MSM8625),
	MSM_CHIP_DEVICE(CFG_CTL, MSM8625),
	MSM_CHIP_DEVICE(CLK_CTL, MSM8625),
	MSM_CHIP_DEVICE(SAW0, MSM8625),
	MSM_CHIP_DEVICE(SAW1, MSM8625),
	MSM_CHIP_DEVICE(AD5, MSM7XXX),
	MSM_CHIP_DEVICE(MDC, MSM7XXX),
#if defined(CONFIG_DEBUG_MSM_UART1) || defined(CONFIG_DEBUG_MSM_UART2) || \
	defined(CONFIG_DEBUG_MSM_UART3)
	MSM_DEVICE(DEBUG_UART),
#endif
#ifdef CONFIG_CACHE_L2X0
	{
		.virtual = (unsigned long) MSM_L2CC_BASE,
		.pfn	 = __phys_to_pfn(MSM7XXX_L2CC_PHYS),
		.length	 = MSM7XXX_L2CC_SIZE,
		.type	 = MT_DEVICE,
	},
#endif
	{
		.virtual = (unsigned long) MSM_SHARED_RAM_BASE,
		.length	 = MSM_SHARED_RAM_SIZE,
		.type	 = MT_DEVICE,
	},
};

void __init msm_map_msm8625_io(void)
{
	msm_map_io(msm8625_io_desc, ARRAY_SIZE(msm8625_io_desc));
}
#else
void __init msm_map_msm8625_io(void) { return; }
#endif /* CONFIG_ARCH_MSM8625 */
