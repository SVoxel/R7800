# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id$

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=kmod-ipv6-cone
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(KERNEL_BUILD_DIR)/$(PKG_NAME)
PKG_GIT_TREEISH:=kmod-ipv6-cone-v3.4-dni1

include $(INCLUDE_DIR)/package.mk

define KernelPackage/ipv6-cone
  SUBMENU:=Other modules
  TITLE:=IPv6 CONE linux kernel module
  VERSION:=$(LINUX_VERSION)-$(PKG_RELEASE)
  FILES:= \
	$(PKG_BUILD_DIR)/ip6t_CONE.$(LINUX_KMOD_SUFFIX)
  AUTOLOAD:=$(call AutoLoad,46,ip6t_CONE)
endef

define Build/Prepare
	$(CP) $(GIT_HOME)/kmod-ipv6-cone.git/* $(PKG_BUILD_DIR)/
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

define KernelPackage/ipv6-cone/install
	$(INSTALL_DIR) $(1)/lib/network/
endef

$(eval $(call KernelPackage,ipv6-cone))
