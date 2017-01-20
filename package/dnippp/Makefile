#
# Copyright (C) 2006-2009 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=dnippp
PKG_VERSION:=2.4.3
PKG_RELEASE:=8

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
#PKG_SOURCE_URL:=ftp://ftp.samba.org/pub/ppp/
#PKG_MD5SUM:=848f6c3cafeb6074ffeb293c3af79b7c

PKG_BUILD_DEPENDS:=libpcap

PKG_GIT_TREEISH=ppp-v2.4.3-dni29

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/kernel.mk

define Package/dnippp/Default
  SECTION:=net
  CATEGORY:=Network
  URL:=http://ppp.samba.org/
endef

define Package/dnippp
$(call Package/dnippp/Default)
  DEPENDS:=@TARGET_ipq806x +kmod-ppp
  TITLE:=PPP daemon
  MENU:=1
endef

define Package/dnippp/description
 This package contains the PPP (Point-to-Point Protocol) daemon.
endef

define Package/dnippp/config
	menu "Configuration"
		depends on PACKAGE_dnippp
		source "$(SOURCE)/Config.in"
	endmenu
endef

define Package/dnippp-mod-pppoa
$(call Package/dnippp/Default)
  DEPENDS:=dnippp +linux-atm +kmod-pppoa
  TITLE:=PPPoA plugin
endef

define Package/dnippp-mod-pppoa/description
 This package contains a PPPoA (PPP over ATM) plugin for ppp.
endef

define Package/dnippp-mod-pppoe
$(call Package/dnippp/Default)
  DEPENDS:=dnippp +kmod-pppoe
  TITLE:=PPPoE plugin
endef

define Package/dnippp-mod-pppoe/description
 This package contains a PPPoE (PPP over Ethernet) plugin for ppp.
endef

define Package/dnippp-mod-pptp
$(call Package/dnippp/Default)
  DEPENDS:=dnippp +kmod-pptp
  TITLE:=PPTP plugin
endef

define Package/dnippp-mod-pptp/description
 This package contains a PPTP (PPP over GRE/IPv4) plugin for ppp.
endef

define Package/dnippp-mod-l2tp
  $(call Package/dnippp/Default)
  DEPENDS:=dnippp +kmod-l2tp
  TITLE:=L2TP plugin
endef

define Package/dnippp-mod-l2tp/description
 This package contains a L2TP (PPP over L2TP) plugin for ppp.
endef

define Package/dnippp-mod-radius
$(call Package/dnippp/Default)
  DEPENDS:=dnippp
  TITLE:=RADIUS plugin
endef

define Package/dnippp-mod-radius/description
 This package contains a RADIUS (Remote Authentication Dial-In User 
 Service) plugin for ppp.
endef

define Package/dnichat
$(call Package/dnippp/Default)
  DEPENDS:=dnippp
  TITLE:=Establish conversation with a modem
endef

define Package/dnichat/description
 This package contains an utility to establish conversation with other 
 PPP servers (via a modem).
endef

define Package/dnipppdump
$(call Package/dnippp/Default)
  DEPENDS:=dnippp
  TITLE:=Read PPP record file
endef

define Package/dnipppdump/description
 This package contains an utility to read PPP record file.
endef

define Package/dnipppstats
$(call Package/dnippp/Default)
  DEPENDS:=dnippp
  TITLE:=Report PPP statistics
endef

define Package/dnipppstats/description
 This package contains an utility to report PPP statistics.
endef

