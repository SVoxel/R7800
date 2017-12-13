# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: Makefile,v 1.1.2.2 2007/01/31 06:23:51 ronger Exp $

include $(TOPDIR)/rules.mk

PKG_NAME:=minidlna
PKG_VERSION:=1.2.1
PKG_RELEASE:=1

PKG_GIT_DIR:=minidlna.git

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PKG_BUILD_DEPENDS:=libsqlite3 ffmpeg libexif libflac libid3tag libjpeg libogg libvorbis zlib libiconv

include $(INCLUDE_DIR)/package.mk


define Package/minidlna
  SECTION:=net
  CATEGORY:=Network
  TITLE:=minidlna server
  URL:=http://sourceforge.net/projects/minidlna/
endef

define Package/minidlna/config
	menu "Configuration"
		depends on PACKAGE_minidlna
		source "$(SOURCE)/Config.in"
	endmenu
endef

define Build/Prepare
	$(CP) $(GIT_HOME)/minidlna.git/* $(PKG_BUILD_DIR)/
endef

define Build/Configure
	@test x$(CONFIG_MINIDLNA_MODELNAME) != x \
		|| (echo "Compile $(PKG_NAME) failed: Some necessary configuration option is invalid, please run "make menuconfig" to specify"; false)
	(cd $(PKG_BUILD_DIR); \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS) $(EXTRA_CFLAGS)" \
		CXXFLAGS="$(TARGET_CFLAGS) $(EXTRA_CFLAGS)" \
		CPPFLAGS="$(TARGET_CPPFLAGS) $(EXTRA_CPPFLAGS)" \
		LDFLAGS="$(TARGET_LDFLAGS)" \
		./configure \
			--target=$(GNU_TARGET_NAME) \
			--host=$(GNU_HOST_NAME) \
			--build=$(GNU_BUILD_NAME) \
			--enable-tivo \
			--enable-netgear \
	);
endef

define Build/Compile
	mv $(STAGING_DIR)/usr/include/libavutil/time.h $(STAGING_DIR)/usr/include/libavutil/time.h_
	$(MAKE) -C $(PKG_BUILD_DIR) \
                $(TARGET_CONFIGURE_OPTS) \
                CROSS="$(TARGET_CROSS)" \
                CFLAGS="-Wall -g -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64 $(TARGET_CFLAGS) -I$(STAGING_DIR)/usr/include -I$(STAGING_DIR)/include -I$(STAGING_DIR)/usr/include/libavutil -I$(STAGING_DIR)/usr/include/libavcodec -I$(STAGING_DIR)/usr/include/libavformat -I$(STAGING_DIR)/usr/include/ffmpeg -I$(STAGING_DIR)/usr/lib/libintl/include -I$(STAGING_DIR)/usr/lib/libiconv/include" \
                LDFLAGS="-L$(STAGING_DIR)/usr/lib -L$(STAGING_DIR)/lib -L$(STAGING_DIR)/usr/lib/libintl/lib -L$(STAGING_DIR)/usr/lib/libiconv/lib -liconv" \
                ARCH="$(ARCH)" \
                STAGING_DIR="$(STAGING_DIR)" \
		all
	mv $(STAGING_DIR)/usr/include/libavutil/time.h_ $(STAGING_DIR)/usr/include/libavutil/time.h
endef

define Package/minidlna/install
#	mv $(PKG_BUILD_DIR)/minidlnad $(PKG_BUILD_DIR)/minidlna
	install -d -m0755 $(1)/usr/sbin
	install -m0755 $(PKG_BUILD_DIR)/minidlnad $(1)/usr/sbin/minidlna
	install -d -m0755 $(1)/sbin
	install -m0755 $(PKG_BUILD_DIR)/cmddlna  $(1)/sbin/
endef

$(eval $(call BuildPackage,minidlna))
