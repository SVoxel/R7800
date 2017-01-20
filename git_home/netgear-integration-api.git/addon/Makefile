#
# addon
#

FW_RULES:=$(wildcard *.rule)

# if firewall rules directory is not '/etc/scripts/firewall/' in your project,
# you must specify FW_RULE_DIR in command line while compile/install this module.
FW_RULE_DIR?=$(INS_DIR_PRE)/etc/scripts/firewall

.PHONY:all install clean uninstall
all:
	@echo "Complete make all in $(CURDIR)"

install:
	$(INSTALL_DIR) $(FW_RULE_DIR)
	$(INSTALL_BIN) $(FW_RULES) $(FW_RULE_DIR)/

clean:
	@echo "Complete make clean in $(CURDIR)"

uninstall:
	-rm -f $(foreach n,$(FW_RULES),$(FW_RULE_DIR)/$(n))
	-rmdir $(FW_RULE_DIR)

