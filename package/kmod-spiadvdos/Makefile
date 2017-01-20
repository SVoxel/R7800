#
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=kmod-spiadvdos
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(KERNEL_BUILD_DIR)/$(PKG_NAME)
PKG_GIT_TREEISH:=kmod-spiadvdos-dni5

include $(INCLUDE_DIR)/package.mk

define KernelPackage/spiadvdos
  SUBMENU:=Other modules
  TITLE:=DNI Advanced SPI DOS firewall module
  VERSION:=$(LINUX_VERSION)-$(BOARD)-$(PKG_RELEASE)
  FILES:= \
	$(PKG_BUILD_DIR)/ipt_spiadvDoS.$(LINUX_KMOD_SUFFIX)
  AUTOLOAD:=$(call AutoLoad,46,ipt_spiadvDoS)
endef

define Build/Prepare
	$(CP) $(GIT_HOME)/kmod-spiadvdos.git/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
endef

define KernelPackage/spiadvdos/install
	$(INSTALL_DIR) $(1)/lib/network/
endef

$(eval $(call KernelPackage,spiadvdos))
