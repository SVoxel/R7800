/* * Copyright (c) 2012-2015 The Linux Foundation. All rights reserved.* */
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
#include <linux/bootmem.h>
#include <linux/gpio.h>
#include <asm/mach-types.h>
#include <asm/mach/mmc.h>
#include <mach/msm_bus_board.h>
#include <mach/board.h>
#include <mach/msm_iomap.h>
#include <mach/gpiomux.h>
#include <mach/socinfo.h>
#include "devices.h"
#include "board-ipq806x.h"

static struct gpiomux_setting gsbi1_suspended_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_12MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting gsbi1_active_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_12MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting gsbi2_suspended_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_12MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting gsbi2_active_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_12MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting gsbi4_suspended_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_12MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting gsbi4_active_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_12MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting mi2s_act_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir = GPIOMUX_OUT_HIGH,
};

static struct gpiomux_setting mi2s_sus_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir = GPIOMUX_OUT_HIGH,
};

static struct gpiomux_setting pcm_in_act_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting pcm_out_act_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_DOWN,
	.dir = GPIOMUX_OUT_LOW,
};

static struct gpiomux_setting pcm_in_sus_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting pcm_out_sus_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_DOWN,
	.dir = GPIOMUX_OUT_LOW,
};

static struct gpiomux_setting spdif_act_cfg = {
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir = GPIOMUX_OUT_HIGH,
};

static struct gpiomux_setting spdif_sus_cfg = {
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir = GPIOMUX_OUT_HIGH,
};

#ifdef CONFIG_SPI_QUP
/* GSBI5 pin configuration */
static struct gpiomux_setting gsbi5_spi_clk_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_12MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting gsbi5_spi_cs_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_10MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting gsbi5_spi_data_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_10MA,
	.pull = GPIOMUX_PULL_DOWN,
};
#endif

static struct gpiomux_setting sdc1_clk_active_cfg = {
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting sdc1_cmd_data_0_3_active_cfg = {
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting sdc1_suspended_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting sdc1_data_1_suspended_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting ipq806x_sdc3_card_det_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting ipq806x_mmc_card_det_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting ipq806x_sdc3_uhs_card_det_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_16MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting ipq806x_buttons_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting ipq806x_leds_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting ak01_1xx_eth_interrupt = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir = GPIOMUX_OUT_HIGH,
};

static struct gpiomux_setting ak01_1xx_11ac_leds_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_12MA,
	.pull = GPIOMUX_PULL_DOWN,
	.dir = GPIOMUX_IN,
};

static struct gpiomux_setting ak01_1xx_fp_leds_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
	.dir = GPIOMUX_OUT_LOW,
};

static struct gpiomux_setting ak01_1xx_fp_red_led_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
	.dir = GPIOMUX_OUT_HIGH,
};

static struct gpiomux_setting ak01_1xx_usb_enable = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_12MA,
	.pull = GPIOMUX_PULL_UP,
	.dir = GPIOMUX_OUT_HIGH
};

static struct gpiomux_setting ak01_1xx_dl_reset = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_12MA,
	.pull = GPIOMUX_PULL_UP,
	.dir = GPIOMUX_OUT_HIGH
};

static struct gpiomux_setting ak01_1xx_hub_reset = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_12MA,
	.pull = GPIOMUX_PULL_UP,
	.dir = GPIOMUX_OUT_HIGH
};

static struct gpiomux_setting ak01_1xx_11ac_disable = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_12MA,
	.pull = GPIOMUX_PULL_UP,
	.dir = GPIOMUX_OUT_HIGH
};

static struct gpiomux_setting ak01_1xx_eth_reset = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_12MA,
	.pull = GPIOMUX_PULL_UP,
	.dir = GPIOMUX_OUT_HIGH
};

static struct gpiomux_setting ak01_1xx_dl_uart_tx = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_12MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting ak01_1xx_rst_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_12MA,
	.pull = GPIOMUX_PULL_UP,
	.dir = GPIOMUX_IN,
};

static struct gpiomux_setting ak01_1xx_11ad_led_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_12MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting ak01_1xx_wifi_led_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_12MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting ak01_1xx_wps_switch = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_12MA,
	.pull = GPIOMUX_PULL_UP,
	.dir = GPIOMUX_IN,
};

static struct gpiomux_setting ak01_1xx_11ad_disable = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_12MA,
	.pull = GPIOMUX_PULL_UP,
	.dir = GPIOMUX_OUT_HIGH
};

static struct gpiomux_setting ak01_1xx_wifi_pwrdn = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_12MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting usb30_pwr_en_n  = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_12MA,
	.pull = GPIOMUX_PULL_DOWN,
	.dir = GPIOMUX_OUT_HIGH,
};

static struct gpiomux_setting ipq806x_power_down_2g = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_12MA,
	.pull = GPIOMUX_PULL_UP,
	.dir = GPIOMUX_OUT_HIGH
};

static struct gpiomux_setting ipq806x_power_down_5g = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_12MA,
	.pull = GPIOMUX_PULL_UP,
	.dir = GPIOMUX_OUT_HIGH
};

#ifdef CONFIG_MSM_PCIE
static struct gpiomux_setting pcie_rst_n = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
};

#endif


/*
 * NSS debug pins configuration
 */

/*
 * Core 0, Data
 * No pull up, Function 2
 */
static struct gpiomux_setting nss_spi_data_0 = {
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir = GPIOMUX_IN,
};

