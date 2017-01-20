/* * Copyright (c) 2012 The Linux Foundation. All rights reserved.* */
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

#define pr_fmt(fmt) "%s: " fmt, __func__

#include "rpm-regulator-private.h"

/* RPM regulator request formats */

/* TODO - Not all members are required for SMB based regulators.
 * Can be optimized */
static struct rpm_vreg_parts smps_parts = {
	.request_len	= 2,
	.uV		= REQUEST_MEMBER(0, 0x007FFFFF,  0),
	.pd		= REQUEST_MEMBER(0, 0x00800000, 23),
	.pc		= REQUEST_MEMBER(0, 0x0F000000, 24),
	.pf		= REQUEST_MEMBER(0, 0xF0000000, 28),
	.ip		= REQUEST_MEMBER(1, 0x000003FF,  0),
	.ia		= REQUEST_MEMBER(1, 0x000FFC00, 10),
	.fm		= REQUEST_MEMBER(1, 0x00700000, 20),
	.pm		= REQUEST_MEMBER(1, 0x00800000, 23),
	.freq		= REQUEST_MEMBER(1, 0x1F000000, 24),
	.freq_clk_src	= REQUEST_MEMBER(1, 0x60000000, 29),
};

static struct vreg_range smps_ranges[] = {
	VOLTAGE_RANGE( 375000,  737500, 12500),
	VOLTAGE_RANGE( 750000, 1487500, 12500),
	VOLTAGE_RANGE(1500000, 3075000, 25000),
	VOLTAGE_RANGE(3100000, 5100000, 25000),
};

static struct vreg_set_points smps_set_points = SET_POINTS(smps_ranges);

static struct vreg_set_points *all_set_points[] = {
	&smps_set_points,
};

