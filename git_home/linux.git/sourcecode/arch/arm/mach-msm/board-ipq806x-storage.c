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
 *
 */

#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <linux/bootmem.h>
#include <asm/mach-types.h>
#include <asm/mach/mmc.h>
#include <mach/msm_bus_board.h>
#include <mach/board.h>
#include <mach/gpiomux.h>
#include "devices.h"
#include "board-ipq806x.h"
#include "board-storage-common-a.h"


/*
 * ipq806x has 2 SDCC controllers sdc1 and sdc3
 * So removed sdc2 and sdc4 related code here...
 */
enum sdcc_controllers {
	SDCC1,
	SDCC3,
	MAX_SDCC_CONTROLLER
};

/* All SDCC controllers require VDD/VCC voltage */
static struct msm_mmc_reg_data mmc_vdd_reg_data[MAX_SDCC_CONTROLLER] = {
	[SDCC1] = {
		.name = "sdc_vdd",
		.high_vol_level = 2950000,
		.low_vol_level = 2950000,
		.hpm_uA = 800000, /* 200mA */
	},
	/* SDCC3 : External card slot connected */
	[SDCC3] = {
		.name = "sdc_vdd",
		.high_vol_level = 2950000,
		.low_vol_level = 2950000,
		.hpm_uA = 800000, /* 800mA */
	}
};

/* SDCC controllers may require voting for VDD IO voltage */
static struct msm_mmc_reg_data mmc_vdd_io_reg_data[MAX_SDCC_CONTROLLER] = {
	[SDCC1] = {
	.name = "sdc_vdd_io",
		.high_vol_level = 2950000,
		.low_vol_level = 1850000,
		.always_on = 1,
		.lpm_sup = 1,
		/* Max. Active current required is 16 mA */
		.hpm_uA = 16000,
		/*
		 * Sleep current required is ~300 uA. But min. vote can be
		 * in terms of mA (min. 1 mA). So let's vote for 2 mA
		 * during sleep.
		 */
		.lpm_uA = 2000,
	},
	/* SDCC3 : External card slot connected */
	[SDCC3] = {
		.name = "sdc_vdd_io",
		.high_vol_level = 2950000,
		.low_vol_level = 1850000,
		.always_on = 1,
		.lpm_sup = 1,
		/* Max. Active current required is 16 mA */
		.hpm_uA = 16000,
		/*
		 * Sleep current required is ~300 uA. But min. vote can be
		 * in terms of mA (min. 1 mA). So let's vote for 2 mA
		 * during sleep.
		 */
		.lpm_uA = 2000,
	}
};

static struct msm_mmc_slot_reg_data mmc_slot_vreg_data[MAX_SDCC_CONTROLLER] = {
	/* SDCC1 : eMMC card connected */
	[SDCC1] = {
		.vdd_data = &mmc_vdd_reg_data[SDCC1],
		.vdd_io_data = &mmc_vdd_io_reg_data[SDCC1],
	},
	/* SDCC3 : External card slot connected */
	[SDCC3] = {
		.vdd_data = &mmc_vdd_reg_data[SDCC3],
		.vdd_io_data = &mmc_vdd_io_reg_data[SDCC3],
	}
};

/* SDC3 pad data */
static struct msm_mmc_pad_drv sdc3_pad_drv_on_cfg[] = {
	{TLMM_HDRV_SDC3_CLK, GPIO_CFG_8MA},
	{TLMM_HDRV_SDC3_CMD, GPIO_CFG_8MA},
	{TLMM_HDRV_SDC3_DATA, GPIO_CFG_8MA}
};

static struct msm_mmc_pad_drv sdc3_pad_drv_off_cfg[] = {
	{TLMM_HDRV_SDC3_CLK, GPIO_CFG_2MA},
	{TLMM_HDRV_SDC3_CMD, GPIO_CFG_2MA},
	{TLMM_HDRV_SDC3_DATA, GPIO_CFG_2MA}
};

static struct msm_mmc_pad_pull sdc3_pad_pull_on_cfg[] = {
	{TLMM_PULL_SDC3_CLK, GPIO_CFG_NO_PULL},
	{TLMM_PULL_SDC3_CMD, GPIO_CFG_PULL_UP},
	{TLMM_PULL_SDC3_DATA, GPIO_CFG_PULL_UP}
};

