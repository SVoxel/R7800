#
# script
#

INS_DIR:=$(INS_DIR_PRE)/etc/scripts/ntgr_sw_api
INS_BIN_DIR:=$(INS_DIR_PRE)/usr/sbin

.PHONY:all install clean uninstall
all:
	@echo "Complete make all in $(CURDIR)"

install:
	$(INSTALL_DIR) $(INS_DIR)
	$(INSTALL_BIN) ntgr_sw_api.sh $(INS_DIR)/
	$(INSTALL_BIN) app_register.sh $(INS_DIR)/
	$(INSTALL_DIR) $(INS_BIN_DIR)
	$(INSTALL_BIN) ntgr_sw_api_event_notify $(INS_BIN_DIR)/

clean:
	@echo "Complete make clean in $(CURDIR)"

uninstall:
	-rm -f $(INS_DIR)/ntgr_sw_api.sh
	-rm -f $(INS_DIR)/app_register.sh
	-rmdir $(INS_DIR)
	-rm -f $(INS_BIN_DIR)/ntgr_sw_api_event_notify
