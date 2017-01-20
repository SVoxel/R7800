include $(TOPDIR)/rules.mk

PKG_TARGETS:=ipkg-prebuilt
PKG_NAME:=customer-keys
PKG_RELEASE:=1
PKG_VERSION:=1-netgear-r7800

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  CATEGORY:=Bigfoot
  SUBMENU:=Utilities
  TITLE:=Callhome Client Keys
  MAINTAINER:=Earl Manning <earl.manning@qca.qualcomm.com>
endef

define Package/$(PKG_NAME)/description
SSL keys and certificates for a specific customer
endef

define Build/Compile
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_DIR) $(1)/etc/ssl
	$(INSTALL_DIR) $(1)/etc/ssl/certs
	$(INSTALL_DATA) ./files/client_cert.pem	$(1)/etc/ssl/certs
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_DIR) $(1)/etc/ssl

	$(INSTALL_DIR) $(1)/etc/ssl/certs
	$(INSTALL_DATA) ./files/CA.cert.pem	$(1)/etc/ssl/certs
	$(INSTALL_DATA) ./files/client_cert.pem	$(1)/etc/ssl/certs

	$(INSTALL_DIR) $(1)/etc/ssl/private
	$(INSTALL_DATA) ./files/client_key.pem	$(1)/etc/ssl/private
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
