#
# Copyright (C) 2007-2008 Delta Networks Inc.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=aws-iot
PKG_VERSION:=1.0
PKG_RELEASE:=1
PKG_BUILD_DEPENDS:=datalib

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

PKG_GIT_TREEISH:=e2346eec2dc4876ef298d8c819cbb7141ce53d50

include $(INCLUDE_DIR)/package.mk

define Package/aws-iot
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=The AWS IoT to Publish Topic
endef

define Build/Prepare
	$(CP) $(GIT_HOME)/aws-iot.git/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
       $(CP) ./files/* $(PKG_BUILD_DIR)/files
endef

define Build/Configure
endef

define Package/aws-iot/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_DIR) $(1)/etc/rc.d
	$(INSTALL_DIR) $(1)/etc/router_analytics
	$(INSTALL_DIR) $(1)/etc/router_analytics/beta
	$(INSTALL_DIR) $(1)/etc/router_analytics/debug
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/files/certs/certificate.pem.crt  $(1)/etc/router_analytics/
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/files/certs/rootCA.crt $(1)/etc/router_analytics
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/files/certs/beta/certificate.pem.crt  $(1)/etc/router_analytics/beta/
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/files/certs/beta/rootCA.crt $(1)/etc/router_analytics/beta/
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/files/certs/beta/rootCA.crt $(1)/etc/router_analytics/beta/
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/files/certs/debug/rootCA.crt $(1)/etc/router_analytics/debug/
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/files/certs/debug/certificate.pem.crt  $(1)/etc/router_analytics/debug
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/aws-iot.init	$(1)/etc/init.d/aws
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/aws-iot			$(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/icqm			$(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ra_check		$(1)/usr/sbin
	$(LN) ../init.d/aws $(1)/etc/rc.d/S99aws
endef

$(eval $(call BuildPackage,aws-iot))
