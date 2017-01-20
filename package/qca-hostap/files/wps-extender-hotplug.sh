#
# Copyright (c) 2015, The Linux Foundation. All rights reserved.
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
# OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
#

if [ "$ACTION" = "pressed" -a "$BUTTON" = "wps" ]; then
	[ -r /var/run/wifi-wps-enhc-extn.conf ] || exit 0
	echo "" > /dev/console
	echo "WPS PUSH BUTTON EVENT DETECTED" > /dev/console

	if [ -r /var/run/wifi-wps-enhc-extn.pid ]; then
		kill "$(cat "/var/run/wifi-wps-enhc-extn.pid")"
		sleep 1
	fi

	if [ -r /var/run/wifi-wps-enhc-extn.pid ]; then
		rm -f /var/run/wifi-wps-enhc-extn.pid
		exit 0
	fi

	echo "START APP TO HANDLE WPS PUSH BUTTON EVENT" > /dev/console
	/usr/sbin/wps_enhc -b /var/run/wifi-wps-enhc-extn.pid -n 2 \
				-d 128 -l /var/run/wifi-wps-enhc-extn.log
fi

