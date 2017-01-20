# 
#  Netgear SW Integration API
#

export INSTALL_DIR?=install -d -m0755
export INSTALL_BIN?=install -m0755
export INSTALL_CONF?=install -m0644
export INSTALL_LINK?=ln -sf

export INS_DIR_PRE?=/tmp

define redirect
$(1):
	$(MAKE) -C src $(1)
	$(MAKE) -C script $(1)
	$(MAKE) -C addon $(1)
endef

.PHONY:all install clean uninstall
$(eval $(call redirect,all))
$(eval $(call redirect,install))
$(eval $(call redirect,clean))
$(eval $(call redirect,uninstall))
