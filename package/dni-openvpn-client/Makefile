#
# Copyright (C) 2017 DNI
#

include $(TOPDIR)/rules.mk

PKG_NAME:=dni-openvpn-client

PKG_VERSION:=1.0

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  TITLE:=OpenVPN Client for Netgear
  SECTION:=net
  CATEGORY:=Network
  SUBMENU:=VPN
  MENU:=1
  DEPENDS:=+openvpn-openssl
endef

define Build/Configure
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Package/$(PKG_NAME)/install
	$(CP) files/* $(1)
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/citylist $(1)/etc/openvpn/client
	$(INSTALL_DIR) $(1)/etc/rc.d/
	$(LN) ../init.d/openvpn_client $(1)/etc/rc.d/S60openvpn_client
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
