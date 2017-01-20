#!/bin/sh

SKINS_PATH=/usr/skins

IMAGE_PATH=/www/luci-static/resources/streamboost/images/alienware-logo.png
P0F_PATH=/etc/appflow/p0f.fp


[ -d ${SKINS_PATH}/$1 ] || {
	echo "ERROR: no skin by that name -- \"$1\""
	exit 3
}

apply_skin() {
	cp ${SKINS_PATH}/$1/masthead_logo.png ${IMAGE_PATH}
	cp ${SKINS_PATH}/$1/p0f.fp            ${P0F_PATH}
}


apply_skin $1
