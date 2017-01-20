# 
# Copyright (C) 2008 Delta Networks Inc.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=led-control
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/led-control
  SECTION:=utils
  CATEGORY:=Base system
  DEFAULT:=y
  TITLE:=Update utility for LED control
endef

define Build/Prepare
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
endef

define Package/led-control/install
	install -d -m0755 $(1)/sbin
	install -m0755 $(PKG_BUILD_DIR)/ledcontrol $(1)/sbin/
endef

$(eval $(call BuildPackage,led-control))
