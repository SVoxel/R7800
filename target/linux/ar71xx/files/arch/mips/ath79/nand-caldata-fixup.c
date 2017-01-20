/*
 *  Atheros AR71XX/AR724X/AR913X Calibration data in NAND flash fixup
 *
 *  Copyright (c) 2012 The Linux Foundation. All rights reserved.
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 */

#include <linux/types.h>
#include <linux/ath9k_platform.h>
#include <linux/mtd/mtd.h>

#include "dev-ap9x-pci.h"
#include "dev-wmac.h"
#include "dev-eth.h"
#include "nand-caldata-fixup.h"

static struct ath79_caldata_fixup *ath79_caldata;

static void mtd_caldata_add(struct mtd_info *mtd)
{
	int i, ret;
	size_t retlen;
	struct ath9k_platform_data *wmac_pdata;
	struct ag71xx_platform_data *eth_pdata;

	/* If the device added doesn't match the name of the device containing
	 * caldata, then we do nothing; just return */
	if (strcmp(mtd->name, ath79_caldata->name))
		return;

	/* If the caldata PCIe address provided by the platform is non-null,
	 * then fetch the data from MTD and fill it into the platform
	 * structure */
	for (i = 0;
	     i < sizeof(ath79_caldata->pcie_caldata_addr) / sizeof(loff_t);
	     i++) {
		if (ath79_caldata->pcie_caldata_addr[i] == FIXUP_UNDEFINED)
			continue;

		wmac_pdata = ap9x_pci_get_wmac_data(i);

		ret =
		    mtd_read(mtd, ath79_caldata->pcie_caldata_addr[i],
			     sizeof(wmac_pdata->eeprom_data), &retlen,
			     (u_char *) wmac_pdata->eeprom_data);
		if (retlen != sizeof(wmac_pdata->eeprom_data) || ret < 0) {
			pr_err("%s: Error while reading %dB at offset 0x%08x\n",
			       mtd->name, sizeof(wmac_pdata->eeprom_data),
			       (u_int) ath79_caldata->pcie_caldata_addr[i]);
		}
	}

	/* Same here for WMAC caldata */
	if (ath79_caldata->wmac_caldata_addr != FIXUP_UNDEFINED) {
		wmac_pdata = ath79_get_wmac_data();

		ret =
		    mtd_read(mtd, ath79_caldata->wmac_caldata_addr,
			     sizeof(wmac_pdata->eeprom_data), &retlen,
			     (u_char *) wmac_pdata->eeprom_data);
		if (retlen != sizeof(wmac_pdata->eeprom_data) || ret < 0) {
			pr_err("%s: Error while reading %dB at offset 0x%08x\n",
			       mtd->name, sizeof(wmac_pdata->eeprom_data),
			       (u_int) ath79_caldata->wmac_caldata_addr);
		}
	}

	/* Same here for Ethernet MAC@ */
	for (i = 0; i < sizeof(ath79_caldata->mac_addr) / sizeof(loff_t); i++) {
		switch (i) {
		case 0:
			eth_pdata = &ath79_eth0_data;
			break;
		case 1:
			eth_pdata = &ath79_eth1_data;
			break;
		default:
			eth_pdata = NULL;
		}

		if (ath79_caldata->mac_addr[i] == FIXUP_UNDEFINED)
			continue;

		ret =
		    mtd_read(mtd, ath79_caldata->mac_addr[i],
			     sizeof(eth_pdata->mac_addr), &retlen,
			     (u_char *) eth_pdata->mac_addr);
		if (retlen != sizeof(eth_pdata->mac_addr) || ret < 0) {
			pr_err("%s: Error while reading %dB at offset 0x%08x\n",
			       mtd->name, sizeof(eth_pdata->mac_addr),
			       (u_int) ath79_caldata->mac_addr[i]);
		}
	}
}

static void mtd_caldata_remove(struct mtd_info *mtd)
{
	return;
}

static void add_mtd_caldata_notifier(void)
{
	static struct mtd_notifier not = {
		.add = mtd_caldata_add,
		.remove = mtd_caldata_remove,
	};

	register_mtd_user(&not);
}

void __init ath79_mtd_caldata_fixup(struct ath79_caldata_fixup
				    *platform_caldata)
{
	ath79_caldata = platform_caldata;
	add_mtd_caldata_notifier();
}
