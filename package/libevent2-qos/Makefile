#
# Copyright (C) 2011-2015 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

BASE_NAME:=libevent2
PKG_NAME:=$(BASE_NAME)-qos
PKG_VERSION:=2.0.22
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/libevent-$(PKG_VERSION)-stable
PKG_SOURCE:=libevent-$(PKG_VERSION)-stable.tar.gz
PKG_SOURCE_URL:=@SF/levent
PKG_MD5SUM:=c4c56f986aa985677ca1db89630a2e11
PKG_SOURCE_SUBDIR:=$(BASE_NAME)-$(PKG_VERSION)
PKG_MAINTAINER:=Jo-Philipp Wich <jow@openwrt.org>
PKG_LICENSE:=BSD-3-Clause

PKG_FIXUP:=autoreconf
PKG_INSTALL:=1
PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk

define Package/libevent2-qos/Default
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=Event notification
  URL:=http://www.monkey.org/~provos/libevent/
endef

define Package/libevent2-qos/Default/description
	The libevent API provides a mechanism to execute a callback function
	when a specific event occurs on a file descriptor or after a timeout
	has been reached. Furthermore, libevent also support callbacks due
	to signals or regular timeouts.

	libevent is meant to replace the event loop found in event driven
	network servers. An application just needs to call event_dispatch()
	and then add or remove events dynamically without having to change
	the event loop.
endef

define Package/libevent2-qos
  $(call Package/libevent2-qos/Default)
  TITLE+= library (version 2.0)
endef

define Package/libevent2-qos/description
	$(call Package/libevent2-qos/Default/description)

	This package contains the libevent shared library historically
	containing both the core & extra libraries.
endef

define Package/libevent2-core-qos
  $(call Package/libevent2-qos/Default)
  TITLE+= core library (version 2.0)
endef

define Package/libevent2-core-qos/description
	$(call Package/libevent2-qos/Default/description)

	This package contains the libevent core shared library for the event,
	buffer & utility functions.
endef

define Package/libevent2-extra-qos
  $(call Package/libevent2-qos/Default)
  TITLE+= extra library (version 2.0)
endef

define Package/libevent2-extra-qos/description
	$(call Package/libevent2-qos/Default/description)

	This package contains the libevent extra shared library for specific
	protocols including HTTP, DNS & RPC.
endef

define Package/libevent2-openssl-qos
  $(call Package/libevent2-qos/Default)
  TITLE+= OpenSSL library (version 2.0)
  DEPENDS+=+libopenssl
endef

define Package/libevent2-openssl-qos/description
	$(call Package/libevent2-qos/Default/description)

	This package contains the libevent OpenSSL shared library for encrypted
	bufferevents.
endef

define Package/libevent2-pthreads-qos
  $(call Package/libevent2-qos/Default)
  TITLE+= Pthreads library (version 2.0)
  DEPENDS+=+libpthread
endef

define Package/libevent2-pthreads-qos/description
	$(call Package/libevent2-qos/Default/description)

	This package contains the libevent Pthreads shared library for
	threading & locking.
endef

TARGET_CFLAGS += $(FPIC)

CONFIGURE_ARGS += \
	--enable-shared \
	--enable-static \
	--disable-debug-mode

MAKE_FLAGS += \
	CFLAGS="$(TARGET_CFLAGS)"

define Package/libevent2-qos/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libevent-2.0.so.* $(1)/usr/lib/
endef

define Package/libevent2-core-qos/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libevent_core-2.0.so.* $(1)/usr/lib/
endef

define Package/libevent2-extra-qos/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libevent_extra-2.0.so.* $(1)/usr/lib/
endef

define Package/libevent2-openssl-qos/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libevent_openssl-2.0.so.* $(1)/usr/lib/
endef

define Package/libevent2-pthreads-qos/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libevent_pthreads-2.0.so.* $(1)/usr/lib/
endef

$(eval $(call BuildPackage,libevent2-qos))
$(eval $(call BuildPackage,libevent2-core-qos))
$(eval $(call BuildPackage,libevent2-extra-qos))
$(eval $(call BuildPackage,libevent2-openssl-qos))
$(eval $(call BuildPackage,libevent2-pthreads-qos))
