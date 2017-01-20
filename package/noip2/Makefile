# 
#  Copyright (C) 2012 Delta Networks Inc.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=noip2
PKG_RELEASE:=1
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/noip2
	SECTION:=utils
	CATEGORY:=Network
	TITLE:=no-ip DDNS utility.
endef

define Build/Prepare
	$(CP) -r ./src/* $(PKG_BUILD_DIR)/
endef

define Package/noip2/install
	install -d -m0755 $(1)/usr/sbin
	install -m0755 $(PKG_BUILD_DIR)/noip2 $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,noip2))
