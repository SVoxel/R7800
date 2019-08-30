include $(TOPDIR)/rules.mk

PKG_NAME:=transmission-web-control
PKG_VERSION:=2019-04-16-a747a87b
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/ronggang/transmission-web-control
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=4382c76d
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.xz
PKG_MIRROR_MD5SUM:=94b25d53956492e3302c4a2b4ed49e31

PKG_LICENSE:=MIT
PKG_LICENSE_FILES:=LICENSE
PKG_MAINTAINER:=Richard Yu <yurichard3839@gmail.com>

include $(INCLUDE_DIR)/package.mk

define Package/transmission-web-control
  SECTION:=net
  CATEGORY:=Network
  SUBMENU:=BitTorrent
  DEPENDS:=@(PACKAGE_transmission-daemon-openssl||PACKAGE_transmission-daemon-mbedtls)
  CONFLICTS:=transmission-web
  TITLE:=Transmission Web Control
  URL:=https://github.com/ronggang/transmission-web-control
  PKGARCH:=all
endef

define Package/transmission-web-control/description
 Transmission Web Control is a custom Web UI for Transmission.
endef

define Build/Compile
endef

define Package/transmission-web-control/install
	$(INSTALL_DIR) $(1)/usr/share/transmission/web
	$(CP) $(PKG_BUILD_DIR)/src/* $(1)/usr/share/transmission/web
endef

$(eval $(call BuildPackage,transmission-web-control))