static struct msm_mmc_pad_pull sdc3_pad_pull_off_cfg[] = {
	{TLMM_PULL_SDC3_CLK, GPIO_CFG_NO_PULL},
	{TLMM_PULL_SDC3_CMD, GPIO_CFG_PULL_UP},
	{TLMM_PULL_SDC3_DATA, GPIO_CFG_PULL_UP}
};

static struct msm_mmc_pad_pull_data mmc_pad_pull_data[MAX_SDCC_CONTROLLER] = {
	[SDCC3] = {
		.on = sdc3_pad_pull_on_cfg,
		.off = sdc3_pad_pull_off_cfg,
		.size = ARRAY_SIZE(sdc3_pad_pull_on_cfg)
	},
};

static struct msm_mmc_pad_drv_data mmc_pad_drv_data[MAX_SDCC_CONTROLLER] = {
	[SDCC3] = {
		.on = sdc3_pad_drv_on_cfg,
		.off = sdc3_pad_drv_off_cfg,
		.size = ARRAY_SIZE(sdc3_pad_drv_on_cfg)
	},
};

static struct msm_mmc_pad_data mmc_pad_data[MAX_SDCC_CONTROLLER] = {
	[SDCC3] = {
		.pull = &mmc_pad_pull_data[SDCC3],
		.drv = &mmc_pad_drv_data[SDCC3]
	},
};

static struct msm_mmc_gpio sdc1_gpio[] = {
	{42, "sdc1_clk"},
	{45, "sdc1_cmd"},
	{44, "sdc1_dat_0"},
	{43, "sdc1_dat_1"},
	{41, "sdc1_dat_2"},
	{40, "sdc1_dat_3"},
#ifdef CONFIG_MMC_MSM_SDC1_8_BIT_SUPPORT
	{47, "sdc1_dat_4"},
	{46, "sdc1_dat_5"},
	{39, "sdc1_dat_6"},
	{38, "sdc1_dat_7"},
#endif
};

static struct msm_mmc_gpio_data mmc_gpio_data[MAX_SDCC_CONTROLLER] = {
	[SDCC1] = {
		.gpio = sdc1_gpio,
		.size = ARRAY_SIZE(sdc1_gpio),
	},
};

static struct msm_mmc_pin_data mmc_slot_pin_data[MAX_SDCC_CONTROLLER] = {
	[SDCC1] = {
		.is_gpio = 1,
		.gpio_data = &mmc_gpio_data[SDCC1],
	},
	[SDCC3] = {
		.pad_data = &mmc_pad_data[SDCC3],
	},
};

#define MSM_MPM_PIN_SDC1_DAT1	17
#define MSM_MPM_PIN_SDC3_DAT1	21

static unsigned int sdc1_sup_clk_rates[] = {
	400000, 24000000, 48000000, 96000000
};

static struct mmc_platform_data sdc1_data = {
	.ocr_mask       = MMC_VDD_27_28 | MMC_VDD_28_29,
#ifdef CONFIG_MMC_MSM_SDC1_8_BIT_SUPPORT
	.mmc_bus_width  = MMC_CAP_8_BIT_DATA,
#else
	.mmc_bus_width  = MMC_CAP_4_BIT_DATA,
#endif
	.sup_clk_table	= sdc1_sup_clk_rates,
	.sup_clk_cnt	= ARRAY_SIZE(sdc1_sup_clk_rates),
	.pin_data	= &mmc_slot_pin_data[SDCC1],
	.vreg_data	= &mmc_slot_vreg_data[SDCC1],
	.status_gpio	= 51,
	.status_irq	= MSM_GPIO_TO_INT(51),
	.irq_flags	= IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING,
	.is_status_gpio_active_low = 1,
	.xpc_cap	= 1,
	.mpm_sdiowakeup_int = MSM_MPM_PIN_SDC1_DAT1,
	.msm_bus_voting_data = &sps_to_ddr_bus_voting_data,
};

