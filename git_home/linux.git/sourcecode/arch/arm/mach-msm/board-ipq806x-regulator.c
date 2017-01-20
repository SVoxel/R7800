/* * Copyright (c) 2012-2015 The Linux Foundation. All rights reserved.* */
/*
 * Copyright (c) 2011-2012, Code Aurora Forum. All rights reserved.
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

#include <linux/regulator/ipq-regulator.h>

#include "board-ipq806x.h"

#define VREG_CONSUMERS(_id) \
	static struct regulator_consumer_supply vreg_consumers_##_id[]

#define REG_IPQ_REGISTER(_id, _name) \
	regulator_register_ipq_dummy(_id, vreg_consumers_##_name, ARRAY_SIZE(vreg_consumers_##_name), SMB208_FIXED_MINUV, SMB208_FIXED_MAXUV)

/*
 * Regulators that are present when using SMB PMIC
 */
VREG_CONSUMERS(S1a) = {
	REGULATOR_SUPPLY("smb208_s1a", NULL),
};

VREG_CONSUMERS(S1a_ap148) = {
	REGULATOR_SUPPLY("smb208_s1a", NULL),
	REGULATOR_SUPPLY("VDD_UBI0", "qca-nss.0"),
	REGULATOR_SUPPLY("VDD_UBI1", "qca-nss.1"),
};

VREG_CONSUMERS(S1b) = {
	REGULATOR_SUPPLY("smb208_s1b", NULL),
	REGULATOR_SUPPLY("VDD_UBI0", "qca-nss.0"),
	REGULATOR_SUPPLY("VDD_UBI1", "qca-nss.1"),

};

VREG_CONSUMERS(S1b_ap148) = {
	REGULATOR_SUPPLY("smb208_s1b", NULL),
};

VREG_CONSUMERS(S2a) = {
	REGULATOR_SUPPLY("smb208_s2a", NULL),
	REGULATOR_SUPPLY("krait0", "acpuclk-ipq806x"),
};

VREG_CONSUMERS(S2b) = {
	REGULATOR_SUPPLY("smb208_s2b", NULL),
	REGULATOR_SUPPLY("krait1", "acpuclk-ipq806x"),
};

VREG_CONSUMERS(fixed_sdc_vdd_io) = {
	REGULATOR_SUPPLY("ipq_fixed_regulator1", NULL),
	REGULATOR_SUPPLY("sdc_vdd_io", "msm_sdcc.1"),
};

VREG_CONSUMERS(fixed_sdc3_vdd_io) = {
	REGULATOR_SUPPLY("ipq_fixed_regulator2", NULL),
	REGULATOR_SUPPLY("sdc_vdd_io", "msm_sdcc.3"),
};

VREG_CONSUMERS(fixed_sata_ext) = {
	REGULATOR_SUPPLY("ipq_fixed_regulator3", NULL),
	REGULATOR_SUPPLY("sata_ext_3p3v", "msm_sata.0"),
};

VREG_CONSUMERS(fixed_sata_pmp) = {
	REGULATOR_SUPPLY("ipq_fixed_regulator4", NULL),
	REGULATOR_SUPPLY("sata_pmp_pwr", "msm_sata.0"),
};

VREG_CONSUMERS(fixed_sdc_vdd) = {
	REGULATOR_SUPPLY("ipq_fixed_regulator5", NULL),
	REGULATOR_SUPPLY("sdc_vdd", "msm_sdcc.1"),
};

VREG_CONSUMERS(fixed_sdc3_vdd) = {
	REGULATOR_SUPPLY("ipq_fixed_regulator6", NULL),
	REGULATOR_SUPPLY("sdc_vdd", "msm_sdcc.3"),
};

VREG_CONSUMERS(fixed_pcie_ext) = {
	REGULATOR_SUPPLY("ipq_fixed_regulator7", NULL),
	REGULATOR_SUPPLY("pcie_ext_3p3v", "msm_pcie.0"),
	REGULATOR_SUPPLY("pcie_ext_3p3v", "msm_pcie.1"),
	REGULATOR_SUPPLY("pcie_ext_3p3v", "msm_pcie.2"),
};

