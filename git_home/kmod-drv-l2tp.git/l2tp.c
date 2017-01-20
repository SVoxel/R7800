/*****************************************************************************
 * Linux PPP over L2TP (PPPoX/PPPoL2TP) Sockets
 *
 * Layer 2 Tunneling Protocol
 *
 * PPPoX    --- Generic PPP encapsulation socket family
 * PPPoL2TP --- PPP over L2TP (RFC 2661)
 *
 *
 * Version:    0.16.0
 *
 * 251003 :	Copied from pppoe.c version 0.6.9.
 *
 * Author:	Martijn van Oosterhout <kleptog@svana.org>
 * Contributors:
 *		James Chapman (jchapman@katalix.com)
 *		Michal Ostrowski <mostrows@speakeasy.net>
 *		Arnaldo Carvalho de Melo <acme@xconectiva.com.br>
 *		David S. Miller (davem@redhat.com)
 *
 * License:
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
 */

/* This driver handles only L2TP data frames; control frames are handled by a
 * userspace application.
 *
 * To send data in an L2TP session, userspace opens a PPPoL2TP socket and
 * attaches it to a bound UDP socket with local tunnel_id / session_id and
 * peer tunnel_id / session_id set. Data can then be sent or received using
 * regular socket sendmsg() / recvmsg() calls. Kernel parameters of the socket
 * can be read or modified using ioctl() or [gs]etsockopt() calls.
 *
 * When a PPPoL2TP socket is connected with local and peer session_id values
 * zero, the socket is treated as a special tunnel management socket.
 *
 * Here's example userspace code to create a socket for sending/receiving data
 * over an L2TP session:-
 *
 *	struct sockaddr_pppol2tp sax;
 *	int fd;
 *	int session_fd;
 *
 *	fd = socket(AF_PPPOX, SOCK_DGRAM, PX_PROTO_OL2TP);
 *
 *	sax.sa_family = AF_PPPOX;
 *	sax.sa_protocol = PX_PROTO_OL2TP;
 *	sax.pppol2tp.fd = tunnel_fd;	// bound UDP socket
 *	sax.pppol2tp.pid = 0;		// current pid owns UDP socket
 *	sax.pppol2tp.addr.sin_addr.s_addr = addr->sin_addr.s_addr;
 *	sax.pppol2tp.addr.sin_port = addr->sin_port;
 *	sax.pppol2tp.addr.sin_family = AF_INET;
 *	sax.pppol2tp.s_tunnel  = tunnel_id;
 *	sax.pppol2tp.s_session = session_id;
 *	sax.pppol2tp.d_tunnel  = peer_tunnel_id;
 *	sax.pppol2tp.d_session = peer_session_id;
 *
 *	session_fd = connect(fd, (struct sockaddr *)&sax, sizeof(sax));
 *
 *    [2007/09/05]: Applied it to Linux-2.4.27 for IPv6 L2TP client using.
 *    [2010/03/11]: Applied it to Linux-2.6.15 for Russia L2TP using.
 *    [2011/01/04]: Applied it to Linux-2.6.31.
 *                      ---- Delta Networks Inc
 */

#include <linux/module.h>
#include <linux/version.h>
#include <linux/string.h>
#include <linux/list.h>
#include <asm/uaccess.h>

#include <linux/kernel.h>
#include <linux/spinlock.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/errno.h>

#include <linux/netfilter_ipv4.h>
#include <linux/netdevice.h>
#include <linux/net.h>
#include <linux/inetdevice.h>
#include <linux/skbuff.h>
#include <linux/init.h>
#include <linux/udp.h>
#include <net/sock.h>
#include <linux/ppp_channel.h>
#include <linux/ppp_defs.h>
#include <linux/if_ppp.h>
#include <linux/file.h>
#include <linux/hash.h>
#include <linux/proc_fs.h>
#include <net/dst.h>
#include <net/ip.h>
#include <net/udp.h>
#include <net/xfrm.h>

#include <asm/byteorder.h>
#include <asm/atomic.h>


#include "if_l2tp.h"

