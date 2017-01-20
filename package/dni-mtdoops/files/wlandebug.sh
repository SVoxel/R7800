#!/bin/sh

usage(){
    echo "wlandebug"
    echo " option:"
    echo "  -b [0|1] set/clear basic system information"
    echo "  -a [0|1] set/clear advanced system information"
    echo "  -w [0|1] set/clear basic wireless information"
    echo "  -h [0|1] set/clear hostapd debug flag"
}

print_section_hd(){
    printf "########### [ %-20s ] #############\n" "$1"
}

basic_sys_info=1
advanced_sys_info=1
basic_wlan_info=1
qca_debug_tool=1
hostapd_debug=0


while getopts "b:a:w:h:" opt;do
    case "$opt" in
        b)
            basic_sys_info=$OPTARG;;
        a)
            advanced_sys_info=$OPTARG;;
        w)
            basic_wlan_info=$OPTARG;;
        h)
            hostapd_debug=$OPTARG;;
        *)
            echo "Unknown option $OPTARG"; usage; exit 1;;
    esac
done

# echo "basic_sys_info=$basic_sys_info"
# echo "advanced_sys_info=$advanced_sys_info"
# echo "basic_wlan_info=$basic_wlan_info"
# echo "hostapd_debug=$hostapd_debug"

# if [ $hostapd_debug -eq 1 ]; then
    # wlan down;
    # HOSTAPD_DEBUG=1 wlan up
# fi

if [ $basic_sys_info -eq 1 ]; then
    [ -f /bin/config ] && {
        print_section_hd 'Configuration file'
        /bin/config show
    }

    print_section_hd 'Show Wireless Settings'

    [ -f /etc/config/wireless ] && cat /etc/config/wireless
    [ -f /sbin/dni_apup ] && cat /sbin/dni_apup

    print_section_hd "uptime"
    cat /proc/uptime

    print_section_hd "meminfo"
    cat /proc/meminfo

    print_section_hd "vmstat"
    cat /proc/vmstat

    [ -f /proc/net/arp ] && {
        print_section_hd "arp"
        cat /proc/net/arp
    }

    [ -f /proc/bus/pci/devices ] && {
        print_section_hd "/proc/bus/pci/devices"
        cat /proc/bus/pci/devices
    }
    print_section_hd "ifconfig"
    ifconfig

    print_section_hd "Process List"
    ps

    print_section_hd "Bridge Information"
    brctl show

    print_section_hd "interrupts (1)"
    cat /proc/interrupts

    sleep 3
    print_section_hd "interrupts (2)"
    cat /proc/interrupts
fi

if [ $advanced_sys_info -eq 1 ]; then
    print_section_hd "modules"
    cat /proc/modules

    # print_section_hd "kallsyms"
    # cat /proc/kallsyms

    [ -f /proc/softirqs ] && {
        print_section_hd "softirqs"
        cat /proc/softirqs
    }
fi

vif_ap_list=`iwconfig 2>/dev/null | grep -e Mode:Master -B1 | grep -e IEEE | awk '{print $1}'`
wifi_list=`ifconfig | grep "^wifi" | awk '{print $1}'`
    
if [ $basic_wlan_info -eq 1 ]; then
    [ -f /proc/athversion ] && { 
        print_section_hd "Wireless Driver version"
        cat /proc/athversion
    }
    
    print_section_hd "iwconfig"

    iwconfig 2>/dev/null

    print_section_hd "Associated station information"

    for vap in $vif_ap_list; do
        echo "[VAP: $vap]"
        wlanconfig $vap list sta

        echo ""
    done

fi

if [ $qca_debug_tool -eq 1 ]; then
    print_section_hd "apstats"

    apstats -R

    print_section_hd "80211stats"

    for vap in $vif_ap_list; do
        echo "[VAP: $vap]"
        80211stats -i $vap
   
        echo ""
    done

    print_section_hd "athstats"
    for w in $wifi_list; do
        echo "[Radio: $w]"
        athstats -i $w
   
        echo ""
    done
fi

# dbgLVL, getdbgLVL, dbgLVL_high, getdbgLVL_high

