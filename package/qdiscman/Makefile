include $(TOPDIR)/rules.mk

PKG_NAME:=qdiscman
PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=ssh://qca-git01.qualcomm.com:29418/streamboost/$(PKG_NAME).git
PKG_BRANCH:=banana
PKG_RELEASE:=1

include $(INCLUDE_DIR)/local-development.mk
ifeq ($(DUMP)$(PKG_VERSION),)
PKG_REV:=f802571
PKG_VERSION:=gf802571
endif
PKG_START_ORDER:=40

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_MIRROR:=none

PKG_BUILD_DEPENDS:=argp-standalone

include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

# use SPACES here instead of tabs
define Package/$(PKG_NAME)
  CATEGORY:=Bigfoot
  SUBMENU:=Flow Detect
  TITLE:=StreamBoost to Netlink interface
  DEPENDS:=+libsbdaemon +hiredis +libevent2-core +libnl-bf +libpthread +tc \
	+iptables-mod-conntrack-extra +iptables-mod-ipopt \
	+kmod-sched +kmod-sched-connmark +kmod-ipt-ipopt \
	+iptables-mod-mark2prio +guest_dhcp
  MAINTAINER:=Nic Costa <ncosta@qca.qualcomm.com>
  KCONFIG:= \
	CONFIG_BRIDGE_IGMP_SNOOPING=y \
	CONFIG_NETFILTER=y \
	CONFIG_NETFILTER_ADVANCED=y \
	CONFIG_NETFILTER_NETLINK=m \
	CONFIG_NETFILTER_XTABLES=m \
	CONFIG_NETFILTER_XT_CONNMARK=m \
	CONFIG_NETFILTER_XT_MARK=m \
	CONFIG_NETFILTER_XT_MATCH_COMMENT=m \
	CONFIG_NETFILTER_XT_MATCH_CONNMARK=m \
	CONFIG_NETFILTER_XT_MATCH_MARK=m \
	CONFIG_NETFILTER_XT_MATCH_MULTIPORT=m \
	CONFIG_NETFILTER_XT_TARGET_CLASSIFY=m \
	CONFIG_NETFILTER_XT_TARGET_CONNMARK=m \
	CONFIG_NETFILTER_XT_TARGET_MARK=m \
	CONFIG_NET_SCHED=m \
	CONFIG_NET_SCH_CODEL=m \
	CONFIG_NET_SCH_FQ_CODEL=m \
	CONFIG_NET_SCH_HFSC=m \
	CONFIG_NET_SCH_PRIO=m \
	CONFIG_NET_SCH_SFQ=m \
	CONFIG_NF_CONNTRACK=m \
	CONFIG_NF_CONNTRACK_CHAIN_EVENTS=y \
	CONFIG_NF_CONNTRACK_EVENTS=y \
	CONFIG_NF_CONNTRACK_FTP=m \
	CONFIG_NF_CONNTRACK_H323=m \
	CONFIG_NF_CONNTRACK_IRC=m \
	CONFIG_NF_CONNTRACK_MARK=y \
	CONFIG_NF_CONNTRACK_PPTP=m \
	CONFIG_NF_CONNTRACK_SIP=m \
	CONFIG_NF_CT_NETLINK=m \
	CONFIG_NF_CT_PROTO_GRE=m
endef

define Package/$(PKG_NAME)/description
Qdiscman receives flow policy information from the oversub pubsub channel and implements the policy into the appropriate network qdisc structures.
endef

# https://forum.openwrt.org/viewtopic.php?id=21953
TARGET_CFLAGS += -Wl,-rpath-link=$(STAGING_DIR)/usr/lib

# use TABs here instead of spaces
MAKE_FLAGS += \
	LDLIBS+="-largp" \
	CFLAGS="-I$(STAGING_DIR)/usr/include/libnl3 -I$(STAGING_DIR)/usr/include $(TARGET_CFLAGS)"

# TABS
define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_DIR) $(1)/etc/appflow
	$(INSTALL_DIR) $(1)/etc/appflow/streamboost.d
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/$(PKG_NAME)     $(1)/usr/sbin/
	$(INSTALL_BIN) ./files/$(PKG_NAME).init         $(1)/etc/appflow/streamboost.d/${PKG_START_ORDER}_$(PKG_NAME)
	$(INSTALL_BIN) ./files/classids.sh		$(1)/etc/appflow/
	$(INSTALL_BIN) ./files/qdiscman.sh		$(1)/etc/appflow/
	$(INSTALL_BIN) ./files/qdiscman-nss.sh		$(1)/etc/appflow/
endef

# NO TABS, NO SPACES
$(eval $(call BuildPackage,$(PKG_NAME)))
