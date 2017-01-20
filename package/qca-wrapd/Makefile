#
# Copyright (c) 2014 Qualcomm Atheros, Inc.
#
# All Rights Reserved.
# Qualcomm Atheros Confidential and Proprietary.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=qca-wrapd

PKG_VERSION:=10.4-4.0.1756.054
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

PKG_BUILD_DEPENDS:=qca-hostap-10.4

define Package/$(PKG_NAME)
  SECTION:=QCA
  CATEGORY:=QCA Proprietary software
  URL:=http://www.qca.qualcomm.com
  MAINTAINER:=Qualcomm Atheros
  TITLE:=QCA wrapd daemon
  DEPENDS:=qca-hostap @(!(PACKAGE_hostapd))
endef

define Package/$(PKG_NAME)/config
$(call Package/$(PKG_NAME)/override_source_path,$(PKG_NAME))
$(call Package/$(PKG_NAME)/override_version,$(PKG_NAME),$(PKG_SUPPORTED_VERSION))
endef

define Package/$(PKG_NAME)/description
  User Space application to support QWRAP feature
endef

MAKEOPTS+= \
	$(TARGET_CONFIGURE_OPTS) \
	COPTS="$(TARGET_CFLAGS)" \
	WPA_PATH=$(STAGING_DIR)/usr/lib \
	QSDK_WRAPD=$(STAGING_DIR)/usr/include

define Build/Prepare
	$(CP) $(GIT_HOME)/qca-wrapd.git/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/wrapd $(1)/usr/sbin

	$(INSTALL_DIR) $(1)/etc/ath
	$(INSTALL_DATA) ./files/wrap-vma.conf $(1)/etc/ath

	$(INSTALL_DIR) $(1)/lib/wifi
	$(INSTALL_DATA) ./files/qwrap.sh $(1)/lib/wifi
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
