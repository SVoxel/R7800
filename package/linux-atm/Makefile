#
# Copyright (C) 2006-2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=linux-atm
PKG_VERSION:=2.5.2
PKG_RELEASE:=4

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=@SF/$(PKG_NAME)
PKG_MD5SUM:=d49499368c3cf15f73a05d9bce8824a8

PKG_INSTALL:=1
PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk

define Package/linux-atm
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=Linux ATM library
  URL:=http://linux-atm.sourceforge.net/
endef

define Package/linux-atm/description
  This package contains a library for accessing the Linux ATM subsystem.
endef

define Package/atm-tools
  SECTION:=net
  CATEGORY:=Network
  DEPENDS:=linux-atm
  TITLE:=Linux ATM tools
  URL:=http://linux-atm.sourceforge.net/
endef

define Package/atm-tools/description
  This package contains the Linux ATM tools.
endef

define Package/atm-diagnostics
  SECTION:=net
  CATEGORY:=Network
  DEPENDS:=linux-atm
  TITLE:=Linux ATM Diagnostics
  URL:=http://linux-atm.sourceforge.net/
endef

define Package/atm-diagnostics/description
  This package contains the Linux ATM diagnostics.
endef

define Package/br2684ctl
  SECTION:=net
  CATEGORY:=Network
  DEPENDS:=linux-atm
  TITLE:=ATM Ethernet bridging configuration utility
  URL:=http://linux-atm.sourceforge.net/
endef

define Package/br2684ctl/description
  Support for AAL5 encapsulation (RFC-1483/RFC-2684) over ATM.
endef

define Build/Configure
	$(call Build/Configure/Default)
	# prevent autoheader invocation
	touch $(PKG_BUILD_DIR)/stamp-h.in
endef

TARGET_CFLAGS += -std=gnu17 -I$(LINUX_DIR)/include -I$(LINUX_DIR)/arch/$(LINUX_KARCH)/include
unexport PREFIX

define Build/Compile
	# src/qgen is built with HOSTCC, which does not really like our LDFLAGS
	+$(MAKE) $(PKG_JOBS) -C $(PKG_BUILD_DIR)/src/qgen \
		LDFLAGS="" \
		all
	+$(MAKE) $(PKG_JOBS) -C $(PKG_BUILD_DIR) OBJCOPY=$(TARGET_CROSS)objcopy all
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr
	$(CP) \
		$(PKG_INSTALL_DIR)/usr/include \
		$(PKG_INSTALL_DIR)/usr/lib \
		$(1)/usr/
endef

define Package/linux-atm/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libatm.so* $(1)/usr/lib/
endef

define Package/atm-tools/install
	$(INSTALL_DIR) $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/arpd/atmarp{,d} $(1)/usr/sbin/

#
#The following is disabled by default but still useful for some debugging
#
#	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/sbin/atmaddr $(1)/usr/sbin/
#	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/sbin/atmloop $(1)/usr/sbin/
#	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/sbin/atmtcp     $(1)/usr/sbin/
#	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/sbin/esi $(1)/usr/sbin/
#	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/aread $(1)/usr/bin/
#	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/awrite $(1)/usr/bin/
#	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/atmdiag $(1)/usr/bin/
#	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/atmdump $(1)/usr/bin/
#
# The following is really for some specific application, and obsolete for most of it
#
#	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/sbin/atmsigd $(1)/usr/sbin/
#	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/sbin/bus $(1)/usr/sbin/
#	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/sbin/ilmid $(1)/usr/sbin/
#	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/sbin/ilmidiag $(1)/usr/sbin/
#	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/sbin/lecs $(1)/usr/sbin/
#	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/sbin/les $(1)/usr/sbin/
#	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/sbin/mpcd $(1)/usr/sbin/
#	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/sbin/zeppelin $(1)/usr/sbin/
#	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/atmswitch $(1)/usr/bin/
#	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/saaldump $(1)/usr/bin/
#	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/sonetdiag $(1)/usr/bin/
#	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/svc_recv $(1)/usr/bin/
#	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/svc_send $(1)/usr/bin/
#	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/ttcp_atm $(1)/usr/bin/
endef

define Package/atm-diagnostics/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/aread $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/awrite $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/atmdiag $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/atmdump $(1)/usr/bin/
endef

define Package/br2684ctl/install
	$(INSTALL_DIR) $(1)/etc/init.d/
	$(INSTALL_BIN) ./files/br2684ctl $(1)/etc/init.d/
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/sbin/br2684ctl $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,linux-atm))
$(eval $(call BuildPackage,atm-tools))
$(eval $(call BuildPackage,atm-diagnostics))
$(eval $(call BuildPackage,br2684ctl))
