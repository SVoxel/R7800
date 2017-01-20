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

#include "l2tp_msg.h"

static int open_l2tp_socket(void) 
{
	int s, flags;
	struct sockaddr_in me;

	memset(&me, 0, sizeof(struct sockaddr_in));
	me.sin_family	= AF_INET;
	me.sin_port	= htons(L2TP_PORT);

	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
		return -1;
	if (bind(s, (struct sockaddr *) &me, sizeof(me)) < 0) {
		l2tp_info("l2tp network init: bind: %s", strerror(errno));
		close(s);
		return -1;
	}

	/* Set socket non-blocking */
	flags = fcntl(s, F_GETFL);
	flags |= O_NONBLOCK;
	fcntl(s, F_SETFL, flags);

	return s;
}

void open_l2tp_callmngr(struct l2tp_conn_mngr *conn) 
{
	int s;
	struct sockaddr_in *peer;

	if ((s = open_l2tp_socket()) < 0) {
		l2tp_info("Can't open socket for L2TP");
		return;
	}
	conn->ctrl_socket = s;

	conn->tunnel_id = (__u16) time(NULL) | 0x0001;
	conn->session_id = (__u16) rand() | 0x0002;

	peer = &conn->peer_addr;
	memset((void *)peer, 0, sizeof(struct sockaddr_in));
	peer->sin_family = AF_INET;
	peer->sin_port = htons(L2TP_PORT);
	peer->sin_addr.s_addr = conn->server.s_addr;

	l2tp_tunnel_open(conn);
	if (conn->tunnel_state != L2TP_TUNNEL_ESTABLISHED) {
		l2tp_info("Can't establish the L2TP Tunnel");
		goto err;
	}

	l2tp_session_open(conn);
	if (conn->session_state != L2TP_SESSION_ESTABLISHED) {
		l2tp_info("Can't establish the L2TP Session");
		goto err;
	}

	return;

err:
	conn->ctrl_socket = -1;
	conn->tunnel_state = L2TP_TUNNEL_IDLE;
	conn->session_state = L2TP_SESSION_IDLE;

	close(s);
}

void close_l2tp_callmngr(struct l2tp_conn_mngr *conn) 
{
	l2tp_tunnel_close(conn);
}

