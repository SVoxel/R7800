# $Id$

include $(TOPDIR)/rules.mk

PKG_NAME:=forked-daapd
PKG_VERSION:=23.0-dni1
PKG_RELEASE:=20140216

#
# Specify git commit hash in this variable if you want to test a commit which
# is not pointed by any tag. Otherwise, specify tag in PKG_VERSION and leave
# blank to PKG_SOURCE_TREEISH.
#
PKG_SOURCE_TREEISH:=

PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=$(GIT_HOME)/$(PKG_NAME).git
PKG_SOURCE_VERSION:=$(if $(PKG_SOURCE_TREEISH),$(PKG_SOURCE_TREEISH),$(PKG_VERSION))
PKG_SOURCE:=$(PKG_NAME)-$(PKG_SOURCE_VERSION).tar.bz2

PKG_FIXUP:=libtool
PKG_INSTALL:=1
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/forked-daapd
SECTION:=sound
CATEGORY:=Sound
TITLE:=Improved iTunes (DAAP) server. Support for Apple Remote and AirPlay.
MAINTAINER:=Espen Jürgensen <espenjurgensen+openwrt@gmail.com>
URL:=https://github.com/jasonmc/forked-daapd
DEPENDS:=+libgpg-error +libgcrypt +libgdbm +zlib +libexpat +libunistring \
	+libevent2 +libdaemon +libantlr3c +confuse +glib2 +alsa-lib +libffmpeg-full \
	+mxml +libavl +avahi-daemon +libavahi-client +sqlite3-cli +libplist
endef

define Package/forked-daapd/conffiles
/etc/forked-daapd.conf
endef

# Fix for libevent
TARGET_CPPFLAGS += -I$(STAGING_DIR)/usr/include/libevent2
TARGET_CPPFLAGS += -DDNI_SUPPORT_LIBEVENT2
TARGET_LDFLAGS += -L$(STAGING_DIR)/usr/lib/libevent2

TARGET_CFLAGS += $(FPIC)
TARGET_LDFLAGS += -Wl,-rpath-link,$(STAGING_DIR)/usr/lib

# Modified from "DownloadMethod/git" in include/download.mk
define DownloadMethod/git
	$(call wrap_mirror, \
		if [ x$(GIT_HOME) == x ]; then \
			echo "ERROR: GIT_HOME not defined!!"; \
			exit 1; \
		fi; \
		if [ ! -d $(PKG_SOURCE_URL) ]; then \
			echo "ERROR: Directory $(PKG_SOURCE_URL) does not exist!!"; \
			exit 1; \
		fi; \
		( \
			cd $(PKG_SOURCE_URL); \
			git-cat-file -e $(PKG_SOURCE_VERSION) && \
			git-archive --format=tar --prefix=$(PKG_SOURCE_SUBDIR)/ \
				$(PKG_SOURCE_VERSION) | bzip2 -c > $(DL_DIR)/$(FILE); \
		) \
	)
endef

define Build/Configure
	$(call Build/Configure/Default, \
		--enable-shared \
		--enable-static \
		--enable-itunes)
endef

define Package/forked-daapd/install
	$(INSTALL_DIR) $(1)/etc
	ln -sf /tmp/etc/forked-daapd.conf $(1)/etc/forked-daapd.conf
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/lib/forked-daapd
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/forked-daapd/* $(1)//usr/lib/forked-daapd/
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) ./files/itunes_allow_control $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/sbin/forked-daapd $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/forked-daapd.init $(1)/etc/init.d/forked-daapd
endef

$(eval $(call BuildPackage,forked-daapd))
