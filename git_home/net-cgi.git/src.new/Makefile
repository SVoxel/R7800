#
#	Copyright (C) 2007-2008 Delta Networks Inc.
#

include $(NETCGI_CONF)

FILES = main.o httpd.o lang.o apply.o asp.o auth.o action.o print.o
FILES += dni_util.o config.o transfer.o debug.o
FILES += upgrade.o util.o detwan.o ftp.o resolv.o unicode.o
FILES += basic_home_asp.o menu_asp.o adv_index_asp.o
FILES += pagetag_asp.o
FILES += home_apply.o
FILES += lan_apply.o top_apply.o hidden_page_apply.o hidden_page_asp.o
FILES += home_asp.o cfg_asp.o lan_asp.o
FILES += detfirmware.o
FILES += logs_apply.o logs_asp.o attached_devices_asp.o passwd_apply.o
FILES += static_route_apply.o static_route_asp.o
FILES += top_asp.o upgrade_asp.o lang_upgrade.o
FILES += dni_system.o
FILES += vpn_apply.o vpn_asp.o
FILES += nc_config.o
FILES += devmode_apply.o


ifeq ($(SOAP_ON), 1)
CFLAGS += -DSOAP_ON=$(SOAP_ON)
FILES += sw_soap.o sw_devconfig.o sw_devinfo.o\
	 sw_lanconfig.o\
	sw_parentalctl.o sw_auth.o\
	sw_time.o sw_agent.o sw_base64.o sw_wepkey.o
FILES += sw_app.o
 ifeq ($(HAVE_WAN), 1)
  FILES += sw_wanip.o sw_wanethlink.o
 endif
 ifeq ($(HAVE_WIFI), 1)
  FILES += sw_wlanconfig_an.o  sw_wlanconfig.o
 endif
 ifeq ($(HAVE_QOS), 1)
  FILES += sw_advancedQoS.o
 endif
 ifeq ($(HAVE_BROADBAND), 1)
  FILES += sw_wan3g.o
 endif
 ifeq ($(HAVE_SECURITY), 1)
  FILES += sw_devconfig_access.o
 endif
endif
ifeq ($(HAVE_WAN), 1)
CFLAGS += -DHAVE_WAN
FILES += internet_setup_apply.o wan_setup_apply.o internet_setup_asp.o
FILES += multipppoe_apply.o multipppoe_asp.o
endif
ifeq ($(HAVE_DSL), 1)
CFLAGS += -DHAVE_DSL=$(HAVE_DSL)
endif

ifeq ($(HAVE_WIFI), 1)
CFLAGS += -DHAVE_WIFI
FILES += wireless.o
FILES += wlan_apply.o wlan_asp.o ssid_passph_to_sec.o
FILES += guest_apply.o
FILES += wladv_apply.o wladv_asp.o
FILES += wps_apply.o wps_asp.o
FILES += wireless_repeating_apply.o wireless_repeating_asp.o
FILES += wless_schedule_apply.o wless_schedule_asp.o wless_schedule_action.o
FILES += fb_wifi_apply.o fbwifi_asp.o
FILES += gre_apply.o
endif

ifeq ($(HAVE_SECURITY), 1)
CFLAGS += -DHAVE_SECURITY
FILES += block_sites_apply.o
FILES += access_control_apply.o access_control_devices_asp.o
FILES += block_service_apply.o  block_service_asp.o
FILES += schedule_apply.o email_apply.o email_asp.o
endif
ifeq ($(HAVE_ADVANCED), 1)
CFLAGS += -DHAVE_ADVANCED
FILES += forwarding_apply.o triggering_apply.o
FILES += forwarding_asp.o triggering_asp.o
FILES += remote_apply.o
FILES += dns_asp.o dns_apply.o
FILES += upnp_apply.o upnp_asp.o
FILES += traffic_apply.o traffic_asp.o
FILES += led_apply.o
endif

ifeq ($(HAVE_IPv6), 1)
FILES += ipv6.o ipv6_apply.o ipv6_asp.o
CFLAGS += -DHAVE_IPv6
endif

ifeq ($(HAVE_OLD_QOS), 1)
FILES += qos_apply.o qos_asp.o
endif
ifeq ($(HAVE_QOS), 1)
CFLAGS += -DHAVE_QOS
FILES += qos_apply.o qos_asp.o
FILES += streamboost_apply.o streamboost_asp.o 
FILES += priority_zone_apply.o priority_zone_asp.o
endif

ifeq ($(CD_LESS), 1)
CFLAGS += -DCD_LESS
FILES += hijack_action.o hijack_apply.o hijack_asp.o
endif

ifeq ($(HAVE_USB_STORAGE), 1)
CFLAGS += -DHAVE_USB_STORAGE
FILES += usb.o usbdisk.o usb_apply.o usb_asp.o
FILES += green_apply.o green_asp.o green_upgrade.o
 ifeq ($(HAVE_BROADBAND), 1)
  FILES += readyshare_mobile_apply.o readyshare_mobile_asp.o
 endif
