include $(TOPDIR)/rules.mk

PKG_NAME:=dumpevents

PKG_VERSION:=0.0.1
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

# use SPACES here instead of tabs
define Package/dumpevents
  CATEGORY:=Bigfoot
  SUBMENU:=Flow Detect
  TITLE:=dumpevents
  DEPENDS:=+redis-cli @(BUSYBOX_CONFIG_AWK||DNIBUSYBOX_CONFIG_AWK)
  MAINTAINER:=Mo Chen <moc@qca.qualcomm.com>
endef

define Package/dumpevents/description
A utility to dump events from the event log.
endef

define Build/Prepare
	$(CP) ./files/* $(PKG_BUILD_DIR)/
endef

define Build/Configure

endef

define Build/Compile
	$(AM_TOOL_PATHS) $(MAKE) -C $(PKG_BUILD_DIR)
endef

define Package/dumpevents/install
	$(INSTALL_DIR) $(1)/usr
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/dump_events_json $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/dump_flow_events_json $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/dump_events_by_time_json $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/dump_nodes_json $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/filter_events.lua $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/filter_events_by_time.lua $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/nodes_json.lua $(1)/usr/bin/
endef

# NO TABS, NO SPACES
$(eval $(call BuildPackage,dumpevents))
