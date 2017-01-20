#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>

#include "l2tp_msg.h"

#define GET_U16(val)	(((__u16) val[0]) * 256 + (__u16) val[1])

typedef int (*l2tp_call_cb)(struct l2tp_conn_mngr *, struct l2tp_dgram*, 
		struct sockaddr_in *);

static void l2tp_wait_packet(struct l2tp_conn_mngr *conn, int timeout, l2tp_call_cb callback)
{
	fd_set readable;
	struct timeval tv;
	struct sockaddr_in from;
	struct l2tp_dgram dgram;

	for (;;) {
		tv.tv_sec = timeout;
		tv.tv_usec = 0;

		FD_ZERO(&readable);
		FD_SET(conn->ctrl_socket, &readable);

		if (select(conn->ctrl_socket + 1, &readable, NULL, NULL, &tv) < 1)
			return; /* Timed out or error happen */

		if (l2tp_get_dgram(conn->ctrl_socket, &dgram, &from) == 0)
			return;

		if (callback(conn, &dgram, &from) == 0)
			return;
	} 
}

/**********************************************************************
*	Sends SCCRQ to establish a tunnel.
*	The following AVPs MUST be present in the SCCRQ (RFC2661):
*		Message Type AVP
*		Protocol Version
*		Host Name
*		Framing Capabilities
*		Assigned Tunnel ID
***********************************************************************/
#define ADD_U16AVP(type, val)	do {	\
	u16_val = htons(val);	\
	l2tp_add_avp(&dgram, MANDATORY, sizeof(u16_val), type, &u16_val);	\
} while (0)

static void tunnel_send_SCCRQ(struct l2tp_conn_mngr *conn)
{
	__u16 u16_val;
	__u32 u32_val;
	struct l2tp_dgram dgram;

	l2tp_new_control(&dgram, MESSAGE_SCCRQ, 0, 0);

	ADD_U16AVP(AVP_PROTOCOL_VERSION, 0x0100);
	ADD_U16AVP(AVP_ASSIGNED_TUNNEL_ID, conn->tunnel_id);
	ADD_U16AVP(AVP_RECEIVE_WINDOW_SIZE, 4);

	u32_val = htonl(3); /* sync and async */
	l2tp_add_avp(&dgram, MANDATORY, sizeof(u32_val), AVP_FRAMING_CAPABILITIES, &u32_val);
	l2tp_add_avp(&dgram, MANDATORY, (__u16)strlen(L2TP_HOSTNAME), AVP_HOST_NAME, L2TP_HOSTNAME);

	dgram.Ns = conn->Ns;
	dgram.Nr = conn->Nr;

	l2tp_send(conn, &dgram);
}

/**********************************************************************
*	Sends a SCCCN control message.
*	The following AVP MUST be present in the SCCCN:
*		Message Type
***********************************************************************/
static void tunnel_send_SCCCN(struct l2tp_conn_mngr *conn)
{
	struct l2tp_dgram dgram;

	l2tp_new_control(&dgram, MESSAGE_SCCCN, conn->peer_tunnel, 0);

	dgram.Ns = conn->Ns;
	dgram.Nr = conn->Nr;

	l2tp_send(conn, &dgram);
}

/**********************************************************************
*	Sends a StopCCN control message.
*	The following AVPs MUST be present in the StopCCN:
*		Message Type
*		Assigned Tunnel ID
*		Result Code
***********************************************************************/
static void tunnel_send_StopCCN(struct l2tp_conn_mngr *conn)
{
	__u16 u16_val, len;
	char	buf[64];
	struct l2tp_dgram dgram;

	/* Build the buffer for the result-code AVP */
	buf[0] = RESULT_SHUTTING_DOWN / 256;
	buf[1] = RESULT_SHUTTING_DOWN & 255;
	buf[2] = 0;
	buf[3] = 0;

	sprintf(buf + 4, "%s", "shutting down");
	len = 4 + strlen(buf + 4);

	/* Build the datagram */
	l2tp_new_control(&dgram, MESSAGE_StopCCN, conn->peer_tunnel, 0);

	/* Add assigned tunnel ID */
	u16_val = htons(conn->tunnel_id);
	l2tp_add_avp(&dgram, MANDATORY, sizeof(u16_val), AVP_ASSIGNED_TUNNEL_ID, &u16_val);

	/* Add result code */
	l2tp_add_avp(&dgram, MANDATORY, len, AVP_RESULT_CODE, buf);

	dgram.Ns = conn->Ns;
	dgram.Nr = conn->Nr;

	l2tp_send(conn, &dgram);
}

