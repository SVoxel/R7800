#
# Copyright (c) 2013 Qualcomm Atheros, Inc.
# All Rights Reserved.
# Qualcomm Atheros Confidential and Proprietary.
#

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
# $Id: //depot/sw/qca_main/components/wlan/qca-wifi/1.2/hal/linux/public/i386-elf.inc#2 $
#

#
# Compilation configuration for building i386-elf.
# This assumes the build platform is also i386-elf.
#

#
ifndef TOOLPREFIX
TOOLPREFIX=
endif
#
CC=	${TOOLPREFIX}gcc
LD=	${TOOLPREFIX}ld
STRIP=	${TOOLPREFIX}strip
OBJCOPY=${TOOLPREFIX}objcopy
NM=	${TOOLPREFIX}nm

COPTS+=	-DAH_BYTE_ORDER=AH_LITTLE_ENDIAN
ifndef CONFIG_FRAME_POINTER
COPTS+=	-fomit-frame-pointer
ifeq ($(BUILD_X86),y)
GCC_VERSION=`CC -v 2>&1 | fgrep 'gcc version'| awk -F" " '{ printf $3 }'`
ifeq (,$(findstring "4.3.3", $(GCC_VERSION)))
COPTS+=   -fno-stack-protector
endif
endif
endif