define Build/Prepare
	$(CP) $(GIT_HOME)/ppp.git/* $(PKG_BUILD_DIR)/
endef

define Build/Configure
$(call Build/Configure/Default,, \
	UNAME_S="Linux" \
	UNAME_R="$(LINUX_VERSION)" \
	UNAME_M="$(ARCH)" \
)
endef

define Build/Compile
	rm -rf $(PKG_INSTALL_DIR)
	mkdir -p $(PKG_INSTALL_DIR)/usr
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		COPTS="$(TARGET_CFLAGS)" \
		PRECOMPILED_FILTER=1 \
		HAVE_INET6=1 \
		STAGING_DIR="$(STAGING_DIR)" \
		DESTDIR="$(PKG_INSTALL_DIR)/usr" \
		all install
endef

define Package/dnippp/install
	$(INSTALL_DIR) $(1)/usr/lib/pppd/$(PKG_VERSION)
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/sbin/pppd $(1)/usr/sbin/
ifdef CONFIG_DNI_3G_DONGLE_SUPPORT
	$(INSTALL_BIN) ./files/do-3g-reconnect.sh $(1)/usr/sbin/
endif
	$(INSTALL_DIR) $(1)/lib/network
	$(INSTALL_BIN) ./files/ppp.sh $(1)/lib/network/
	$(INSTALL_DIR) $(1)/etc/modules.d
	$(INSTALL_BIN) ./files/etc/modules.d/60-pptp-mod $(1)/etc/modules.d/
	$(INSTALL_BIN) ./files/etc/modules.d/60-pppoe-mod $(1)/etc/modules.d/
	$(INSTALL_BIN) ./files/etc/modules.d/60-l2tp-mod $(1)/etc/modules.d/
ifdef CONFIG_DNI_3G_DONGLE_SUPPORT
	$(INSTALL_BIN) ./files/etc/modules.d/60-3g-mod $(1)/etc/modules.d/
endif
	ln -sf /tmp/ppp $(1)/etc/ppp
endef

define Package/dnippp-mod-pppoa/install
	$(INSTALL_DIR) $(1)/usr/lib/pppd/$(PKG_VERSION)
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/lib/pppd/$(PKG_VERSION)/pppoatm.so \
		$(1)/usr/lib/pppd/$(PKG_VERSION)/
	$(INSTALL_DIR) $(1)/lib/network
	$(INSTALL_BIN) ./files/pppoa.sh $(1)/lib/network/
endef

define Package/dnippp-mod-pppoe/install
	$(INSTALL_DIR) $(1)/usr/lib/pppd/$(PKG_VERSION)
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/lib/pppd/$(PKG_VERSION)/rp-pppoe.so \
		$(1)/usr/lib/pppd/$(PKG_VERSION)/
	install -d -m0777 $(1)/usr/share/ppp
	install -m0777 $(PKG_BUILD_DIR)/pppd/ppp-session.script $(1)/usr/share/ppp/
endef

define Package/dnippp-mod-pptp/install
	$(INSTALL_DIR) $(1)/usr/lib/pppd/$(PKG_VERSION)
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/lib/pppd/$(PKG_VERSION)/dni-pptp.so \
		$(1)/usr/lib/pppd/$(PKG_VERSION)/
endef

define Package/dnippp-mod-l2tp/install
	$(INSTALL_DIR) $(1)/etc/modules.d
	$(INSTALL_BIN) ./files/etc/modules.d/60-l2tp-mod $(1)/etc/modules.d/
	$(INSTALL_DIR) $(1)/usr/lib/pppd/$(PKG_VERSION)
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/lib/pppd/$(PKG_VERSION)/dni-l2tp.so \
		$(1)/usr/lib/pppd/$(PKG_VERSION)/
endef

define Package/dnippp-mod-radius/install
	$(INSTALL_DIR) $(1)/usr/lib/pppd/$(PKG_VERSION)
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/lib/pppd/$(PKG_VERSION)/radius.so \
		$(1)/usr/lib/pppd/$(PKG_VERSION)/
	$(INSTALL_DIR) $(1)/etc/ppp
	$(INSTALL_DATA) ./files/etc/ppp/radius.conf $(1)/etc/ppp/
	$(INSTALL_DIR) $(1)/etc/ppp/radius
	$(INSTALL_DATA) ./files/etc/ppp/radius/dictionary* \
		$(1)/etc/ppp/radius/
	$(INSTALL_CONF) ./files/etc/ppp/radius/servers \
		$(1)/etc/ppp/radius/
endef

define Package/dnichat/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/sbin/chat $(1)/usr/sbin/
endef

define Package/dnipppdump/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/sbin/pppdump $(1)/usr/sbin/
endef

define Package/dnipppstats/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/sbin/pppstats $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,dnippp))
$(eval $(call BuildPackage,dnippp-mod-pppoa))
$(eval $(call BuildPackage,dnippp-mod-pppoe))
$(eval $(call BuildPackage,dnippp-mod-pptp))
$(eval $(call BuildPackage,dnippp-mod-l2tp))
$(eval $(call BuildPackage,dnippp-mod-radius))
$(eval $(call BuildPackage,dnichat))
$(eval $(call BuildPackage,dnipppdump))
$(eval $(call BuildPackage,dnipppstats))
