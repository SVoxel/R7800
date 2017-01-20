include $(TOPDIR)/rules.mk

PKG_NAME:=policy-reader
PKG_START_ORDER:=10

PKG_VERSION:=0.0.1
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

# use SPACES here instead of tabs
define Package/policy-reader
  CATEGORY:=Bigfoot
  SUBMENU:=Flow Detect
  TITLE:=Policy-reader
  DEPENDS:=+redis-cli
  MAINTAINER:=Mo Chen <moc@qca.qualcomm.com>
endef

define Package/policy-reader/description
Policy-reader reads policy.redis into redis.
endef

define Build/Configure
endef

define Build/Compile
endef

define Package/policy-reader/install
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_DIR) $(1)/etc/appflow
	$(INSTALL_DIR) $(1)/etc/appflow/streamboost.d
	$(INSTALL_BIN) ./files/policy-reader.init $(1)/etc/appflow/streamboost.d/${PKG_START_ORDER}_policy-reader
endef

# NO TABS, NO SPACES
$(eval $(call BuildPackage,policy-reader))
