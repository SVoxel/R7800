# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: Makefile,v 1.1.2.6 2007/02/08 05:48:27 agui Exp $

include $(TOPDIR)/rules.mk

PKG_NAME:=bc
PKG_VERSION:=1.06
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=http://ftp.debian.org/debian/pool/main/b/bc
PKG_MD5SUM:=d44b5dddebd8a7a7309aea6c36fda117
PKG_CAT:=zcat

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/bc
  SECTION:=net
  CATEGORY:=Base system
  TITLE:=Bc is a command line calculator
  URL:=http://packages.debian.org/bc
endef

define Build/Configure
	(cd $(PKG_BUILD_DIR);CC="$(TARGET_CC)" CFLAGS="$(TARGET_CFLAGS)" ./configure --target="$(ARCH)" --prefix=/usr --with-readline --mandir=/usr/share/man --infodir=/usr/share/info)
endef

define Package/bc/install
	install -m0755 -d $(1)/usr/sbin
	install -m0755 $(PKG_BUILD_DIR)/bc/bc $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,bc))
