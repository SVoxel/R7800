#
# Copyright (C) 2019-2020 Voxel
#

include $(TOPDIR)/rules.mk

BASENAME:=dnscrypt-proxy
PKG_NAME:=$(BASENAME)-2
PKG_VERSION:=2.1.8
PKG_RELEASE:=1
PKG_SOURCE=$(BASENAME)-$(PKG_VERSION).tar.gz
PKG_MD5SUM:=676517ae6e86a9272ff7808fbc07b301
PKG_BUILD_DIR:=$(BUILD_DIR)/$(BASENAME)-$(PKG_VERSION)

INSTALL_UPX:=upx --ultra-brute -o

include $(INCLUDE_DIR)/package.mk

define Package/dnscrypt-proxy-2
  SECTION:=net
  CATEGORY:=Network
  TITLE:=Flexible DNS proxy with encrypted DNS protocols
  URL:=https://github.com/DNSCrypt/dnscrypt-proxy
  DEPENDS:=+libpthread
endef

define Package/dnscrypt-proxy-2/description
  A flexible DNS proxy, with support for modern encrypted DNS protocols
  such as DNSCrypt v2 and DNS-over-HTTPS.
endef

define Build/Compile
	( \
		cd $(PKG_BUILD_DIR)/$(BASENAME); \
		CC=$(TOOLCHAIN_DIR)/bin/arm-openwrt-linux-gcc \
		CCX=$(TOOLCHAIN_DIR)/bin/arm-openwrt-linux-g++ \
		GOOS=linux \
		GOARCH=arm \
		GOARM=7 \
		CGO_ENABLED=0 \
		GOPATH=$(PKG_BUILD_DIR)/$(BASENAME) \
		CGO_CFLAGS="$(TARGET_CFLAGS)" \
		CGO_CXXFLAGS="$(TARGET_CFLAGS)" \
		go build \
		-gcflags=-trimpath=$(PKG_BUILD_DIR)/$(BASENAME) \
		-asmflags=-trimpath=$(PKG_BUILD_DIR)/$(BASENAME) \
		-ldflags "-s -w"; \
	)
	chmod -R u+w $(PKG_BUILD_DIR)/$(BASENAME)
endef

define Package/dnscrypt-proxy-2/conffiles
/etc/dnscrypt-proxy-2.toml
endef

define Package/dnscrypt-proxy-2/install
	$(INSTALL_DIR)  $(1)/usr/sbin
	$(INSTALL_UPX)  $(1)/usr/sbin/dnscrypt-proxy-2 $(PKG_BUILD_DIR)/$(BASENAME)/dnscrypt-proxy
	$(INSTALL_DIR)  $(1)/etc/init.d
	$(INSTALL_BIN)  files/dnscrypt-proxy-2.init $(1)/etc/init.d/dnscrypt-proxy-2
	$(INSTALL_DATA) files/example-dnscrypt-proxy.toml $(1)/etc/dnscrypt-proxy-2.toml
endef

$(eval $(call BuildPackage,dnscrypt-proxy-2))
