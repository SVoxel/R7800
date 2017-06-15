#!/bin/bash

make V=99 >& report
cd build_dir/target-arm_uClibc-0.9.33.2_eabi
tar xmf ../../asm-openssl-1.0.2l.tar
cd ../../
make V=99 >& report1
