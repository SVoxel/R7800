include $(TOPDIR)/rules.mk

# Build package version from git sha-1
PKG_NAME:=sbsaved
PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=ssh://qca-git01.qualcomm.com:29418/streamboost/sbsaved
PKG_BRANCH:=banana
PKG_START_ORDER:=11

include $(INCLUDE_DIR)/local-development.mk
ifeq ($(DUMP)$(PKG_VERSION),)
PKG_REV:=c54fe0b
PKG_VERSION:=gc54fe0b
endif
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_MIRROR:=none
PKG_BUILD_DEPENDS:=argp-standalone

include $(INCLUDE_DIR)/local-development.mk
include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

# use SPACES here instead of tabs
define Package/sbsaved
  CATEGORY:=Bigfoot
  SUBMENU:=Utils
  TITLE:=Streamboost persistence daemon
  DEPENDS:=+libsbdaemon +hiredis +libevent2
  MAINTAINER:=Mo Chen <moc@qca.qualcomm.com>
endef

define Package/sbsaved/description
sbsaved stores redis keys to a file, triggered by pubsub channels, and defined as a keyset and key patterns.
endef

# use TABs here instead of spaces
MAKE_FLAGS += \
	LDLIBS+="-largp"

# TABS
define Package/sbsaved/install
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_DIR) $(1)/etc/appflow
	$(INSTALL_DIR) $(1)/etc/appflow/streamboost.d
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/sbsaved     $(1)/usr/sbin/
	$(INSTALL_BIN) ./files/sbsaved.init         $(1)/etc/appflow/streamboost.d/${PKG_START_ORDER}_sbsaved
	$(INSTALL_BIN) ./files/persistence.sh         $(1)/etc/appflow/
endef

# NO TABS, NO SPACES
$(eval $(call BuildPackage,sbsaved))
