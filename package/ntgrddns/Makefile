# 
# Copyright (C) 2013 Delta Networks Inc.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=ntgrddns
PKG_VERSION:=1
PKG_RELEASE:=1
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PKG_GIT_TREEISH=3e859af53f590d74665572881ef6d97312877921

include $(INCLUDE_DIR)/package.mk

define Package/ntgrddns
  SECTION:=utils
  CATEGORY:=Network
  DEPENDS:=@TARGET_ipq806x
  TITLE:=Tiny ddns util for Netgear Home Router
endef

define Package/ntgrddns/description
	This package contains a tiny ddns util for Netgear Home Router.
endef

define Build/Prepare
	$(CP) $(GIT_HOME)/ntgrddns.git/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
endef

define Package/ntgrddns/install
	install -d -m0755 $(1)/usr/sbin
	install -m0755 $(PKG_BUILD_DIR)/ntgrddns $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,ntgrddns))
