# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=curl
ifdef CONFIG_DNI_GREEN_DOWNLOAD
	PKG_VERSION:=7.23.1
else
	PKG_VERSION:=7.20.0
endif
PKG_RELEASE:=5

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_URL:=http://www.zlib.net @SF/zlib
PKG_MD5SUM:=3dda78c4a808d9a779dc3a2ae81b47d8

include $(INCLUDE_DIR)/package.mk

define Package/curl
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=Library implementing the deflate compression method
endef

define Package/curl/config
	menu "Configuration"
		depends on PACKAGE_curl
		source "$(SOURCE)/Config.in"
	endmenu
endef

define Build/Configure
	(cd $(PKG_BUILD_DIR); \
		$(TARGET_CONFIGURE_OPTS) \
		./configure \
			--host=$(GNU_TARGET_NAME) \
			--prefix=$(PKG_BUILD_DIR)/usr	\
	);
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		all

	mkdir -p $(PKG_INSTALL_DIR)
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		install
endef

define Build/InstallDev
	mkdir -p $(1)/usr/include/curl
	$(CP)	$(PKG_BUILD_DIR)/usr/include/curl/*.h \
		$(1)/usr/include/curl
	mkdir -p $(1)/usr/lib
	$(CP)	$(PKG_BUILD_DIR)/usr/lib/libcurl.{a,so*} \
		$(1)/usr/lib/
	mkdir -p $(1)/usr/lib/pkgconfig
	$(CP)	$(PKG_BUILD_DIR)/usr/lib/pkgconfig/libcurl.pc \
		$(1)/usr/lib/pkgconfig/
	$(SED) 's#$(TARGET_LDFLAGS)##g' $(1)/usr/lib/pkgconfig/libcurl.pc
endef

# libz.so is needed for openssl (zlib-dynamic)
define Package/curl/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/usr/lib/libcurl.so* $(1)/usr/lib/
endef

$(eval $(call BuildPackage,curl))
