#
# Copyright (C) 2011-2016 Entware
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=mp707
PKG_VERSION:=1.0.2-20171025
PKG_REV:=af6ab2c371c77a6237a90ae2b078db8f
PKG_RELEASE:=2

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_URL:=https://github.com/bubbafix/openwrt-bm1707
PKG_SOURCE_PROTO:=git
PKG_SOURCE_VERSION:=$(PKG_REV)

include $(INCLUDE_DIR)/package.mk

define Package/mp707
	SECTION:=utils
	CATEGORY:=Utilities
	DEPENDS:=+libusb-compat +libusb +libstdcpp
	TITLE:=USB thermometer RODOS-5 (MP707) utility
	URL:=$(PKG_SOURCE_URL)
	MAINTAINER:=Entware team, http://entware.net
endef

define Package/mp707/description
 This utility works with USB thermometer RODOS-5 (MP707).
 More information on device can be found at http://www.olimp-z.ru
endef

define Build/Compile
	$(TARGET_CXX) $(TARGET_CXXFLAGS) -c -I$(STAGING_DIR)/usr/include -o $(PKG_BUILD_DIR)/$(PKG_NAME).o $(PKG_BUILD_DIR)/package/bmtemp/src/bmtemp.cpp
	$(TARGET_CXX) $(TARGET_LDFLAGS) -L$(STAGING_DIR)/usr/lib -o $(PKG_BUILD_DIR)/bmtemp $(PKG_BUILD_DIR)/$(PKG_NAME).o -lusb -lusb-1.0
endef

define Package/mp707/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/bmtemp $(1)/usr/bin
endef

$(eval $(call BuildPackage,mp707))
