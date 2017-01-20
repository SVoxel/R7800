#
# Copyright (c) 2013 The Linux Foundation. All rights reserved.
#

define Profile/QSDK_Base
	PACKAGES:=bridge
endef

define Profile/QSDK_Open
	NAME:=Qualcomm-Atheros SDK Open Profile
	PACKAGES+=kmod-ath9k wpad-mini \
	  kmod-qca-nss-connmgr
endef

define Profile/QSDK_Open/Description
	QSDK Open package set configuration.
	This profile includes only open source packages and provides basic Wi-Fi router features using the QCA upstream Linux Wi-Fi driver. It supports:
	-Bridging and routing networking
	-LuCI web configuration interface
	-Integrated 11abgn support using the ath9k driver
endef
$(eval $(call Profile,QSDK_Open))

define Profile/QSDK_Retail
	$(Profile/QSDK_Base)
	NAME:=Qualcomm-Atheros SDK Retail Profile
	PACKAGES+=kmod-qca-wifi qca-hostap qca-hostapd-cli qca-wpa-cli qca-wpa-supplicant \
	  qca-spectral \
	  kmod-qca-nss-connmgr kmod-qca-nss-tun6rd kmod-qca-nss-tunipip6
endef

define Profile/QSDK_Retail/Description
	QSDK Retail package set configuration.
	This profile provides basic Wi-Fi router features using the QCA proprietary Wi-Fi driver. It supports:
	-Bridging and routing networking
	-LuCI web configuration interface
	-Integrated 11abgn/ac support using the proprietary qca-wifi driver
endef
$(eval $(call Profile,QSDK_Retail))

define Profile/QSDK_Carrier
	$(Profile/QSDK_Base)
	NAME:=Qualcomm-Atheros SDK Carrier Profile
	PACKAGES+=kmod-qca-wifi qca-hostap qca-hostapd-cli qca-wpa-cli qca-wpa-supplicant \
		  qca-spectral hyfi streamboost
endef

define Profile/QSDK_Carrier/Description
	QSDK Enterprise package set configuration.
endef
$(eval $(call Profile,QSDK_Carrier))

define Profile/QSDK_Enterprise
	$(Profile/QSDK_Base)
	NAME:=Qualcomm-Atheros SDK Enterprise Profile
	PACKAGES+=kmod-qca-wifi qca-hostap qca-hostapd-cli qca-wpa-cli qca-wpa-supplicant \
	  qca-spectral kmod-qca-nss-connmgr-noload \
	  kmod-openswan-nss openswan-nss kmod-crypto-ocf kmod-qca-nss-crypto kmod-qca-nss-cfi 
endef

define Profile/QSDK_Enterprise/Description
	QSDK Enterprise package set configuration.
endef
$(eval $(call Profile,QSDK_Enterprise))
