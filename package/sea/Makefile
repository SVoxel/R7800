include $(TOPDIR)/rules.mk

PKG_NAME:=sea
BIN_NAME:=sead
PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=ssh://qca-git01.qualcomm.com:29418/streamboost/$(PKG_NAME).git
PKG_BRANCH:=banana
PKG_RELEASE:=1

include $(INCLUDE_DIR)/local-development.mk
ifeq ($(DUMP)$(PKG_VERSION),)
PKG_REV:=16f93db
PKG_VERSION:=g16f93db
endif
PKG_START_ORDER:=25

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_MIRROR:=none

PKG_BUILD_DEPENDS:=argp-standalone

include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

# use SPACES here instead of tabs
define Package/$(PKG_NAME)
  CATEGORY:=Bigfoot
  SUBMENU:=Flow Detect
  TITLE:=Stats and Events Aggregator for StreamBoost
  DEPENDS:=+libsbdaemon +hiredis +libevent2-core +libuuid +jansson
  MAINTAINER:=Nic Costa <ncosta@qca.qualcomm.com>
endef

define Package/$(PKG_NAME)/description
SEA receives statistics and events from other StreamBoost utilities on various pubsub channels and maintains this data in a stats db area within a redis instance.
endef

# https://forum.openwrt.org/viewtopic.php?id=21953
TARGET_CFLAGS += -Wl,-rpath-link=$(STAGING_DIR)/usr/lib

# use TABs here instead of spaces
MAKE_FLAGS += \
	LDLIBS+="-largp"

# TABS
define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_DIR) $(1)/etc/appflow
	$(INSTALL_DIR) $(1)/etc/appflow/streamboost.d
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/$(BIN_NAME)     $(1)/usr/sbin/
	$(INSTALL_BIN) ./files/$(PKG_NAME).init         $(1)/etc/appflow/streamboost.d/${PKG_START_ORDER}_$(PKG_NAME)
endef

# NO TABS, NO SPACES
$(eval $(call BuildPackage,$(PKG_NAME)))
