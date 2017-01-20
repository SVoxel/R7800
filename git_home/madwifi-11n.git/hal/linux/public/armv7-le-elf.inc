#
# Copyright (c) 2013 Qualcomm Atheros, Inc.
# All Rights Reserved.
# Qualcomm Atheros Confidential and Proprietary.
#
# $Id: //components/rel/wifi.cnss/1.2/hal/linux/public/armv7-le-elf.inc#1 $
#

#
# Compilation configuration for building little-endian ARMv7.
#

#
# Built with GNU cross-devel tools:
#
# PREFIX=/pub/gnu
# BINUTILS=binutils-2.14
# GCC=gcc-3.3.2
# target=arm-elf
#
# ${BINUTILS}/configure --target=$target --prefix=${PREFIX}
# ${GCC}/configure --target=$target   --prefix=${PREFIX} \
#	--enable-languages=c   --with-gnu-as --with-gnu-ld \
#	--with-newlib   --with-gxx-include-dir=${PREFIX}/$target/include
#
ifndef TOOLPREFIX
TOOLPREFIX=	/pub/gnu/bin/arm-elf-
endif
#
CC=	${TOOLPREFIX}gcc
LD=	${TOOLPREFIX}ld
STRIP=	${TOOLPREFIX}strip
OBJCOPY=${TOOLPREFIX}objcopy
NM=	${TOOLPREFIX}nm

LDOPTS=	-EL
COPTS+=	-DAH_BYTE_ORDER=AH_LITTLE_ENDIAN
COPTS+= -fno-omit-frame-pointer -mapcs -mno-sched-prolog
COPTS+= -D__LINUX_ARM_ARCH__=7 -march=armv7-a
COPTS+= -mabi=aapcs-linux -mno-thumb-interwork -msoft-float
COPTS+= -mlittle-endian -fno-strict-aliasing -fno-common -mlong-calls
COPTS+= -Wno-error=unused-but-set-variable
ifneq ($(strip ${BEELINER_HOST_FPGA_FLAG}),0)
COPTS+=-DBEELINER_HOST_FPGA_FLAG=1
else
COPTS+=-DBEELINER_HOST_FPGA_FLAG=0
endif
