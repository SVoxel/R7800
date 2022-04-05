#!/bin/bash

export TZ=UTC
PATH=/home/voxel/local/bin:/home/voxel/go/bin:/usr/local/bin:/usr/bin:/bin:/usr/local/games:/usr/games
/usr/bin/make tools/install V=s >& reptools
make -j4 toolchain/install V=s >& reptoolchain
make V=99 >& report
