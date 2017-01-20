#ifndef INC_PPTP_H
#define INC_PPTP_H

#include "pptp.h"
#include "../../config.h"

#include <sys/types.h>


#define hton8(x)  (x)
#define ntoh8(x)  (x)
#define hton16(x) htons(x)
#define ntoh16(x) ntohs(x)
#define hton32(x) htonl(x)
#define ntoh32(x) ntohl(x)

/* PPTP magic numbers: ----------------------------------------- */

#define PPTP_MAGIC	0x1A2B3C4D	/* Magic cookie for PPTP datagrams */
#define PPTP_PORT	1723 		/* PPTP TCP port number            */
#define PPTP_PROTO	47			/* PPTP IP protocol number         */


/* Control Connection Message Types: --------------------------- */

#define PPTP_MESSAGE_CONTROL		1
#define PPTP_MESSAGE_MANAGE		2

/* Control Message Types: -------------------------------------- */

/* (Control Connection Management) */
#define PPTP_START_CTRL_CONN_RQST	1
#define PPTP_START_CTRL_CONN_RPLY	2
#define PPTP_STOP_CTRL_CONN_RQST	3
#define PPTP_STOP_CTRL_CONN_RPLY	4
#define PPTP_ECHO_RQST				5
#define PPTP_ECHO_RPLY				6

/* (Call Management) */
#define PPTP_OUT_CALL_RQST			7
#define PPTP_OUT_CALL_RPLY			8
#define PPTP_IN_CALL_RQST			9
#define PPTP_IN_CALL_RPLY			10
#define PPTP_IN_CALL_CONNECT		11
#define PPTP_CALL_CLEAR_RQST		12
#define PPTP_CALL_CLEAR_NTFY		13

/* (Error Reporting) */
#define PPTP_WAN_ERR_NTFY			14

/* (PPP Session Control) */
#define PPTP_SET_LINK_INFO			15


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
#define PPTP_FRAME_CAP			PPTP_FRAME_ANY
#define PPTP_BEARER_CAP			PPTP_BEARER_ANY


/* PPTP capabilities: ---------------------------------------------*/

/* (Framing capabilities for msg sender) */
#define PPTP_FRAME_ASYNC	1
#define PPTP_FRAME_SYNC		2
#define PPTP_FRAME_ANY 		3

/* (Bearer capabilities for msg sender) */
#define PPTP_BEARER_ANALOG	1
#define PPTP_BEARER_DIGITAL 	2
#define PPTP_BEARER_ANY		3


/* (Reasons to close a connection) */
#define PPTP_STOP_NONE				1	/* no good reason                        */
#define PPTP_STOP_PROTOCOL			2	/* can't support peer's protocol version */
#define PPTP_STOP_LOCAL_SHUTDOWN	3	/* requester is being shut down          */

/* PPTP datagram structures (all data in network byte order): ----------*/
struct pptp_header 
{
	u16	length;		/* message length in octets, including header */
	u16	pptp_type;	/* PPTP message type. 1 for control message.  */

	u32	magic;		/* this should be PPTP_MAGIC.                 */

	u16	ctrl_type;	/* Control message type (0-15)                */
	u16	reserved0;	/* reserved.  MUST BE ZERO.                   */
};

/* For control message types 1 and 2 */
struct pptp_start_ctrl_conn 
{ 
	struct pptp_header header;

	u16	version;      	/* PPTP protocol version.  = PPTP_VERSION     */

	u8	result_code;  /* these two fields should be zero on rqst msg*/
	u8	error_code;   /* 0 unless result_code==2 (General Error)    */

	u32	framing_cap;  /* Framing capabilities                       */
	u32	bearer_cap;   /* Bearer Capabilities                        */

	u16	max_channels; /* Maximum Channels (=0 for PNS, PAC ignores) */
	u16	firmware_rev; /* Firmware or Software Revision              */

	u8	hostname[64];	/* Host Name (64 octets, zero terminated)     */
	u8	vendor[64];	/* Vendor string (64 octets, zero term.)      */
		/* MS says that end of hostname/vendor fields should be filled with   */
		/* octets of value 0, but Win95 PPTP driver doesn't do this.          */
};

 /* For control message types 3 and 4 */
struct pptp_stop_ctrl_conn 
{
	struct pptp_header header;

	u8	reason_result; /* reason for rqst, result for rply          */
	u8	error_code;	  /* MUST be 0, unless rply result==2 (general err)*/

	u16	reserved1;    /* MUST be 0                                */
};

/* For control message type 5 */
struct pptp_echo_rqst 
{ 
	struct pptp_header header;

	u32	identifier;   /* arbitrary value set by sender which is used */
				  /* to match up reply and request               */
};

/* For control message type 6 */
struct pptp_echo_rply 
{ 
	struct pptp_header header;

	u32	identifier;	  /* should correspond to id of rqst             */

	u8	result_code;
	u8	error_code;    /* =0, unless result_code==2 (general error)   */

	u16	reserved1;    /* MUST BE ZERO                                */
};

/* For control message type 7 */
struct pptp_out_call_rqst 
{ 
	struct pptp_header header;

	u16	call_id;	  /* Call ID (unique id used to multiplex data)  */
	u16	call_sernum;  /* Call Serial Number (used for logging)       */