static unsigned int sdc3_sup_clk_rates[] = {
	400000, 24000000, 48000000, 96000000, 192000000
};

static struct mmc_platform_data sdc3_data = {
	.ocr_mask       = MMC_VDD_27_28 | MMC_VDD_28_29,
	.mmc_bus_width  = MMC_CAP_4_BIT_DATA,
	.sup_clk_table	= sdc3_sup_clk_rates,
	.sup_clk_cnt	= ARRAY_SIZE(sdc3_sup_clk_rates),
	.pin_data	= &mmc_slot_pin_data[SDCC3],
	.vreg_data	= &mmc_slot_vreg_data[SDCC3],
	.status_gpio	= SDCARD_DETECT_GPIO,
	.uhs_gpio	= 61,
	.status_irq	= MSM_GPIO_TO_INT(SDCARD_DETECT_GPIO),
	.irq_flags	= IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING,
	.is_status_gpio_active_low = 1,
	.xpc_cap	= 1,
	.uhs_caps	= (MMC_CAP_UHS_SDR12 | MMC_CAP_UHS_SDR25 |
			MMC_CAP_UHS_SDR50 | MMC_CAP_UHS_DDR50 |
			MMC_CAP_UHS_SDR104 | MMC_CAP_MAX_CURRENT_800),
	.mpm_sdiowakeup_int = MSM_MPM_PIN_SDC3_DAT1,
	.msm_bus_voting_data = &sps_to_ddr_bus_voting_data,
};

static struct mmc_platform_data sdc3_data_ap160 = {
	.ocr_mask       = MMC_VDD_27_28 | MMC_VDD_28_29,
	.mmc_bus_width  = MMC_CAP_4_BIT_DATA,
	.sup_clk_table  = sdc3_sup_clk_rates,
	.sup_clk_cnt    = ARRAY_SIZE(sdc3_sup_clk_rates),
	.pin_data       = &mmc_slot_pin_data[SDCC3],
	.vreg_data      = &mmc_slot_vreg_data[SDCC3],
	.status_gpio    = 6,
	.uhs_gpio       = 8,
	.status_irq     = MSM_GPIO_TO_INT(6),
	.irq_flags      = IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING,
	.is_status_gpio_active_low = 1,
	.xpc_cap        = 1,
	.uhs_caps       = (MMC_CAP_UHS_SDR12 | MMC_CAP_UHS_SDR25 |
			MMC_CAP_UHS_SDR50 | MMC_CAP_UHS_DDR50 |
			MMC_CAP_UHS_SDR104 | MMC_CAP_MAX_CURRENT_800),
	.mpm_sdiowakeup_int = MSM_MPM_PIN_SDC3_DAT1,
	.msm_bus_voting_data = &sps_to_ddr_bus_voting_data,
};

void __init ipq806x_init_mmc(void)
{
	int i;
	struct msm_mmc_pad_drv_data *drv;

	if (machine_is_ipq806x_db149_1xx() || machine_is_ipq806x_ap145_1xx() ||
						machine_is_ipq806x_emmc_boot())
		ipq806x_add_sdcc(1, &sdc1_data);
	if (machine_is_ipq806x_ap145_1xx() ||
			(machine_is_ipq806x_ap145() && machine_is_ipq806x_emmc_boot())) {
		sdc1_data.status_gpio = -1;
		sdc1_data.status_irq = 0;
	}

	if (machine_is_ipq806x_db149() ||
		machine_is_ipq806x_db149_1xx() ||
		machine_is_ipq806x_db149_2xx()) {
		drv = sdc3_data.pin_data->pad_data->drv;
		for (i = 0; i < drv->size; i++)
			drv->on[i].val = GPIO_CFG_10MA;
		ipq806x_add_sdcc(2, &sdc3_data);
	}

	if (machine_is_ipq806x_ap160() ||
		machine_is_ipq806x_ap160_2xx()) {
		drv = sdc3_data_ap160.pin_data->pad_data->drv;
		for (i = 0; i < drv->size; i++)
			drv->on[i].val = GPIO_CFG_10MA;
		ipq806x_add_sdcc(2, &sdc3_data_ap160);
	}
}
