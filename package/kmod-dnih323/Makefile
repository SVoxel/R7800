# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=kmod-dnih323
PKG_RELEASE:=1
PKG_GIT_TREEISH:=alg-h323-v3.4-dni3
NF=nf
PKG_BUILD_DIR:=$(KERNEL_BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define KernelPackage/dnih323
  SUBMENU:=DNI Kernel Module
  TITLE:=DNI H323 ALG
  VERSION:=$(LINUX_VERSION)-$(BOARD)-$(PKG_RELEASE)
  FILES:= \
	$(PKG_BUILD_DIR)/$(NF)_conntrack_dnih323.$(LINUX_KMOD_SUFFIX) \
	$(PKG_BUILD_DIR)/$(NF)_nat_dnih323.$(LINUX_KMOD_SUFFIX)
  AUTOLOAD:=$(call AutoLoad,46,$(NF)_conntrack_dnih323 $(NF)_nat_dnih323)
endef

define Build/Prepare
	 $(CP) $(GIT_HOME)/kmod-dnih323.git/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C "$(LINUX_DIR)" \
		CROSS_COMPILE="$(TARGET_CROSS)" \
		ARCH="$(LINUX_KARCH)" \
		SUBDIRS="$(PKG_BUILD_DIR)" \
		EXTRA_CFLAGS="$(BUILDFLAGS)" \
		modules
endef

$(eval $(call KernelPackage,dnih323))
