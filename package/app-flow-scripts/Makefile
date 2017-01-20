include $(TOPDIR)/rules.mk

# Build package version from git sha-1
PKG_NAME:=app-flow-scripts
PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=ssh://qca-git01.qualcomm.com:29418/streamboost/app-flow-scripts
PKG_BRANCH:=banana
PKG_RELEASE:=2

include $(INCLUDE_DIR)/local-development.mk
ifeq ($(DUMP)$(PKG_VERSION),)
PKG_REV:=f69f16b
PKG_VERSION:=gf69f16b
endif

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_MIRROR:=none

include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

# use SPACES here instead of tabs
define Package/app-flow-scripts/Default
  CATEGORY:=Bigfoot
  SUBMENU:=Flow Detect
  TITLE:=Bigfoot Networks App Detect Scripts
  DEPENDS:=+drflocs +p0f +ubus +libubox
  MAINTAINER:=Cristian Prundeanu <prundeanu@qca.qualcomm.com>
endef

define Package/app-flow-scripts
  $(call Package/app-flow-scripts/Default)
  VARIANT:=load
endef

define Package/app-flow-scripts-noload
  $(call Package/app-flow-scripts/Default)
  VARIANT:=noload
endef

define Package/app-flow-scripts/Default/description
Scripts to help start all the app flow stuff
endef

define Package/app-flow-scripts/Description
  $(call Package/app-flow-scripts/Default/description)
endef

define Package/app-flow-scripts-noload/Description
  $(call Package/app-flow-scripts/Default/description)
endef

define Build/Configure
endef

define Build/Compile
endef

define Package/app-flow-scripts/Default/install
	$(INSTALL_DIR) $(1)/etc/appflow
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_DIR) $(1)/etc/hotplug.d/iface
	$(INSTALL_DIR) $(1)/etc/hotplug.d/firewall
	$(INSTALL_BIN) ./files/rc.appflow $(1)/etc/appflow/
	$(INSTALL_BIN) ./files/streamboost $(1)/etc/init.d/
	$(INSTALL_BIN) ./files/uci2streamboost $(1)/etc/appflow
	$(INSTALL_BIN) ./files/streamboost.hotplug.iface $(1)/etc/hotplug.d/iface/40-streamboost
	$(INSTALL_BIN) ./files/streamboost.hotplug.firewall $(1)/etc/hotplug.d/firewall/20-streamboost
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) ./files/streamboost_status_monit $(1)/usr/sbin/streamboost_status_monit
	$(INSTALL_DATA) ./files/streamboost.opkg.conf $(1)/etc/appflow/
	$(INSTALL_DATA) ./files/notice.txt $(1)/etc/appflow/
	$(LN) /etc/init.d/streamboost $(1)/usr/sbin/streamboost
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_BIN) ./files/appflow $(1)/etc/config/
endef

define Package/app-flow-scripts/install
	$(call Package/app-flow-scripts/Default/install, $(1))
endef

define Package/app-flow-scripts-noload/install
	$(call Package/app-flow-scripts/Default/install, $(1))

	# make sure the checkbox is disabled in the GUI
	$(SED) 's/option .enable_streamboost. 1/option '"'"'enable_streamboost'"'"' 0/g' -i $(1)/etc/config/appflow
endef

define Package/$(PKG_NAME)/postinst
#!/bin/sh
echo "Enabling rc.d symlink for streamboost"
mkdir -p $${IPKG_INSTROOT}/etc/rc.d
$${IPKG_INSTROOT}/etc/init.d/streamboost enable
exit 0
endef

define Package/$(PKG_NAME)/prerm
#!/bin/sh
echo "Removing rc.d symlink for streamboost"
$${IPKG_INSTROOT}/etc/init.d/streamboost disable
exit 0
endef

define Package/$(PKG_NAME)-noload/postinst
#!/bin/sh
echo "Enabling rc.d symlink for streamboost"
mkdir -p $${IPKG_INSTROOT}/etc/rc.d
$${IPKG_INSTROOT}/etc/init.d/streamboost enable
exit 0
endef

define Package/$(PKG_NAME)-noload/prerm
#!/bin/sh
echo "Removing rc.d symlink for streamboost"
$${IPKG_INSTROOT}/etc/init.d/streamboost disable
exit 0
endef

$(eval $(call BuildPackage,app-flow-scripts))
$(eval $(call BuildPackage,app-flow-scripts-noload))
