
#define AX_MONITOR_MODE			0x01
#define AX_MONITOR_LINK			0x02
#define AX_MONITOR_MAGIC		0x04
#define AX_MONITOR_HSFS			0x10

/* AX88172 Medium Status Register values */
#define AX_MEDIUM_FULL_DUPLEX		0x02
#define AX_MEDIUM_TX_ABORT_ALLOW	0x04
#define AX_MEDIUM_FLOW_CONTROL_EN	0x10
#define AX_MCAST_FILTER_SIZE		8
#define AX_MAX_MCAST			64

#define AX_EEPROM_LEN			0x40

#define AX_SWRESET_CLEAR			0x00
#define AX_SWRESET_RR			0x01
#define AX_SWRESET_RT			0x02
#define AX_SWRESET_PRTE			0x04
#define AX_SWRESET_PRL			0x08
#define AX_SWRESET_BZ			0x10
#define AX_SWRESET_IPRL			0x20
#define AX_SWRESET_IPPD			0x40

#define AX88772_IPG0_DEFAULT		0x15
#define AX88772_IPG1_DEFAULT		0x0c
#define AX88772_IPG2_DEFAULT		0x12

#define AX88772A_IPG0_DEFAULT		0x15
#define AX88772A_IPG1_DEFAULT		0x16
#define AX88772A_IPG2_DEFAULT		0x1A

#define AX88772_MEDIUM_FULL_DUPLEX	0x0002
#define AX88772_MEDIUM_RESERVED		0x0004
#define AX88772_MEDIUM_RX_FC_ENABLE	0x0010
#define AX88772_MEDIUM_TX_FC_ENABLE	0x0020
#define AX88772_MEDIUM_PAUSE_FORMAT	0x0080
#define AX88772_MEDIUM_RX_ENABLE	0x0100
#define AX88772_MEDIUM_100MB		0x0200
#define AX88772_MEDIUM_DEFAULT	\
	(AX88772_MEDIUM_FULL_DUPLEX | AX88772_MEDIUM_RX_FC_ENABLE | \
	 AX88772_MEDIUM_TX_FC_ENABLE | AX88772_MEDIUM_100MB | \
	 AX88772_MEDIUM_RESERVED | AX88772_MEDIUM_RX_ENABLE )

#define AX_CMD_SET_SW_MII		0x06
#define AX_CMD_READ_MII_REG		0x07
#define AX_CMD_WRITE_MII_REG		0x08
#define AX_CMD_SET_HW_MII		0x0a
#define AX_CMD_READ_EEPROM		0x0b
#define AX_CMD_WRITE_EEPROM		0x0c
#define AX_CMD_WRITE_EEPROM_EN		0x0d
#define AX_CMD_WRITE_EEPROM_DIS		0x0e
#define AX_CMD_WRITE_RX_CTL		0x10
#define AX_CMD_READ_IPG012		0x11
#define AX_CMD_WRITE_IPG0		0x12
#define AX_CMD_WRITE_IPG1		0x13
#define AX_CMD_WRITE_IPG2		0x14
#define AX_CMD_WRITE_MULTI_FILTER	0x16
#define AX_CMD_READ_NODE_ID		0x17
#define AX_CMD_READ_PHY_ID		0x19
#define AX_CMD_WRITE_MEDIUM_MODE	0x1b
#define AX_CMD_READ_MONITOR_MODE	0x1c
#define AX_CMD_WRITE_MONITOR_MODE	0x1d
#define AX_CMD_WRITE_GPIOS		0x1f
#define AX_CMD_SW_RESET 			0x20
#define AX_CMD_SW_PHY_STATUS		0x21
#define AX_CMD_SW_PHY_SELECT		0x22
#define AX88772_CMD_READ_NODE_ID	0x13

#define REG_LENGTH		2

#define PHY_ID_MASK	0x1f

#define AX_RX_CTL_MFB			0x0300		/* Maximum Frame size 16384bytes */
#define AX_RX_CTL_START			0x0080		/* Ethernet MAC start */
#define AX_RX_CTL_AP			0x0020		/* Accept physcial address from Multicast array */
#define AX_RX_CTL_AM			0x0010	
#define AX_RX_CTL_AB			0x0008		/* Accetp Brocadcast frames*/
#define AX_RX_CTL_SEP			0x0004		/* Save error packets */	
#define AX_RX_CTL_AMALL			0x0002		/* Accetp all multicast frames */
#define AX_RX_CTL_PRO			0x0001		/* Promiscuous Mode */
#define AX_RX_CTL_STOP			0x0000		/* Stop MAC */

#define AX_MONITOR_MODE 			0x01
#define AX_MONITOR_LINK 			0x02
#define AX_MONITOR_MAGIC			0x04
#define AX_MONITOR_HSFS 			0x10