#define PPPOL2TP_DRV_VERSION	"V0.16.1"

#if 0
#define L2TP_DEBUGP(format, args...) printk(format, ## args)
#else
#define L2TP_DEBUGP(format, args...)
#endif

/* L2TP header constants */
#define L2TPHDR_MSG_TYPE	0x8000
#define L2TPHDR_LENGTH		0x4000
#define L2TPHDR_SEQUENCE	0x0800
#define L2TPHDR_OFFSET		0x0200
#define L2TPHDR_PRIORITY	0x0100

#define L2TPHDR_VER_MASK	0x000F
#define L2TPHDR_VERSION		0x0002


/* send 'offset size = 0' instead of 'length' field as Cisco server does for data message. */
#define L2TP_FLAG_OFFSET	0x0200
#define L2TP_FLAG_VERSION	0x0002

struct l2tphdr
{
	__u16 flags;
	__u16 tunnel;
	__u16 session;
	__u16 offset;
};

/* IP/UDP/L2TP/PPP Headers */
#define L2TP_RESV_HDRLEN (sizeof(struct iphdr) + sizeof(struct udphdr) + sizeof(struct l2tphdr) + 4)


struct l2tp_tunnel
{
	struct l2tp_sock *tunnel_sock;

	/* saved the original call-back function address. */
	void (*sk_data_ready)(struct sock *, int);
	void (*sk_destruct)(struct sock *);
};


/* Internal receive frame. Do the real work of receiving an L2TP data frame
  * here.
  * Returns 0 if the packet was a data packet and was successfully passed on.
  * Returns 1 if the packet was not a good data packet and could not be
  * forwarded.  All such packets are passed up to userspace to deal with.
  */
static int l2tp_recv_core(struct l2tp_tunnel *tunnel, struct sk_buff *skb)
{
	__u16 hdrflags;
	__u16 tunnel_id;
	__u16 session_id;

	unsigned char *ptr;
	struct l2tp_sock *sk;

	if (!pskb_may_pull(skb, 16))
		goto discard;

	ptr = skb->data + sizeof(struct udphdr);	/* Point to L2TP header */
	hdrflags = ntohs(*(__u16*)ptr);			/* Get L2TP header flags */

	if (hdrflags & L2TPHDR_MSG_TYPE) {
		L2TP_DEBUGP("Rcv'd a L2TP control message!\n");
		return 1;
	}

	ptr += 2;	/* Skip flags */
	sk = tunnel->tunnel_sock;

	if (hdrflags & L2TPHDR_LENGTH)
		ptr += 2;	/* If length is present, skip it */

	/* Extract tunnel and session ID */
	tunnel_id = *(__u16 *)ptr;
	ptr += 2;
	session_id = *(__u16 *)ptr;
	ptr += 2;
	if (tunnel_id != sk->opt.pa.tunnel_id || session_id != sk->opt.pa.session_id) {
		L2TP_DEBUGP("Tunnel or session ID of L2TP data message NOT matched!\n");
		return 1;
	}

	if (hdrflags & L2TPHDR_SEQUENCE)
		ptr += 4;
	if (hdrflags & L2TPHDR_OFFSET)
		ptr += 2 + ntohs(*(__u16 *) ptr);

	__skb_pull(skb, ptr - skb->data);

	if (skb->data[0] == 0xFF && skb->data[1] == 0x03)
		__skb_pull(skb, 2);

	/* Pass this skb to ppp channel */
	sock_hold(sk_l2tp(sk));
	skb_orphan(skb);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,31)
	skb_dst_drop(skb);
#else
	dst_release(skb->dst);
	skb->dst = NULL;
#endif

	skb->dev = NULL;
	skb->pkt_type = 0;
	ppp_input(&sk->chan, skb);
	sock_put(sk_l2tp(sk));

	return 0;

discard:
	kfree_skb(skb);
	return 0;
}

/* 
  * The data_ready hook on the UDP socket. Scan the incoming packet list for
  * packets to process. Only control packets are delivered to userspace.
  */
