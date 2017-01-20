include $(TOPDIR)/rules.mk

PKG_NAME:=flowmark
PKG_VERSION:=g1595d4a
PKG_RELEASE:=1
PKG_START_ORDER:=43

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  CATEGORY:=Bigfoot
  SUBMENU:=Flow Detect
  TITLE:=StreamBoost to Conntrack interface
  DEPENDS:=+libsbdaemon +hiredis +libevent2-core +libnetfilter-conntrack +libpthread
  MAINTAINER:=Nic Costa <ncosta@qca.qualcomm.com>
endef

define Package/$(PKG_NAME)/description
Flowmark receives a 5-tuple and a flow ID and marks the appropriate conntrack entry with the flow ID.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
endef

# TABS
define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_DIR) $(1)/etc/appflow
	$(INSTALL_DIR) $(1)/etc/appflow/streamboost.d
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/$(PKG_NAME)     $(1)/usr/sbin/
	$(INSTALL_BIN) ./files/$(PKG_NAME).init         $(1)/etc/appflow/streamboost.d/${PKG_START_ORDER}_$(PKG_NAME)
endef

# NO TABS, NO SPACES
$(eval $(call BuildPackage,$(PKG_NAME)))