/*
 * Core 0, CLK, CS
 * Pull up high, Function 2
 */
static struct gpiomux_setting nss_spi_cs_clk_0 = {
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_UP,
	.dir = GPIOMUX_IN,
};
/*
 * Core 1, CS
 * Pull up high, Function 4
 */
static struct gpiomux_setting nss_spi_cs_1 = {
	.func = GPIOMUX_FUNC_4,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_UP,
	.dir = GPIOMUX_IN,
};

/*
 * Core 1, CLK
 * Pull up high, Function 5
 */
static struct gpiomux_setting nss_spi_clk_1 = {
	.func = GPIOMUX_FUNC_5,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_UP,
	.dir = GPIOMUX_IN,
};

/*
 * Core 1, Data
 * Pull up none, Function 5
 */
static struct gpiomux_setting nss_spi_data_1 = {
	.func = GPIOMUX_FUNC_5,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir = GPIOMUX_IN,
};

static struct gpiomux_setting nss_spi_suspended = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir = GPIOMUX_IN,
};

/*
 * RGMII pins configuration for NSS GMAC1
 */
static struct gpiomux_setting nss_gmac1_rgmii_set = {
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting mdio_n = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting nss_gmac0_rgmii_set0 = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting nss_gmac0_rgmii_set1 = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_16MA,
	.pull = GPIOMUX_PULL_NONE,
};

/* GSBI6 pin configuration for PCM SLIC*/
static struct gpiomux_setting gsbi6_spi_clk_cfg = {
	.func = GPIOMUX_FUNC_3,
	.drv = GPIOMUX_DRV_12MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting gsbi6_spi_cs_cfg = {
	.func = GPIOMUX_FUNC_3,
	.drv = GPIOMUX_DRV_12MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting gsbi6_spi_mosi_cfg = {
	.func = GPIOMUX_FUNC_3,
	.drv = GPIOMUX_DRV_12MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting gsbi6_spi_miso_cfg = {
	.func = GPIOMUX_FUNC_3,
	.drv = GPIOMUX_DRV_14MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting gsbi6_spi_reset_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_10MA,
	.pull = GPIOMUX_PULL_DOWN,
	.dir = GPIOMUX_OUT_HIGH,
};

static struct gpiomux_setting reset_control = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir = GPIOMUX_OUT_LOW,
};

static struct gpiomux_setting dect_reset_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_10MA,
	.pull = GPIOMUX_PULL_DOWN,
	.dir = GPIOMUX_OUT_HIGH,
};

#define ipq_mux(g, a, s)			\
{						\
	.gpio = g,				\
	.settings = {				\
		[GPIOMUX_ACTIVE] = &a,		\
		[GPIOMUX_SUSPENDED] = &s,	\
	},					\
}

static struct msm_gpiomux_config ipq806x_db149_2xx_gpiomux_override[] = {
	ipq_mux(10,	gsbi4_active_cfg,	gsbi4_suspended_cfg),
};

static struct msm_gpiomux_config ipq806x_ap160_2xx_gpiomux_override[] = {
	ipq_mux( 2, mdio_n,				mdio_n),
	ipq_mux(66, mdio_n,				mdio_n),
};

static struct msm_gpiomux_config ipq806x_db149_gpiomux[] = {
	ipq_mux( 0, mdio_n,				mdio_n),
	ipq_mux( 1, mdio_n,				mdio_n),
	ipq_mux( 2, nss_gmac0_rgmii_set0,		nss_gmac0_rgmii_set0),
	ipq_mux( 3, pcie_rst_n,				pcie_rst_n),
	ipq_mux(10, spdif_act_cfg,			spdif_sus_cfg),
	ipq_mux(11, gsbi4_active_cfg,			gsbi4_suspended_cfg),
	ipq_mux(12, gsbi4_active_cfg,			gsbi4_suspended_cfg),
	ipq_mux(13, gsbi4_active_cfg,			gsbi4_suspended_cfg),
	ipq_mux(14, pcm_out_act_cfg,			pcm_out_sus_cfg),
	ipq_mux(15, pcm_in_act_cfg,			pcm_in_sus_cfg),
	ipq_mux(16, pcm_out_act_cfg,			pcm_out_sus_cfg),
	ipq_mux(17, pcm_out_act_cfg,			pcm_out_sus_cfg),
	ipq_mux(18, gsbi5_spi_data_cfg,			gsbi5_spi_data_cfg),
	ipq_mux(19, gsbi5_spi_data_cfg,			gsbi5_spi_data_cfg),
	ipq_mux(20, gsbi5_spi_cs_cfg,			gsbi5_spi_cs_cfg),
	ipq_mux(21, gsbi5_spi_clk_cfg,			gsbi5_spi_clk_cfg),
	ipq_mux(24, gsbi2_active_cfg,			gsbi2_suspended_cfg),
	ipq_mux(25, gsbi2_active_cfg,			gsbi2_suspended_cfg),
	ipq_mux(26, ipq806x_buttons_cfg,		ipq806x_buttons_cfg),

	ipq_mux(27, mi2s_act_cfg,			mi2s_sus_cfg),
	ipq_mux(28, mi2s_act_cfg,			mi2s_sus_cfg),
	ipq_mux(29, mi2s_act_cfg,			mi2s_sus_cfg),
	ipq_mux(30, mi2s_act_cfg,			mi2s_sus_cfg),
	ipq_mux(31, mi2s_act_cfg,			mi2s_sus_cfg),
	ipq_mux(32, mi2s_act_cfg,			mi2s_sus_cfg),
	ipq_mux(33, mi2s_act_cfg,			mi2s_sus_cfg),

