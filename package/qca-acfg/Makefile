#
# Copyright (c) 2014 Qualcomm Atheros, Inc.
#
# All Rights Reserved.
# Qualcomm Atheros Confidential and Proprietary.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=qca-acfg

PKG_VERSION:=10.4-4.0.5.020
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

PKG_BUILD_DEPENDS:=qca-wifi-10.4

define Package/$(PKG_NAME)
  SECTION:=QCA
  CATEGORY:=QCA Proprietary software
  URL:=http://www.qca.qualcomm.com
  MAINTAINER:=Qualcomm Atheros
  TITLE:=QCA Configuration Utility
  DEPENDS:=kmod-qca-wifi @(!(PACKAGE_hostapd))
endef

define Package/$(PKG_NAME)/config
$(call Package/$(PKG_NAME)/override_source_path,$(PKG_NAME))
$(call Package/$(PKG_NAME)/override_version,$(PKG_NAME),$(PKG_SUPPORTED_VERSION))
endef

define Package/$(PKG_NAME)/description
 QCA configuration utility provides a set of library
 API's through which user applications can configure
 and register events for wifi driver and hostap
endef

MAKEOPTS+= \
	$(TARGET_CONFIGURE_OPTS) \
	WLAN_TOP=$(STAGING_DIR)/usr \
	COPTS="$(TARGET_CFLAGS) -I. -I$(PKG_BUILD_DIR)/include" \
	QSDK_ACFG=1

define Build/Prepare
	$(CP) $(GIT_HOME)/qca-acfg.git/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/acfg_test_app/acfg_tool $(1)/usr/sbin
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
