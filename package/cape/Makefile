include $(TOPDIR)/rules.mk

# Build package version from git sha-1
PKG_NAME:=cape
PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=ssh://qca-git01.qualcomm.com:29418/streamboost/cape
PKG_BRANCH:=banana
PKG_START_ORDER:=49
PKG_RELEASE:=1

include $(INCLUDE_DIR)/local-development.mk
ifeq ($(DUMP)$(PKG_VERSION),)
PKG_REV:=381256a
PKG_VERSION:=g381256a
endif
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_MIRROR:=none
PKG_BUILD_DEPENDS:=argp-standalone

include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

# use SPACES here instead of tabs
define Package/cape
  CATEGORY:=Bigfoot
  SUBMENU:=Flow Detect
  TITLE:=Class Arbiter/Policy Engine
  DEPENDS:=+libsbdaemon +hiredis
  MAINTAINER:=Mo Chen <moc@qca.qualcomm.com>
endef

define Package/cape/description
CAPE chooses a policy for a given flow, given classification tags.
endef

# use TABs here instead of spaces
MAKE_FLAGS += \
	LDLIBS+="-largp"

# TABS
define Package/cape/install
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_DIR) $(1)/etc/appflow
	$(INSTALL_DIR) $(1)/etc/appflow/streamboost.d
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/cape     $(1)/usr/sbin/
	$(INSTALL_BIN) ./files/cape.init         $(1)/etc/appflow/streamboost.d/${PKG_START_ORDER}_cape
endef

# NO TABS, NO SPACES
$(eval $(call BuildPackage,cape))
