# 
# Copyright (C) 2007 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id$

include $(TOPDIR)/rules.mk

PKG_NAME:=soapclient
PKG_VERSION:=1.0.0
PKG_RELEASE:=1
PKG_BUILD_DEPENDS:=datalib

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/soapclient
  SECTION:=net
  CATEGORY:=Network
  TITLE:= soapclient
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
endef

define Package/soapclient/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/soapclient $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/satelliteinfo $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/soap_check_fw $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/keep_ext_connect $(1)/usr/sbin
endef

$(eval $(call BuildPackage,soapclient))
