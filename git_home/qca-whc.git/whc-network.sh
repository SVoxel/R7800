#!/bin/sh /etc/rc.common
# Copyright (c) 2015 Qualcomm Atheros, Inc.
#
# All Rights Reserved.
# Qualcomm Atheros Confidential and Proprietary.

whc_network_restart() {
    # This already grabs a lock when reconfiguring the Wi-Fi interfaces.
    # If WHC ever grows to configure more than just Wi-Fi, then we may
    # need to grab an additional lock.
    /sbin/wifi
}
