# 
#  Copyright (C) 2007-2009 Delta Networks Inc.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=net-scan
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

PKG_GIT_TREEISH:=net-scan-v1.0-dni25

include $(INCLUDE_DIR)/package.mk

define Package/net-scan
	SECTION:=utils
	CATEGORY:=Network
	DEPENDS:=@TARGET_ipq806x
	TITLE:=Scanning the attached devices according to NETGEAR's spec.
endef

define Build/Prepare
	$(CP) $(GIT_HOME)/net-scan.git/* $(PKG_BUILD_DIR)/
endef

define Package/net-scan/install
	install -d -m0755 $(1)/etc/init.d
	install -m0755 ./files/net-scan.init $(1)/etc/init.d/net-scan
	install -d -m0755 $(1)/usr/sbin
	install -m0755 $(PKG_BUILD_DIR)/net-scan $(1)/usr/sbin/
	install -m0755 $(PKG_BUILD_DIR)/dev-scan $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,net-scan))
