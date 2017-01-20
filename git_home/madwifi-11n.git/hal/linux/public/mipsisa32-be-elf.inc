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
# $Id: //depot/sw/qca_main/components/wlan/qca-wifi/1.2/hal/linux/public/mipsisa32-be-elf.inc#1 $
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
TOOLPREFIX=	/pub/gnu/bin/mipsisa32-elf-
endif
#
CC=	${TOOLPREFIX}gcc
LD=	${TOOLPREFIX}ld
STRIP=	${TOOLPREFIX}strip
OBJCOPY=${TOOLPREFIX}objcopy
NM=	${TOOLPREFIX}nm

ifndef USEMARCH
USEMARCH=mips32r2
endif

LDOPTS+=	-EB
COPTS+=	-DAH_BYTE_ORDER=AH_BIG_ENDIAN -D=AH_REGOPS_FUNC -D__LINUX_MIPS32_ARCH__

ifndef Tool_build
COPTS+=-mno-abicalls
endif

COPTS+=	-G 0 -fno-pic -Wa,--trap \
	-fno-strict-aliasing -fno-common -fomit-frame-pointer -mlong-calls \

ifneq ($(USE_LINUX_ARCH_MIPS_INCLUDE_GENERATED),y)
COPTS+= -isystem ${KERNELPATH}/include
endif

ifeq ($(CONFIG_MIPS_74K_KERNEL_OPTIMIZATION), 1)
COPTS += -march=74kc -mtune=74kc -funit-at-a-time -pipe
else
COPTS += -march=${USEMARCH}
endif

ifeq ($(BUILD_WLAN_PBO_DATA),y)
GCOV_PROFILE := y
COPTS += -fprofile-arcs -ftest-coverage
endif

ifeq ($(USE_WLAN_PBO_DATA),y)
COPTS += -fprofile-use -fprofile-correction
endif