/**********************************************************************
*	Sends a HELLO control message.
*	The Following AVP MUST be present in the HELLO message:
*		Message Type
***********************************************************************/
static void tunnel_send_Hello(struct l2tp_conn_mngr *conn)
{
	struct l2tp_dgram	dgram;

	l2tp_new_control(&dgram, MESSAGE_HELLO, conn->peer_tunnel, 0);

	dgram.Ns = conn->Ns;
	dgram.Nr = conn->Nr;

	conn->Ns++;

	l2tp_send(conn, &dgram);
}

/**********************************************************************
*	Sends a ZLB ack packet.
***********************************************************************/
static void tunnel_send_ZLB(struct l2tp_conn_mngr *conn)
{
	struct l2tp_dgram	dgram;
	l2tp_new_control(&dgram, MESSAGE_ZLB, conn->peer_tunnel, 0);

	dgram.Nr = conn->Nr;
	dgram.Ns = conn->Ns;

	l2tp_send(conn, &dgram);
}

/**********************************************************************
*	Sends a ZLB ack packet.
Nr of the next message sent is not be updated by
   the Ns of the ZLB.

/**********************************************************************
*	Handles an incoming SCCRP
*	The following AVPs MUST be present in the SCCRP:
*		Message Type
*		Protocol Version
*		Framing Capabilities
*		Host Name
*		Assigned Tunnel ID
***********************************************************************/
static int tunnel_handle_SCCRP(struct l2tp_conn_mngr *conn,
		struct l2tp_dgram *dgram, struct sockaddr_in *from)
{
	__u8 *val;
	__u16 type, len, vendor;
	__u16 tunnel_id = 0, protocol_vld = 0;
	__u16 framing_vld = 0, host_vld = 0;

	if (from->sin_addr.s_addr != conn->server.s_addr ||
	    dgram->msg_type != MESSAGE_SCCRP ||
	    dgram->tid != conn->tunnel_id)
		return -1;

	conn->Nr = dgram->Ns + 1;

	while(1) {
		val = l2tp_pull_avp(dgram, &type, &len, &vendor);

		if (val == NULL)
			break;

		if (type == AVP_HOST_NAME)
			host_vld = 1;
		else if (type == AVP_ASSIGNED_TUNNEL_ID)
			tunnel_id = GET_U16(val);
		else if (type == AVP_PROTOCOL_VERSION)
			protocol_vld = (GET_U16(val) == 0x0100);
		else if (type == AVP_FRAMING_CAPABILITIES)
			framing_vld = 1;
	}

	if (tunnel_id == 0 ||protocol_vld == 0 ||framing_vld == 0 ||host_vld == 0)
		return -1;

	conn->peer_tunnel = tunnel_id;
	conn->tunnel_state = L2TP_TUNNEL_WAIT_CTL_CONN;

	return 0;
}

static int tunnel_handle_ZLB(struct l2tp_conn_mngr *conn,
		struct l2tp_dgram *dgram, struct sockaddr_in *from)
{
	if (from->sin_addr.s_addr != conn->server.s_addr ||
	    dgram->msg_type != MESSAGE_ZLB ||dgram->tid != conn->tunnel_id)
		return -1;

	if (conn->tunnel_state == L2TP_TUNNEL_WAIT_CTL_CONN)
		conn->tunnel_state = L2TP_TUNNEL_ESTABLISHED;
	else if (conn->tunnel_state == L2TP_TUNNEL_SENT_STOP_CCN)
		conn->tunnel_state = L2TP_TUNNEL_IDLE;

	return 0;
}

/**********************************************************************
*  Processes a received StopCCN frame (shuts tunnel down)
***********************************************************************/
static int tunnel_handle_StopCCN(struct l2tp_conn_mngr *conn,
		struct l2tp_dgram *dgram, struct sockaddr_in *from)
{
	__u8 *val;
	__u16 tunnel_id = 0;
	__u16 type, len, vendor;

	if (from->sin_addr.s_addr != conn->server.s_addr)
		return -1;

	if (dgram->msg_type != MESSAGE_StopCCN)
		return -1;
	
	while(1) {
		val = l2tp_pull_avp(dgram, &type, &len, &vendor);
		if (val == NULL) 
			break;
	
		/* Only care about assigned tunnel ID */
		if (vendor != VENDOR_IETF || type != AVP_ASSIGNED_TUNNEL_ID)
			continue;

		tunnel_id = GET_U16(val);
		break;
	}

	if (tunnel_id == 0)
		return -1;

	tunnel_send_ZLB(conn);

