include $(TOPDIR)/rules.mk

# Build package version from git sha-1
PKG_NAME:=flowman
PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=ssh://qca-git01.qualcomm.com:29418/streamboost/flowman.git
PKG_BRANCH:=banana
PKG_START_ORDER:=45
PKG_RELEASE:=2

include $(INCLUDE_DIR)/local-development.mk
ifeq ($(DUMP)$(PKG_VERSION),)
PKG_REV:=2b87ed0
PKG_VERSION:=g2b87ed0
endif
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_MIRROR:=none
PKG_BUILD_DEPENDS:=argp-standalone

include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

# use SPACES here instead of tabs
define Package/flowman
  CATEGORY:=Bigfoot
  SUBMENU:=Flow Detect
  TITLE:=Streamboost Flow Manager
  DEPENDS:=+hiredis +libsbdaemon
  MAINTAINER:=Cristian Prundeanu <prundeanu@qca.qualcomm.com>
endef

define Package/flowman/description
Flowman is Da Man in charge of flow creation, deletion and association with connections.
endef

# https://forum.openwrt.org/viewtopic.php?id=21953
TARGET_CFLAGS += -Wl,-rpath-link=$(STAGING_DIR)/usr/lib

# Note: -largp seems to be required with uclibc
MAKE_FLAGS += \
	EXTRA_LDLIBS+="-largp"

# TABS
define Package/flowman/install
	$(INSTALL_DIR) $(1)/etc/appflow/streamboost.d/
	$(INSTALL_DIR) $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/flowman $(1)/usr/sbin/
	$(INSTALL_BIN) ./files/flowman.init $(1)/etc/appflow/streamboost.d/${PKG_START_ORDER}_${PKG_NAME}
endef

# NO TABS, NO SPACES
$(eval $(call BuildPackage,flowman))
