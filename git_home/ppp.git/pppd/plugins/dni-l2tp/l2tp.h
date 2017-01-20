#ifndef _L2TP_H_
#define _L2TP_H_

#include <netinet/in.h>
#include <arpa/inet.h>

/* Tunnel states */
enum {
	L2TP_TUNNEL_IDLE,
	L2TP_TUNNEL_WAIT_CTL_REPLY,
	L2TP_TUNNEL_WAIT_CTL_CONN,
	L2TP_TUNNEL_ESTABLISHED,
	L2TP_TUNNEL_RECEIVED_STOP_CCN,
	L2TP_TUNNEL_SENT_STOP_CCN
};

/* Session states */
enum {
	L2TP_SESSION_IDLE,
	L2TP_SESSION_WAIT_TUNNEL,
	L2TP_SESSION_WAIT_REPLY,
	L2TP_SESSION_WAIT_CONNECT,
	L2TP_SESSION_ESTABLISHED
};

#define L2TP_PORT	1701           	/* L2TP UDP port number */

#define MAX_L2TP_CTRL_ATTEMPTS	4
#define L2TP_INIT_TIMEOUT		1
#define L2TP_MAX_TIMEOUT		8

typedef unsigned short	__u16;
typedef unsigned int	__u32;
typedef unsigned char 	__u8;

#define AF_PPPOL2TP		27
#define PF_PPPOL2TP		AF_PPPOL2TP
#define PX_PROTO_L2TP	0 /* L2TP for transmitting PPP data. */

struct l2tp_addr 
{
	int fd;		/* FD of L2TP-UDP socket to use */
	
	__u32 peer_addr;	/* The Server's IP */
	__u16 peer_port;
	__u16 sport;

	/* For matching incoming packets, in network order. */
	__u16 tunnel_id;
	__u16 session_id;

	/* For sending outgoing packets, in network order. */
	__u16 peer_tunnel;
	__u16 peer_session;
};

struct sockaddr_l2tp 
{
	__u16 sa_family;	/* address family, AF_PPPOL2TP */
	__u32 sa_protocol;	/* protocol identifier, PX_PROTO_L2TP */

	struct l2tp_addr addr;
};

struct l2tp_conn_mngr 
{
	int ctrl_socket;
	int data_socket;
	int tunnel_state;
	int session_state;

	/* in host order */
	__u16 tunnel_id;
	__u16 session_id;
	__u16 peer_tunnel;
	__u16 peer_session;

	__u16 Ns;			/* Sequence of next packet to queue */
	__u16 Nr;			/* Expected sequence of next received packet */

	__u32 call_sernum;

	struct in_addr server;
	struct sockaddr_in peer_addr; /* Server address in internet socket format */
};

/* This is return result used by 'l2tp_get_input' for further using. */
#define L2TP_RET_GEN			0
#define L2TP_RET_TERM_LINK	1

extern int l2tp_get_input(struct l2tp_conn_mngr * conn);

extern void l2tp_handle_timer(struct l2tp_conn_mngr * conn);

extern void open_l2tp_callmngr(struct l2tp_conn_mngr *conn);
extern void close_l2tp_callmngr(struct l2tp_conn_mngr * conn);

void l2tp_info(char *, ...);
void l2tp_fatal(char *, ...);

#endif

