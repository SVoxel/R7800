#
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: Makefile 6565 2007-03-14 13:42:21Z nbd $

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=kmod-netgear-reject
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(KERNEL_BUILD_DIR)/$(PKG_NAME)
PKG_GIT_TREEISH:=kmod-netgear-reject-v3.4-dni1

include $(INCLUDE_DIR)/package.mk

define KernelPackage/netgear-reject
  SUBMENU:=Other modules
  TITLE:=REJECT target module designed for Netgear
  VERSION:=$(LINUX_VERSION)-$(BOARD)-$(PKG_RELEASE)
  FILES:= \
	$(PKG_BUILD_DIR)/ipt_NETGEAR_REJECT.$(LINUX_KMOD_SUFFIX)
  AUTOLOAD:=$(call AutoLoad,45,ipt_NETGEAR_REJECT)
endef

define Build/Prepare
	$(CP) $(GIT_HOME)/kmod-netgear-reject.git/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C "$(LINUX_DIR)" \
		CROSS_COMPILE="$(TARGET_CROSS)" \
		ARCH="$(LINUX_KARCH)" \
		SUBDIRS="$(PKG_BUILD_DIR)" \
		EXTRA_CFLAGS="$(BUILDFLAGS)" \
		modules
endef

define KernelPackage/netgear-reject/install
	$(INSTALL_DIR) $(1)/lib/network/
endef

$(eval $(call KernelPackage,netgear-reject))