# enum {
#     /* IEEE80211_PARAM_DBG_LVL */
#     IEEE80211_MSG_TDLS      = 0,  0x00000001   /* TDLS */
#     IEEE80211_MSG_ACS,            0x00000002   /* auto channel selection */
#     IEEE80211_MSG_SCAN_SM,        0x00000004   /* scan state machine */
#     IEEE80211_MSG_SCANENTRY,      0x00000008   /* scan entry */
#     IEEE80211_MSG_WDS,            0x00000010   /* WDS handling */
#     IEEE80211_MSG_ACTION,         0x00000020   /* action management frames */
#     IEEE80211_MSG_ROAM,           0x00000040   /* sta-mode roaming */
#     IEEE80211_MSG_INACT,          0x00000080   /* inactivity handling */
#     IEEE80211_MSG_DOTH      = 8,  0x00000100   /* 11.h */
#     IEEE80211_MSG_IQUE,           0x00000200   /* IQUE features */
#     IEEE80211_MSG_WME,            0x00000400   /* WME protocol */
#     IEEE80211_MSG_ACL,            0x00000800   /* ACL handling */
#     IEEE80211_MSG_WPA,            0x00001000   /* WPA/RSN protocol */
#     IEEE80211_MSG_RADKEYS,        0x00002000   /* dump 802.1x keys */
#     IEEE80211_MSG_RADDUMP,        0x00004000   /* dump 802.1x radius packets */
#     IEEE80211_MSG_RADIUS,         0x00008000   /* 802.1x radius client */
#     IEEE80211_MSG_DOT1XSM   = 16, 0x00010000   /* 802.1x state machine */
#     IEEE80211_MSG_DOT1X,          0x00020000   /* 802.1x authenticator */
#     IEEE80211_MSG_POWER,          0x00040000   /* power save handling */
#     IEEE80211_MSG_STATE,          0x00080000   /* state machine */
#     IEEE80211_MSG_OUTPUT,         0x00100000   /* output handling */
#     IEEE80211_MSG_SCAN,           0x00200000   /* scanning */
#     IEEE80211_MSG_AUTH,           0x00400000   /* authentication handling */
#     IEEE80211_MSG_ASSOC,          0x00800000   /* association handling */
#     IEEE80211_MSG_NODE      = 24, 0x01000000   /* node handling */
#     IEEE80211_MSG_ELEMID,         0x02000000   /* element id parsing */
#     IEEE80211_MSG_XRATE,          0x04000000   /* rate set handling */
#     IEEE80211_MSG_INPUT,          0x08000000   /* input handling */
#     IEEE80211_MSG_CRYPTO,         0x10000000   /* crypto work */
#     IEEE80211_MSG_DUMPPKTS,       0x20000000   /* IFF_LINK2 equivalant */
#     IEEE80211_MSG_DEBUG,          0x40000000   /* IFF_DEBUG equivalent */
#     IEEE80211_MSG_MLME,           0x80000000   /* MLME */
#     /* IEEE80211_PARAM_DBG_LVL_HIGH */
#     IEEE80211_MSG_RRM       = 32,           /* Radio resource measurement */
#     IEEE80211_MSG_WNM,                      /* Wireless Network Management */
#     IEEE80211_MSG_P2P_PROT,                 /* P2P Protocol driver */
#     IEEE80211_MSG_PROXYARP,                 /* 11v Proxy ARP */
#     IEEE80211_MSG_L2TIF,                    /* Hotspot 2.0 L2 TIF */
#     IEEE80211_MSG_WIFIPOS,                  /* WifiPositioning Feature */
#     IEEE80211_MSG_WRAP,                     /* WRAP or Wireless ProxySTA */
#     IEEE80211_MSG_DFS,                      /* DFS debug mesg */

#     IEEE80211_MSG_NUM_CATEGORIES,           /* total ieee80211 messages */
#     IEEE80211_MSG_UNMASKABLE = IEEE80211_MSG_MAX,  /* anything */
#     IEEE80211_MSG_ANY = IEEE80211_MSG_MAX,  /* anything */
# };


# ATHDebug, getATHDebug

# enum {
#     ATH_DEBUG_XMIT = 0,    /* basic xmit operation */
#     ATH_DEBUG_XMIT_DESC,   /* xmit descriptors */
#     ATH_DEBUG_RECV,        /* basic recv operation */
#     ATH_DEBUG_RECV_DESC,   /* recv descriptors */
#     ATH_DEBUG_RATE,        /* rate control */
#     ATH_DEBUG_RESET,       /* reset processing */
#     ATH_DEBUG_MAT,         /* MAT for s/w proxysta */
#     ATH_DEBUG_BEACON,      /* beacon handling */
#     ATH_DEBUG_WATCHDOG,    /* watchdog timeout */
#     ATH_DEBUG_SCAN,        /* scan debug prints */
#     ATH_DEBUG_GREEN_AP,    /* GreenAP debug prints */
#     ATH_DEBUG_HTC_WMI,     /* htc/wmi debug prints */
#     ATH_DEBUG_INTR,        /* ISR */
#     ATH_DEBUG_TX_PROC,     /* tx ISR proc */
#     ATH_DEBUG_RX_PROC,     /* rx ISR proc */
#     ATH_DEBUG_BEACON_PROC, /* beacon ISR proc */
#     ATH_DEBUG_CALIBRATE,   /* periodic calibration */
#     ATH_DEBUG_KEYCACHE,    /* key cache management */
#     ATH_DEBUG_STATE,       /* 802.11 state transitions */
#     ATH_DEBUG_NODE,        /* node management */
#     ATH_DEBUG_LED,         /* led management */
#     ATH_DEBUG_TX99,        /* tx99 function */
#     ATH_DEBUG_DCS,         /* dynamic channel switch */
#     ATH_DEBUG_UAPSD,       /* uapsd */
#     ATH_DEBUG_DOTH,        /* 11.h */
#     ATH_DEBUG_CWM,         /* channel width managment */
#     ATH_DEBUG_PPM,         /* Force PPM management */
#     ATH_DEBUG_PWR_SAVE,    /* PS Poll and PS save */
#     ATH_DEBUG_SWR,         /* SwRetry mechanism */
#     ATH_DEBUG_AGGR_MEM,
#     ATH_DEBUG_BTCOEX,      /* BT coexistence */
#     ATH_DEBUG_FATAL,       /* fatal errors */
#     ATH_DEBUG_WNM_FMS,
#     /*
#      * First fill in the UNUSED values above, then
#      * add new values here.
#      */

#     ATH_DEBUG_NUM_CATEGORIES,
#     ATH_DEBUG_ANY,
#     ATH_DEBUG_UNMASKABLE = ATH_DEBUG_MAX_MASK,


# HALDbg, GetHALDbg,

# Special part, only for QTN.

print_section_hd "Status of QTN EP side"
ping 1.1.1.2 -c 3 2>&1

print_section_hd "Status of configuring QTN module"
cat /tmp/qt-wireless.log
