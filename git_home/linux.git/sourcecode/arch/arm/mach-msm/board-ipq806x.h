/* * Copyright (c) 2012 The Linux Foundation. All rights reserved.* */
/* Copyright (c) 2011-2012, Code Aurora Forum. All rights reserved.
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

#ifndef __ARCH_ARM_MACH_MSM_BOARD_IPQ806X_H
#define __ARCH_ARM_MACH_MSM_BOARD_IPQ806X_H

#include <linux/regulator/msm-gpio-regulator.h>
#include <linux/mfd/pm8xxx/pm8921.h>
#include <mach/msm_memtypes.h>
#include <mach/irqs.h>
#include <mach/rpm-regulator.h>
#include <linux/regulator/fixed.h>
#include <mach/msm_rtb.h>
#include <mach/msm_cache_dump.h>
#include <mach/msm_nss_gmac.h>
#include <linux/regulator/fixed.h>

#include "pcie.h"
#include <mach/socinfo.h>

/* Macros assume PMIC GPIOs and MPPs start at 1 */
#define PM8921_GPIO_BASE		NR_GPIO_IRQS
#define PM8921_GPIO_PM_TO_SYS(pm_gpio)	(pm_gpio - 1 + PM8921_GPIO_BASE)
#define PM8921_MPP_BASE			(PM8921_GPIO_BASE + PM8921_NR_GPIOS)
#define PM8921_MPP_PM_TO_SYS(pm_mpp)	(pm_mpp - 1 + PM8921_MPP_BASE)
#define PM8921_IRQ_BASE			(NR_MSM_IRQS + NR_GPIO_IRQS)

#define PM8821_MPP_BASE			(PM8921_MPP_BASE + PM8921_NR_MPPS)
#define PM8821_MPP_PM_TO_SYS(pm_mpp)	(pm_mpp - 1 + PM8821_MPP_BASE)
#define PM8821_IRQ_BASE			(PM8921_IRQ_BASE + PM8921_NR_IRQS)

#define TABLA_INTERRUPT_BASE		(PM8821_IRQ_BASE + PM8821_NR_IRQS)

#define GPIO_VREG_ID_EXT_5V		0
#define GPIO_VREG_ID_EXT_3P3V		1
#define GPIO_VREG_ID_EXT_TS_SW		2
#define GPIO_VREG_ID_EXT_MPP8		3
#define GPIO_VREG_ID_EXT_SATA_PWR	4

#define GPIO_VREG_ID_AVC_1P2V		0
#define GPIO_VREG_ID_AVC_1P8V		1
#define GPIO_VREG_ID_AVC_2P2V		2
#define GPIO_VREG_ID_AVC_5V		3
#define GPIO_VREG_ID_AVC_3P3V		4

#define IPQ806X_EXT_3P3V_REG_EN_GPIO	77

extern struct gpio_regulator_platform_data
	ipq806x_gpio_regulator_pdata[] __devinitdata;

extern struct rpm_regulator_platform_data
	ipq806x_rpm_regulator_pdata __devinitdata;

extern struct rpm_regulator_platform_data
	ipq806x_rpm_regulator_pm8921_pdata __devinitdata;

extern struct rpm_regulator_platform_data
    ipq806x_rpm_regulator_smb_pdata __devinitdata;

extern struct fixed_voltage_config ipq806x_fixed_regul_ssusb_vdd_dig1;
extern struct fixed_voltage_config ipq806x_fixed_regul_SSUSB_VDDCX1;
extern struct fixed_voltage_config ipq806x_fixed_regul_SSUSB_1p81;
extern struct fixed_voltage_config ipq806x_fixed_regul_hsusb_vdd_dig1;
extern struct fixed_voltage_config ipq806x_fixed_regul_HSUSB_VDDCX1;
extern struct fixed_voltage_config ipq806x_fixed_regul_HSUSB_3p31;
extern struct fixed_voltage_config ipq806x_fixed_regul_HSUSB_1p81;

extern struct fixed_voltage_config ipq806x_fixed_regul_ssusb_vdd_dig2;
extern struct fixed_voltage_config ipq806x_fixed_regul_SSUSB_VDDCX2;
extern struct fixed_voltage_config ipq806x_fixed_regul_SSUSB_1p82;
extern struct fixed_voltage_config ipq806x_fixed_regul_hsusb_vdd_dig2;
extern struct fixed_voltage_config ipq806x_fixed_regul_HSUSB_VDDCX2;
extern struct fixed_voltage_config ipq806x_fixed_regul_HSUSB_3p32;
extern struct fixed_voltage_config ipq806x_fixed_regul_HSUSB_1p82;

extern struct fixed_voltage_config ipq806x_fixed_regul_hsic_vdd_dig1;
void __init fixup_ipq806x_smb_power_grid(void);

extern struct mdio_board_info ipq806x_db149_mdio_info[IPQ806X_MDIO_BUS_MAX];
extern struct mdio_board_info ipq806x_db147_mdio_info[IPQ806X_MDIO_BUS_MAX];
extern struct mdio_board_info ipq806x_ap148_mdio_info[IPQ806X_MDIO_BUS_MAX];
extern struct mdio_board_info ipq806x_ap160_mdio_info[IPQ806X_MDIO_BUS_MAX];

struct mmc_platform_data;
int __init ipq806x_add_sdcc(unsigned int controller,
		struct mmc_platform_data *plat);

void ipq806x_init_mmc(void);
void ipq806x_init_gpiomux(void);
void ipq806x_init_pmic(void);

