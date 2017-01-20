#
# Copyright (c) 2002-2005 Sam Leffler, Errno Consulting
# Copyright (c) 2002-2005 Atheros Communications, Inc.
# All rights reserved.
#
# $Id: //depot/sw/qca_main/components/wlan/qca-wifi/1.2/hal/linux/Makefile#1 $
#
DEPTH=	..

#
# Makefile for building the Atheros/MADWIFI HAL from source code with gmake
#
HAL=	${DEPTH}
PUBLIC=	public

#
ifdef TARGET
ALL=	${TARGET}
else
ALL=	i386-elf \
	x86_64-elf \
	arm-elf \
	mipsisa32-be-elf mips3-elf mips2-elf mips1-elf mipsisa64-be-elf \
	powerpc-eabi powerpc-elf \
	sh4-elf \
	ubicom32_elf
endif
#

all:	${ALL}

release:
	for i in ${ALL}; do \
		${MAKE} $$i-release; \
	done

symrelease:
	for i in ${ALL}; do \
		${MAKE} $$i-symrelease; \
	done

clean:
	for i in ${ALL}; do \
		${MAKE} $$i-clean; \
	done

#
# XXX someone with some gmake foo should be able to do this right...
#
i386-elf: ${PUBLIC}/i386-elf.inc
	${MAKE} -f Makefile.inc TARGET=i386-elf
i386-elf-release:
	${MAKE} -f Makefile.inc TARGET=i386-elf release
i386-elf-symrelease:
	${MAKE} -f Makefile.inc TARGET=i386-elf symrelease
i386-elf-clean:
	${MAKE} -f Makefile.inc TARGET=i386-elf clean

x86_64-elf: ${PUBLIC}/x86_64-elf.inc
	${MAKE} -f Makefile.inc TARGET=x86_64-elf
x86_64-elf-release:
	${MAKE} -f Makefile.inc TARGET=x86_64-elf release
x86_64-elf-clean:
	${MAKE} -f Makefile.inc TARGET=x86_64-elf clean

arm-elf: armv4-be-elf armv4-le-elf armv7-le-elf arm9-le-thumb-elf xscale-elf
arm-elf-release: armv4-be-elf-release armv4-le-elf-release armv7-le-elf-release \
	arm9-le-thumb-elf-release xscale-elf-release
arm-elf-clean: armv4-be-elf-clean armv4-le-elf-clean armv7-le-elf-clean\
	arm9-le-thumb-elf-clean xscale-elf-clean

armv4-be-elf: ${PUBLIC}/armv4-be-elf.inc
	${MAKE} -f Makefile.inc TARGET=armv4-be-elf
armv4-be-elf-release:
	${MAKE} -f Makefile.inc TARGET=armv4-be-elf release
armv4-be-elf-clean:
	${MAKE} -f Makefile.inc TARGET=armv4-be-elf clean
armv4-le-elf: ${PUBLIC}/armv4-le-elf.inc
	${MAKE} -f Makefile.inc TARGET=armv4-le-elf
armv4-le-elf-release:
	${MAKE} -f Makefile.inc TARGET=armv4-le-elf release
armv4-le-elf-clean:
	${MAKE} -f Makefile.inc TARGET=armv4-le-elf clean
armv7-le-elf: ${PUBLIC}/armv7-le-elf.inc
	${MAKE} -f Makefile.inc TARGET=armv7-le-elf
armv7-le-elf-release:
	${MAKE} -f Makefile.inc TARGET=armv7-le-elf release
armv7-le-elf-clean:
	${MAKE} -f Makefile.inc TARGET=armv7-le-elf clean
arm9-le-thumb-elf: ${PUBLIC}/arm9-le-thumb-elf.inc
	${MAKE} -f Makefile.inc TARGET=arm9-le-thumb-elf
arm9-le-thumb-elf-release:
	${MAKE} -f Makefile.inc TARGET=arm9-le-thumb-elf release
arm9-le-thumb-elf-clean:
	${MAKE} -f Makefile.inc TARGET=arm9-le-thumb-elf clean
