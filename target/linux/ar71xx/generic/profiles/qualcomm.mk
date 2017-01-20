#
# Copyright (c) 2013 The Linux Foundation. All rights reserved.
#

define Profile/QSDK_Open
	NAME:=Qualcomm-Atheros SDK Open Profile
	PACKAGES:=luci uhttpd kmod-usb-core kmod-usb2 kmod-usb-storage -kmod-ath5k \
	  kmod-ipt-nathelper-extra luci-app-upnp tftp-hpa sysstat igmpproxy kmod-ipt-nathelper-rtsp \
	  kmod-fs-msdos kmod-fs-ntfs kmod-fs-vfat kmod-nls-cp437 kmod-nls-iso8859-1 \
	  luci-app-ddns ddns-scripts kmod-ipv6 iwinfo luci-app-qos \
	  quagga quagga-ripd quagga-zebra quagga-watchquagga rp-pppoe-relay \
	  -dnsmasq dnsmasq-dhcpv6 radvd wide-dhcpv6-client \
	  -wpad-mini hostapd hostapd-utils wpa-supplicant wpa-cli wireless-tools
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
	NAME:=Qualcomm-Atheros SDK Retail Profile
	PACKAGES:=luci uhttpd kmod-ipt-nathelper-extra luci-app-upnp \
	  kmod-usb-core kmod-usb2 kmod-usb-storage kmod-fs-msdos kmod-fs-ntfs kmod-fs-vfat \
	  kmod-nls-cp437 kmod-nls-iso8859-1 tftp-hpa sysstat igmpproxy kmod-ipt-nathelper-rtsp \
	  kmod-ipv6 luci-app-qos kmod-art2-4.9-scorpion qca-spectral \
	  quagga quagga-ripd quagga-zebra quagga-watchquagga rp-pppoe-relay \
	  kmod-qca-wifi qca-hostap qca-hostapd-cli qca-wpa-cli qca-wpa-supplicant \
	  -dnsmasq dnsmasq-dhcpv6 radvd wide-dhcpv6-client \
	  luci-app-ddns ddns-scripts qca-legacy-uboot-ap135 qca-legacy-uboot-db12x \
	  -kmod-ath9k -kmod-ath5k -kmod-ath -wpad-mini hyfi
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
	# based on retail profile
	$(Profile/QSDK_Retail)
	NAME:=Qualcomm-Atheros SDK Carrier Profile
	PACKAGES+=streamboost hyfi
endef

define Profile/QSDK_Carrier/Description
	QSDK Enterprise package set configuration.
endef
$(eval $(call Profile,QSDK_Carrier))

define Profile/QSDK_Enterprise
	# based on carrier profile
	$(Profile/QSDK_Carrier)
	NAME:=Qualcomm-Atheros SDK Enterprise Profile
	PACKAGES+=kmod-ipsec kmod-ipsec4 kmod-ipsec6 ipsec-tools
endef

define Profile/QSDK_Enterprise/Description
	QSDK Enterprise package set configuration.
endef
$(eval $(call Profile,QSDK_Enterprise))

define Profile/Factory/Default
	PACKAGES:=-dropbear -firewall -ppp -wpad-mini alsa -luci-theme-openwrt \
		-kmod-ath9k -kmod-ath5k -kmod-ath -hostapd -wpa-supplicant \
		-hostapd-utils kmod-usb-core kmod-usb2 kmod-usb-storage \
		kmod-nls-cp437 kmod-nls-iso8859-1 tftp-hpa dumpregs \
		kmod-fs-msdos kmod-fs-ntfs kmod-fs-vfat avahi-daemon
endef

define Profile/Factory/Description/Default
	Qualcomm-Atheros Factory package set configuration
	This profile provides the package set required to run factory focus
	tools. In supports the following:
	- ART2 (customized for a certain chip)
	- USB tools
endef

define Profile/Factory_Wasp
	NAME:=Qualcomm-Atheros Factory Profile for Wasp chipset
	$(call Profile/Factory/Default)
	PACKAGES+=kmod-art2-wasp qca-romboot-cus227
endef

define Profile/Factory_Wasp/Description
	$(call Profile/Factory/Description/Default)
	This profile contains the ART2 build for Wasp based platform
endef
$(eval $(call Profile,Factory_Wasp))
