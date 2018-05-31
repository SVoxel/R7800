# 
#  Copyright (C) 2007-2018 OpenWrt
#

include $(TOPDIR)/rules.mk

PKG_NAME:=qca-whc-init
PKG_VERSION:=4.0.1756.382
PKG_RELEASE:=1
PKG_BUILD_DEPENDS:=libc, luci, luci-mod-admin-core, luci-mod-admin-full
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

PKG_GIT_TREEISH:=$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  SECTION:=QCA
  MAINTAINER:=Qualcomm Atheros, Inc.
  CATEGORY:=QCA Proprietary software
  TITLE:=This Package contains the script helper files for the Whole Home Coverage init scripts
endef

define Build/Prepare
	$(CP) $(GIT_HOME)/qca-whc.git/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/lib/functions
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/whc-debug.sh $(1)/lib/functions
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/whc-iface.sh $(1)/lib/functions
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
