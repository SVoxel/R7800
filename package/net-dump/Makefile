# 
# Copyright (C) 2008 OpenWrt.org
#

include $(TOPDIR)/rules.mk

PKG_NAME:=net-dump
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/net-dump
	SECTION:=utils
	CATEGORY:=Network
	TITLE:=Mini dump tool to capture packets.
endef


define Package/net-dump/description
	It is used for NTGR's Router Debug mode.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)
endef

define Package/net-dump/install
	install -d -m0755 $(1)/usr/sbin
	install -m0755 $(PKG_BUILD_DIR)/net-dump $(1)/usr/sbin/
	install -d -m0755 $(1)/sbin
	install -m0755 ./file/wan_debug $(1)/sbin/
endef

$(eval $(call BuildPackage,net-dump))
