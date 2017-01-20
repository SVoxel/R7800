# 
# Copyright (C) 2007 OpenWrt.org
#

include $(TOPDIR)/rules.mk

PKG_NAME:=ppp-nas
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

PKG_BUILD_DEPENDS:=ipp


include $(INCLUDE_DIR)/package.mk

define Package/ppp-nas
	SECTION:=utils
	CATEGORY:=Network
	DEPENDS:=@TARGET_ipq806x
	TITLE:=PPP tools for controlling Multi-PPPoE
endef

define Package/ppp-nas/config
	menu "Configuration"
		depends on PACKAGE_ppp-nas
		source "$(SOURCE)/Config.in"
	endmenu
endef

define Build/Prepare
	$(CP) $(GIT_HOME)/ppp-nas.git/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		CROSS="$(TARGET_CROSS)" \
		LDFLAGS="-L$(STAGING_DIR)/usr/lib -L$(STAGING_DIR)/lib -lnetlink" \
		ARCH="$(ARCH)"
endef

define Package/ppp-nas/install
	install -d -m0755 $(1)/usr/sbin
	install -d -m0755 $(1)/sbin
	install -m0755 $(PKG_BUILD_DIR)/ppp-nas $(1)/usr/sbin/
	install -m0755 ./files/check_route $(1)/sbin
endef

$(eval $(call BuildPackage,ppp-nas))
