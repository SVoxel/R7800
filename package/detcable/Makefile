# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: Makefile,v 1.1.2.1 2008-03-22 04:48:39 xinwei.niu Exp $

include $(TOPDIR)/rules.mk

PKG_NAME:=detcable
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/detcable
  SECTION:=utils
  CATEGORY:=Base system
  DEPENDS:=@TARGET_ipq806x
  TITLE:=Update utility for wan cable link detect
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
endef

define Package/$(PKG_NAME)/install
	install -d -m0755 $(1)/usr/bin
	install -d -m0755 $(1)/etc
	install -m0755 $(PKG_BUILD_DIR)/detcable $(1)/usr/bin/
	install -m0755 ./files/flushRoute $(1)/usr/bin/
	install -d -m0755 $(1)/etc/init.d
	install -m0755 ./files/detcable.init $(1)/etc/init.d/detcable
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
