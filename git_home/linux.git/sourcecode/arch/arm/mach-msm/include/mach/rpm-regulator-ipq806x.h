/* * Copyright (c) 2012 The Linux Foundation. All rights reserved.* */
/*
 * Copyright (c) 2011, Code Aurora Forum. All rights reserved.
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

#ifndef __ARCH_ARM_MACH_MSM_INCLUDE_MACH_RPM_REGULATOR_IPQ806X_H
#define __ARCH_ARM_MACH_MSM_INCLUDE_MACH_RPM_REGULATOR_IPQ806X_H

/**
 * enum rpm_vreg_pin_fn_ipq806x - RPM regulator pin function choices
 * %RPM_VREG_PIN_FN_IPQ806X_DONT_CARE:	do not care about pin control state of
 *					the regulator; allow another master
 *					processor to specify pin control
 * %RPM_VREG_PIN_FN_IPQ806X_ENABLE:	pin control switches between disable and
 *					enable
 * %RPM_VREG_PIN_FN_IPQ806X_MODE:	pin control switches between LPM and HPM
 * %RPM_VREG_PIN_FN_IPQ806X_SLEEP_B:	regulator is forced into LPM when
 *					sleep_b signal is asserted
 * %RPM_VREG_PIN_FN_IPQ806X_NONE:	do not use pin control for the regulator
 *					and do not allow another master to
 *					request pin control
 *
 * The pin function specified in platform data corresponds to the active state
 * pin function value.  Pin function will be NONE until a consumer requests
 * pin control to be enabled.
 */
enum rpm_vreg_pin_fn_IPQ806X {
	RPM_VREG_PIN_FN_IPQ806X_DONT_CARE,
	RPM_VREG_PIN_FN_IPQ806X_ENABLE,
	RPM_VREG_PIN_FN_IPQ806X_MODE,
	RPM_VREG_PIN_FN_IPQ806X_SLEEP_B,
	RPM_VREG_PIN_FN_IPQ806X_NONE,
};

/**
 * enum rpm_vreg_force_mode_ipq806x - RPM regulator force mode choices
 * %RPM_VREG_FORCE_MODE_IPQ806X_PIN_CTRL:	allow pin control usage
 * %RPM_VREG_FORCE_MODE_IPQ806X_NONE:	do not force any mode
 * %RPM_VREG_FORCE_MODE_IPQ806X_LPM:	force into low power mode
 * %RPM_VREG_FORCE_MODE_IPQ806X_AUTO:	allow regulator to automatically select
 *					its own mode based on realtime current
 *					draw (only available for SMPS
 *					regulators)
 * %RPM_VREG_FORCE_MODE_IPQ806X_HPM:	force into high power mode
 * %RPM_VREG_FORCE_MODE_IPQ806X_BYPASS:	set regulator to use bypass mode, i.e.
 *					to act as a switch and not regulate
 *					(only available for LDO regulators)
 *
 * Force mode is used to override aggregation with other masters and to set
 * special operating modes.
 */
enum rpm_vreg_force_mode_ipq806x {
	RPM_VREG_FORCE_MODE_IPQ806X_PIN_CTRL = 0,
	RPM_VREG_FORCE_MODE_IPQ806X_NONE = 0,
	RPM_VREG_FORCE_MODE_IPQ806X_LPM,
	RPM_VREG_FORCE_MODE_IPQ806X_AUTO,		/* SMPS only */
	RPM_VREG_FORCE_MODE_IPQ806X_HPM,
	RPM_VREG_FORCE_MODE_IPQ806X_BYPASS,	/* LDO only */
};

/**
 * enum rpm_vreg_power_mode_ipq806x - power mode for SMPS regulators
 * %RPM_VREG_POWER_MODE_IPQ806X_HYSTERETIC: Use hysteretic mode for HPM and when
 *					 usage goes high in AUTO
 * %RPM_VREG_POWER_MODE_IPQ806X_PWM:	 Use PWM mode for HPM and when usage
 *					 goes high in AUTO
 */
enum rpm_vreg_power_mode_ipq806x {
	RPM_VREG_POWER_MODE_IPQ806X_HYSTERETIC,
	RPM_VREG_POWER_MODE_IPQ806X_PWM,
};

/**
 * enum rpm_vreg_id - RPM regulator ID numbers (both real and pin control)
 */
enum rpm_vreg_id_ipq806x {
	RPM_VREG_ID_SMB208_S1a,
	RPM_VREG_ID_SMB208_S1b,
	RPM_VREG_ID_SMB208_S2a,
	RPM_VREG_ID_SMB208_S2b,
	RPM_VREG_ID_SMB208_S3a,
	RPM_VREG_ID_SMB208_S3b,
	RPM_VREG_ID_SMB208_S4,
	RPM_VREG_ID_SMB208_S5,
	RPM_VREG_ID_SMB208_S6a,
	RPM_VREG_ID_SMB208_S6b,
};

#endif
