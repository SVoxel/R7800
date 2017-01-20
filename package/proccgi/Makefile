# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: Makefile,v 1.1.2.1 2007/01/09 00:57:44 ronger Exp $

include $(TOPDIR)/rules.mk

PKG_NAME:=proccgi
PKG_RELEASE:=4

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/proccgi
  SECTION:=utils
  CATEGORY:=Base system
  DEFAULT:=y
  TITLE:=The CGI interface
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Package/proccgi/install
	install -d -m0755 $(1)/www/cgi-bin
	install -m0755 $(PKG_BUILD_DIR)/proccgi $(1)/www/cgi-bin/
endef

$(eval $(call BuildPackage,proccgi))
