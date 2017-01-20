#
# Copyright (C) 2007-2009 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
  
include $(TOPDIR)/rules.mk

PKG_NAME:=aMule
PKG_VERSION:=2.3.1
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_URL:=@SF/amule
PKG_MD5SUM:=31724290a440943f5b05d4dca413fe02
PKG_BUILD_DEPENDS:=libgd libwxwidgets

include $(INCLUDE_DIR)/package.mk

define Package/amule
  SECTION:=utils
  CATEGORY:=Network
  DEPENDS:= +libpng +libpthread +libncurses +libreadline +libwxwidgets +libcryptoxx 
  TITLE:=A multi-platform eMule-like ed2k client
endef

CONFIGURE_ARGS+= \
           --enable-shared \
           --disable-static \
           --disable-rpath \
           --with-gnu-ld \
           --disable-ccache \
           --disable-debug \
           --disable-optimize \
           --disable-profile \
           --disable-monolithic \
           --enable-amule-daemon \
           --enable-amulecmd \
           --disable-amulecmdgui \
           --disable-webserver \
           --disable-webservergui \
           --disable-amule-gui \
           --disable-cas \
           --disable-wxcas \
           --disable-ed2k \
           --disable-alc \
           --disable-alcc \
           --disable-systray \
           --disable-utf8-systray \
           --enable-embedded-crypto \
           --enable-gsocket \
           --disable-gtktest \
           --disable-crypto \
           --prefix=$(PKG_INSTALL_DIR) \
           --exec-prefix=$(PKG_INSTALL_DIR) \
           --with-zlib="$(STAGING_DIR)/usr" \
           --with-gdlib-prefix="$(STAGING_DIR)/usr" \
           --with-libpng-prefix="$(STAGING_DIR)/usr" \
           --with-wx-prefix=../wxWidgets-2.8.12/ipkg-install/usr \
           --with-wxdir=../wxWidgets-2.8.12 \
           --with-wx-config=../wxWidgets-2.8.12/wx-config \
           --with-crypto-prefix="$(STAGING_DIR)/usr" \
           --with-libiconv-prefix="$(ICONV_PREFIX)" \
           --with-libintl-prefix="$(INTL_PREFIX)" \
           --without-x \

#define Build/Configure
#	(cd $(PKG_BUILD_DIR) ; \
#                ./configure \
#                        $(CONFIGURE_ARGS) \
#        );
#endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
                DESTDIR="$(PKG_INSTALL_DIR)" \
                all
endef

define Package/amule/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/etc/aMule
	$(INSTALL_DIR) $(1)/etc/aMule/config
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/amule{cmd,d} $(1)/usr/bin/
	$(INSTALL_BIN) ./file/amule.sh $(1)/etc/aMule/
	$(INSTALL_BIN) ./file/remote.conf $(1)/etc/aMule/
	$(INSTALL_BIN) ./file/amule.conf $(1)/etc/aMule/
	$(INSTALL_BIN) ./file/config/* $(1)/etc/aMule/config
endef

$(eval $(call BuildPackage,amule))
