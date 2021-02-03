#
# Copyright (C) 2020-2021 Voxel
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=iprange
PKG_VERSION:=1.0.4
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.xz
PKG_MD5SUM:=f3126b8c239eaa51d79591748f091c7b

PKG_LICENSE:=GPL-2.0-or-later

PKG_INSTALL:=1
PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk

TARGET_CFLAGS := $(filter-out -O%,$(TARGET_CFLAGS)) -Os
TARGET_CFLAGS += -ffunction-sections -fdata-sections -flto
TARGET_LDFLAGS += -Wl,--gc-sections -flto

define Package/iprange
  SECTION:=net
  CATEGORY:=Network
  DEPENDS:=+libpthread
  TITLE:=iprange: small utility to manage IP ranges
  URL:=https://github.com/firehol/iprange
endef

define Package/iprange/description
  Small utility to manage IP ranges  
endef

define Package/iprange/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(CP) $(PKG_INSTALL_DIR)/usr/bin/$(PKG_NAME) $(1)/usr/bin/
endef

$(eval $(call BuildPackage,iprange))
