include $(TOPDIR)/rules.mk

# Build package version from git sha-1
PKG_NAME:=luci-app-streamboost
PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=ssh://qca-git01.qualcomm.com:29418/streamboost/luci-app-streamboost
PKG_BRANCH:=banana
PKG_RELEASE:=2

include $(INCLUDE_DIR)/local-development.mk
ifeq ($(DUMP)$(PKG_VERSION),)
PKG_REV:=9b80471
PKG_VERSION:=g9b80471
endif
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_MIRROR:=none

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)/Default
  SECTION:=luci
  CATEGORY:=LuCI
  TITLE:=Luci Application for Streamboost
  DEPENDS:=luci +flux +background-size-polyfill
  MAINTAINER:=Earl Manning <earl.manning@qca.qualcomm.com>
  SUBMENU:=3. Applications
  MENU:=1
endef

define Package/$(PKG_NAME)
  $(call Package/$(PKG_NAME)/Default)
  VARIANT:=premium
endef

define Package/$(PKG_NAME)-mainstream
  $(call Package/$(PKG_NAME)/Default)
  TITLE += ( Mainstream)
  VARIANT:=mainstream
endef

define Package/$(PKG_NAME)-assets
  SECTION:=luci
  CATEGORY:=LuCI
  TITLE:=Static assets for Streamboost Luci Application
  DEPENDS:=luci +luci-app-streamboost
  MAINTAINER:=Earl Manning <earl.manning@qca.qualcomm.com>
  SUBMENU:=3. Applications
endef

define Package/$(PKG_NAME)/description
Luci Application for Monitoring and Management of Streamboost
endef

define Package/$(PKG_NAME)-mainstream/description
Luci Application for Streamboost Mainstream
endef

define Package/$(PKG_NAME)-assets/description
Static assets for the Streamboost Luci Application.
endef

include $(INCLUDE_DIR)/package-version-override.mk

define Build/Configure
endef

define Build/Compile
	## This will run the JSON files through the packager to make JSONp files.
	make -C $(PKG_BUILD_DIR)/htdocs/luci-static/resources/streamboost/trans all
endef


## don't use '$(INSTALL_DATA)' because that doesn't do recursive
define Package/$(PKG_NAME)/Default/install
	$(INSTALL_DIR) $(1)/usr
	$(INSTALL_DIR) $(1)/usr/lib/
	$(INSTALL_DIR) $(1)/usr/lib/lua
	$(INSTALL_DIR) $(1)/usr/lib/lua/luci
	$(CP) -aR $(PKG_BUILD_DIR)/luasrc/* $(1)/usr/lib/lua/luci

	$(INSTALL_DIR) $(1)/www
	$(CP) -a $(PKG_BUILD_DIR)/htdocs/debug.html $(1)/www
	$(INSTALL_DIR) $(1)/www/js
	$(CP) $(PKG_BUILD_DIR)/htdocs/js/* $(1)/www/js

	$(INSTALL_DIR) $(1)/www/luci-static
	$(INSTALL_DIR) $(1)/www/luci-static/resources
	$(INSTALL_DIR) $(1)/www/luci-static/resources/streamboost
	$(INSTALL_DIR) $(1)/www/luci-static/resources/streamboost/js
	echo 'var g_strManufacturer=$(CONFIG_KROUTER_CUSTOMER);' \
		>  $(1)/www/luci-static/resources/streamboost/js/ident.js
	echo 'var g_strBoard=$(CONFIG_KROUTER_BOARD);' \
		>> $(1)/www/luci-static/resources/streamboost/js/ident.js

	$(INSTALL_DIR) $(1)/www/luci-static/resources/streamboost/css
	$(CP) -aR $(PKG_BUILD_DIR)/htdocs/luci-static/resources/streamboost/css/* $(1)/www/luci-static/resources/streamboost/css

	$(INSTALL_DIR) $(1)/www/luci-static/resources/streamboost/images
	$(CP) -aR $(PKG_BUILD_DIR)/htdocs/luci-static/resources/streamboost/images/* $(1)/www/luci-static/resources/streamboost/images
	cd $(1)/www/; ln -s luci-static/resources/streamboost/images images

	$(INSTALL_DIR) $(1)/etc/uci-defaults
	$(CP) -a ./files/etc/uci-defaults/* $(1)/etc/uci-defaults/
endef

define Package/$(PKG_NAME)/install
	$(call Package/$(PKG_NAME)/Default/install,$(1))
	rm $(1)/usr/lib/lua/luci/controller/streamboost/sbmainstream.lua
	rm $(1)/usr/lib/lua/luci/controller/streamboost/nostreamboost.lua
endef

define Package/$(PKG_NAME)-mainstream/install
	$(call Package/$(PKG_NAME)/Default/install,$(1))
	rm $(1)/usr/lib/lua/luci/controller/streamboost/streamboost.lua
	rm $(1)/usr/lib/lua/luci/controller/streamboost/nostreamboost.lua
	$(CP) $(PKG_BUILD_DIR)/luasrc/controller/streamboost/sbmainstream.lua $(1)/usr/lib/lua/luci/controller/streamboost/streamboost.lua
endef


## don't use '$(INSTALL_DATA)' because that doesn't do recursive
define Package/$(PKG_NAME)-assets/install
	$(INSTALL_DIR) $(1)/www
	$(INSTALL_DIR) $(1)/www/luci-static
	$(INSTALL_DIR) $(1)/www/luci-static/resources
	$(INSTALL_DIR) $(1)/www/luci-static/resources/streamboost

	## Note, this is copying from the icons directory to the images directory
	$(INSTALL_DIR) $(1)/www/luci-static/resources/streamboost/images
	$(CP) -aR $(PKG_BUILD_DIR)/htdocs/luci-static/resources/streamboost/icons/* $(1)/www/luci-static/resources/streamboost/images

	$(INSTALL_DIR) $(1)/www/luci-static/resources/streamboost/trans
	$(CP) -aR $(PKG_BUILD_DIR)/htdocs/luci-static/resources/streamboost/trans/*.{js,json} $(1)/www/luci-static/resources/streamboost/trans
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
$(eval $(call BuildPackage,$(PKG_NAME)-mainstream))
$(eval $(call BuildPackage,$(PKG_NAME)-assets))
