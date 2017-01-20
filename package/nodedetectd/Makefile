include $(TOPDIR)/rules.mk

# Build package version from git sha-1
PKG_NAME:=nodedetectd
PKG_SOURCE_PROTO:=git
#PKG_SOURCE_URL:=ssh://qca-git01.qualcomm.com:29418/streamboost/nodedetectd
PKG_SOURCE_URL:=ssh://qca-git01.qualcomm.com:29418/streamboost/nodedetectd
PKG_BRANCH:=banana
PKG_RELEASE:=1
PKG_START_ORDER:=31

include $(INCLUDE_DIR)/local-development.mk
ifeq ($(DUMP)$(PKG_VERSION),)
PKG_REV:=bfefe1a
PKG_VERSION:=gbfefe1a
endif

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_MIRROR:=none
PKG_BUILD_DEPENDS:=argp-standalone

include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

# use SPACES here instead of tabs
define Package/nodedetectd
  CATEGORY:=Bigfoot
  SUBMENU:=Node Management
  TITLE:=Nodedetectd
  DEPENDS:=+libsbdaemon +hiredis
  MAINTAINER:=Mo Chen <moc@qca.qualcomm.com>
endef

define Package/nodedetectd/description
Nodedetectd computes node types using characteristics sensed by other daemons.
endef

# use TABs here instead of spaces
MAKE_FLAGS += \
	LDLIBS+="-largp"

# TABS
define Package/nodedetectd/install
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_DIR) $(1)/etc/appflow
	$(INSTALL_DIR) $(1)/etc/appflow/streamboost.d
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/nodedetectd     $(1)/usr/sbin/
	$(INSTALL_BIN) ./files/nodedetectd.init         $(1)/etc/appflow/streamboost.d/${PKG_START_ORDER}_nodedetectd
endef

# NO TABS, NO SPACES
$(eval $(call BuildPackage,nodedetectd))
