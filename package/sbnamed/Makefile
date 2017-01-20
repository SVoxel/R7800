include $(TOPDIR)/rules.mk

# Build package version from git sha-1
PKG_NAME:=sbnamed
PKG_SOURCE_PROTO:=git
#PKG_SOURCE_URL:=ssh://qca-git01.qualcomm.com:29418/streamboost/sbnamed
PKG_SOURCE_URL:=ssh://qca-git01.qualcomm.com:29418/streamboost/sbnamed
PKG_BRANCH:=banana
PKG_START_ORDER:=32

include $(INCLUDE_DIR)/local-development.mk
ifeq ($(DUMP)$(PKG_VERSION),)
PKG_REV:=d9b6ddb
PKG_VERSION:=gd9b6ddb
endif
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_MIRROR:=none
PKG_BUILD_DEPENDS:=argp-standalone

include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

# use SPACES here instead of tabs
define Package/sbnamed
  CATEGORY:=Bigfoot
  SUBMENU:=Node Management
  TITLE:=sbnamed
  DEPENDS:=+libsbdaemon +hiredis +leash
  MAINTAINER:=Mo Chen <moc@qca.qualcomm.com>
endef

define Package/sbnamed/description
 sbnamed updates node names based on dhcp leases.
endef

# use TABs here instead of spaces
MAKE_FLAGS += \
	LDLIBS+="-largp"

# TABS
define Package/sbnamed/install
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_DIR) $(1)/etc/appflow
	$(INSTALL_DIR) $(1)/etc/appflow/streamboost.d
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/sbnamed     $(1)/usr/sbin/
	$(INSTALL_BIN) ./files/sbnamed.init         $(1)/etc/appflow/streamboost.d/${PKG_START_ORDER}_sbnamed
endef

# NO TABS, NO SPACES
$(eval $(call BuildPackage,sbnamed))
