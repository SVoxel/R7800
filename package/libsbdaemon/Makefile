include $(TOPDIR)/rules.mk

PKG_NAME:=libsbdaemon
PKG_VERSION:=ge9e259d
PKG_RELEASE:=2

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/libsbdaemon/Default
  SECTION:=Bigfoot
  CATEGORY:=Bigfoot
  SUBMENU:=Utilities
  TITLE:=Daemonizing Library
  MAINTAINER:=Mo Chen <moc@qca.qualcomm.com>
endef

define Package/libsbdaemon
$(call Package/libsbdaemon/Default)
endef

define Build/Configure
endef

define Build/Compile
endef

define Package/libsbdaemon/install
	$(INSTALL_DIR) $(1)/usr/lib
	cp -arp ./files/* $(1)/usr/lib/
endef

$(eval $(call BuildPackage,libsbdaemon))
