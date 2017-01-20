include $(TOPDIR)/rules.mk
## Utility defines...used in others
empty:=
space:= $(empty) $(empty)

PKG_TARGETS:=ipkg-prebuilt
# Build package version from git sha-1
PKG_NAME:=p0f-db
PKG_START_ORDER:=13
PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=ssh://qca-git01.qualcomm.com:29418/streamboost/p0f-db
PKG_BRANCH:=master # This is master on purpose

PKG_REV_DATE_RES=$(shell TMPDIR=`mktemp -d /tmp/$(PKG_NAME).XXXXX` && \
			cd $${TMPDIR} && \
			git init >/dev/null && \
			git fetch $(PKG_SOURCE_URL) $(PKG_BRANCH) >/dev/null 2>&1 && \
			git log --format="%ci" FETCH_HEAD -1 | \
				sed 's/-\|:\| //g' | cut -b -14 && \
			rm -rf $${TMPDIR})
PKG_REV_DATE:=20141230102121

PKG_VERSION:=20141230102121-g3251481-generic
PKG_VERSION_SAVED:=$(PKG_VERSION)
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=$(PKG_BRANCH)
PKG_SOURCE_MIRROR:=none

include $(INCLUDE_DIR)/local-development.mk
#check if we used a localversion then let's update PKG_VERSION if we did
ifneq ($(PKG_VERSION),$(PKG_VERSION_SAVED))
PKG_VERSION:=20141230102121-g3251481-generic
endif

include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

define Package/p0f-db
  CATEGORY:=Bigfoot
  SUBMENU:=Flow Detect
  TITLE:=p0f fingerprint database.
  DEPENDS:=
  MAINTAINER:=Mo Chen <moc@qca.qualcomm.com>
  PKGARCH:=all
endef

define Package/p0f-db/description
Database of fingerprints for p0f.
endef

define Build/Configure
endef

define Build/Compile
	python $(PKG_BUILD_DIR)/generate_redis.py $(PKG_BUILD_DIR)/priorities > $(PKG_BUILD_DIR)/priorities.redis
endef

define Package/p0f-db/install
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_DIR) $(1)/etc/appflow
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/p0f.fp $(1)/etc/appflow/
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/devices-1.2 $(1)/etc/appflow/devices
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/priorities.redis $(1)/etc/appflow/
	$(INSTALL_DIR) $(1)/etc/appflow/streamboost.d
	$(INSTALL_BIN) ./files/prio-reader.init $(1)/etc/appflow/streamboost.d/${PKG_START_ORDER}_prio-reader
endef

$(eval $(call BuildPackage,p0f-db))
