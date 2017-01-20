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
# $Id: //depot/sw/releases/7.0_AP/wlan/common/hal/linux/public/mipsisa32-be-elf.inc#1 $
#

#
# Copyright (c) 2013 Qualcomm Atheros, Inc.
# All Rights Reserved.
# Qualcomm Atheros Confidential and Proprietary.
#

#
# Compilation configuration for building big-endian mipsisa32-elf.
#

#
# Built with GNU cross-devel tools:
#
# PREFIX=/pub/gnu
# BINUTILS=binutils-2.14
# GCC=gcc-3.3.2
# target=mipsisa32-elf
#
# ${BINUTILS}/configure --target=$target --prefix=${PREFIX}
# ${GCC}/configure --target=$target   --prefix=${PREFIX} \
#	--enable-languages=c   --with-gnu-as --with-gnu-ld \
#	--with-newlib   --with-gxx-include-dir=${PREFIX}/$target/include
#
ifndef TOOLPREFIX
TOOLPREFIX=	/pub/gnu/bin/mipsisa64-elf-
endif
#
CC=	${TOOLPREFIX}gcc
LD=	${TOOLPREFIX}ld
STRIP=	${TOOLPREFIX}strip
OBJCOPY=${TOOLPREFIX}objcopy
NM=	${TOOLPREFIX}nm

LDOPTS+=	-EB
COPTS+=	-DAH_BYTE_ORDER=AH_BIG_ENDIAN -D=AH_REGOPS_FUNC -D__LINUX_MIPS64_ARCH__
COPTS+=	-G 0 -mno-abicalls -fno-pic -Wa,--trap \
	-fno-strict-aliasing -fno-common -fomit-frame-pointer -mlong-calls \
	-isystem ${KERNELPATH}/include

