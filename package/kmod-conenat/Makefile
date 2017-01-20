# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id$

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=kmod-conenat
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(KERNEL_BUILD_DIR)/$(PKG_NAME)
PKG_GIT_TREEISH:=kmod-conenat-v3.4-dni1

include $(INCLUDE_DIR)/package.mk

define KernelPackage/conenat
  SUBMENU:=Other modules
  TITLE:=Conenat linux kernel module
  VERSION:=$(LINUX_VERSION)-$(PKG_RELEASE)
  FILES:= \
	$(PKG_BUILD_DIR)/ipt_CONENAT.$(LINUX_KMOD_SUFFIX)
  AUTOLOAD:=$(call AutoLoad,46,ipt_CONENAT)
endef

define Build/Prepare
	$(CP) $(GIT_HOME)/kmod-conenat.git/* $(PKG_BUILD_DIR)/
endef

#ifeq ($(BOARD),brcm)
#BUILDFLAGS := -DBROADCOM -DBCMDRIVER -I$(LINUX_DIR)/arch/mips/bcm947xx/include
#else
#ifeq ($(BOARD),brcm47xx)
#BUILDFLAGS := -DBROADCOM
#endif
#endif


define Build/Compile
endef

define KernelPackage/conenat/install
	$(INSTALL_DIR) $(1)/lib/network/
endef

$(eval $(call KernelPackage,conenat))
