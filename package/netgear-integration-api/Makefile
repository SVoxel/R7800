# 
#  Copyright (C) 2014 Delta Networks Inc.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=netgear-integration-api
PKG_RELEASE:=1
PKG_BUILD_DEPENDS:=datalib
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/netgear-integration-api
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Netgear integration API
endef

define Build/Prepare
	$(CP) $(GIT_HOME)/netgear-integration-api.git/* $(PKG_BUILD_DIR)/
endef

define Build/Configure
endef

define Build/Compile
endef

define Package/netgear-integration-api/install
	$(MAKE) -C $(PKG_BUILD_DIR) install INS_DIR_PRE=$(1)
	ln -sf ntgr_sw_api $(1)/usr/sbin/api 
	ln -sf ntgr_sw_api $(1)/usr/sbin/app-register 
	ln -sf ntgr_sw_api $(1)/usr/sbin/dlna 
	ln -sf ntgr_sw_api $(1)/usr/sbin/firewall 
	ln -sf ntgr_sw_api $(1)/usr/sbin/internet 
	ln -sf ntgr_sw_api $(1)/usr/sbin/nvconfig
	ln -sf ntgr_sw_api $(1)/usr/sbin/system
endef

$(eval $(call BuildPackage,netgear-integration-api))
