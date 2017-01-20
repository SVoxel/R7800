#ifndef _NET_SCAN_H
#define _NET_SCAN_H

#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/signal.h>
#include <sys/sysinfo.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <netinet/ether.h>
#include <netpacket/packet.h>

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#define ARP_IFNAME	"br0"
#define ARP_FILE	"/tmp/netscan/attach_device"
#define DHCP_LIST_FILE	"/tmp/dhcpd_hostlist"
#define WLAN_STA_FILE	"/tmp/sta_dev"

/* The max length of NETBIOS name is 15, the max length of DHCP hostanme is 255 (BOOTP/DHCP option 12) */
#define MAX_HOSTNAME_LEN	255

typedef unsigned int	uint32;
typedef unsigned short	uint16;
typedef unsigned char	uint8;

struct arpmsg
{
	/* Ethernet header */
	uint8	h_dest[6];	/* destination ether addr */
	uint8	h_source[6];	/* source ether addr */
	uint16	h_proto;		/* packet type ID field */

	/* ARP packet */
	uint16	ar_hrd;	/* hardware type (must be ARPHRD_ETHER) */
	uint16	ar_pro;	/* protocol type (must be ETH_P_IP) */
	uint8	ar_hln;	/* hardware address length (must be 6) */
	uint8	ar_pln;	/* protocol address length (must be 4) */
	uint16	ar_op;	/* ARP opcode */
	uint8	ar_sha[6];	/* sender's hardware address */
	uint8	ar_sip[4];	/* sender's IP address */
	uint8	ar_tha[6];	/* target's hardware address */
	uint8	ar_tip[4];	/* target's IP address */
	
	uint8	pad[18];	/* pad for min. Ethernet payload (60 bytes) */
} __attribute__ ((packed, aligned(4)));

struct nb_request 
{
	uint16	xid;
 	uint16	flags;
	uint16	questions;
	uint16	answer;
 	uint16	authority;
	uint16	additional;
	
	char		qname[34];

	uint16	qtype;
	uint16	qclass;
} __attribute__ ((packed));

struct nb_response_header
{
	uint16	xid;
	uint16	flags;
	uint16	questions;
	uint16	answer;
	uint16	authority;
	uint16	additional;
	
	char		qname[34];

	uint16	qtype;
	uint16	qclass;
	
	uint32	ttl;
	uint16	datalen;
	uint8	num_names;
} __attribute__ ((packed));

extern int open_arp_socket(struct sockaddr *me);
extern int open_bios_socket(void);
extern int dni_system(const char *output, const char *cmd, ...);

extern int recv_arp_pack(struct arpmsg *arpkt, struct in_addr *send_ip);
extern void recv_bios_pack(char *buf, int len, struct in_addr from);

extern void update_bios_name(uint8 *mac, char *host, struct in_addr ip);
extern void send_bios_query(int sock, struct in_addr dst_ip);

extern void reset_arp_table();
extern void scan_arp_table(int sock, struct sockaddr *me);
extern void show_arp_table(void);

extern char *config_get(char *name);

#endif

