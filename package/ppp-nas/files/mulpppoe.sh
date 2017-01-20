setup_interface_mulpppoe() {
    mknod /dev/ppp c 180 0

    load_modules /etc/modules.d/60-pppoe-mod

    ppp-nas pppoe-session1 start
}

