
ifdef TOPDIR
include $(TOPDIR)/rules.mk
endif

#BINDIR=../bin
BINDIR=/usr/local/bin
ETCDIR=/etc
MANDIR=/usr/share/man

EXTRA_FLAG = -DUSE_ATH_HEADER
# CFLAGS=-g
CFLAGS=-O2 -pipe -march=armv7-a -mcpu=cortex-a9 -mfpu=neon-vfpv4 -mtune=cortex-a9 -mfloat-abi=softfp
CFLAGS += $(EXTRA_FLAGS) -DUSE_ATH_HEADER -DDNI_MULTI_LAN_SUPPORT

default : build.h igmpproxy

all	: igmpproxy

clean	:
	rm -f *.o *.asm build.h igmpproxy  

#install : 
#	cp igmpproxy ${BINDIR}
#	cp ../doc/igmpproxy.8 ${MANDIR}/man8
#	cp ../doc/igmpproxy.conf.5 ${MANDIR}/man5
#	if [ ! -e ${ETCDIR}/igmpproxy.conf ]; then cp igmpproxy.conf ${ETCDIR}; fi

igmpproxy : build.h igmpproxy.o config.o confread.o request.o udpsock.o mcgroup.o rttable.o \
			igmp.o ifvc.o callout.o kern.o syslog.o lib.o mroute-api.o \
			igmpsnooping.o fdbtable.o switch-api.o set_comp.o igmpv3.o bt_igmp.o
	        $(CC) -lconfig -lpcap igmpproxy.o config.o confread.o request.o udpsock.o mcgroup.o rttable.o \
				igmpsnooping.o fdbtable.o switch-api.o set_comp.o igmpv3.o bt_igmp.o \
                igmp.o ifvc.o callout.o kern.o syslog.o lib.o mroute-api.o -o igmpproxy


build.h	:  
	echo '#define BUILD "' `date +%y%m%d` '"' >build.h


