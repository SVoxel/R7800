# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: Makefile,v 1.1.2.2 2006/12/06 00:48:31 ronger Exp $

include $(TOPDIR)/rules.mk

PKG_NAME:=utelnetd
PKG_VERSION:=0.1.9
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=$(DNI_DL_WEB) http://gentoo.osuosl.org/distfiles @SF/utelnetd
PKG_MD5SUM:=45fc2b66b3d2dade396ed122a41d5b23
PKG_CAT:=zcat

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/utelnetd
  SECTION:=net
  CATEGORY:=Base system
  TITLE:=utelnetd for CLI
  URL:=http://utelnetd.sourceforge.net/
endef

define Build/Configure
endef

define Package/utelnetd/install
	install -m0755 -d $(1)/usr/sbin
	install -m0755 $(PKG_BUILD_DIR)/utelnetd $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,utelnetd))
