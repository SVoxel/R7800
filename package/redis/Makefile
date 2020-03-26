include $(TOPDIR)/rules.mk

PKG_NAME:=redis
PKG_VERSION:=2.6.17
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=http://redis.googlecode.com/files/
PKG_MD5SUM:=918d74591e272e4419cba5beef67e995
PKG_CAT:=zcat
PKG_START_ORDER:=01

PKG_BUILD_DIR:=$(BUILD_DIR)/redis-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/redis-server
  SECTION:=utils
  CATEGORY:=Utilities
  SUBMENU:=database
  TITLE:=Redis
  MAINTAINER:=Mo Chen <moc@qualcomm.com>
  URL:=http://redis.io
  DEPENDS:=+libpthread +redis-cli
endef

define Package/redis-server/description
  Redis is an open source, BSD licensed, advanced key-value store. It is often referred to as a data structure server since keys can contain strings, hashes, lists, sets and sorted sets.
endef

define Package/redis-server/conffiles
/etc/appflow/redis.conf
endef

define Package/redis-cli
  SECTION:=utils
  CATEGORY:=Utilities
  SUBMENU:=database
  TITLE:=Redis
  MAINTAINER:=Mo Chen <moc@qualcomm.com>
  URL:=http://redis.io
  DEPENDS:=+libpthread
endef

CONFIG_CACHE_FILE:=$(shell mktemp --tmpdir redis-openwrt-config.cache.XXXXXX)

CONFIGURE_ARGS+= \
	--host=$(GNU_TARGET_NAME) \
	--cache-file=$(CONFIG_CACHE_FILE)
export CONFIGURE_ARGS

define Build/Compile
	cp ./files/config.cache $(CONFIG_CACHE_FILE)
	$(call Build/Compile/Default)
endef

define Package/redis-server/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/redis-server $(1)/usr/bin
	$(INSTALL_DIR) $(1)/etc/appflow
	$(INSTALL_DATA) ./files/redis.conf $(1)/etc/appflow/redis.conf
	$(INSTALL_DIR) $(1)/etc/appflow/streamboost.d
	$(INSTALL_BIN) ./files/redis.init $(1)/etc/appflow/streamboost.d/${PKG_START_ORDER}_${PKG_NAME}
endef

define Package/redis-cli/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/redis-cli $(1)/usr/bin
endef

$(eval $(call BuildPackage,redis-server))
$(eval $(call BuildPackage,redis-cli))
