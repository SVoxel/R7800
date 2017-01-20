OBJS=update_smb usbled
CFLAGS+= -Wall
USB_CONFIG_ADD := 1
ifeq ($(USB_CONFIG_ADD), 1)
CFLAGS += -DUSB_CONFIG_ADD
endif
GCC=$(CROSS_COMPILE)gcc

all:	$(OBJS)

usbled: usbled.c
	$(GCC) $(CFLAGS) -o $@ $< $(EXTRA_LDFLAGS) $(LDFLAGS)

update_smb: update_smb.c
	$(GCC) $(CFLAGS) -o $@ $< $(EXTRA_LDFLAGS) $(LDFLAGS) -lconfig

clean:
	rm -f $(OBJS)
