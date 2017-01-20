include $(NETCGI_CONF)

.PHONY: all install clean
all:

install:
	$(INSTALL_DIR) $(INS_BIN_DIR)
	$(INSTALL_BIN) wget_netgear $(INS_BIN_DIR)
ifeq ($(HAVE_USB_STORAGE),1)
	$(INSTALL_BIN) select_partition $(INS_BIN_DIR)
endif

clean:

