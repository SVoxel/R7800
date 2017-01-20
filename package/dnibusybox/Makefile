# 
# Copyright (C) 2006-2009 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=dnibusybox
PKG_VERSION:=1.4.2
PKG_RELEASE:=2

PKG_GIT_TREEISH:=busybox-v1.4.2-dni63
BUSYBOX_HOME=$(GIT_HOME)/busybox.git

include $(INCLUDE_DIR)/package.mk

ifeq ($(DUMP),)
  STAMP_CONFIGURED:=$(strip $(STAMP_CONFIGURED))_$(shell grep '^CONFIG_BUSYBOX_' $(TOPDIR)/.config | md5s)
endif

init-y :=
init-$(CONFIG_DNIBUSYBOX_CONFIG_HTTPD) += httpd
init-$(CONFIG_DNIBUSYBOX_CONFIG_CROND) += cron
init-$(CONFIG_DNIBUSYBOX_CONFIG_TELNETD) += telnet

define Package/dnibusybox
  SECTION:=base
  CATEGORY:=Base system
  DEPENDS:=@TARGET_ipq806x
  TITLE:=Core utilities for embedded Linux
  URL:=http://busybox.net/
  MENU:=1
endef

define Package/dnibusybox/description
 The Swiss Army Knife of embedded Linux.
 It slices, it dices, it makes Julian Fries.
endef

define Build/Prepare
	$(CP) $(GIT_HOME)/busybox.git/* $(PKG_BUILD_DIR)/
endef

define Package/dnibusybox/config
	menu "Configuration"
		depends on PACKAGE_dnibusybox
		source "$(SOURCE)/target-config.in"
		source "$(SOURCE)/config/Config.in"
	endmenu
endef

define Build/Configure
	rm -f $(PKG_BUILD_DIR)/.configured*
	grep 'CONFIG_DNIBUSYBOX_' $(TOPDIR)/.config | sed -e "s,\\(# \)\\?CONFIG_DNIBUSYBOX_\\(.*\\),\\1\\2,g" > $(PKG_BUILD_DIR)/.config
	yes '' | $(MAKE) -C $(PKG_BUILD_DIR) \
		CC="$(TARGET_CC)" \
		CROSS_COMPILE="$(TARGET_CROSS)" \
		ARCH="$(ARCH)" \
		oldconfig
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		CC="$(TARGET_CC)" \
		CROSS_COMPILE="$(TARGET_CROSS)" \
		EXTRA_CFLAGS="$(TARGET_CFLAGS)" \
		ARCH="$(ARCH)" \
		IPKG_ARCH="$(ARCH)" \
		all
endef

define Package/dnibusybox/install
	find $(PKG_BUILD_DIR) -lname "*busybox" -exec rm \{\} \;
	$(MAKE) -C $(PKG_BUILD_DIR) \
		CC="$(TARGET_CC)" \
		CROSS_COMPILE="$(TARGET_CROSS)" \
		EXTRA_CFLAGS="$(TARGET_CFLAGS)" \
		ARCH="$(ARCH)" \
		IPKG_ARCH="$(ARCH)" \
		CONFIG_PREFIX="$(1)" \
		install
	$(INSTALL_DIR) $(1)/etc/init.d
	for tmp in $(init-y); do \
		$(INSTALL_BIN) $(PKG_BUILD_DIR)/dni-examples/$$$$tmp $(1)/etc/init.d/$$$$tmp; \
	done

	$(INSTALL_DIR) $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/dni-examples/dsyslog $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/dni-examples/dsyslog $(1)/etc/init.d/syslogd
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/dni-examples/cmd_cron $(1)/usr/sbin/
	$(INSTALL_BIN) ./files/cron $(1)/etc/init.d/
	$(INSTALL_DIR) $(1)/etc/bandcheck
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/dni-examples/band-check $(1)/etc/bandcheck/band-check
	-rm -rf $(1)/lib64
endef

$(eval $(call BuildPackage,dnibusybox))
