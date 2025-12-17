#
# Copyright (C) 2006-2017 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=dropbear
PKG_VERSION:=2025.89
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_URL:= \
	http://matt.ucc.asn.au/dropbear/releases/ \
	https://dropbear.nl/mirror/releases/
PKG_MD5SUM:=2816ff711130f030daee12cbb10fd5ec

PKG_LICENSE:=MIT
PKG_LICENSE_FILES:=LICENSE libtomcrypt/LICENSE libtommath/LICENSE

PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk

define Package/dropbear/Default
  URL:=http://matt.ucc.asn.au/dropbear/
endef

define Package/dropbear
  $(call Package/dropbear/Default)
  SECTION:=net
  CATEGORY:=Base system
  DEPENDS:=+zlib
  TITLE:=Small SSH2 client/server
endef

define Package/dropbear/description
A small SSH2 server/client designed for small memory environments.
endef

define Package/dropbear/conffiles
/etc/dropbear/dropbear_ecdsa_host_key
/etc/dropbear/dropbear_ed25519_host_key
/etc/dropbear/dropbear_rsa_host_key
/etc/dropbear/ssh_host_ecdsa_key.pub
/etc/dropbear/ssh_host_ed25519_key.pub
/etc/dropbear/ssh_host_rsa_key.pub
endef

CONFIGURE_ARGS += --prefix=/usr

TARGET_CFLAGS := $(filter-out -O%,$(TARGET_CFLAGS)) -O3
TARGET_CFLAGS += -ffunction-sections -fdata-sections
TARGET_LDFLAGS += -Wl,--gc-sections

define Build/Configure
	$(Build/Configure/Default)
endef

define Build/Compile
	+$(MAKE) $(PKG_JOBS) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		PROGRAMS="dbclient dropbear dropbearconvert dropbearkey scp ssh" \
		MULTI=1
endef

define Package/dropbear/install
	$(INSTALL_DIR) $(1)/usr/{bin,sbin}
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/dropbearmulti $(1)/usr/bin/dropbearmulti
	$(LN) dropbearmulti $(1)/usr/bin/dbclient
	$(LN) dropbearmulti $(1)/usr/bin/dropbearconvert
	$(LN) dropbearmulti $(1)/usr/bin/dropbearkey
	$(LN) dropbearmulti $(1)/usr/bin/scp
	$(LN) dropbearmulti $(1)/usr/bin/ssh
	$(LN) ../bin/dropbearmulti $(1)/usr/sbin/dropbear
	$(INSTALL_DIR) $(1)/etc/{dropbear,init.d}
	$(INSTALL_BIN) ./files/dropbear.init $(1)/etc/init.d/dropbear
	$(CP) ./files/dropbear_ecdsa_host_key   $(1)/etc/dropbear
	$(CP) ./files/dropbear_ed25519_host_key $(1)/etc/dropbear
	$(CP) ./files/dropbear_rsa_host_key     $(1)/etc/dropbear
	chmod 0600 $(1)/etc/dropbear/dropbear_ecdsa_host_key
	chmod 0600 $(1)/etc/dropbear/dropbear_ed25519_host_key
	chmod 0600 $(1)/etc/dropbear/dropbear_rsa_host_key
	$(CP) ./files/ssh_host_ecdsa_key.pub    $(1)/etc/dropbear
	$(CP) ./files/ssh_host_ed25519_key.pub  $(1)/etc/dropbear
	$(CP) ./files/ssh_host_rsa_key.pub      $(1)/etc/dropbear
endef

$(eval $(call BuildPackage,dropbear))
