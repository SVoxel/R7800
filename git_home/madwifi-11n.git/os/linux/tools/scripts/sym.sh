#!/bin/bash
# Script to generate back trance from panic.log and kext file.

#
# Copyright (c) 2013 Qualcomm Atheros, Inc.
# All Rights Reserved.
# Qualcomm Atheros Confidential and Proprietary.
#

function usage()
{
echo "usage: sym.sh [-i] [-p <relative path of AtherosAirport.kext>] <panic log file>"
echo "       if kext path  is not given it uses the current directory" 
echo "       -i prints the instruction causing the panic"
exit 1;
}

KEXT_PATH=/System/Library/Extensions/IO80211Family.kext/Contents/PlugIns/
PANIC_LOG="0"
PANIC_ADDR=0
INSTR=0
args=($*)

i=0
while [ $i -lt $# ]
do
case ${args[$i]} in
	-i) if [ $i -eq $# ]
            then
               usage;
            fi;
            INSTR=1;;  
        -p) let i=$i+1;
	    if [ $i -eq $# ]
            then
               usage;
            fi;
            KEXT_PATH=${args[$i]};;  
        -*) usage;;
         *) PANIC_LOG=${args[$i]};;  
esac
let i=$i+1;
done

if [ $PANIC_LOG = "0" ]
then
usage;
fi

echo 
echo "Back Trace "
#get the base address where the kext is loaded.
BASE_ADDR=$(cat $PANIC_LOG  | awk  ' { FS="@" } /AtherosAirPort/ { print $2 }' |  awk -F'-' '{print $1}')
#get all the function addresses.
FUNC_ADDR=$(cat $PANIC_LOG  | awk  '/0x[0-9,a-f]* : 0x[0-9,a-f]*/ { print $0 }' | awk -F':' '{ print $2 }' |  awk -F' ' '{ print $1 }')

for i in $FUNC_ADDR
do

#get relative function addresses also subtract 0x1000 .
REL_ADDR=$(echo "$i $BASE_ADDR" | awk '{ printf "0x%x ", $1 - $2 }') 
REL_ADDR=$(echo "$REL_ADDR 0x1000" | awk '{ printf "0x%x ", $1 - $2 }') 
REL_ADDR_DEC=$(echo "$i $BASE_ADDR" | awk '{ printf "%d ", $1 - $2 - 0x1000 }') 

if [ $REL_ADDR_DEC -gt 0 ]
then
#save the relative function address first fuction when the panic occured.
if [ $PANIC_ADDR = "0" ]
then
PANIC_ADDR=$REL_ADDR;
fi
fi
atos -o $KEXT_PATH/AtherosAirPort.kext/Contents/MacOS/AtherosAirPort $REL_ADDR

done

if [ $INSTR -eq 1 ]
then
#remove 0x prefix from the address
PANIC_ADDR=$(echo "$PANIC_ADDR" | awk '{ printf "%x ", $1}') 
echo
echo "PANIC at the following instruction "
# grep for the address in the disassembled file.
otool -t -V $KEXT_PATH/AtherosAirPort.kext/Contents/MacOS/AtherosAirPort  | grep $PANIC_ADDR 
fi
