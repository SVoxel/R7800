# 
# Copyright (C) 2014 Delta Networks Inc.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=zz-dni-streamboost
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/zz-dni-streamboost
  CATEGORY:=Bigfoot
  TITLE:=override QCA streamboost configure files instead of DNI's configure files
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./files/* $(PKG_BUILD_DIR)/
#	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
endef

define Package/zz-dni-streamboost/install
	install -d -m0755 $(1)/etc/init.d/
	install -d -m0755 $(1)/etc/appflow/streamboost.d/
	install -d -m0755 $(1)/usr/sbin/
	install -d -m0755 $(1)/usr/bin/
	install -d -m0755 $(1)/www/
	install -d -m0755 $(1)/www/js
	install -m0755 $(PKG_BUILD_DIR)/sched_updates $(1)/etc/init.d/
	install -m0755 $(PKG_BUILD_DIR)/streamboost $(1)/etc/init.d/
	install -m0755 $(PKG_BUILD_DIR)/streamboost.sys.conf $(1)/etc/appflow/
	install -m0755 $(PKG_BUILD_DIR)/rc.appflow $(1)/etc/appflow/
	install -m0755 $(PKG_BUILD_DIR)/upload_stats $(1)/usr/bin/
	install -m0755 $(PKG_BUILD_DIR)/database_versions $(1)/etc/appflow
	install -m0755 $(PKG_BUILD_DIR)/redis.conf $(1)/etc/appflow
	install -m0755 $(PKG_BUILD_DIR)/sb_update_database $(1)/usr/bin/
	install -m0755 $(PKG_BUILD_DIR)/12_settings-loader $(1)/etc/appflow/streamboost.d/
	install -m0755 $(PKG_BUILD_DIR)/35_leash $(1)/etc/appflow/streamboost.d/
	install -m0755 $(PKG_BUILD_DIR)/40_qdiscman $(1)/etc/appflow/streamboost.d/
	install -m0755 $(PKG_BUILD_DIR)/60_ozker $(1)/etc/appflow/streamboost.d/
	install -m0755 $(PKG_BUILD_DIR)/sb_set_priority $(1)/usr/sbin/
	install -m0755 $(PKG_BUILD_DIR)/poll_set_priority $(1)/usr/sbin/
	install -m0755 $(PKG_BUILD_DIR)/soap_flowman_nodes $(1)/usr/sbin/
	install -m0755 $(PKG_BUILD_DIR)/ping_arp $(1)/usr/sbin/
	install -m0644 $(PKG_BUILD_DIR)/flows_EN-US.json $(1)/www/
	install -m0644 $(PKG_BUILD_DIR)/debug.html $(1)/www/
	install -m0644 $(PKG_BUILD_DIR)/jquery-1.7.2.min.js $(1)/www/js/

endef

$(eval $(call BuildPackage,zz-dni-streamboost))
