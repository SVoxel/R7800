# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: Makefile,v 1.1.2.5.2.1 2008-05-13 06:54:33 echo.niu Exp $

include $(TOPDIR)/rules.mk

PKG_NAME:=samba
PKG_VERSION:=3.0.24
PKG_RELEASE:=1

PKG_GIT_TREEISH:=samba-v3.0.24-dni5

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PKG_INSTALL_DIR:=$(PKG_BUILD_DIR)/ipkg-install

include $(INCLUDE_DIR)/package.mk

define Package/samba
  SECTION:=net
  CATEGORY:=Network
  TITLE:=SAMBA
  URL:=http://www.samba.org
endef

define Build/Configure
endef

define Build/Prepare
	$(CP) $(GIT_HOME)/samba.git/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	rm -rf $(PKG_INSTALL_DIR)
	mkdir -p $(PKG_INSTALL_DIR)
	$(MAKE) -C $(PKG_BUILD_DIR)/source \
                $(TARGET_CONFIGURE_OPTS) \
                CROSS="$(TARGET_CROSS)" \
                CFLAGS="$(TARGET_CFLAGS) -I$(DEP_BUILD_DIR)/include -I$(STAGING_DIR)/usr/include -I$(STAGING_DIR)/include -O3 -fhonour-copts" \
                LDFLAGS="-L$(STAGING_DIR)/usr/lib -L$(STAGING_DIR)/lib" \
                CODEPAGEDIR="/usr/share/samba" \
                all
	(cd $(PKG_BUILD_DIR)/source/bin ; \
                mv -f make_smbcodepage.old make_smbcodepage ; touch make_smbcodepage ; \
                mv -f make_unicodemap.old make_unicodemap ; touch make_unicodemap ; \
        );
	mkdir -p $(PKG_INSTALL_DIR)/usr/share/samba
	mkdir -p $(PKG_INSTALL_DIR)/var/log
	$(MAKE) -C $(PKG_BUILD_DIR)/source \
                BASEDIR="$(PKG_INSTALL_DIR)/usr" \
                BINDIR="$(PKG_INSTALL_DIR)/usr/bin" \
                SBINDIR="$(PKG_INSTALL_DIR)/usr/bin" \
                LIBDIR="$(PKG_INSTALL_DIR)/usr/lib" \
                VARDIR="$(PKG_INSTALL_DIR)/var/log/samba" \
                MANDIR="$(PKG_INSTALL_DIR)/usr/share/man" \
                CONFIGDIR="$(PKG_INSTALL_DIR)/etc/samba" \
                PRIVATEDIR="$(PKG_INSTALL_DIR)/etc/samba" \
                SWATDIR="$(PKG_INSTALL_DIR)/usr/swat" \
                LOCKDIR="$(PKG_INSTALL_DIR)/var/run/samba" \
                SAMBABOOK="$(PKG_INSTALL_DIR)/usr/swat/using_samba" \
                CODEPAGEDIR="$(PKG_INSTALL_DIR)/usr/share/samba" \
                install
endef

define Package/samba/install
	install -d -m0755 $(1)/usr/sbin
	install -d -m0755 $(1)/usr/bin
	install -d -m0755 $(1)/usr/config
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/source/bin/smbpasswd $(1)/usr/bin/smbpasswd
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/source/bin/smbd $(1)/usr/sbin/smbd
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/source/bin/nmbd $(1)/usr/sbin/nmbd
	install -m0644 $(PKG_BUILD_DIR)/source/codepages/lowcase.dat $(1)/usr/config
	install -m0644 $(PKG_BUILD_DIR)/source/codepages/upcase.dat $(1)/usr/config
	install -m0644 $(PKG_BUILD_DIR)/source/codepages/valid.dat $(1)/usr/config
	install -d -m0755 $(1)/usr/share/samba
	install -d -m0755 $(1)/etc/modules.d
	install -m0755 ./files/50-samba-mod $(1)/etc/modules.d/
endef
$(eval $(call BuildPackage,samba))
