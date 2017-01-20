
CC := $(ATH_CROSS_COMPILE_TYPE)gcc

all:
	$(CC) -g -Wall \
		-I ../../include/ \
		-I ../../../../include/ \
		-I ../../../../../include/ \
		-I ../../../../../../../drivers/wlan_modules/lmac/ath_dev/ \
		-o mkdsetimg mkdsetimg.c lz.c