VREG_CONSUMERS(fixed_pcie_vp) = {
	REGULATOR_SUPPLY("ipq_fixed_regulator8", NULL),
	REGULATOR_SUPPLY("vp_pcie", "msm_pcie.0"),
	REGULATOR_SUPPLY("vptx_pcie", "msm_pcie.0"),
	REGULATOR_SUPPLY("vp_pcie", "msm_pcie.1"),
	REGULATOR_SUPPLY("vptx_pcie", "msm_pcie.1"),
	REGULATOR_SUPPLY("vp_pcie", "msm_pcie.2"),
	REGULATOR_SUPPLY("vptx_pcie", "msm_pcie.2"),
};

VREG_CONSUMERS(fixed_pcie_vph) = {
	REGULATOR_SUPPLY("ipq_fixed_regulator9", NULL),
	REGULATOR_SUPPLY("vdd_pcie_vph", "msm_pcie.0"),
	REGULATOR_SUPPLY("vdd_pcie_vph", "msm_pcie.1"),
	REGULATOR_SUPPLY("vdd_pcie_vph", "msm_pcie.2"),
};

/*
 * TODO - Not all members are required for SMB based regulators.
 * Can be optimized
 */
#define RPM_SMB_INIT(_id, _min_uV, _max_uV, _modes, _ops, _apply_uV, _default_uV, \
		 _peak_uA, _avg_uA, _pull_down, _pin_ctrl, _freq, _pin_fn, \
		 _force_mode, _sleep_set_force_mode, _power_mode, _state, \
		 _sleep_selectable, _always_on, _supply_regulator, _system_uA) \
	{ \
		.init_data = { \
			.constraints = { \
				.valid_modes_mask	= _modes, \
				.valid_ops_mask		= _ops, \
				.min_uV			= _min_uV, \
				.max_uV			= _max_uV, \
				.input_uV		= _min_uV, \
				.apply_uV		= _apply_uV, \
				.always_on		= _always_on, \
			}, \
			.num_consumer_supplies	= \
					ARRAY_SIZE(vreg_consumers_##_id), \
			.consumer_supplies	= vreg_consumers_##_id, \
			.supply_regulator	= _supply_regulator, \
		}, \
		.id			=  RPM_VREG_ID_SMB208_##_id, \
		.default_uV		= _default_uV, \
		.peak_uA		= _peak_uA, \
		.avg_uA			= _avg_uA, \
		.pull_down_enable	= _pull_down, \
		.pin_ctrl		= _pin_ctrl, \
		.freq			= RPM_VREG_FREQ_##_freq, \
		.pin_fn			= _pin_fn, \
		.force_mode		= _force_mode, \
		.sleep_set_force_mode	= _sleep_set_force_mode, \
		.power_mode		= _power_mode, \
		.state			= _state, \
		.sleep_selectable	= _sleep_selectable, \
		.system_uA		= _system_uA, \
	}

#define RPM_SMB_SMPS(_id, _always_on, _pd, _sleep_selectable, _min_uV, _max_uV, \
		 _supply_regulator, _system_uA, _freq, _force_mode, \
		 _sleep_set_force_mode) \
	RPM_SMB_INIT(_id, _min_uV, _max_uV, REGULATOR_MODE_NORMAL \
		 | REGULATOR_MODE_IDLE, REGULATOR_CHANGE_VOLTAGE \
		 | REGULATOR_CHANGE_STATUS | REGULATOR_CHANGE_MODE \
		 | REGULATOR_CHANGE_DRMS, 0, _max_uV, _system_uA, 0, _pd, \
		 RPM_VREG_PIN_CTRL_NONE, _freq, RPM_VREG_PIN_FN_IPQ806X_NONE, \
		 RPM_VREG_FORCE_MODE_IPQ806X_##_force_mode, \
		 RPM_VREG_FORCE_MODE_IPQ806X_##_sleep_set_force_mode, \
		 RPM_VREG_POWER_MODE_IPQ806X_PWM, RPM_VREG_STATE_OFF, \
		 _sleep_selectable, _always_on, _supply_regulator, _system_uA)

static struct rpm_regulator_init_data
ipq806x_rpm_regulator_smb_init_data[] __devinitdata = {
	/*       ID a_on pd ss min_uV   max_uV  supply sys_uA  freq  fm  ss_fm */

	RPM_SMB_SMPS(S1a, 0, 1, 1, 1050000, 1150000, NULL, 1200000,
							0p50, NONE, NONE),
	RPM_SMB_SMPS(S1b, 0, 1, 1, 1050000, 1150000, NULL, 1679000,
							0p50, NONE, NONE),
	RPM_SMB_SMPS(S2a, 0, 1, 1, 800000, 1275000, NULL, 1740000,
							0p50, NONE, NONE),
	RPM_SMB_SMPS(S2b, 0, 1, 1, 800000, 1275000, NULL, 1740000,
							0p50, NONE, NONE),
};

