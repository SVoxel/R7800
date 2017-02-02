# 
# Copyright (C) 2006-2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=bridge-utils
PKG_RELEASE:=1
PKG_SOURCE_URL:=@SF/bridge
PKG_VERSION:=1.6
PKG_MD5SUM:=f369e90e85e4bb46baa26a7b9d66b578

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz

include $(INCLUDE_DIR)/package.mk

define Package/bridge
  SECTION:=net
  CATEGORY:=Base system
  TITLE:=Ethernet bridging configuration utility
  URL:=http://bridge.sourceforge.net/
endef

define Package/bridge/description
 Manage ethernet bridging: a way to connect networks together to 
 form a larger network.
endef

CONFIGURE_ARGS += \
	--with-linux-headers="$(LINUX_DIR)" \

define Build/Prepare
$(call Build/Prepare/Default)
	( cd $(PKG_BUILD_DIR) ; \
		[ -f ./configure ] || { \
			autoconf ; \
		} \
	)
endef

define Package/bridge/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/brctl/brctl $(1)/usr/sbin
endef

$(eval $(call BuildPackage,bridge))
