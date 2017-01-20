# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: Makefile 5196 2006-10-17 20:52:40Z nbd $
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk
PKG_NAME:=nat2.5
PKG_VERSION:=
PKG_RELEASE:=1

PKG_SOURCE:=
PKG_SOURCE_URL:=
PKG_MD5SUM:=
PKG_CAT:=zcat

PKG_GIT_TREEISH=6d52a453b80c4db5d37f93b131a917b598ab886f
GIT_REPOSITORY=dni-nat2.5.git

PKG_BUILD_DIR:=$(KERNEL_BUILD_DIR)/nat2.5

include $(INCLUDE_DIR)/package.mk

define KernelPackage/nat2.5
  SUBMENU:=Other modules
  TITLE:=NAT 2.5 (client bridge)
  VERSION:=none
  FILES:= $(PKG_BUILD_DIR)/clbr.$(LINUX_KMOD_SUFFIX)
  AUTOLOAD:=$(call AutoLoad,15,clbr)
endef

define Build/Prepare
	$(CP) $(GIT_HOME)/dni-nat2.5.git/* $(PKG_BUILD_DIR)/
endef

NAT2_5_MAKEOPTS= \
		ARCH="$(LINUX_KARCH)" \
		CROSS_COMPILE="$(TARGET_CROSS)" \
		TOOLPREFIX=$(TARGET_CROSS) \
		TOOLPATH= \
		KERNELPATH="$(LINUX_DIR)"

define Build/Compile
endef

define Build/InstallDev
endef

define Build/UninstallDev
	rm -rf	$(LINUX_DIR)/include/net/if_ath_clbr.h
	rm -rf	$(LINUX_DIR)/include/net/clbr_queue.h
endef

$(eval $(call KernelPackage,nat2.5))
