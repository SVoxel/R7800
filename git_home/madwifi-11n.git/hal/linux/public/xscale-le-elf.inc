#
# Copyright (c) 2010, Atheros Communications Inc.
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
# OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
#
# $Id: //depot/sw/qca_main/components/wlan/qca-wifi/1.2/hal/linux/public/xscale-le-elf.inc#1 $
#

#
# Compilation configuration for building little-endian XScale/arm-elf.
#
# NB: built with AH_REGOPS_FUNC to so that register accesses
#     can be done using the Linux readl/writel functions on
#     systems that need it.
#
# Reported to work on:
#	Compulab's ARMBASE using ARMCORE GX
#		(http://www.compulab.co.il/armbase.htm)
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
COPTS+=	-DAH_BYTE_ORDER=AH_LITTLE_ENDIAN -DAH_REGOPS_FUNC
COPTS+= -march=armv4 -mlittle-endian -fno-strict-aliasing -fno-common \
	-mapcs-32 -mtune=xscale  -mshort-load-bytes -msoft-float -mfp=2
