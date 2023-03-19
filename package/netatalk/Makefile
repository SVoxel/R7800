#
# Copyright (C) 2009-2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=netatalk
PKG_VERSION:=3.1.14
PKG_SOURCE:=netatalk-$(PKG_VERSION).tar.bz2
PKG_SOURCE_URL:=@SF/netatalk/
PKG_MD5SUM:=96874d1505fae5b313846d9d8949452f

include $(INCLUDE_DIR)/package.mk

TARGET_CFLAGS += -ffunction-sections -fdata-sections
TARGET_LDFLAGS += -Wl,--gc-sections

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

EXTRA_CFLAGS  += -I$(STAGING_DIR)/usr/include
EXTRA_LDFLAGS += -ldb-4.7

CONFIGURE_ARGS += \
	--disable-afs \
	--enable-hfs \
	--disable-debugging \
	--disable-shell-check \
	--disable-timelord \
	--disable-a2boot \
	--disable-cups \
	--disable-tcp-wrappers \
	--with-cnid-cdb-backend \
	--with-cnid-default-backend=dbd \
	--with-bdb="$(STAGING_DIR)/usr" \
	--with-libgcrypt-dir="$(STAGING_DIR)/usr" \
	--libdir="$(STAGING_DIR)/usr" \
	--prefix="$(STAGING_DIR)/usr" \
	--libexecdir="$(STAGING_DIR)/usr/lib" \
	--with-ssl-dir="$(STAGING_DIR)/usr" \
	--with-uams-path="/usr/lib/uams" \
	--disable-admin-group \
	--disable-zeroconf \
	--disable-srvloc \


define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		DESTDIR="$(PKG_INSTALL_DIR)" \
		all install
endef

define Package/netatalk/install
	$(INSTALL_DIR) $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/usr/lib/netatalk/
	$(INSTALL_DIR) $(1)/etc/netatalk/
	$(RM) $(1)/etc/netatalk/afp_signature.conf
	$(LN) /tmp/netatalk/afp_signature.conf $(1)/etc/netatalk/afp_signature.conf
	$(LN) /tmp/netatalk/afp.conf $(1)/etc/netatalk/afp.conf
	$(RM) $(1)/etc/netatalk/afppasswd
	$(LN) /tmp/netatalk/afppasswd $(1)/etc/netatalk/afppasswd
	$(LN) /tmp/netatalk/afppasswd $(1)/etc/afppasswd
	$(INSTALL_DIR) $(1)/etc/init.d/
	$(INSTALL_DIR) $(1)/usr/config/netatalk
	$(CP) $(PKG_INSTALL_DIR)/usr/sbin/afpd $(1)/usr/sbin/
	$(CP) $(PKG_INSTALL_DIR)/usr/bin/afppasswd $(1)/usr/sbin/
	$(CP) $(PKG_INSTALL_DIR)/usr/bin/ad $(1)/usr/bin/
	$(CP) $(PKG_INSTALL_DIR)/usr/bin/afpldaptest $(1)/usr/bin/
	$(CP) $(PKG_INSTALL_DIR)/usr/bin/dbd $(1)/usr/bin/
	$(CP) $(PKG_INSTALL_DIR)/usr/bin/netatalk-config $(1)/usr/bin/
	$(CP) $(PKG_INSTALL_DIR)/usr/sbin/cnid_dbd $(1)/usr/sbin/
	$(CP) $(PKG_INSTALL_DIR)/usr/sbin/cnid_metad $(1)/usr/sbin/
	$(CP) $(PKG_INSTALL_DIR)/usr/sbin/netatalk $(1)/usr/sbin/
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/uams/*.so $(1)/usr/lib/netatalk/
	$(CP) $(PKG_BUILD_DIR)/libatalk/.libs/libatalk.so.18.0.0 $(1)/usr/lib/
	$(CP) $(PKG_BUILD_DIR)/libatalk/.libs/libatalk.so.18 $(1)/usr/lib/
	$(CP) $(PKG_BUILD_DIR)/libatalk/.libs/libatalk.so $(1)/usr/lib/
	$(INSTALL_DATA) ./files/afp.conf $(1)/usr/config/netatalk
	$(INSTALL_DATA) ./files/extmap.conf $(1)/etc/netatalk/
	$(INSTALL_BIN) ./files/afpd.init $(1)/etc/init.d/run_afpd
	$(INSTALL_BIN) ./files/detach_afp_shares $(1)/usr/sbin/detach_afp_shares
	$(INSTALL_BIN) ./files/check_time_machine $(1)/usr/sbin/check_time_machine
endef

$(eval $(call BuildPackage,netatalk))
