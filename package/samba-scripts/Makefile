# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: Makefile,v 1.1.2.5.2.1 2008-05-13 06:54:33 echo.niu Exp $

include $(TOPDIR)/rules.mk

PKG_NAME:=samba-scripts
PKG_VERSION:=1.0.0
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)
PKG_INSTALL_DIR:=$(PKG_BUILD_DIR)/ipkg-install

include $(INCLUDE_DIR)/package.mk

define Package/samba-scripts
  SECTION:=net
  CATEGORY:=Network
  TITLE:=SAMBA Scripts
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)
endef

define Package/samba-scripts/install
	install -d -m0755 $(1)/etc/init.d
	install -m0755 ./files/samba.init $(1)/etc/init.d/samba
	rm -f $(1)/etc/samba
	ln -s /tmp/samba $(1)/etc/samba
	rm -f  $(1)/usr/private
	install -d -m0755 $(1)/etc
	install -d -m0755 $(1)/etc/config
	install -d -m0755 $(1)/etc/config/samba
	install -d -m0755 $(1)/usr
	ln -s /tmp/private $(1)/etc/config/samba/private
	install -d -m0755 $(1)/usr/config
	install -d -m0755 $(1)/usr/lib
	install -d -m0755 $(1)/usr/sbin
	install -d -m0755 $(1)/tmp/samba
	rm -f $(1)/usr/lib/smb.conf
	ln -s /tmp/samba/smb.conf $(1)/etc/config/samba/smb.conf
	install -m0644 ./files/smb.conf $(1)/etc/config/samba
	install -m0644 ./files/passwd $(1)/usr/config
	install -m0644 ./files/group $(1)/usr/config
	install -m0755 ./files/update_user_v_02 $(1)/usr/sbin/update_user
	install -m0755 ./files/detectSATA $(1)/usr/sbin/detectSATA
	install -m0755 $(PKG_BUILD_DIR)/update_smb $(1)/usr/sbin/update_smb
	-rm -f $(1)/etc/gshadow
	ln -s /tmp/config/gshadow $(1)/etc/gshadow
	-rm -f $(1)/home/
	ln -sf /tmp $(1)/home
	-rm -f $(1)/etc/passwd
	ln -s /tmp/config/passwd $(1)/etc/passwd
	-rm -f $(1)/etc/passwd+
	ln -s /tmp/config/passwd+ $(1)/etc/passwd+
	-rm -f  $(1)/etc/group
	ln -s /tmp/config/group $(1)/etc/group
	-rm -f $(1)/etc/shadow
	ln -s /tmp/config/shadow $(1)/etc/shadow
endef
$(eval $(call BuildPackage,samba-scripts))
