/*
**  igmpproxy - IGMP proxy based multicast router 
**  Copyright (C) 2005 Johnny Egeland <johnny@rlo.org>
**
**  This program is free software; you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation; either version 2 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
**
**----------------------------------------------------------------------------
**
**  This software is derived work from the following software. The original
**  source code has been modified from it's original state by the author
**  of igmpproxy.
**
**  smcroute 0.92 - Copyright (C) 2001 Carsten Schill <carsten@cschill.de>
**  - Licensed under the GNU General Public License, version 2
**  
**  mrouted 3.9-beta3 - COPYRIGHT 1989 by The Board of Trustees of 
**  Leland Stanford Junior University.
**  - Original license can be found in the "doc/mrouted-LINCESE" file.
**
*/
/**
*   defs.h - Header file for common includes.
*/

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <signal.h>

#include <sys/socket.h>
#include <sys/un.h>
#include <sys/time.h>

#include <net/if.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <pcap.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include "bt_igmp.h"

// The multicats API needs linux spesific headers !!!                  
#ifdef USE_LINUX_IN_H
    #include <linux/in.h>
    #include <linux/mroute.h>
#else
    #include <netinet/in.h>
    #include <netinet/ip.h>
    #include <netinet/igmp.h>
    #include <linux/igmp.h>
    #include <arpa/inet.h>
#endif

// The default path for the config file...
#define     IGMPPROXY_CONFIG_FILEPATH     "/etc/igmpproxy.conf"
#define     ENABLE_DEBUG    0

#define 	IGMP_IMMEDIATE_LEAVE	1
#define 	SUPPORT_IGMPV3			1
#define   SUPPURT_MCAST_TO_UNICAST 1
#define   DNI_MULTI_LAN_SUPPORT 1

#define PORT_MAX_NUM 4

#if (DNI_MULTI_LAN_SUPPORT) && (SUPPORT_IGMPV3)
#include "source_set.h"
#endif

#if (SUPPURT_MCAST_TO_UNICAST) || (DNI_MULTI_LAN_SUPPORT)
#define NIPQUAD(addr) \
	((unsigned char *)&addr)[0], \
	((unsigned char *)&addr)[1], \
	((unsigned char *)&addr)[2], \
	((unsigned char *)&addr)[3]

#define MULTICAST(x)	(((x) & htonl(0xf0000000)) == htonl(0xe0000000))
extern int join_group(unsigned long src, unsigned long group);
#endif
/*
 * Limit on length of route data
 */
#define MAX_IP_PACKET_LEN	576
#define MIN_IP_HEADER_LEN	20
#define MAX_IP_HEADER_LEN	60

#define MAX_MC_VIFS    32     // !!! check this const in the specific includes

// Useful macros..          
#define MIN( a, b ) ((a) < (b) ? (a) : (b))
#define MAX( a, b ) ((a) < (b) ? (b) : (a))
#define VCMC( Vc )  (sizeof( Vc ) / sizeof( (Vc)[ 0 ] ))
#define VCEP( Vc )  (&(Vc)[ VCMC( Vc ) ])

// Bit manipulation macros...
#define BIT_ZERO(X)      ((X) = 0)
#define BIT_SET(X,n)     ((X) |= 1 << (n))
#define BIT_CLR(X,n)     ((X) &= ~(1 << (n)))
#define BIT_TST(X,n)     ((X) & 1 << (n))


// Useful defs...
#define FALSE		0
#define TRUE		1

#if (ENABLE_DEBUG)
#define DPRINTF(fmt,arg...) do { \
	FILE *fp = fopen("/dev/console", "w"); \
	if (fp) { \
		fprintf(fp, fmt, ## arg); \
		fclose(fp); \
	} \
} while (0)
#else
#define DPRINTF(fmt,arg...) 
#endif

typedef void (*cfunc_t)   (void *);

typedef u_int8_t   uint8;
typedef u_int16_t  uint16;
typedef u_int32_t  uint32;

//#################################################################################
//  Globals
//#################################################################################

/*
 * External declarations for global variables and functions.
 */
#define RECV_BUF_SIZE 8192
extern char     *recv_buf;
extern char     *send_buf;