	u32	bps_min;      /* Minimum BPS (lowest acceptable line speed)  */
	u32	bps_max;	  /* Maximum BPS (highest acceptable line speed) */
	u32	bearer;	  /* Bearer type                                 */
	u32	framing;      /* Framing type                                */

	u16	recv_size;	  /* Recv. Window Size (no. of buffered packets) */
	u16	delay;	  /* Packet Processing Delay (in 1/10 sec)       */

	u16	phone_len;	  /* Phone Number Length (num. of valid digits)  */
	u16	reserved1;    /* MUST BE ZERO				 */

	u8	phone_num[64]; /* Phone Number (64 octets, null term.)       */
	u8	subaddress[64]; /* Subaddress (64 octets, null term.)         */
};

/* For control message type 8 */
struct pptp_out_call_rply 
{ 
	struct pptp_header header;

	u16	call_id;      /* Call ID (used to multiplex data over tunnel)*/
	u16	call_id_peer; /* Peer's Call ID (call_id of pptp_out_call_rqst)*/

	u8	result_code;  /* Result Code (1 is no errors)                */
	u8	error_code;   /* Error Code (=0 unless result_code==2)       */
	u16	cause_code;   /* Cause Code (addt'l failure information)     */

	u32	speed;        /* Connect Speed (in BPS)                      */

	u16	recv_size;    /* Recv. Window Size (no. of buffered packets) */
	u16	delay;	  /* Packet Processing Delay (in 1/10 sec)       */

	u32	channel;      /* Physical Channel ID (for logging)           */
};

/* For control message type 9 */
struct pptp_in_call_rqst 
{
	struct pptp_header header;

	u16	call_id;	  /* Call ID (unique id used to multiplex data)  */
	u16	call_sernum;  /* Call Serial Number (used for logging)       */

	u32	bearer;	  /* Bearer type                                 */
	u32	channel;      /* Physical Channel ID (for logging)           */

	u16	dialed_len;   /* Dialed Number Length (# of valid digits)    */
	u16	dialing_len;  /* Dialing Number Length (# of valid digits)   */

	u8	dialed_num[64]; /* Dialed Number (64 octets, zero term.)      */
	u8	dialing_num[64]; /* Dialing Number (64 octets, zero term.)    */
	u8	subaddress[64];  /* Subaddress (64 octets, zero term.)        */
};

/* For control message type 10 */
struct pptp_in_call_rply 
{
	struct pptp_header header;

	u16	call_id;      /* Call ID (used to multiplex data over tunnel)*/
	u16	call_id_peer; /* Peer's Call ID (call_id of pptp_out_call_rqst)*/

	u8	result_code;  /* Result Code (1 is no errors)                */
	u8	error_code;   /* Error Code (=0 unless result_code==2)       */
	u16	recv_size;    /* Recv. Window Size (no. of buffered packets) */

	u16	delay;	  /* Packet Processing Delay (in 1/10 sec)       */
	u16	reserved1;    /* MUST BE ZERO                                */
};

/* For control message type 11 */
struct pptp_in_call_connect 
{
	struct pptp_header header;

	u16	call_id_peer; /* Peer's Call ID (call_id of pptp_out_call_rqst)*/
	u16	reserved1;    /* MUST BE ZERO                                */

	u32	speed;        /* Connect Speed (in BPS)                      */

	u16	recv_size;    /* Recv. Window Size (no. of buffered packets) */
	u16	delay;	  /* Packet Processing Delay (in 1/10 sec)       */

	u32	framing;      /* Framing type                                */
};

/* For control message type 12 */
struct pptp_call_clear_rqst 
{ 
	struct pptp_header header;

	u16	call_id;      /* Call ID (used to multiplex data over tunnel)*/
	u16	reserved1;    /* MUST BE ZERO                                */
};

/* For control message type 13 */
struct pptp_call_clear_ntfy 
{
	struct pptp_header header;

	u16	call_id;      /* Call ID (used to multiplex data over tunnel)*/

	u8	result_code;  /* Result Code                                 */
	u8	error_code;   /* Error Code (=0 unless result_code==2)       */

	u16	cause_code;   /* Cause Code (for ISDN, is Q.931 cause code)  */
	u16	reserved1;    /* MUST BE ZERO                                */

	u8	call_stats[128]; /* Call Statistics: 128 octets, ascii, 0-term */
};

/* For control message type 14 */
struct pptp_wan_err_ntfy 
{    
	struct pptp_header header;

	u16	call_id_peer; /* Peer's Call ID (call_id of pptp_out_call_rqst)*/
	u16	reserved1;    /* MUST BE ZERO                                */

	u32	crc_errors;   /* CRC errors 				 */
	u32	frame_errors; /* Framing errors 				 */
	u32	hard_errors;  /* Hardware overruns 				 */
	u32	buff_errors;  /* Buffer overruns				 */
	u32	time_errors;  /* Time-out errors				 */
	u32	align_errors; /* Alignment errors				 */
};

/* For control message type 15 */
struct pptp_set_link_info 
{   
	struct pptp_header header;

	u16	call_id_peer; /* Peer's Call ID (call_id of pptp_out_call_rqst) */
	u16	reserved1;    /* MUST BE ZERO                                   */

	u32	send_accm;    /* Send ACCM (for PPP packets; default 0xFFFFFFFF)*/
	u32	recv_accm;    /* Receive ACCM (for PPP pack.;default 0xFFFFFFFF)*/
};

#endif /* INC_PPTP_H */

