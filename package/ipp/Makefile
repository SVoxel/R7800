# 
# Copyright (C) 2007 OpenWrt.org
#

include $(TOPDIR)/rules.mk

PKG_NAME:=ipp
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/ipp
	SECTION:=dni
	CATEGORY:=DNI-Package
	TITLE:=IP setting tools for Home Router
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)
endef

define Build/InstallDev
	mkdir -p $(STAGING_DIR)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/libnetlink.so $(STAGING_DIR)/usr/lib/
endef

define Build/UninstallDev
	rm -f $(STAGING_DIR)/usr/lib/libnetlink.so
endef
			
define Package/ipp/install
	install -d -m0755 $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/libnetlink.so $(1)/usr/lib/
	install -d -m0755 $(1)/usr/sbin
	install -m0755 $(PKG_BUILD_DIR)/ipp $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,ipp))
