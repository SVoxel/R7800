#
# Copyright (c) 2013 Qualcomm Atheros, Inc.
#
# All Rights Reserved.
# Qualcomm Atheros Confidential and Proprietary.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=qca-ssdk-shell
PKG_SOURCE_PROTO:=git
PKG_BRANCH:=master
PKG_RELEASE:=1

LOCAL_VARIANT=$(patsubst qca-ssdk-shell-%,%,$(patsubst qca-ssdk-shell-%,%,$(BUILD_VARIANT)))
include $(INCLUDE_DIR)/package.mk

define Package/qca-ssdk-shell
  SECTION:=QCA
  CATEGORY:=QCA Proprietary software
  URL:=http://www.qca.qualcomm.com
  MAINTAINER:=Qualcomm Atheros
  SUBMENU:=QCA SSDK
  TITLE:=Shell application for QCA SSDK
endef


define Package/qca-ssdk-shell/Description
  This package contains a qca-ssdk shell application for QCA chipset
endef


QCASSDK_CONFIG_OPTS+= TOOL_PATH=$(TOOLCHAIN_DIR)/bin/ \
                SYS_PATH=$(LINUX_DIR) \
                TOOLPREFIX=$(TARGET_CROSS) \
                KVER=$(LINUX_VERSION) \
                ARCH=$(LINUX_KARCH)

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) $(strip $(QCASSDK_CONFIG_OPTS))
endef

define Package/qca-ssdk-shell/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/build/bin/ssdk_sh $(1)/usr/sbin/
endef


$(eval $(call BuildPackage,qca-ssdk-shell))
