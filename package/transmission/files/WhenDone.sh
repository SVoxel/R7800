#!/bin/sh

echo "$TR_TORRENT_ID*$TR_TORRENT_NAME*$TR_TORRENT_HASH*$TR_TIME_LOCALTIME*$TR_TORRENT_DIR" >> /tmp/admin/.transbt-dlog
/usr/bin/transbt-poptsk.sh BT auto
