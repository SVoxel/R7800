include $(NETCGI_CONF)

check_language=$(shell ./check_language.sh)

.PHONY: all install clean
all:
ifeq ($(check_language),success)
	@echo "check language success"
else
	$(error language check fail, please see invalid/)
endif

install:

clean:

