include $(TOPDIR)/rules.mk

PKG_NAME:=hiredis
PKG_VERSION:=bf161d996fcab9cf351b2fdb851a9db5f275103b
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/hiredis-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/hiredis
  SECTION:=libs
  CATEGORY:=Libraries
  SUBMENU:=database
  TITLE:=hiredis
  MAINTAINER:=Mo Chen <moc@qualcomm.com>
  URL:=https://github.com/redis/hiredis
endef

define Build/Configure
endef

define Build/Compile
endef

define Package/hiredis/install
	$(INSTALL_DIR) $(1)/usr/lib
	cp -arp ./files/* $(1)/usr/lib/
endef

$(eval $(call BuildPackage,hiredis))
