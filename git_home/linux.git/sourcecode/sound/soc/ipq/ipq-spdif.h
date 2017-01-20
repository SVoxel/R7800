
/* Copyright (c) 2013 The Linux Foundation. All rights reserved.
 *
 * Copyright (c) 2010-2011, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef _IPQ_SPDIF_H
#define _IPQ_SPDIF_H

#define LPA_IF_SPDIF_TX_VER				0x0
#define STEP(x)						((x & 0xffff))
#define SPDIF_MINOR(x)					((x & 0xfff0000) >> 16)
#define SPDIF_MAJOR(x)					((x & 0xf0000000) >> 28)

#define LPA_IF_SPDIF_TX_CFG				0x4
#define LPA_IF_SPDIF_FR_SZ				(0xC0)
#define LPA_IF_SPDIF_DR_LB				(1 << 16)
#define LPA_IF_SPDIF_EN_INH_WTTS			(1 << 17)

#define LPA_IF_SPDIF_TXP_SEL				0x8
#define LPA_IF_SPDIF_TXP_PARITY_ERR			(1 << 7)
#define LPA_If_SPDIF_TXP_PARITY_EN			(0 << 6)
#define LPA_IF_SPDIF_TXP_PREAMB_POL			(1 << 5)
#define LPA_IF_SPDIF_TXP_PEN				(1 << 0)
#define LPA_IF_SPDIF_TXP_USER_DATA_EN			(3 << 3)
#define LPA_IF_SPDIF_TXP_CH_STAT_EN			(3 << 1)

#define LPA_IF_SPDIF_TX_CMD				0xc
#define LPA_IF_SPDIF_TX_CMD_PAUSE			(1 << 0)
#define LPA_IF_SPDIF_TX_CMD_CLRFIFO			(1 << 1)
#define LPA_IF_SPDIF_TX_CMD_ABRT			(1 << 2)

#define LPA_IF_SPDIF_TX_PORT_CFG			0x10
#define LPA_IF_SPDIF_TX_PORT_CFG_BIT_MASK		0xF
#define LPA_IF_SPDIF_TX_PORT_CFG_L16			(0x0)
#define LPA_IF_SPDIF_TX_PORT_CFG_L17			(1 << 0)
#define LPA_IF_SPDIF_TX_PORT_CFG_L18			(2 << 0)
#define LPA_IF_SPDIF_TX_PORT_CFG_L19			(3 << 0)
#define LPA_IF_SPDIF_TX_PORT_CFG_L20			(4 << 0)
#define LPA_IF_SPDIF_TX_PORT_CFG_L21			(5 << 0)
#define LPA_IF_SPDIF_TX_PORT_CFG_L22			(6 << 0)
#define LPA_IF_SPDIF_TX_PORT_CFG_L23			(7 << 0)
#define LPA_IF_SPDIF_TX_PORT_CFG_L24			(8 << 0)
#define LPA_IF_SPDIF_TX_PORT_CFG_MONO			(1 << 4)
#define LPA_IF_SPDIF_TX_PORT_CFG_REPEAT_MUTE		(1 << 6)
#define LPA_IF_SPDIF_TX_PORT_CFG_SOFT_MUTE		(1 << 8)
#define LPA_IF_SPDIF_TX_CFG_SOFT_MUTE_AFTER_EOF		(1 << 9)
#define LPA_IF_SPDIF_TX_PORT_CFG_MONO_SEL		(0 << 10)

#define LPA_IF_SPDIF_FIFO_CNTL				0x14
#define LPA_IF_SPDIF_FIFO_WD_BYTE_SWAP			(1 << 1)
#define LPA_IF_SPDIF_FIFO_DWD_WD_SWAP			(1 << 2)
#define LPA_IF_SPDIF_FIFO_PRE_BUF_EN			(1 << 7)
#define LPA_IF_SPDIF_FIFO_JUMP_RD_PTR			(1 << 8)
#define LPA_IF_SPDIF_FIFO_LJUSTIFIED			(1 << 9)

#define LPA_IF_SPDIF_TX_SUBBUF_FIFO			0x18
#define LPA_IF_SPDIF_TX_SUBBUF_FIFO_WTTS		(1 << 12)
#define LPA_IF_SPDIF_TX_SUBBUF_FIFO_FR_SIZ_ADJUST(x)	(x & 0xfff)

#define LPA_IF_SPDIF_TIME_STAMP_FIFO_1			0x1C

#define LPA_IF_SPDIF_TIME_STAMP_FIFO_2			0x20
#define LPA_IF_SPDIF_TIME_STAMP_UPPER_24(x)		(x & 0xfffff)

#define LPA_IF_SPDIF_SUBBUF_FIFO_STA			0x24
#define LPA_IF_SPDIF_SUBBUF_FIFO_FILL_LEVEL_READ(x)	(x & 0xf)
#define LPA_IF_SPDIF_SUBBUF_FIFO_FILL_LEVEL_RELEASE(x)	(x & 0xf0 >> 4)

#define LPA_IF_SPDIF_TS_DIFF_1				0x28

#define LPA_IF_SPDIF_TS_DIFF_2				0x2C
#define LPA_IF_SPDIF_TS_DIFF_UPPER_24(x)		(x & 0xfffff)

#define LPA_IF_SPDIF_TS_DIFF_ACC_1			0x30
#define LPA_IF_SPDIF_TS_DIFF_ACC_RESET_ALL_56(x)	(x << 0)
#define LPA_IF_SPDIF_TS_DIFF_ACC_LOW_32(x)		(x)

#define LPA_IF_SPDIF_TS_DIFF_ACC_2			0x34
#define LPA_IF_SPDIF_TS_DIFF_ACC_UPPER_24(x)		(x & 0xffffff)

#define LPA_IF_SPDIF_LB_SHAPER_CFG			0x38
#define LPA_IF_SPDIF_TSIZE_ONE_DWORD			(0 << 30)
#define LPA_IF_SPDIF_TSIZE_FOUR_DWORDS			(2 << 30)
#define LPA_IF_SPDIF_TSIZE_EIGHT_DWORDS			(3 << 30)

#define LPA_IF_SPDIF_BUF_PNTR				0x3C

#define LPA_IF_SPDIF_FIFO_TST_CNTL			0x40
#define LPA_IF_SPDIF_FIFO_URUN_LOST_SAMPLES(x)		(x & 0xffff)

#define LPA_IF_SPDIF_STATISTICS				0x44
#define LPA_IF_SPDIF_STAT_BLOCK_NUM(x)			(x & 0xffff0000 >> 16)
#define LPA_IF_SPDIF_STAT_FRAME_NUM(x)			(x & 0xff)

#define LPA_IF_SPDIF_TX_INT_CNTL			0x48
#define LPA_IF_SPDIF_TX_INT_MASK			(0 << 7)
#define LPA_IF_SPDIF_TX_MC_ERR				(1 << 6)
#define LPA_IF_SPDIF_TX_TS_DIFF_INT			(1 << 5)
#define LPA_IF_SPDIF_TX_DROPLB_INT			(1 << 4)
#define LPA_IF_SPDIF_TX_PAUSE_INT			(1 << 3)
#define LPA_IF_SPDIF_TX_IDLE_INT			(1 << 2)
#define LPA_IF_SPDIF_TX_FIFO_UNDERFLOW_INT		(1 << 1)
#define LPA_IF_SPDIF_TX_BLKDONE_INT			(1 << 0)

#define LPA_IF_SPDIF_TX_INT_STAT			0x4c
#define LPA_IF_SPDIF_TX_BLKDONE_STA			(1 << 0)
#define LPA_IF_SPDIF_TX_BLKDONE_ACK			(1 << 0)
#define LPA_IF_SPDIF_TX_FIFO_UNDERFLOW_STA		(1 << 1)
#define LPA_IF_SPDIF_TX_FIFO_UNDEFLOW_ACK		(1 << 1)
#define LPA_IF_SPDIF_TX_IDLE_STA			(1 << 2)
#define LPA_IF_SPDIF_TX_IDLE_ACK			(1 << 2)
#define LPA_IF_SPDIF_TX_PAUSE_STA			(1 << 3)
#define LPA_IF_SPDIF_TX_PAUSE_ACK			(1 << 3)
#define LPA_IF_SPDIF_TX_DROP_LATEBLK_STA		(1 << 4)
#define LPA_IF_SPDIF_TX_DROP_LATEBLK_ACK		(1 << 4)
#define LPA_IF_SPDIF_TX_TS_DIFF_STA			(1 << 5)
#define LPA_IF_SPDIF_TX_TS_DIFF_ACK			(1 << 5)
#define LPA_IF_SPDIF_TX_MC_ERR_STA			(1 << 6)
#define LPA_IF_SPDIF_TX_MC_ERR_ACK			(1 << 6)

#define LPA_IF_SPDIF_TX_CH_STAT				0x50
#define LPA_IF_SPDIF_TX_CH_STAT_F_MASK			0x3c000
#define LPA_IF_SPDIF_TX_CH_STAT_F_22_05KHZ		(1 << 14)
#define LPA_IF_SPDIF_TX_CH_STAT_F_44_1KHZ		(2 << 14)
#define LPA_IF_SPDIF_TX_CH_STAT_F_88_2KHZ		(3 << 14)
#define LPA_IF_SPDIF_TX_CH_STAT_F_176_4KHZ		(4 << 14)
#define LPA_IF_SPDIF_TX_CH_STAT_F_24KHZ			(5 << 14)
#define LPA_IF_SPDIF_TX_CH_STAT_F_192KHZ		(6 << 14)
#define LPA_IF_SPDIF_TX_CH_STAT_F_32KHZ			(7 << 14)
#define LPA_IF_SPDIF_TX_CH_STAT_CAT_CODE		(0xff << 6)
#define LPA_IF_SPDIF_TX_CH_STAT_CNTL			(0x1c)
#define LPA_IF_SPDIF_TX_CH_STAT_CLK_ACC_L2		(0 << 18)
#define LPA_IF_SPDIF_TX_CH_STAT_CLK_ACC_L1		(1 << 18)
#define LPA_IF_SPDIF_TX_CH_STAT_CLK_ACC_L3		(2 << 18)
#define LPA_IF_SPDIF_TX_CH_STAT_CLK_ACC_IRF_NOT_MATCHED	(3 << 18)

#define LPA_IF_SPDIF_TX_CHA_STAT_BUF0			0x54
#define LPA_IF_SPDIF_TX_CHA_STAT_BUF1			0x58
#define LPA_IF_SPDIF_TX_CHA_STAT_BUF2			0x5C
#define LPA_IF_SPDIF_TX_CHA_STAT_BUF3			0x60
#define LPA_IF_SPDIF_TX_CHA_STAT_BUF4			0x64
#define LPA_IF_SPDIF_TX_CHA_STAT_BUF5			0x68

#define LPA_IF_SPDIF_TX_CHB_STAT_BUF0			0x6C
#define LPA_IF_SPDIF_TX_CHB_STAT_BUF1			0x70
#define LPA_IF_SPDIF_TX_CHB_STAT_BUF2			0x74
#define LPA_IF_SPDIF_TX_CHB_STAT_BUF3			0x78
#define LPA_IF_SPDIF_TX_CHB_STAT_BUF4			0x7C
#define LPA_IF_SPDIF_TX_CHB_STAT_BUF5			0x80

#define LPA_IF_SPDIF_TX_USERA_DATA_BUF0			0x84
#define LPA_IF_SPDIF_TX_USERA_DATA_BUF1			0x88
#define LPA_IF_SPDIF_TX_USERA_DATA_BUF2			0x8C
#define LPA_IF_SPDIF_TX_USERA_DATA_BUF3			0x90
#define LPA_IF_SPDIF_TX_USERA_DATA_BUF4			0x94
#define LPA_IF_SPDIF_TX_USERA_DATA_BUF5			0x98

#define LPA_IF_SPDIF_TX_USERB_DATA_BUF0			0x9C
#define LPA_IF_SPDIF_TX_USERB_DATA_BUF1			0xA0
#define LPA_IF_SPDIF_TX_USERB_DATA_BUF2			0xA4
#define LPA_IF_SPDIF_TX_USERB_DATA_BUF3			0xA8
#define LPA_IF_SPDIF_TX_USERB_DATA_BUF4			0xAC
#define LPA_IF_SPDIF_TX_USERB_DATA_BUF5			0xB0

#define LPA_IF_SPDIF_TX_BURST_CNTL			0xb4
#define LPA_IF_SPDIF_TX_BURST_CNTL_MASK			0x1FFFFFF
#define LPA_IF_SPDIF_TX_REP_PERIOD_DATA_BURST(x)	((x & 0x7fff) << 7)
#define LPA_IF_SPDIF_TX_REF_POINT(x)			((x & 0x3) << 5)
#define LPA_IF_SPDIF_TX_REP_PERIOD_PAUSE(x)		((x & 0x7) << 22)
#define LPA_IF_SPDIF_TX_DATA_TYPE_AC3			0x1
#define LPA_IF_SPDIF_TX_DATA_TYPE_MPEG_1_L1		0x4
#define LPA_IF_SPDIF_TX_DATA_TYPE_MPEG_1_L2		0x5
#define LPA_IF_SPDIF_TX_DATA_TYPE_MPEG_2_w_ext		0x6
#define LPA_IF_SPDIF_TX_DATA_TYPE_MPEG_2_AAC		0x7
#define LPA_IF_SPDIF_TX_DATA_TYPE_MPEG_2_L1		0x8
#define LPA_IF_SPDIF_TX_DATA_TYPE_MPEG_2_L2		0x9
#define LPA_IF_SPDIF_TX_DATA_TYPE_MPEG_2_L3		0xa
#define LPA_IF_SPDIF_TX_DATA_TYPE_DTS_T1		0xB
#define LPA_IF_SPDIF_TX_DATA_TYPE_DTS_T2		0xC
#define LPA_IF_SPDIF_TX_DATA_TYPE_DTS_T3		0xD
#define LPA_IF_SPDIF_TX_DATA_TYPE_ATRAC			0xe
#define LPA_IF_SPDIF_TX_DATA_TYPE_ATRAC2_3		0xf
#define LPA_IF_SPDIF_TX_DATA_TYPE_LINEAR		0x0

#define LPA_IF_SPDIF_TX_BURST_MISC			0xb8

#define LPA_IF_SPDIF_TX_DEBUG_PKT_TRIG			0xbc
#define LPA_IF_SPDIF_TX_PKT_CNT(x)			(x & 0xff)
#define LPA_IF_SPDIF_TX_PKT_CNT_TRIG_EN(x)		(x << 31)

#define LPA_IF_SPDIF_TX_DEBUG_DATA_PAT_TRIG		0xc0

#define LPA_IF_SPDIF_TX_DEBUG_CRC_CNTL			0xD4
#define LPA_IF_SPDIF_TX_TRIG_READY(x)			((x & 0x4) >> 2)
#define LPA_IF_SPDIF_TX_CRC_VAL_READY(x)		((x & 0x8) >> 3)

#define LPA_IF_SPDIF_TX_DEBUG_CRC_RAW			0xc8

#define LPA_IF_SPDIF_TX_DEBUG_CRC_ASM			0xcc

#define LPA_IF_SPDIF_TX_DEBUG_CRC_COUNT			0xD0

#define LPA_IF_SPDIF_TX_DEBUG_CNTL			0xD4

#define LPA_IF_SPDIF_TX_POWER_MGMT_CNTL			0xD8
#define LPA_IF_SPDIF_TX_HYST_SEL_N_4_CLOCKS		(0 << 6)
#define LPA_IF_SPDIF_TX_HYST_SEL_N_8_CLOCKS		(1 << 6)
#define LPA_IF_SPDIF_TX_HYST_SEL_N_16_CLOCKS		(2 << 6)
#define LPA_IF_SPDIF_TX_HYST_SEL_N_32_CLOCKS		(3 << 6)

#define LPA_IF_SPDIF_INTR				121
#define IPQ_LPAIF_SPDIF_PHYS				0x28080000
#define IPQ_LPAIF_SPDIF_END				0x28081FFC

/*
 * For Linear PCM, the number of frames per transmit shoud
 * be 192.
 * For Non Linear PCM (Compressed) the number of frames per
 * transmit is dependent on the encoding used. So our
 * buffer configurations here should be by default in multiples
 * of 192 to keep ALSA happy. The period min and period max
 * configuration shoudl be such a way that number of periods
 * min and max should be multiple of 192.
 */