#define SMPS_SMB(_id, _name, _name_pc, _ranges, _hpm_min_load) \
	[RPM_VREG_ID_SMB208_##_id] = { \
		.req = { \
			[0] = { .id = MSM_RPM_ID_SMB208_##_id##_0, }, \
			[1] = { .id = MSM_RPM_ID_SMB208_##_id##_1, }, \
		}, \
		.hpm_min_load  = RPM_VREG_8960_##_hpm_min_load##_HPM_MIN_LOAD, \
		.type		 = RPM_REGULATOR_TYPE_SMPS, \
		.set_points	 = &_ranges##_set_points, \
		.part		 = &smps_parts, \
		.id		 = RPM_VREG_ID_SMB208_##_id, \
		.rdesc.name	 = _name, \
		.rdesc_pc.name	 = _name_pc, \
	}

#define SMPS_SMB_FIXED(_id, _name, _name_pc, _ranges, _hpm_min_load) \
	[RPM_VREG_ID_SMB208_##_id] = { \
		.req = { \
			[0] = { .id = MSM_RPM_ID_SMB208_##_id##_0, }, \
			[1] = { .id = MSM_RPM_ID_SMB208_##_id##_1, }, \
		}, \
		.hpm_min_load  = RPM_VREG_8960_##_hpm_min_load##_HPM_MIN_LOAD, \
		.type		 = RPM_REGULATOR_TYPE_SMPS_FIXED, \
		.set_points	 = &_ranges##_set_points, \
		.part		 = &smps_parts, \
		.id		 = RPM_VREG_ID_SMB208_##_id, \
		.rdesc.name	 = _name, \
		.rdesc_pc.name	 = _name_pc, \
	}

static struct vreg vregs[] = {
#ifdef CONFIG_MSM_SMB_FIXED_VOLTAGE
	SMPS_SMB_FIXED(S1a, "smb208_s1a", NULL, smps, SMPS_1500),
	SMPS_SMB_FIXED(S1b, "smb208_s1b", NULL, smps, SMPS_1500),
	SMPS_SMB_FIXED(S2a, "smb208_s2a", NULL, smps, SMPS_1500),
	SMPS_SMB_FIXED(S2b, "smb208_s2b", NULL, smps, SMPS_1500),
#else
	SMPS_SMB(S1a, "smb208_s1a", NULL, smps, SMPS_1500),
	SMPS_SMB(S1b, "smb208_s1b", NULL, smps, SMPS_1500),
	SMPS_SMB(S2a, "smb208_s2a", NULL, smps, SMPS_1500),
	SMPS_SMB(S2b, "smb208_s2b", NULL, smps, SMPS_1500),
#endif
	/*
	 * These regulators are always fixed
	 */
	SMPS_SMB_FIXED(S3a, "smb208_s3a", NULL, smps, SMPS_1500),
	SMPS_SMB_FIXED(S3b, "smb208_s3b", NULL, smps, SMPS_1500),
	SMPS_SMB_FIXED(S4, "smb208_s4", NULL, smps, SMPS_1500),
	SMPS_SMB_FIXED(S5, "smb208_s5", NULL, smps, SMPS_1500),
	SMPS_SMB_FIXED(S6a, "smb208_s6a", NULL, smps, SMPS_1500),
	SMPS_SMB_FIXED(S6b, "smb208_s6b", NULL, smps, SMPS_1500),
};


static const char *pin_func_label[] = {
        [RPM_VREG_PIN_FN_IPQ806X_DONT_CARE]        = "don't care",
        [RPM_VREG_PIN_FN_IPQ806X_ENABLE]           = "on/off",
        [RPM_VREG_PIN_FN_IPQ806X_MODE]             = "HPM/LPM",
        [RPM_VREG_PIN_FN_IPQ806X_SLEEP_B]          = "sleep_b",
        [RPM_VREG_PIN_FN_IPQ806X_NONE]             = "none",
};

static const char *force_mode_label[] = {
        [RPM_VREG_FORCE_MODE_IPQ806X_NONE]         = "none",
        [RPM_VREG_FORCE_MODE_IPQ806X_LPM]          = "LPM",
        [RPM_VREG_FORCE_MODE_IPQ806X_AUTO]         = "auto",
        [RPM_VREG_FORCE_MODE_IPQ806X_HPM]          = "HPM",
        [RPM_VREG_FORCE_MODE_IPQ806X_BYPASS]       = "BYP",
};

static const char *power_mode_label[] = {
        [RPM_VREG_POWER_MODE_IPQ806X_HYSTERETIC]   = "HYS",
        [RPM_VREG_POWER_MODE_IPQ806X_PWM]          = "PWM",
};

static const char *pin_control_label[] = {
	" D1",
	" A0",
	" A1",
	" A2",
};
static int is_real_id(int id)
{
	return (id >= 0) && (id <= RPM_VREG_ID_PM8921_MAX_REAL);
}

static int pc_id_to_real_id(int id)
{
	int real_id;

	if (id >= RPM_VREG_ID_PM8921_L1_PC && id <= RPM_VREG_ID_PM8921_L12_PC)
		real_id = id - RPM_VREG_ID_PM8921_L1_PC;
	else if (id >= RPM_VREG_ID_PM8921_L14_PC
			&& id <= RPM_VREG_ID_PM8921_L23_PC)
		real_id = id - RPM_VREG_ID_PM8921_L14_PC
				+ RPM_VREG_ID_PM8921_L14;
	else if (id >= RPM_VREG_ID_PM8921_L29_PC
			&& id <= RPM_VREG_ID_PM8921_S4_PC)
		real_id = id - RPM_VREG_ID_PM8921_L29_PC
				+ RPM_VREG_ID_PM8921_L29;
	else if (id >= RPM_VREG_ID_PM8921_S7_PC
			&& id <= RPM_VREG_ID_PM8921_LVS1_PC)
		real_id = id - RPM_VREG_ID_PM8921_S7_PC + RPM_VREG_ID_PM8921_S7;
	else
		real_id = id - RPM_VREG_ID_PM8921_LVS3_PC
				+ RPM_VREG_ID_PM8921_LVS3;

	return real_id;
}

static struct vreg_config config = {
	.vregs			= vregs,
	.vregs_len		= ARRAY_SIZE(vregs),

	.vreg_id_min		= RPM_VREG_ID_SMB208_S1a,
	.vreg_id_max		= RPM_VREG_ID_SMB208_S6b,

	.pin_func_none		= RPM_VREG_PIN_FN_8960_NONE,
	.pin_func_sleep_b	= RPM_VREG_PIN_FN_8960_SLEEP_B,

	.mode_lpm		= REGULATOR_MODE_IDLE,
	.mode_hpm		= REGULATOR_MODE_NORMAL,

	.set_points		= all_set_points,
	.set_points_len		= ARRAY_SIZE(all_set_points),

	.label_pin_ctrl		= pin_control_label,
	.label_pin_ctrl_len	= ARRAY_SIZE(pin_control_label),
	.label_pin_func		= pin_func_label,
	.label_pin_func_len	= ARRAY_SIZE(pin_func_label),
	.label_force_mode	= force_mode_label,
	.label_force_mode_len	= ARRAY_SIZE(force_mode_label),
	.label_power_mode	= power_mode_label,
	.label_power_mode_len	= ARRAY_SIZE(power_mode_label),

	.is_real_id		= is_real_id,
	.pc_id_to_real_id	= pc_id_to_real_id,
};

struct vreg_config *get_config_ipq806x(void)
{
	return &config;
}