#define AX_MCAST_FILTER_SIZE		8
#define AX_MAX_MCAST			64
#define AX_INTERRUPT_BUFSIZE		8

#define AX_EEPROM_LEN			0x40
#define AX_EEPROM_MAGIC 			0xdeadbeef
#define EEPROMMASK				0x7f

#define AX_SWRESET_CLEAR			0x00
#define AX_SWRESET_RR			0x01
#define AX_SWRESET_RT			0x02
#define AX_SWRESET_PRTE 			0x04
#define AX_SWRESET_PRL			0x08
#define AX_SWRESET_BZ			0x10
#define AX_SWRESET_IPRL 			0x20
#define AX_SWRESET_IPPD 			0x40

#define AX88772_IPG0_DEFAULT		0x15
#define AX88772_IPG1_DEFAULT		0x0c
#define AX88772_IPG2_DEFAULT		0x12

/* GPIO REGISTER */
#define AXGPIOS_GPO0EN			0X01
#define AXGPIOS_GPO0			0X02
#define AXGPIOS_GPO1EN			0X04
#define AXGPIOS_GPO1			0X08
#define AXGPIOS_GPO2EN			0X10
#define AXGPIOS_GPO2			0X20
#define AXGPIOS_RSE			0X80

// GMII register definitions
#define GMII_PHY_CONTROL			0x00	// control reg
#define GMII_PHY_STATUS			0x01	// status reg
#define GMII_PHY_OUI			0x02	// most of the OUI bits
#define GMII_PHY_MODEL			0x03	// model/rev bits, and rest of OUI
#define GMII_PHY_ANAR			0x04	// AN advertisement reg
#define GMII_PHY_ANLPAR			0x05	// AN Link Partner
#define GMII_PHY_ANER			0x06	// AN expansion reg
#define GMII_PHY_1000BT_CONTROL		0x09	// control reg for 1000BT
#define GMII_PHY_1000BT_STATUS		0x0A	// status reg for 1000BT

// Bit definitions: GMII Control
#define GMII_CONTROL_RESET		0x8000	// reset bit in control reg
#define GMII_CONTROL_LOOPBACK		0x4000	// loopback bit in control reg
#define GMII_CONTROL_10MB		0x0000	// 10 Mbit
#define GMII_CONTROL_100MB		0x2000	// 100Mbit
#define GMII_CONTROL_1000MB		0x0040	// 1000Mbit
#define GMII_CONTROL_SPEED_BITS		0x2040	// speed bit mask
#define GMII_CONTROL_ENABLE_AUTO	0x1000	// autonegotiate enable
#define GMII_CONTROL_POWER_DOWN		0x0800
#define GMII_CONTROL_ISOLATE		0x0400	// islolate bit
#define GMII_CONTROL_START_AUTO		0x0200	// restart autonegotiate
#define GMII_CONTROL_FULL_DUPLEX	0x0100

// Bit definitions: GMII Status
#define GMII_STATUS_100MB_MASK		0xE000	// any of these indicate 100 Mbit
#define GMII_STATUS_10MB_MASK		0x1800	// either of these indicate 10 Mbit
#define GMII_STATUS_AUTO_DONE		0x0020	// auto negotiation complete
#define GMII_STATUS_AUTO			0x0008	// auto negotiation is available
#define GMII_STATUS_LINK_UP		0x0004	// link status bit
#define GMII_STATUS_EXTENDED		0x0001	// extended regs exist
#define GMII_STATUS_100T4		0x8000	// capable of 100BT4
#define GMII_STATUS_100TXFD		0x4000	// capable of 100BTX full duplex
#define GMII_STATUS_100TX		0x2000	// capable of 100BTX
#define GMII_STATUS_10TFD		0x1000	// capable of 10BT full duplex
#define GMII_STATUS_10T			0x0800	// capable of 10BT

// Bit definitions: Auto-Negotiation Advertisement
#define GMII_ANAR_ASYM_PAUSE		0x0800	// support asymetric pause
#define GMII_ANAR_PAUSE			0x0400	// support pause packets
#define GMII_ANAR_100T4			0x0200	// support 100BT4
#define GMII_ANAR_100TXFD		0x0100	// support 100BTX full duplex
#define GMII_ANAR_100TX			0x0080	// support 100BTX half duplex
#define GMII_ANAR_10TFD			0x0040	// support 10BT full duplex
#define GMII_ANAR_10T			0x0020	// support 10BT half duplex
#define GMII_SELECTOR_FIELD		0x001F	// selector field.

