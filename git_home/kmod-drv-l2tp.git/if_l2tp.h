#ifndef __IF_PPPOL2TP_H
#define __IF_PPPOL2TP_H

#include <asm/types.h>
#include <asm/byteorder.h>

#ifdef  __KERNEL__
#include <linux/if_ether.h>
#include <linux/if.h>
#include <linux/netdevice.h>
#include <linux/sched.h>
#include <linux/ppp_channel.h>
#endif

/* I don't want to touch other perfect code.	--- 2007/09/05 */
#define AF_PPPOL2TP		27
#define PF_PPPOL2TP		AF_PPPOL2TP
#define PX_PROTO_L2TP	0 /* L2TP for transmitting PPP data. */

/*
  * RFC2661:
  * Tunnel ID in each message is that of the intended recipient, not the sender. Tunnel IDs
  * are selected and exchanged as `Assigned Tunnel ID` AVPs during the creation of a tunnel.
  *
  * Session ID in each message is that of the intended recipient, not the sender. Session IDs
  * are selected and exchanged as Assigned Session ID AVPs during the creation of a session.
  */

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
	sa_family_t     sa_family;	/* address family, AF_PPPOL2TP */
	unsigned int    sa_protocol;	/* protocol identifier, PX_PROTO_L2TP */

	struct l2tp_addr addr;
};

#ifdef  __KERNEL__

/* L2TP socket states */
enum 
{
	L2TP_NONE			= 0,  /* initial state */
	L2TP_CONNECTED		= 1,  /* connection established == TCP_ESTABLISHED */
	L2TP_BOUND			= 2,  /* bound to ppp device */
	L2TP_DEAD			= 4  /* dead, useless, please clean me up!*/
};

extern struct ppp_channel_ops l2tp_chan_ops;

struct l2tp_opt
{
	struct sock *udp_sock; /* point to UDP tunnel sock */

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,0)
	struct flowi4 fl;	/* cache lookup keys */
#else
	struct flowi fl;	/* cache lookup keys */
#endif
	struct l2tp_addr pa;
};

struct l2tp_sock 
{
	/* inet_sock has to be the first member */
	struct inet_sock inet;

	struct ppp_channel	chan;
	struct l2tp_opt opt;
};

static inline struct l2tp_sock *l2tp_sk(struct sock *sk)
{
	return (struct l2tp_sock *)sk;
}

static inline struct sock *sk_l2tp(struct l2tp_sock *po)
{
	return (struct sock *)po;
}

#endif

#endif