	conn->tunnel_state = L2TP_TUNNEL_IDLE;
	conn->session_state = L2TP_SESSION_IDLE;
	
	return 0;
}

void l2tp_tunnel_open(struct l2tp_conn_mngr *conn)
{
	int attempts, timeout;

	attempts = 0;
	timeout = L2TP_INIT_TIMEOUT;

	conn->tunnel_state = L2TP_TUNNEL_IDLE;
 	conn->Ns = 0;
	conn->Nr = 0;
	do {
		attempts++;
		if (attempts > MAX_L2TP_CTRL_ATTEMPTS) {
			l2tp_info("Timeout waiting for SCCRP");
			return;
		}

		tunnel_send_SCCRQ(conn);
		conn->tunnel_state = L2TP_TUNNEL_WAIT_CTL_REPLY;
		l2tp_wait_packet(conn, timeout, tunnel_handle_SCCRP);
		timeout = timeout * 2;
		if (timeout > L2TP_MAX_TIMEOUT)
			timeout = L2TP_MAX_TIMEOUT;
	} while (conn->tunnel_state == L2TP_TUNNEL_WAIT_CTL_REPLY);

	/* 
	  * The retransmitted message contains the same Ns value, but the Nr value 
	  * MUST be updated with the sequence number of the next expected message.
	  */
	conn->Ns++;
	
	attempts	= 0;
	timeout	= L2TP_INIT_TIMEOUT;	
	do {
		attempts++;
		if (attempts > MAX_L2TP_CTRL_ATTEMPTS) {
			l2tp_info("Timeout waiting for SCCRP-ZLB");
			return;
		}

		tunnel_send_SCCCN(conn);
		conn->tunnel_state = L2TP_TUNNEL_WAIT_CTL_CONN;
		l2tp_wait_packet(conn, timeout, tunnel_handle_ZLB);
		timeout = timeout * 2;
		if (timeout > L2TP_MAX_TIMEOUT)
			timeout = L2TP_MAX_TIMEOUT;
	} while (conn->tunnel_state == L2TP_TUNNEL_WAIT_CTL_CONN);

	conn->Ns++;
}

/**********************************************************************
*	Sends SCCRQ to establish a session.
*	The following AVPs MUST be present in the ICRQ:
*		Message Type
*		Assigned Session ID
*		Call Serial Number
***********************************************************************/
static void session_send_ICRQ(struct l2tp_conn_mngr *conn)
{
	__u16 u16_val;
	__u32 u32_val;
	struct l2tp_dgram dgram;

	l2tp_new_control(&dgram, MESSAGE_ICRQ, conn->peer_tunnel, 0);

	u16_val = htons(conn->session_id); /* Assigned session ID */
	l2tp_add_avp(&dgram, MANDATORY, sizeof(u16_val), AVP_ASSIGNED_SESSION_ID, &u16_val);

	u32_val = htonl(conn->call_sernum); /* Call serial number */
	conn->call_sernum++;
	l2tp_add_avp(&dgram, MANDATORY, sizeof(u32_val), AVP_CALL_SERIAL_NUMBER, &u32_val);

	dgram.Ns = conn->Ns;
	dgram.Nr = conn->Nr;

	l2tp_send(conn, &dgram);
}

/**********************************************************************
*	Sends a ICCN control message.
*	The following AVPs MUST be present in the ICCN:
*		Message Type
*		(Tx) Connect Speed
*		Framing Type
***********************************************************************/
static void session_send_ICCN(struct l2tp_conn_mngr *conn)
{
	__u32 u32_val;
	struct l2tp_dgram dgram;

	l2tp_new_control(&dgram, MESSAGE_ICCN, conn->peer_tunnel, conn->peer_session);

	u32_val = htonl(10000000); /* Connect speed */
	l2tp_add_avp(&dgram, MANDATORY, sizeof(u32_val), AVP_TX_CONNECT_SPEED, &u32_val);

	u32_val = htonl(1); /* Framing Type */
	l2tp_add_avp(&dgram, MANDATORY, sizeof(u32_val), AVP_FRAMING_TYPE, &u32_val);

	dgram.Ns = conn->Ns;
	dgram.Nr = conn->Nr;

	l2tp_send(conn, &dgram);
}

/**********************************************************************
*	Handles an ICRP
*	The following AVPs MUST be present in the ICRP:
*		Message Type
*		Assigned Session ID
***********************************************************************/
static int session_handle_ICRP(struct l2tp_conn_mngr *conn,
		struct l2tp_dgram *dgram, struct sockaddr_in *from)
{
	__u16 type, len;
	__u16 vendor, session_id = 0;
	__u8 *val;

