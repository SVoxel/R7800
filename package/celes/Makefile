include $(TOPDIR)/rules.mk

PKG_NAME:=celes
PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=ssh://qca-git01.qualcomm.com:29418/streamboost/$(PKG_NAME).git
PKG_BRANCH:=banana
PKG_RELEASE:=1

include $(INCLUDE_DIR)/local-development.mk
ifeq ($(DUMP)$(PKG_VERSION),)
PKG_REV:=306e753
PKG_VERSION:=g306e753
endif
PKG_START_ORDER:=26

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
  TITLE:=Connection Event Logger Service
  DEPENDS:=+libsbdaemon +hiredis +libnetfilter-conntrack +libevent2
  MAINTAINER:=Mo Chen <moc@qca.qualcomm.com>
endef

define Package/$(PKG_NAME)/description
SEA receives statistics and events from conntrack and maintains this data in an event db in Redis.
endef

# https://forum.openwrt.org/viewtopic.php?id=21953
TARGET_CFLAGS += -Wl,-rpath-link=$(STAGING_DIR)/usr/lib

# use TABs here instead of spaces
MAKE_FLAGS += \
	LDLIBS+="-largp"

# NO TABS, NO SPACES
$(eval $(call BuildPackage,$(PKG_NAME)))
