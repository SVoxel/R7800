#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <signal.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>

#include "pptp_msg.h"

/* The max length of PPTP control packet, 256 seems to be enough. */
#define MAX_PKT_SIZE		256

typedef int (*pptp_call_cb)(struct pptp_conn_mngr *, char *, int);

static void send_ctrl_conn_rqst(struct pptp_conn_mngr * conn)
{
	struct pptp_start_ctrl_conn req;

	memset(&req, 0, sizeof(req));
	
	req.header.length		= hton16(sizeof(req));
	req.header.pptp_type	= hton16(PPTP_MESSAGE_CONTROL);
	req.header.magic		= hton32(PPTP_MAGIC);
	req.header.ctrl_type	= hton16(PPTP_START_CTRL_CONN_RQST);

	req.version		= hton16(PPTP_VERSION);
	req.framing_cap	= hton32(PPTP_FRAME_CAP);
	req.bearer_cap	= hton32(PPTP_BEARER_CAP);
	req.max_channels	= hton16(PPTP_MAX_CHANNELS);
	req.firmware_rev	= hton16(PPTP_FIRMWARE_VERSION);

	strcpy(req.hostname, PPTP_HOSTNAME);
	strcpy(req.vendor, PPTP_VENDOR);
	
	send(conn->call_socket, &req, sizeof(req), 0);	
}

static void send_out_call_rqst(struct pptp_conn_mngr * conn)
{
	struct pptp_out_call_rqst req;

	memset(&req, 0, sizeof(req));
	
	req.header.length		= hton16(sizeof(req));
	req.header.pptp_type	= hton16(PPTP_MESSAGE_CONTROL);
	req.header.magic		= hton32(PPTP_MAGIC);
	req.header.ctrl_type	= hton16(PPTP_OUT_CALL_RQST);

	req.call_id		= hton16(conn->call_id);
	req.call_sernum	= hton16(conn->call_sernum);
	req.bps_min		= hton32(PPTP_BPS_MIN);
	req.bps_max		= hton32(PPTP_BPS_MAX);
	req.bearer		= hton32(PPTP_BEARER_CAP);
	req.framing		= hton32(PPTP_FRAME_CAP);
	req.recv_size		= hton16(PPTP_WINDOW);
	
	send(conn->call_socket, &req, sizeof(req), 0);	
}

static void send_ctrl_conn_stop(struct pptp_conn_mngr * conn, u8 reason) 
{
	struct pptp_stop_ctrl_conn req;

	memset(&req, 0, sizeof(req));
	
	req.header.length		= hton16(sizeof(req));
	req.header.pptp_type	= hton16(PPTP_MESSAGE_CONTROL);
	req.header.magic		= hton32(PPTP_MAGIC);
	req.header.ctrl_type	= hton16(PPTP_STOP_CTRL_CONN_RQST);

	req.reason_result	= hton8(reason);
	
	send(conn->call_socket, &req, sizeof(req), 0);
}

static void send_set_link_info(struct pptp_conn_mngr *conn)
{
	struct pptp_set_link_info pkt;

	memset(&pkt, 0, sizeof(pkt));
	
	pkt.header.length		= hton16(sizeof(pkt));
	pkt.header.pptp_type	= hton16(PPTP_MESSAGE_CONTROL);
	pkt.header.magic		= hton32(PPTP_MAGIC);
	pkt.header.ctrl_type	= hton16(PPTP_SET_LINK_INFO);

	pkt.call_id_peer	= hton16(conn->peer_call_id);
	pkt.send_accm		= 0xFFFFFFFF;
	pkt.recv_accm		= 0xFFFFFFFF;
	
	send(conn->call_socket, &pkt, sizeof(pkt), 0);
}

static void send_call_clear_rqst(struct pptp_conn_mngr *conn)
{
	struct pptp_call_clear_rqst req;

	memset(&req, 0, sizeof(req));

	req.header.length		= hton16(sizeof(req));
	req.header.pptp_type	= hton16(PPTP_MESSAGE_CONTROL);
	req.header.magic		= hton32(PPTP_MAGIC);
	req.header.ctrl_type	= hton16(PPTP_CALL_CLEAR_RQST);

	req.call_id	= hton16(conn->call_id);

	send(conn->call_socket, &req, sizeof(req), 0);
}

static void send_call_clear_ntfy(struct pptp_conn_mngr *conn)
{
	struct pptp_call_clear_ntfy reply;

	memset(&reply, 0, sizeof(reply));

	reply.header.length		= hton16(sizeof(reply));
	reply.header.pptp_type	= hton16(PPTP_MESSAGE_CONTROL);
	reply.header.magic		= hton32(PPTP_MAGIC);
	reply.header.ctrl_type	= hton16(PPTP_CALL_CLEAR_NTFY);

	reply.call_id	= hton16(conn->call_id);

	send(conn->call_socket, &reply, sizeof(reply), 0);
}

