/* * Copyright (c) 2012-2013 The Linux Foundation. All rights reserved.* */
/* linux/arch/arm/mach-msm/devices.h
 *
 * Copyright (C) 2008 Google, Inc.
 * Copyright (c) 2009-2013, The Linux Foundation. All rights reserved.
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

#ifndef __ARCH_ARM_MACH_MSM_DEVICES_H
#define __ARCH_ARM_MACH_MSM_DEVICES_H

#include <linux/clkdev.h>
#include <linux/platform_device.h>
#include "clock.h"

extern struct platform_device asoc_msm_pcm;
extern struct platform_device asoc_msm_dai0;
extern struct platform_device asoc_msm_dai1;

extern struct platform_device msm_ebi0_thermal;
extern struct platform_device msm_ebi1_thermal;

extern struct platform_device msm_device_uart1;
extern struct platform_device msm_device_uart2;
extern struct platform_device msm_device_uart3;
extern struct platform_device msm8625_device_uart1;

extern struct platform_device msm_device_uart_dm1;
extern struct platform_device msm_device_uart_dm2;
extern struct platform_device msm_device_uart_dm3;
extern struct platform_device msm_device_uart_dm12;
extern struct platform_device *msm_device_uart_gsbi9;
extern struct platform_device msm_device_uart_dm6;
extern struct platform_device msm_device_uart_dm8;
extern struct platform_device msm_device_uart_dm9;
extern struct platform_device mpq8064_device_uartdm_gsbi6;

extern struct platform_device msm8960_device_uart_gsbi2;
extern struct platform_device msm8960_device_uart_gsbi5;
extern struct platform_device msm8960_device_uart_gsbi8;
extern struct platform_device msm8960_device_ssbi_pmic;
extern struct platform_device msm8960_device_qup_i2c_gsbi3;
extern struct platform_device msm8960_device_qup_i2c_gsbi4;
extern struct platform_device msm8960_device_qup_i2c_gsbi8;
extern struct platform_device msm8960_device_qup_i2c_gsbi9;
extern struct platform_device msm8960_device_qup_i2c_gsbi10;
extern struct platform_device msm8960_device_qup_i2c_gsbi12;
extern struct platform_device msm8960_device_qup_spi_gsbi1;
extern struct platform_device msm8960_gemini_device;
extern struct platform_device apq8064_device_uart_gsbi1;
extern struct platform_device apq8064_device_uart_gsbi3;
extern struct platform_device apq8064_device_uart_gsbi4;
extern struct platform_device apq8064_device_uartdm_gsbi4;
extern struct platform_device apq8064_device_uart_gsbi7;
extern struct platform_device ipq806x_device_uartdm_gsbi2;
extern struct platform_device apq8064_device_qup_i2c_gsbi1;
extern struct platform_device apq8064_device_qup_i2c_gsbi3;
extern struct platform_device apq8064_device_qup_i2c_gsbi4;
extern struct platform_device apq8064_device_qup_spi_gsbi5;
extern struct platform_device apq8064_slim_ctrl;
extern struct platform_device apq8064_device_ssbi_pmic1;
extern struct platform_device apq8064_device_ssbi_pmic2;
extern struct platform_device apq8064_device_cache_erp;


extern struct platform_device msm_device_sdc1;
extern struct platform_device msm_device_sdc2;
extern struct platform_device msm_device_sdc3;
extern struct platform_device msm_device_sdc4;

extern struct platform_device msm8960_pc_cntr;
extern struct platform_device msm8064_pc_cntr;

extern struct platform_device msm_device_gadget_peripheral;
extern struct platform_device ipq806x_device_dwc3_host1;
extern struct platform_device ipq806x_device_dwc3_host2;
extern struct platform_device msm_device_hsusb_host;
extern struct platform_device msm_device_hsusb_host2;
extern struct platform_device msm_device_hsic_host;

extern struct platform_device msm8960_cpu_slp_status;
extern struct platform_device msm8064_cpu_slp_status;

extern struct platform_device msm_device_otg;
extern struct platform_device msm_android_usb_device;
extern struct platform_device msm_android_usb_hsic_device;
extern struct platform_device msm_device_hsic_peripheral;
extern struct platform_device msm8960_device_otg;
extern struct platform_device msm8960_device_gadget_peripheral;

extern struct platform_device apq8064_device_otg;
extern struct platform_device apq8064_usb_diag_device;
extern struct platform_device apq8064_device_gadget_peripheral;
extern struct platform_device apq8064_device_hsusb_host;
extern struct platform_device apq8064_device_hsic_host;
extern struct platform_device apq8064_device_ehci_host3;
extern struct platform_device apq8064_device_ehci_host4;

extern struct platform_device msm_device_i2c;

extern struct platform_device msm_device_i2c_2;

extern struct platform_device qup_device_i2c;

extern struct platform_device msm_gsbi0_qup_i2c_device;
extern struct platform_device msm_gsbi1_qup_i2c_device;
extern struct platform_device msm_gsbi3_qup_i2c_device;
extern struct platform_device msm_gsbi4_qup_i2c_device;
extern struct platform_device msm_gsbi7_qup_i2c_device;
extern struct platform_device msm_gsbi8_qup_i2c_device;
extern struct platform_device msm_gsbi9_qup_i2c_device;
extern struct platform_device msm_gsbi12_qup_i2c_device;

extern struct platform_device msm8625_gsbi0_qup_i2c_device;
extern struct platform_device msm8625_gsbi1_qup_i2c_device;
extern struct platform_device msm8625_device_uart_dm1;
extern struct platform_device msm8625_device_uart_dm2;
extern struct platform_device msm8625_device_sdc1;
extern struct platform_device msm8625_device_sdc2;
extern struct platform_device msm8625_device_sdc3;
extern struct platform_device msm8625_device_sdc4;
extern struct platform_device msm8625_device_gadget_peripheral;
extern struct platform_device msm8625_device_hsusb_host;
extern struct platform_device msm8625_device_otg;
extern struct platform_device msm8625_kgsl_3d0;
extern struct platform_device msm8625_device_adsp;

extern struct platform_device msm_slim_ctrl;
extern struct platform_device msm_device_sps;
extern struct platform_device msm_device_usb_bam;
extern struct platform_device msm_device_sps_apq8064;
extern struct platform_device msm_device_bam_dmux;
extern struct platform_device msm_device_smd;
extern struct platform_device msm_device_smd_apq8064;
extern struct platform_device msm8625_device_smd;
extern struct platform_device msm_device_dmov;
extern struct platform_device msm8960_device_dmov;
extern struct platform_device apq8064_device_dmov;
extern struct platform_device msm8625_device_dmov;
extern struct platform_device msm_device_dmov_adm0;
extern struct platform_device msm_device_dmov_adm1;

extern struct platform_device msm_device_pcie[];
extern struct platform_device msm_device_nand;

extern struct platform_device msm_device_tssc;

extern struct platform_device msm_rotator_device;

#ifdef CONFIG_MSM_BUS_SCALING
extern struct msm_bus_scale_pdata rotator_bus_scale_pdata;
#endif

extern struct platform_device msm_device_tsif[2];
extern struct platform_device msm_8064_device_tsif[2];
extern struct platform_device msm_8064_device_tspp;

extern struct platform_device msm_device_ssbi_pmic1;
extern struct platform_device msm_device_ssbi_pmic2;
extern struct platform_device msm_device_ssbi1;
extern struct platform_device msm_device_ssbi2;
extern struct platform_device msm_device_ssbi3;
extern struct platform_device msm_device_ssbi6;
extern struct platform_device msm_device_ssbi7;

extern struct platform_device msm_gsbi1_qup_spi_device;

extern struct platform_device msm_device_vidc_720p;

extern struct platform_device msm_pcm;
extern struct platform_device msm_multi_ch_pcm;
extern struct platform_device msm_lowlatency_pcm;
extern struct platform_device msm_pcm_routing;
extern struct platform_device msm_cpudai0;
extern struct platform_device msm_cpudai1;
extern struct platform_device mpq_cpudai_sec_i2s_rx;
extern struct platform_device msm8960_cpudai_slimbus_2_rx;
extern struct platform_device msm8960_cpudai_slimbus_2_tx;
extern struct platform_device msm_cpudai_hdmi_rx;
extern struct platform_device msm_cpudai_bt_rx;
extern struct platform_device msm_cpudai_bt_tx;
extern struct platform_device msm_cpudai_fm_rx;
extern struct platform_device msm_cpudai_fm_tx;
extern struct platform_device msm_cpudai_auxpcm_rx;
extern struct platform_device msm_cpudai_auxpcm_tx;
extern struct platform_device msm_cpudai_sec_auxpcm_rx;
extern struct platform_device msm_cpudai_sec_auxpcm_tx;
extern struct platform_device msm_cpu_fe;
extern struct platform_device msm_stub_codec;
extern struct platform_device msm_voice;
extern struct platform_device msm_voip;
extern struct platform_device msm_lpa_pcm;
extern struct platform_device msm_pcm_hostless;
extern struct platform_device msm_cpudai_afe_01_rx;
extern struct platform_device msm_cpudai_afe_01_tx;
extern struct platform_device msm_cpudai_afe_02_rx;
extern struct platform_device msm_cpudai_afe_02_tx;
extern struct platform_device msm_pcm_afe;
extern struct platform_device msm_compr_dsp;
extern struct platform_device msm_cpudai_incall_music_rx;
extern struct platform_device msm_cpudai_incall_record_rx;
extern struct platform_device msm_cpudai_incall_record_tx;
extern struct platform_device msm_i2s_cpudai0;
extern struct platform_device msm_i2s_cpudai1;
extern struct platform_device msm_i2s_cpudai4;
extern struct platform_device msm_i2s_cpudai5;
extern struct platform_device msm_cpudai_stub;
extern struct platform_device msm_fm_loopback;

extern struct platform_device msm_pil_q6v3;
extern struct platform_device msm_pil_modem;
extern struct platform_device msm_pil_tzapps;
extern struct platform_device msm_pil_dsps;
extern struct platform_device msm_pil_vidc;
extern struct platform_device msm_8960_q6_lpass;
extern struct platform_device msm_8960_q6_mss_fw;
extern struct platform_device msm_8960_q6_mss_sw;
extern struct platform_device msm_8960_riva;
extern struct platform_device msm_gss;

extern struct platform_device *msm_footswitch_devices[];
extern unsigned msm_num_footswitch_devices;
extern struct platform_device *msm8660_footswitch[];
extern unsigned msm8660_num_footswitch;
extern struct platform_device *msm8960_footswitch[];
extern unsigned msm8960_num_footswitch;
extern struct platform_device *msm8960ab_footswitch[];
extern unsigned msm8960ab_num_footswitch;
extern struct platform_device *apq8064_footswitch[];
extern unsigned apq8064_num_footswitch;

extern struct platform_device fsm_qfp_fuse_device;

extern struct platform_device fsm_xo_device;

extern struct platform_device qfec_device;

extern struct platform_device msm_kgsl_3d0;
extern struct platform_device msm_kgsl_2d0;
extern struct platform_device msm_kgsl_2d1;

extern struct platform_device msm_mipi_dsi1_device;
extern struct platform_device mipi_dsi_device;
extern struct platform_device msm_lvds_device;
extern struct platform_device msm_ebi2_lcdc_device;

extern struct clk_lookup msm_clocks_fsm9xxx[];
extern unsigned msm_num_clocks_fsm9xxx;

extern struct platform_device msm_footswitch;

void __init msm_fb_register_device(char *name, void *data);
void __init msm_camera_register_device(void *, uint32_t, void *);
struct platform_device *msm_add_gsbi9_uart(void);
extern struct platform_device msm_device_touchscreen;

extern struct platform_device led_pdev;

extern struct platform_device msm8960_rpm_device;
extern struct platform_device msm8960_rpm_stat_device;
extern struct platform_device msm8960_rpm_master_stat_device;
extern struct platform_device msm8960_rpm_log_device;


extern struct platform_device msm8660_rpm_device;
extern struct platform_device msm8660_rpm_stat_device;
extern struct platform_device msm8660_rpm_log_device;

extern struct platform_device apq8064_rpm_device;
extern struct platform_device apq8064_rpm_stat_device;
extern struct platform_device apq8064_rpm_master_stat_device;
extern struct platform_device apq8064_rpm_log_device;

extern struct platform_device msm_device_rng;
extern struct platform_device apq8064_device_rng;

extern struct platform_device msm8960_device_watchdog;
extern struct platform_device msm8660_device_watchdog;
extern struct platform_device msm8064_device_watchdog;
extern struct platform_device fsm9xxx_device_watchdog;

extern struct platform_device coresight_tpiu_device;
extern struct platform_device coresight_etb_device;
extern struct platform_device coresight_funnel_device;
extern struct platform_device apq8064_coresight_funnel_device;
extern struct platform_device coresight_etm0_device;
extern struct platform_device coresight_etm1_device;
extern struct platform_device coresight_etm2_device;
extern struct platform_device coresight_etm3_device;
#endif

extern struct platform_device msm_bus_8064_apps_fabric;
extern struct platform_device msm_bus_8064_sys_fabric;
extern struct platform_device msm_bus_8064_mm_fabric;
extern struct platform_device msm_bus_8064_sys_fpb;
extern struct platform_device msm_bus_8064_cpss_fpb;

extern struct platform_device mdm_8064_device;
extern struct platform_device i2s_mdm_8064_device;
extern struct platform_device msm_dsps_device_8064;

extern struct platform_device apq8064_dcvs_device;
extern struct platform_device apq8064_msm_gov_device;

extern struct platform_device mpq8064_device_qup_i2c_gsbi5;

extern struct platform_device msm8660_iommu_domain_device;
extern struct platform_device msm8960_iommu_domain_device;
extern struct platform_device apq8064_iommu_domain_device;

extern struct platform_device msm8960_rtb_device;
extern struct platform_device apq8064_rtb_device;

extern struct platform_device msm8960_cache_dump_device;
extern struct platform_device apq8064_cache_dump_device;

extern struct platform_device mdm_sglte_device;

extern struct platform_device msm7x27_device_acpuclk;
extern struct platform_device msm7x27a_device_acpuclk;
extern struct platform_device msm7x27aa_device_acpuclk;
extern struct platform_device msm7x30_device_acpuclk;
extern struct platform_device apq8064_device_acpuclk;
extern struct platform_device msm8625_device_acpuclk;
extern struct platform_device msm8625ab_device_acpuclk;
extern struct platform_device msm8x50_device_acpuclk;
extern struct platform_device msm8x60_device_acpuclk;
extern struct platform_device msm8960_device_acpuclk;
extern struct platform_device msm8960ab_device_acpuclk;

extern struct platform_device apq8064_msm_mpd_device;

extern struct platform_device msm_gpio_device;

extern struct platform_device amdm_8064_device;
extern struct platform_device bmdm_8064_device;

extern struct platform_device sglte_mdm_8064_device;
extern struct platform_device sglte2_qsc_8064_device;

extern struct platform_device ipq806x_device_ssbi_pmic1;
extern struct platform_device ipq806x_coresight_funnel_device;
extern struct platform_device ipq806x_device_acpuclk;
extern struct platform_device ipq806x_device_ehci_host3;
extern struct platform_device ipq806x_device_hsic_host;
extern struct platform_device ipq806x_device_qup_i2c_gsbi1;
extern struct platform_device ipq806x_device_qup_i2c_gsbi2;
extern struct platform_device ipq806x_device_qup_i2c_gsbi4;
extern struct platform_device ipq806x_device_qup_spi_gsbi5;
extern struct platform_device ipq806x_device_dmov;
extern struct platform_device msm_device_smd_ipq806x;
extern struct platform_device ipq806x_device_otg;
extern struct platform_device ipq806x_device_gadget_peripheral;
extern struct platform_device ipq806x_device_hsusb_host;
extern struct platform_device ipq806x_device_watchdog;
extern struct platform_device qcom_wdt_device;
extern struct platform_device ipq806x_device_rng;
extern struct platform_device ipq806x_rpm_device;
extern struct platform_device ipq806x_rpm_log_device;
extern struct platform_device ipq806x_rpm_stat_device;
extern struct platform_device ipq806x_rpm_master_stat_device;
extern struct platform_device ipq806x_rtb_device;
extern struct platform_device ipq806x_msm_gov_device;
extern struct platform_device ipq806x_device_cache_erp;
extern struct platform_device ipq806x_device_sata;
extern struct platform_device ipq806x_device_ebi1_ch0_erp;
extern struct platform_device ipq806x_cache_dump_device;
extern struct platform_device ipq806x_msm_mpd_device;
extern struct platform_device ipq806x_device_uart_gsbi2;
extern struct platform_device ipq806x_device_uart_gsbi4;
extern struct platform_device ipq806x_device_uart_gsbi7;
extern struct platform_device msm_device_sps_ipq806x;
extern struct platform_device ipq806x_pc_cntr;
extern struct platform_device ipq806x_device_qup_i2c_gsbi5;
extern struct platform_device msm_bus_ipq806x_apps_fabric;
extern struct platform_device msm_bus_ipq806x_sys_fabric;
extern struct platform_device msm_bus_ipq806x_nss_fabric_0;
extern struct platform_device msm_bus_ipq806x_nss_fabric_1;
extern struct platform_device msm_bus_ipq806x_sys_fpb;
extern struct platform_device msm_bus_ipq806x_cpss_fpb;
extern struct platform_device ipq_device_tz_log;
extern struct platform_device ipq806x_device_nss0;
extern struct platform_device ipq806x_device_nss1;
extern struct platform_device nss_gmac_0;
extern struct platform_device nss_gmac_1;
extern struct platform_device nss_gmac_2;
extern struct platform_device nss_gmac_3;
extern struct platform_device *nss_gmac[];
extern struct platform_device nss_macsec1;
extern struct platform_device nss_macsec2;
extern struct platform_device nss_macsec3;
extern struct platform_device ipq806x_device_nss_crypto[];
extern struct platform_device ipq806x_mdio_device;
extern struct platform_device ap160_2xx_aq_phy;
extern struct platform_device ipq806x_mdio_device2;
extern struct platform_device ap160_2_qca_8511_sw;
extern struct platform_device ipq806x_lpass_dmlite;
extern struct platform_device ipq806x_lpass_cpudai;
extern struct platform_device ipq806x_lpass_lpaif;
extern struct platform_device ipq806x_lpass_clock;
extern struct platform_device ipq806x_lpass_spdif;
extern struct platform_device ipq806x_lpass_pcm_mi2s;
extern struct platform_device ipq806x_lpass_pcm_voip;
extern struct platform_device ipq806x_lpass_pcm_spdif;
extern struct platform_device ipq806x_mi2s_codec;
extern struct platform_device ipq806x_spdif_codec;
extern struct platform_device ipq806x_pcm_codec;
extern struct platform_device ipq806x_lpass_pcm_raw;
extern struct platform_device ipq806x_device_qup_spi_gsbi6;
extern struct platform_device ipq806x_device_qup_spi_gsbi2;
