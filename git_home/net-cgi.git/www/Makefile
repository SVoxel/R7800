
include $(NETCGI_CONF)

# opt_folders is a string list of all selected folders' name in www
opt_folders:=

ifneq ($(NEW_SOURCE),)

opt_folders+=guest_manage
opt_folders+=device_name
opt_folders+=multipppoe
opt_folders+=l2tp
opt_folders+=debug
opt_folders+=rip

ifeq ($(HAVE_SECURITY), 1)
opt_folders+=block_sites
opt_folders+=access_control
opt_folders+=schedule
opt_folders+=email
opt_folders+=parental_control
endif
ifeq ($(HAVE_ADVANCED), 1)
opt_folders+=old_forwarding
opt_folders+=forwarding_range
opt_folders+=triggering
opt_folders+=ddns
opt_folders+=netgear_ddns
opt_folders+=upnp
opt_folders+=traffic
opt_folders+=remote
opt_folders+=led_control
endif
ifeq ($(HAVE_WIFI), 1)
  opt_folders+=wireless
  opt_folders+=guest
  opt_folders+=wl_bridge
  opt_folders+=wds
  opt_folders+=wps
  opt_folders+=gre
endif
ifeq ($(CD_LESS),1)
  opt_folders+=browser_hijack
  opt_folders+=cd_less_download
endif
ifeq ($(HAVE_USB_STORAGE),1)
  opt_folders+=usb_storage
  opt_folders+=readyshare_remote
  opt_folders+=green_download
  opt_folders+=readyshare_vault
  opt_folders+=readyshare_print
  ifeq ($(HAVE_BROADBAND), 1)
    opt_folders+=readyshare_mobile
  endif
endif
ifeq ($(HAVE_QOS),1)
  opt_folders+=dynamic_qos
  opt_folders+=advanced_qos
  opt_folders+=priority_zone
endif
ifeq ($(HAVE_IPv6),1)
  opt_folders+=ipv6
endif
ifeq ($(HAVE_VPN),1)
  opt_folders+=vpn
  opt_folders+=vpn_pptp
endif
ifeq ($(HAVE_TR069),1)
  opt_folders+=tr069
endif
ifeq ($(MULTI_OPERATION),1)
  opt_folders+=apmode
  opt_folders+=rae
endif
ifeq ($(HAVE_VLAN),1)
  opt_folders+=vlan
endif
ifeq ($(HAVE_BROADBAND),1)
  opt_folders+=3g
  opt_folders+=browser_hijack_3g
endif
ifeq ($(HAVE_LTE),1)
  opt_folders+=lte
  opt_folders+=browser_hijack_lte
endif
ifeq ($(CD_LESS),1)
  ifeq ($(MULTI_OPERATION), 1)
  opt_folders+=browser_hijack_ap
  endif
endif
ifeq ($(HAVE_DSL),1)
  opt_folders+=dsl_wiard
endif

ifeq ($(DEPRECATED), 1)
 ifeq ($(HAVE_BIGPOND),1)
  opt_folders+=bigpond
 endif
 ifeq ($(HAVE_USB_STORAGE),1)
  ifeq ($(HDD_MULTI_USER),1)
   opt_folders+=user_hdd_storage
  endif
 endif
 ifeq ($(HAVE_OLD_QOS),1)
  opt_folders+=qos
 endif
 ifeq ($(HAVE_PLC),1)
  opt_folders+=plc
 endif
 ifeq ($(HAVE_FAST_LANE),1)
  opt_folders+=fastlane
 endif
 ifeq ($(HAVE_OPENVPN),1)
  opt_folders+=openvpn
 endif

endif


else #NEW_SOURCE

opt_folders+=wireless
opt_folders+=wps
opt_folders+=triggering
opt_folders+=old_forwarding
opt_folders+=upnp
opt_folders+=ddns
opt_folders+=schedule
opt_folders+=email
opt_folders+=parental_control
opt_folders+=remote
opt_folders+=led_control
ifeq ($(CD_LESS),1)
  opt_folders+=browser_hijack
  ifeq ($(CD_LESS_DOWNLOAD),1)
    opt_folders+=cd_less_download
  endif
endif
ifeq ($(HAVE_USB_STORAGE),1)
  opt_folders+=usb_storage
  ifeq ($(GREEN_DOWNLOAD),1)
    opt_folders+=green_download
  endif
  ifeq ($(HDD_MULTI_USER),1)
    opt_folders+=user_hdd_storage
  endif
endif
ifeq ($(GENIECLOUD),1)
  opt_folders+=readyshare_remote
endif
ifeq ($(READYSHARE_PRINT),1)
  opt_folders+=readyshare_print
endif
ifeq ($(HAVE_VAULT),1)
  opt_folders+=readyshare_vault
endif
ifeq ($(HAVE_L2TP),1)
  opt_folders+=l2tp
endif
ifeq ($(HAVE_BIGPOND),1)
  opt_folders+=bigpond
