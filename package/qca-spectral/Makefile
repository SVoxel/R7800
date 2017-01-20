#
# Copyright (c) 2014 Qualcomm Atheros, Inc.
#
# All Rights Reserved.
# Qualcomm Atheros Confidential and Proprietary.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=qca-spectral

PKG_VERSION:=10.4-4.0.5.020
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  SECTION:=QCA
  CATEGORY:=QCA Proprietary software
  URL:=http://www.qca.qualcomm.com
  MAINTAINER:=Qualcomm Atheros
  TITLE:=QCA Spectral Analysis
  DEPENDS:=kmod-qca-wifi +libpthread
endef

define KernelPackage/$(PKG_NAME)/config
$(call Package/$(PKG_NAME)/override_source_path,kmod-$(PKG_NAME))
$(call Package/$(PKG_NAME)/override_version,kmod-$(PKG_NAME),$(PKG_SUPPORTED_VERSION))
endef

MAKEOPTS+= \
	$(TARGET_CONFIGURE_OPTS) \
	COPTS="$(TARGET_CFLAGS)" \
	ATHEROSPATH=$(STAGING_DIR)/usr/include \
	ATH_SUPPORT_ICM=1 \
	ATH_SUPPORT_SPECTRAL=1 \
	QSDK_SPECTRAL=1

define Build/Prepare
	$(CP) $(GIT_HOME)/qca-spectral.git/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/athssd $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/icm $(1)/usr/sbin

	$(INSTALL_DIR) $(1)/lib/wifi
	$(INSTALL_BIN) ./files/icm.sh $(1)/lib/wifi

	$(INSTALL_DIR) $(1)/etc/ath
	$(INSTALL_DATA) ./files/icm.conf $(1)/etc/ath
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