static void l2tp_data_ready(struct sock *sk, int len)
{
	struct sk_buff *skb;
	struct l2tp_tunnel *tunnel;

	/* sock_queue_rcv_skb => skb_queue_tail => sk->sk_data_ready(sk, skb_len) */
	skb = skb_dequeue_tail(&sk->sk_receive_queue);
	if (skb == NULL)
		return;
	tunnel = sk->sk_user_data;
	if (tunnel == NULL)
		return;
	if (l2tp_recv_core(tunnel, skb)) {
		skb_queue_head(&sk->sk_receive_queue, skb);

		tunnel->sk_data_ready(sk, len);
	}
}

/* Tunnel UDP socket destruct hook. */
static void l2tp_tunnel_destruct(struct sock *sk)
{
	struct sock *l2tp_sock;
	struct l2tp_tunnel *tunnel;
	tunnel = (struct l2tp_tunnel *)sk->sk_user_data;

	write_lock_bh(&sk->sk_callback_lock);
	sk->sk_data_ready = tunnel->sk_data_ready;
	sk->sk_destruct = tunnel->sk_destruct;
	sk->sk_user_data = NULL;
	write_unlock_bh(&sk->sk_callback_lock);

	l2tp_sock = sk_l2tp(tunnel->tunnel_sock);
	write_lock_bh(&l2tp_sock->sk_callback_lock);
	tunnel->tunnel_sock->opt.udp_sock = NULL;
	write_unlock_bh(&l2tp_sock->sk_callback_lock);

	kfree(tunnel);
}

 /* 
   * Internal function to prepare a tunnel (UDP) socket to have PPPoL2TP sockets
   * attached to it.
   */
static struct sock *l2tp_get_tunnel(struct sock *l2tp_sock, int fd, int *error)
{
	int err;
	struct sock *tunnel_sock;
	struct socket *tunnel_socket;
	struct l2tp_tunnel *tunnel;

	*error = -EBADF;
	if ((tunnel_socket = sockfd_lookup(fd, &err)) == NULL)
		return NULL;

	tunnel_sock = NULL;
	err = -ESOCKTNOSUPPORT;
	if (tunnel_socket->type != SOCK_DGRAM)
		goto out;

	err = -EAFNOSUPPORT;
	if (tunnel_socket->ops->family != AF_INET)
		goto out;

	tunnel_sock = tunnel_socket->sk;
	tunnel_sock->sk_user_data = tunnel = kmalloc(sizeof(struct l2tp_tunnel), GFP_KERNEL);
	if (tunnel == NULL) {
		err = -ENOMEM;
		tunnel_sock = NULL;
		goto out;
	}

	tunnel->tunnel_sock = l2tp_sk(l2tp_sock);
	tunnel->sk_data_ready = tunnel_sock->sk_data_ready;
	tunnel->sk_destruct = tunnel_sock->sk_destruct;
	write_lock_bh(&tunnel_sock->sk_callback_lock);
	tunnel_sock->sk_data_ready	= l2tp_data_ready;
	tunnel_sock->sk_destruct	= l2tp_tunnel_destruct;
	tunnel_sock->sk_allocation = GFP_ATOMIC;
	write_unlock_bh(&tunnel_sock->sk_callback_lock);

	err = 0;
out:
	*error = err;

	sockfd_put(tunnel_socket);

	return tunnel_sock;
}

/*********************************************************************
  *	 		Transmit data from PPP channel
  *********************************************************************/
static int l2tp_xmit(struct ppp_channel *chan, struct sk_buff *skb)
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,4,0)
	int err;
#endif
	__u16 *p;

	struct iphdr *iph;
	struct udphdr *uh;
	struct l2tphdr *lh;

	struct sk_buff *skb2;
	struct rtable *rt = NULL;
	struct sock *sk = chan->private;
	struct l2tp_opt *po = &(l2tp_sk(sk)->opt);

	if (sock_flag(sk, SOCK_DEAD)  || !(sk->sk_state & L2TP_CONNECTED))
		goto done;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,0)
	rt = ip_route_output_key(dev_net(skb->dev), &po->fl);
	if (IS_ERR(rt)) {
		rt = NULL;
		goto done;
	}
	if (rt->rt_flags & (RTCF_MULTICAST |RTCF_BROADCAST))
