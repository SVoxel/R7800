#
# Based on include/package-ipkg.mk
#
# Copyright (c) 2013 The Linux Foundation. All rights reserved.
# Copyright (C) 2006,2007 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

ifeq ($(DUMP),)
  define BuildTarget/ipkg-prebuilt
    ifeq ($(PKG_FORCE_PREBUILT)$(wildcard $(PREBUILT_DIR)/$(1)_$(VERSION)_$(PKGARCH).ipk),)
      $(BuildTarget/ipkg)
    else
      PKG_$(1):=$(1)_$(VERSION)_$(PKGARCH).ipk
      PRE_$(1):=$(PREBUILT_DIR)/$$(PKG_$(1))
      IPKG_$(1):=$(PACKAGE_DIR)/$$(PKG_$(1))
      INFO_$(1):=$(IPKG_STATE_DIR)/info/$(1).list

      Build/InstallDev:=

      ifeq ($(if $(VARIANT),$(BUILD_VARIANT)),$(VARIANT))
        ifdef Package/$(1)/install
          ifneq ($(CONFIG_PACKAGE_$(1))$(SDK)$(DEVELOPER),)
            compile: $$(IPKG_$(1))

            ifeq ($(CONFIG_PACKAGE_$(1)),y)
              install: $$(INFO_$(1))
            endif
          else
            compile: $(1)-disabled
            $(1)-disabled:
		@echo "WARNING: skipping $(1) -- package not selected"
          endif
        endif
      endif

      $$(IPKG_$(1)):
	$(CP) $$(PRE_$(1)) $$@

      $$(INFO_$(1)): $$(IPKG_$(1))
	@[ -d $(TARGET_DIR)/tmp ] || mkdir -p $(TARGET_DIR)/tmp
	$(OPKG) install $$(IPKG_$(1))
	$(if $(filter-out essential,$(PKG_FLAGS)),for flag in $(filter-out essential,$(PKG_FLAGS)); do $(OPKG) flag $$$$flag $(1); done,$(OPKG) flag ok $(1))

      $(1)-clean:
	rm -f $(PACKAGE_DIR)/$(1)_*

      clean: $(1)-clean
    endif
  endef
endif