#define RPM_SMB_REG_MAP(_id, _voter, _supply, _dev_name) \
{ \
	.vreg_id = RPM_VREG_ID_SMB208_##_id, \
	.sleep_also = 0, \
	.voter = _voter, \
	.supply = _supply, \
	.dev_name = _dev_name, \
}

static struct rpm_regulator_consumer_mapping
msm_rpm_regulator_smb_tb732_consumer_mapping[] __devinitdata = {
	/* SMB208_S1a   - VDD_UBI */
	RPM_SMB_REG_MAP(S1a, RPM_VREG_VOTER1, "VDD_UBI0",       "nss-drv"),
	RPM_SMB_REG_MAP(S1a, RPM_VREG_VOTER2, "VDD_UBI1",       "nss-drv"),

	/* SMB208_S1b HFPLLs , VDD_CX */
	RPM_SMB_REG_MAP(S1b,  RPM_VREG_VOTER1, "krait0_hfpll", "acpuclk-ipq806x"),
	RPM_SMB_REG_MAP(S1b,  RPM_VREG_VOTER2, "krait1_hfpll", "acpuclk-ipq806x"),
	RPM_SMB_REG_MAP(S1b,  RPM_VREG_VOTER4, "l2_hfpll",     "acpuclk-ipq806x"),
	RPM_SMB_REG_MAP(S1b,  RPM_VREG_VOTER5, "krait0_mem",   "acpuclk-ipq806x"),
	RPM_SMB_REG_MAP(S1b,  RPM_VREG_VOTER6, "krait1_mem",   "acpuclk-ipq806x"),
	RPM_SMB_REG_MAP(S1b,  RPM_VREG_VOTER7, "krait0_dig",   "acpuclk-ipq806x"),
	RPM_SMB_REG_MAP(S1b,  RPM_VREG_VOTER8, "krait1_dig",   "acpuclk-ipq806x"),
};

static struct rpm_regulator_consumer_mapping
msm_rpm_regulator_smb_db14x_consumer_mapping[] __devinitdata = {

	/* SMB208_S1b  - HFPLLs , VDD_CX,VDD_CDC_SDCx */
	RPM_SMB_REG_MAP(S1a,  RPM_VREG_VOTER1, "krait0_hfpll", "acpuclk-ipq806x"),
	RPM_SMB_REG_MAP(S1a,  RPM_VREG_VOTER2, "krait1_hfpll", "acpuclk-ipq806x"),
	RPM_SMB_REG_MAP(S1a,  RPM_VREG_VOTER4, "l2_hfpll",     "acpuclk-ipq806x"),
	RPM_SMB_REG_MAP(S1a,  RPM_VREG_VOTER5, "krait0_mem",   "acpuclk-ipq806x"),
	RPM_SMB_REG_MAP(S1a,  RPM_VREG_VOTER6, "krait1_mem",   "acpuclk-ipq806x"),
	RPM_SMB_REG_MAP(S1a,  RPM_VREG_VOTER7, "krait0_dig",   "acpuclk-ipq806x"),
	RPM_SMB_REG_MAP(S1a,  RPM_VREG_VOTER8, "krait1_dig",   "acpuclk-ipq806x"),

	/* SMB208_S1a   - VDD_UBI */
	RPM_SMB_REG_MAP(S1b, RPM_VREG_VOTER1, "VDD_UBI0",       "qca-nss.0"),
	RPM_SMB_REG_MAP(S1b, RPM_VREG_VOTER2, "VDD_UBI1",       "qca-nss.1"),

};

