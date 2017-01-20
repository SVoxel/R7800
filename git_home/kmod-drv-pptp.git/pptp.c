/*
  * Copyright (C) 2007 Delta Networks Inc.
  *
  * Much code and many ideas derived from PPPoE/PPPoX & UDP/RAW/TCP socket.
  *
  * The PPTPD processes GRE-PPP packets of the Ethernet and forwards them to PPP
  * driver over a pseudo-terminal (PTY-PTM), then the PPP driver forwards them to IP
  * networking stack for further processing. The major drawback of this approach is 
  * that the intermediate pseudo-terminal requires an additional transition between 
  * the kernel and the user space, which reduces the performance considerably. Our 
  * Router LAN <--> WAN throughput is ONLY about 5 Mbps. For this reason, we took
  * the PPPoE for reference to implement our new PPTP-GRE-PPP packets processing.
  *
  */

#include <linux/string.h>
#include <linux/module.h>
#include <linux/version.h>

#include <asm/uaccess.h>

#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/errno.h>

#include <linux/netdevice.h>
#include <linux/net.h>
#include <linux/inetdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>
#include <linux/init.h>
#include <linux/if_ether.h>
#include <linux/netfilter_ipv4.h>
#include <net/checksum.h>
#include <net/ip.h>
#include <net/sock.h>
#include <net/route.h>
#include <net/protocol.h>
#include <linux/ppp_channel.h>
#include <linux/ppp_defs.h>
#include <linux/if_ppp.h>
#include <linux/notifier.h>
#include <linux/file.h>
#include <linux/proc_fs.h>
#include <linux/netfilter.h>

#include "if_pptp.h"

#if 0
#define DEBUGP printk
#else
#define DEBUGP(format, args...)
#endif

#define PPTP_HASH_SIZE		16
#define RESV_HDR_LEN  (sizeof(struct pptp_hdr) + sizeof(struct iphdr) + 16)

int pptp_ioctl(struct socket *skt, unsigned int cmd, unsigned long arg);
int pptp_xmit(struct ppp_channel *chan, struct sk_buff *skb);
int __pptp_xmit(struct sock *sk, struct sk_buff *skb);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,0)
static rwlock_t pptp_hash_lock = __RW_LOCK_UNLOCKED(pptp_hash_lock);
#else
static rwlock_t pptp_hash_lock = RW_LOCK_UNLOCKED;
#endif

struct ppp_channel_ops pptp_chan_ops = { 
	.start_xmit	= pptp_xmit,
};

static inline int cmp_pptp_addr(struct pptp_addr *a, u32 addr, u16 cid)
{
	return (a->peer_addr == addr && a->call_id == cid);
}

static u32 hash_pptp_item(u32 addr, u16 cid)
{
	u32 h0 = (addr ^ (addr >> 4)) & 0xF;
	u32 h1 = (cid ^ (cid >> 4)) & 0xF;
	
	return (h0 ^ h1);
}

static struct pptp_sock  *pptp_item_table[PPTP_HASH_SIZE];

static int __set_pptp_item(struct pptp_sock *po)
{
	u32 hash = hash_pptp_item(po->pptp.pa.peer_addr, po->pptp.pa.call_id);
	struct pptp_sock *ret = pptp_item_table[hash];
	
	while (ret) {
		if (cmp_pptp_addr(&ret->pptp.pa, po->pptp.pa.peer_addr, 
				po->pptp.pa.call_id))
			return -EALREADY;

		ret = ret->next;
	}

	po->next = pptp_item_table[hash];
	pptp_item_table[hash] = po;
	
	return 0;
}

static struct pptp_sock *__get_pptp_item(u32 addr, u16 cid)
{
	u32 hash = hash_pptp_item(addr, cid);
	struct pptp_sock *ret = pptp_item_table[hash];

	while (ret && !cmp_pptp_addr(&ret->pptp.pa, addr, cid))
		ret = ret->next;

