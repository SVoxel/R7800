#
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=kmod-trigger
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(KERNEL_BUILD_DIR)/$(PKG_NAME)
PKG_GIT_TREEISH:=kmod-trigger-dni7

include $(INCLUDE_DIR)/package.mk

define KernelPackage/trigger
  SUBMENU:=Other modules
  TITLE:=DNI Port Triggerring module
  VERSION:=$(LINUX_VERSION)-$(BOARD)-$(PKG_RELEASE)
  FILES:= \
	$(PKG_BUILD_DIR)/ipt_TRIGGER.$(LINUX_KMOD_SUFFIX)
  AUTOLOAD:=$(call AutoLoad,46,ipt_TRIGGER)
endef

define Build/Prepare
	$(CP) $(GIT_HOME)/kmod-trigger.git/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C "$(LINUX_DIR)" \
		CROSS_COMPILE="$(TARGET_CROSS)" \
		ARCH="$(LINUX_KARCH)" \
		SUBDIRS="$(PKG_BUILD_DIR)" \
		EXTRA_CFLAGS="$(BUILDFLAGS)" \
		modules
endef

define KernelPackage/trigger/install
	$(INSTALL_DIR) $(1)/lib/network/
endef

$(eval $(call KernelPackage,trigger))