// Bit definitions: Auto-Negotiation Link Partner Ability
#define GMII_ANLPAR_100T4		0x0200	// support 100BT4
#define GMII_ANLPAR_100TXFD		0x0100	// support 100BTX full duplex
#define GMII_ANLPAR_100TX		0x0080	// support 100BTX half duplex
#define GMII_ANLPAR_10TFD		0x0040	// support 10BT full duplex
#define GMII_ANLPAR_10T			0x0020	// support 10BT half duplex
#define GMII_ANLPAR_PAUSE		0x0400	// support pause packets
#define GMII_ANLPAR_ASYM_PAUSE		0x0800	// support asymetric pause
#define GMII_ANLPAR_ACK			0x4000	// means LCB was successfully rx'd
#define GMII_SELECTOR_8023		0x0001;

// Bit definitions: 1000BaseT AUX Control
#define GMII_1000_AUX_CTRL_MASTER_SLAVE	0x1000
#define GMII_1000_AUX_CTRL_FD_CAPABLE		0x0200	// full duplex capable
#define GMII_1000_AUX_CTRL_HD_CAPABLE		0x0100	// half duplex capable

// Bit definitions: 1000BaseT AUX Status
#define GMII_1000_AUX_STATUS_FD_CAPABLE	0x0800	// full duplex capable
#define GMII_1000_AUX_STATUS_HD_CAPABLE 	0x0400	// half duplex capable

// Cicada MII Registers
#define GMII_AUX_CTRL_STATUS			0x1C
#define GMII_AUX_ANEG_CPLT			0x8000
#define GMII_AUX_FDX				0x0020
#define GMII_AUX_SPEED_1000			0x0010
#define GMII_AUX_SPEED_100			0x0008

// medium mode register
#define MEDIUM_GIGA_MODE				0x0001
#define MEDIUM_FULL_DUPLEX_MODE			0x0002
#define MEDIUM_TX_ABORT_MODE			0x0004
#define MEDIUM_ENABLE_125MHZ			0x0008
#define MEDIUM_ENABLE_RX_FLOWCTRL		0x0010
#define MEDIUM_ENABLE_TX_FLOWCTRL		0x0020
#define MEDIUM_ENABLE_JUMBO_FRAME		0x0040
#define MEDIUM_CHECK_PAUSE_FRAME_MODE		0x0080
#define MEDIUM_ENABLE_RECEIVE			0x0100
#define MEDIUM_MII_100M_MODE			0x0200
#define MEDIUM_ENABLE_JAM_PATTERN		0x0400
#define MEDIUM_ENABLE_STOP_BACKPRESSURE	0x0800
#define MEDIUM_ENABLE_SUPPER_MAC_SUPPORT	0x1000

/* PHY mode */
#define PHY_MODE_MARVELL		0
#define PHY_MODE_CICADA_FAMILY		1
#define PHY_MODE_CICADA_V1		1
#define PHY_MODE_AGERE_FAMILY		2
#define PHY_MODE_AGERE_V0		2
#define PHY_MODE_CICADA_V2		5
#define PHY_MODE_AGERE_V0_GMII		6
#define PHY_MODE_CICADA_V2_ASIX		9
#define PHY_MODE_REALTEK_8211CL		12
#define PHY_MODE_REALTEK_8211BN		13
#define PHY_MODE_REALTEK_8251CL		14
#define PHY_MODE_ATTANSIC_V0		0x40
#define PHY_MODE_ATTANSIC_FAMILY	0x40

/*  */
#define LED_MODE_MARVELL			0
#define LED_MODE_CAMEO			1

#define MARVELL_LED_CTRL			0x18
#define MARVELL_MANUAL_LED		0x19

#define PHY_IDENTIFIER			0x0002
#define PHY_AGERE_IDENTIFIER		0x0282
#define PHY_CICADA_IDENTIFIER		0x000f
#define PHY_MARVELL_IDENTIFIER		0x0141

#define PHY_MARVELL_STATUS		0x001b
#define MARVELL_STATUS_HWCFG		0x0004		/* SGMII without clock */

#define PHY_MARVELL_CTRL			0x0014
#define MARVELL_CTRL_RXDELAY		0x0080
#define MARVELL_CTRL_TXDELAY		0x0002

#define PHY_CICADA_EXTPAGE		0x001f
#define CICADA_EXTPAGE_EN		0x0001
#define CICADA_EXTPAGE_DIS		0x0000


