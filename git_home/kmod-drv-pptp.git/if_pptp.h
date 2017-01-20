#ifndef __LINUX_IF_PPTP_H
#define __LINUX_IF_PPTP_H

/* Modified for Linux-2.6.15 by DNI's haiyue @ 2007-11-07 keep walking!! */

#include <asm/types.h>
#include <asm/byteorder.h>

#include <linux/semaphore.h>

#include <net/route.h>
#include <net/sock.h>

#include <linux/if.h>
#include <linux/if_ether.h>
#include <linux/netdevice.h>
#include <linux/ppp_channel.h>

/* sock type definition */
#define SOCK_GRE_PPP	8

/* PPTP addressing definition */
struct pptp_addr
{ 
	__u16	call_id;		/* Our Client's Call-ID */
	__u16	peer_callid;	/* The Server's Call-ID */
	
	__u32	peer_addr;	/* The Server's IP */
};
 
struct sockaddr_pptp 
{ 
	sa_family_t		sa_family;            /* address family, AF_INET */ 

	struct pptp_addr	pa;
}; 

#define PPTP_GRE_PROTO  0x880B
#define PPTP_GRE_VER		0x1

#define PPTP_GRE_CSUM		0x80
#define PPTP_GRE_ROUTING	0x40
#define PPTP_GRE_KEY		0x20
#define PPTP_GRE_SEQ		0x10
#define PPTP_GRE_ACK		0x80

struct pptp_hdr
{
	__u8 flags;
	__u8 ver;	

	__u16 protocol;		/* should be PPTP_GRE_PROTO (ppp-encaps) */
	__u16 payload_len;	/* size of ppp payload, not inc. gre header */
	__u16 call_id;			/* peer's call_id for this session */
	
	__u32 seq;		/* sequence number.  Present if S == 1 */
	__u32 ack;		/* seq number of highest packet recieved by sender in this session */
};

/* PPTP socket states */
enum 
{
	PPTP_NONE			= 0,  /* initial state */
	PPTP_CONNECTED		= 1,  /* connection established == TCP_ESTABLISHED */
	PPTP_BOUND			= 2,  /* bound to ppp device */
	PPTP_DEAD			= 4  /* dead, useless, please clean me up!*/
};

extern struct ppp_channel_ops pptp_chan_ops;

struct pptp_opt
{
	struct pptp_addr	pa;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,0)
	struct flowi4		fl;
#else
	struct flowi		fl;
#endif
						
	__u32 ack_snd;
	__u32 seq_snd;
	__u32 seq_rcv;
};

struct pptp_sock 
{
	/* inet_sock has to be the first member */
	struct inet_sock inet;

	struct pptp_sock *next;

	struct ppp_channel	chan;
	struct pptp_opt pptp;
};

static inline struct pptp_sock *pptp_sk(struct sock *sk)
{
	return (struct pptp_sock *)sk;
}

static inline struct sock *sk_pptp(struct pptp_sock *po)
{
	return (struct sock *)po;
}

#endif
