# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: Makefile 7477 2007-06-03 11:40:06Z nbd $

include $(TOPDIR)/rules.mk

PKG_NAME:=wireless-tools
PKG_VERSION:=29
PKG_MINOR:=.pre21
PKG_RELEASE:=1

PKG_GIT_TREEISH:=27dee86bcab04be6c3d9ecf15cba3aaa07131bf6

PKG_BUILD_DIR:=$(BUILD_DIR)/wireless-tools
PLATFORM_PATCH_DIR:=./patches-$(BOARD)

include $(INCLUDE_DIR)/package.mk

define Package/wireless-tools
  SECTION:=net
  CATEGORY:=Base system
  TITLE:=Tools for manipulating Linux Wireless Extensions
  URL:=http://hplabs.hp.com/personal/Jean_Tourrilhes/Linux/Tools.html
endef

define Build/Prepare
	$(CP) $(GIT_HOME)/wireless-tools.git/* $(PKG_BUILD_DIR)/
endef

COMMON_CFLAGS="-DNETGEAR_ANYSSID"

define Build/Compile
	rm -rf $(PKG_INSTALL_DIR)
	mkdir -p $(PKG_INSTALL_DIR)
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS) $(COMMON_CFLAGS) -I." \
		libiw.so.$(PKG_VERSION) iwmulticall
	$(MAKE) -C $(PKG_BUILD_DIR) \
		PREFIX="$(PKG_INSTALL_DIR)" \
		INSTALL_DIR="$(PKG_INSTALL_DIR)/usr/sbin" \
		INSTALL_LIB="$(PKG_INSTALL_DIR)/usr/lib" \
		install-iwmulticall
endef

define Build/InstallDev
	mkdir -p $(STAGING_DIR)/usr/include
	$(CP) $(PKG_BUILD_DIR)/{iwlib,wireless}.h $(STAGING_DIR)/usr/include/
	mkdir -p $(STAGING_DIR)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/libiw.so* $(STAGING_DIR)/usr/lib/
	ln -sf libiw.so.$(PKG_VERSION) $(STAGING_DIR)/usr/lib/libiw.so
endef
	
define Build/UninstallDev
	rm -f	$(STAGING_DIR)/usr/include/{iwlib,wireless}.h \
		$(STAGING_DIR)/usr/lib/libiw.so*
endef

define Package/wireless-tools/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(CP) $(PKG_INSTALL_DIR)/usr/sbin/iw{config,getid,list,priv,spy} $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,wireless-tools))