struct {unsigned short value, offset; } CICADA_FAMILY_HWINIT[] =
{
	{0x0001, 0x001f}, {0x1c25, 0x0017}, {0x2a30, 0x001f}, {0x234c, 0x0010},
	{0x2a30, 0x001f}, {0x0212, 0x0008}, {0x52b5, 0x001f}, {0xa7fa, 0x0000},
	{0x0012, 0x0002}, {0x3002, 0x0001}, {0x87fa, 0x0000}, {0x52b5, 0x001f},
	{0xafac, 0x0000}, {0x000d, 0x0002}, {0x001c, 0x0001}, {0x8fac, 0x0000},
	{0x2a30, 0x001f}, {0x0012, 0x0008}, {0x2a30, 0x001f}, {0x0400, 0x0014},
	{0x2a30, 0x001f}, {0x0212, 0x0008}, {0x52b5, 0x001f}, {0xa760, 0x0000},
	{0x0000, 0x0002}, {0xfaff, 0x0001}, {0x8760, 0x0000}, {0x52b5, 0x001f},
	{0xa760, 0x0000}, {0x0000, 0x0002}, {0xfaff, 0x0001}, {0x8760, 0x0000},
	{0x52b5, 0x001f}, {0xafae, 0x0000}, {0x0004, 0x0002}, {0x0671, 0x0001},
	{0x8fae, 0x0000}, {0x2a30, 0x001f}, {0x0012, 0x0008}, {0x0000, 0x001f},
};

struct {unsigned short value, offset; } CICADA_V2_HWINIT[] =
{
	{0x2a30, 0x001f}, {0x0212, 0x0008}, {0x52b5, 0x001f}, {0x000f, 0x0002},
	{0x472a, 0x0001}, {0x8fa4, 0x0000}, {0x2a30, 0x001f}, {0x0212, 0x0008},
	{0x0000, 0x001f},
};

struct {unsigned short value, offset; } CICADA_V2_ASIX_HWINIT[] =
{
	{0x2a30, 0x001f}, {0x0212, 0x0008}, {0x52b5, 0x001f}, {0x0012, 0x0002},
	{0x3002, 0x0001}, {0x87fa, 0x0000}, {0x52b5, 0x001f}, {0x000f, 0x0002},
	{0x472a, 0x0001}, {0x8fa4, 0x0000}, {0x2a30, 0x001f}, {0x0212, 0x0008},
	{0x0000, 0x001f},
};

struct {unsigned short value, offset; } AGERE_FAMILY_HWINIT[] =
{
	{0x0800, 0x0000}, {0x0007, 0x0012}, {0x8805, 0x0010}, {0xb03e, 0x0011},
	{0x8808, 0x0010}, {0xe110, 0x0011}, {0x8806, 0x0010}, {0xb03e, 0x0011},
	{0x8807, 0x0010}, {0xff00, 0x0011}, {0x880e, 0x0010}, {0xb4d3, 0x0011},
	{0x880f, 0x0010}, {0xb4d3, 0x0011}, {0x8810, 0x0010}, {0xb4d3, 0x0011},
	{0x8817, 0x0010}, {0x1c00, 0x0011}, {0x300d, 0x0010}, {0x0001, 0x0011},
	{0x0002, 0x0012},
};

struct ax88178_data {
	u16	EepromData;
	int	UseGpio0;
	u16	MediaLink;
	int	UseRgmii;
	u8	PhyMode;
	u8	LedMode;
	int	nx_state;						//add by philip
	u8	buf[6];
	u16	PhyID;
	u16	buf16_1[1];
	int	i;

	int	ret;
	int	fullduplex;
	u16	phylinkstatus1;
	u16	phylinkstatus2;
	u16	tempshort;
	u16	phyctrl;
	u16	phyreg;
	u16	phyanar;
	u16	phyauxctrl;
	u8	link;
	u8	init_phy;
};
/*
typedef enum _PHY_PROCESS {
	AX_NOP = 0,
	AX_CHK_CABLE,
	AX_PHY_POWER_PROCESS,
	AX_SET_RX_CFG,
} PHY_PROCESS;
*/
enum watchdog_state {
	AX_NOP = 0,
	CHK_LINK,			/* Routine A */
	CHK_CABLE_EXIST,		/* Called by A */
	CHK_CABLE_EXIST_AGAIN,		/* Routine B */
	PHY_POWER_UP,			/* Called by B */
	PHY_POWER_UP_BH,
	PHY_POWER_DOWN,
	CHK_CABLE_STATUS,		/* Routine C */
	WAIT_AUTONEG_COMPLETE,
	AX_SET_RX_CFG,
};

struct ax88772a_data {
	struct usbnet *dev;
	struct workqueue_struct *ax_work;
	struct work_struct check_link;
	unsigned long autoneg_start;
	u8 Event;
	u8 TickToExpire;
	u8 DlyIndex;
	u8 DlySel;
};

/* This structure cannot exceed sizeof(unsigned long [5]) AKA 20 bytes */
struct ax8817x_data {
	u8 multi_filter[AX_MCAST_FILTER_SIZE];
	struct ax88178_data *ax178dataptr;
	struct ax88772a_data *ax772a_data;
};

struct ax88172_int_data {
	u16 res1;
	u8 link;
	u16 res2;
	u8 status;
	u16 res3;
} __attribute__ ((packed));

