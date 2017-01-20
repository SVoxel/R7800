include $(TOPDIR)/rules.mk

# Build package version from git sha-1
PKG_NAME:=luci-theme-steelblue
PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=ssh://qca-git01.qualcomm.com:29418/streamboost/luci-theme-steelblue
PKG_BRANCH:=banana
PKG_RELEASE:=2

include $(INCLUDE_DIR)/local-development.mk
ifeq ($(DUMP)$(PKG_VERSION),)
PKG_REV:=2c5071a
PKG_VERSION:=g2c5071a
endif
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_MIRROR:=none

include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  SECTION:=luci
  CATEGORY:=LuCI
  TITLE:=Luci Theme for Killer Router (left-menus)
  DEPENDS:=luci
  MAINTAINER:=Earl Manning <earl.manning@qca.qualcomm.com>
  SUBMENU:=4. Themes
endef

define Package/$(PKG_NAME)/description
Luci Theme for the Killer Router Look. (left-menus)
endef

define Build/Configure
endef

define Build/Compile
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/www
	$(INSTALL_DIR) $(1)/www/luci-static

	$(INSTALL_DIR) $(1)/usr
	$(INSTALL_DIR) $(1)/usr/lib/
	$(INSTALL_DIR) $(1)/usr/lib/lua
	$(INSTALL_DIR) $(1)/usr/lib/lua/luci

	$(INSTALL_DIR) $(1)/CONTROL

	## don't use '$(INSTALL_DATA)' because that doesn't do recursive
	$(CP) -aR $(PKG_BUILD_DIR)/root/* $(1)/
	$(CP) -aR $(PKG_BUILD_DIR)/htdocs/luci-static/* $(1)/www/luci-static
	$(CP) -aR $(PKG_BUILD_DIR)/luasrc/* $(1)/usr/lib/lua/luci
	$(CP) -a  $(PKG_BUILD_DIR)/ipkg/* $(1)/CONTROL/
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
