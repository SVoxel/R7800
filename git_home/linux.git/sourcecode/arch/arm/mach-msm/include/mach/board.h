/* * Copyright (c) 2012 The Linux Foundation. All rights reserved.* */
/* arch/arm/mach-msm/include/mach/board.h
 *
 * Copyright (C) 2007 Google, Inc.
 * Copyright (c) 2008-2013, The Linux Foundation. All rights reserved.
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

#ifndef __ASM_ARCH_MSM_BOARD_H
#define __ASM_ARCH_MSM_BOARD_H

#include <linux/types.h>
#include <linux/input.h>
#include <linux/usb.h>
#include <linux/leds-pmic8058.h>
#include <linux/clkdev.h>
#include <linux/of_platform.h>
#include <linux/msm_ssbi.h>
#include <mach/msm_bus.h>

struct clk_lookup;

struct snd_endpoint {
	int id;
	const char *name;
};

struct msm_snd_endpoints {
	struct snd_endpoint *endpoints;
	unsigned num;
};

struct cad_endpoint {
	int id;
	const char *name;
	uint32_t capability;
};

struct msm_cad_endpoints {
	struct cad_endpoint *endpoints;
	unsigned num;
};

#define MSM_MAX_DEC_CNT 14
/* 7k target ADSP information */
/* Bit 23:0, for codec identification like mp3, wav etc *
 * Bit 27:24, for mode identification like tunnel, non tunnel*
 * bit 31:28, for operation support like DM, DMA */
enum msm_adspdec_concurrency {
	MSM_ADSP_CODEC_WAV = 0,
	MSM_ADSP_CODEC_ADPCM = 1,
	MSM_ADSP_CODEC_MP3 = 2,
	MSM_ADSP_CODEC_REALAUDIO = 3,
	MSM_ADSP_CODEC_WMA = 4,
	MSM_ADSP_CODEC_AAC = 5,
	MSM_ADSP_CODEC_RESERVED = 6,
	MSM_ADSP_CODEC_MIDI = 7,
	MSM_ADSP_CODEC_YADPCM = 8,
	MSM_ADSP_CODEC_QCELP = 9,
	MSM_ADSP_CODEC_AMRNB = 10,
	MSM_ADSP_CODEC_AMRWB = 11,
	MSM_ADSP_CODEC_EVRC = 12,
	MSM_ADSP_CODEC_WMAPRO = 13,
	MSM_ADSP_CODEC_AC3 = 23,
	MSM_ADSP_MODE_TUNNEL = 24,
	MSM_ADSP_MODE_NONTUNNEL = 25,
	MSM_ADSP_MODE_LP = 26,
	MSM_ADSP_OP_DMA = 28,
	MSM_ADSP_OP_DM = 29,
};

struct msm_adspdec_info {
	const char *module_name;
	unsigned module_queueid;
	int module_decid; /* objid */
	unsigned nr_codec_support;
};

/* Carries information about number codec
 * supported if same codec or different codecs
 */
struct dec_instance_table {
	uint8_t max_instances_same_dec;
	uint8_t max_instances_diff_dec;
};

struct msm_adspdec_database {
	unsigned num_dec;
	unsigned num_concurrency_support;
	unsigned int *dec_concurrency_table; /* Bit masked entry to *
					      *	represents codec, mode etc */
	struct msm_adspdec_info  *dec_info_list;
	struct dec_instance_table *dec_instance_list;
};

enum msm_mdp_hw_revision {
	MDP_REV_20 = 1,
	MDP_REV_22,
	MDP_REV_30,
	MDP_REV_303,
	MDP_REV_31,
	MDP_REV_40,
	MDP_REV_41,
	MDP_REV_42,
	MDP_REV_43,
	MDP_REV_44,
};

