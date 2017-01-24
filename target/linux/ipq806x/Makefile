# Copyright (c) 2013 The Linux Foundation. All rights reserved.
#
include $(TOPDIR)/rules.mk

ARCH:=arm
BOARD:=ipq806x
BOARDNAME:=QCA IPQ806X
FEATURES:=ubifs targz squashfs usb usbgadget dtc

LINUX_VERSION:=3.4.103
KERNELNAME:="uImage"

CUSTOMER:=dni
LINUX_TREEISH:=linux-v3.4-dni1
NETFILTER_TREEISH:=netfilter-v3.4-dni1

CFLAGS:=-O2 -pipe -march=armv7-a -mcpu=cortex-a9 -mfpu=neon-vfpv4 -mtune=cortex-a9 -mfloat-abi=softfp

include $(INCLUDE_DIR)/target.mk

DEFAULT_PACKAGES += \
	mtd-utils kmod-button-hotplug uboot-envtools swconfig \
	mtd-utils mtd-utils-mkfs.ubifs mtd-utils-ubiattach mtd-utils-ubidetach \
	mtd-utils-ubiformat mtd-utils-ubimkvol mtd-utils-ubiupdatevol \
	kmod-button-hotplug uboot-envtools swconfig \
	kmod-input-gpio-keys kmod-qca-nss-drv kmod-qca-nss-gmac kmod-leds-gpio \
	kmod-ledtrig-default-on kmod-ledtrig-timer kmod-ledtrig-netdev \
	kmod-usb-gadget kmod-usb-gadget-dwc3 kmod-usb-gadget-dwc3-ipq \
	kmod-usb-core kmod-usb2 kmod-usb-xhci uboot-ipq806x \
	kmod-ledtrig-usbdev uboot-ipq806x-fwupgrade-tools

define Target/Description
	Build firmware images for Qualcomm Atheros IPQ806x (arm) platform.
endef

$(eval $(call BuildTarget))
