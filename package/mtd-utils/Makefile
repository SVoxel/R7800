#
# Copyright (C) 2009-2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=mtd-utils
PKG_REV:=aea36417067dade75192bafa03af70b6eb2677b1
PKG_VERSION:=1.5.2
PKG_RELEASE:=2
PKG_INSTALL:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.xz
PKG_SOURCE_URL:=git://git.infradead.org/mtd-utils.git
PKG_SOURCE_PROTO:=git
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_MIRROR_MD5SUM:=f69aca3fd198b419bdc7aa2e60d108b4

PKG_BUILD_DEPENDS:=util-linux

include $(INCLUDE_DIR)/package.mk

TARGET_CFLAGS := $(filter-out -O%,$(TARGET_CFLAGS)) -Os
TARGET_CFLAGS += -std=gnu17 -Wno-incompatible-pointer-types

FILES:= \
	doc_loadbios \
	docfdisk \
	flash_erase \
	flash_eraseall \
	flash_lock \
	flash_otp_dump \
	flash_otp_info \
	flash_unlock \
	flashcp \
	ftl_check \
	ftl_format \
	jffs2dump \
	mtd_debug \
	mtdinfo \
	nanddump \
	nandtest \
	nandwrite \
	nftldump \
	nftl_format \
	recv_image \
	rfddump \
	rfdformat \
	serve_image \
	sumtool \
	ubiattach \
	ubicrc32 \
	ubidetach \
	ubiformat \
	ubimkvol \
	ubinfo \
	ubinize \
	ubirename \
	ubirmvol \
	ubirsvol \
	ubiupdatevol

define PartGen
define Package/mtd-utils-$(subst _,-,$(firstword $(subst :, ,$(1))))
  TITLE:=$(firstword $(subst :, ,$(1))) package from mtd-utils
  URL:=http://www.linux-mtd.infradead.org/
  SECTION:=utils
  CATEGORY:=Utilities
  DEPENDS:=mtd-utils $(wordlist 2,$(words $(subst :, ,$(1))),$(subst :, ,$(1)))
endef
endef

define Package/mtd-utils
  TITLE:=Utilities for flash info/debug
  SECTION:=utils
  CATEGORY:=Utilities
  URL:=http://www.linux-mtd.infradead.org/
  MENU:=1
endef

define Package/mtd-utils/description
  Utilities for manipulating memory technology devices.
endef

define Package/mtd-utils/install
	true
endef

$(foreach file,$(FILES),$(eval $(call PartGen,$(file))))

MAKE_FLAGS += \
	DESTDIR="$(PKG_INSTALL_DIR)" \
	BUILDDIR="$(PKG_BUILD_DIR)" \
	LDLIBS+="$(LIBGCC_S)" \
	WITHOUT_XATTR=1 \
	WITHOUT_LZO=1

define PartInstall
define Package/mtd-utils-$(subst _,-,$(firstword $(subst :, ,$(1))))/install
	$(INSTALL_DIR) \
		$$(1)/usr/sbin

	$(INSTALL_BIN) \
		$(PKG_INSTALL_DIR)/usr/sbin/$(firstword $(subst :, ,$(1))) \
		$$(1)/usr/sbin/
endef
endef

$(foreach file,$(FILES),$(eval $(call PartInstall,$(file))))

$(eval $(call BuildPackage,mtd-utils))
$(foreach file,$(FILES),$(eval $(call BuildPackage,mtd-utils-$(subst _,-,$(firstword $(subst :, ,$(file)))))))
