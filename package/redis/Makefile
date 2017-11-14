#
# Copyright (C) 2011-2017 Entware
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# Partially taken from https://github.com/pdf/openwrt-14.07-x86_64-packages/tree/master/net/redis

include $(TOPDIR)/rules.mk

PKG_NAME:=redis
PKG_VERSION:=4.0.2
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=http://download.redis.io/releases
PKG_MD5SUM:=f0497cc1311cd10dfdf215e9e6fd7416
PKG_START_ORDER:=01

PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk

define Package/redis
	SECTION:=utils
	CATEGORY:=Utilities
	SUBMENU:=database
	TITLE:=in-memory key-value cache and store
	DEPENDS:=+libpthread
	URL:=http://redis.io/
	MAINTAINER:=Entware team, http://entware.net
endef

define Package/redis/description
 Redis is an open source (BSD licensed), in-memory data structure store,
 used as database, cache and message broker.
endef

MAKE_FLAGS += \
	MALLOC="libc" \
	PREFIX="$(PKG_INSTALL_DIR)/usr"

TARGET_CFLAGS += -ffunction-sections -fdata-sections
TARGET_LDFLAGS += -ldl -Wl,--gc-sections

define Build/Compile
	sed -e 's#^\(REAL_CFLAGS.* \)$$$$(ARCH)#\1#g' -i $(PKG_BUILD_DIR)/deps/hiredis/Makefile
	$(call Build/Compile/Default,-C $(PKG_BUILD_DIR)/deps hiredis linenoise lua PLAT=linux)
	$(call Build/Compile/Default)
endef

define Package/redis/conffiles
/etc/appflow/redis.conf
endef

define Package/redis/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(CP) $(PKG_INSTALL_DIR)/usr/bin/redis-server $(1)/usr/bin
	$(CP) $(PKG_INSTALL_DIR)/usr/bin/redis-cli $(1)/usr/bin
	$(INSTALL_DIR) $(1)/etc/appflow
	$(INSTALL_DATA) ./files/redis.conf $(1)/etc/appflow/redis.conf
	$(INSTALL_DIR) $(1)/etc/appflow/streamboost.d
	$(INSTALL_BIN) ./files/redis.init $(1)/etc/appflow/streamboost.d/${PKG_START_ORDER}_${PKG_NAME}
endef

$(eval $(call BuildPackage,redis))
