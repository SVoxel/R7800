/*
 *  Atheros AR71XX/AR724X/AR913X Calibration data in NAND flash fixup
 *
 *  Copyright (c) 2012 The Linux Foundation. All rights reserved.
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 */

/* 0 is a valid value for an MTD offset. So we'll use -1 as an undefined value
 * That means that all the fields in the structure below have to be initialized
 * in the platform code when the fixup is used */
#define FIXUP_UNDEFINED		-1LL

struct ath79_caldata_fixup {
	char *name;		/* Name of the MTD dev to read from */
	loff_t pcie_caldata_addr[2];	/* Addr (in flash) of radios caldata */
	loff_t wmac_caldata_addr;
	loff_t mac_addr[2];	/* Addr (in flash) of mac addresses */
};

void __init ath79_mtd_caldata_fixup(struct ath79_caldata_fixup *);
