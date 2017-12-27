include $(TOPDIR)/rules.mk
## Utility defines...used in others
empty:=
space:= $(empty) $(empty)

PKG_TARGETS:=ipkg-prebuilt
# Build package version from git sha-1
PKG_NAME:=macoui-db
PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=ssh://qca-git01.qualcomm.com:29418/streamboost/macoui-db
PKG_BRANCH:=banana
PKG_REV:=4db3611

PKG_REV_DATE_RES=$(shell TMPDIR=`mktemp -d /tmp/$(PKG_NAME).XXXXX` && \
			cd $${TMPDIR} && \
			git init >/dev/null && \
			git fetch $(PKG_SOURCE_URL) $(PKG_BRANCH) >/dev/null 2>&1 && \
			git log --format="%ci" FETCH_HEAD -1 | \
				sed 's/-\|:\| //g' | cut -b -14 && \
			rm -rf $${TMPDIR})
PKG_REV_DATE:=20141230102119

PKG_VERSION:=20141230102119-g4db3611-generic
PKG_VERSION_SAVED:=$(PKG_VERSION)
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=$(PKG_BRANCH)
PKG_SOURCE_MIRROR:=none

include $(INCLUDE_DIR)/local-development.mk
#check if we used a localversion then let's update PKG_VERSION if we did
ifneq ($(PKG_VERSION),$(PKG_VERSION_SAVED))
PKG_VERSION:=20141230102119-g4db3611-generic
endif

include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

define Package/macoui-db
  CATEGORY:=Bigfoot
  SUBMENU:=Flow Detect
  TITLE:=macoui database
  MAINTAINER:=Mo Chen <moc@qca.qualcomm.com
  PKGARCH:=all
endef

define Package/macoui-db/description
Mac OUI mappings.
endef

define Package/macoui-db/install
	$(INSTALL_DIR) $(1)/etc/appflow
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/macoui.txt $(1)/etc/appflow
endef

$(eval $(call BuildPackage,macoui-db))
