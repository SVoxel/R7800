# 
#  Copyright (C) 2009 Delta Networks Inc.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=vmstat
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/vmstat
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=System CPU and memory monitor
endef

define Build/Prepare
	$(CP) ./src/* $(PKG_BUILD_DIR)
endef

define Build/Compile
endef

define Package/vmstat/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/vmstat $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,vmstat))
