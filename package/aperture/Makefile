include $(TOPDIR)/rules.mk

# Build package version from git sha-1
PKG_NAME:=aperture
PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=ssh://qca-git01.qualcomm.com:29418/streamboost/aperture
PKG_BRANCH:=banana
PKG_START_ORDER:=81
PKG_RELEASE:=2

include $(INCLUDE_DIR)/local-development.mk
ifeq ($(DUMP)$(PKG_VERSION),)
PKG_REV:=2e65dce
PKG_VERSION:=g2e65dce
endif
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_MIRROR:=none
PKG_BUILD_DEPENDS:=jansson argp-standalone curl libxml2

include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

# use SPACES here instead of tabs
define Package/$(PKG_NAME)/Default
  CATEGORY:=Bigfoot
  SUBMENU:=Utilities
  TITLE:=Automatic Bandwidth Monitor
  DEPENDS:=+jansson +curl +libxml2 +app-flow-scripts
  MAINTAINER:=Cristian Prundeanu <prundeanu@qca.qualcomm.com>
endef

define Package/$(PKG_NAME)
$(call Package/$(PKG_NAME)/Default)
  VARIANT:=full
  DEPENDS+=@!PACKAGE_bwestd
endef

# the noauto variant is only able to perform manual measurements
define Package/$(PKG_NAME)-noauto
$(call Package/$(PKG_NAME)/Default)
  VARIANT:=noauto
  PROVIDES:=$(PKG_NAME)
endef

define Package/$(PKG_NAME)/description
Aperture continuously monitors the available upload and download rate and
adjusts the TBF of the root interfaces accordingly.
endef

define Package/$(PKG_NAME)-noauto/description
$(call Package/$(PKG_NAME)/description)

This is the package without auto-start.
endef

# nothing to configure
define Build/Configure
endef

MAKE_FLAGS += LDLIBS+="-largp"

define Package/$(PKG_NAME)-noauto/install
	$(INSTALL_DIR) $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/etc/appflow/streamboost.d/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/$(PKG_NAME) $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/$(PKG_NAME)-controller.sh $(1)/usr/sbin/
	$(INSTALL_BIN) ./files/$(PKG_NAME).init $(1)/usr/sbin/
endef

define Package/$(PKG_NAME)/install
$(call Package/$(PKG_NAME)-noauto/install,$(1))
	$(INSTALL_BIN) ./files/$(PKG_NAME).init $(1)/etc/appflow/streamboost.d/${PKG_START_ORDER}_$(PKG_NAME)
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
$(eval $(call BuildPackage,$(PKG_NAME)-noauto))
