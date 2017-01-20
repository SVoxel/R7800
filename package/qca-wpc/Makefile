#
# Copyright (c) 2014 Qualcomm Atheros, Inc
#
# All Rights Reserved.
# Qualcomm Atheros Confidential and Proprietary.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=qca-wpc

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
  TITLE:=QCA Wifi Positioning
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
	ATH_SUPPORT_WIFIPOS=1

define Build/Prepare
	$(CP) $(GIT_HOME)/qca-wpc.git/* $(PKG_BUILD_DIR)/
endef

define Build/RunMake
	CFLAGS="$(TARGET_CPPFLAGS) $(TARGET_CFLAGS)" \
	$(MAKE) -C $(PKG_BUILD_DIR)/$(1) \
	$(MAKEOPTS) \
	$(TARGET_CONFIGURE_OPTS) \
	LIBS="$(TARGET_LDFLAGS)" \
	$(2)
endef

define Build/Compile/$(PKG_NAME)
	$(call Build/RunMake,wpc, \
		wpc \
	)
endef

define Build/Compile
	$(Build/Compile/$(PKG_NAME))
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/wpc/wpc $(1)/usr/sbin

	$(INSTALL_DIR) $(1)/lib/wifi
	$(INSTALL_BIN) ./files/wpc.sh $(1)/lib/wifi
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
