include $(TOPDIR)/rules.mk

# Build package version from git sha-1
PKG_NAME:=overlord
PKG_BRANCH:=banana
PKG_RELEASE:=1
PKG_START_ORDER:=44

include $(INCLUDE_DIR)/local-development.mk
ifeq ($(DUMP)$(PKG_VERSION),)
PKG_REV:=d4a80eb
PKG_VERSION:=gd4a80eb
endif
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_MIRROR:=none
PKG_BUILD_DEPENDS:=argp-standalone

include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

# use SPACES here instead of tabs
define Package/overlord
  CATEGORY:=Bigfoot
  SUBMENU:=Bandwidth Control
  TITLE:=Overlord
  DEPENDS:=+libsbdaemon +hiredis +jansson
  MAINTAINER:=Mo Chen <moc@qca.qualcomm.com>
endef

define Package/overlord/description
Overlord manages bandwidth allocation by computing the current policy profile for each flow.
endef

# use TABs here instead of spaces
MAKE_FLAGS += \
	LDLIBS+="-largp"

# TABS
define Build/Configure
endef

define Build/Compile
endef

define Package/overlord/install
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_DIR) $(1)/etc/appflow
	$(INSTALL_DIR) $(1)/etc/appflow/streamboost.d
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) ./files/overlord $(1)/usr/sbin/
	$(INSTALL_BIN) ./files/overlord.init $(1)/etc/appflow/streamboost.d/${PKG_START_ORDER}_overlord
endef

# NO TABS, NO SPACES
$(eval $(call BuildPackage,overlord))