	return ret;
}

static struct pptp_sock *__delete_pptp_item(u32 addr, u16 cid)
{
	u32 hash = hash_pptp_item(addr, cid);
	struct pptp_sock *ret, **pprev;

	ret = pptp_item_table[hash];
	pprev = &pptp_item_table[hash];

	while (ret) {
		if (cmp_pptp_addr(&ret->pptp.pa, addr, cid)) {
			*pprev = ret->next;
			break;
		}

		pprev = &ret->next;
		ret = ret->next;
	}

	return ret;
}

static inline int set_pptp_item(struct pptp_sock *po)
{
	int i;

	if (!po)
		return -EINVAL;

	write_lock_bh(&pptp_hash_lock);
	i = __set_pptp_item(po);
	write_unlock_bh(&pptp_hash_lock);

	return i;
}

static inline struct pptp_sock *get_pptp_item(u32 addr, u16 cid)
{
	struct pptp_sock *po;

	read_lock_bh(&pptp_hash_lock);
	po = __get_pptp_item(addr, cid);
	if (po)
		sock_hold(sk_pptp(po));
	read_unlock_bh(&pptp_hash_lock);

	return po;
}

static inline struct pptp_sock *delete_pptp_item(u32 addr, u16 cid)
{
	struct pptp_sock *ret;

	write_lock_bh(&pptp_hash_lock);
	ret = __delete_pptp_item(addr, cid);
	write_unlock_bh(&pptp_hash_lock);

	return ret;
}

/********************************************************
  *
  * Do the real work of receiving a GRE-PPP frame from IP networking stack
  *
  ********************************************************/
static int __pptp_recv(struct pptp_sock *po, struct sk_buff *skb)
{
	struct pptp_hdr *ph;
	u8 flags, ver;

	if (!pskb_may_pull(skb, sizeof(struct pptp_hdr) + 2))
		goto drop;

	ph = (struct pptp_hdr *) skb_transport_header(skb);
	
	flags = ph->flags; ver = ph->ver;

	if ((flags & 0xEF) != PPTP_GRE_KEY ||(ver & 0x7F) != PPTP_GRE_VER ||
	     (ph->protocol != __constant_htons(PPTP_GRE_PROTO))) 
		goto drop;

	if (flags & PPTP_GRE_SEQ) {
		u32 hdr_size, seqno = ntohl(ph->seq);
		u16 * p;
		
		if (po->pptp.seq_rcv && (s32)(seqno - po->pptp.seq_rcv) < 0)
			goto drop;
			
		if ((ver & PPTP_GRE_ACK) == 0) 
			hdr_size = sizeof(struct pptp_hdr) - sizeof(__u32);
		else
			hdr_size = sizeof(struct pptp_hdr);
			
		p = (void *)ph + hdr_size;
		if (*p == __constant_htons(0xFF03)) /* PPP Control Flags */ 
			hdr_size += 2;

		/********************************************************************
		  *
		  * PPTP-RFC 2637:
		  * This protocol allows acknowledgments to be carried with the data and makes the 
		  * overall protocol more efficient, which in turn requires less buffering of packets. The 
		  * sliding window protocol used on the PPTP data path is used for flow control by each 
		  * side of the data exchange.  The enhanced GRE protocol allows packet acknowledg-
		  * ments to be piggybacked on data packets.  Acknowledgments can also be sent sep-
		  * arately from data packets.  Again, the main purpose of the sliding window protocol is
		  * for flow control--retransmissions are not performed by the tunnel peers.
		  * Simple is the best, I just send the ACK next transmitting time for performance:-)
		  *
		  *										---- DNI' haiyue @ 2007-05-30
		  ********************************************************************/
		po->pptp.seq_rcv = seqno;
				
		__skb_pull(skb, hdr_size);

		/* Reset the routing etc. information. */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,31)
		skb_dst_drop(skb);
#else
		dst_release(skb->dst);
		skb->dst = NULL;
#endif
		nf_reset(skb);
		skb->dev = NULL;
		skb->pkt_type = 0;
		
		ppp_input(&po->chan, skb);
	}
	else
		kfree_skb(skb); /* This is ONLY a ACk */
	
	return NET_RX_SUCCESS;

