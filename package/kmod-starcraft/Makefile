# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=kmod-starcraft
PKG_VERSION:=
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(KERNEL_BUILD_DIR)/$(PKG_NAME)
PKG_GIT_TREEISH:=alg-starcraft-dni3

include $(INCLUDE_DIR)/package.mk

define KernelPackage/dnistarcraft-alg
  SUBMENU:=Other modules
  TITLE:=Starcraft ALG linux kernel module
  VERSION:=$(LINUX_VERSION)-$(PKG_RELEASE)
  FILES:= \
	$(PKG_BUILD_DIR)/nf_nat_STARCRAFT.$(LINUX_KMOD_SUFFIX)
  AUTOLOAD:=$(call AutoLoad,45,nf_nat_STARCRAFT)
endef

define Build/Prepare
	$(CP) $(GIT_HOME)/kmod-starcraft.git/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
endef

$(eval $(call KernelPackage,dnistarcraft-alg))
