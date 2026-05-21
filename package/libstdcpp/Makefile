#
# Copyright (C) 2007-2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=libstdcpp
PKG_VERSION:=4.8.5
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define Package/libstdcpp
  NAME:=libstdc++
  TITLE:=GNU Standard C++ Library v3
endef

define Build/Configure
endef

define Build/Compile
endef

define Package/libstdcpp/install
	$(INSTALL_DIR) $(1)/usr/lib/
	$(CP) $(PKG_BUILD_DIR)/usr/lib/* $(1)/usr/lib/
endef

$(eval $(call BuildPackage,libstdcpp))