drop:
	kfree_skb(skb);

	return NET_RX_DROP;
}

int pptp_rcv_core(struct sock *sk, struct sk_buff *skb)
{
	if (sk->sk_state & PPTP_BOUND)
		return __pptp_recv(pptp_sk(sk), skb); 

	kfree_skb(skb);
	
	return NET_RX_DROP;
}

int pptp_rcv(struct sk_buff *skb)
{
	int ret;
	struct sock *sk;
	struct iphdr *iph;
	struct pptp_hdr *ph;
	struct pptp_sock *po;
	
	if (!pskb_may_pull(skb, sizeof(struct pptp_hdr)))
		goto drop;

	iph = (struct iphdr *)skb_network_header(skb);
	ph = (struct pptp_hdr *) skb_transport_header(skb);
	po = get_pptp_item(iph->saddr, ph->call_id);
	if (!po)
		goto drop;
	
	sk = sk_pptp(po);
	bh_lock_sock(sk);
	
	if (sock_owned_by_user(sk) != 0) {
		sk_add_backlog(sk, skb);
		ret = NET_RX_SUCCESS;
	} else {
		ret = __pptp_recv(po, skb);
	}
	
	bh_unlock_sock(sk);
	sock_put(sk);

	return ret;
	
drop:
	kfree_skb(skb);

	return NET_RX_DROP;
}

void pptp_unbind_sock(struct sock *sk)
{
	if (sk->sk_state & PPTP_BOUND) {
		DEBUGP("Unregister the PPP channel\n");
		
		ppp_unregister_channel(&pptp_sk(sk)->chan);

		sk->sk_state = PPTP_DEAD;
	}
}

void pptp_sock_destruct(struct sock *sk)
{
	dst_release(sk->sk_dst_cache);
}

int pptp_init_sock(struct sock *sk)
{
	sk->sk_state = PPTP_NONE;

	sk->sk_destruct = pptp_sock_destruct;
	
	return 0;
}

void pptp_close(struct sock *sk, long timeout)
{
	struct pptp_sock *po;
	
	if (sk->sk_prot->destroy)
		sk->sk_prot->destroy(sk);

	pptp_unbind_sock(sk);

	sk->sk_state = PPTP_DEAD;

	po = pptp_sk(sk);
	if (po->pptp.pa.call_id)
		delete_pptp_item(po->pptp.pa.peer_addr, po->pptp.pa.call_id);

	sock_orphan(sk);
	skb_queue_purge(&sk->sk_receive_queue);
	sock_put(sk);
}

int pptp_release(struct socket *skt)
{
	struct sock *sk = skt->sk;
	
	if (!sk)
		return 0;

	if (sock_flag(sk, SOCK_DEAD))
		return -EBADF;

	skt->sk = NULL;
	sk->sk_prot->close(sk, 0);
	
	return 0;
}

int pptp_disconnect(struct sock *sk, int flags)
{
	sk->sk_state = PPTP_NONE;

	sk_dst_reset(sk);

	return 0;
}

int pptp_connect(struct sock *sk, struct sockaddr * uaddr, int addr_len)
{
	int err;
	struct rtable *rt = NULL;
	struct pptp_sock *po = pptp_sk(sk);
	struct sockaddr_pptp *usin = (struct sockaddr_pptp *) uaddr;
	
	lock_sock(sk);

	err = -EINVAL;
	if (addr_len < sizeof(*usin)) 
		goto end;

	err = -EAFNOSUPPORT;
	if (usin->sa_family != AF_INET) 
		goto end;

	err = -EBUSY;
	if ((sk->sk_state & PPTP_CONNECTED) && usin->pa.call_id)
		goto end;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,36)
	inet_sk(sk)->inet_id = (__u16)jiffies;
