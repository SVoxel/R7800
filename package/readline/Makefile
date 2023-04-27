#
# Copyright (C) 2006-2014 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=readline
PKG_VERSION:=8.2
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=@GNU/readline
PKG_MD5SUM:=4aa1b31be779e6b84f9a96cb66bc50f6

PKG_LICENSE:=GPL-3.0
PKG_LICENSE_FILES:=COPYING

PKG_BUILD_PARALLEL:=1
PKG_FIXUP:=autoreconf
PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk

define Package/libreadline
  SECTION:=libs
  CATEGORY:=Libraries
  DEPENDS:=+libncurses
  TITLE:=Command lines edition library
  URL:=http://cnswww.cns.cwru.edu/php/chet/readline/rltop.html
endef

define Package/libreadline/description
	The Readline library provides a set of functions for use by applications 
	that allow users to edit command lines as they are typed in. Both Emacs 
	and vi editing modes are available. The Readline library includes 
	additional functions to maintain a list of previously-entered command 
	lines, to recall and perhaps reedit those lines, and perform csh-like 
	history expansion on previous commands.
endef

define Package/libhistory
  SECTION:=libs
  CATEGORY:=Libraries
  DEPENDS:=+libncurses
  TITLE:=Command lines edition library (history facilities)
  URL:=http://cnswww.cns.cwru.edu/php/chet/readline/rltop.html
endef

define Package/libhistory/description
	The Readline history facilities are also placed into a separate library,
	the History library, as part of the build process.  The History library
	may be used without Readline in applications which desire its
	capabilities.
endef

# prevent "autoreconf" from removing "aclocal.m4"
PKG_REMOVE_FILES:=

CONFIGURE_ARGS += --with-curses --disable-install-examples

CONFIGURE_VARS += \
	bash_cv_wcwidth_broken=no \
	bash_cv_func_sigsetjmp=yes \

TARGET_CPPFLAGS:=-I. -I.. $(TARGET_CPPFLAGS)

TARGET_CFLAGS += $(FPIC)
TARGET_CFLAGS := $(filter-out -O%,$(TARGET_CFLAGS)) -Os

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) $(PKG_INSTALL_DIR)/usr/include/readline $(1)/usr/include/
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/lib{history,readline}.{a,so*} $(1)/usr/lib/
endef

define Package/libreadline/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libreadline.so.* $(1)/usr/lib/
endef

define Package/libhistory/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libhistory.so.* $(1)/usr/lib/
endef

$(eval $(call BuildPackage,libreadline))
$(eval $(call BuildPackage,libhistory))
