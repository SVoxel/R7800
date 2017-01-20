include $(TOPDIR)/rules.mk

PKG_NAME:=settings-loader
PKG_START_ORDER:=12

PKG_VERSION:=0.0.1
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

# use SPACES here instead of tabs
define Package/settings-loader
  CATEGORY:=Bigfoot
  SUBMENU:=Utilities
  TITLE:=Settings Loader
  DEPENDS:=+redis-cli
  MAINTAINER:=Mo Chen <moc@qca.qualcomm.com>
endef

define Package/settings-loader/description
Settings loader (wait for it) loads settings into Redis.
endef

define Build/Configure
endef

define Build/Compile
endef

define Package/settings-loader/install
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_DIR) $(1)/etc/appflow
	$(INSTALL_DIR) $(1)/etc/appflow/streamboost.d
	$(INSTALL_BIN) ./files/settings-loader.init $(1)/etc/appflow/streamboost.d/${PKG_START_ORDER}_settings-loader
endef

# NO TABS, NO SPACES
$(eval $(call BuildPackage,settings-loader))