#else
	err = ip_route_output_key(dev_net(skb->dev), &rt, &po->fl);
	if (err ||rt->rt_flags & (RTCF_MULTICAST |RTCF_BROADCAST))
#endif
		goto done;

	if (skb_headroom(skb) < L2TP_RESV_HDRLEN) {
		if ((skb2 = dev_alloc_skb(skb->len + L2TP_RESV_HDRLEN)) == NULL)
			goto done;
		L2TP_DEBUGP("L2TP: The head room size of sk_buff is not enough!\n");

		skb_reserve(skb2, L2TP_RESV_HDRLEN);
		memcpy(skb_put(skb2, skb->len), skb->data, skb->len);
		kfree_skb(skb);
	} else {
		nf_reset(skb);
		skb_orphan(skb);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,31)
		skb_dst_drop(skb);
#else
		dst_release(skb->dst);
		skb->dst = NULL;
#endif

		skb2 = skb;
	}

	/* Add PPP's address and control fields */
	p = (__u16 *) __skb_push(skb2, 2); *p = __constant_htons(0xFF03);

	lh = (struct l2tphdr *) __skb_push(skb2, sizeof(struct l2tphdr));
	lh->flags = __constant_htons(L2TP_FLAG_OFFSET |L2TP_FLAG_VERSION);
	lh->tunnel = po->pa.peer_tunnel;
	lh->session = po->pa.peer_session;
	lh->offset = 0;

	uh = (struct udphdr *) __skb_push(skb2, sizeof(struct udphdr));
	skb_reset_transport_header(skb2);
	uh->source = po->pa.sport;
	uh->dest = po->pa.peer_port;
	uh->len = htons(skb2->len);
	uh->check = 0;	/* None Checksum */

	iph = (struct iphdr *) __skb_push(skb2, sizeof(struct iphdr));
	skb_reset_network_header(skb2);
	iph->version = 4;
	iph->ihl = 5;
	iph->tos = 0;
	iph->tot_len = htons(skb2->len);
	iph->frag_off = 0;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,36)
	ip_select_ident(skb, NULL);
#else
	ip_select_ident(iph, &rt->u.dst, NULL);
#endif
	iph->ttl = 128;
	iph->protocol = IPPROTO_UDP;
#ifdef CONFIG_IP_NF_IP_ALIGNMENT
	u32_set(&iph->saddr, &rt->rt_src);
	u32_set(&iph->daddr, &rt->rt_dst);
#else
	iph->saddr = rt->rt_src;
	iph->daddr = rt->rt_dst;
#endif
	iph->check = 0;
	iph->check = ip_fast_csum((unsigned char *)iph, 5);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,36)
	skb_dst_set(skb2, dst_clone(&rt->dst));

	NF_HOOK(PF_INET, NF_INET_LOCAL_OUT, skb2, NULL, rt->dst.dev, dst_output);
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,31)
	skb_dst_set(skb2, dst_clone(&rt->u.dst));

	NF_HOOK(PF_INET, NF_INET_LOCAL_OUT, skb2, NULL, rt->u.dst.dev, dst_output);
#else
	skb2->dst = dst_clone(&rt->u.dst);

	NF_HOOK(PF_INET, NF_INET_LOCAL_OUT, skb2, NULL, rt->u.dst.dev, dst_output);
#endif

	ip_rt_put(rt);

	return 1;

done:
	ip_rt_put(rt);
	
	return 0;
}

struct ppp_channel_ops l2tp_chan_ops = { 
	.start_xmit	= l2tp_xmit, 
	.ioctl			= NULL,
};

/**********************************************************************/