#define SCM_GSBI_ADM_MUX_SEL_CMD	0x5

/* Masks for GSBI ADM MUX SEL */
#define IPQ806X_GSBI1_ADM_CRCI_MUX_SEL_MASK	0x1800
#define IPQ806X_GSBI2_ADM_CRCI_MUX_SEL_MASK	0x6000
#define IPQ806X_GSBI3_ADM_CRCI_MUX_SEL_MASK	0x0030
#define IPQ806X_GSBI4_ADM_CRCI_MUX_SEL_MASK	0x00C0
#define IPQ806X_GSBI5_ADM_CRCI_MUX_SEL_MASK	0x0300
#define IPQ806X_GSBI6_ADM_CRCI_MUX_RX_SEL_MASK	0x0400
#define IPQ806X_GSBI6_ADM_CRCI_MUX_TX_SEL_MASK	0x0020
#define IPQ806X_GSBI7_ADM_CRCI_MUX_SEL_MASK	0x00C0

/* TCSR A/B REG */
#define IPQ806X_TCSR_REG_A_ADM_CRCI_MUX_SEL	0
#define IPQ806X_TCSR_REG_B_ADM_CRCI_MUX_SEL	1
#define IPQ806X_TCSR_REG_ADM_CRCI_MUX_SEL_UART	1
#define IPQ806X_TCSR_REG_ADM_CRCI_MUX_SEL_QUP	0

#define IPQ806X_MCP23S08_I2C_SLAVE_ADDRESS 0x46

#define IPQ806X_GSBI1_QUP_I2C_BUS_ID 0
#define IPQ806X_GSBI2_QUP_I2C_BUS_ID 1
#define IPQ806X_GSBI4_QUP_I2C_BUS_ID 3
#define IPQ806X_GSBI5_QUP_I2C_BUS_ID 4

void ipq806x_pm8xxx_gpio_mpp_init(void);

#define GPIO_EXPANDER_IRQ_BASE	(TABLA_INTERRUPT_BASE + \
					NR_TABLA_IRQS)
#define GPIO_EXPANDER_GPIO_BASE	(PM8821_MPP_BASE + PM8821_NR_MPPS)

#define GPIO_EPM_EXPANDER_BASE	GPIO_EXPANDER_GPIO_BASE
#define SX150X_EPM_NR_GPIOS	16
#define SX150X_EPM_NR_IRQS	8

#define SX150X_EXP1_GPIO_BASE	(GPIO_EPM_EXPANDER_BASE + \
					SX150X_EPM_NR_GPIOS)
#define SX150X_EXP1_IRQ_BASE	(GPIO_EXPANDER_IRQ_BASE + \
				SX150X_EPM_NR_IRQS)
#define SX150X_EXP1_NR_IRQS	16
#define SX150X_EXP1_NR_GPIOS	16

#define SX150X_EXP2_GPIO_BASE	(SX150X_EXP1_GPIO_BASE + \
					SX150X_EXP1_NR_GPIOS)
#define SX150X_EXP2_IRQ_BASE	(SX150X_EXP1_IRQ_BASE + SX150X_EXP1_NR_IRQS)
#define SX150X_EXP2_NR_IRQS	8
#define SX150X_EXP2_NR_GPIOS	8

#define SX150X_EXP3_GPIO_BASE	(SX150X_EXP2_GPIO_BASE + \
					SX150X_EXP2_NR_GPIOS)
#define SX150X_EXP3_IRQ_BASE	(SX150X_EXP2_IRQ_BASE + SX150X_EXP2_NR_IRQS)
#define SX150X_EXP3_NR_IRQS	8
#define SX150X_EXP3_NR_GPIOS	8

#define SX150X_EXP4_GPIO_BASE	(SX150X_EXP3_GPIO_BASE + \
					SX150X_EXP3_NR_GPIOS)
#define SX150X_EXP4_IRQ_BASE	(SX150X_EXP3_IRQ_BASE + SX150X_EXP3_NR_IRQS)
#define SX150X_EXP4_NR_IRQS	16
#define SX150X_EXP4_NR_GPIOS	16

#define SX150X_GPIO(_expander, _pin) (SX150X_EXP##_expander##_GPIO_BASE + _pin)

enum {
	SX150X_EPM,
	SX150X_EXP1,
	SX150X_EXP2,
	SX150X_EXP3,
	SX150X_EXP4,
};

/* GPIO Definitions for IPQ806x */

/* PCIE gpios */
#define PCIE_RST_GPIO		3
#define PCIE_PWR_EN_GPIO	-EINVAL
#define PCIE_WAKE_N_GPIO	4

#define PCIE_1_RST_GPIO		48
#define PCIE_1_PWR_EN_GPIO	-EINVAL
#define PCIE_1_WAKE_N_GPIO	49

#define PCIE_2_RST_GPIO		63
#define PCIE_2_PWR_EN_GPIO	-EINVAL
#define PCIE_2_WAKE_N_GPIO	64

/*
 * Summit PMIC related constants
 */
/* Default Voltage setting for Summit programmable regulators */
#define SMB208_DEFAULT_UV  1150000

/* Range of voltages supported by fixed summit regulators on board */
#define SMB208_FIXED_MINUV	100000
#define SMB208_FIXED_MAXUV	5000000

/*This GPIO is for SDCARD detection*/
#define SDCARD_DETECT_GPIO	52

extern struct msm_rtb_platform_data ipq806x_rtb_pdata;
extern struct msm_cache_dump_platform_data ipq806x_cache_dump_pdata;
#endif
