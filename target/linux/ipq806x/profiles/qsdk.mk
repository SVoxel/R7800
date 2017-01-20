#
# Copyright (c) 2013 The Linux Foundation. All rights reserved.
#

define Profile/QSDK_Base
	PACKAGES:=luci uhttpd kmod-ipt-nathelper-extra luci-app-upnp kmod-fs-ext4 \
	  kmod-usb-storage kmod-fs-msdos kmod-fs-ntfs kmod-fs-vfat \
	  kmod-sound-core kmod-sound-soc-ipq806x alsa mplayer \
	  ntfs-3g dosfsck e2fsprogs iozone fdisk mkdosfs \
	  kmod-nls-cp437 kmod-nls-iso8859-1 tftp-hpa sysstat igmpproxy kmod-ipt-nathelper-rtsp \
	  kmod-ipv6 iperf devmem2 ip ethtool ip6tables \
	  quagga quagga-ripd quagga-zebra quagga-watchquagga rp-pppoe-relay \
	  -dnsmasq dnsmasq-dhcpv6 radvd wide-dhcpv6-client luci-app-samba bridge \
	  luci-app-ddns ddns-scripts cups cups-client mdadm\
	  kmod-md-mod kmod-md-linear kmod-md-raid0 kmod-md-raid1 \
	  iputils-tracepath iputils-tracepath6 \
	  alljoyn alljoyn-c alljoyn-about alljoyn-config alljoyn-controlpanel \
	  alljoyn-notification alljoyn-samples alljoyn-services_common \
	  kmod-qca-ssdk-nohnat qca-ssdk-shell \
	  kmod-art2 file pure-ftpd xl2tpd ppp-mod-pptp flock
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
	PACKAGES+=kmod-qca-wifi-akronite-perf qca-hostap qca-hostapd-cli qca-wpa-cli qca-wpa-supplicant \
	  qca-spectral qca-wapid sigma-dut streamboost-noload \
	  kmod-qca-nss-connmgr hyfi kmod-qca-nss-tun6rd kmod-qca-nss-tunipip6 \
	  qca-nss-fw-retail
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
	PACKAGES+=kmod-qca-wifi-akronite-perf qca-hostap qca-hostapd-cli qca-wpa-cli qca-wpa-supplicant \
		  qca-spectral qca-wapid hyfi streamboost sigma-dut \
		  qca-nss-fw-carrier
endef

define Profile/QSDK_Carrier/Description
	QSDK Enterprise package set configuration.
endef
$(eval $(call Profile,QSDK_Carrier))

define Profile/QSDK_Enterprise
	$(Profile/QSDK_Base)
	NAME:=Qualcomm-Atheros SDK Enterprise Profile
	PACKAGES+=kmod-qca-wifi-akronite-perf qca-hostap qca-hostapd-cli qca-wpa-cli qca-wpa-supplicant \
	  qca-spectral qca-wapid kmod-qca-nss-connmgr-noload luci-app-qos \
	  kmod-openswan-nss openswan-nss luci-app-openswan \
	  kmod-crypto-ocf kmod-qca-nss-crypto kmod-qca-nss-cfi sigma-dut \
	  qca-nss-fw-enterprise
endef

define Profile/QSDK_Enterprise/Description
	QSDK Enterprise package set configuration.
endef
$(eval $(call Profile,QSDK_Enterprise))
