# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: Makefile 6501 2007-03-04 04:41:46Z pavlov $

include $(TOPDIR)/rules.mk

PKG_NAME:=artmtd
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/artmtd
  SECTION:=utils
  CATEGORY:=Base system
  TITLE:=Read mac , wpspin, serialnumber and etc from art data
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
endef

define Package/artmtd/install
	$(INSTALL_DIR) $(1)/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/artmtd $(1)/sbin/
endef

$(eval $(call BuildPackage,artmtd))