static struct rpm_regulator_consumer_mapping
msm_rpm_regulator_smb_ap148_consumer_mapping[] __devinitdata = {

	/* SMB208_S1b  - HFPLLs , VDD_CX,VDD_CDC_SDCx */
	RPM_SMB_REG_MAP(S1a,  RPM_VREG_VOTER1, "krait0_hfpll", "acpuclk-ipq806x"),
	RPM_SMB_REG_MAP(S1a,  RPM_VREG_VOTER2, "krait1_hfpll", "acpuclk-ipq806x"),
	RPM_SMB_REG_MAP(S1a,  RPM_VREG_VOTER4, "l2_hfpll",     "acpuclk-ipq806x"),
	RPM_SMB_REG_MAP(S1a,  RPM_VREG_VOTER5, "krait0_mem",   "acpuclk-ipq806x"),
	RPM_SMB_REG_MAP(S1a,  RPM_VREG_VOTER6, "krait1_mem",   "acpuclk-ipq806x"),
	RPM_SMB_REG_MAP(S1a,  RPM_VREG_VOTER7, "krait0_dig",   "acpuclk-ipq806x"),
	RPM_SMB_REG_MAP(S1a,  RPM_VREG_VOTER8, "krait1_dig",   "acpuclk-ipq806x"),

	RPM_SMB_REG_MAP(S1a, RPM_VREG_VOTER11, "VDD_CX",        NULL),
	RPM_SMB_REG_MAP(S1a, RPM_VREG_VOTER12, "VCC_CDC_SDCx",  NULL),

	/* SMB208_S1a   - VDD_UBI */
	RPM_SMB_REG_MAP(S1a, RPM_VREG_VOTER1, "VDD_UBI0",       "qca-nss.0"),
	RPM_SMB_REG_MAP(S1a, RPM_VREG_VOTER2, "VDD_UBI1",       "qca-nss.1"),

};

static struct rpm_regulator_consumer_mapping
msm_rpm_regulator_smb_rumi3_consumer_mapping[] __devinitdata = {
	/*
	 * These will be tested using sysfs using command line interface
	 * and dont need to be mapped to any consumers
	 */
	RPM_SMB_REG_MAP(S1a, RPM_VREG_VOTER1, "VDD_SMB_S1a",       NULL),
	RPM_SMB_REG_MAP(S1b, RPM_VREG_VOTER9, "VDD_SMB_S1b",       NULL),

	/* SMB208_S1a   - VDD_UBI */
	RPM_SMB_REG_MAP(S1a, RPM_VREG_VOTER2, "VDD_UBI0",       "nss-drv"),
	RPM_SMB_REG_MAP(S1a, RPM_VREG_VOTER3, "VDD_UBI1",       "nss-drv"),

	/* SMB208_S1b  - VDD_CX,VDD_CDC_SDCx */
	RPM_SMB_REG_MAP(S1b,  RPM_VREG_VOTER1, "krait0_hfpll", "acpuclk-ipq806x"),
	RPM_SMB_REG_MAP(S1b,  RPM_VREG_VOTER2, "krait1_hfpll", "acpuclk-ipq806x"),
	RPM_SMB_REG_MAP(S1b,  RPM_VREG_VOTER4, "l2_hfpll",     "acpuclk-ipq806x"),
	RPM_SMB_REG_MAP(S1b,  RPM_VREG_VOTER5, "krait0_mem",   "acpuclk-ipq806x"),
	RPM_SMB_REG_MAP(S1b,  RPM_VREG_VOTER6, "krait1_mem",   "acpuclk-ipq806x"),
	RPM_SMB_REG_MAP(S1b,  RPM_VREG_VOTER7, "krait0_dig",   "acpuclk-ipq806x"),
	RPM_SMB_REG_MAP(S1b,  RPM_VREG_VOTER8, "krait1_dig",   "acpuclk-ipq806x"),

};

/* Regulators that are only present when using SMB208 */
struct rpm_regulator_platform_data
ipq806x_rpm_regulator_smb_pdata __devinitdata = {
	.init_data = ipq806x_rpm_regulator_smb_init_data,
	.num_regulators = ARRAY_SIZE(ipq806x_rpm_regulator_smb_init_data),
	.version = RPM_VREG_VERSION_IPQ806X,
	.vreg_id_vdd_mem = RPM_VREG_ID_SMB208_S1b,
	.vreg_id_vdd_dig = RPM_VREG_ID_SMB208_S1b,
	.requires_tcxo_workaround = false,
	/*
	 * Consumer map is board-dependent , and is initialized in board-specific
	 * fixup function
	 */
	.consumer_map = NULL,
	.consumer_map_len = 0,
};

/*
 *  Board specific configuration for SMB based designs
 *  1) Regulator consumer mapping
 *  2) Fixed regulator configuration
 */
