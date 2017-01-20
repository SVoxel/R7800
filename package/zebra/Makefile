# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: Makefile,v 1.1.2.6 2007/02/08 05:48:27 agui Exp $

include $(TOPDIR)/rules.mk

PKG_NAME:=zebra
PKG_VERSION:=0.95a
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=$(DNI_DL_WEB) ftp://ftp.zebra.org/pub/zebra
PKG_MD5SUM:=9b8fec2d4f910334e50167414fcf193b
PKG_CAT:=zcat

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/zebra
  SECTION:=net
  CATEGORY:=Base system
  TITLE:=Zebra is free software that manages various routing protocols
  URL:=http://zebra.sourceforge.net/
endef

define Build/Configure
	(cd $(PKG_BUILD_DIR);CC="$(TARGET_CC)" CFLAGS="$(TARGET_CFLAGS)" ./configure --target="$(ARCH)" --host="$(GNU_TARGET_NAME)" --disable-ripd --disable-ospfd --disable-ospf6d --disable-bgpd --disable-bgp-announce --disable-ospfapi --disable-ospfclient)
endef

define Package/zebra/install
	install -m0755 -d $(1)/usr/sbin
	install -m0755 -d $(1)/etc
	install -m0755 -d $(1)/etc/init.d
	install -m0755 $(PKG_BUILD_DIR)/zebra/zebra $(1)/usr/sbin/
	install -m0755 ./files/zebra.conf $(1)/etc/zebra.conf
	install -m0755 ./files/zebra.init $(1)/etc/init.d/zebra
	install -m0755 $(PKG_BUILD_DIR)/ripngd/ripngd $(1)/usr/sbin/
	install -m0755 ./files/ripngd.conf $(1)/etc/ripngd.conf
	install -m0755 ./files/ripngd.init $(1)/etc/init.d/ripngd
endef

$(eval $(call BuildPackage,zebra))
