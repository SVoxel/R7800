# Parameters to build standard AR6002 ROM hardware image
# after cd'ing to $WORKAREA/target/rom/support/AR6002:
	--desc=../dsets.rom.hw.txt
	--out=data.rom.hw.bin
	--mapout=dsets.rom.hw.map
	--end=0x4f3ffc
	--pad=4

# Parameters to build standard AR6002 ROM FPGA image
# after cd'ing to $WORKAREA/target/rom/support/AR6002:
	--desc=../dsets.rom.fpga.txt
	--out=data.rom.fpga.bin
	--mapout=dsets.rom.fpga.map
	--end=0x4f3ffc
	--pad=4

# Parameters to build AR6002 RAM image for use on hardware
	--desc=dsets.ram.txt
	--out=data.ram.bin
	--mapin=$WORKAREA/target/rom/support/AR6002/dsets.rom.hw.map
	--mapout=dsets.ram.map
	--end=0x52dffc

# Parameters to build AR6002 RAM image for use on FPGA
	--desc=dsets.ram.txt
	--out=data.ram.bin
	--mapin=$WORKAREA/target/rom/support/AR6002/dsets.rom.fpga.map
	--mapout=dsets.ram.map
	--end=0x52dffc

# Parameters to build AR9888v2 ROM hardware image
# after cd'ing to $WORKAREA/perf_pwr_offload/drivers/target/support/AR9888/v2:
	--desc=../dsets.rom.hw.txt
	--out=data.rom.hw.bin
	--mapout=dsets.rom.hw.map
	--end=0x97b000

# Parameters to build AR9888v2 FGPA hardware image
# after cd'ing to $WORKAREA/perf_pwr_offload/drivers/target/support/AR9888/v2:
	--desc=../dsets.rom.fpga.txt
	--out=data.rom.fpga.bin
	--mapout=dsets.rom.fpga.map
	--end=0x97b000
