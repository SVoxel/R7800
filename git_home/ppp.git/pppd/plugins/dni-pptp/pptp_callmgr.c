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
#include <errno.h>

#include "pptp_msg.h"

#define PPTP_CONN_TIMEOUT	6

static int open_inetsock(struct in_addr inetaddr) 
{
	int s;
	struct sockaddr_in dest;

	memset(&dest, 0, sizeof(struct sockaddr_in));
	dest.sin_family = AF_INET;
	dest.sin_port   = htons(PPTP_PORT);
	dest.sin_addr   = inetaddr;

	if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return s;

	struct timeval tv;
	tv.tv_sec	= PPTP_CONN_TIMEOUT;
	tv.tv_usec	= 0;
	if (setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv)) != 0)
		printf("Can't set the sndtimeo value!\n");
	if (connect(s, (struct sockaddr *) &dest, sizeof(dest)) < 0) {
		close(s); 
		return -1;
	}

	/* Make this socket non-blocking. */
	fcntl(s, F_SETFL, O_NONBLOCK);
	
	return s;
}

void open_callmngr(struct pptp_conn_mngr * conn) 
{
	int s;
	struct sockaddr uaddr;	
	struct sockaddr_in *sin;
	socklen_t ulen;
	

	if ((s = open_inetsock(conn->svr_addr)) < 0) {
		pptp_info("Can't open connection to PPTP Server");
		return;
	}
	conn->call_socket = s;

	ulen = sizeof(uaddr);
	if (getsockname(s, &uaddr, &ulen) != 0) {
		pptp_info("Can't get the sockname %d %m", errno);
		goto err;
	}
	sin = (struct sockaddr_in *)&uaddr;	

	if(conn->call_id == 0)
		conn->call_id = ntohs(sin->sin_port); /* For PPTP ALG problem .... ^_^ */

	pptp_conn_open(conn);	
	if (conn->conn_state != CONN_ESTABLISHED) {
		pptp_info("Can't establish the Control-Connection");
		goto err;
	}

	pptp_call_open(conn);
	if (conn->call_state != CALL_ESTABLISHED) {
		pptp_info("Can't establish the Outgoing-Call-Connection");
		goto err;
	}

	return;
	
err:
	conn->call_socket = -1;
	conn->conn_state = CONN_IDLE;
	conn->call_state = CALL_IDLE;

	close(s);
}

void close_callmngr(struct pptp_conn_mngr * conn) 
{
	pptp_call_close(conn);

	pptp_conn_close(conn);
}

