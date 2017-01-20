#ifndef _INTERNET_H
#define _INTERNET_H

#include "httpd.h"

/*======================================================
                                                    [ PPPoE Detection ]
    ======================================================*/
/*********************** PPPoE RFC 2516 ***********************/
#define ETH_PPPOE_DISC	0x8863

#define CODE_PADI	0x09
#define CODE_PADO	0x07
#define CODE_PADR	0x19
#define CODE_PADS	0x65
#define CODE_PADT	0xA7
#define CODE_SESS	0x00

#define TAG_END_OF_LIST		0x0000
#define TAG_SERVICE_NAME	0x0101
#define TAG_AC_NAME			0x0102
#define TAG_HOST_UNIQ		0x0103
#define TAG_AC_COOKIE		0x0104
#define TAG_VENDOR_SPECIFIC	0x0105
#define TAG_RELAY_SESSION_ID	0x0110
#define TAG_SERVICE_NAME_ERROR	0x0201
#define TAG_AC_SYSTEM_ERROR	0x0202
#define TAG_GENERIC_ERROR		0x0203

/* Discovery phase states */
#define STATE_SENT_PADI		0
#define STATE_RECEIVED_PADO	1
#define STATE_SENT_PADR		2
#define STATE_SESSION			3
#define STATE_TERMINATED		4

/***********************************************************/
#define ETH_HDRSIZE		14
#define PPPOE_HDRSIZE	6
#define PPPOE_TAG_SIZE	4
#define PPPOE_DATA_LEN	1400
#define PPPOE_TIMEOUT	2

/* PPPoE Raw Packet */
struct pppoe_packet
{
	/* Ethernet header */
	uint8 h_dest[ETH_ALEN];	/* destination eth addr */
	uint8 h_source[ETH_ALEN];	/* source ether addr    */
	uint16 h_proto;			/* packet type ID field */

	/* PPPoE header */
	uint8 ver_type;
	uint8 code;
	uint16 sid;
	uint16 length;

	uint8 data[PPPOE_DATA_LEN];
} __attribute__ ((__packed__));

/* PPPoE Tag */
struct pppoe_tag 
{
	uint16 tag_type;
	uint16 tag_len;
	uint8 tag_data[0];
} __attribute ((packed));

#define TAG_TYPE(tag)	(((uint16) tag[0]) << 8) + (uint16) tag[1]
#define TAG_LEN(tag)	(((uint16) tag[2]) << 8) + (uint16) tag[3]

/*======================================================
                                                  [ DHCP & BPA Detection ]
    ======================================================*/
#define DHCP_TIMEOUT	2

#define SERVER_PORT		67
#define CLIENT_PORT		68

#define DHCP_MAGIC		0x63825363

#define BPA_SVR_SUFFIX	"bigpond.net.au"
#define VENDOR			"udhcp 0.9.8"

/* DHCP option codes (partial list) */
#define DHCP_PADDING		0x00
#define DHCP_SUBNET		0x01
#define DHCP_TIME_OFFSET	0x02
#define DHCP_ROUTER		0x03
#define DHCP_TIME_SERVER	0x04
#define DHCP_NAME_SERVER	0x05
#define DHCP_DNS_SERVER		0x06
#define DHCP_LOG_SERVER		0x07
#define DHCP_COOKIE_SERVER	0x08
#define DHCP_LPR_SERVER		0x09
#define DHCP_HOST_NAME		0x0c
#define DHCP_BOOT_SIZE		0x0d
#define DHCP_DOMAIN_NAME	0x0f
#define DHCP_SWAP_SERVER	0x10
#define DHCP_ROOT_PATH		0x11
#define DHCP_IP_TTL		0x17
#define DHCP_MTU		0x1a
#define DHCP_BROADCAST		0x1c
#define DHCP_NTP_SERVER		0x2a
#define DHCP_WINS_SERVER	0x2c
#define DHCP_REQUESTED_IP	0x32
#define DHCP_LEASE_TIME		0x33
#define DHCP_OPTION_OVER	0x34
#define DHCP_MESSAGE_TYPE	0x35
#define DHCP_SERVER_ID		0x36
#define DHCP_PARAM_REQ		0x37
#define DHCP_MESSAGE		0x38
#define DHCP_MAX_SIZE		0x39
#define DHCP_T1			0x3a
#define DHCP_T2			0x3b
#define DHCP_VENDOR		0x3c
#define DHCP_CLIENT_ID		0x3d

#define DHCP_END		0xFF


#define BOOTREQUEST		1
#define BOOTREPLY		2

#define ETH_10MB		1
#define ETH_10MB_LEN		6

#define DHCPDISCOVER	1
#define DHCPOFFER		2
#define DHCPREQUEST		3
#define DHCPDECLINE		4
#define DHCPACK			5
#define DHCPNAK			6
#define DHCPRELEASE		7
#define DHCPINFORM		8

