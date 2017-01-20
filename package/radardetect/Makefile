#
# Copyright (c) 2014 Qualcomm Atheros, Inc..
#
# All Rights Reserved.
# Qualcomm Atheros Confidential and Proprietary.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=radardetect
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/local-development.mk
include $(INCLUDE_DIR)/package-version-override.mk

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  SECTION:=net
  CATEGORY:=Network
  TITLE:=DNI radardetect tool
endef

define Package/$(PKG_NAME)/description
  timer tool for DFS channels
endef

define Package/$(PKG_NAME)/config
        menu "Configurations"
                depends on PACKAGE_radardetect
        endmenu
endef

define Build/Prepare
	$(CP) $(GIT_HOME)/radardetect.git/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/radardetect $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/radardetect_cli $(1)/usr/sbin
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