struct msm_panel_common_pdata {
	uintptr_t hw_revision_addr;
	int gpio;
	bool bl_lock;
	spinlock_t bl_spinlock;
	int (*backlight_level)(int level, int max, int min);
	int (*pmic_backlight)(int level);
	int (*rotate_panel)(void);
	int (*backlight) (int level, int mode);
	int (*panel_num)(void);
	void (*panel_config_gpio)(int);
	int (*vga_switch)(int select_vga);
	int *gpio_num;
	u32 mdp_max_clk;
	u32 mdp_max_bw;
	u32 mdp_bw_ab_factor;
	u32 mdp_bw_ib_factor;
#ifdef CONFIG_MSM_BUS_SCALING
	struct msm_bus_scale_pdata *mdp_bus_scale_table;
#endif
	int mdp_rev;
	u32 ov0_wb_size;  /* overlay0 writeback size */
	u32 ov1_wb_size;  /* overlay1 writeback size */
	u32 mem_hid;
	char cont_splash_enabled;
	u32 splash_screen_addr;
	u32 splash_screen_size;
	char mdp_iommu_split_domain;
};



struct lcdc_platform_data {
	int (*lcdc_gpio_config)(int on);
	int (*lcdc_power_save)(int);
	unsigned int (*lcdc_get_clk)(void);
#ifdef CONFIG_MSM_BUS_SCALING
	struct msm_bus_scale_pdata *bus_scale_table;
#endif
	int (*lvds_pixel_remap)(void);
};

struct tvenc_platform_data {
	int poll;
	int (*pm_vid_en)(int on);
#ifdef CONFIG_MSM_BUS_SCALING
	struct msm_bus_scale_pdata *bus_scale_table;
#endif
};

struct mddi_platform_data {
	int (*mddi_power_save)(int on);
	int (*mddi_sel_clk)(u32 *clk_rate);
	int (*mddi_client_power)(u32 client_id);
};

struct mipi_dsi_platform_data {
	int vsync_gpio;
	int (*dsi_power_save)(int on);
	int (*dsi_client_reset)(void);
	int (*get_lane_config)(void);
	char (*splash_is_enabled)(void);
	int target_type;
};

enum mipi_dsi_3d_ctrl {
	FPGA_EBI2_INTF,
	FPGA_SPI_INTF,
};

/* DSI PHY configuration */
struct mipi_dsi_phy_ctrl {
	uint32_t regulator[5];
	uint32_t timing[12];
	uint32_t ctrl[4];
	uint32_t strength[4];
	uint32_t pll[21];
};

struct mipi_dsi_panel_platform_data {
	int fpga_ctrl_mode;
	int fpga_3d_config_addr;
	int *gpio;
	struct mipi_dsi_phy_ctrl *phy_ctrl_settings;
	char dlane_swap;
	void (*dsi_pwm_cfg)(void);
	char enable_wled_bl_ctrl;
	void (*gpio_set_backlight)(int bl_level);
};

struct lvds_panel_platform_data {
	int *gpio;
};

struct msm_wfd_platform_data {
	char (*wfd_check_mdp_iommu_split)(void);
};

#define PANEL_NAME_MAX_LEN 50
struct msm_fb_platform_data {
	int (*detect_client)(const char *name);
	int mddi_prescan;
	unsigned char ext_resolution;
	int (*allow_set_offset)(void);
	char prim_panel_name[PANEL_NAME_MAX_LEN];
	char ext_panel_name[PANEL_NAME_MAX_LEN];
};

struct msm_hdmi_platform_data {
	int irq;
	int (*cable_detect)(int insert);
	int (*comm_power)(int on, int show);
	int (*enable_5v)(int on);
	int (*core_power)(int on, int show);
	int (*cec_power)(int on);
	int (*panel_power)(int on);
	int (*gpio_config)(int on);
	int (*init_irq)(void);
	bool (*check_hdcp_hw_support)(void);
	bool (*source)(void);
	bool is_mhl_enabled;
};

struct msm_mhl_platform_data {
	int irq;
	/* GPIO no. for mhl intr */
	uint32_t gpio_mhl_int;
	/* GPIO no. for mhl block reset */
	uint32_t gpio_mhl_reset;
	/*
	 * below gpios are specific to targets
	 * that have the integrated MHL soln.
	 */
	/* GPIO no. for mhl block power */
	uint32_t gpio_mhl_power;
	/* GPIO no. for hdmi-mhl mux */
	uint32_t gpio_hdmi_mhl_mux;
	bool mhl_enabled;
};

