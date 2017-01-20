# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: Makefile 6565 2007-03-14 13:42:21Z nbd $

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=kmod-dnisip
PKG_RELEASE:=1
PKG_GIT_TREEISH=alg-sip-v3.4-dni1
NF=nf
PKG_BUILD_DIR:=$(KERNEL_BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define KernelPackage/dnisip
  SUBMENU:=DNI Kernel Module
  TITLE:=DNI SIP ALG
  VERSION:=$(LINUX_VERSION)-$(BOARD)-$(PKG_RELEASE)
  FILES:= \
	$(PKG_BUILD_DIR)/$(NF)_conntrack_dnisip.$(LINUX_KMOD_SUFFIX) \
	$(PKG_BUILD_DIR)/$(NF)_nat_dnisip.$(LINUX_KMOD_SUFFIX)
  AUTOLOAD:=$(call AutoLoad,46,$(NF)_conntrack_dnisip $(NF)_nat_dnisip)
endef

define Build/Prepare
	$(CP) $(GIT_HOME)/kmod-dnisip.git/* $(PKG_BUILD_DIR)/
endef

ifeq ($(BOARD),brcm)
BUILDFLAGS := -DBROADCOM -DBCMDRIVER -I$(LINUX_DIR)/arch/mips/bcm947xx/include
else
ifeq ($(BOARD),brcm47xx)
BUILDFLAGS := -DBROADCOM
endif
endif

define Build/Compile
	$(MAKE) -C "$(LINUX_DIR)" \
		CROSS_COMPILE="$(TARGET_CROSS)" \
		ARCH="$(LINUX_KARCH)" \
		SUBDIRS="$(PKG_BUILD_DIR)" \
		EXTRA_CFLAGS="$(BUILDFLAGS)" \
		modules
endef

$(eval $(call KernelPackage,dnisip))
