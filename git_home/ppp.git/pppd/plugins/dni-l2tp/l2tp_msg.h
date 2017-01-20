#ifndef _L2TP_MSG_H_
#define _L2TP_MSG_H_

#include "l2tp.h"

#include <sys/types.h>

#define L2TP_HOSTNAME		"NETGEAR-Router(--DNI--)"

/* Bit definitions */
#define TYPE_BIT				0x80
#define LENGTH_BIT			0x40
#define SEQUENCE_BIT			0x08
#define OFFSET_BIT			0x02
#define PRIORITY_BIT			0x01
#define RESERVED_BITS		0x34
#define VERSION_MASK		0x0F
#define VERSION_RESERVED	0xF0

#define AVP_MANDATORY_BIT	0x80
#define AVP_HIDDEN_BIT		0x40
#define AVP_RESERVED_BITS	0x3C

#define MANDATORY		1
#define NOT_MANDATORY	0
#define HIDDEN			1
#define NOT_HIDDEN		0
#define VENDOR_IETF		0

#define AVP_MESSAGE_TYPE			0
#define AVP_RESULT_CODE				1
#define AVP_PROTOCOL_VERSION		2
#define AVP_FRAMING_CAPABILITIES	3
#define AVP_BEARER_CAPABILITIES		4
#define AVP_TIE_BREAKER				5
#define AVP_FIRMWARE_REVISION		6
#define AVP_HOST_NAME				7
#define AVP_VENDOR_NAME			8
#define AVP_ASSIGNED_TUNNEL_ID		9
#define AVP_RECEIVE_WINDOW_SIZE	10
#define AVP_CHALLENGE				11
#define AVP_Q931_CAUSE_CODE		12
#define AVP_CHALLENGE_RESPONSE		13
#define AVP_ASSIGNED_SESSION_ID	14
#define AVP_CALL_SERIAL_NUMBER		15
#define AVP_MINIMUM_BPS				16
#define AVP_MAXIMUM_BPS			17
#define AVP_BEARER_TYPE				18
#define AVP_FRAMING_TYPE			19
#define AVP_CALLED_NUMBER			21
#define AVP_CALLING_NUMBER			22
#define AVP_SUB_ADDRESS				23
#define AVP_TX_CONNECT_SPEED		24
#define AVP_PHYSICAL_CHANNEL_ID       25
#define AVP_INITIAL_RECEIVED_CONFREQ	26
#define AVP_LAST_SENT_CONFREQ		27
#define AVP_LAST_RECEIVED_CONFREQ	28
#define AVP_PROXY_AUTHEN_TYPE		29
#define AVP_PROXY_AUTHEN_NAME		30
#define AVP_PROXY_AUTHEN_CHALLENGE	31
#define AVP_PROXY_AUTHEN_ID		32
#define AVP_PROXY_AUTHEN_RESPONSE	33
#define AVP_CALL_ERRORS				34
#define AVP_ACCM						35
#define AVP_RANDOM_VECTOR			36
#define AVP_PRIVATE_GROUP_ID		37
#define AVP_RX_CONNECT_SPEED		38
#define AVP_SEQUENCING_REQUIRED	39

#define HIGHEST_AVP	39

#define MESSAGE_SCCRQ	1
#define MESSAGE_SCCRP	2
#define MESSAGE_SCCCN	3
#define MESSAGE_StopCCN	4
#define MESSAGE_HELLO	6

#define MESSAGE_OCRQ	7
#define MESSAGE_OCRP	8
#define MESSAGE_OCCN	9

#define MESSAGE_ICRQ	10
#define MESSAGE_ICRP		11
#define MESSAGE_ICCN	12

#define MESSAGE_CDN		14
#define MESSAGE_WEN		15
#define MESSAGE_SLI		16

/* A fake type for our own consumption */
#define MESSAGE_ZLB		32767

/* Result and error codes */
#define RESULT_GENERAL_REQUEST		1
#define RESULT_GENERAL_ERROR		2
#define RESULT_CHANNEL_EXISTS		3
#define RESULT_NOAUTH				4
#define RESULT_UNSUPPORTED_VERSION	5
#define RESULT_SHUTTING_DOWN		6
#define RESULT_FSM_ERROR			7

#define ERROR_OK		0
#define ERROR_NO_CONTROL_CONNECTION	1
#define ERROR_BAD_LENGTH		2
#define ERROR_BAD_VALUE			3
#define ERROR_OUT_OF_RESOURCES	4
#define ERROR_INVALID_SESSION_ID	5
#define ERROR_VENDOR_SPECIFIC		6
#define ERROR_TRY_ANOTHER			7
#define ERROR_UNKNOWN_AVP_WITH_M_BIT	8

/* Maximum size of L2TP datagram we accept... kludge... */
#define MAX_PACKET_LEN	1024

#define MAX_SECRET_LEN	96
#define MAX_HOSTNAME	128
#define MAX_OPTS		64

#define MAX_RETRANSMISSIONS	5

/* an L2TP (control) datagram */
struct l2tp_dgram 
{
	__u16 msg_type;	/* Message type */

	__u8 bits;			/* Options bits */
	__u8 version;		/* Version */

	__u16 length;	/* Length (opt) */
	__u16 tid;		/* Tunnel ID */
	__u16 sid;		/* Session ID */
	__u16 Ns;		/* Ns (opt) */
	__u16 Nr;		/* Nr (opt) */

	size_t payload_len;	/* Payload len (not including L2TP header) */
	size_t cursor;			/* Cursor for adding/stripping AVP's */
	size_t alloc_len;		/* Length allocated for data */

	__u8 data[MAX_PACKET_LEN];	/* Data */
};

extern void l2tp_tunnel_open(struct l2tp_conn_mngr *conn);
extern void l2tp_tunnel_close(struct l2tp_conn_mngr *conn);
extern void l2tp_session_open(struct l2tp_conn_mngr *conn);

extern void l2tp_new_control(struct l2tp_dgram *dgram, __u16 msg_type,
		__u16 tid, __u16 sid);
extern int l2tp_get_dgram(int fd, struct l2tp_dgram *dgram, struct sockaddr_in *from);
extern int l2tp_send(struct l2tp_conn_mngr *conn, struct l2tp_dgram *dgram);
extern int l2tp_add_avp(struct l2tp_dgram *dgram, int mandatory, 
		__u16 len, __u16 type, void *val);
extern __u8 *l2tp_pull_avp(struct l2tp_dgram *dgram, __u16 *type, __u16 *len, __u16 *vendor);

#endif

