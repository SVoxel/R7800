# $Id: //depot/sw/qca_main/components/wlan/qca-wifi/1.2/os/linux/ath_hal/Makefile#1 $

#
# Makefile for the Atheros HAL Module.
#
# NB: we can't support MODVERSIONS because it requires building
#     from source which may not be available
#
ifeq ($(obj),)
obj := .
endif

DEPTH := ../../..
ADF := ${obj}/${DEPTH}/adf
ASF := ${obj}/${DEPTH}/asf

obj-m		+= ath_hal.o
ath_hal-objs	:= ah_adf.o hal.o
ifeq ($(findstring 2.6,$(KERNELRELEASE)),)
export-objs	:= ah_adf.o
list-multi	:= ath_hal.o
endif

# If building into kernel

ifneq ($(MAKING_MODULES),1)
obj-$(CONFIG_ATHEROS_HAL) := ah_adf.o hal.o
ifeq ($(findstring 2.6,$(KERNELRELEASE)),)
O_TARGET	:= ath_hal.o
endif
endif

ifeq ($(strip ${QCA_PARTNER_MAKE_F_SUPPORT}),1)
export QCA_PARTNER_MAKE_F_INC=1
endif
include $(obj)/${DEPTH}/os/linux/Makefile.inc

UUDECODE?=uudecode

ifeq ($(ATH_SUPPORT_HTC),1)
INCS=	-I${HAL} -I${HAL}/${OS} -I${obj}/${ATH_HAL} -I${obj}/${DEPTH} -I${COMMON} -I${ATH_RATE} -I${ATH}
else
INCS+=	-I${HAL} -I${HAL}/${OS} -I${obj}/${ATH_HAL} -I${obj}/${DEPTH} -I${COMMON} -I${ATH_RATE} -I${ATH}
endif
# -I${WLAN} 
INCS+=  -I${obj}/$(DEPTH)/include
INCS+=  -I$(ADF)/include/os -I$(ADF)/include/nbuf -I$(ADF)/include/net
INCS+=  -I$(ADF)/os/linux -I$(ADF)/nbuf/linux -I$(ADF)/net/linux
INCS+=  -I$(ASF)/include

EXTRA_CFLAGS+= ${INCS} ${COPTS} -DBUILD_HAL

-include $(WLAN_TOP)/Rules.make

ifeq ($(strip ${QCA_PARTNER_MAKE_F_SUPPORT}),1)
MOD_CFLAGS = -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(ath_hal.mod)"  -D"KBUILD_MODNAME=KBUILD_STR(ath_hal)" 
all: ath_hal.ko
else
all:	ah_adf.c hal.o opt_ah.h


ifneq ($(BUILD_X86),y)
	$(MAKE) -C $(KERNELPATH) SUBDIRS=$(shell pwd) modules
else
	$(MAKE) -C $(KERNELPATH) KBUILD_EXTRA_SYMBOLS=$(PWD)/../Module.symvers SUBDIRS=$(shell pwd) modules
endif
endif

ah_adf.c: opt_ah.h
#	cp -f $(obj)/${DEPTH}/os/${OS}/ah_adf.c ah_osdep.c

$(obj)/hal.o:${HAL}/${OS}/obj/${TARGET}/hal.o
	cp ${HAL}/${OS}/obj/${TARGET}/hal.o .
#${UUDECODE} ${HAL}/public/${TARGET}.hal.o.uu

opt_ah.h: ${HAL}/${OS}/public/${TARGET}.opt_ah.h
	cp -f ${HAL}/${OS}/public/${TARGET}.opt_ah.h opt_ah.h

# Copy the unstripped version of the driver to /tftpboot/`whoami`/drivers/
# if debug is enabled.

install: all
ifeq ($(CONFIG_DEBUG_INFO),1)
	test -d ${TFTPPATH}/drivers || mkdir -p ${TFTPPATH}/drivers
	cp ath_hal.ko ${TFTPPATH}/drivers/
endif
	test -d ${DESTDIR}/${MODULEPATH} || mkdir -p ${DESTDIR}/${MODULEPATH}
	$(STRIP) -S ath_hal.ko
	cp ath_hal.ko ${DESTDIR}/${MODULEPATH}

clean:
	rm -f *~ *.o *.ko *.mod.c
	rm -f .depend .version .*.o.flags .*.o.d .*.o.cmd .*.ko.cmd
# remove the files we copied to this dir
	rm -f opt_ah.h hal.o
	rm -rf .tmp_versions Module.symvers modules.order

ifeq ($(strip ${QCA_PARTNER_MAKE_F_SUPPORT}),1)
ah_adf.o: ah_adf.c
	${CC} -c -o $@ ${EXTRA_CFLAGS} $<
ath_hal.mod.o: ath_hal.mod.c
	${CC} -c -o $@ ${EXTRA_CFLAGS} ${MOD_CFLAGS} $<
ath_hal.o: ${ath_hal-objs}
	$(LD) -m elf32btsmip -r -o ath_hal.o $(ath_hal-objs)
	$(KERNELPATH)/scripts/mod/modpost ath_hal.o
ath_hal.ko: ath_hal.o ath_hal.mod.o
	$(LD) $(LDOPTS) -o ath_hal.ko ath_hal.o ath_hal.mod.o
else

ath_hal.o: ${ath_hal-objs}
	$(LD) $(LDOPTS) -o ath_hal.$(ko) -r $(ath_hal-objs)

endif