endif

ifeq ($(HAVE_TR069), 1)
FILES += tr069_apply.o
CFLAGS += -DHAVE_TR069
endif
ifeq ($(MULTI_OPERATION), 1)
FILES += apmode_apply.o apmode_asp.o
FILES += rae_apply.o rae_asp.o
FILES += bridge_mode_apply.o bridge_mode_asp.o
CFLAGS += -DMULTI_OPERATION
endif
ifeq ($(HAVE_BROADBAND), 1)
CFLAGS += -DHAVE_BROADBAND
FILES += broadband_apply.o
FILES += 3g_apply.o 3g_asp.o
endif
ifeq ($(HAVE_LTE),1)
CFLAGS += -DHAVE_LTE
FILES += lte_broadband_apply.o lte_apply.o lte_asp.o
endif
ifeq ($(HAVE_VPN),1)
CFLAGS += -DHAVE_VPN
endif

ifeq ($(HAVE_VLAN),1)
CFLAGS += -DHAVE_VLAN  
FILES += vlan_apply.o vlan_asp.o
endif

ifeq ($(DEPRECATED), 1)

 ifeq ($(HAVE_PLC), 1)
  FILES += plc_devs.o plc_devs_action.o plc_devs_apply.o plc_devs_asp.o
  CFLAGS += -DHAVE_PLC
 endif
 ifeq ($(HAVE_FAST_LANE), 1)
  CFLAGS += -DHAVE_FAST_LANE
  FILES += fastlane_apply.o
 endif
 ifeq ($(HNAP_ON), 1)
  CFLAGS += -DHNAP_ON
  FILES += hnap_agent.o hnap_general.o hnap_router.o hnap_soap.o hnap_wireless.o
 endif
 ifeq ($(HDD_MULTI_USER), 1)
  CFLAGS += -DHDD_MULTI_USER
  FILES += hdd_action.o hdd_apply.o hdd_asp.o
 endif
 ifeq ($(FAN_PROBE_SET),1)
  CFLAGS += -DFAN_PROBE_SET
 endif

endif


CFLAGS += -I$(STAGING_DIR)/usr/lib/libiconv/include
CFLAGS += -Wall -Wunused -g -O2

LDFLAGS += -L$(STAGING_DIR)/usr/lib/libiconv/lib

USBLDFLAGS =
ifeq ($(HAVE_USB_STORAGE),1)
USBLDFLAGS += -liconv
endif


.PHONY: all install clean
all : net-cgi usb_cfg inetd detplc remote_smb_conf remote_user_conf remote_share_conf readycloud_unregister

net-cgi : $(FILES)
	$(CC) $(CFLAGS) $^ -o $@ $(EXTRA_LDFLAGS) $(LDFLAGS) -lconfig -lpthread $(USBLDFLAGS)
	@echo "make $@ finished on `date`"

usb_cfg : usbdisk.o usb_cfg.o print.o nc_config.o
	$(CC) $(CFLAGS) $^ -o $@ $(EXTRA_LDFLAGS) $(LDFLAGS) -lconfig $(USBLDFLAGS)

inetd : http_scan.o
	$(CC) $(CFLAGS) $^ -o $@ $(EXTRA_LDFLAGS) $(LDFLAGS)

remote_smb_conf remote_user_conf remote_share_conf readycloud_unregister: usbdisk.o remote_smb_conf.o print.o nc_config.o
	$(CC) $(CFLAGS) $^ -o $@ $(EXTRA_LDFLAGS) $(LDFLAGS) -lconfig $(USBLDFLAGS)

detplc: plc_devs_detect.o
	$(CC) $(CFLAGS) $^ -o $@ $(EXTRA_LDFLAGS) $(LDFLAGS) -lconfig

%.o : %.c
	$(CC) $(CFLAGS) -c $<

install:
	$(INSTALL_DIR)	$(INS_BIN_DIR)
	$(INSTALL_BIN)	net-cgi		$(INS_BIN_DIR)
	$(INSTALL_BIN)	inetd		$(INS_BIN_DIR)
ifeq ($(HAVE_USB_STORAGE),1)
	$(INSTALL_BIN)	usb_cfg		$(INS_BIN_DIR)
	$(INSTALL_BIN)  remote_smb_conf         $(INS_BIN_DIR)
	$(INSTALL_BIN)  remote_user_conf        $(INS_BIN_DIR)
	$(INSTALL_BIN)  remote_share_conf       $(INS_BIN_DIR)
	$(INSTALL_BIN)  readycloud_unregister   $(INS_BIN_DIR)
endif

ifeq ($(DEPRECATED), 1)
 ifeq ($(HAVE_PLC),1)
	$(INSTALL_BIN)	detplc		$(INS_BIN_DIR)
 endif
endif
clean:
	rm -f *.o 
	rm -f net-cgi usb_cfg inetd detplc remote_smb_conf remote_user_conf remote_share_conf readycloud_unregister
