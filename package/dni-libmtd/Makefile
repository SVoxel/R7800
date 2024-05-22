# 
# Copyright (C) 2008 Delta Networks Inc.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=dni-libmtd
PKG_VERSION:=1
PKG_RELEASE:=1
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

TARGET_CFLAGS += -Wno-incompatible-pointer-types

define Package/dni-libmtd
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Tiny utils for configuring the Router.
endef

define Package/dni-libmtd/description
	This package contains an utility for configuring Home Router.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/InstallDev
	mkdir -p $(STAGING_DIR)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/libmtd.so $(STAGING_DIR)/usr/lib/
endef

define Package/dni-libmtd/install
	install -d -m0755 $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/libmtd.so $(1)/usr/lib
endef

$(eval $(call BuildPackage,dni-libmtd))
