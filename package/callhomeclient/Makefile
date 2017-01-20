include $(TOPDIR)/rules.mk

# Build package version from git sha-1
PKG_NAME:=callhomeclient
PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=ssh://qca-git01.qualcomm.com:29418/streamboost/callhomeclient
PKG_BRANCH:=banana
PKG_RELEASE:=2

include $(INCLUDE_DIR)/local-development.mk
ifeq ($(DUMP)$(PKG_VERSION),)
PKG_REV:=28ceed2
PKG_VERSION:=g28ceed2
endif
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_MIRROR:=none

include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

define Package/callhomeclient
  CATEGORY:=Bigfoot
  SUBMENU:=Flow Detect
  TITLE:=Client for the callhome server
  DEPENDS:=+curl +openssl-util +customer-keys
  MAINTAINER:=Ryan Nowakowski <rnowakow@qca.qualcomm.com>
endef

define Package/callhomeclient/description
Client for the callhome server
endef

define Build/Configure
endef

define Build/Compile
endef

define Package/callhomeclient/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/etc/appflow/streamboost.d/

	$(INSTALL_BIN) $(PKG_BUILD_DIR)/authcurl $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/send_event $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/upload_events $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/send_bwestdata $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/upload_bwestdata $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/upload_stats $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/query_upload_host_status $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/anonymize.awk $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/getsalt $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/hashmac $(1)/usr/bin/
endef

$(eval $(call BuildPackage,callhomeclient))
