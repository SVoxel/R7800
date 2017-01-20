#ifndef _PPTP_H_
#define _PPTP_H_

/* 
  * Sorry, I don't like Configuring file, for I always fail to configure them. :) 
  * For an embedded system, simple is the best!	-- DNI's haiyue.
  */

#include <netinet/in.h>
#include <arpa/inet.h>

/* Call-Management Phase States */
enum {
	CONN_IDLE, 
	CONN_WAIT_CTL_REPLY, 
	CONN_WAIT_STOP_REPLY, 
	CONN_ESTABLISHED,
};

enum {
	CALL_IDLE, 
	CALL_WAIT_REPLY, 
	CALL_ESTABLISHED, 
	CALL_WAIT_DISCONNECT
};

#define MAX_PPTP_MTU			1450
#define MAX_PPTP_CTRL_ATTEMPTS	3
#define PPTP_TIMEOUT				3

typedef unsigned short	u16;
typedef unsigned int	u32;
typedef unsigned char 	u8;

/* sock type definition */
#define SOCK_GRE_PPP	8

/* PPTP addressing definition */
struct pptp_addr
{ 
	u16	call_id;		/* Our Client's Call-ID */
	u16	peer_callid;	/* The Server's Call-ID */
	
	u32	peer_addr;	/* The Server's IP */
};
 
struct sockaddr_pptp 
{ 
	u16		sa_family;            /* address family, AF_INET */ 

	struct pptp_addr	pa;
}; 

struct pptp_conn_mngr 
{
	int conn_state;
	int call_state;
	
	int call_socket;
	int gre_socket;

	/* Keep-alive ID; monotonically increasing (watch wrap-around!)  on startup: 1  */
	u32 ka_id;
	
	u16 call_sernum;
	u16 call_id;
	u16 peer_call_id;
	
	struct in_addr svr_addr;
};

void open_callmngr(struct pptp_conn_mngr *conn);
void close_callmngr(struct pptp_conn_mngr *conn);

/* This is return result used by 'pptp_get_input' for further using. */
#define PPTP_RET_GEN			0
#define PPTP_RET_TERM_LINK	1
int pptp_get_input(struct pptp_conn_mngr * conn);

void pptp_handle_timer(struct pptp_conn_mngr * conn);

void pptp_info(char *, ...);
void pptp_fatal(char *, ...);

#endif