static void send_echo_rqst(struct pptp_conn_mngr *conn)
{
	struct pptp_echo_rqst req;

	memset(&req, 0, sizeof(req));

	req.header.length		= hton16(sizeof(req));
	req.header.pptp_type	= hton16(PPTP_MESSAGE_CONTROL);
	req.header.magic		= hton32(PPTP_MAGIC);
	req.header.ctrl_type	= hton16(PPTP_ECHO_RQST);

	req.identifier = hton32(conn->ka_id);

	send(conn->call_socket, &req, sizeof(req), 0);
}

static void send_echo_reply(struct pptp_conn_mngr *conn, u32 identifier)
{
	struct pptp_echo_rply reply;

	memset(&reply, 0, sizeof(reply));

	reply.header.length		= hton16(sizeof(reply));
	reply.header.pptp_type	= hton16(PPTP_MESSAGE_CONTROL);
	reply.header.magic		= hton32(PPTP_MAGIC);
	reply.header.ctrl_type	= hton16(PPTP_ECHO_RPLY);

	reply.identifier = identifier; /* It is in network order now. */
	reply.result_code = hton8(1);
		
	send(conn->call_socket, &reply, sizeof(reply), 0);
}

static int pptp_check_header(struct pptp_header * head, u16 size)
{
	return (ntoh32(head->magic) == PPTP_MAGIC &&
		     ntoh16(head->length) == size &&
		     ntoh16(head->pptp_type) == PPTP_MESSAGE_CONTROL);	
}

void wait_packet(struct pptp_conn_mngr *conn, int timeout, pptp_call_cb callback)
{
	int len;
	fd_set readable;
	struct timeval tv;
	char packet[MAX_PKT_SIZE];
	
	for(; ;) {	
		tv.tv_sec = timeout;
		tv.tv_usec = 0;
	
		FD_ZERO(&readable);
		FD_SET(conn->call_socket, &readable);

		if (select(conn->call_socket + 1, &readable, NULL, NULL, &tv) < 1)
			return; /* Timed out or error happen */

		len = recv(conn->call_socket, &packet, sizeof(packet), 0);

		if (len < 1 ||callback(conn, packet, len) == 0)
			return;
	} 
}

int wait_ctrl_conn_reply(struct pptp_conn_mngr * conn, char * packet, int len)
{
	struct pptp_start_ctrl_conn *reply = (struct pptp_start_ctrl_conn *)packet;
	
	if (len < sizeof(struct pptp_start_ctrl_conn) ||
	     pptp_check_header((struct pptp_header *)packet, (u16)len) == 0 ||
	     ntoh16(reply->header.ctrl_type) != PPTP_START_CTRL_CONN_RPLY ||
	     (ntoh8(reply->result_code) != 1 && ntoh8(reply->error_code) != 0))
		return -1;

	conn->conn_state = CONN_ESTABLISHED;

	return 0;
}

int wait_stop_conn_reply(struct pptp_conn_mngr * conn, char * packet, int len)
{
	struct pptp_stop_ctrl_conn *reply = (struct pptp_stop_ctrl_conn *)packet;
	
	if (len < sizeof(struct pptp_stop_ctrl_conn) ||
	     pptp_check_header((struct pptp_header *)packet, (u16)len) == 0 ||
	     ntoh16(reply->header.ctrl_type) != PPTP_STOP_CTRL_CONN_RPLY)
		return -1;

	conn->conn_state = CONN_IDLE;

	return 0;
}

int wait_out_call_reply(struct pptp_conn_mngr * conn, char * packet, int len)
{
	struct pptp_out_call_rply *reply = (struct pptp_out_call_rply *)packet;
	
	if (len < sizeof(struct pptp_out_call_rply) ||
	     pptp_check_header((struct pptp_header *)packet, (u16)len) == 0 ||
	     ntoh16(reply->header.ctrl_type) != PPTP_OUT_CALL_RPLY ||
	     ntoh16(reply->call_id_peer) != conn->call_id ||
	     reply->result_code != 1)
		return -1;
		
	conn->peer_call_id = ntoh16(reply->call_id);
	conn->call_state = CALL_ESTABLISHED;

	return 0;
}

int wait_call_clear_reply(struct pptp_conn_mngr * conn, char * packet, int len)
{
	struct pptp_call_clear_ntfy *reply = (struct pptp_call_clear_ntfy *)packet;
	
	if (len < sizeof(struct pptp_call_clear_ntfy) ||
	     pptp_check_header((struct pptp_header *)packet, (u16)len) == 0 ||
	     ntoh16(reply->header.ctrl_type) != PPTP_CALL_CLEAR_NTFY ||
	     ntoh16(reply->call_id) != conn->peer_call_id)
		return -1;
		
	conn->call_state = CALL_IDLE;

	return 0;
}

