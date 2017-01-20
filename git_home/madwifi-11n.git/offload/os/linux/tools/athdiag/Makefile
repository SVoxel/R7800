
CC := $(TOOLPREFIX)gcc
TARGET_TYPE ?= AR900B
TARGET_VERS ?= v1

ifeq ($(TARGET_TYPE), AR900B)
CFLAGS += -DAR900B -DWLAN_HEADERS
endif

all:
	$(CC) -g3 -Wall  \
		-I . \
		-I ../../include/ \
		-I ../../../../include/ \
		-I ../../../../../include/ \
		-I ../../../../../../../drivers/wlan_modules/lmac/ath_dev/ \
		-I ../../../../../include/$(TARGET_TYPE)/$(TARGET_VERS)/ \
		-I ../../../../include/$(TARGET_TYPE)/extra/ \
		-I ../../../../include/$(TARGET_TYPE)/hw/ \
		-I ../../../../include/$(TARGET_TYPE)/extra/hw/ \
		-I ../../../../tools/systemtools/src2/include/ \
        $(INCS) $(CFLAGS)\
		athdiag.c Qc98xxEepromStructGet.c Qc98xxDevice.c Qc98xxEepromStruct.c -o athdiag

clean:
	rm -f athdiag