static int l2tp_connect(struct socket *sock, struct sockaddr *uaddr, int addr_len, int flags)
{
	int err;
	struct rtable *rt = NULL;
	struct ppp_channel	*chan;
	struct sock *udp_sk, *sk = sock->sk;
	struct l2tp_opt *po = &(l2tp_sk(sk)->opt);
	struct sockaddr_l2tp *sp = (struct sockaddr_l2tp *) uaddr;

	lock_sock(sk);

	err = -EINVAL;
	if (sp->sa_family != AF_PPPOL2TP ||sp->sa_protocol != PX_PROTO_L2TP)
		goto end;

	err = -EBUSY;
	if (sk->sk_state & L2TP_CONNECTED)
		goto end;

	udp_sk = l2tp_get_tunnel(sk, sp->addr.fd, &err);
	if (udp_sk == NULL)
		goto end;

	po->udp_sock= udp_sk;
	po->pa = sp->addr;

	err = -ENETUNREACH;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,0)
	po->fl.daddr = sp->addr.peer_addr;
	po->fl.flowi4_proto = IPPROTO_UDP;
	rt = ip_route_output_key(&init_net, &po->fl);
	if (IS_ERR(rt))
#else
	po->fl.fl4_dst = sp->addr.peer_addr;
	po->fl.proto = IPPROTO_UDP;
	if (ip_route_output_key(&init_net, &rt, &po->fl))
#endif
		goto end;
	/*
	 * Save the output Ethernet interface index for route selection.
	 * [__mkroute_output]: rth->rt_iif = oldflp->oif ? : dev_out->ifindex;
	 */
	L2TP_DEBUGP("L2TP: The output interface index is %d.\n", rt->rt_iif);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,0)
	po->fl.flowi4_oif = rt->rt_iif;
#else
	po->fl.oif = rt->rt_iif;
#endif

	chan = &(l2tp_sk(sk)->chan);
	chan->hdrlen = L2TP_RESV_HDRLEN;
	chan->private = sk;
	chan->ops = &l2tp_chan_ops;
	err = ppp_register_channel(chan);
	if (err == 0)
		sk->sk_state = L2TP_CONNECTED;

end:
	ip_rt_put(rt);
	release_sock(sk);

	return err;
}

/**********************************************************/

static void l2tp_unbind_sock(struct sock *sk)
{
	if (sk->sk_state & (L2TP_CONNECTED |L2TP_BOUND))
		ppp_unregister_channel(&(l2tp_sk(sk)->chan));

	sk->sk_state = L2TP_DEAD;
}

static void l2tp_sock_destruct(struct sock *sk)
{
	struct l2tp_opt *po = &(l2tp_sk(sk)->opt);

	if (po->udp_sock)
		l2tp_tunnel_destruct(po->udp_sock);
}

/**********************************************************/

static int l2tp_release(struct socket *sock)
{
	struct sock *sk;

	if (!(sk = sock->sk))
		return 0;

	if (sock_flag(sk, SOCK_DEAD))
		return -EBADF;

	l2tp_unbind_sock(sk);

	sock_orphan(sk);
	sock->sk = NULL;

	sock_put(sk);

	return 0;
}

static int l2tp_backlog_rcv(struct sock *sk, struct sk_buff *skb)
{
	kfree_skb(skb);

	return NET_RX_DROP;
}

static int l2tp_getname(struct socket *sock, struct sockaddr *uaddr,
		int *usockaddr_len, int peer)
{
	int err;
	struct sock *sk;
	struct l2tp_sock *po;
	struct sockaddr_l2tp sp;

	err = -ENOTCONN;
	sk = sock->sk;
	po = l2tp_sk(sk);
	if (!(sk->sk_state & L2TP_CONNECTED))
		goto end;

	sp.sa_family	= AF_PPPOL2TP;
	sp.sa_protocol	= PX_PROTO_L2TP;
	memcpy(&sp.addr, &po->opt.pa, sizeof(struct l2tp_addr));
	memcpy(uaddr, &sp, sizeof(struct sockaddr_l2tp));

	*usockaddr_len = sizeof(struct sockaddr_l2tp);

	err = 0;

end:
	return err;
}

static int l2tp_sendmsg(struct kiocb *iocb, struct socket *sock, struct msghdr *m, size_t size)
{
	return 0;
}