#define SPDIF_FRAMESIZE		192 /* never change this */
#define SPDIF_DWORD_SZ		4
#define SPDIF_FIFO_DEPTH	8
#define SPDIF_BUFFERS		64
#define SPDIF_PERIODS_MIN	8
#define SPDIF_PERIODS_MAX	SPDIF_BUFFERS
#define SPDIF_BUF_MAX_BYTES	\
	(SPDIF_FRAMESIZE * SPDIF_DWORD_SZ * SPDIF_BUFFERS)


#define SPDIF_FIFO_CTL		0x000000B8
#define SPDIF_TX_CH_ST		0x00003FDC
#define SPDIF_TX_DBG_CRC_CNT	0x00000101
#define SPDIF_TX_INTR		0x0000007F
#define SPDIF_TXP_SEL		0x0000000A
#define SPDIF_INTR_MASK		0x00000083
#define SPDIF_TX_BURST_CTL_LINEAR	0x00C30000

#define LPA_IF_SPDIF_TX_CHA_STAT_BUF0_VAL		0x02110006
#define LPA_IF_SPDIF_TX_CHA_STAT_BUF1_VAL		0x2
#define LPA_IF_SPDIF_TX_CHA_STAT_BUF2_VAL		0x9CA54EA6
#define LPA_IF_SPDIF_TX_CHA_STAT_BUF3_VAL		0x9F325B41
#define LPA_IF_SPDIF_TX_CHA_STAT_BUF4_VAL		0x22B783B5
#define LPA_IF_SPDIF_TX_CHA_STAT_BUF5_VAL		0xF1017648