	if (dgram->msg_type != MESSAGE_ICRP || dgram->tid != conn->tunnel_id ||
	    dgram->sid != conn->session_id)
		return -1;
	
	conn->Nr = dgram->Ns + 1;

	while (1) {
		val = l2tp_pull_avp(dgram, &type, &len, &vendor);
		if (val == NULL)
			break;

		if (type == AVP_ASSIGNED_SESSION_ID) {
			session_id = GET_U16(val);
			break;
		}
	}

	if (session_id) {
		conn->peer_session = session_id;
		conn->session_state = L2TP_SESSION_WAIT_CONNECT;
		return 0;
	}

	return -1;
}

static int session_handle_ZLB(struct l2tp_conn_mngr *conn,
		struct l2tp_dgram *dgram, struct sockaddr_in *from)
{
	if (from->sin_addr.s_addr != conn->server.s_addr ||
	    dgram->msg_type != MESSAGE_ZLB ||dgram->tid != conn->tunnel_id)
		return -1;

	if (conn->session_state == L2TP_SESSION_WAIT_CONNECT)
		conn->session_state = L2TP_SESSION_ESTABLISHED;

	return 0;
}

void l2tp_session_open(struct l2tp_conn_mngr *conn)
{
	int attempts, timeout;

	attempts = 0;
	timeout = L2TP_INIT_TIMEOUT;
	conn->session_state = L2TP_SESSION_IDLE;
	do {
		attempts++;
		if (attempts > MAX_L2TP_CTRL_ATTEMPTS) {
			l2tp_info("Timeout waiting for ICRP");
			return;
		}

		session_send_ICRQ(conn);
		conn->session_state = L2TP_SESSION_WAIT_REPLY;
		l2tp_wait_packet(conn, timeout, session_handle_ICRP);
		timeout = timeout * 2;
		if (timeout > L2TP_MAX_TIMEOUT)
			timeout = L2TP_MAX_TIMEOUT;
	} while (conn->session_state == L2TP_SESSION_WAIT_REPLY);

	conn->Ns++;
	attempts = 0;
	timeout = L2TP_INIT_TIMEOUT;
	do {
		attempts++;
		if (attempts > MAX_L2TP_CTRL_ATTEMPTS) {
			l2tp_info("Timeout waiting for ICRP-ZLB");
			return;
		}

		session_send_ICCN(conn);
		conn->session_state = L2TP_SESSION_WAIT_CONNECT;
		l2tp_wait_packet(conn, timeout, session_handle_ZLB);
		timeout = timeout * 2;
		if (timeout > L2TP_MAX_TIMEOUT)
			timeout = L2TP_MAX_TIMEOUT;
	} while (conn->session_state == L2TP_SESSION_WAIT_CONNECT);

	conn->Ns++;
}

int l2tp_get_input(struct l2tp_conn_mngr *conn)
{
	struct sockaddr_in from;
	struct l2tp_dgram dgram;

	if (l2tp_get_dgram(conn->ctrl_socket, &dgram, &from) == 0)
		return L2TP_RET_GEN;

	if (dgram.msg_type == MESSAGE_ZLB)
		return L2TP_RET_GEN; /* Maybe ACK to our Hello message. */
	
	//we should check the tunnel id of the packet, if not match, we drop it
	if(dgram.tid != conn->tunnel_id)
		return L2TP_RET_GEN;

	conn->Nr = dgram.Ns + 1;

	switch(dgram.msg_type) {
	case MESSAGE_HELLO:
		tunnel_send_ZLB(conn);
		break;
	case MESSAGE_StopCCN:
		if (tunnel_handle_StopCCN(conn, &dgram, &from) == 0)
			return L2TP_RET_TERM_LINK;
		break;
	default:
		break;
	}

	return L2TP_RET_GEN;
}

void l2tp_handle_timer(struct l2tp_conn_mngr *conn)
{
	tunnel_send_Hello(conn);
}

void l2tp_tunnel_close(struct l2tp_conn_mngr *conn)
{
	int attempts = 0;
	int timeout = L2TP_INIT_TIMEOUT;

	do {
		attempts++; /* 1s is enough. */
		if (attempts > 1) {
			l2tp_info("Timeout waiting for ZLB-ACK");
			return;
		}

		tunnel_send_StopCCN(conn);
		conn->tunnel_state = L2TP_TUNNEL_SENT_STOP_CCN;
		l2tp_wait_packet(conn, timeout, tunnel_handle_ZLB);
	} while (conn->tunnel_state == L2TP_TUNNEL_SENT_STOP_CCN);
}

