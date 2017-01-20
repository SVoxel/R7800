/*
 * ar80xx.h: ar80xx PHY driver
 *
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef __AR80XX_H__
#define __AR80XX_H__

#define AR80XX_PHY_ID_MASK		0xffffffff
#define AR80XX_PHY_ID_AR8033		0x004dd074
#define ETH_SGMII_ADDRESS_OFFSET	0x48

#define AR80XX_REG_CHIP_CONFIG		0x1f
#define AR80XX_BT_BX_REG_SEL		0x8000	/* bit 15, select copper page register */
#define AR80XX_AUTO_NEGO		0x1000	/* bit 12, enable auto negotiation */

#define ETH_SGMII_GIGE_MSB		24
#define ETH_SGMII_GIGE_LSB		24
#define ETH_SGMII_GIGE_MASK		0x01000000
#define ETH_SGMII_GIGE_GET(x)		(((x) & ETH_SGMII_GIGE_MASK) >> ETH_SGMII_GIGE_LSB)
#define ETH_SGMII_GIGE_SET(x)		(((x) << ETH_SGMII_GIGE_LSB) & ETH_SGMII_GIGE_MASK)

#define ETH_SGMII_CLK_SEL_MSB		25
#define ETH_SGMII_CLK_SEL_LSB		25
#define ETH_SGMII_CLK_SEL_MASK		0x02000000
#define ETH_SGMII_CLK_SEL_GET(x)	(((x) & ETH_SGMII_CLK_SEL_MASK) >> ETH_SGMII_CLK_SEL_LSB)
#define ETH_SGMII_CLK_SEL_SET(x)	(((x) << ETH_SGMII_CLK_SEL_LSB) & ETH_SGMII_CLK_SEL_MASK)

#define ETH_SGMII_PHASE0_COUNT_MSB	7
#define ETH_SGMII_PHASE0_COUNT_LSB	0
#define ETH_SGMII_PHASE0_COUNT_MASK	0x000000ff
#define ETH_SGMII_PHASE0_COUNT_GET(x)	(((x) & ETH_SGMII_PHASE0_COUNT_MASK) >> ETH_SGMII_PHASE0_COUNT_LSB)
#define ETH_SGMII_PHASE0_COUNT_SET(x)	(((x) << ETH_SGMII_PHASE0_COUNT_LSB) & ETH_SGMII_PHASE0_COUNT_MASK)

#define ETH_SGMII_PHASE1_COUNT_MSB	15
#define ETH_SGMII_PHASE1_COUNT_LSB	8
#define ETH_SGMII_PHASE1_COUNT_MASK	0x0000ff00
#define ETH_SGMII_PHASE1_COUNT_GET(x)	(((x) & ETH_SGMII_PHASE1_COUNT_MASK) >> ETH_SGMII_PHASE1_COUNT_LSB)
#define ETH_SGMII_PHASE1_COUNT_SET(x)	(((x) << ETH_SGMII_PHASE1_COUNT_LSB) & ETH_SGMII_PHASE1_COUNT_MASK)
#endif
