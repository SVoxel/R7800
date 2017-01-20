/* * Copyright (c) 2012-2013 The Linux Foundation. All rights reserved.* */
/*
 * Copyright (C) 2007 Google, Inc.
 * Copyright (c) 2008-2012, Code Aurora Forum. All rights reserved.
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
 *
 * The MSM peripherals are spread all over across 768MB of physical
 * space, which makes just having a simple IO_ADDRESS macro to slide
 * them into the right virtual location rough.  Instead, we will
 * provide a master phys->virt mapping for peripherals here.
 *
 */

#ifndef __ASM_ARCH_MSM_IOMAP_IPQ806X_H
#define __ASM_ARCH_MSM_IOMAP_IPQ806X_H

/* Physical base address and size of peripherals.
 * Ordered by the virtual base addresses they will be mapped at.
 *
 * If you add or remove entries here, you'll want to edit the
 * msm_io_desc array in arch/arm/mach-msm/io.c to reflect your
 * changes.
 *
 */

#define IPQ806X_TMR_PHYS		0x0200A000
#define IPQ806X_TMR_SIZE		SZ_4K

#define IPQ806X_TMR0_PHYS		0x0208A000
#define IPQ806X_TMR0_SIZE		SZ_4K

#define IPQ806X_QGIC_DIST_PHYS		0x02000000
#define IPQ806X_QGIC_DIST_SIZE		SZ_4K

#define IPQ806X_QGIC_CPU_PHYS		0x02002000
#define IPQ806X_QGIC_CPU_SIZE		SZ_4K

#define IPQ806X_TLMM_PHYS		0x00800000
#define IPQ806X_TLMM_SIZE		SZ_16K

#define IPQ806X_ACC0_PHYS		0x02088000
#define IPQ806X_ACC0_SIZE		SZ_4K

#define IPQ806X_ACC1_PHYS		0x02098000
#define IPQ806X_ACC1_SIZE		SZ_4K

#define IPQ806X_APCS_GCC_PHYS		0x02011000
#define IPQ806X_APCS_GCC_SIZE		SZ_4K

#define IPQ806X_CLK_CTL_PHYS		0x00900000
#define IPQ806X_CLK_CTL_SIZE		SZ_16K

#define IPQ806X_MMSS_CLK_CTL_PHYS	0x04000000
#define IPQ806X_MMSS_CLK_CTL_SIZE	SZ_4K

#define IPQ806X_LPASS_CLK_CTL_PHYS	0x28000000
#define IPQ806X_LPASS_CLK_CTL_SIZE	SZ_4K

#define IPQ806X_HFPLL_PHYS		0x00903000
#define IPQ806X_HFPLL_SIZE		SZ_4K

/*
 * IMEM starts at 0x2A000000. We skip the initial
 * 252K for the TrustZone image
 */
#define IPQ806X_IMEM_PHYS		0x2A03F000
#define IPQ806X_IMEM_SIZE		SZ_4K

#define IPQ806X_RPM_PHYS		0x00108000
#define IPQ806X_RPM_SIZE		SZ_4K

#define IPQ806X_RPM_MPM_PHYS		0x00200000
#define IPQ806X_RPM_MPM_SIZE		SZ_4K

#define IPQ806X_SAW0_PHYS		0x02089000
#define IPQ806X_SAW0_SIZE		SZ_4K

#define IPQ806X_SAW1_PHYS		0x02099000
#define IPQ806X_SAW1_SIZE		SZ_4K

#define IPQ806X_SAW_L2_PHYS		0x02012000
#define IPQ806X_SAW_L2_SIZE		SZ_4K

#define IPQ806X_RPM_TIMERS_PHYS		0x00062000
#define IPQ806X_RPM_TIMERS_SIZE		SZ_4K

#define IPQ806X_QFPROM_PHYS		0x00700000
#define IPQ806X_QFPROM_SIZE		SZ_4K

#define IPQ806X_SIC_NON_SECURE_PHYS	0x12100000
#define IPQ806X_SIC_NON_SECURE_SIZE	SZ_64K

#ifdef CONFIG_DEBUG_IPQ806X_UART
#define MSM_DEBUG_UART_BASE		IOMEM(0xFA740000)
#define MSM_DEBUG_UART_PHYS		0x16640000
#endif

#define IPQ806X_NSS_TCM_PHYS            0x39000000
#define IPQ806X_NSS_TCM_SIZE            SZ_128K

#define IPQ806X_NSS_FPB_PHYS            0x03000000
#define IPQ806X_NSS_FPB_SIZE            SZ_4K

#define IPQ806X_UBI32_0_CSM_PHYS        0x36000000
#define IPQ806X_UBI32_0_CSM_SIZE        SZ_4K

#define IPQ806X_UBI32_1_CSM_PHYS        0x36400000
#define IPQ806X_UBI32_1_CSM_SIZE        SZ_4K

#define IPQ806X_SFAB_PHYS		0x01300000
#define IPQ806X_SFAB_SIZE		SZ_16K

#define IPQ806X_AFAB_PHYS		0x01400000
#define IPQ806X_AFAB_SIZE		SZ_16K

#define IPQ806X_NSSFAB_0_PHYS		0x00D00000
#define IPQ806X_NSSFAB_0_SIZE		SZ_16K

#define IPQ806X_NSSFAB_1_PHYS		0x01900000
#define IPQ806X_NSSFAB_1_SIZE		SZ_16K

#define IPQ806X_DAY_CFG_PHYS		0x01500000
#define IPQ806X_DAY_CFG_SIZE		SZ_16K

/* Virtrual address for GSBI6 port selection */
#define IPQ806X_GSBI6_PORT_SEL_BASE	(MSM_TLMM_BASE + 0x2088)

#endif /* __ASM_ARCH_MSM_IOMAP_IPQ806X_H */
