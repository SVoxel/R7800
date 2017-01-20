#
# Copyright (c) 2013 The Linux Foundation. All rights reserved.
# Copyright (C) 2011-2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#


include $(TOPDIR)/rules.mk

PKG_NAME:=jansson
PKG_VERSION:=2.4
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=http://www.digip.org/jansson/releases/
PKG_MD5SUM:=c4629b89bf0432f3158c461e88fe0113

PKG_INSTALL:=1
PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk

define Package/jansson
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=JSON library
endef

TARGET_CFLAGS += $(FPIC)
TARGET_LDFLAGS += -lm

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/{lib,include}
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libjansson* $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/include/* $(1)/usr/include/
endef

define Package/jansson/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libjansson*so* $(1)/usr/lib/
endef

$(eval $(call BuildPackage,jansson))
