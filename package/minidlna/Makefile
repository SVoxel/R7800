#
# Copyright (C) 2010-2014 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=minidlna
PKG_VERSION:=2018-04-10-799e6cf5
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.xz
PKG_MD5SUM:=0e18fd365167e6988404b6e8d88ff258 

PKG_LICENSE:=GPL-2.0 BSD-3-Clause
PKG_LICENSE_FILES:=COPYING LICENCE.miniupnpd

PKG_MAINTAINER:=Knyazkov Dmitry <medavedik@gmail.com>

PKG_FIXUP:=autoreconf

PKG_BUILD_PARALLEL:=0
PKG_BUILD_DEPENDS:=util-linux

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/nls.mk

define Package/minidlna
  SECTION:=multimedia
  CATEGORY:=Multimedia
  TITLE:=UPnP A/V & DLNA Media Server
  URL:=http://minidlna.sourceforge.net/
  DEPENDS:= +libpthread +libexif +libjpeg +libsqlite3 +libffmpeg \
  	+libid3tag +libflac +libvorbis +libuuid \
  	$(ICONV_DEPENDS) $(INTL_DEPENDS)
  USERID:=minidlna:minidlna
endef

define Package/minidlna/description
  MiniDLNA (aka ReadyDLNA) is server software with the aim of
  being fully compliant with DLNA/UPnP-AV clients.
endef

ICONV_PREFIX := $(STAGING_DIR)/usr/lib/libiconv
INTL_PREFIX := $(STAGING_DIR)/usr/lib/libintl
TARGET_CFLAGS += -std=gnu99 

TARGET_CPPFLAGS += \
	-I$(ICONV_PREFIX)/include \
	-I$(INTL_PREFIX)/include \
	-D_GNU_SOURCE -D_FILE_OFFSET_BITS=64 \

TARGET_LDFLAGS += \
	-L$(ICONV_PREFIX)/lib \
	-L$(INTL_PREFIX)/lib \
	-Wl,-rpath-link=$(STAGING_DIR)/usr/lib \

MAKE_FLAGS +=\
	CFLAGS="$(TARGET_CFLAGS) $(TARGET_CPPFLAGS)" \
	LDFLAGS="$(TARGET_LDFLAGS)" \
	ICONV_LIBS="-liconv $(if $(INTL_FULL),-lintl)" \

CONFIGURE_ARGS +=\
	--with-libiconv-prefix="$(ICONV_PREFIX)" \
	--with-libintl-prefix="$(INTL_PREFIX)" \
	--with-os-name="Linux IPQ8065" \
	--with-os-version="$(LINUX_VERSION)" \
	--with-os-url="https://www.voxel-firmware.com/" \
	--enable-tivo \
	--enable-netgear \

define Package/minidlna/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/minidlnad $(1)/usr/sbin/minidlna
	$(INSTALL_DIR) $(1)/sbin
	$(INSTALL_BIN) ./files/cmddlna.in  $(1)/sbin/cmddlna
endef

$(eval $(call BuildPackage,minidlna))
