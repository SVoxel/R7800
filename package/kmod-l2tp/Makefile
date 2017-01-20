# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id$

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=kmod-l2tp
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(KERNEL_BUILD_DIR)/$(PKG_NAME)

PKG_GIT_TREEISH:=kmod-drv-l2tp-dni2

include $(INCLUDE_DIR)/package.mk

define KernelPackage/l2tp-dni
  SUBMENU:=Other modules
  TITLE:=PPP over L2TP linux kernel module
  VERSION:=$(LINUX_VERSION)-$(PKG_RELEASE)
  FILES:= \
	$(PKG_BUILD_DIR)/l2tp.$(LINUX_KMOD_SUFFIX)
  AUTOLOAD:=$(call AutoLoad,46,l2tp)
endef

define Build/Prepare
	$(CP) $(GIT_HOME)/kmod-drv-l2tp.git/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C "$(LINUX_DIR)" \
		CROSS_COMPILE="$(TARGET_CROSS)" \
		ARCH="$(LINUX_KARCH)" \
		SUBDIRS="$(PKG_BUILD_DIR)" \
		V=1 \
		modules
endef

$(eval $(call KernelPackage,l2tp-dni))
