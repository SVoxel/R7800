include $(TOPDIR)/rules.mk

PKG_TARGETS:=ipkg-prebuilt
PKG_NAME:=leash
PKG_VERSION:=1
PKG_RELEASE:=1
PKG_START_ORDER:=35

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  CATEGORY:=Bigfoot
  SUBMENU:=Utilities
  TITLE:=Filter to read node data from DHCP, ARP table and WiFi table
  MAINTAINER:=Cristian Prundeanu <c.p@qca.qualcomm.com>
endef

define Package/$(PKG_NAME)/description
 This package contains an a daemon that will watch the DHCP leases file, along
 with the ARP table and the WiFi client list.
 When it sees a change, it will filter and compound the data into a single file,
 to prepare for inclusion into Redis.
endef

define Build/Configure
endef

define Build/Compile
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) ./files/dhcp-lease-filter $(1)/usr/sbin/

	$(INSTALL_DIR) $(1)/etc/appflow/streamboost.d/
	$(INSTALL_BIN) ./files/leash.init $(1)/etc/appflow/streamboost.d/${PKG_START_ORDER}_${PKG_NAME}
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