extern char     s1[];
extern char     s2[];
extern char		s3[];
extern char		s4[];


//#################################################################################
//  Lib function prototypes.
//#################################################################################

/* syslog.c
 */
extern int  Log2Stderr;           // Log threshold for stderr, LOG_WARNING .... LOG_DEBUG 
extern int  LogLastServerity;     // last logged serverity
extern int  LogLastErrno;         // last logged errno value
extern char LogLastMsg[ 128 ];    // last logged message

#define	    IF_DEBUG	 //if(Log2Stderr & LOG_DEBUG)

void igmp_syslog( int Serverity, int Errno, const char *FmtSt, ... );

/* ifvc.c
 */
#define MAX_IF         40     // max. number of interfaces recognized 

// Interface states
#define IF_STATE_DISABLED      0   // Interface should be ignored.
#define IF_STATE_UPSTREAM      1   // Interface is the upstream interface
#define IF_STATE_DOWNSTREAM    2   // Interface is a downstream interface

// Multicast default values...
#define DEFAULT_ROBUSTNESS     2
#define DEFAULT_THRESHOLD      1
#define DEFAULT_RATELIMIT      0

// Define timer constants (in seconds...)
#define INTERVAL_QUERY          125
#define INTERVAL_QUERY_RESPONSE  10
//#define INTERVAL_QUERY_RESPONSE  10

#define ROUTESTATE_NOTJOINED            0   // The group corresponding to route is not joined
#define ROUTESTATE_JOINED               1   // The group corresponding to route is joined
#define ROUTESTATE_CHECK_LAST_MEMBER    2   // The router is checking for hosts

// Linked list of networks... 
struct SubnetList {
    uint32              subnet_addr;
    uint32              subnet_mask;
    struct SubnetList*  next;
};

struct IfDesc {
    char                Name[ sizeof( ((struct ifreq *)NULL)->ifr_name ) ];
    struct in_addr      InAdr;          /* == 0 for non IP interfaces */            
    short               Flags;
    short               state;
    struct SubnetList*  allowednets;
    unsigned int        robustness;
    unsigned char       threshold;   /* ttl limit */
    unsigned int        ratelimit; 
    unsigned int        index;
    //Add by Timo
    unsigned int	ifIndex;
    //End
};

// Keeps common configuration settings 
struct Config {
#ifdef USE_ATH_HEADER
    // 0:Undefined; 1:IGMP Snooping; 2:IGMP Proxy; 3: Snooping & Proxy;
    unsigned int        mode;
#endif
    unsigned int        robustnessValue;
    unsigned int        queryInterval;
    unsigned int        queryResponseInterval;
    // Used on startup..
    unsigned int        startupQueryInterval;
    unsigned int        startupQueryCount;
    // Last member probe...
    unsigned int        lastMemberQueryInterval;
    unsigned int        lastMemberQueryCount;
    // Set if upstream leave messages should be sent instantly..
    unsigned short      fastUpstreamLeave;
};

#if SUPPORT_IGMPV3
#define IP_RA_OPTION_LEN	4

#define IGMPV3_MINLEN			12
#define IGMP_V3_MEMBERSHIP_REPORT	0x22
//#define IGMPV3_ALL_MCR    ((in_addr_t) 0xe0000016) /* 224.0.0.22 */

#define IGMPV1_V2				0
#define IGMPV3					1

/* V3 group record types [grec_type] */
#define IGMPV3_MODE_IS_INCLUDE		1
#define IGMPV3_MODE_IS_EXCLUDE		2
#define IGMPV3_CHANGE_TO_INCLUDE	3
#define IGMPV3_CHANGE_TO_EXCLUDE	4
#define IGMPV3_ALLOW_NEW_SOURCE 5
#define IGMPV3_BLOCK_OLD_SOURCE 6

#if 0 /* Ares*/
/* Group record */
struct igmpv3_grec {
	uint8	grec_type;				/* record type */			
	uint8	grec_auxwords;			/* aux data length */
	uint16	grec_nsrcs;				/* number of sources */
	struct in_addr	grec_mca;				/* multicast address */
	struct in_addr	grec_src[0];			/* source address */
};