armv7-le-elf: ${PUBLIC}/armv7-le-elf.inc
	${MAKE} -f Makefile.inc TARGET=armv7-le-elf
armv7-le-elf-release:
	${MAKE} -f Makefile.inc TARGET=armv7-le-elf release
armv7-le-elf-clean:
	${MAKE} -f Makefile.inc TARGET=armv7-le-elf clean

xscale-elf: xscale-be-elf xscale-le-elf
xscale-elf-release: xscale-be-elf-release xscale-le-elf-release
xscale-elf-clean: xscale-be-elf-clean xscale-le-elf-clean

xscale-be-elf: ${PUBLIC}/xscale-be-elf.inc
	${MAKE} -f Makefile.inc TARGET=xscale-be-elf
xscale-be-elf-release:
	${MAKE} -f Makefile.inc TARGET=xscale-be-elf release
xscale-be-elf-clean:
	${MAKE} -f Makefile.inc TARGET=xscale-be-elf clean
xscale-le-elf: ${PUBLIC}/xscale-le-elf.inc
	${MAKE} -f Makefile.inc TARGET=xscale-le-elf
xscale-le-elf-release:
	${MAKE} -f Makefile.inc TARGET=xscale-le-elf release
xscale-le-elf-clean:
	${MAKE} -f Makefile.inc TARGET=xscale-le-elf clean

mipsisa32-be-elf: mips3-be-elf
mipsisa32-be-elf-release: mips3-be-elf-release
mipsisa32-be-elf-symrelease: mips3-be-elf-symrelease
mipsisa32-be-elf-clean: mips3-be-elf-clean

mipsisa64-be-elf:
	${MAKE} -f Makefile.inc TARGET=mipsisa64-be-elf
mipsisa64-be-elf-release:
	${MAKE} -f Makefile.inc TARGET=mipsisa64-be-elf release
mipsisa64-be-elf-symrelease:
	${MAKE} -f Makefile.inc TARGET=mipsisa64-be-elf symrelease
mipsisa64-be-elf-clean:
	${MAKE} -f Makefile.inc TARGET=mipsisa64-be-elf clean

mips3-elf: mips3-be-elf mips3-le-elf
mips3-elf-release: mips3-be-elf-release mips3-le-elf-release
mips3-elf-clean: mips3-be-elf-clean mips3-le-elf-clean

# NB: the .inc file naming is historical and needs changing
mips3-be-elf: ${PUBLIC}/mipsisa32-be-elf.inc
	${MAKE} -f Makefile.inc TARGET=mipsisa32-be-elf
mips3-be-elf-release:
	${MAKE} -f Makefile.inc TARGET=mipsisa32-be-elf release
mips3-be-elf-symrelease:
	${MAKE} -f Makefile.inc TARGET=mipsisa32-be-elf symrelease
mips3-be-elf-clean:
	${MAKE} -f Makefile.inc TARGET=mipsisa32-be-elf clean

mips3-le-elf: ${PUBLIC}/mipsisa32-le-elf.inc
	${MAKE} -f Makefile.inc TARGET=mipsisa32-le-elf
mips3-le-elf-release:
	${MAKE} -f Makefile.inc TARGET=mipsisa32-le-elf release
mips3-le-elf-clean:
	${MAKE} -f Makefile.inc TARGET=mipsisa32-le-elf clean

# NB: the .inc file naming is historical and needs changing
mips2-elf: mips2-be-elf mips2-le-elf
mips2-elf-release: mips2-be-elf-release mips2-le-elf-release
mips2-elf-clean: mips2-be-elf-clean mips2-le-elf-clean

mips2-be-elf: ${PUBLIC}/mips-be-elf.inc
	${MAKE} -f Makefile.inc TARGET=mips-be-elf
mips2-be-elf-release:
	${MAKE} -f Makefile.inc TARGET=mips-be-elf release
mips2-be-elf-clean:
	${MAKE} -f Makefile.inc TARGET=mips-be-elf clean

mips2_fp_le-elf: ${PUBLIC}/mips2_fp_le-elf.inc
	${MAKE} -f Makefile.inc TARGET=mips2_fp_le-elf