endif
ifeq ($(HAVE_GUEST_NETWORK),1)
  opt_folders+=guest
endif
ifeq ($(HAVE_GRE_NETWORK),1)
  opt_folders+=gre
endif
ifeq ($(HAVE_QOS),1)
  opt_folders+=qos
endif
ifeq ($(HAVE_DYNA_QOS),1)
  opt_folders+=dynamic_qos
  ifeq ($(HAVE_ADVANCED_QOS),1)
    opt_folders+=advanced_qos
  endif
endif
ifeq ($(HAVE_WDS),1)
  opt_folders+=wds
endif
ifeq ($(HAVE_IPv6),1)
  opt_folders+=ipv6
endif
ifeq ($(HAVE_VPN),1)
  opt_folders+=vpn
endif
ifeq ($(HAVE_PPTP_VPN),1)
  opt_folders+=vpn_pptp
endif
ifeq ($(HAVE_TRAFFIC_METER),1)
  opt_folders+=traffic
endif
ifeq ($(HAVE_BLOCK_SITES),1)
  opt_folders+=block_sites
endif
ifeq ($(HAVE_ACCESS_CONTROL),1)
  opt_folders+=access_control
endif
ifeq ($(HAVE_DEVICE_NAME),1)
  opt_folders+=device_name
endif
ifeq ($(HAVE_TR069),1)
  opt_folders+=tr069
endif
ifeq ($(HAVE_PLC),1)
  opt_folders+=plc
endif
ifeq ($(HAVE_AP_MODE),1)
  opt_folders+=apmode
endif
ifeq ($(HAVE_JP),1)
  opt_folders+=multipppoe
endif
ifeq ($(NETGEAR_DDNS),1)
  opt_folders+=netgear_ddns
endif
ifeq ($(HAVE_VLAN),1)
  opt_folders+=vlan
endif
ifneq ($(OPERATION_MODES),0)
  opt_folders+=apmode
  opt_folders+=rae
endif
ifeq ($(HAVE_FAST_LANE),1)
  opt_folders+=fastlane
endif
ifeq ($(HAVE_BRIDGE_MODE),1)
  opt_folders+=wl_bridge
endif
ifneq ($(HAVE_WIRELESS_AN),1)
  opt_folders+=bgn_wireless
endif
ifeq ($(FORWARD_RANGE),1)
  opt_folders+=forwarding_range
endif
ifeq ($(READYSHARE_MOBILE),1)
  opt_folders+=readyshare_mobile
endif
ifeq ($(HAVE_BROADBAND),1)
  opt_folders+=3g
  opt_folders+=browser_hijack_3g
endif
ifeq ($(HAVE_LTE),1)
  opt_folders+=lte
  opt_folders+=browser_hijack_lte
endif
ifeq ($(GREEN_DOWNLOAD),1)
  opt_folders+=green_download
endif
ifeq ($(AP_MODE_DETECTION),1)
  opt_folders+=browser_hijack_ap
endif
ifeq ($(HAVE_DSL),1)
  opt_folders+=dsl_wiard
endif
ifeq ($(GUEST_MANAGE),1)
  opt_folders+=guest_manage
endif
ifeq ($(HAVE_DEBUG),1)
  opt_folders+=debug
endif
ifeq ($(HAVE_OPENVPN),1)
  opt_folders+=openvpn
endif
ifeq ($(HAVE_RIP),1)
  opt_folders+=rip
endif
endif #NEW_SOURCE

define copy_folder
	$(CP) $(1)/* $(WWW_OUT_DIR);
endef

virtual_cgis:=apply.cgi func.cgi upgrade_check.cgi upgrade.cgi \
	backup.cgi brs_backup.cgi restore.cgi langupg.cgi \
	ubootupg.cgi unauth.cgi recover.cgi debug_cloud.cgi \
	debug.cgi green_upg.cgi bt_file.cgi mobile_install.cgi

.PHONY: all install clean
all:
	echo $(opt_folders)
	$(INSTALL_DIR)		$(WWW_OUT_DIR)
	$(CP) *.*		$(WWW_OUT_DIR)
	$(CP) image/		$(WWW_OUT_DIR)
	$(CP) style/		$(WWW_OUT_DIR)
	$(CP) help/		$(WWW_OUT_DIR)
	$(CP) language/		$(WWW_OUT_DIR)
ifeq ($(NEW_SOURCE),)
	$(CP) cgi-bin/		$(WWW_OUT_DIR)
endif

	$(foreach n,$(strip $(opt_folders)),$(call copy_folder,$(n)))

	$(TOUCH) $(foreach n,$(virtual_cgis),$(WWW_OUT_DIR)/$(n))

install:
	$(INSTALL_DIR)		$(INS_WWW_DIR)
	$(CP) $(WWW_OUT_DIR)/*	$(INS_WWW_DIR)

clean:
	rm -rf $(WWW_OUT_DIR)