#else
	inet_sk(sk)->id = (__u16)jiffies;
#endif

	if (usin->pa.call_id != 0) {
		err = -ENETUNREACH;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,0)
		po->pptp.fl.daddr = usin->pa.peer_addr;
		po->pptp.fl.flowi4_proto = IPPROTO_GRE;
		rt = ip_route_output_key(&init_net, &po->pptp.fl);
		if (IS_ERR(rt))
#else
		po->pptp.fl.fl4_dst = usin->pa.peer_addr;
		po->pptp.fl.proto = IPPROTO_GRE;
		if (ip_route_output_key(&init_net, &rt, &po->pptp.fl))
#endif
			goto end;

		/*
		 * Save the output Ethernet interface index for route selection.
		 * [__mkroute_output]: rth->rt_iif = oldflp->oif ? : dev_out->ifindex;
		 */
		DEBUGP("PPTP: The output interface index is %d.\n", rt->rt_iif);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,0)
		po->pptp.fl.flowi4_oif = rt->rt_iif;
#else
		po->pptp.fl.oif = rt->rt_iif;
#endif

		po->pptp.pa = usin->pa;
		po->chan.hdrlen = RESV_HDR_LEN;
		po->chan.private = sk;
		po->chan.ops = &pptp_chan_ops;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,36)
		inet_sk(sk)->inet_daddr =usin->pa.peer_addr;
#else
		inet_sk(sk)->daddr =usin->pa.peer_addr;
#endif

		err = set_pptp_item(po);
		if (err < 0)
			goto end;
		
		err = ppp_register_channel(&po->chan);
		if (err)
			goto end;

		sk->sk_state = PPTP_CONNECTED;
	}

end:
	ip_rt_put(rt);
	release_sock(sk);
	return err;
}

int pptp_getname(struct socket *sock, struct sockaddr *uaddr, int *usockaddr_len, 
		int peer)
{
	int len = sizeof(struct sockaddr_pptp);
	struct sockaddr_pptp sp;

	sp.sa_family = AF_INET;
	
	memcpy(&sp.pa, &pptp_sk(sock->sk)->pptp.pa, sizeof(struct pptp_addr));

	memcpy(uaddr, &sp, len);

	*usockaddr_len = len;

	return 0;
}

int pptp_ioctl(struct socket *skt, unsigned int cmd, unsigned long arg)
{
	struct sock *sk = skt->sk;
	struct pptp_sock *po = pptp_sk(sk);
	int  rc = 0;

	lock_sock(sk);
	
	switch (cmd) {
	case PPPIOCGCHAN: {
		int index;
		rc = -ENOTCONN;
		if (!(sk->sk_state & PPTP_CONNECTED))
			break;

		rc = -EINVAL;
		index = ppp_channel_index(&po->chan);
		if (put_user(index , (int *) arg))
			break;

		rc = 0;
		sk->sk_state |= PPTP_BOUND;
		break;
		}
	default:
		break;
	}
	
	release_sock(sk);
	return rc;
}

int pptp_sendmsg(struct kiocb *iocb, struct sock *sk, struct msghdr *msg, size_t len)
{
	return 0;
}

int pptp_recvmsg(struct kiocb *iocb, struct sock *sk, struct msghdr *msg, size_t len, 
		int noblock, int flags, int *addr_len)
{
	return 0;
}

int inet_gre_connect(struct socket *skt, struct sockaddr * uaddr, int addr_len, int flags)
{
	struct sock *sk = skt->sk;

	return sk->sk_prot->connect(sk, (struct sockaddr *)uaddr, addr_len);
}