static int l2tp_recvmsg(struct kiocb *iocb, struct socket *sock, struct msghdr *m, size_t size, int flags)
{
	return 0;
}

static int l2tp_ioctl(struct socket *sock, unsigned int cmd, unsigned long arg)
{
	int err = 0;
	struct sock *sk = sock->sk;
	struct l2tp_sock *po = l2tp_sk(sk);

	switch (cmd) {
	case PPPIOCGCHAN: {
		int index;
		err = -ENOTCONN;
		if (!(sk->sk_state & L2TP_CONNECTED))
			break;

		err = -EINVAL;
		index = ppp_channel_index(&po->chan);
		if (put_user(index , (int *) arg))
			break;

		err = 0;
		sk->sk_state |= L2TP_BOUND;
		break;
		}
	default:
		break;
	}

	return err;
}

static struct proto_ops l2tp_ops = {
	.family	= AF_PPPOL2TP,
	.owner	= THIS_MODULE,
	.poll			= datagram_poll,
	.bind			= sock_no_bind,
	.socketpair	= sock_no_socketpair,
	.accept		= sock_no_accept,
	.listen		= sock_no_listen,
	.shutdown	= sock_no_shutdown,
	.setsockopt	= sock_no_setsockopt,
	.getsockopt	= sock_no_getsockopt,
	.mmap		= sock_no_mmap,

	/* L2TP supporting */
	.release		= l2tp_release,
	.connect		= l2tp_connect,
	.getname		= l2tp_getname,
	.sendmsg		= l2tp_sendmsg,
	.recvmsg		= l2tp_recvmsg,
	.ioctl			= l2tp_ioctl,
};

/**********************************************************************************/

static struct proto l2tp_sk_proto = {
	.name	= "L2TP",
	.owner	= THIS_MODULE,
	.obj_size	= sizeof(struct l2tp_sock),
};

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,0)
static int l2tp_create(struct net *net, struct socket *sock, int protocol, int kern)
#else
static int l2tp_create(struct net *net, struct socket *sock, int protocol)
#endif
{
	struct sock *sk;
	int err = -ENOMEM;

	if (protocol != PX_PROTO_L2TP)
		return -EPROTOTYPE;

	try_module_get(THIS_MODULE);

	sk = sk_alloc(net, PF_PPPOL2TP, GFP_KERNEL, &l2tp_sk_proto);
	if (!sk)
		goto out;

	sock_init_data(sock, sk);

	sock->state  = SS_UNCONNECTED;
	sock->ops    = &l2tp_ops;

	sk->sk_protocol    = PX_PROTO_L2TP;
	sk->sk_family      = PF_PPPOL2TP;
	sk->sk_state       = L2TP_NONE;
	sk->sk_type        = SOCK_STREAM;
	sk->sk_backlog_rcv = l2tp_backlog_rcv;
	sk->sk_destruct    = l2tp_sock_destruct;

	err = 0;

out:
	module_put(THIS_MODULE);

	return err;
}

static struct net_proto_family l2tp_proto_family = {
	.family	= PF_PPPOL2TP,
	.create	= l2tp_create,
};

int __init l2tp_init(void)
{
	int err;

	err = proto_register(&l2tp_sk_proto, 0);
	if (err)
		goto out;

	err = sock_register(&l2tp_proto_family);
	if (err)
		goto out_unregister_proto;

	printk(KERN_INFO "PPPoL2TP kernel driver, %s\n", PPPOL2TP_DRV_VERSION);

out:
	return err;

out_unregister_proto:
	proto_unregister(&l2tp_sk_proto);
	goto out;
}

void __exit l2tp_exit(void)
{
	proto_unregister(&l2tp_sk_proto);

	sock_unregister(PF_PPPOL2TP);
}

module_init(l2tp_init);
module_exit(l2tp_exit);

MODULE_AUTHOR("Martijn van Oosterhout <kleptog@svana.org>");
MODULE_DESCRIPTION("PPP over L2TP over UDP");
MODULE_LICENSE("GPL");
MODULE_VERSION(PPPOL2TP_DRV_VERSION);

