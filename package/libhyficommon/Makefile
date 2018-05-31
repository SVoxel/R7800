# 
#  Copyright (C) 2007-2018 OpenWrt
#

include $(TOPDIR)/rules.mk

PKG_NAME:=libhyficommon
PKG_VERSION:=g4.0.1756.382
PKG_RELEASE:=1
PKG_BUILD_DEPENDS:=libc
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

PKG_GIT_TREEISH:=libhyficommon

include $(INCLUDE_DIR)/package.mk

define Package/libhyficommon
  SECTION:=QCA
  MAINTAINER:=Qualcomm Atheros, Inc.
  CATEGORY:=QCA Proprietary software
  TITLE:=This Package contains the Hy-Fi Common Functionality Library
endef

define Build/Prepare
	$(CP) $(GIT_HOME)/libhyficommon.git/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
endef

define Package/libhyficommon/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libhyficommon.so $(1)/usr/lib
endef

$(eval $(call BuildPackage,libhyficommon))
