#
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: Makefile 6565 2007-03-14 13:42:21Z nbd $

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=kmod-urlblock
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(KERNEL_BUILD_DIR)/$(PKG_NAME)
PKG_GIT_TREEISH:=kmod-urlblock-dni9

include $(INCLUDE_DIR)/package.mk

define KernelPackage/urlblock
  SUBMENU:=Other modules
  TITLE:=DNI URL Block firewall module
  VERSION:=$(LINUX_VERSION)-$(BOARD)-$(PKG_RELEASE)
  FILES:= \
	$(PKG_BUILD_DIR)/ipt_urlBlock.$(LINUX_KMOD_SUFFIX)
  AUTOLOAD:=$(call AutoLoad,46,ipt_urlBlock)
endef

define Build/Prepare
	$(CP) $(GIT_HOME)/kmod-urlblock.git/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
endef

define KernelPackage/urlblock/install
	$(INSTALL_DIR) $(1)/lib/network/
endef

$(eval $(call KernelPackage,urlblock))