#define LPA_IF_SPDIF_TX_CHB_STAT_BUF0_VAL		0x02210006
#define LPA_IF_SPDIF_TX_CHB_STAT_BUF1_VAL		0x00000002
#define LPA_IF_SPDIF_TX_CHB_STAT_BUF2_VAL		0x75D5E829
#define LPA_IF_SPDIF_TX_CHB_STAT_BUF3_VAL		0x50A320B2
#define LPA_IF_SPDIF_TX_CHB_STAT_BUF4_VAL		0xC3C5F68B
#define LPA_IF_SPDIF_TX_CHB_STAT_BUF5_VAL		0x11DB7E92

struct ipq_lpaif_spdif_baseinfo {
	void __iomem *base;
};

struct ipq_spdif_buf_info {
	dma_addr_t curr_buf;
	dma_addr_t start_buf;
	size_t size_buf;
};

enum ipq_spdif_freq {
	F_22_05_KHZ = 22050,
	F_44_1_KHZ = 44100,
	F_48_KHZ = 48000,
	F_88_2_KHZ = 88200,
	F_96_KHZ = 96000,
	F_176_4_KHZ = 176400,
	F_24_KHZ = 24000,
	F_192_KHZ = 192000,
	F_32_KHZ = 32000,
};

extern uint32_t ipq_spdif_cfg_bit_width(uint32_t bitwidth);
extern uint32_t ipq_spdif_cfg_freq(uint32_t freq);
extern void ipq_cfg_spdif_rate(uint32_t rate);
extern void ipq_spdif_onetime_cfg(void);
extern dma_addr_t ipq_spdif_set_params(dma_addr_t frame_buf_ptr,
				uint32_t spdif_frame_size);
extern void ipq_cfg_spdif_hwparams(int bit_width);
extern int ipq_spdif_cfg_compr_mode(uint32_t compr_mode,
					uint32_t compr_frame_size);

#endif /* _IPQ_SPDIF_H */
