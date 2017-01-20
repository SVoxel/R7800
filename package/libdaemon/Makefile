#
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=libdaemon
PKG_VERSION:=0.14
PKG_RELEASE:=2

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=http://0pointer.de/lennart/projects/libdaemon/
PKG_MD5SUM:=509dc27107c21bcd9fbf2f95f5669563

#PKG_FIXUP:=libtool
PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk

define Package/libdaemon
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=A lightweight C library that eases the writing of UNIX daemons
  URL:=http://0pointer.de/lennart/projects/libdaemon/
endef

define Package/libdaemon/description
	libdaemon is a lightweight C library that eases the writing of UNIX daemons.
	It consists of the following parts:
	- A wrapper around fork() which does the correct daemonization procedure of a process
	- A wrapper around syslog() for simpler and compatible log output to Syslog or STDERR
	- An API for writing PID files
	- An API for serializing UNIX signals into a pipe for usage with select() or poll()
	- An API for running subprocesses with STDOUT and STDERR redirected to syslog

	APIs like these are used in most daemon software available. It is not that
	simple to get it done right and code duplication is not a goal.
endef


define Build/Configure
	(cd $(PKG_BUILD_DIR); \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS) $(EXTRA_CFLAGS)" \
		CXXFLAGS="$(TARGET_CFLAGS) $(EXTRA_CFLAGS)" \
		CPPFLAGS="$(TARGET_CPPFLAGS) $(EXTRA_CPPFLAGS)" \
		LDFLAGS="$(TARGET_LDFLAGS)" \
		PKG_CONFIG_PATH="$(STAGING_DIR)/usr/lib/pkgconfig" \
		PKG_CONFIG_LIBDIR="$(STAGING_DIR)/usr/lib/pkgconfig" \
		./configure \
		ac_cv_func_setpgrp_void=no \
			ac_cv_func_setpgrp_void=yes \
			ac_cv_func_memcmp_working=yes \
			rb_cv_binary_elf=no \
			rb_cv_negative_time_t=no \
		--host=$(GNU_TARGET_NAME) \
		--build=$(GNU_HOST_NAME) \
		--prefix=$(PKG_INSTALL_DIR))
endef


#
#define Build/Configure
#	$(call Build/Configure/Default, \
#		--enable-shared \
#		--enable-static \
#		--disable-lynx \
#		--prefix=$(PKG_INSTALL_DIR))
#endef
#


define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		install
endef



TARGET_CFLAGS += $(FPIC)

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) $(PKG_INSTALL_DIR)/include/libdaemon $(1)/usr/include/
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/lib/libdaemon.a $(1)/usr/lib/
	$(CP) $(PKG_INSTALL_DIR)/lib/libdaemon.so* $(1)/usr/lib/
	$(INSTALL_DIR) $(1)/usr/lib/pkgconfig
	$(CP) $(PKG_INSTALL_DIR)/lib/pkgconfig/libdaemon.pc $(1)/usr/lib/pkgconfig/
endef

define Package/libdaemon/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/lib/libdaemon.so.* $(1)/usr/lib/
endef

$(eval $(call BuildPackage,libdaemon))