#define OPTION_FIELD		0
#define FILE_FIELD		1
#define SNAME_FIELD		2

#define OPT_CODE	0
#define OPT_LEN		1
#define OPT_DATA		2

#define OPT_SIZE		312

struct dhcpMessage 
{
	uint8 op;
	uint8 htype;
	uint8 hlen;
	uint8 hops;
	
	uint32 xid;
	uint16 secs;
	uint16 flags;
	
	uint32 ciaddr;
	uint8 yiaddr[4];
	uint32 siaddr;
	uint32 giaddr;

	uint8 chaddr[16];
	uint8 sname[64];
	uint8 file[128];

	uint32 cookie;

	uint8 options[OPT_SIZE]; /* 312 - cookie */
};

struct rawPacket 
{
	/* Ethernet header */
	struct ether_header eth;
	struct iphdr ip;
	struct udphdr udp;
	struct dhcpMessage data;
} __attribute__ ((packed));

#define PKTSIZE_NO_OPT	( \
		sizeof(struct iphdr) + sizeof(struct udphdr) + \
		sizeof(struct dhcpMessage) - OPT_SIZE)

#define UDP_IP(iphdr)	( \
		(iphdr)->protocol	== IPPROTO_UDP &&	\
		(iphdr)->version	== 4 &&	\
		(iphdr)->ihl == 5 )
/*======================================================
                                                    [ PPTP Detection ]
    ======================================================*/
#define PPTP_MAGIC		0x1A2B3C4D	/* Magic cookie for PPTP datagrams */
#define PPTP_PORT		1723 		/* PPTP TCP port number            */
#define PPTP_TIMEOUT		3

/* The max length of PPTP control packet */
#define MAX_PKT_SIZE		512

/* Control Connection Message Types: --------------------------- */

#define PPTP_MESSAGE_CONTROL		1
#define PPTP_MESSAGE_MANAGE		2

/* (Control Connection Management) */
#define PPTP_START_CTRL_CONN_RQST	1
#define PPTP_START_CTRL_CONN_RPLY	2

/* (Framing capabilities for msg sender) */
#define PPTP_FRAME_ASYNC	1
#define PPTP_FRAME_SYNC		2
#define PPTP_FRAME_ANY 		3

/* (Bearer capabilities for msg sender) */
#define PPTP_BEARER_ANALOG	1
#define PPTP_BEARER_DIGITAL 	2
#define PPTP_BEARER_ANY		3

#define PPTP_VERSION_STRING		"1.00"
#define PPTP_VERSION				0x100
#define PPTP_FIRMWARE_STRING	"0.01"
#define PPTP_FIRMWARE_VERSION	0x001
#define PPTP_BUF_MAX			65536
#define PPTP_CONNECT_SPEED		100000000
#define PPTP_WINDOW			3
#define PPTP_DELAY				0
#define PPTP_BPS_MIN			2400
#define PPTP_BPS_MAX			100000000
#define PPTP_MAX_CHANNELS		0
#define PPTP_HOSTNAME			HOST_NAME
#define PPTP_VENDOR				"NETGEAR"
#define PPTP_FRAME_CAP			PPTP_FRAME_ANY
#define PPTP_BEARER_CAP			PPTP_BEARER_ANY

/* PPTP datagram structures (all data in network byte order): ----------*/
struct pptp_header 
{
	uint16	length;		/* message length in octets, including header */
	uint16	pptp_type;	/* PPTP message type. 1 for control message.  */

	uint32	magic;		/* this should be PPTP_MAGIC.                 */

	uint16	ctrl_type;	/* Control message type (0-15)                */
	uint16	reserved0;	/* reserved.  MUST BE ZERO.                   */
};

/* For control message types 1 and 2 */
struct pptp_start_ctrl_conn 
{ 
	struct pptp_header header;

	uint16	version;      	/* PPTP protocol version.  = PPTP_VERSION     */

	uint8	result_code;  /* these two fields should be zero on rqst msg*/
	uint8	error_code;   /* 0 unless result_code==2 (General Error)    */

	uint32	framing_cap;  /* Framing capabilities                       */
	uint32	bearer_cap;   /* Bearer Capabilities                        */

	uint16	max_channels; /* Maximum Channels (=0 for PNS, PAC ignores) */
	uint16	firmware_rev; /* Firmware or Software Revision              */

	uint8	hostname[64];	/* Host Name (64 octets, zero terminated)     */
	uint8	vendor[64];	/* Vendor string (64 octets, zero term.)      */
		/* MS says that end of hostname/vendor fields should be filled with   */
		/* octets of value 0, but Win95 PPTP driver doesn't do this.          */
};

#endif

