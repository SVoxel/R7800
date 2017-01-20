/*
 * Qualcomm reference boards support: REH132 DB120 APH126 APH128 APH131
 *
 * Copyright (c) 2013 The Linux Foundation. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

#include <linux/pci.h>
#include <linux/phy.h>
#include <linux/platform_device.h>
#include <linux/ath9k_platform.h>
#include <linux/ar8216_platform.h>
#include <linux/of_platform.h>

#include <asm/mach-ath79/ar71xx_regs.h>
#include <asm/mach-ath79/ath79.h>

#include "common.h"
#include "dev-ap9x-pci.h"
#include "dev-eth.h"
#include "dev-gpio-buttons.h"
#include "dev-leds-gpio.h"
#include "dev-m25p80.h"
#include "dev-spi.h"
#include "dev-usb.h"
#include "dev-wmac.h"
#include "dev-nfc.h"
#include "machtypes.h"

#define DT_MAC0_OFFSET		0
#define DT_MAC1_OFFSET		6
#define DT_WMAC_CALDATA_OFFSET	0x1000
#define DT_PCIE_CALDATA_OFFSET	0x5000

static struct ath9k_platform_data generic_ath_pci_data;

static struct of_device_id __initdata generic_common_ids[] = {
	{ .compatible = "simple-bus", },
	{},
};

struct of_dev_auxdata generic_auxdata_lookup[] __initdata = {
	OF_DEV_AUXDATA("qcom,ag71xx-wmac", AR934X_WMAC_BASE, 	"ar934x_wmac", 		&ath79_wmac_data),
	OF_DEV_AUXDATA("qcom,ag71xx-spi",  AR71XX_SPI_BASE,  	"ath79-spi",   		&ath79_spi_data),
	OF_DEV_AUXDATA("qcom,ag71xx-mdio", AR71XX_GE0_BASE,  	"ag71xx-mdio.0", 	&ath79_mdio0_data),
	OF_DEV_AUXDATA("qcom,ag71xx-mdio", AR71XX_GE1_BASE,  	"ag71xx-mdio.1", 	&ath79_mdio1_data),
	OF_DEV_AUXDATA("qcom,ag71xx-eth",  AR71XX_GE0_BASE,  	"ag71xx.0",    		&ath79_eth0_data),
	OF_DEV_AUXDATA("qcom,ag71xx-eth",  AR71XX_GE1_BASE,  	"ag71xx.1",    		&ath79_eth1_data),
	OF_DEV_AUXDATA("qcom,ag71xx-ehci", AR71XX_EHCI_BASE, 	"ehci-platform",  	&ath79_ehci0_pdata_v2),
	OF_DEV_AUXDATA("qcom,ag71xx-pci",  AR724X_PCI_MEM_BASE, "ath_pci",  		&generic_ath_pci_data),
#ifdef CONFIG_ATH79_DEV_NFC
	OF_DEV_AUXDATA("qcom,ag71xx-nfc",  AR934X_NFC_BASE, 	"ar934x-nfc",  		&ath79_nfc_data),
#endif
	{}
};

static void __init generic_init(void)
{
	u8 *art = (u8 *) KSEG1ADDR(0x1fff0000);

	ath79_init_usb_pdata();

	ath79_init_m25p80_pdata(NULL);

	ath79_init_mdio_pdata(0, 0);
	ath79_init_mdio_pdata(1, 0);

	ath79_init_wmac_pdata(art + DT_WMAC_CALDATA_OFFSET, NULL);
	ath79_wmac_dt_fixup(&generic_auxdata_lookup[0]);

	memcpy(generic_ath_pci_data.eeprom_data, art + DT_PCIE_CALDATA_OFFSET,
			sizeof(generic_ath_pci_data.eeprom_data));

	ath79_init_mac(ath79_eth0_data.mac_addr, art + DT_MAC0_OFFSET, 0);
	ath79_eth0_data.phy_if_mode = PHY_INTERFACE_MODE_RGMII;
	ath79_init_eth_pdata(0);
	ath79_eth0_data.mii_bus_dev = NULL;

	ath79_init_mac(ath79_eth1_data.mac_addr, art + DT_MAC1_OFFSET, 0);
	ath79_eth1_data.phy_if_mode = PHY_INTERFACE_MODE_GMII;
	ath79_init_eth_pdata(1);
	ath79_eth1_data.mii_bus_dev = NULL;

	ath79_init_nfc_pdata();
}

static void __init generic_setup(void)
{
	generic_init();
	of_platform_populate(NULL, generic_common_ids, generic_auxdata_lookup, NULL);
}

MIPS_MACHINE(ATH79_MACH_GENERIC_DT, "GENERIC-DT", "Qualcomm GENERIC-DT reference board", generic_setup);
