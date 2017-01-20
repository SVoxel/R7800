include $(TOPDIR)/rules.mk

PKG_NAME:=angular-ozker
PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=ssh://qca-git01.qualcomm.com:29418/streamboost/angular-ozker
PKG_BRANCH:=banana
PKG_RELEASE:=1

include $(INCLUDE_DIR)/local-development.mk
ifeq ($(DUMP)$(PKG_VERSION),)
PKG_REV:=04154c5
PKG_VERSION:=g04154c5
endif
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_MIRROR:=none

include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

define Package/angular-ozker
  SECTION:=language-javascript
  CATEGORY:=Languages
  SUBMENU:=JavaScript
  TITLE:=Angular ozker model components
  DEPENDS:=+jquery +angular +angular-mocks +angular-route +angular-translate +urijs
endef

define Package/angular-ozker/description
  Angular ozker model components
endef

define Build/Configure
endef

define Build/Compile
endef

define Package/angular-ozker/install
	$(INSTALL_DIR) $(1)/www/js/
	cp $(PKG_BUILD_DIR)/angular-ozker.js $(1)/www/js/
	cp $(PKG_BUILD_DIR)/angular-poll.js $(1)/www/js/
	cp $(PKG_BUILD_DIR)/angular-rate.js $(1)/www/js/
	cp $(PKG_BUILD_DIR)/angular-stats.js $(1)/www/js/
endef

$(eval $(call BuildPackage,angular-ozker))