int inet_gre_sendmsg(struct kiocb *iocb, struct socket *skt, struct msghdr *m, size_t size)
{
	struct sock *sk = skt->sk;

	return sk->sk_prot->sendmsg(iocb, sk, m, size);
}

int inet_gre_recvmsg(struct kiocb *iocb, struct socket *skt, struct msghdr *m, size_t size, int flags)
{
	struct sock *sk = skt->sk;
	int addr_len = 0;
	int err;

	err = sk->sk_prot->recvmsg(iocb, sk, m, size, flags&MSG_DONTWAIT, flags&~MSG_DONTWAIT, 
				&addr_len);
	if (err >= 0)
		m->msg_namelen = addr_len;

	return err;
}

unsigned int inet_gre_poll(struct file * file, struct socket *sock, poll_table *wait)
{
	return 0;
}

int __pptp_xmit(struct sock *sk, struct sk_buff *skb)
{
	struct iphdr *iph;
	struct pptp_hdr *ph;
	struct sk_buff *skb2;
	struct rtable *rt = NULL;
	struct pptp_sock *po = pptp_sk(sk);
		
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,0)
	int pkt_len;
#else
	int pkt_len, err;
#endif
	u16 *p;
	
	if (sock_flag(sk, SOCK_DEAD) ||!(sk->sk_state & PPTP_CONNECTED))
		goto done;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,0)
	rt = ip_route_output_key(dev_net(skb->dev), &po->pptp.fl);
#else
	err = ip_route_output_key(dev_net(skb->dev), &rt, &po->pptp.fl);
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,0)
	if (IS_ERR(rt)) {
		rt = NULL;
		goto done;
	}
	if (rt->rt_flags & (RTCF_MULTICAST |RTCF_BROADCAST))
#else
	if (err ||rt->rt_flags & (RTCF_MULTICAST |RTCF_BROADCAST))
#endif
		goto done;

	/* Copy the skb if there is no space for the header. */
	if (skb_headroom(skb) < RESV_HDR_LEN) {
		if ((skb2 = dev_alloc_skb(skb->len + RESV_HDR_LEN)) == NULL)
			goto done;

		skb_reserve(skb2, RESV_HDR_LEN);
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
	
	/* 
	  * When MIPS CPU running under 32bit mode, int must 32bit aligned; long 32bit aligned; 
	  * pointer must be 32bit aligned. Linux PPP generic driver only sends PPP header with 
	  * two bytes protocol field. Not like PPPoE is 6 bytes header length, GRE is 12 or 16
	  * bytes, it will cause the IP layer for misaligned word accesses and performance will 
	  * suffer - a lot, so we add two bytes of A/C field.
	  * 										--- DNI's haiyue @ 2007/06/28
	  */
	p = (u16 *) __skb_push(skb2, 2); *p = __constant_htons(0xFF03);
	
	pkt_len = skb2->len; /* save the 'original' packet length */
	
	if (po->pptp.ack_snd != po->pptp.seq_rcv) {
		ph = (struct pptp_hdr *) __skb_push(skb2, sizeof(struct pptp_hdr));
		ph->ack = htonl(po->pptp.seq_rcv);
		ph->ver = PPTP_GRE_ACK | PPTP_GRE_VER;
		
		po->pptp.ack_snd = po->pptp.seq_rcv;
	} else {
		ph = (struct pptp_hdr *) __skb_push(skb2, sizeof(struct pptp_hdr) - sizeof(__u32));
		ph->ver = PPTP_GRE_VER;
	}

	skb_reset_transport_header(skb2);
	ph->flags = PPTP_GRE_KEY | PPTP_GRE_SEQ;
	ph->protocol = __constant_htons(PPTP_GRE_PROTO);
	ph->payload_len = htons(pkt_len);
	ph->call_id = po->pptp.pa.peer_callid;
	ph->seq = htonl(po->pptp.seq_snd++);

	iph = (struct iphdr *) __skb_push(skb2, sizeof(struct iphdr));
	skb_reset_network_header(skb2);
	iph->version = 4;
	iph->ihl = 5;
	iph->tos = 0;
	iph->tot_len = htons(skb2->len);
	iph->frag_off = __constant_htons(IP_DF); /* C/S are in the same subnet. */
	iph->ttl = 64;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,36)
	ip_select_ident(skb, sk);