struct igmpv3_report {
	uint8	type;					/* type */
	uint8	resv1;					/* reserved */
	uint16	csum;					/* checksum */
	uint16	resv2;					/* reserved */
	uint16	ngrec;					/* number of group records */
	struct igmpv3_grec grec[0];		/* group record */
};

struct igmpv3_query {
	uint8	igmp_type;             	/* IGMP type */
	uint8 	igmp_code;             	/* routing code */
	uint16 	igmp_cksum;           	/* checksum */
	struct in_addr igmp_group;      /* group address */
	uint8	resv:4,					/* reserved */
	     	suppress:1,				/* suppress router-side processing */
	     	qrv:3;					/* querier's robustness variable */
	uint8	qqic;					/* querier's query interval code */
	uint16	nsrcs;					/* number of sources */
	struct in_addr	srcs[0];				/* first entry of src list */
};
#endif
#endif //IGMPV3

// Defines the Index of the upstream VIF...
extern int upStreamVif;

/* ifvc.c
 */
void buildIfVc( void );
struct IfDesc *getIfByName( const char *IfName );
struct IfDesc *getIfByIx( unsigned Ix );
struct IfDesc *getIfByAddress( uint32 Ix );
#ifdef USE_ATH_HEADER
int initSnoopBr();
int clearSnoopBr();
#endif

/* mroute-api.c
 */
struct MRouteDesc {
    struct in_addr  OriginAdr, McAdr;
    short           InVif;
    uint8           TtlVc[ MAX_MC_VIFS ];
};

#ifdef USE_ATH_HEADER
int igmpProxyInit();
void igmpProxyCleanUp();
int getIgmpPState();
void setIgmpPState(int);

int igmpSnoopInit();
void igmpSnoopCleanUp();
int getIgmpSState();
void setIgmpSState(int);
void delVIF( struct IfDesc *IfDp );
#endif

// IGMP socket as interface for the mrouted API
// - receives the IGMP messages
extern int MRouterFD;
#ifdef USE_ATH_HEADER
extern struct Config commonConfig;
#endif

int enableMRouter( void );
void disableMRouter( void );
void addVIF( struct IfDesc *Dp );
int addMRoute( struct MRouteDesc * Dp );
int delMRoute( struct MRouteDesc * Dp );
int getVifIx( struct IfDesc *IfDp );

/* config.c
 */
int loadConfig(char *configFile);
void configureVifs();
struct Config *getCommonConfig();

/* igmp.c
*/
extern uint32 allhosts_group;
extern uint32 allrouters_group;
#if (SUPPORT_IGMPV3)
extern uint32 all_group_igmpv3;
extern int group_compatibilty_mode;
#endif
void initIgmp(void);
void acceptIgmp(int);
void sendIgmp (uint32, uint32, int, int, uint32,int);

/* lib.c
 */
char   *fmtInAdr( char *St, struct in_addr InAdr );
char   *inetFmt(uint32 addr, char *s);
char   *inetFmts(uint32 addr, uint32 mask, char *s);
int     inetCksum(u_short *addr, u_int len);

/* kern.c
 */
void k_set_rcvbuf(int bufsize, int minsize);
void k_hdr_include(int bool);
void k_set_ttl(int t);
void k_set_loop(int l);
void k_set_if(uint32 ifa);
/*
void k_join(uint32 grp, uint32 ifa);
void k_leave(uint32 grp, uint32 ifa);
*/

/* udpsock.c
 */
int openUdpSocket( uint32 PeerInAdr, uint16 PeerPort );

/* mcgroup.c
 */
int joinMcGroup( int UdpSock, struct IfDesc *IfDp, uint32 mcastaddr );
int leaveMcGroup( int UdpSock, struct IfDesc *IfDp, uint32 mcastaddr );


/* rttable.c
 */
void initRouteTable();
void clearAllRoutes();
#if (IGMP_IMMEDIATE_LEAVE)
#ifdef USE_ATH_HEADER
int insertRoute(uint32 port, uint32 group, int ifx, uint32 src);
#else
int insertRoute(uint32 group, int ifx, uint32 src);
#endif
#else
int insertRoute(uint32 group, int ifx);
#endif
int activateRoute(uint32 group, uint32 originAddr);
void ageActiveRoutes();
#if (IGMP_IMMEDIATE_LEAVE)
void setRouteLastMemberMode(uint32 group, uint32 src);
#else
void setRouteLastMemberMode(uint32 group);
#endif
int lastMemberGroupAge(uint32 group);

