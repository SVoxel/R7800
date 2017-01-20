include $(TOPDIR)/rules.mk

PKG_NAME=guest_dhcp
PKG_VERSION=1
PKG_RELEASE=1

include $(INCLUDE_DIR)/package.mk

define Package/guest_dhcp
  SECTION:=base
  CATEGORY:=Bigfoot
  SUBMENU:=Utilities
  DEPENDS:=+dnsmasq +iptables-mod-extra +iptables-mod-rawnat +iptables-mod-nat-extra
  TITLE:=Guest Network support for dnsmasq
  MAINTAINER:=Cristian Prundeanu <prundeanu@qca.qualcomm.com>
endef

define Build/Compile
endef

define Package/$(PKG_NAME)/install
	$(CP) -a ./files/* $(1)/
endef

$(eval $(call BuildPackage,guest_dhcp))
