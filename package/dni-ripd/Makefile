#
# Copyright (C) 2008 Delta Networks Inc.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=dni-ripd
PKG_VERSION:=0.01
PKG_RELEASE:=1
PKG_BUILD_DEPENDS:=datalib
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

PKG_GIT_TREEISH:=dni-rip-v1.0-dni25

include $(INCLUDE_DIR)/package.mk

define Package/dni-ripd
  SECTION:=dni
  CATEGORY:=DNI-Package
  DEPENDS:=@TARGET_ipq806x
  TITLE:=ripd
endef

define Build/Prepare
	$(CP) $(GIT_HOME)/dni-ripd.git/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
endef

define Package/dni-ripd/install
	install -d -m0755 $(1)/usr
	install -d -m0755 $(1)/usr/sbin
	install -m0755 $(PKG_BUILD_DIR)/ripd $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,dni-ripd))
