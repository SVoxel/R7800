#!/bin/bash

export TZ=UTC

# make tools/install
# make -j4 toolchain/install
# make -j4 target/compile
PATH=/home/voxel/local/bin:/usr/local/bin:/usr/bin:/bin:/usr/local/games:/usr/games
make V=99 >& report
cd build_dir/target-arm_uClibc-0.9.33.2_eabi
tar xmf ../../asm-openssl-1.0.2n.tar
cd ../../
make V=99 >& report1
