#
# Copyright (c) 2013 The Linux Foundation. All rights reserved.
# Copyright (C) 2008-2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

ARCH:=mips
BOARD:=ar71xx
BOARDNAME:=Atheros AR7xxx/AR9xxx
FEATURES:=squashfs jffs2 targz
CFLAGS:=-Os -pipe -mips32r2 -mtune=74kc -fno-caller-saves -freorder-blocks
SUBTARGETS:=generic nand

LINUX_VERSION:=3.3.8

include $(INCLUDE_DIR)/target.mk

DEFAULT_PACKAGES += \
	kmod-leds-gpio kmod-gpio-button-hotplug kmod-wdt-ath79 swconfig \
	kmod-ledtrig-default-on kmod-ledtrig-timer kmod-ledtrig-netdev \
	kmod-button-hotplug kmod-ath9k wpad-mini uboot-envtools

define Target/Description
	Build firmware images for Atheros AR7xxx/AR9xxx based boards.
endef

$(eval $(call BuildTarget))
