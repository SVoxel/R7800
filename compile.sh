#!/bin/bash

export TZ=UTC

# make tools/install
# make -j4 toolchain/install
# make -j4 target/compile
PATH=/home/voxel/local/bin:/usr/local/bin:/usr/bin:/bin:/usr/local/games:/usr/games
make V=99 >& report
