# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id$

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=kmod-ftp-alg
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(KERNEL_BUILD_DIR)/$(PKG_NAME)
PKG_GIT_TREEISH:=kmod-ftp-v3.4-dni1

include $(INCLUDE_DIR)/package.mk

define KernelPackage/ftp-alg
  SUBMENU:=Other modules
  TITLE:=FTP ALG linux kernel module
  VERSION:=$(LINUX_VERSION)-$(PKG_RELEASE)
  FILES:= \
	$(PKG_BUILD_DIR)/nf_conntrack_ftp.$(LINUX_KMOD_SUFFIX) $(PKG_BUILD_DIR)/nf_nat_ftp.$(LINUX_KMOD_SUFFIX)
  AUTOLOAD:=$(call AutoLoad,48,nf_conntrack_ftp nf_nat_ftp)
endef

define Build/Prepare
	$(CP) $(GIT_HOME)/kmod-ftp.git/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C "$(LINUX_DIR)" \
		CROSS_COMPILE="$(TARGET_CROSS)" \
		ARCH="$(LINUX_KARCH)" \
		SUBDIRS="$(PKG_BUILD_DIR)" \
		V=1 \
		modules
endef

define KernelPackage/ftp-alg/install
	$(INSTALL_DIR) $(1)/lib/network/
endef

$(eval $(call KernelPackage,ftp-alg))
