#!/bin/sh

append CONFIGS "dniconfig"

. /etc/dni-wifi-config

config_get_dniconfig()
{
    dni_config_get $@
    eval export -- "${1}"
}

config_set_dniconfig()
{
    dni_config_set $@
}

config_commit_dniconfig()
{
    dni_config_commit
}