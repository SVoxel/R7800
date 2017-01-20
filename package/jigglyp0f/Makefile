include $(TOPDIR)/rules.mk

# Build package version from git sha-1
PKG_NAME:=jigglyp0f
PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=ssh://qca-git01.qualcomm.com:29418/streamboost/jigglyp0f
PKG_BRANCH:=banana
PKG_RELEASE:=2

include $(INCLUDE_DIR)/local-development.mk
ifeq ($(DUMP)$(PKG_VERSION),)
PKG_REV:=ae11533
PKG_VERSION:=gae11533
endif
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_MIRROR:=none

PKG_START_ORDER:=34

include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

# use SPACES here instead of tabs
define Package/jigglyp0f
  CATEGORY:=Bigfoot
  SUBMENU:=Node Management
  TITLE:=p0f to Redis interface
  DEPENDS:=+p0f +libevent2 +hiredis +libsbdaemon
  MAINTAINER:=Mo Chen <moc@qca.qualcomm.com>
endef

define Package/jigglyp0f/description
An interface to translate p0f stdout to Redis pubsub.
endef

# use TABs here instead of spaces
MAKE_FLAGS += \
	LDLIBS+="-largp"

define Package/jigglyp0f/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/jigglyp0f $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/etc/appflow/streamboost.d
	$(INSTALL_BIN) ./files/$(PKG_NAME).init $(1)/etc/appflow/streamboost.d/${PKG_START_ORDER}_$(PKG_NAME)
endef

$(eval $(call BuildPackage,jigglyp0f))
