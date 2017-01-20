#
# Copyright (C) 2009-2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=netatalk
PKG_VERSION:=2.2.1
PKG_RELEASE:=3
PKG_SOURCE:=netatalk-$(PKG_VERSION).tar.bz2
PKG_SOURCE_URL:=@SF/netatalk/
#PKG_MD5SUM:=df4897c686a2792847a6739d6fa4cc67
include $(INCLUDE_DIR)/package.mk

define Package/netatalk
  SECTION:=net
  CATEGORY:=Network
  DEPENDS:=+libdb47 +libgcrypt +libopenssl +netatalk-utility
  TITLE:=netatalk
  URL:=http://netatalk.sourceforge.net
endef

define Package/netatalk/decription
  Netatalk is a freely-available Open Source AFP fileserver.
  It also provides a kernel level implementation of the AppleTalk
  Protocol Suite.
endef

EXTRA_CFLAGS += -I$(STAGING_DIR)/usr/include

CONFIGURE_ARGS += \
	--disable-afs \
	--enable-hfs \
	--disable-debugging \
	--disable-shell-check \
	--disable-timelord \
	--disable-a2boot \
	--disable-cups \
	--disable-tcp-wrappers \
	--with-cnid-default-backend=dbd \
	--with-bdb="$(STAGING_DIR)/usr" \
	--with-libgcrypt-dir="$(STAGING_DIR)/usr" \
	--libdir="$(STAGING_DIR)/usr" \
	--prefix="$(STAGING_DIR)/usr" \
	--libexecdir="$(STAGING_DIR)/usr/lib" \
	--with-ssl-dir="$(STAGING_DIR)/usr" \
	--with-uams-path="/usr/lib/uams" \
	--disable-admin-group \
	--disable-srvloc \

define Package/netatalk/install
	$(INSTALL_DIR) $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/usr/lib/uams/
	$(INSTALL_DIR) $(1)/etc/netatalk/
	rm -f $(1)/etc/netatalk/AppleVolumes.default
	ln -s /tmp/netatalk/AppleVolumes.default $(1)/etc/netatalk/AppleVolumes.default
	rm -f $(1)/etc/netatalk/afp_signature.conf
	ln -s /tmp/netatalk/afp_signature.conf $(1)/etc/netatalk/afp_signature.conf
	rm -f $(1)/etc/netatalk/afppasswd
	ln -s /tmp/netatalk/afppasswd $(1)/etc/netatalk/afppasswd
	$(INSTALL_DIR) $(1)/etc/init.d/
	$(INSTALL_DIR) $(1)/usr/config/netatalk
	$(CP) $(PKG_BUILD_DIR)/etc/afpd/afpd $(1)/usr/sbin/
	$(CP) $(PKG_BUILD_DIR)/bin/afppasswd/afppasswd $(1)/usr/sbin/
	$(CP) $(PKG_BUILD_DIR)/etc/cnid_dbd/cnid_dbd $(1)/usr/sbin/
	$(CP) $(PKG_BUILD_DIR)/etc/cnid_dbd/cnid_metad $(1)/usr/sbin/
	$(CP) $(PKG_BUILD_DIR)/etc/uams/.libs/*.so $(1)/usr/lib/uams/
	ln -s /usr/lib/uams/uams_dhx_passwd.so $(1)/usr/lib/uams/uams_dhx.so
	ln -s /usr/lib/uams/uams_dhx2_passwd.so $(1)/usr/lib/uams/uams_dhx2.so
	$(CP) ./files/AppleVolumes.default $(1)/usr/config/netatalk/
	$(CP) ./files/AppleVolumes.system $(1)/etc/netatalk/
	$(INSTALL_CONF) ./files/afpd.conf $(1)/etc/netatalk/
	$(INSTALL_BIN) ./files/afpd.init $(1)/etc/init.d/run_afpd
	install -m0755 ./files/detach_afp_shares $(1)/usr/sbin/detach_afp_shares
	install -m0755 ./files/check_time_machine $(1)/usr/sbin/check_time_machine
endef

$(eval $(call BuildPackage,netatalk))
