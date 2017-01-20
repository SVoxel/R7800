#
# Copyright (c) 2013 The Linux Foundation. All rights reserved.
# Copyright (C) 2006-2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

define KernelPackage/leds-rb750
  SUBMENU:=$(LEDS_MENU)
  TITLE:=RouterBOARD 750 LED support
  DEPENDS:=@TARGET_ar71xx
  KCONFIG:=CONFIG_LEDS_RB750
  FILES:=$(LINUX_DIR)/drivers/leds/leds-rb750.ko
  AUTOLOAD:=$(call AutoLoad,60,leds-rb750)
endef

define KernelPackage/leds-rb750/description
 Kernel module for the LEDs on the MikroTik RouterBOARD 750.
endef

$(eval $(call KernelPackage,leds-rb750))


define KernelPackage/leds-wndr3700-usb
  SUBMENU:=$(LEDS_MENU)
  TITLE:=WNDR3700 USB LED support
  DEPENDS:=@TARGET_ar71xx
  KCONFIG:=CONFIG_LEDS_WNDR3700_USB
  FILES:=$(LINUX_DIR)/drivers/leds/leds-wndr3700-usb.ko
  AUTOLOAD:=$(call AutoLoad,60,leds-wndr3700-usb)
endef

define KernelPackage/leds-wndr3700-usb/description
 Kernel module for the USB LED on the NETGEAR WNDR3700 board.
endef

$(eval $(call KernelPackage,leds-wndr3700-usb))


define KernelPackage/nand-ar934x
  SUBMENU:=$(OTHER_MENU)
  TITLE:=Atheros AR934x NAND flash controller driver
  KCONFIG:=CONFIG_MTD_NAND_AR934X
  DEPENDS:=@TARGET_ar71xx +kmod-nand
  FILES:=$(LINUX_DIR)/drivers/mtd/nand/ar934x_nfc.ko
  AUTOLOAD:=$(call AutoLoad,30,ar934x_nfc)
endef

define KernelPackage/nand-ar934x/description
  Atheros AR934x NAND flash controller driver.
endef

$(eval $(call KernelPackage,nand-ar934x))


define KernelPackage/spi-vsc7385
  SUBMENU:=$(SPI_MENU)
  TITLE:=Vitesse VSC7385 ethernet switch driver
  DEPENDS:=@TARGET_ar71xx
  KCONFIG:=CONFIG_SPI_VSC7385
  FILES:=$(LINUX_DIR)/drivers/spi/spi-vsc7385.ko
  AUTOLOAD:=$(call AutoLoad,93,spi-vsc7385)
endef

define KernelPackage/spi-vsc7385/description
  This package contains the SPI driver for the Vitesse VSC7385 ethernet switch.
endef

$(eval $(call KernelPackage,spi-vsc7385))


define KernelPackage/wdt-ath79
  SUBMENU:=$(OTHER_MENU)
  TITLE:=Atheros AR7XXX/AR9XXX watchdog timer
  DEPENDS:=@TARGET_ar71xx
  KCONFIG:=CONFIG_ATH79_WDT
  FILES:=$(LINUX_DIR)/drivers/$(WATCHDOG_DIR)/ath79_wdt.ko
  AUTOLOAD:=$(call AutoLoad,50,ath79_wdt)
endef

define KernelPackage/wdt-ath79/description
  Kernel module for AR7XXX/AR9XXX watchdog timer.
endef

$(eval $(call KernelPackage,wdt-ath79))


define KernelPackage/nand-ath79
  SUBMENU:=$(OTHER_MENU)
  TITLE:=NAND flash support
  DEPENDS:=@TARGET_ar71xx kmod-nand
  KCONFIG:=CONFIG_MTD_NAND_ATH79
  FILES:= \
	$(LINUX_DIR)/drivers/mtd/nand/ath79_nand.ko
  AUTOLOAD:=$(call AutoLoad,20,ath79_nand)
endef

define KernelPackage/nand-ath79/description
 Kernel module for NAND support of Qualcomm Atheros SoC AR72xx & AR93xx
endef

$(eval $(call KernelPackage,nand-ath79))


define KernelPackage/sound-soc-cus227
  TITLE:=Qualcomm-Atheros CUS227 board sound support
  KCONFIG:= \
	CONFIG_SND_ATH79_SOC \
	CONFIG_SND_ATH79_SOC_I2S \
	CONFIG_SND_ATH79_SOC_CODEC \
	CONFIG_SND_ATH79_SOC_CUS227
  FILES:= \
	$(LINUX_DIR)/sound/soc/ath79/snd-soc-ath79-i2s.ko \
	$(LINUX_DIR)/sound/soc/ath79/snd-soc-ath79-pcm.ko \
	$(LINUX_DIR)/sound/soc/codecs/snd-soc-wm8988.ko \
	$(LINUX_DIR)/sound/soc/ath79/snd-soc-cus227.ko
  AUTOLOAD:=$(call AutoLoad,60,snd-soc-ath79-i2s snd-soc-ath79-pcm \
				snd-soc-wm8988 snd-soc-cus227)
  DEPENDS:=@TARGET_ar71xx +kmod-sound-soc-core +kmod-regmap-spi +kmod-regmap-i2c
  $(call AddDepends/sound)
endef

$(eval $(call KernelPackage,sound-soc-cus227))


define KernelPackage/sound-soc-db120
  TITLE:=Qualcomm-Atheros DB12x board sound support
  KCONFIG:= \
	CONFIG_SND_ATH79_SOC \
	CONFIG_SND_ATH79_SOC_I2S \
	CONFIG_SND_ATH79_SOC_CODEC \
	CONFIG_SND_ATH79_SOC_DB120
  FILES:= \
	$(LINUX_DIR)/sound/soc/ath79/snd-soc-ath79-i2s.ko \
	$(LINUX_DIR)/sound/soc/ath79/snd-soc-ath79-pcm.ko \
	$(LINUX_DIR)/sound/soc/ath79/snd-soc-ath79-codec.ko \
	$(LINUX_DIR)/sound/soc/ath79/snd-soc-db120.ko
  AUTOLOAD:=$(call AutoLoad,60,snd-soc-ath79-i2s snd-soc-ath79-pcm \
				snd-soc-ath79-codec snd-soc-db120)
  DEPENDS:=@TARGET_ar71xx +kmod-sound-soc-core +kmod-regmap-spi +kmod-regmap-i2c
  $(call AddDepends/sound)
endef

$(eval $(call KernelPackage,sound-soc-db120))