/* request.c
 */
#ifdef USE_ATH_HEADER
void acceptGroupReport(uint32 port, uint32 src, uint32 group, uint8 type);
 #else
void acceptGroupReport(uint32 src, uint32 group, uint8 type);
#endif
void acceptLeaveMessage(uint32 src, uint32 group);
void sendGeneralMembershipQuery();
#ifdef USE_ATH_HEADER
void igmpTimerHandle();
void setStartCount();
#endif

/* callout.c 
*/
void callout_init();
void free_all_callouts();
void age_callout_queue(int);
int timer_nextTimer();
int timer_setTimer(int, cfunc_t, void *);
int timer_clearTimer(int);
int timer_leftTimer(int);

#ifdef USE_ATH_HEADER
/* fdbtable.c 
 */
int insertFdb(uint32 group, uint32 port);
void initFdbTable();

/* fdb.c
 */
void clearAllFdbs();
void ageActiveFdbs();
int internUpdateFdb(int, uint32, int);
void logFdbTable(char *);
#endif

/* confread.c
 */
#define MAX_TOKEN_LENGTH    30

int openConfigFile(char *filename);
void closeConfigFile();
char* nextConfigToken();
char* getCurrentConfigToken();

#ifdef USE_ATH_HEADER    
/* switch-api.c
*/
int addMFdb(uint32 group, uint32 port);
int delMFdb(uint32 group);
int setMultiOnVlan(int enable);
int enableHwIGMPS();
int disableHwIGMPS();
#endif

struct RouteTable {
    struct RouteTable   *nextroute;     // Pointer to the next group in line.
    struct RouteTable   *prevroute;     // Pointer to the previous group in line.
    uint32              group;          // The group to route
    uint32              originAddr;     // The origin adress (only set on activated routes)
    uint32              vifBits;        // Bits representing recieving VIFs.

    // Keeps the upstream membership state...
    short               upstrState;     // Upstream membership state.

    // These parameters contain aging details.
    uint32              ageVifBits;     // Bits representing aging VIFs.
    int                 ageValue;       // Downcounter for death.          
    int                 ageActivity;    // Records any acitivity that notes there are still listeners.
#if (DNI_MULTI_LAN_SUPPORT)
	uint32 mode;
	struct source_set source;
	uint32 port_mode[PORT_MAX_NUM + 1];
	struct source_set port_source[PORT_MAX_NUM + 1];
#endif
#ifdef USE_ATH_HEADER
	uint32 port[PORT_MAX_NUM+1];
#endif
 #if (IGMP_IMMEDIATE_LEAVE)
	struct Listener		*listeners;
#endif
};

#if (IGMP_IMMEDIATE_LEAVE)
#define IGMP_MBR_TIMEOUT 260
struct Listener {
#if (DNI_MULTI_LAN_SUPPORT)
	int version;
	int mode;
	struct source_set source;
#endif
	uint32              srcAddr;
	uint32  timeout;
#ifdef USE_ATH_HEADER
	uint32 port;
#endif
	struct Listener		*nextlistener;
};
#endif

#ifdef USE_ATH_HEADER
#define atlog igmp_syslog
#endif

#if (DNI_MULTI_LAN_SUPPORT) && (SUPPORT_IGMPV3)
enum group_state
{
	st_none,
	st_is_in,
	st_is_ex,
	st_max
};

enum incoming_event
{
	ev_none,
	ev_is_in,
	ev_is_ex,
	ev_to_in,
	ev_to_ex,
	ev_allow,
	ev_block,
	ev_max
};

#define FSM(a, b) (a * ev_max + b)
#define PORT_INIT_STATE 0xff

extern struct source_set ZEROSET;
#endif

/* bt_igmp.c
 */

#define LAN_IF "br0"
#define WAN_IF "brwan"
#define PPP_IF "ppp0"

extern int	recv_len;
extern char	*device;
extern char *config_get(char *name);
void accept_membership_query(uint32 src, uint32 dst, uint32 group, int code);
