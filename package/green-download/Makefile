# 
#  Copyright (C) 2007 Delta Networks Inc.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=greendownload
PKG_RELEASE:=1
PKG_VERSION:=V1.0.0

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)


include $(INCLUDE_DIR)/package.mk

define Package/greendownload
	SECTION:=utils
	CATEGORY:=Network
	DEPENDS:= amule +wget +transmission
	TITLE:=Green Download core manager
endef

define Package/greendownload/config
        menu "Configurations"
                depends on PACKAGE_greendownload
                source "$(SOURCE)/Config.in"
        endmenu
endef


define Build/Prepare
	$(CP) $(GIT_HOME)/greendownload.git/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
endef

define Package/greendownload/install
	install -d -m0755 $(1)/usr/sbin
	install -m0755 $(PKG_BUILD_DIR)/dlclient $(1)/usr/sbin/
	install -m0755 $(PKG_BUILD_DIR)/remote_fsize $(1)/usr/sbin/
	install -m0755 $(PKG_BUILD_DIR)/greendownload $(1)/usr/sbin/
	install -m0755 $(PKG_BUILD_DIR)/green_download.sh $(1)/usr/sbin/
	install -m0755 $(PKG_BUILD_DIR)/green_download_upgrade.sh $(1)/usr/sbin/
	install -m0755 $(PKG_BUILD_DIR)/emule_firewall $(1)/usr/sbin/
	install -d -m0755 $(1)/etc/init.d
#	install -m0755 $(PKG_BUILD_DIR)/green_download_init $(1)/etc/init.d/
	install -m0755 $(PKG_BUILD_DIR)/glboot $(1)/etc/init.d/
	
endef

$(eval $(call BuildPackage,greendownload))
