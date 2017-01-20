#
# Makefile for Atheros tools.
#

ifeq ($(obj),)
obj := .
endif

DEPTH := ../../..

ifeq ($(BINDIR),)
BINDIR=/usr/local/bin
endif

include $(obj)/${DEPTH}/os/linux/Makefile.inc

# Remove all the kernel include from the INCS variable as we're building
# a user space application
KMAJ := $(strip $(shell head -n 1 ${KERNELPATH}/Makefile | cut -f 2 -d'='))
KMIN := $(strip $(shell head -n 2 ${KERNELPATH}/Makefile | tail -1 | cut -f 2 -d'='))
KREV := $(strip $(shell head -n 3 ${KERNELPATH}/Makefile | tail -1 | cut -f 2 -d'='))

kver_ge = $(shell [ $(KMAJ)$(KMIN)$(KREV) -ge $(1)$(2)$(3) ] && \
		echo 1 || echo 0)
	
ifeq ($(call kver_ge,3,10,0), 1)
ifeq ($(QCA_THIRDPARTY), 1)
INCS:=$(filter-out -I${KERNELPATH}%, ${INCS})
COPTS:=$(filter-out -isystem${KERNELPATH}%, ${COPTS})
else
INCS:=$(filter-out -I${KERNELPATH}%, ${INCS})
COPTS:=$(filter-out -isystem${KERNELPATH}%, ${COPTS})
INCS += -I$(KERNELPATH)/arch/$(KERNELARCH)/include/uapi
INCS += -I$(KERNELPATH)/arch/$(KERNELARCH)/include/generated/uapi
INCS += -I$(KERNELPATH)/include/uapi
INCS += -I$(KERNELPATH)/include/generated/uapi
INCS += -I$(KERNELPATH)/include
endif
else
ifeq ($(QCA_THIRDPARTY), 1)
INCS:=$(filter-out -I${KERNELPATH}%, ${INCS})
COPTS:=$(filter-out -isystem${KERNELPATH}%, ${COPTS})
else
#INCS:=$(filter-out -I${KERNELPATH}%, ${INCS})
#COPTS:=$(filter-out -isystem${KERNELPATH}%, ${COPTS})
endif
endif

ifeq ($(ATH_SUPPORT_CFG_8K), 1)
ALL=  cgiMain 80211stats athstats athstatsclr apstats athkey athchans athctrl athdebug wlanconfig \
      athcwm athampdutrc atrc pktlogconf pktlogdump thermaltool

ATH_TOOLS= cgiMain 80211stats athstats athstatsclr apstats pktlogconf pktlogdump wifitool wlanconfig thermaltool
else
ALL=  80211stats athstats athstatsclr apstats athkey athchans athctrl athdebug wlanconfig \
      athcwm athampdutrc atrc pktlogconf pktlogdump thermaltool wps_enhc
ATH_TOOLS= 80211stats athstats athstatsclr apstats pktlogconf pktlogdump wifitool wlanconfig thermaltool wps_enhc
ifeq ($(UMAC_SUPPORT_WNM), 1)
ATH_TOOLS += wnm-app
endif
endif

ifeq ($(DEBUG_TOOLS), 1)
ATH_TOOLS += dumpregs reg
endif


ALL += athadhoc
ATH_TOOLS += athadhoc

ifneq ($(strip ${ATH_SSID_STEERING}),0)
ALL += ssidsteering
ATH_TOOLS += ssidsteering
endif

ifneq ($(strip ${ASSOC_DENIAL_NOTIFY}),0)
ALL += assocdenialnotify
ATH_TOOLS += assocdenialnotify
endif

ifneq ($(BOARD_TYPE),ap91-2MB)
ifneq ($(strip ${ATH_SUPPORT_DFS}),0)
#ifneq ($(ATH_SUPPORT_HTC),1)
ALL+= radartool
ATH_TOOLS+= radartool
#endif
endif

ifneq ($(strip ${ATH_SUPPORT_AOW}),0)
ALL+= athplay
ATH_TOOLS+= athplay
endif

