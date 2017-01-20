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

#ifndef _IPQ_DMLITE_H
#define _IPQ_DMLITE_H

#define IPQ_LPM_START		0x28400000
#define IPQ_LPM_SIZE		(1024 * 16)
#define IPQ_LPM_PERIOD_SIZE	(1024 * 8)

/*
Single DML transfer size is 8K bytes,
half of total LPM size
*/
#define DML_TRANSFER_SIZE	0x2000

#define DML_CTL			0x0
#define DML_SRC			0x4
#define DML_DST			0x8
#define DML_TRAN		0xc
#define DML_STATUS		0x10

/*
 * DMLITE CTL CMDS
 */
#define DML_CTL_CMD_FLUSH	(0x0000)
#define DML_CTL_CMD_INITIATE	(1 << 0)
#define DML_CTL_CMD_ZERO	(1 << 1)

#define DML_CTL_SWAP_NOC	(0x0000)
#define DML_CTL_SWAP_HW		(1 << 2)
#define DML_CTL_SWAP_HWB	(2 << 2)
#define DML_CTL_SWAP_BYTE	(3 << 2)
#define DML_CTL_CRYPTO_EN	(1 << 4)


/*
 * DMLITE STATUS
 */
#define DML_STATUS_DONE		(1 << 0)
#define DML_STATUS_ERR		(1 << 1)
#define DML_STATUS_PENDING	(0xffffc)


struct lpass_dml_baseinfo {
	void __iomem *base;
};

extern void ipq_dml_trigger(unsigned long data);

#endif /*_IPQ_DMLITE_H */
