include $(TOPDIR)/rules.mk

PKG_NAME:=tresbar
PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=ssh://qca-git01.qualcomm.com:29418/streamboost/tresbar
PKG_BRANCH:=banana
PKG_RELEASE:=1

include $(INCLUDE_DIR)/local-development.mk
ifeq ($(DUMP)$(PKG_VERSION),)
PKG_REV:=dea08b4
PKG_VERSION:=gdea08b4
endif
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_MIRROR:=none

include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

define Package/tresbar
  SECTION:=language-javascript
  CATEGORY:=Languages
  SUBMENU:=JavaScript
  TITLE:=3d bar charts for d3.js
  DEPENDS:=+d3js
endef

define Package/tresbar/description
3D bar chart d3 reusable component
endef

define Build/Configure
endef

define Build/Compile
endef

define Package/tresbar/install
	$(INSTALL_DIR) $(1)/www/js/
	cp $(PKG_BUILD_DIR)/tresbar.js $(1)/www/js/
endef

$(eval $(call BuildPackage,tresbar))
