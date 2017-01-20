
CC := $(ATH_CROSS_COMPILE_TYPE)gcc

all:
	$(CC) -Wall  \
		-I ../../include/ \
		-I ../../../../include/ \
		-I ../../../../../include/ \
		-I ../../../../../../../drivers/wlan_modules/lmac/ath_dev/ \
		bmiloader.c -o bmiloader