ifneq ($(strip ${ATH_SUPPORT_AOW}),0)
ALL+= mplay
ATH_TOOLS+= mplay
ALL+= aowstatlog
ATH_TOOLS+= aowstatlog
endif

ifneq ($(strip ${ATH_SUPPORT_SPECTRAL}),0)
ALL+= spectraltool
ATH_TOOLS+= spectraltool
SPECTRAL_INC := $(ATHEROSPATH)/lmac/spectral
INCS += -I$(SPECTRAL_INC)
# The following are added to compile spectraltool for x86
# platform
ifeq ($(TARGET),x86)
OS_VERSION:=$(shell uname -r)
AUTOCONF_PATH:=/usr/src/linux-headers-$(OS_VERSION)/include/
INCS += -I$(AUTOCONF_PATH)
endif
endif


ifneq ($(strip ${UMAC_SUPPORT_RPTPLACEMENT}),0)
ALL+= rpttool 
ATH_TOOLS+= rpttool
ifeq ($(KERNELVER),2.6.31)
IWDIR=$(TOPDIR)/apps/wireless_tools.29
else 
IWDIR=$(TOPDIR)/apps/wireless_tools.28
endif
endif

ifeq ($(ATH_SUPPORT_TX99),1)
ALL+= tx99tool
ATH_TOOLS+= tx99tool
INCS += -I$(TX99)
endif
endif

all:	${ALL}

ifdef ATHEROSPATH
INCS+= -I$(ATHEROSPATH)
endif
INCS+= -I${HAL} -I${HAL}/${OS} -I${ATH_HAL} -I${obj}/${DEPTH} -I${COMMON} -I${ATH_RATE} -I${ATH} -I${WLAN} 
CFLAGS += ${INCS} ${COPTS}

ifeq (${BUILD_STATIC}, y)
LDFLAGS= -static
endif

ifeq (${BUILD_DEBUG}, y)
CFLAGS += -g
endif

ifeq ($(ATH_CONFIG_NVRAM), 1)
CFLAGS += -DCONFIG_NVRAM=1
endif

ifndef __EXPORTED_HEADERS__
CFLAGS += -D__EXPORTED_HEADERS__
endif

CFLAGS += -DCONFIG_LOCALPC=1 -DCONFIG_DATA_AT_ROOT=1

ifeq ($(BUILD_OPTIMIZED),y)
ifeq ($(KERNELARCH),mips)
CFLAGS += -Os -pipe -mips32r2 -mtune=mips32r2 -funit-at-a-time
else
CFLAGS += -Os -pipe -funit-at-a-time
endif
endif

ifndef BUILD_HOSTAPD_071
export BUILD_HOSTAPD_071=0
endif

ifneq ($(strip ${BUILD_HOSTAPD_071}),0)
COPTS+= -DCFG_HOSTAPD_071=1
endif

ifndef BUILD_HOSTAPD_072
export BUILD_HOSTAPD_072=0
endif

ifneq ($(strip ${BUILD_HOSTAPD_072}),0)
COPTS+= -DCFG_HOSTAPD_072=1
endif

ifndef BUILD_ATHR_HOSTAP
export BUILD_ATHR_HOSTAP=0
endif

ifneq ($(strip ${BUILD_ATHR_HOSTAP}),0)
COPTS+= -DCFG_ATHR_HOSTAP=1
endif

all:	${ALL}

ath_tools: ${ATH_TOOLS}
ifneq ($(BOARD_TYPE),ap91-2MB)
	cp athstats $(INSTALL_ROOT)/sbin;\
	cp 80211stats $(INSTALL_ROOT)/sbin;\
	cp athstatsclr $(INSTALL_ROOT)/sbin;\
	cp apstats $(INSTALL_ROOT)/sbin;\
	cp pktlogconf $(INSTALL_ROOT)/sbin;\
	cp pktlogdump $(INSTALL_ROOT)/sbin;\
	cp wifitool $(INSTALL_ROOT)/sbin;
	cp wlanconfig $(INSTALL_ROOT)/sbin;
	cp thermaltool $(INSTALL_ROOT)/sbin;
