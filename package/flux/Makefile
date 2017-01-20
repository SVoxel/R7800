include $(TOPDIR)/rules.mk

PKG_NAME:=flux
PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=ssh://qca-git01.qualcomm.com:29418/streamboost/flux
PKG_BRANCH:=banana
PKG_RELEASE:=1

include $(INCLUDE_DIR)/local-development.mk
ifeq ($(DUMP)$(PKG_VERSION),)
PKG_REV:=a5a315d
PKG_VERSION:=ga5a315d
endif
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_MIRROR:=none

include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

define Package/flux
  SECTION:=language-javascript
  CATEGORY:=Languages
  SUBMENU:=JavaScript
  TITLE:=Killer Router Web UI Toolkit
  DEPENDS:=+jquery +jquery-ui +jquery-contextmenu +jquery-jscrollpane +jquery-swapsies +jquery-flot +jquery-flot-axislabels +jquery-flot-gant +jquery-sparkline +raphael +tresbar +angular +angular-mocks +angular-route +angular-translate +urijs +angular-ozker +angular-loadcontent
endef

define Package/flux/description
  Killer Router Web UI Toolkit
endef

define Build/Configure
endef

define Build/Compile
endef

define Package/flux/install
	$(INSTALL_DIR) $(1)/www/css/
	$(INSTALL_DIR) $(1)/www/debug/
	$(INSTALL_DIR) $(1)/www/debug/js/
	$(INSTALL_DIR) $(1)/www/debug/partials/
	$(INSTALL_DIR) $(1)/www/js/
	$(INSTALL_DIR) $(1)/www/js/BWV/
	$(INSTALL_DIR) $(1)/www/js/PRV/
	$(INSTALL_DIR) $(1)/www/trans/
	cp -r $(PKG_BUILD_DIR)/css/* $(1)/www/css/
	cp -d $(PKG_BUILD_DIR)/js/*.js $(1)/www/js/
	cp -d $(PKG_BUILD_DIR)/js/BWV/*.js $(1)/www/js/BWV/
	cp -d $(PKG_BUILD_DIR)/js/PRV/*.js $(1)/www/js/PRV/
	cp $(PKG_BUILD_DIR)/debug/*.html $(1)/www/debug/
	cp $(PKG_BUILD_DIR)/debug/partials/*.html $(1)/www/debug/partials/
	cp $(PKG_BUILD_DIR)/debug/js/*.js $(1)/www/debug/js/
	cp $(PKG_BUILD_DIR)/trans/* $(1)/www/trans/
endef

$(eval $(call BuildPackage,flux))
