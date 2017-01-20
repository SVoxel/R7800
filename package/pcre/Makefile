#
# Copyright (C) 2006-2008 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: Makefile 12228 2008-08-06 22:43:15Z nbd $

include $(TOPDIR)/rules.mk

PKG_NAME:=pcre
PKG_VERSION:=7.6
PKG_RELEASE:=2

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_URL:=@SF/pcre
PKG_MD5SUM:=2af38e083fb90ef60fa9eda7cc290e86

#PKG_FIXUP = libtool

include $(INCLUDE_DIR)/package.mk

define Package/libpcre
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=A Perl Compatible Regular Expression library
  URL:=http://www.pcre.org/
endef

TARGET_CFLAGS += $(FPIC)

CONFIGURE_ARGS += \
	--enable-shared \
	--enable-static \
	--enable-utf8 \
	--disable-cpp \

MAKE_FLAGS += \
	DESTDIR="$(PKG_INSTALL_DIR)" \
	all install

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/bin $(2)/bin $(1)/usr/include $(1)/usr/lib $(1)/usr/lib/pkgconfig
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/pcre-config $(1)/usr/bin/
	ln -sf $(STAGING_DIR)/usr/bin/pcre-config $(2)/bin
	$(INSTALL_DATA) $(PKG_INSTALL_DIR)/usr/include/pcre{,posix}.h $(1)/usr/include/
	$(INSTALL_DATA) $(PKG_INSTALL_DIR)/usr/lib/libpcre{,posix}.{a,so*} $(1)/usr/lib/
	$(INSTALL_DATA) $(PKG_INSTALL_DIR)/usr/lib/pkgconfig/libpcre.pc $(1)/usr/lib/pkgconfig/
endef

define Package/libpcre/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libpcre{,posix}.so $(1)/usr/lib/
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libpcre{,posix}.so.* $(1)/usr/lib/
endef

$(eval $(call BuildPackage,libpcre))
