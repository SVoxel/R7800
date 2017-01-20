include $(TOPDIR)/rules.mk

PKG_NAME:=netatalk-utility
PKG_VERSION:=1.0.0
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)
PKG_INSTALL_DIR:=$(PKG_BUILD_DIR)/ipkg-install

include $(INCLUDE_DIR)/package.mk

define Package/netatalk-utility
  SECTION:=net
  CATEGORY:=Network
  TITLE:=Netatalk Utility
endef

define Build/Prepare
	$(CP) ./src/* $(PKG_BUILD_DIR)
endef

define Package/netatalk-utility/install
	install -d -m0755 $(1)/usr
	install -d -m0755 $(1)/usr/sbin
	install -m0755 $(PKG_BUILD_DIR)/update_afp $(1)/usr/sbin/update_afp
	install -m0755 $(PKG_BUILD_DIR)/wol $(1)/usr/sbin/wol
	install -m0755 $(PKG_BUILD_DIR)/send_wol $(1)/usr/sbin/send_wol
endef
$(eval $(call BuildPackage,netatalk-utility))
