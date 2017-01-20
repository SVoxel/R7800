# 
# Copyright (C) 2007-2009 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=wxWidgets
PKG_VERSION:=2.8.12
PKG_RELEASE:=1

PKG_SOURCE:=wxWidgets-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=@SF/wxwindows
PKG_MD5SUM:=2fa39da14bc06ea86fe902579fedc5b1

PKG_BUILD_DIR:=$(BUILD_DIR)/wxWidgets-$(PKG_VERSION)
PKG_BUILD_DEPENDS:=uclibcxx

PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk

define Package/libwxwidgets
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=Cross-platform C++ framework
  URL:=http://www.wxwidgets.org/
  DEPENDS:=+libexpat +libstdcpp +zlib
endef

TARGET_CFLAGS += $(FPIC)
TARGET_CPPFLAGS += -I../uClibc++-0.2.2/include

# XXX: aMule *needs* the following configure options to be enabled:
#	--enable-intl
#	--enable-largefile
#	--disable-no_rtti
#	--with-regex
#	--enable-unicode
CONFIGURE_ARGS+= \
	--enable-shared \
	--disable-rpath \
	--with-gnu-ld \
	--disable-gui \
	--disable-monolithic \
	--enable-plugins \
	--disable-universal \
	--disable-nanox \
	--disable-gtk2 \
	--disable-gpe \
	--disable-optimise \
	--disable-debug \
	--disable-stl \
	--disable-omf \
	--disable-debug_flag \
	--disable-debug_info \
	--disable-debug_gdb \
	--disable-debug_cntxt \
	--disable-mem_tracing \
	--disable-profile \
	--disable-no_rtti \
	--disable-no_exceptions \
	--disable-compat22 \
	--disable-compat24 \
	--enable-intl \
	--disable-backtrace \
	--enable-exceptions \
	--disable-sound \
	--enable-largefile \
	\
	--without-gtk \
	--without-motif \
	--without-mac \
	--without-cocoa \
	--without-wine \
	--without-msw \
	--without-pm \
	--without-mgl \
	--without-microwin \
	--without-x11 \
	--without-libpng \
	--without-libjpeg \
	--without-libtiff \
	--without-libxpm \
	--without-libmspack \
	--without-sdl \
	--without-gnomeprint \
	--without-opengl \
	--without-dmalloc \
	--with-regex="sys" \
	--with-zlib="sys" \
	--without-odbc \
	--with-expat="sys" \
	--without-gtk-prefix \
	--without-x \
	--without-libiconv-prefix \
	--without-sdl-prefix \
	--without-cppunit-prefix \

UNICODE=yes

ifeq ($(UNICODE),yes)
  CONFIGURE_ARGS+= --enable-unicode
  LIB_PFX:=u
else
  CONFIGURE_ARGS+= --disable-unicode
  LIB_PFX:=
endif

define Package/libwxwidgets/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/* $(1)/usr/lib
endef

$(eval $(call BuildPackage,libwxwidgets))