struct msm_i2c_platform_data {
	int clk_freq;
	uint32_t rmutex;
	const char *rsl_id;
	uint32_t pm_lat;
	int pri_clk;
	int pri_dat;
	int aux_clk;
	int aux_dat;
	int src_clk_rate;
	int use_gsbi_shared_mode;
	int keep_ahb_clk_on;
	void (*msm_i2c_config_gpio)(int iface, int config_type);
};

struct msm_i2c_ssbi_platform_data {
	const char *rsl_id;
	enum msm_ssbi_controller_type controller_type;
};

struct msm_vidc_platform_data {
	int memtype;
	u32 enable_ion;
	int disable_dmx;
	int disable_fullhd;
	u32 cp_enabled;
	u32 secure_wb_heap;
	u32 enable_sec_metadata;
#ifdef CONFIG_MSM_BUS_SCALING
	struct msm_bus_scale_pdata *vidc_bus_client_pdata;
#endif
	int cont_mode_dpb_count;
	int disable_turbo;
	unsigned long fw_addr;
};

struct vcap_platform_data {
	unsigned *gpios;
	int num_gpios;
	struct msm_bus_scale_pdata *bus_client_pdata;
};

#if defined(CONFIG_USB_PEHCI_HCD) || defined(CONFIG_USB_PEHCI_HCD_MODULE)
struct isp1763_platform_data {
	unsigned reset_gpio;
	int (*setup_gpio)(int enable);
};
#endif
/* common init routines for use by arch/arm/mach-msm/board-*.c */

void msm_add_devices(void);
void msm_map_common_io(void);
void msm_map_qsd8x50_io(void);
void msm_map_msm8x60_io(void);
void msm_map_msm8960_io(void);
void msm_map_apq8064_io(void);
void msm_map_ipq806x_io(void);
void msm_map_msm7x30_io(void);
void msm_map_fsm9xxx_io(void);
void msm_map_msm8625_io(void);
void msm_init_irq(void);
void vic_handle_irq(struct pt_regs *regs);
void msm_map_mpq8092_io(void);
void mpq8092_init_gpiomux(void);

struct mmc_platform_data;
int msm_add_sdcc(unsigned int controller,
		struct mmc_platform_data *plat);

void msm_pm_register_irqs(void);
struct msm_usb_host_platform_data;
int msm_add_host(unsigned int host,
		struct msm_usb_host_platform_data *plat);
#if defined(CONFIG_USB_FUNCTION_MSM_HSUSB) \
	|| defined(CONFIG_USB_MSM_72K_MODULE)
void msm_hsusb_set_vbus_state(int online);
#else
static inline void msm_hsusb_set_vbus_state(int online) {}
#endif

void msm_snddev_init(void);
void msm_snddev_init_timpani(void);
void msm_snddev_poweramp_on(void);
void msm_snddev_poweramp_off(void);
void msm_snddev_hsed_voltage_on(void);
void msm_snddev_hsed_voltage_off(void);
void msm_snddev_tx_route_config(void);
void msm_snddev_tx_route_deconfig(void);

extern unsigned int msm_shared_ram_phys; /* defined in arch/arm/mach-msm/io.c */

extern int wifi_board_data_read(loff_t from, size_t len, size_t *retlen,
		u_char *buf);
extern int wifi_board_data_backup_read(loff_t from, size_t len, size_t *retlen,
		u_char *buf);

extern unsigned int ipq_boot_flash_type;

enum {
        BOOT_NO_FLASH        = 0,
        BOOT_NOR_FLASH       = 1,
        BOOT_NAND_FLASH      = 2,
        BOOT_ONENAND_FLASH   = 3,
        BOOT_SDC_FLASH       = 4,
        BOOT_MMC_FLASH       = 5,
        BOOT_SPI_FLASH       = 6,
};

static inline int machine_is_ipq806x_emmc_boot(void)
{
	return ipq_boot_flash_type == BOOT_MMC_FLASH;

}
#endif
