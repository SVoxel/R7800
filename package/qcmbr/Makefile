#
# Copyright (c) 2014 Qualcomm Atheros, Inc..
#
# All Rights Reserved.
# Qualcomm Atheros Confidential and Proprietary.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=qcmbr
PKG_VERSION:=10.4
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/local-development.mk
include $(INCLUDE_DIR)/package-version-override.mk

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  SECTION:=QCA
  CATEGORY:=QCA Proprietary software
  URL:=http://www.qca.qualcomm.com
  MAINTAINER:=Qualcomm Atheros
  TITLE:=$(1) - QCA ART Driver & tools
  DEPENDS:=+libpthread +libc +librt
endef

define Package/$(PKG_NAME)/description
  ART package support for 10.4
endef

define Package/$(PKG_NAME)/config
$(call Package/$(PKG_NAME)/override_source_path,$(PKG))
$(call Package/$(PKG_NAME)/override_version,$(PKG),$(PKG_SUPPORTED_VERSION))
endef

define Build/Prepare
	$(CP) $(GIT_HOME)/qcmbr.git/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/Qcmbr $(1)/usr/sbin
	$(INSTALL_BIN) ./files/qcmbr.init $(1)/etc/init.d/qcmbr
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
