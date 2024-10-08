#
# Copyright (C) 2008-2015 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=libvorbis
PKG_VERSION:=1.3.7
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.xz
PKG_SOURCE_URL:=http://downloads.xiph.org/releases/vorbis/
PKG_MD5SUM:=50902641d358135f06a8392e61c9ac77
PKG_MAINTAINER:=Ted Hess <thess@kitschensync.net>

PKG_LICENSE:=BSD-3-Clause
PKG_LICENSE_FILES:=COPYING

PKG_FIXUP:=autoreconf
PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk

TARGET_CFLAGS := $(filter-out -O%,$(TARGET_CFLAGS)) -Os

define Package/libvorbis
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=libvorbis
  URL:=http://xiph.org/vorbis/
  DEPENDS:=+libogg
endef

define Package/libvorbis/description
Vorbis is a general purpose audio and music encoding format
contemporary to MPEG-4's AAC and TwinVQ, the next generation beyond
MPEG audio layer 3. Unlike the MPEG sponsored formats (and other
proprietary formats such as RealAudio G2 and Windows' flavor of the
month), the Vorbis CODEC specification belongs to the public domain.
All the technical details are published and documented, and any
software entity may make full use of the format without license
fee, royalty or patent concerns.
endef

CONFIGURE_ARGS+= \
	--disable-oggtest \

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include/vorbis/
	$(INSTALL_DATA) $(PKG_INSTALL_DIR)/usr/include/vorbis/* $(1)/usr/include/vorbis/
	$(INSTALL_DIR) $(1)/usr/lib/
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libvorbis.{so*,a,la} $(1)/usr/lib/
	$(INSTALL_DIR) $(1)/usr/lib/pkgconfig/
	$(INSTALL_DATA) $(PKG_INSTALL_DIR)/usr/lib/pkgconfig/* $(1)/usr/lib/pkgconfig/
	$(INSTALL_DIR) $(1)/usr/share/aclocal/
	$(INSTALL_DATA) $(PKG_INSTALL_DIR)/usr/share/aclocal/* $(1)/usr/share/aclocal/
endef

define Package/libvorbis/install
	$(INSTALL_DIR) $(1)/usr/lib/
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libvorbis.so.* $(1)/usr/lib/
endef

$(eval $(call BuildPackage,libvorbis))
