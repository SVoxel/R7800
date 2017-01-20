# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: Makefile 6501 2007-03-04 04:41:46Z pavlov $

include $(TOPDIR)/rules.mk

PKG_NAME:=bridge-mode
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/bridge-mode
  SECTION:=utils
  CATEGORY:=Base system
  TITLE:=For bridge-mode
endef

define Build/Prepare
endef

define Build/Compile
endef

define Package/bridge-mode/install
	mkdir -p $(1)/etc/init.d
	install -m0755 ./files/br-mode $(1)/etc/init.d/
	install -d -m0755 $(1)/usr/sbin
	install -m0755 ./files/start_br_udhcpc $(1)/usr/sbin
	install -m0755 ./files/br-dhcp-helper $(1)/usr/sbin
endef

$(eval $(call BuildPackage,bridge-mode))