void pptp_conn_open(struct pptp_conn_mngr * conn)
{
	int attempts	= 0;
	int timeout	= PPTP_TIMEOUT;
	
	conn->conn_state	= CONN_IDLE;
 	
	do {
		attempts++;
		if (attempts > MAX_PPTP_CTRL_ATTEMPTS) {
			pptp_info("Timeout waiting for Control-Connection-Reply");
			return;
		}
		
		send_ctrl_conn_rqst(conn);
		conn->conn_state = CONN_WAIT_CTL_REPLY;
		wait_packet(conn, timeout, wait_ctrl_conn_reply);
	} while (conn->conn_state == CONN_WAIT_CTL_REPLY);	
}

void pptp_call_open(struct pptp_conn_mngr * conn)
{
	int attempts	= 0;
	int timeout	= PPTP_TIMEOUT;
	
	conn->call_state = CALL_IDLE;
 		
	do {
		attempts++;
		if (attempts > MAX_PPTP_CTRL_ATTEMPTS) {
			pptp_info("Timeout waiting for Outgoing-Call-Reply");
			return;
		}
		
		send_out_call_rqst(conn);
		conn->call_state = CALL_WAIT_REPLY;
		wait_packet(conn, timeout, wait_out_call_reply);
	} while (conn->call_state == CALL_WAIT_REPLY);

	conn->ka_id = 0;
	send_set_link_info(conn);
}

void pptp_conn_close(struct pptp_conn_mngr * conn)
{
	int attempts	= 0;
	int timeout	= 1;
	
	do {
		attempts++;
		if (attempts > MAX_PPTP_CTRL_ATTEMPTS) {
			pptp_info("Timeout waiting for Stop-Control-Connection-Reply");
			return;
		}

		send_ctrl_conn_stop(conn,PPTP_STOP_NONE);
		conn->conn_state = CONN_WAIT_STOP_REPLY;
		wait_packet(conn, timeout, wait_stop_conn_reply);
	} while (conn->conn_state == CONN_WAIT_STOP_REPLY);	
}

void pptp_call_close(struct pptp_conn_mngr * conn)
{
	int attempts	= 0;
	int timeout	= 1;
 
	do {
		attempts++;
		if (attempts > MAX_PPTP_CTRL_ATTEMPTS) {
			pptp_info("Timeout waiting for Outgoing-Call-Reply");
			return;
		}

		send_call_clear_rqst(conn);
		conn->call_state = CALL_WAIT_DISCONNECT;
		wait_packet(conn, timeout, wait_call_clear_reply);
	} while (conn->call_state == CALL_WAIT_DISCONNECT);
}

int pptp_get_input(struct pptp_conn_mngr * conn)
{
	int len;
	struct pptp_header *hdr;
	char packet[MAX_PKT_SIZE];

	hdr = (struct pptp_header *)packet;
	len = recv(conn->call_socket, &packet, sizeof(packet), 0);

	if (len < 1 || pptp_check_header(hdr, (u16)len) == 0)
		return;

	switch (ntoh16(hdr->ctrl_type)) {
	case PPTP_SET_LINK_INFO:
		send_set_link_info(conn);
		break;

	case PPTP_ECHO_RQST: {
  		struct pptp_echo_rqst *pkt = (struct pptp_echo_rqst *) packet;

		send_echo_reply(conn, pkt->identifier);
		break;
	    }

	case PPTP_CALL_CLEAR_RQST: {
		struct pptp_call_clear_rqst *pkt = (struct pptp_call_clear_rqst *)packet;

		if (ntoh16(pkt->call_id) == conn->peer_call_id) {
			/* WinXP PPTP client is doing so when disconnected by server. */
			send_call_clear_ntfy(conn);
			
			send_ctrl_conn_stop(conn, PPTP_STOP_NONE);

			wait_packet(conn, 2, wait_stop_conn_reply);

			conn->call_state = CALL_IDLE;
			conn->conn_state = CONN_IDLE;
			
			return PPTP_RET_TERM_LINK;
		}
	    }
	}

	return PPTP_RET_GEN;
}

void pptp_handle_timer(struct pptp_conn_mngr * conn)
{
	conn->ka_id ++;

	/* 
	  * Try to send the ECHO-REQUEST to keep the session alive and doesn't 
	  * care the result. Let the PPP/LCP to monitor the session.
	  */
	send_echo_rqst(conn);
}

