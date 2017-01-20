# 
# Copyright (C) 2008 OpenWrt.org
#
#echo "#define NETGEAR_GUEST_ACCESS" >> config.h \

include $(TOPDIR)/rules.mk

PKG_NAME:=proftpd
PKG_VERSION:=1.3.5V
PKG_RELEASE:=1

PKG_GIT_TREEISH:=proftpd-1.3.5V

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/proftpd
  SECTION:=net
  CATEGORY:=Network
  TITLE:=FTP Daemon
  URL:=http://www.proftpd.org/
endef

define Build/Prepare
	$(CP) $(GIT_HOME)/proftpd.git/* $(PKG_BUILD_DIR)/
endef

define Build/Configure
	(cd $(PKG_BUILD_DIR) ; \
		echo "ac_cv_func_setgrent_void=yes" > $(GNU_TARGET_NAME)-linux.cache ; \
		echo "ac_cv_func_setpgrp_void=no" >> $(GNU_TARGET_NAME)-linux.cache ; \
		CFLAGS="$(TARGET_CFLAGS) -I$(DEP_BUILD_DIR)/include -I$(STAGING_DIR)/usr/include -I$(STAGING_DIR)/include -I./include -I$(STAGING_DIR)/usr/lib/libiconv/include -I$(STAGING_DIR)/usr/lib/libintl/include"; \
		./configure \
			--with-includes=$(STAGING_DIR)/include:$(STAGING_DIR)/usr/include:$(STAGING_DIR)/usr/lib/libintl/include:$(STAGING_DIR)/usr/lib/libiconv/include \
			--with-libraries=$(STAGING_DIR)/lib:$(STAGING_DIR)/usr/lib:$(STAGING_DIR)/usr/lib/libintl/lib:$(STAGING_DIR)/usr/lib/libiconv/lib \
			--host=$(GNU_TARGET_NAME) \
			--cache-file=$(GNU_TARGET_NAME)-linux.cache \
			--sysconfdir=/etc \
			--localstatedir=/var/run \
			--enable-nls \
			--enable-shadow \
			--disable-cap \
			--disable-auth-pam \
			--disable-curses \
			--disable-ncurses ;\
		echo "#undef HAVE___PROGNAME" >> config.h ; \
		echo "#undef HAVE_UTMPX_H" >> config.h ; \
		echo "#undef HAVE_STROPTS_H" >> config.h ; \
		echo "#undef HAVE_SYS_ACL_H" >> config.h ; \
		echo "#undef HAVE_EXECINFO_H" >> config.h ; \
		echo "#undef HAVE_FUTIMES" >> config.h ; \
		echo "#define HAVE_NET_IF_H 1" >> config.h ; \
		echo "#define LAN_IFNAME \"br0\"" >> config.h ; \
		echo "#define WAN_IFNAME \"brwan\"" >> config.h ; \
		echo "#define PPPOE_IFNAME \"ppp0\"" >> config.h ; \
		echo "#define USB_NET_ACCESS_LOG" >> config.h ; \
		echo "#define NETGEAR_GUEST_ACCESS" >> config.h ; \
	);
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		CROSS="$(TARGET_CROSS)" \
		CFLAGS="$(TARGET_CFLAGS) -I$(DEP_BUILD_DIR)/include -I$(STAGING_DIR)/usr/include -I$(STAGING_DIR)/include -I./include -I$(STAGING_DIR)/usr/lib/libiconv/include -I$(STAGING_DIR)/usr/lib/libintl/include" \
                LDFLAGS="-L$(STAGING_DIR)/usr/lib -L$(STAGING_DIR)/lib -L./lib -L$(STAGING_DIR)/usr/lib/libintl/lib -L$(STAGING_DIR)/usr/lib/libiconv/lib -lintl -liconv" \
		all
endef

define Package/proftpd/install
	install -d -m0755 $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/proftpd $(1)/usr/sbin/
	install -m0755 $(PKG_BUILD_DIR)/ftptop $(1)/usr/sbin/
	install -d -m0755 $(1)/sbin
	install -m0755 ./files/cmdftp $(1)/sbin/
	ln -sf /usr/sbin/ftptop $(1)/usr/sbin/ftpload
endef

$(eval $(call BuildPackage,proftpd))