#else
	ip_select_ident(iph, &rt->u.dst, sk);
#endif
	iph->protocol = IPPROTO_GRE;
	iph->saddr = rt->rt_src;
	iph->daddr = rt->rt_dst;
	iph->check = 0;
	iph->check = ip_fast_csum((unsigned char *)iph, 5);
	
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,36)
	skb_dst_set(skb2,dst_clone(&rt->dst));

	NF_HOOK(PF_INET, NF_INET_LOCAL_OUT, skb2, NULL, rt->dst.dev, dst_output);
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,31)
	skb_dst_set(skb2,dst_clone(&rt->u.dst));

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

int pptp_xmit(struct ppp_channel *chan, struct sk_buff *skb)
{
	struct sock *sk = (struct sock *) chan->private;

	return __pptp_xmit(sk, skb);
}

struct proto_ops inet_gre_ops = {
	.family		= PF_INET,
	.owner 		= THIS_MODULE,		
	.release		= pptp_release,
	.bind			= sock_no_bind,
	.connect		= inet_gre_connect,
	.socketpair	= sock_no_socketpair,
 	.accept		= sock_no_accept,
	.getname		= pptp_getname,
	.poll			= inet_gre_poll,
	.ioctl			= pptp_ioctl,
	.listen		= sock_no_listen,
	.shutdown	= sock_no_shutdown,
	.setsockopt	= sock_no_setsockopt,
	.getsockopt	= sock_no_getsockopt,
	.sendmsg		= inet_gre_sendmsg,
	.recvmsg		= inet_gre_recvmsg,
	.mmap		= sock_no_mmap
};

struct proto pptp_prot = {
	.name		= "GRE-PPP",
	.owner 		= THIS_MODULE,
	.obj_size		= sizeof(struct pptp_sock),
	.close		= pptp_close,
	.connect		= pptp_connect,
	.disconnect	= pptp_disconnect,
	.sendmsg		= pptp_sendmsg,
	.recvmsg		= pptp_recvmsg,
	.backlog_rcv	= pptp_rcv_core,
	.init			= pptp_init_sock,
};

struct inet_protosw inetsw_pptp = {
	.type	= SOCK_GRE_PPP,
	.protocol	= IPPROTO_GRE,
	.prot		= &pptp_prot,
	.ops		= &inet_gre_ops,
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
	.capability	= CAP_NET_RAW,
#endif
	.no_check	= 0,
	.flags	= INET_PROTOSW_REUSE,
};


struct net_protocol pptp_protocol = {
	.handler		= pptp_rcv,
	.no_policy		= 1,
};

int __init pptp_init(void)
{
	int err;

	err = proto_register(&pptp_prot, 1);
	if (err)
		return err;

	inet_add_protocol(&pptp_protocol, IPPROTO_GRE);
		
	inet_register_protosw(&inetsw_pptp);

	printk(KERN_INFO "PPTP-GRE-v1.13 2007 (C) Delta Networks Inc.\n");

	return 0;
}

void __exit pptp_exit(void)
{
	inet_unregister_protosw(&inetsw_pptp);
	
	inet_del_protocol(&pptp_protocol, IPPROTO_GRE);

	proto_unregister(&pptp_prot);
}

module_init(pptp_init);
module_exit(pptp_exit);

MODULE_AUTHOR("haiyue <haiyue.wang@deltaww.com.cn>");
MODULE_DESCRIPTION("PPTP-GRE Linux kernel support.");
MODULE_LICENSE("GPL");