void __init fixup_ipq806x_smb_power_grid(void)
{
	int i;
	static struct rpm_regulator_init_data *rpm_data;

	/*
	 * For RUMI, though the voltage rails are always fixed, we could test
	 * the voltage scaling using SMB test board, and observing the voltages
	 * on the pins.
	 */
	if (machine_is_ipq806x_rumi3()) {
		ipq806x_rpm_regulator_smb_pdata.consumer_map =
			msm_rpm_regulator_smb_rumi3_consumer_mapping;
		ipq806x_rpm_regulator_smb_pdata.consumer_map_len =
			ARRAY_SIZE(msm_rpm_regulator_smb_rumi3_consumer_mapping);
	} else if (machine_is_ipq806x_tb726()) {
		ipq806x_rpm_regulator_smb_pdata.consumer_map =
			msm_rpm_regulator_smb_tb732_consumer_mapping;
		ipq806x_rpm_regulator_smb_pdata.consumer_map_len =
			ARRAY_SIZE(msm_rpm_regulator_smb_tb732_consumer_mapping);
	} else if (machine_is_ipq806x_db149() ||
		   machine_is_ipq806x_db149_1xx() ||
		   machine_is_ipq806x_db149_2xx() ||
		   machine_is_ipq806x_db147()) {
		ipq806x_rpm_regulator_smb_pdata.consumer_map =
			msm_rpm_regulator_smb_db14x_consumer_mapping;
		ipq806x_rpm_regulator_smb_pdata.consumer_map_len =
			ARRAY_SIZE(msm_rpm_regulator_smb_db14x_consumer_mapping);
	} else if (machine_is_ipq806x_ap148() ||
			machine_is_ipq806x_ap145() ||
			machine_is_ipq806x_ap145_1xx() ||
			machine_is_ipq806x_ap148_1xx() ||
			machine_is_ipq806x_ap160() ||
			machine_is_ipq806x_ap160_2xx() ||
			machine_is_ipq806x_ap161() ||
			machine_is_ipq806x_ak01_1xx()) {
		ipq806x_rpm_regulator_smb_pdata.consumer_map =
			msm_rpm_regulator_smb_ap148_consumer_mapping;
		ipq806x_rpm_regulator_smb_pdata.consumer_map_len =
			ARRAY_SIZE(msm_rpm_regulator_smb_ap148_consumer_mapping);

		/* In AP148, VDD_UBI also maps to S1a regulator */

		rpm_data = &ipq806x_rpm_regulator_smb_init_data[0];
		rpm_data->init_data.num_consumer_supplies = ARRAY_SIZE(vreg_consumers_S1a_ap148);
		rpm_data->init_data.consumer_supplies = vreg_consumers_S1a_ap148;

		rpm_data = &ipq806x_rpm_regulator_smb_init_data[1];
		rpm_data->init_data.num_consumer_supplies = ARRAY_SIZE(vreg_consumers_S1b_ap148);
		rpm_data->init_data.consumer_supplies = vreg_consumers_S1b_ap148;

		if (machine_is_ipq806x_ap145()|| machine_is_ipq806x_ap145_1xx()) {
			rpm_vreg_regulator_set_fixed();

			for (i = 0; i < ARRAY_SIZE(ipq806x_rpm_regulator_smb_init_data); i++) {

				rpm_data = &ipq806x_rpm_regulator_smb_init_data[i];

				/* All regulators are at fixed voltage in AP145
				 * We will not change the min_uV and let the voltage requests
				 * between min_uV and max_uV from consumers, end gracefully.
				 */
				rpm_data->init_data.constraints.min_uV = SMB208_FIXED_MINUV;
				rpm_data->init_data.constraints.max_uV = SMB208_FIXED_MAXUV;
			}
		}

	}

	/*
	 * Register all dummy fixed regulators
	 */
	REG_IPQ_REGISTER(0, fixed_pcie_ext);
	REG_IPQ_REGISTER(1, fixed_pcie_vp);
	REG_IPQ_REGISTER(2, fixed_pcie_vph);
	REG_IPQ_REGISTER(3, fixed_sdc_vdd_io);
	REG_IPQ_REGISTER(4, fixed_sdc3_vdd_io);
	REG_IPQ_REGISTER(5, fixed_sdc_vdd);
	REG_IPQ_REGISTER(6, fixed_sdc3_vdd);
	REG_IPQ_REGISTER(7, fixed_sata_ext);
	REG_IPQ_REGISTER(8, fixed_sata_pmp);
}