	/*
	 * Missing entries are related to NAND. They would have been
	 * configured appropriately by the boot loaders
	 */

	ipq_mux(48, pcie_rst_n,				pcie_rst_n),
	ipq_mux(51, usb30_pwr_en_n,			usb30_pwr_en_n),
	ipq_mux(52, ipq806x_sdc3_card_det_cfg,		ipq806x_sdc3_card_det_cfg),
	ipq_mux(53, gsbi1_active_cfg,			gsbi1_suspended_cfg),
	ipq_mux(54, gsbi1_active_cfg,			gsbi1_suspended_cfg),
	ipq_mux(55, gsbi6_spi_mosi_cfg,			gsbi6_spi_mosi_cfg),
	ipq_mux(56, gsbi6_spi_miso_cfg,			gsbi6_spi_miso_cfg),
	ipq_mux(57, gsbi6_spi_cs_cfg,			gsbi6_spi_cs_cfg),
	ipq_mux(58, gsbi6_spi_clk_cfg,			gsbi6_spi_clk_cfg),
	ipq_mux(59, gsbi6_spi_reset_cfg,		gsbi6_spi_reset_cfg),
	ipq_mux(60, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(61, ipq806x_sdc3_uhs_card_det_cfg,		ipq806x_sdc3_uhs_card_det_cfg),
	ipq_mux(62, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(63, pcie_rst_n,				pcie_rst_n),
	ipq_mux(66, nss_gmac0_rgmii_set1,		nss_gmac0_rgmii_set1),
};

static struct msm_gpiomux_config ipq806x_db149_1xx_gpiomux[] = {
	ipq_mux( 0, mdio_n,				mdio_n),
	ipq_mux( 1, mdio_n,				mdio_n),
	ipq_mux( 2, nss_gmac0_rgmii_set0,		nss_gmac0_rgmii_set0),
	ipq_mux( 3, pcie_rst_n,				pcie_rst_n),
	ipq_mux(10, spdif_act_cfg,			spdif_sus_cfg),
	ipq_mux(12, gsbi4_active_cfg,			gsbi4_suspended_cfg),
	ipq_mux(13, gsbi4_active_cfg,			gsbi4_suspended_cfg),
	ipq_mux(14, pcm_out_act_cfg,			pcm_out_sus_cfg),
	ipq_mux(15, pcm_in_act_cfg,			pcm_in_sus_cfg),
	ipq_mux(16, pcm_out_act_cfg,			pcm_out_sus_cfg),
	ipq_mux(17, pcm_out_act_cfg,			pcm_out_sus_cfg),
	ipq_mux(18, gsbi5_spi_data_cfg,			gsbi5_spi_data_cfg),
	ipq_mux(19, gsbi5_spi_data_cfg,			gsbi5_spi_data_cfg),
	ipq_mux(20, gsbi5_spi_cs_cfg,			gsbi5_spi_cs_cfg),
	ipq_mux(21, gsbi5_spi_clk_cfg,			gsbi5_spi_clk_cfg),
	ipq_mux(24, gsbi2_active_cfg,			gsbi2_suspended_cfg),
	ipq_mux(25, gsbi2_active_cfg,			gsbi2_suspended_cfg),
	ipq_mux(26, ipq806x_buttons_cfg,		ipq806x_buttons_cfg),

	ipq_mux(27, mi2s_act_cfg,			mi2s_sus_cfg),
	ipq_mux(28, mi2s_act_cfg,			mi2s_sus_cfg),
	ipq_mux(29, mi2s_act_cfg,			mi2s_sus_cfg),
	ipq_mux(30, mi2s_act_cfg,			mi2s_sus_cfg),
	ipq_mux(31, mi2s_act_cfg,			mi2s_sus_cfg),
	ipq_mux(32, mi2s_act_cfg,			mi2s_sus_cfg),
	ipq_mux(33, mi2s_act_cfg,			mi2s_sus_cfg),
	ipq_mux(38, sdc1_cmd_data_0_3_active_cfg,	sdc1_suspended_cfg),
	ipq_mux(39, sdc1_cmd_data_0_3_active_cfg,	sdc1_suspended_cfg),
	ipq_mux(40, sdc1_cmd_data_0_3_active_cfg,	sdc1_suspended_cfg),
	ipq_mux(41, sdc1_cmd_data_0_3_active_cfg,	sdc1_suspended_cfg),
	ipq_mux(42, sdc1_clk_active_cfg,		sdc1_suspended_cfg),
	ipq_mux(43, sdc1_cmd_data_0_3_active_cfg,	sdc1_data_1_suspended_cfg),
	ipq_mux(44, sdc1_cmd_data_0_3_active_cfg,	sdc1_suspended_cfg),
	ipq_mux(45, sdc1_cmd_data_0_3_active_cfg,	sdc1_suspended_cfg),
	ipq_mux(46, sdc1_cmd_data_0_3_active_cfg,	sdc1_suspended_cfg),
	ipq_mux(47, sdc1_cmd_data_0_3_active_cfg,	sdc1_suspended_cfg),
	ipq_mux(48, pcie_rst_n,				pcie_rst_n),
	ipq_mux(51, ipq806x_mmc_card_det_cfg,		ipq806x_mmc_card_det_cfg),
	ipq_mux(52, ipq806x_sdc3_card_det_cfg,		ipq806x_sdc3_card_det_cfg),
	ipq_mux(53, gsbi1_active_cfg,			gsbi1_suspended_cfg),
	ipq_mux(54, gsbi1_active_cfg,			gsbi1_suspended_cfg),
	ipq_mux(55, gsbi6_spi_mosi_cfg,			gsbi6_spi_mosi_cfg),
	ipq_mux(56, gsbi6_spi_miso_cfg,			gsbi6_spi_miso_cfg),
	ipq_mux(57, gsbi6_spi_cs_cfg,			gsbi6_spi_cs_cfg),
	ipq_mux(58, gsbi6_spi_clk_cfg,			gsbi6_spi_clk_cfg),
	ipq_mux(59, gsbi6_spi_reset_cfg,		gsbi6_spi_reset_cfg),
	ipq_mux(60, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(61, ipq806x_sdc3_uhs_card_det_cfg,		ipq806x_sdc3_uhs_card_det_cfg),
	ipq_mux(62, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(63, pcie_rst_n,				pcie_rst_n),
	ipq_mux(66, nss_gmac0_rgmii_set1,		nss_gmac0_rgmii_set1),
};

static struct msm_gpiomux_config ipq806x_db147_gpiomux[] = {
	ipq_mux( 0, mdio_n,				mdio_n),
	ipq_mux( 1, mdio_n,				mdio_n),
	ipq_mux( 2, nss_gmac0_rgmii_set0,		nss_gmac0_rgmii_set0),
	ipq_mux( 3, pcie_rst_n,				pcie_rst_n),
	ipq_mux(10, spdif_act_cfg,			spdif_sus_cfg),
	ipq_mux(12, gsbi4_active_cfg,			gsbi4_suspended_cfg),
	ipq_mux(13, gsbi4_active_cfg,			gsbi4_suspended_cfg),
	ipq_mux(14, pcm_out_act_cfg,			pcm_out_sus_cfg),
	ipq_mux(15, pcm_in_act_cfg,			pcm_in_sus_cfg),
	ipq_mux(16, pcm_out_act_cfg,			pcm_out_sus_cfg),
	ipq_mux(17, pcm_out_act_cfg,			pcm_out_sus_cfg),
	ipq_mux(18, gsbi5_spi_data_cfg,			gsbi5_spi_data_cfg),
	ipq_mux(19, gsbi5_spi_data_cfg,			gsbi5_spi_data_cfg),
	ipq_mux(20, gsbi5_spi_cs_cfg,			gsbi5_spi_cs_cfg),
	ipq_mux(21, gsbi5_spi_clk_cfg,			gsbi5_spi_clk_cfg),
	ipq_mux(24, gsbi2_active_cfg,			gsbi2_suspended_cfg),
	ipq_mux(25, gsbi2_active_cfg,			gsbi2_suspended_cfg),
	ipq_mux(27, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(28, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(29, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(30, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(31, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(32, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),

#ifdef CONFIG_MMC_MSM_SDC1_SUPPORT
#ifdef CONFIG_MMC_MSM_SDC1_8_BIT_SUPPORT
	ipq_mux(38, sdc1_cmd_data_0_3_active_cfg,	sdc1_suspended_cfg),
	ipq_mux(39, sdc1_cmd_data_0_3_active_cfg,	sdc1_suspended_cfg),
#endif
	ipq_mux(40, sdc1_cmd_data_0_3_active_cfg,	sdc1_suspended_cfg),
	ipq_mux(41, sdc1_cmd_data_0_3_active_cfg,	sdc1_suspended_cfg),
	ipq_mux(42, sdc1_clk_active_cfg,		sdc1_suspended_cfg),
	ipq_mux(43, sdc1_cmd_data_0_3_active_cfg,	sdc1_data_1_suspended_cfg),
	ipq_mux(44, sdc1_cmd_data_0_3_active_cfg,	sdc1_suspended_cfg),
	ipq_mux(45, sdc1_cmd_data_0_3_active_cfg,	sdc1_suspended_cfg),
#ifdef CONFIG_MMC_MSM_SDC1_8_BIT_SUPPORT
	ipq_mux(46, sdc1_cmd_data_0_3_active_cfg,	sdc1_suspended_cfg),
	ipq_mux(47, sdc1_cmd_data_0_3_active_cfg,	sdc1_suspended_cfg),
#endif
#endif
	ipq_mux(48, pcie_rst_n,				pcie_rst_n),
	ipq_mux(51, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(52, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	/*        53 Not available in pkg */
	/*        54 Not available in pkg */
	ipq_mux(55, gsbi6_spi_mosi_cfg,			gsbi6_spi_mosi_cfg),
	ipq_mux(56, gsbi6_spi_miso_cfg,			gsbi6_spi_miso_cfg),
	ipq_mux(57, gsbi6_spi_cs_cfg,			gsbi6_spi_cs_cfg),
	ipq_mux(58, gsbi6_spi_clk_cfg,			gsbi6_spi_clk_cfg),
	ipq_mux(59, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(60, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(61, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(62, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	/*        63 Not available in pkg */
};

static struct msm_gpiomux_config ipq806x_ap148_gpiomux[] = {
	ipq_mux( 0, mdio_n,				mdio_n),
	ipq_mux( 1, mdio_n,				mdio_n),
	ipq_mux( 2, nss_gmac0_rgmii_set0,		nss_gmac0_rgmii_set0),
	ipq_mux( 3, pcie_rst_n,				pcie_rst_n),
	ipq_mux( 6, ipq806x_buttons_cfg,		ipq806x_buttons_cfg),
	ipq_mux( 7, ipq806x_leds_cfg,	ipq806x_leds_cfg),
	ipq_mux( 8, ipq806x_leds_cfg,	ipq806x_leds_cfg),
	ipq_mux( 9, ipq806x_leds_cfg,	ipq806x_leds_cfg),
	ipq_mux(12, gsbi4_active_cfg,			gsbi4_suspended_cfg),
	ipq_mux(13, gsbi4_active_cfg,			gsbi4_suspended_cfg),
	ipq_mux(14, pcm_out_act_cfg,			pcm_out_sus_cfg),
	ipq_mux(15, usb30_pwr_en_n,			usb30_pwr_en_n),
	ipq_mux(16, usb30_pwr_en_n,			usb30_pwr_en_n),
	ipq_mux(17, pcm_out_act_cfg,			pcm_out_sus_cfg),
	ipq_mux(18, gsbi5_spi_data_cfg,			gsbi5_spi_data_cfg),
	ipq_mux(19, gsbi5_spi_data_cfg,			gsbi5_spi_data_cfg),
	ipq_mux(20, gsbi5_spi_cs_cfg,			gsbi5_spi_cs_cfg),
	ipq_mux(21, gsbi5_spi_clk_cfg,			gsbi5_spi_clk_cfg),
	ipq_mux(22, ipq806x_leds_cfg,		ipq806x_leds_cfg),
	ipq_mux(23, ipq806x_leds_cfg,		ipq806x_leds_cfg),
	ipq_mux(24, ipq806x_leds_cfg,		ipq806x_leds_cfg),
	ipq_mux(25, gsbi2_active_cfg,			gsbi2_suspended_cfg),
	ipq_mux(26, ipq806x_leds_cfg,	ipq806x_leds_cfg),
	ipq_mux(27, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(28, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(29, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(30, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(31, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(32, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(33, gsbi6_spi_reset_cfg,		gsbi6_spi_reset_cfg),
	ipq_mux(48, pcie_rst_n,				pcie_rst_n),
	ipq_mux(51, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(52, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(53, ipq806x_leds_cfg,	ipq806x_leds_cfg),
	ipq_mux(54, ipq806x_buttons_cfg,	ipq806x_buttons_cfg),
	ipq_mux(55, gsbi6_spi_mosi_cfg,			gsbi6_spi_mosi_cfg),
	ipq_mux(56, gsbi6_spi_miso_cfg,			gsbi6_spi_miso_cfg),
	ipq_mux(57, gsbi6_spi_cs_cfg,			gsbi6_spi_cs_cfg),
	ipq_mux(58, gsbi6_spi_clk_cfg,			gsbi6_spi_clk_cfg),

	ipq_mux(59, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(60, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(61, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(62, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(63, reset_control,			reset_control),
	ipq_mux(64, ipq806x_leds_cfg,		ipq806x_leds_cfg),
	ipq_mux(65, ipq806x_buttons_cfg,	ipq806x_buttons_cfg),
	ipq_mux(67, ipq806x_leds_cfg,		ipq806x_leds_cfg),
	ipq_mux(68, usb30_pwr_en_n,			usb30_pwr_en_n),
};

static struct msm_gpiomux_config ipq806x_ap145_gpiomux[] = {
	ipq_mux( 0, mdio_n,				mdio_n),
	ipq_mux( 1, mdio_n,				mdio_n),
	ipq_mux( 3, pcie_rst_n,				pcie_rst_n),
	ipq_mux(12, gsbi4_active_cfg,			gsbi4_suspended_cfg),
	ipq_mux(13, gsbi4_active_cfg,			gsbi4_suspended_cfg),
	ipq_mux(14, nss_spi_data_0,				nss_spi_suspended),
	ipq_mux(15, nss_spi_data_0,				nss_spi_suspended),
	ipq_mux(16, nss_spi_cs_clk_0,			nss_spi_suspended),
	ipq_mux(17, nss_spi_cs_clk_0,			nss_spi_suspended),
	ipq_mux(18, gsbi5_spi_data_cfg,			gsbi5_spi_data_cfg),
	ipq_mux(19, gsbi5_spi_data_cfg,			gsbi5_spi_data_cfg),
	ipq_mux(20, gsbi5_spi_cs_cfg,			gsbi5_spi_cs_cfg),
	ipq_mux(21, gsbi5_spi_clk_cfg,			gsbi5_spi_clk_cfg),
	ipq_mux(22, ipq806x_buttons_cfg,	ipq806x_buttons_cfg),
	ipq_mux(23, ipq806x_buttons_cfg,		ipq806x_buttons_cfg),
	ipq_mux(24, ipq806x_buttons_cfg,	ipq806x_buttons_cfg),
	ipq_mux(25, nss_gmac0_rgmii_set0,		nss_gmac0_rgmii_set0),
	ipq_mux(26, ipq806x_buttons_cfg,		ipq806x_buttons_cfg),
	ipq_mux(27, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(28, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(29, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(30, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(31, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(32, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(33, ipq806x_buttons_cfg,		ipq806x_buttons_cfg),
	ipq_mux(48, pcie_rst_n,					pcie_rst_n),
	ipq_mux(51, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(52, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(55, nss_spi_data_1,				nss_spi_suspended),
	ipq_mux(56, nss_spi_data_1,				nss_spi_suspended),
	ipq_mux(57, nss_spi_cs_1,				nss_spi_suspended),
	ipq_mux(58, nss_spi_clk_1,				nss_spi_suspended),
	ipq_mux(59, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(60, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(61, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(62, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
};

static struct msm_gpiomux_config ipq806x_mmc_gpiomux_override[] = {
#ifdef CONFIG_MMC_MSM_SDC1_SUPPORT
#ifdef CONFIG_MMC_MSM_SDC1_8_BIT_SUPPORT
	ipq_mux(38, sdc1_cmd_data_0_3_active_cfg,	sdc1_suspended_cfg),
	ipq_mux(39, sdc1_cmd_data_0_3_active_cfg,	sdc1_suspended_cfg),
#endif
	ipq_mux(40, sdc1_cmd_data_0_3_active_cfg,	sdc1_suspended_cfg),
	ipq_mux(41, sdc1_cmd_data_0_3_active_cfg,	sdc1_suspended_cfg),
	ipq_mux(42, sdc1_clk_active_cfg,		sdc1_suspended_cfg),
	ipq_mux(43, sdc1_cmd_data_0_3_active_cfg,	sdc1_data_1_suspended_cfg),
	ipq_mux(44, sdc1_cmd_data_0_3_active_cfg,	sdc1_suspended_cfg),
	ipq_mux(45, sdc1_cmd_data_0_3_active_cfg,	sdc1_suspended_cfg),
#ifdef CONFIG_MMC_MSM_SDC1_8_BIT_SUPPORT
	ipq_mux(46, sdc1_cmd_data_0_3_active_cfg,	sdc1_suspended_cfg),
	ipq_mux(47, sdc1_cmd_data_0_3_active_cfg,	sdc1_suspended_cfg),
#endif
#endif
};

static struct msm_gpiomux_config ipq806x_ap160_gpiomux[] = {
	ipq_mux( 0, mdio_n,				mdio_n),
	ipq_mux( 1, mdio_n,				mdio_n),
	ipq_mux( 2, nss_gmac0_rgmii_set0,		nss_gmac0_rgmii_set0),
	ipq_mux( 3, pcie_rst_n,				pcie_rst_n),
	ipq_mux( 6, ipq806x_sdc3_card_det_cfg,          ipq806x_sdc3_card_det_cfg),
	ipq_mux( 7, ipq806x_leds_cfg,	ipq806x_leds_cfg),
	ipq_mux( 8, ipq806x_sdc3_uhs_card_det_cfg,	ipq806x_sdc3_uhs_card_det_cfg),
	ipq_mux( 9, ipq806x_power_down_2g,		ipq806x_power_down_2g),
	ipq_mux(12, gsbi4_active_cfg,			gsbi4_suspended_cfg),
	ipq_mux(13, gsbi4_active_cfg,			gsbi4_suspended_cfg),
	ipq_mux(14, pcm_out_act_cfg,			pcm_out_sus_cfg),
	ipq_mux(15, pcm_in_act_cfg,			pcm_in_sus_cfg),
	ipq_mux(16, pcm_out_act_cfg,			pcm_out_sus_cfg),
	ipq_mux(17, pcm_out_act_cfg,			pcm_out_sus_cfg),
	ipq_mux(18, gsbi5_spi_data_cfg,			gsbi5_spi_data_cfg),
	ipq_mux(19, gsbi5_spi_data_cfg,			gsbi5_spi_data_cfg),
	ipq_mux(20, gsbi5_spi_cs_cfg,			gsbi5_spi_cs_cfg),
	ipq_mux(21, gsbi5_spi_clk_cfg,			gsbi5_spi_clk_cfg),
	ipq_mux(22, ipq806x_buttons_cfg,	ipq806x_buttons_cfg),
	ipq_mux(26, ipq806x_power_down_5g,		ipq806x_power_down_5g),
	ipq_mux(27, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(28, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(29, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(30, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(31, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(32, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(33, gsbi6_spi_reset_cfg,		gsbi6_spi_reset_cfg),
	ipq_mux(48, pcie_rst_n,				pcie_rst_n),
	ipq_mux(51, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(52, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(53, ipq806x_leds_cfg,			ipq806x_leds_cfg),
	ipq_mux(54, ipq806x_buttons_cfg,		ipq806x_buttons_cfg),
	ipq_mux(55, gsbi6_spi_mosi_cfg,			gsbi6_spi_mosi_cfg),
	ipq_mux(56, gsbi6_spi_miso_cfg,			gsbi6_spi_miso_cfg),
	ipq_mux(57, gsbi6_spi_cs_cfg,			gsbi6_spi_cs_cfg),
	ipq_mux(58, gsbi6_spi_clk_cfg,			gsbi6_spi_clk_cfg),
	ipq_mux(59, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(60, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(61, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(62, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(63, pcie_rst_n,				pcie_rst_n),
	ipq_mux(66, nss_gmac0_rgmii_set1,		nss_gmac0_rgmii_set1),
};
/*
static struct msm_gpiomux_config ipq806x_ap161_gpiomux[] = {
	ipq_mux( 0, mdio_n,				mdio_n),
	ipq_mux( 1, mdio_n,				mdio_n),
	ipq_mux( 2, nss_gmac0_rgmii_set0,		nss_gmac0_rgmii_set0),
	ipq_mux( 3, pcie_rst_n,				pcie_rst_n),
	ipq_mux( 6, dect_reset_cfg,			dect_reset_cfg),
	ipq_mux( 7, ipq806x_leds_cfg,			ipq806x_leds_cfg),
	ipq_mux( 8, ipq806x_leds_cfg,			ipq806x_leds_cfg),
	ipq_mux( 9, ipq806x_leds_cfg,			ipq806x_leds_cfg),
	ipq_mux(12, gsbi4_active_cfg,			gsbi4_suspended_cfg),
	ipq_mux(13, gsbi4_active_cfg,			gsbi4_suspended_cfg),
	ipq_mux(14, pcm_out_act_cfg,			pcm_out_sus_cfg),
	ipq_mux(15, pcm_in_act_cfg,			pcm_in_sus_cfg),
	ipq_mux(16, pcm_out_act_cfg,			pcm_out_sus_cfg),
	ipq_mux(17, pcm_out_act_cfg,			pcm_out_sus_cfg),
	ipq_mux(18, gsbi5_spi_data_cfg,			gsbi5_spi_data_cfg),
	ipq_mux(19, gsbi5_spi_data_cfg,			gsbi5_spi_data_cfg),
	ipq_mux(20, gsbi5_spi_cs_cfg,			gsbi5_spi_cs_cfg),
	ipq_mux(21, gsbi5_spi_clk_cfg,			gsbi5_spi_clk_cfg),
	ipq_mux(22, gsbi2_active_cfg,			gsbi2_suspended_cfg),
	ipq_mux(23, gsbi2_active_cfg,			gsbi2_suspended_cfg),
	ipq_mux(24, gsbi2_active_cfg,			gsbi2_suspended_cfg),
	ipq_mux(25, gsbi2_active_cfg,			gsbi2_suspended_cfg),
	ipq_mux(26, ipq806x_leds_cfg,			ipq806x_leds_cfg),
	ipq_mux(27, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(28, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(29, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(30, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(31, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(32, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(33, gsbi6_spi_reset_cfg,		gsbi6_spi_reset_cfg),
	ipq_mux(48, pcie_rst_n,				pcie_rst_n),
	ipq_mux(51, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(52, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(53, gsbi1_active_cfg,			gsbi1_suspended_cfg),
	ipq_mux(54, ipq806x_buttons_cfg,		ipq806x_buttons_cfg),
	ipq_mux(55, gsbi6_spi_mosi_cfg,			gsbi6_spi_mosi_cfg),
	ipq_mux(56, gsbi6_spi_miso_cfg,			gsbi6_spi_miso_cfg),
	ipq_mux(57, gsbi6_spi_cs_cfg,			gsbi6_spi_cs_cfg),
	ipq_mux(58, gsbi6_spi_clk_cfg,			gsbi6_spi_clk_cfg),
	ipq_mux(59, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(60, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(61, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(62, nss_gmac1_rgmii_set,		nss_gmac1_rgmii_set),
	ipq_mux(63, pcie_rst_n,				pcie_rst_n),
	ipq_mux(66, ipq806x_power_down_5g,		ipq806x_power_down_5g),
	ipq_mux(67, ipq806x_power_down_5g,		ipq806x_power_down_5g),
};
*/

static struct msm_gpiomux_config ipq806x_ak01_1xx_gpiomux[] = {
	ipq_mux(0, mdio_n,				mdio_n),
	ipq_mux(1, mdio_n,				mdio_n),
	ipq_mux(2, ak01_1xx_eth_interrupt,		ak01_1xx_eth_interrupt),
	ipq_mux(3, pcie_rst_n,				pcie_rst_n),
	ipq_mux(6, ak01_1xx_11ac_leds_cfg,		ak01_1xx_11ac_leds_cfg),
	ipq_mux(7, ak01_1xx_fp_leds_cfg,		ak01_1xx_fp_leds_cfg),
	ipq_mux(8, ak01_1xx_fp_leds_cfg,		ak01_1xx_fp_leds_cfg),
	ipq_mux(9, ak01_1xx_fp_red_led_cfg,		ak01_1xx_fp_red_led_cfg),
	ipq_mux(12, gsbi4_active_cfg,			gsbi4_suspended_cfg),
	ipq_mux(13, gsbi4_active_cfg,			gsbi4_suspended_cfg),
	ipq_mux(18, gsbi5_spi_data_cfg,			gsbi5_spi_data_cfg),
	ipq_mux(19, gsbi5_spi_data_cfg,			gsbi5_spi_data_cfg),
	ipq_mux(20, gsbi5_spi_cs_cfg,			gsbi5_spi_cs_cfg),
	ipq_mux(21, gsbi5_spi_clk_cfg,			gsbi5_spi_clk_cfg),
	ipq_mux(27, ak01_1xx_usb_enable,		ak01_1xx_usb_enable),
	ipq_mux(28, ak01_1xx_dl_reset,			ak01_1xx_dl_reset),
	ipq_mux(29, ak01_1xx_hub_reset,			ak01_1xx_hub_reset),
	ipq_mux(30, ak01_1xx_11ac_disable,		ak01_1xx_11ac_disable),
	ipq_mux(32, ak01_1xx_eth_reset,			ak01_1xx_eth_reset),
	ipq_mux(48, pcie_rst_n,				pcie_rst_n),
	ipq_mux(52, ak01_1xx_dl_uart_tx,		ak01_1xx_dl_uart_tx),
	ipq_mux(53, ipq806x_leds_cfg,			ipq806x_leds_cfg),
	ipq_mux(54, ak01_1xx_rst_cfg,			ak01_1xx_rst_cfg),
	ipq_mux(55, gsbi6_spi_mosi_cfg,			gsbi6_spi_mosi_cfg),
	ipq_mux(56, gsbi6_spi_miso_cfg,			gsbi6_spi_miso_cfg),
	ipq_mux(57, gsbi6_spi_cs_cfg,			gsbi6_spi_cs_cfg),
	ipq_mux(58, gsbi6_spi_clk_cfg,			gsbi6_spi_clk_cfg),
	ipq_mux(60, ak01_1xx_11ad_led_cfg,		ak01_1xx_11ad_led_cfg),
	ipq_mux(61, ak01_1xx_wifi_led_cfg,		ak01_1xx_wifi_led_cfg),
	ipq_mux(62, ak01_1xx_wps_switch,		ak01_1xx_wps_switch),
	ipq_mux(63, reset_control,			reset_control),
	ipq_mux(65, ipq806x_buttons_cfg,		ipq806x_buttons_cfg),
	ipq_mux(67, ak01_1xx_11ad_disable,		ak01_1xx_11ad_disable),
	ipq_mux(68, ak01_1xx_wifi_pwrdn,		ak01_1xx_wifi_pwrdn),
};

void __init ipq806x_init_gpiomux(void)
{
	int rc;

	rc = msm_gpiomux_init(NR_GPIO_IRQS);
	if (rc) {
		pr_err(KERN_ERR "msm_gpiomux_init failed %d\n", rc);
		return;
	}

	if (machine_is_ipq806x_db149() || machine_is_ipq806x_db149_2xx()) {
		msm_gpiomux_install(ipq806x_db149_gpiomux,
			ARRAY_SIZE(ipq806x_db149_gpiomux));
		/* GSBI6 needs copy B */
		msm_gpiomux_gsbi_select_copy(IPQ806X_GSBI6_PORT_SEL_BASE, GPIOMUX_COPY_B);
		if (machine_is_ipq806x_db149_2xx())
			msm_gpiomux_install(ipq806x_db149_2xx_gpiomux_override,
			ARRAY_SIZE(ipq806x_db149_2xx_gpiomux_override));
		if (machine_is_ipq806x_emmc_boot())
			msm_gpiomux_install(ipq806x_mmc_gpiomux_override,
			ARRAY_SIZE(ipq806x_mmc_gpiomux_override));

	} else if (machine_is_ipq806x_db149_1xx()) {
		msm_gpiomux_install(ipq806x_db149_1xx_gpiomux,
			ARRAY_SIZE(ipq806x_db149_1xx_gpiomux));
		/* GSBI6 needs copy B */
		msm_gpiomux_gsbi_select_copy(IPQ806X_GSBI6_PORT_SEL_BASE, GPIOMUX_COPY_B);
	} else if (machine_is_ipq806x_db147()) {
		msm_gpiomux_install(ipq806x_db147_gpiomux,
			ARRAY_SIZE(ipq806x_db147_gpiomux));
		/* GSBI6 needs copy B */
		msm_gpiomux_gsbi_select_copy(IPQ806X_GSBI6_PORT_SEL_BASE, GPIOMUX_COPY_B);
	} else if (machine_is_ipq806x_ap148() ||
		machine_is_ipq806x_ap148_1xx()) {
		msm_gpiomux_install(ipq806x_ap148_gpiomux,
			ARRAY_SIZE(ipq806x_ap148_gpiomux));
		/* GSBI6 needs copy B */
		msm_gpiomux_gsbi_select_copy(IPQ806X_GSBI6_PORT_SEL_BASE, GPIOMUX_COPY_B);
	} else if (machine_is_ipq806x_ap145()) {
		msm_gpiomux_install(ipq806x_ap145_gpiomux,
			ARRAY_SIZE(ipq806x_ap145_gpiomux));
		if (machine_is_ipq806x_emmc_boot())
			msm_gpiomux_install(ipq806x_mmc_gpiomux_override,
				ARRAY_SIZE(ipq806x_mmc_gpiomux_override));
	} else if (machine_is_ipq806x_ap145_1xx()) {
		msm_gpiomux_install(ipq806x_ap145_gpiomux,
			ARRAY_SIZE(ipq806x_ap145_gpiomux));
		msm_gpiomux_install(ipq806x_mmc_gpiomux_override,
			ARRAY_SIZE(ipq806x_mmc_gpiomux_override));
	} else if (machine_is_ipq806x_ap160() || machine_is_ipq806x_ap160_2xx()) {
		msm_gpiomux_install(ipq806x_ap160_gpiomux,
			ARRAY_SIZE(ipq806x_ap160_gpiomux));
		if (machine_is_ipq806x_ap160_2xx()) {
			msm_gpiomux_install(ipq806x_ap160_2xx_gpiomux_override,
			ARRAY_SIZE(ipq806x_ap160_2xx_gpiomux_override));
		}
	} else if (machine_is_ipq806x_ap161()) {
//		msm_gpiomux_install(ipq806x_ap161_gpiomux,
//			ARRAY_SIZE(ipq806x_ap161_gpiomux));
		//For R7600 board, it is ap148 gpio, ap161 cpu.
		msm_gpiomux_install(ipq806x_ap148_gpiomux,
			ARRAY_SIZE(ipq806x_ap148_gpiomux));
		/* GSBI6 needs copy B */
		msm_gpiomux_gsbi_select_copy(IPQ806X_GSBI6_PORT_SEL_BASE, GPIOMUX_COPY_B);
	} else if (machine_is_ipq806x_ak01_1xx()) {
		msm_gpiomux_install(ipq806x_ak01_1xx_gpiomux,
			ARRAY_SIZE(ipq806x_ak01_1xx_gpiomux));
		/* GSBI6 needs copy B */
		msm_gpiomux_gsbi_select_copy(IPQ806X_GSBI6_PORT_SEL_BASE,
			GPIOMUX_COPY_B);
	}

}
