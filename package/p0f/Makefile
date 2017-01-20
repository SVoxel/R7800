include $(TOPDIR)/rules.mk

PKG_NAME:=p0f
PKG_MAJ:=3
PKG_MIN:=0
PKG_BLD:=6b
PKG_VERSION:=$(PKG_MAJ).$(PKG_MIN)$(PKG_BLD)
PKG_RELEASE:=1
PKG_START_ORDER:=52

PKG_SOURCE=$(PKG_NAME)-$(PKG_VERSION).tgz
PKG_SOURCE_URL:=http://lcamtuf.coredump.cx/$(PKG_NAME)$(PKG_MAJ)/releases
MD5SUM:=edbc4b135b2646db3227a441268fd2e2

PKG_BUILD_DEPENDS:=libpcap

PKG_UNPACK = tar xzf "$(DL_DIR)/$(PKG_SOURCE)" --strip-components 1 -C "$(PKG_BUILD_DIR)"

include $(INCLUDE_DIR)/package.mk

define Package/p0f
  SECTION:=net
  CATEGORY:=Network
  DEPENDS:=+libpcap +p0f-db
  MAINTAINER:=Ryan Nowakowski <rnowakow@qca.qualcomm.com>
  TITLE:=p0f: passive fingerprinter
  URL:=http://lcamtuf.coredump.cx/p0f.shtml
endef

define Package/p0f/description
	p0f is a tool that utilizes an array of sophisticated, purely
	passive traffic fingerprinting mechanisms to identify the
	players behind any incidental TCP/IP communications (often
	as little as a single normal SYN) without interfering in
	any way.
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) -f Makefile.owrt \
	  $(TARGET_CONFIGURE_OPTS) \
	  CFLAGS="$(TARGET_CFLAGS) -I$(STAGING_DIR)/usr/include" \
	  LDFLAGS="$(TARGET_LDFLAGS) -L$(STAGING_DIR)/usr/lib" \
	  all
	$(MAKE) -C $(PKG_BUILD_DIR) -f Makefile.owrt \
	  $(TARGET_CONFIGURE_OPTS) \
	  CFLAGS="$(TARGET_CFLAGS) -I$(STAGING_DIR)/usr/include" \
	  LDFLAGS="$(TARGET_LDFLAGS) -L$(STAGING_DIR)/usr/lib" \
	  tools
endef

define Package/p0f/install
	$(INSTALL_DIR) $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/etc/
	$(INSTALL_DIR) $(1)/etc/appflow/
	$(INSTALL_DIR) $(1)/etc/appflow/streamboost.d/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/p0f $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/tools/p0f-client $(1)/usr/bin/
	$(INSTALL_BIN) ./files/p0f.init $(1)/etc/appflow/streamboost.d/${PKG_START_ORDER}_${PKG_NAME}

endef

$(eval $(call BuildPackage,p0f))
