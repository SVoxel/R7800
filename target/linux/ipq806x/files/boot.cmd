# Copyright (c) 2013 The Linux Foundation. All rights reserved.
setenv bootargs root=/dev/ram rw console=ttyHSL1,115200n8 ramdisk_size=49152
nand read 0x82000000 0x40000 0x400000
nand read 0x84000000 0xa0000 0xc00000
bootm 0x82000000 0x84000000