ifeq ($(ATH_SUPPORT_CFG_8K), 1)
	cp cgiMain $(INSTALL_ROOT)/sbin;
	cp scripts/wlan/rc.* $(INSTALL_ROOT)/sbin;
	cp scripts/network $(INSTALL_ROOT)/sbin;
	cp scripts/ip8k/ap* $(INSTALL_ROOT)/sbin;
	cp scripts/ip8k/*VAP $(INSTALL_ROOT)/sbin;
	cp scripts/ip8k/rcS $(INSTALL_ROOT)/sbin;
	cp scripts/ip8k/rc.network $(INSTALL_ROOT)/sbin;
	cp scripts/ip8k/rc.bridge $(INSTALL_ROOT)/sbin;
else
	cp wps_enhc $(INSTALL_ROOT)/sbin;
endif
ifneq ($(strip ${ATH_SUPPORT_DFS}),0)
	cp radartool $(INSTALL_ROOT)/sbin;
endif
ifneq ($(strip ${ATH_SUPPORT_SPECTRAL}),0)
	cp spectraltool $(INSTALL_ROOT)/sbin;
endif
	cp athadhoc $(INSTALL_ROOT)/sbin;
ifneq ($(strip ${ATH_SSID_STEERING}),0)
	cp ssidsteering $(INSTALL_ROOT)/sbin;
endif
ifneq ($(strip ${ASSOC_DENIAL_NOTIFY}),0)
	cp assocdenialnotify $(INSTALL_ROOT)/sbin;
endif
ifneq ($(strip ${ATH_SUPPORT_AOW}),0)
	cp mplay $(INSTALL_ROOT)/sbin;
	cp aowstatlog $(INSTALL_ROOT)/sbin;
endif    
ifneq ($(strip ${UMAC_SUPPORT_RPTPLACEMENT}),0)
	cp rpttool $(INSTALL_ROOT)/sbin;
endif
ifeq ($(ATH_SUPPORT_TX99),1)
	cp tx99tool $(INSTALL_ROOT)/sbin/tx99tool_bin;
endif
else
	@echo Athtools done....
endif
ifeq ($(DEBUG_TOOLS), 1)
	cp dumpregs $(INSTALL_ROOT)/sbin;
	cp reg $(INSTALL_ROOT)/sbin;
endif
ifeq ($(UMAC_SUPPORT_WNM), 1)
	cp wnm-app $(INSTALL_ROOT)/sbin;
endif


#- athampdutrc athcwm atrc pktlogconf pktlogdump radartool

dumpregs: hal_diag/dumpregs.o hal_diag/dumpregs_5212.o hal_diag/dumpregs_5416.o hal_diag/dumpregs_9300.o
	${CC} -o dumpregs ${CFLAGS} ${LDFLAGS} hal_diag/dumpregs.o \
		hal_diag/dumpregs_5212.o hal_diag/dumpregs_5416.o hal_diag/dumpregs_9300.o

reg: hal_diag/reg.c
	${CC} -o reg ${CFLAGS} ${LDFLAGS} hal_diag/reg.c

wps_enhc: wps_enhc.c
	${CC} -o $@ ${CFLAGS} -I../ath -lpthread $<

athstats: athstats.c
	${CC} -o athstats ${CFLAGS} -I../ath ${LDFLAGS} athstats.c

athstatsclr: athstatsclr.c
	${CC} -o athstatsclr ${CFLAGS} -I../ath ${LDFLAGS} athstatsclr.c

apstats: apstats.c
	${CC} -o apstats ${CFLAGS} -I../ath ${LDFLAGS} apstats.c

athctrl: athctrl.c
	${CC} -o athctrl ${CFLAGS} ${LDFLAGS} athctrl.c

athampdutrc: athampdutrc.c
	${CC} -o athampdutrc ${CFLAGS} -I../ath ${LDFLAGS} athampdutrc.c

80211stats: 80211stats.c
	${CC} -o 80211stats ${CFLAGS} ${LDFLAGS} 80211stats.c
athkey: athkey.c
	${CC} -o athkey ${CFLAGS} ${LDFLAGS} athkey.c

thermaltool: thermaltool.c
	${CC} -o thermaltool ${CFLAGS} -I$(PERF_PWR_OFFLOAD_DIR_PATH)/wlan/include ${LDFLAGS} thermaltool.c

ifeq ($(ATH_SINGLE_CFG),1)

CFLAGS += -DATH_SINGLE_CFG=1 -DKERNELVER='"$(KERNELVER)"'

ifeq ($(ATH_SINGLE_CFG_DISABLE_SET_DEFAULT),1)
CFLAGS += -DATH_SINGLE_CFG_DISABLE_SET_DEFAULT=1
endif

ifeq ($(KERNELVER),2.6.31)
IWDIR=$(TOPDIR)/apps/wireless_tools.29
else
IWDIR=$(TOPDIR)/apps/wireless_tools.28
endif
cgiMain: cgiMain.c wlancfg.c
	${CC} -o cgiMain ${CFLAGS} ${LDFLAGS} cgiMain.c wlancfg.c $(IWDIR)/iwlib.c -I $(IWDIR) -lm
ifneq ($(ATH_SUPPORT_CFG_8K), 1)
	cp cgiMain $(INSTALL_ROOT)/usr/www/cgi-bin
	ln -s /usr/www/cgi-bin/cgiMain $(INSTALL_ROOT)/bin/cfg
	ln -s /tmp $(INSTALL_ROOT)/var/run
endif
else	
ifeq ($(BUILD_X86), y)
CFLAGS += -DCONFIG_DATA_AT_ROOT=1
endif
cgiMain: cgiMain.c
	$(CC) $(COPTS) -o cgiMain ${CFLAGS} ${LDFLAGS} cgiMain.c
#ifneq ($(ATH_SUPPORT_CFG_8K), 1)
#ifneq ($(BUILD_X86), y)
#	cp cgiMain $(INSTALL_ROOT)/usr/www/cgi-bin
#	ln -s /usr/www/cgi-bin/cgiMain $(INSTALL_ROOT)/bin/cfg
#	ln -s /tmp $(INSTALL_ROOT)/var/run
#endif
#endif
endif

athdebug: athdebug.c
	${CC} -o athdebug ${CFLAGS} ${LDFLAGS} athdebug.c
wifitool: wifitool.c
	${CC} -o wifitool ${CFLAGS} ${LDFLAGS} wifitool.c
wlanconfig: wlanconfig.c
	${CC} -o wlanconfig ${CFLAGS} ${LDFLAGS} wlanconfig.c
wnm-app: tfs-app.c
	${CC} -o wnm-app ${CFLAGS} ${LDFLAGS} tfs-app.c
80211debug: 80211debug.c
	${CC} -o 80211debug ${CFLAGS} ${LDFLAGS} 80211debug.c
athcwm: athcwm.c
	${CC} -o athcwm ${CFLAGS} -I../ath ${LDFLAGS} athcwm.c
atrc: atrc.c
	${CC} -o atrc ${CFLAGS} -I../atrc ${LDFLAGS} atrc.c
pktlogdump: pktlogdump.c
	${CC} -o pktlogdump ${CFLAGS} -I$(COMMON)/ath_pktlog ${LDFLAGS} pktlogdump.c -lm
athadhoc: athadhoc.c
	${CC} -o athadhoc ${CFLAGS} -I$(COMMON)/athadhoc ${LDFLAGS} athadhoc.c -lm	
ssidsteering: ssidsteering.c
	${CC} -o ssidsteering ${CFLAGS} -I$(COMMON)/ssidsteering ${LDFLAGS} ssidsteering.c -lm
assocdenialnotify: assocdenialnotify.c
	${CC} -o assocdenialnotify ${CFLAGS} -I$(COMMON)/assocdenialnotify ${LDFLAGS} assocdenialnotify.c -lm
pktlogconf: pktlogconf.c
	${CC} -o pktlogconf ${CFLAGS} -I$(COMMON)/ath_pktlog ${LDFLAGS} pktlogconf.c
radartool: radartool.c
	${CC} -o radartool ${CFLAGS} -I$(COMMON)/dfs -I../ath ${LDFLAGS} radartool.c
spectraltool: spectraltool.c
	${CC} -o spectraltool ${CFLAGS} -I../ath -I$(COMMON)/dfs -I$(COMMON)/spectral ${LDFLAGS} spectraltool.c
mplay: mplay.c
	${CC} -o mplay ${CFLAGS} -I../ath ${LDFLAGS} mplay.c
aowstatlog: aowstatlog.c
	${CC} -o aowstatlog ${CFLAGS} -I../ath ${LDFLAGS} aowstatlog.c
rpttool: rpttool.c rptsm.c
	${CC} -o rpttool ${CFLAGS} ${LDFLAGS} rpttool.c rptsm.c $(IWDIR)/iwlib.c -I $(IWDIR) -lm
ifeq ($(ATH_SUPPORT_TX99),1)
tx99tool: tx99tool.c
	${CC} -o tx99tool ${CFLAGS} ${LDFLAGS} tx99tool.c
endif
install: ${ALL}
	cp athstats ${DESTDIR}${BINDIR}
	cp 80211stats ${DESTDIR}${BINDIR}
	cp athstatsclr ${DESTDIR}${BINDIR}
	cp apstats ${DESTDIR}${BINDIR}
	cp athampdutrc ${DESTDIR}${BINDIR}
	cp athcwm ${DESTDIR}${BINDIR}
	cp athdebug ${DESTDIR}${BINDIR}
	cp wifitool ${DESTDIR}${BINDIR}
	cp atrc ${DESTDIR}${BINDIR}
	cp pktlogconf ${DESTDIR}${BINDIR}
	cp pktlogdump ${DESTDIR}${BINDIR}
	-rm ${DESTDIR}/${BINDIR}/wlanstats
	ln ${DESTDIR}/${BINDIR}/80211stats ${DESTDIR}/${BINDIR}/wlanstats
	cp 80211debug ${DESTDIR}${BINDIR}
	-rm ${DESTDIR}/${BINDIR}/wlandebug
	ln ${DESTDIR}/${BINDIR}/80211debug ${DESTDIR}/${BINDIR}/wlandebug
	cp wlanconfig ${DESTDIR}${BINDIR}
	cp tfs-app ${DESTDIR}${BINDIR}
	cp radartool ${DESTDIR}${BINDIR}/radartool
	cp spectraltool ${DESTDIR}${BINDIR}/spectraltool
	cp thermaltool ${DESTDIR}${BINDIR}/thermaltool
ifeq ($(ATH_SUPPORT_CFG_8K), 1)
	cp cgiMain ${DESTDIR}${BINDIR}/cgiMain
	cp scripts/network ${DESTDIR}${BINDIR}
	cp scripts/wlan/rc.* ${DESTDIR}${BINDIR}
	cp scripts/ip8k/ap* ${DESTDIR}${BINDIR}
	cp scripts/ip8k/*VAP ${DESTDIR}${BINDIR}
	cp scripts/ip8k/rcS ${DESTDIR}${BINDIR}
	cp scripts/ip8k/rc.network ${DESTDIR}${BINDIR}
	cp scripts/ip8k/rc.bridge ${DESTDIR}${BINDIR}
else
	cp wps_enhc ${DESTDIR}${BINDIR}
endif
ifneq ($(strip ${ATH_SUPPORT_AOW}),0)
	cp mplay ${DESTDIR}${BINDIR}/mplay
	cp aowstatlog ${DESTDIR}${BINDIR}/aowstatlog
endif    
	cp rpttool ${DESTDIR}${BINDIR}/rpttool 
ifeq ($(ATH_SUPPORT_TX99),1)
	cp tx99tool ${DESTDIR}${BINDIR}
endif

ath_tools_clean:
	@rm -f wps_enhc athstats 80211stats athstatsclr apstats pktlogdump pktlogconf radartool spectraltool rpttool tx99tool wifitool cgiMain wlanconfig thermaltool
ifeq ($(DEBUG_TOOLS), 1)
	@rm -f dumpregs reg && rm -f hal_diag/*.o
endif

clean: ath_tools_clean
	rm -f ${ALL} core a.out *.o

spectraltool_clean:
	@rm -f spectraltool
