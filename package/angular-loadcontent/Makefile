include $(TOPDIR)/rules.mk

PKG_NAME:=angular-loadcontent
PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=ssh://qca-git01.qualcomm.com:29418/streamboost/angular-loadcontent
PKG_BRANCH:=banana
PKG_RELEASE:=1

include $(INCLUDE_DIR)/local-development.mk
ifeq ($(DUMP)$(PKG_VERSION),)
PKG_REV:=8402b96
PKG_VERSION:=g8402b96
endif
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_MIRROR:=none

include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

define Package/angular-loadcontent
  SECTION:=language-javascript
  CATEGORY:=Languages
  SUBMENU:=JavaScript
  TITLE:=Angular media loader
  DEPENDS:=+jquery +angular +angular-mocks +angular-route +angular-translate +urijs
endef

define Package/angular-loadcontent/description
  Angular media loader
endef

define Build/Configure
endef

define Build/Compile
endef

define Package/angular-loadcontent/install
	$(INSTALL_DIR) $(1)/www/js/
	cp $(PKG_BUILD_DIR)/angular-fallback-loader.js $(1)/www/js/
	cp $(PKG_BUILD_DIR)/angular-icon-loader.js $(1)/www/js/
	cp $(PKG_BUILD_DIR)/angular-translate-partial-fallback-loader.js $(1)/www/js/
endef

$(eval $(call BuildPackage,angular-loadcontent))