mips2_fp_le-elf-release:
	${MAKE} -f Makefile.inc TARGET=mips2_fp_le-elf release
mips2_fp_le-elf-clean:
	${MAKE} -f Makefile.inc TARGET=mips2_fp_le-elf clean

mips2-le-elf: ${PUBLIC}/mips-le-elf.inc
	${MAKE} -f Makefile.inc TARGET=mips-le-elf
mips2-le-elf-release:
	${MAKE} -f Makefile.inc TARGET=mips-le-elf release
mips2-le-elf-clean:
	${MAKE} -f Makefile.inc TARGET=mips-le-elf clean

mips1-elf: mips1-be-elf mips1-le-elf
mips1-elf-release: mips1-be-elf-release mips1-le-elf-release
mips1-elf-clean: mips1-be-elf-clean mips1-le-elf-clean

mips1-be-elf: ${PUBLIC}/mips1-be-elf.inc
	${MAKE} -f Makefile.inc TARGET=mips1-be-elf
mips1-be-elf-release:
	${MAKE} -f Makefile.inc TARGET=mips1-be-elf release
mips1-be-elf-clean:
	${MAKE} -f Makefile.inc TARGET=mips1-be-elf clean

mips1-le-elf: ${PUBLIC}/mips1-le-elf.inc
	${MAKE} -f Makefile.inc TARGET=mips1-le-elf
mips1-le-elf-release:
	${MAKE} -f Makefile.inc TARGET=mips1-le-elf release
mips1-le-elf-clean:
	${MAKE} -f Makefile.inc TARGET=mips1-le-elf clean

powerpc-eabi: powerpc-be-eabi powerpc-le-eabi
powerpc-eabi-release: powerpc-be-eabi-release powerpc-le-eabi-release
powerpc-eabi-clean: powerpc-be-eabi-clean powerpc-le-eabi-clean

powerpc-be-eabi: ${PUBLIC}/powerpc-be-eabi.inc
	${MAKE} -f Makefile.inc TARGET=powerpc-be-eabi
powerpc-be-eabi-release:
	${MAKE} -f Makefile.inc TARGET=powerpc-be-eabi release
powerpc-be-eabi-clean:
	${MAKE} -f Makefile.inc TARGET=powerpc-be-eabi clean
powerpc-le-eabi: ${PUBLIC}/powerpc-le-eabi.inc
	${MAKE} -f Makefile.inc TARGET=powerpc-le-eabi
powerpc-le-eabi-release:
	${MAKE} -f Makefile.inc TARGET=powerpc-le-eabi release
powerpc-le-eabi-clean:
	${MAKE} -f Makefile.inc TARGET=powerpc-le-eabi clean

powerpc-elf: powerpc-be-elf
powerpc-elf-release: powerpc-be-elf-release
powerpc-elf-clean: powerpc-be-elf-clean

powerpc-be-elf: ${PUBLIC}/powerpc-be-elf.inc
	${MAKE} -f Makefile.inc TARGET=powerpc-be-elf
powerpc-be-elf-release:
	${MAKE} -f Makefile.inc TARGET=powerpc-be-elf release
powerpc-be-elf-clean:
	${MAKE} -f Makefile.inc TARGET=powerpc-be-elf clean

sh4-elf: sh4-le-elf
sh4-elf-release: sh4-le-elf-release
sh4-elf-clean: sh4-le-elf-clean

sh4-le-elf: ${PUBLIC}/sh4-le-elf.inc
	${MAKE} -f Makefile.inc TARGET=sh4-le-elf
sh4-le-elf-release:
	${MAKE} -f Makefile.inc TARGET=sh4-le-elf release
sh4-le-elf-clean:
	${MAKE} -f Makefile.inc TARGET=sh4-le-elf clean

ubicom32-elf: ${PUBLIC}/ubicom32-elf.inc
	${MAKE} -f Makefile.inc TARGET=ubicom32-elf
ubicom32-elf-release:
	${MAKE} -f Makefile.inc TARGET=ubicom32-elf release
ubicom32-elf-clean:
	${MAKE} -f Makefile.inc TARGET=ubicom32-elf clean
