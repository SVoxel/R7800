#!/bin/sh

# Collect basi debug information

iwpriv ath0 dbgLVL 0x00C0180
iwpriv ath1 dbgLVL 0x00C0180

while [ 1 ]
do
	wlanconfig ath0 list
	wlanconfig ath1 list
	athstats
	
	sleep 300
done

