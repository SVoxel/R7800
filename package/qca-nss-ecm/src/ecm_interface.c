/*
 **************************************************************************
 * Copyright (c) 2014-2015 The Linux Foundation.  All rights reserved.
 * Permission to use, copy, modify, and/or distribute this software for
 * any purpose with or without fee is hereby granted, provided that the
 * above copyright notice and this permission notice appear in all copies.
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 **************************************************************************
 */

#include <linux/version.h>
#include <linux/types.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/icmp.h>
#include <linux/kthread.h>
#include <linux/pkt_sched.h>
#include <linux/string.h>
#include <net/ip6_route.h>
#include <net/ip6_fib.h>
#include <net/ipv6.h>
#include <net/route.h>
#include <net/ip_fib.h>
#include <net/ip.h>
#include <net/tcp.h>
#include <asm/unaligned.h>
#include <asm/uaccess.h>	/* for put_user */
#include <linux/inet.h>
#include <linux/in6.h>
#include <linux/in.h>
#include <linux/udp.h>
#include <linux/tcp.h>
#if defined(ECM_DB_XREF_ENABLE) && defined(ECM_BAND_STEERING_ENABLE)
#include <linux/if_bridge.h>
#endif
#include <linux/inetdevice.h>
#if defined(ECM_INTERFACE_TUNIPIP6_ENABLE) || defined(ECM_INTERFACE_SIT_ENABLE)
#if (LINUX_VERSION_CODE <= KERNEL_VERSION(3, 9, 0))
#include <net/ipip.h>
#else
#include <net/ip_tunnels.h>
#endif
#endif
#include <net/ip6_tunnel.h>
#include <net/addrconf.h>
#include <linux/if_arp.h>
#include <linux/netfilter_ipv4.h>
#include <linux/netfilter_bridge.h>
#include <linux/if_bridge.h>
#include <net/arp.h>
#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_conntrack_acct.h>
#include <net/netfilter/nf_conntrack_helper.h>
#include <net/netfilter/nf_conntrack_l4proto.h>
#include <net/netfilter/nf_conntrack_l3proto.h>
#include <net/netfilter/nf_conntrack_zones.h>
#include <net/netfilter/nf_conntrack_core.h>
#include <net/netfilter/ipv4/nf_conntrack_ipv4.h>
#include <net/netfilter/ipv4/nf_defrag_ipv4.h>
#ifdef ECM_INTERFACE_VLAN_ENABLE
#include <linux/../../net/8021q/vlan.h>
#include <linux/if_vlan.h>
#endif
#ifdef ECM_INTERFACE_PPP_ENABLE
#include <linux/if_pppox.h>
#endif

/*
 * Debug output levels
 * 0 = OFF
 * 1 = ASSERTS / ERRORS
 * 2 = 1 + WARN
 * 3 = 2 + INFO
 * 4 = 3 + TRACE
 */
#define DEBUG_LEVEL ECM_INTERFACE_DEBUG_LEVEL

#ifdef ECM_MULTICAST_ENABLE
#include <mc_ecm.h>
#endif

#include "ecm_types.h"
#include "ecm_db_types.h"
#include "ecm_state.h"
#include "ecm_tracker.h"
#include "ecm_classifier.h"
#include "ecm_front_end_types.h"
#include "ecm_tracker_datagram.h"
#include "ecm_tracker_udp.h"
#include "ecm_tracker_tcp.h"
#include "ecm_db.h"
#include "ecm_interface.h"

#ifdef ECM_IPV6_ENABLE
/*
 * TODO: Remove once the Linux image and headers get propogated.
 */
struct net_device *ipv6_dev_find(struct net *net, struct in6_addr *addr, int strict);
#endif

/*
 * Locking - concurrency control
 */
static DEFINE_SPINLOCK(ecm_interface_lock);			/* Protect against SMP access between netfilter, events and private threaded function. */

/*
 * Management thread control
 */
static bool ecm_interface_terminate_pending = false;		/* True when the user has signalled we should quit */

/*
 * ecm_interface_get_and_hold_dev_master()
 *	Returns the master device of a net device if any.
 */
struct net_device *ecm_interface_get_and_hold_dev_master(struct net_device *dev)
{
	struct net_device *master;
#if (LINUX_VERSION_CODE > KERNEL_VERSION(3,6,0))
	rcu_read_lock();
	master = netdev_master_upper_dev_get_rcu(dev);
	if (!master) {
		rcu_read_unlock();
		return NULL;
	}
	dev_hold(master);
	rcu_read_unlock();
#else
	master = dev->master;
	if (!master) {
		return NULL;
	}
	dev_hold(master);
#endif
	return master;
}
EXPORT_SYMBOL(ecm_interface_get_and_hold_dev_master);

/*
 * ecm_interface_dev_find_by_local_addr_ipv4()
 *	Return a hold to the device for the given local IP address.  Returns NULL on failure.
 */
static struct net_device *ecm_interface_dev_find_by_local_addr_ipv4(ip_addr_t addr)
{
	__be32 be_addr;
	struct net_device *dev;

	ECM_IP_ADDR_TO_NIN4_ADDR(be_addr, addr);
	dev = ip_dev_find(&init_net, be_addr);
	return dev;
}

#ifdef ECM_IPV6_ENABLE
/*
 * ecm_interface_dev_find_by_local_addr_ipv6()
 *	Return a hold to the device for the given local IP address.  Returns NULL on failure.
 */
static struct net_device *ecm_interface_dev_find_by_local_addr_ipv6(ip_addr_t addr)
{
	struct in6_addr addr6;
	struct net_device *dev;

	ECM_IP_ADDR_TO_NIN6_ADDR(addr6, addr);
	dev = (struct net_device *)ipv6_dev_find(&init_net, &addr6, 1);
	return dev;
}
#endif

/*
 * ecm_interface_dev_find_by_local_addr()
 *	Return the device on which the local address resides.
 *
 * Returns a hold to the device or NULL on failure.
 */
struct net_device *ecm_interface_dev_find_by_local_addr(ip_addr_t addr)
{
	char __attribute__((unused)) addr_str[40];

	DEBUG_ECM_IP_ADDR_TO_STRING(addr_str, addr);
	DEBUG_TRACE("Locate dev for: %s\n", addr_str);

	if (ECM_IP_ADDR_IS_V4(addr)) {
		return ecm_interface_dev_find_by_local_addr_ipv4(addr);
	}

#ifdef ECM_IPV6_ENABLE
	return ecm_interface_dev_find_by_local_addr_ipv6(addr);
#else
	return NULL;
#endif
}
EXPORT_SYMBOL(ecm_interface_dev_find_by_local_addr);

/*
 * ecm_interface_dev_find_by_addr()
 *	Return the net device on which the given IP address resides.  Returns NULL on faiure.
 *
 * NOTE: The device may be the device upon which has a default gateway to reach the address.
 * from_local_addr is true when the device was found by a local address search.
 */
struct net_device *ecm_interface_dev_find_by_addr(ip_addr_t addr, bool *from_local_addr)
{
	char __attribute__((unused)) addr_str[40];
	struct ecm_interface_route ecm_rt;
	struct net_device *dev;
	struct dst_entry *dst;

	DEBUG_ECM_IP_ADDR_TO_STRING(addr_str, addr);

	/*
	 * Is the address a local IP?
	 */
	DEBUG_TRACE("find net device for address: %s\n", addr_str);
	dev = ecm_interface_dev_find_by_local_addr(addr);
	if (dev) {
		 *from_local_addr = true;
		DEBUG_TRACE("addr: %s is local: %p (%s)\n", addr_str, dev, dev->name);
		return dev;
	}

	DEBUG_TRACE("addr: %s is not local\n", addr_str);

	/*
	 * Try a route to the address instead
	 * NOTE: This will locate a route entry in the route destination *cache*.
	 */
	if (!ecm_interface_find_route_by_addr(addr, &ecm_rt)) {
		DEBUG_WARN("addr: %s - no dev locatable\n", addr_str);
		return NULL;
	}

	*from_local_addr = false;
	dst = ecm_rt.dst;
	dev = dst->dev;
	dev_hold(dev);
	ecm_interface_route_release(&ecm_rt);
	DEBUG_TRACE("dest_addr: %s uses dev: %p(%s)\n", addr_str, dev, dev->name);
	return dev;
}
EXPORT_SYMBOL(ecm_interface_dev_find_by_addr);

#ifdef ECM_IPV6_ENABLE
/*
 * ecm_interface_mac_addr_get_ipv6()
 *	Return mac for an IPv6 address
 *
 * GGG TODO Need to make sure this also works for local IP addresses too.
 */
static bool ecm_interface_mac_addr_get_ipv6(ip_addr_t addr, uint8_t *mac_addr, bool *on_link, ip_addr_t gw_addr)
{
	struct in6_addr daddr;
	struct ecm_interface_route ecm_rt;
	struct neighbour *neigh;
	struct rt6_info *rt;
	struct dst_entry *dst;

	/*
	 * Get the MAC address that corresponds to IP address given.
	 * We look up the rt6_info entries and, from its neighbour structure, obtain the hardware address.
	 * This means we will also work if the neighbours are routers too.
	 */
	ECM_IP_ADDR_TO_NIN6_ADDR(daddr, addr);
	if (!ecm_interface_find_route_by_addr(addr, &ecm_rt)) {
		*on_link = false;
		return false;
	}
	DEBUG_ASSERT(!ecm_rt.v4_route, "Did not locate a v6 route!\n");

	/*
	 * Is this destination on link or off-link via a gateway?
	 */
	rt = ecm_rt.rt.rtv6;
	if (!ECM_IP_ADDR_MATCH(rt->rt6i_dst.addr.in6_u.u6_addr32, rt->rt6i_gateway.in6_u.u6_addr32) || (rt->rt6i_flags & RTF_GATEWAY)) {
		*on_link = false;
		ECM_NIN6_ADDR_TO_IP_ADDR(gw_addr, rt->rt6i_gateway)
	} else {
		*on_link = true;
	}

	rcu_read_lock();
	dst = ecm_rt.dst;
#if (LINUX_VERSION_CODE <= KERNEL_VERSION(3,6,0))
	neigh = dst_get_neighbour_noref(dst);
	if (neigh) {
		neigh_hold(neigh);
	}
#else
	neigh = dst_neigh_lookup(dst, &daddr);
#endif
	if (!neigh) {
		neigh = neigh_lookup(&nd_tbl, &daddr, dst->dev);
	}

	if (!neigh) {
		rcu_read_unlock();
		ecm_interface_route_release(&ecm_rt);
		DEBUG_WARN("No neigh reference\n");
		return false;
	}
	if (!(neigh->nud_state & NUD_VALID)) {
		rcu_read_unlock();
		neigh_release(neigh);
		ecm_interface_route_release(&ecm_rt);
		DEBUG_WARN("NUD invalid\n");
		return false;
	}
	if (!neigh->dev) {
		rcu_read_unlock();
		neigh_release(neigh);
		ecm_interface_route_release(&ecm_rt);
		DEBUG_WARN("Neigh dev invalid\n");
		return false;
	}

	/*
	 * If neigh->dev is a loopback then addr is a local address in which case we take the MAC from given device
	 */
	if (neigh->dev->flags & IFF_LOOPBACK) {
		// GGG TODO Create an equivalent logic to that for ipv4, maybe need to create an ip6_dev_find()?
		DEBUG_TRACE("local address " ECM_IP_ADDR_OCTAL_FMT " (found loopback)\n", ECM_IP_ADDR_TO_OCTAL(addr));
		memset(mac_addr, 0, 6);
	} else {
		memcpy(mac_addr, neigh->ha, 6);
	}
	rcu_read_unlock();
	neigh_release(neigh);
	ecm_interface_route_release(&ecm_rt);

	DEBUG_TRACE(ECM_IP_ADDR_OCTAL_FMT " maps to %pM\n", ECM_IP_ADDR_TO_OCTAL(addr), mac_addr);
	return true;
}
#endif

/*
 * ecm_interface_mac_addr_get_ipv4()
 *	Return mac for an IPv4 address
 */
static bool ecm_interface_mac_addr_get_ipv4(ip_addr_t addr, uint8_t *mac_addr, bool *on_link, ip_addr_t gw_addr)
{
	struct neighbour *neigh;
	struct ecm_interface_route ecm_rt;
	struct rtable *rt;
	struct dst_entry *dst;
	__be32 ipv4_addr;

	/*
	 * Get the MAC address that corresponds to IP address given.
	 * We look up the rtable entries and, from its neighbour structure, obtain the hardware address.
	 * This means we will also work if the neighbours are routers too.
	 * We also locate the MAC if the address is a local host address.
	 */
	ECM_IP_ADDR_TO_NIN4_ADDR(ipv4_addr, addr);
	if (!ecm_interface_find_route_by_addr(addr, &ecm_rt)) {
		*on_link = false;
		return false;
	}
	DEBUG_ASSERT(ecm_rt.v4_route, "Did not locate a v4 route!\n");
	DEBUG_TRACE("Found route\n");

	/*
	 * Is this destination on link or off-link via a gateway?
	 */
	rt = ecm_rt.rt.rtv4;
#if (LINUX_VERSION_CODE <= KERNEL_VERSION(3,6,0))
	if ((rt->rt_dst != rt->rt_gateway) || (rt->rt_flags & RTF_GATEWAY)) {
#else
	if (rt->rt_uses_gateway || (rt->rt_flags & RTF_GATEWAY)) {
#endif
		*on_link = false;
		ECM_NIN4_ADDR_TO_IP_ADDR(gw_addr, rt->rt_gateway)
	} else {
		*on_link = true;
	}

	/*
	 * Get the neighbour entry for the address
	 */
	rcu_read_lock();
	dst = ecm_rt.dst;
#if (LINUX_VERSION_CODE <= KERNEL_VERSION(3,6,0))
	neigh = dst_get_neighbour_noref(dst);
	if (neigh) {
		neigh_hold(neigh);
	}
#else
	neigh = dst_neigh_lookup(dst, &ipv4_addr);
#endif
	if (!neigh) {
		neigh = neigh_lookup(&arp_tbl, &ipv4_addr, dst->dev);
	}
	if (!neigh) {
		rcu_read_unlock();
		ecm_interface_route_release(&ecm_rt);
		DEBUG_WARN("no neigh\n");
		return false;
	}
	if (!(neigh->nud_state & NUD_VALID)) {
		rcu_read_unlock();
		neigh_release(neigh);
		ecm_interface_route_release(&ecm_rt);
		DEBUG_WARN("neigh nud state is not valid\n");
		return false;
	}
	if (!neigh->dev) {
		rcu_read_unlock();
		neigh_release(neigh);
		ecm_interface_route_release(&ecm_rt);
		DEBUG_WARN("neigh has no device\n");
		return false;
	}

	/*
	 * If the device is loopback this will be because the address is a local address
	 * In this case locate the device that has this local address and get its mac.
	 */
	if (neigh->dev->type == ARPHRD_LOOPBACK) {
		struct net_device *dev;

		DEBUG_TRACE("%pI4 finds loopback device, dev: %p (%s)\n", &ipv4_addr, neigh->dev, neigh->dev->name);
		rcu_read_unlock();
		neigh_release(neigh);
		ecm_interface_route_release(&ecm_rt);

		/*
		 * Lookup the device that has this IP address assigned
		 */
		dev = ip_dev_find(&init_net, ipv4_addr);
		if (!dev) {
			DEBUG_WARN("Unable to locate dev for: %pI4\n", &ipv4_addr);
			return false;
		}
		memcpy(mac_addr, dev->dev_addr, (size_t)dev->addr_len);
		DEBUG_TRACE("is local addr: %pI4, mac: %pM, dev ifindex: %d, dev: %p (%s), dev_type: %d\n",
				&ipv4_addr, mac_addr, dev->ifindex, dev, dev->name, dev->type);
		dev_put(dev);
		return true;
	}

	if (!(neigh->dev->flags & IFF_NOARP)) {
		memcpy(mac_addr, neigh->ha, (size_t)neigh->dev->addr_len);
	} else {
		DEBUG_TRACE("non-arp device: %p (%s, type: %d) to reach %pI4\n", neigh->dev, neigh->dev->name, neigh->dev->type, &ipv4_addr);
		memset(mac_addr, 0, 6);
	}
	DEBUG_TRACE("addr: %pI4, mac: %pM, iif: %d, neigh dev ifindex: %d, dev: %p (%s), dev_type: %d\n",
			&ipv4_addr, mac_addr, rt->rt_iif, neigh->dev->ifindex, neigh->dev, neigh->dev->name, neigh->dev->type);

	rcu_read_unlock();
	neigh_release(neigh);
	ecm_interface_route_release(&ecm_rt);
	return true;
}

/*
 * ecm_interface_mac_addr_get()
 *	Return the mac address for the given IP address.  Returns false on failure.
 *
 * dev is the device on which the addr was sent/received.  If addr is a local address then mac shall be the given dev mac.
 *
 * GGG TODO Make this function work for IPv6!!!!!!!!!!!!!!
 */
bool ecm_interface_mac_addr_get(ip_addr_t addr, uint8_t *mac_addr, bool *on_link, ip_addr_t gw_addr)
{
	if (ECM_IP_ADDR_IS_V4(addr)) {
		return ecm_interface_mac_addr_get_ipv4(addr, mac_addr, on_link, gw_addr);
	}

#ifdef ECM_IPV6_ENABLE
	return ecm_interface_mac_addr_get_ipv6(addr, mac_addr, on_link, gw_addr);
#else
	return false;
#endif
}
EXPORT_SYMBOL(ecm_interface_mac_addr_get);

#ifdef ECM_MULTICAST_ENABLE
/*
 * ecm_interface_multicast_check_for_br_dev()
 * 	Find a bridge dev is present or not in an
 * 	array of Ifindexs
 */
bool ecm_interface_multicast_check_for_br_dev(uint32_t dest_if[], uint8_t max_if)
{
	struct net_device *br_dev;
	int i;

	for (i = 0; i < max_if; i++) {
		br_dev = dev_get_by_index(&init_net, dest_if[i]);
		if (!br_dev) {
			DEBUG_ASSERT(NULL, "expected only valid netdev here\n");
			continue;
		}

		if (ecm_front_end_is_bridge_device(br_dev)) {
			dev_put(br_dev);
			return true;
		}
		dev_put(br_dev);
	}
	return false;
}
EXPORT_SYMBOL(ecm_interface_multicast_check_for_br_dev);
#endif

/*
 * ecm_interface_addr_find_route_by_addr_ipv4()
 *	Return the route for the given IP address.  Returns NULL on failure.
 */
static bool ecm_interface_find_route_by_addr_ipv4(ip_addr_t addr, struct ecm_interface_route *ecm_rt)
{
	__be32 be_addr;

	/*
	 * Get a route to the given IP address, this will allow us to also find the interface
	 * it is using to communicate with that IP address.
	 */
	ECM_IP_ADDR_TO_NIN4_ADDR(be_addr, addr);
	ecm_rt->rt.rtv4 = ip_route_output(&init_net, be_addr, 0, 0, 0);
	if (IS_ERR(ecm_rt->rt.rtv4)) {
		DEBUG_TRACE("No output route to: %pI4n\n", &be_addr);
		return false;
	}
	DEBUG_TRACE("Output route to: %pI4n is: %p\n", &be_addr, ecm_rt->rt.rtv4);
	ecm_rt->dst = (struct dst_entry *)ecm_rt->rt.rtv4;
	ecm_rt->v4_route = true;
	return true;
}

#ifdef ECM_IPV6_ENABLE
/*
 * ecm_interface_addr_find_route_by_addr_ipv6()
 *	Return the route for the given IP address.  Returns NULL on failure.
 */
static bool ecm_interface_find_route_by_addr_ipv6(ip_addr_t addr, struct ecm_interface_route *ecm_rt)
{
	struct in6_addr naddr;

	ECM_IP_ADDR_TO_NIN6_ADDR(naddr, addr);

	/*
	 * Get a route to the given IP address, this will allow us to also find the interface
	 * it is using to communicate with that IP address.
	 */
	ecm_rt->rt.rtv6 = rt6_lookup(&init_net, &naddr, NULL, 0, 0);
	if (!ecm_rt->rt.rtv6) {
		DEBUG_TRACE("No output route to: " ECM_IP_ADDR_OCTAL_FMT "\n", ECM_IP_ADDR_TO_OCTAL(addr));
		return NULL;
	}
	DEBUG_TRACE("Output route to: " ECM_IP_ADDR_OCTAL_FMT " is: %p\n", ECM_IP_ADDR_TO_OCTAL(addr), ecm_rt->rt.rtv6);
	ecm_rt->dst = (struct dst_entry *)ecm_rt->rt.rtv6;
	ecm_rt->v4_route = false;
	return true;
}
#endif

/*
 * ecm_interface_addr_find_route_by_addr()
 *	Return the route (in the given parameter) for the given IP address.  Returns false on failure.
 *
 * Route is the device on which the addr is reachable, which may be loopback for local addresses.
 *
 * Returns true if the route was able to be located.  The route must be released using ecm_interface_route_release().
 */
bool ecm_interface_find_route_by_addr(ip_addr_t addr, struct ecm_interface_route *ecm_rt)
{
	char __attribute__((unused)) addr_str[40];

	DEBUG_ECM_IP_ADDR_TO_STRING(addr_str, addr);
	DEBUG_TRACE("Locate route to: %s\n", addr_str);

	if (ECM_IP_ADDR_IS_V4(addr)) {
		return ecm_interface_find_route_by_addr_ipv4(addr, ecm_rt);
	}

#ifdef ECM_IPV6_ENABLE
	return ecm_interface_find_route_by_addr_ipv6(addr, ecm_rt);
#else
	return false;
#endif
}
EXPORT_SYMBOL(ecm_interface_find_route_by_addr);

/*
 * ecm_interface_route_release()
 *	Release an ecm route
 */
void ecm_interface_route_release(struct ecm_interface_route *rt)
{
	dst_release(rt->dst);
}
EXPORT_SYMBOL(ecm_interface_route_release);

#ifdef ECM_IPV6_ENABLE
/*
 * ecm_interface_send_neighbour_solicitation()
 *	Issue an IPv6 Neighbour soliciation request.
 */
void ecm_interface_send_neighbour_solicitation(struct net_device *dev, ip_addr_t addr)
{
	struct in6_addr dst_addr, src_addr;
	struct in6_addr mc_dst_addr;
	struct rt6_info *rt6i;
	struct neighbour *neigh;
	ip_addr_t ecm_mc_dst_addr, ecm_src_addr;
	struct net *netf = dev_net(dev);
	int ret;

	char __attribute__((unused)) dst_addr_str[ECM_IP_ADDR_STRING_BUFFER_SIZE];
	char __attribute__((unused)) mc_dst_addr_str[ECM_IP_ADDR_STRING_BUFFER_SIZE];
	char __attribute__((unused)) src_addr_str[ECM_IP_ADDR_STRING_BUFFER_SIZE];

	/*
	 * Find source and destination addresses in Linux format. We need
	 * mcast destination address as well.
	 */
	ECM_IP_ADDR_TO_NIN6_ADDR(dst_addr, addr);
	addrconf_addr_solict_mult(&dst_addr, &mc_dst_addr);
	ret = ipv6_dev_get_saddr(netf, dev, &dst_addr, 0, &src_addr);

	/*
	 * IP address in string format for debug
	 */
	ecm_ip_addr_to_string(dst_addr_str, addr);
	ECM_NIN6_ADDR_TO_IP_ADDR(ecm_mc_dst_addr, mc_dst_addr);
	ecm_ip_addr_to_string(mc_dst_addr_str, ecm_mc_dst_addr);
	ECM_NIN6_ADDR_TO_IP_ADDR(ecm_src_addr, src_addr);
	ecm_ip_addr_to_string(src_addr_str, ecm_src_addr);

	/*
	 * Find the route entry
	 */
	rt6i = rt6_lookup(netf, &dst_addr, NULL, 0, 0);
	if (!rt6i) {
		DEBUG_TRACE("IPv6 Route lookup failure for destination IPv6 address %s\n", dst_addr_str);
		return;
	}

	/*
	 * Find the neighbor entry
	 */
#if (LINUX_VERSION_CODE <= KERNEL_VERSION(3,6,0))
	neigh = rt6i->dst.ops->neigh_lookup(&rt6i->dst, &dst_addr);
#else
	neigh = rt6i->dst.ops->neigh_lookup(&rt6i->dst, NULL, &dst_addr);
#endif
	if (neigh == NULL) {
		DEBUG_TRACE("Neighbour lookup failure for destination IPv6 address %s\n", dst_addr_str);
		dst_release(&rt6i->dst);
		return;
	}

	/*
	 * Issue a Neighbour soliciation request
	 */
	DEBUG_TRACE("Issue Neighbour solicitation request\n");
	ndisc_send_ns(dev, neigh, &dst_addr, &mc_dst_addr, &src_addr);
	neigh_release(neigh);
	dst_release(&rt6i->dst);
}
EXPORT_SYMBOL(ecm_interface_send_neighbour_solicitation);
#endif

/*
 * ecm_interface_send_arp_request()
 *	Issue and ARP request.
 */
void ecm_interface_send_arp_request(struct net_device *dest_dev, ip_addr_t dest_addr, bool on_link, ip_addr_t gw_addr)
{
	/*
	 * Possible ARP does not know the address yet
	 */
	__be32 ipv4_addr;
	__be32 src_ip;

	/*
	 * Issue an ARP request for it, select the src_ip from which to issue the request.
	 */
	ECM_IP_ADDR_TO_NIN4_ADDR(ipv4_addr, dest_addr);
	src_ip = inet_select_addr(dest_dev, ipv4_addr, RT_SCOPE_LINK);
	if (!src_ip) {
		DEBUG_TRACE("Failed to lookup IP for %pI4\n", &ipv4_addr);
		return;
	}

	/*
	 * If we have a GW for this address, then we have to send ARP request to the GW
	 */
	if (!on_link && !ECM_IP_ADDR_IS_NULL(gw_addr)) {
		ECM_IP_ADDR_TO_NIN4_ADDR(ipv4_addr, gw_addr);
	}

	DEBUG_TRACE("Send ARP for %pI4 using src_ip as %pI4\n", &ipv4_addr, &src_ip);
	arp_send(ARPOP_REQUEST, ETH_P_ARP, ipv4_addr, dest_dev, src_ip, NULL, NULL, NULL);

	return;
}
EXPORT_SYMBOL(ecm_interface_send_arp_request);

/*
 * ecm_interface_ipv4_neigh_get()
 * 	Returns neighbour reference for a given IP address which must be released when you are done with it.
 *
 * Returns NULL on fail.
 */
struct neighbour *ecm_interface_ipv4_neigh_get(ip_addr_t addr)
{
	struct neighbour *neigh;
	struct rtable *rt;
	struct dst_entry *dst;
	__be32 ipv4_addr;

	ECM_IP_ADDR_TO_NIN4_ADDR(ipv4_addr, addr);
	rt = ip_route_output(&init_net, ipv4_addr, 0, 0, 0);
	if (IS_ERR(rt)) {
		return NULL;
	}
	dst = (struct dst_entry *)rt;
	neigh = dst_neigh_lookup(dst, &ipv4_addr);
	ip_rt_put(rt);
	return neigh;
}

#ifdef ECM_IPV6_ENABLE
/*
 * ecm_interface_ipv6_neigh_get()
 * 	Returns neighbour reference for a given IP address which must be released when you are done with it.
 *
 * Returns NULL on fail.
 */
struct neighbour *ecm_interface_ipv6_neigh_get(ip_addr_t addr)
{
	struct neighbour *neigh;
	struct rt6_info *rt;
	struct dst_entry *dst;
	struct in6_addr ipv6_addr;

	ECM_IP_ADDR_TO_NIN6_ADDR(ipv6_addr, addr);
	rt = rt6_lookup(&init_net, &ipv6_addr, NULL, 0, 0);
	if (!rt) {
		return NULL;
	}
	dst = (struct dst_entry *)rt;
	neigh = dst_neigh_lookup(dst, &ipv6_addr);
	dst_release(dst);
	return neigh;
}
#endif

#ifdef ECM_INTERFACE_PPP_ENABLE
/*
 * ecm_interface_skip_l2tp_pptp()
 *	skip l2tp/pptp tunnel encapsulated traffic
 *
 * ECM does not handle L2TP or PPTP encapsulated packets,
 * this function detects packets of that type so they can be skipped over to improve their throughput.
 */
bool ecm_interface_skip_l2tp_pptp(struct sk_buff *skb, const struct net_device *out)
{
	struct ppp_channel *ppp_chan[1];
	int px_proto;
	struct net_device *in;

	/*
	 * skip first pass of l2tp/pptp tunnel encapsulated traffic
	 */
	if (out->type == ARPHRD_PPP) {
		if (ppp_hold_channels((struct net_device *)out, ppp_chan, 1) == 1) {
			px_proto = ppp_channel_get_protocol(ppp_chan[0]);
			ppp_release_channels(ppp_chan, 1);
			return ((px_proto == PX_PROTO_OL2TP) || (px_proto == PX_PROTO_PPTP));
		}
	}

	in = dev_get_by_index(&init_net, skb->skb_iif);
	if (unlikely(!in)) {
		return false;
	}

	/*
	 * Skip wan to lan l2tp packets
	 */
	if (in->type == ARPHRD_PPP) {
		if (__ppp_hold_channels((struct net_device *)in, ppp_chan, 1) == 1) {
			px_proto = ppp_channel_get_protocol(ppp_chan[0]);
			ppp_release_channels(ppp_chan, 1);
			dev_put(in);
			return ((px_proto == PX_PROTO_OL2TP) || (px_proto == PX_PROTO_PPTP));
		}
	}
	dev_put(in);

	return false;
}
#endif

#ifdef ECM_INTERFACE_VLAN_ENABLE
/*
 * ecm_interface_vlan_interface_establish()
 *	Returns a reference to a iface of the VLAN type, possibly creating one if necessary.
 * Returns NULL on failure or a reference to interface.
 */
static struct ecm_db_iface_instance *ecm_interface_vlan_interface_establish(struct ecm_db_interface_info_vlan *type_info,
							char *dev_name, int32_t dev_interface_num, int32_t ae_interface_num, int32_t mtu)
{
	struct ecm_db_iface_instance *nii;
	struct ecm_db_iface_instance *ii;

	DEBUG_INFO("Establish VLAN iface: %s with address: %pM, vlan tag: %u, vlan_tpid: %x MTU: %d, if num: %d, accel engine if id: %d\n",
			dev_name, type_info->address, type_info->vlan_tag, type_info->vlan_tpid, mtu, dev_interface_num, ae_interface_num);

	/*
	 * Locate the iface
	 */
	ii = ecm_db_iface_find_and_ref_vlan(type_info->address, type_info->vlan_tag, type_info->vlan_tpid);
	if (ii) {
		DEBUG_TRACE("%p: iface established\n", ii);
		return ii;
	}

	/*
	 * No iface - create one
	 */
	nii = ecm_db_iface_alloc();
	if (!nii) {
		DEBUG_WARN("Failed to establish iface\n");
		return NULL;
	}

	/*
	 * Add iface into the database, atomically to avoid races creating the same thing
	 */
	spin_lock_bh(&ecm_interface_lock);
	ii = ecm_db_iface_find_and_ref_vlan(type_info->address, type_info->vlan_tag, type_info->vlan_tpid);
	if (ii) {
		spin_unlock_bh(&ecm_interface_lock);
		ecm_db_iface_deref(nii);
		return ii;
	}
	ecm_db_iface_add_vlan(nii, type_info->address, type_info->vlan_tag, type_info->vlan_tpid, dev_name,
			mtu, dev_interface_num, ae_interface_num, NULL, nii);
	spin_unlock_bh(&ecm_interface_lock);

	DEBUG_TRACE("%p: vlan iface established\n", nii);
	return nii;
}
#endif

/*
 * ecm_interface_bridge_interface_establish()
 *	Returns a reference to a iface of the BRIDGE type, possibly creating one if necessary.
 * Returns NULL on failure or a reference to interface.
 */
static struct ecm_db_iface_instance *ecm_interface_bridge_interface_establish(struct ecm_db_interface_info_bridge *type_info,
							char *dev_name, int32_t dev_interface_num, int32_t ae_interface_num, int32_t mtu)
{
	struct ecm_db_iface_instance *nii;
	struct ecm_db_iface_instance *ii;

	DEBUG_INFO("Establish BRIDGE iface: %s with address: %pM, MTU: %d, if num: %d, accel engine if id: %d\n",
			dev_name, type_info->address, mtu, dev_interface_num, ae_interface_num);

	/*
	 * Locate the iface
	 */
	ii = ecm_db_iface_find_and_ref_bridge(type_info->address);
	if (ii) {
		DEBUG_TRACE("%p: iface established\n", ii);
		return ii;
	}

	/*
	 * No iface - create one
	 */
	nii = ecm_db_iface_alloc();
	if (!nii) {
		DEBUG_WARN("Failed to establish iface\n");
		return NULL;
	}

	/*
	 * Add iface into the database, atomically to avoid races creating the same thing
	 */
	spin_lock_bh(&ecm_interface_lock);
	ii = ecm_db_iface_find_and_ref_bridge(type_info->address);
	if (ii) {
		spin_unlock_bh(&ecm_interface_lock);
		ecm_db_iface_deref(nii);
		return ii;
	}
	ecm_db_iface_add_bridge(nii, type_info->address, dev_name,
			mtu, dev_interface_num, ae_interface_num, NULL, nii);
	spin_unlock_bh(&ecm_interface_lock);

	DEBUG_TRACE("%p: bridge iface established\n", nii);
	return nii;
}

#ifdef ECM_INTERFACE_BOND_ENABLE
/*
 * ecm_interface_lag_interface_establish()
 *	Returns a reference to a iface of the LAG type, possibly creating one if necessary.
 * Returns NULL on failure or a reference to interface.
 */
static struct ecm_db_iface_instance *ecm_interface_lag_interface_establish(struct ecm_db_interface_info_lag *type_info,
							char *dev_name, int32_t dev_interface_num, int32_t ae_interface_num, int32_t mtu)
{
	struct ecm_db_iface_instance *nii;
	struct ecm_db_iface_instance *ii;

	DEBUG_INFO("Establish LAG iface: %s with address: %pM, MTU: %d, if num: %d, accel engine if id: %d\n",
			dev_name, type_info->address, mtu, dev_interface_num, ae_interface_num);

	/*
	 * Locate the iface
	 */
	ii = ecm_db_iface_find_and_ref_lag(type_info->address);
	if (ii) {
		DEBUG_TRACE("%p: iface established\n", ii);
		return ii;
	}

	/*
	 * No iface - create one
	 */
	nii = ecm_db_iface_alloc();
	if (!nii) {
		DEBUG_WARN("Failed to establish iface\n");
		return NULL;
	}

	/*
	 * Add iface into the database, atomically to avoid races creating the same thing
	 */
	spin_lock_bh(&ecm_interface_lock);
	ii = ecm_db_iface_find_and_ref_lag(type_info->address);
	if (ii) {
		spin_unlock_bh(&ecm_interface_lock);
		ecm_db_iface_deref(nii);
		return ii;
	}
	ecm_db_iface_add_lag(nii, type_info->address, dev_name,
			mtu, dev_interface_num, ae_interface_num, NULL, nii);
	spin_unlock_bh(&ecm_interface_lock);

	DEBUG_TRACE("%p: lag iface established\n", nii);
	return nii;
}
#endif

/*
 * ecm_interface_ethernet_interface_establish()
 *	Returns a reference to a iface of the ETHERNET type, possibly creating one if necessary.
 * Returns NULL on failure or a reference to interface.
 */
static struct ecm_db_iface_instance *ecm_interface_ethernet_interface_establish(struct ecm_db_interface_info_ethernet *type_info,
							char *dev_name, int32_t dev_interface_num, int32_t ae_interface_num, int32_t mtu)
{
	struct ecm_db_iface_instance *nii;
	struct ecm_db_iface_instance *ii;

	DEBUG_INFO("Establish ETHERNET iface: %s with address: %pM, MTU: %d, if num: %d, accel engine if id: %d\n",
			dev_name, type_info->address, mtu, dev_interface_num, ae_interface_num);

	/*
	 * Locate the iface
	 */
	ii = ecm_db_iface_ifidx_find_and_ref_ethernet(type_info->address, dev_interface_num);

	if (ii) {
		DEBUG_TRACE("%p: iface established\n", ii);
		return ii;
	}

	/*
	 * No iface - create one
	 */
	nii = ecm_db_iface_alloc();
	if (!nii) {
		DEBUG_WARN("Failed to establish iface\n");
		return NULL;
	}

	/*
	 * Add iface into the database, atomically to avoid races creating the same thing
	 */
	spin_lock_bh(&ecm_interface_lock);
	ii = ecm_db_iface_ifidx_find_and_ref_ethernet(type_info->address, dev_interface_num);
	if (ii) {
		spin_unlock_bh(&ecm_interface_lock);
		ecm_db_iface_deref(nii);
		return ii;
	}
	ecm_db_iface_add_ethernet(nii, type_info->address, dev_name,
			mtu, dev_interface_num, ae_interface_num, NULL, nii);
	spin_unlock_bh(&ecm_interface_lock);

	DEBUG_TRACE("%p: ethernet iface established\n", nii);
	return nii;
}

#ifdef ECM_INTERFACE_PPP_ENABLE
/*
 * ecm_interface_pppoe_interface_establish()
 *	Returns a reference to a iface of the PPPoE type, possibly creating one if necessary.
 * Returns NULL on failure or a reference to interface.
 */
static struct ecm_db_iface_instance *ecm_interface_pppoe_interface_establish(struct ecm_db_interface_info_pppoe *type_info,
							char *dev_name, int32_t dev_interface_num, int32_t ae_interface_num, int32_t mtu)
{
	struct ecm_db_iface_instance *nii;
	struct ecm_db_iface_instance *ii;

	DEBUG_INFO("Establish PPPoE iface: %s with session id: %u, remote mac: %pM, MTU: %d, if num: %d, accel engine if id: %d\n",
			dev_name, type_info->pppoe_session_id, type_info->remote_mac, mtu, dev_interface_num, ae_interface_num);

	/*
	 * Locate the iface
	 */
	ii = ecm_db_iface_find_and_ref_pppoe(type_info->pppoe_session_id, type_info->remote_mac);
	if (ii) {
		DEBUG_TRACE("%p: iface established\n", ii);
		return ii;
	}

	/*
	 * No iface - create one
	 */
	nii = ecm_db_iface_alloc();
	if (!nii) {
		DEBUG_WARN("Failed to establish iface\n");
		return NULL;
	}

	/*
	 * Add iface into the database, atomically to avoid races creating the same thing
	 */
	spin_lock_bh(&ecm_interface_lock);
	ii = ecm_db_iface_find_and_ref_pppoe(type_info->pppoe_session_id, type_info->remote_mac);
	if (ii) {
		spin_unlock_bh(&ecm_interface_lock);
		ecm_db_iface_deref(nii);
		return ii;
	}
	ecm_db_iface_add_pppoe(nii, type_info->pppoe_session_id, type_info->remote_mac, dev_name,
			mtu, dev_interface_num, ae_interface_num, NULL, nii);
	spin_unlock_bh(&ecm_interface_lock);

	DEBUG_TRACE("%p: pppoe iface established\n", nii);
	return nii;
}
#endif

/*
 * ecm_interface_unknown_interface_establish()
 *	Returns a reference to a iface of the UNKNOWN type, possibly creating one if necessary.
 * Returns NULL on failure or a reference to interface.
 */
static struct ecm_db_iface_instance *ecm_interface_unknown_interface_establish(struct ecm_db_interface_info_unknown *type_info,
							char *dev_name, int32_t dev_interface_num, int32_t ae_interface_num, int32_t mtu)
{
	struct ecm_db_iface_instance *nii;
	struct ecm_db_iface_instance *ii;

	DEBUG_INFO("Establish UNKNOWN iface: %s with os_specific_ident: %u, MTU: %d, if num: %d, accel engine if id: %d\n",
			dev_name, type_info->os_specific_ident, mtu, dev_interface_num, ae_interface_num);

	/*
	 * Locate the iface
	 */
	ii = ecm_db_iface_find_and_ref_unknown(type_info->os_specific_ident);
	if (ii) {
		DEBUG_TRACE("%p: iface established\n", ii);
		return ii;
	}

	/*
	 * No iface - create one
	 */
	nii = ecm_db_iface_alloc();
	if (!nii) {
		DEBUG_WARN("Failed to establish iface\n");
		return NULL;
	}

	/*
	 * Add iface into the database, atomically to avoid races creating the same thing
	 */
	spin_lock_bh(&ecm_interface_lock);
	ii = ecm_db_iface_find_and_ref_unknown(type_info->os_specific_ident);
	if (ii) {
		spin_unlock_bh(&ecm_interface_lock);
		ecm_db_iface_deref(nii);
		return ii;
	}
	ecm_db_iface_add_unknown(nii, type_info->os_specific_ident, dev_name,
			mtu, dev_interface_num, ae_interface_num, NULL, nii);
	spin_unlock_bh(&ecm_interface_lock);

	DEBUG_TRACE("%p: unknown iface established\n", nii);
	return nii;
}

/*
 * ecm_interface_loopback_interface_establish()
 *	Returns a reference to a iface of the LOOPBACK type, possibly creating one if necessary.
 * Returns NULL on failure or a reference to interface.
 */
static struct ecm_db_iface_instance *ecm_interface_loopback_interface_establish(struct ecm_db_interface_info_loopback *type_info,
							char *dev_name, int32_t dev_interface_num, int32_t ae_interface_num, int32_t mtu)
{
	struct ecm_db_iface_instance *nii;
	struct ecm_db_iface_instance *ii;

	DEBUG_INFO("Establish LOOPBACK iface: %s with os_specific_ident: %u, MTU: %d, if num: %d, accel engine if id: %d\n",
			dev_name, type_info->os_specific_ident, mtu, dev_interface_num, ae_interface_num);

	/*
	 * Locate the iface
	 */
	ii = ecm_db_iface_find_and_ref_loopback(type_info->os_specific_ident);
	if (ii) {
		DEBUG_TRACE("%p: iface established\n", ii);
		return ii;
	}

	/*
	 * No iface - create one
	 */
	nii = ecm_db_iface_alloc();
	if (!nii) {
		DEBUG_WARN("Failed to establish iface\n");
		return NULL;
	}

	/*
	 * Add iface into the database, atomically to avoid races creating the same thing
	 */
	spin_lock_bh(&ecm_interface_lock);
	ii = ecm_db_iface_find_and_ref_loopback(type_info->os_specific_ident);
	if (ii) {
		spin_unlock_bh(&ecm_interface_lock);
		ecm_db_iface_deref(nii);
		return ii;
	}
	ecm_db_iface_add_loopback(nii, type_info->os_specific_ident, dev_name,
			mtu, dev_interface_num, ae_interface_num, NULL, nii);
	spin_unlock_bh(&ecm_interface_lock);

	DEBUG_TRACE("%p: loopback iface established\n", nii);
	return nii;
}

#ifdef ECM_INTERFACE_IPSEC_ENABLE
/*
 * ecm_interface_ipsec_tunnel_interface_establish()
 *	Returns a reference to a iface of the IPSEC_TUNNEL type, possibly creating one if necessary.
 * Returns NULL on failure or a reference to interface.
 *
 * NOTE: GGG TODO THIS NEEDS TO TAKE A PROPER APPROACH TO IPSEC TUNNELS USING ENDPOINT ADDRESSING AS THE TYPE INFO KEYS
 */
static struct ecm_db_iface_instance *ecm_interface_ipsec_tunnel_interface_establish(struct ecm_db_interface_info_ipsec_tunnel *type_info,
							char *dev_name, int32_t dev_interface_num, int32_t ae_interface_num, int32_t mtu)
{
	struct ecm_db_iface_instance *nii;
	struct ecm_db_iface_instance *ii;

	DEBUG_INFO("Establish IPSEC_TUNNEL iface: %s with os_specific_ident: %u, MTU: %d, if num: %d, accel engine if id: %d\n",
			dev_name, type_info->os_specific_ident, mtu, dev_interface_num, ae_interface_num);

	/*
	 * Locate the iface
	 */
	ii = ecm_db_iface_find_and_ref_ipsec_tunnel(type_info->os_specific_ident);
	if (ii) {
		DEBUG_TRACE("%p: iface established\n", ii);
		return ii;
	}

	/*
	 * No iface - create one
	 */
	nii = ecm_db_iface_alloc();
	if (!nii) {
		DEBUG_WARN("Failed to establish iface\n");
		return NULL;
	}

	/*
	 * Add iface into the database, atomically to avoid races creating the same thing
	 */
	spin_lock_bh(&ecm_interface_lock);
	ii = ecm_db_iface_find_and_ref_ipsec_tunnel(type_info->os_specific_ident);
	if (ii) {
		spin_unlock_bh(&ecm_interface_lock);
		ecm_db_iface_deref(nii);
		return ii;
	}
	ecm_db_iface_add_ipsec_tunnel(nii, type_info->os_specific_ident, dev_name,
			mtu, dev_interface_num, ae_interface_num, NULL, nii);
	spin_unlock_bh(&ecm_interface_lock);

	DEBUG_TRACE("%p: ipsec_tunnel iface established\n", nii);
	return nii;
}
#endif

#ifdef ECM_INTERFACE_SIT_ENABLE
#ifdef CONFIG_IPV6_SIT_6RD
/*
 * ecm_interface_sit_interface_establish()
 *	Returns a reference to a iface of the SIT type, possibly creating one if necessary.
 * Returns NULL on failure or a reference to interface.
 */
static struct ecm_db_iface_instance *ecm_interface_sit_interface_establish(struct ecm_db_interface_info_sit *type_info,
							char *dev_name, int32_t dev_interface_num, int32_t ae_interface_num, int32_t mtu)
{
	struct ecm_db_iface_instance *nii;
	struct ecm_db_iface_instance *ii;

	DEBUG_INFO("Establish SIT iface: %s with saddr: " ECM_IP_ADDR_OCTAL_FMT ", daddr: " ECM_IP_ADDR_OCTAL_FMT ", MTU: %d, if num: %d, accel engine if id: %d\n",
			dev_name, ECM_IP_ADDR_TO_OCTAL(type_info->saddr), ECM_IP_ADDR_TO_OCTAL(type_info->daddr), mtu, dev_interface_num, ae_interface_num);

	/*
	 * Locate the iface
	 */
	ii = ecm_db_iface_find_and_ref_sit(type_info->saddr, type_info->daddr);
	if (ii) {
		DEBUG_TRACE("%p: iface established\n", ii);
		return ii;
	}

	/*
	 * No iface - create one
	 */
	nii = ecm_db_iface_alloc();
	if (!nii) {
		DEBUG_WARN("Failed to establish iface\n");
		return NULL;
	}

	/*
	 * Add iface into the database, atomically to avoid races creating the same thing
	 */
	spin_lock_bh(&ecm_interface_lock);
	ii = ecm_db_iface_find_and_ref_sit(type_info->saddr, type_info->daddr);
	if (ii) {
		spin_unlock_bh(&ecm_interface_lock);
		ecm_db_iface_deref(nii);
		return ii;
	}
	ecm_db_iface_add_sit(nii, type_info, dev_name, mtu, dev_interface_num,
			ae_interface_num, NULL, nii);
	spin_unlock_bh(&ecm_interface_lock);

	DEBUG_TRACE("%p: sit iface established\n", nii);
	return nii;
}
#endif
#endif

#ifdef ECM_INTERFACE_TUNIPIP6_ENABLE
#ifdef ECM_IPV6_ENABLE
/*
 * ecm_interface_tunipip6_interface_establish()
 *	Returns a reference to a iface of the TUNIPIP6 type, possibly creating one if necessary.
 * Returns NULL on failure or a reference to interface.
 */
static struct ecm_db_iface_instance *ecm_interface_tunipip6_interface_establish(struct ecm_db_interface_info_tunipip6 *type_info,
							char *dev_name, int32_t dev_interface_num, int32_t ae_interface_num, int32_t mtu)
{
	struct ecm_db_iface_instance *nii;
	struct ecm_db_iface_instance *ii;

	DEBUG_INFO("Establish TUNIPIP6 iface: %s with saddr: " ECM_IP_ADDR_OCTAL_FMT ", daddr: " ECM_IP_ADDR_OCTAL_FMT ", MTU: %d, if num: %d, accel engine if id: %d\n",
			dev_name, ECM_IP_ADDR_TO_OCTAL(type_info->saddr), ECM_IP_ADDR_TO_OCTAL(type_info->daddr), mtu, dev_interface_num, ae_interface_num);

	/*
	 * Locate the iface
	 */
	ii = ecm_db_iface_find_and_ref_tunipip6(type_info->saddr, type_info->daddr);
	if (ii) {
		DEBUG_TRACE("%p: iface established\n", ii);
		return ii;
	}

	/*
	 * No iface - create one
	 */
	nii = ecm_db_iface_alloc();
	if (!nii) {
		DEBUG_WARN("Failed to establish iface\n");
		return NULL;
	}

	/*
	 * Add iface into the database, atomically to avoid races creating the same thing
	 */
	spin_lock_bh(&ecm_interface_lock);
	ii = ecm_db_iface_find_and_ref_tunipip6(type_info->saddr, type_info->daddr);
	if (ii) {
		spin_unlock_bh(&ecm_interface_lock);
		ecm_db_iface_deref(nii);
		return ii;
	}
	ecm_db_iface_add_tunipip6(nii, type_info, dev_name, mtu, dev_interface_num,
			ae_interface_num, NULL, nii);
	spin_unlock_bh(&ecm_interface_lock);

	DEBUG_TRACE("%p: tunipip6 iface established\n", nii);
	return nii;
}
#endif
#endif

/*
 * ecm_interface_establish_and_ref()
 *	Establish an interface instance for the given interface detail.
 */
struct ecm_db_iface_instance *ecm_interface_establish_and_ref(struct ecm_front_end_connection_instance *feci,
								struct net_device *dev)
{
	int32_t dev_interface_num;
	char *dev_name;
	int32_t dev_type;
	int32_t dev_mtu;
	int32_t ae_interface_num;
	struct ecm_db_iface_instance *ii;
	union {
		struct ecm_db_interface_info_ethernet ethernet;		/* type == ECM_DB_IFACE_TYPE_ETHERNET */
#ifdef ECM_INTERFACE_VLAN_ENABLE
		struct ecm_db_interface_info_vlan vlan;			/* type == ECM_DB_IFACE_TYPE_VLAN */
#endif
#ifdef ECM_INTERFACE_BOND_ENABLE
		struct ecm_db_interface_info_lag lag;			/* type == ECM_DB_IFACE_TYPE_LAG */
#endif
		struct ecm_db_interface_info_bridge bridge;		/* type == ECM_DB_IFACE_TYPE_BRIDGE */
#ifdef ECM_INTERFACE_PPP_ENABLE
		struct ecm_db_interface_info_pppoe pppoe;		/* type == ECM_DB_IFACE_TYPE_PPPOE */
#endif
		struct ecm_db_interface_info_unknown unknown;		/* type == ECM_DB_IFACE_TYPE_UNKNOWN */
		struct ecm_db_interface_info_loopback loopback;		/* type == ECM_DB_IFACE_TYPE_LOOPBACK */
#ifdef ECM_INTERFACE_IPSEC_ENABLE
		struct ecm_db_interface_info_ipsec_tunnel ipsec_tunnel;	/* type == ECM_DB_IFACE_TYPE_IPSEC_TUNNEL */
#endif
#ifdef ECM_INTERFACE_SIT_ENABLE
		struct ecm_db_interface_info_sit sit;			/* type == ECM_DB_IFACE_TYPE_SIT */
#endif
#ifdef ECM_INTERFACE_TUNIPIP6_ENABLE
#ifdef ECM_IPV6_ENABLE
		struct ecm_db_interface_info_tunipip6 tunipip6;		/* type == ECM_DB_IFACE_TYPE_TUNIPIP6 */
#endif
#endif
	} type_info;

#ifdef ECM_INTERFACE_PPP_ENABLE
	int channel_count;
	struct ppp_channel *ppp_chan[1];
	int channel_protocol;
	struct pppoe_opt addressing;
#endif

	/*
	 * Get basic information about the given device
	 */
	dev_interface_num = dev->ifindex;
	dev_name = dev->name;
	dev_type = dev->type;
	dev_mtu = dev->mtu;

	/*
	 * Does the accel engine recognise this interface?
	 */
	ae_interface_num = feci->ae_interface_number_by_dev_get(dev);

	DEBUG_TRACE("Establish interface instance for device: %p is type: %d, name: %s, ifindex: %d, ae_if: %d, mtu: %d\n",
			dev, dev_type, dev_name, dev_interface_num, ae_interface_num, dev_mtu);

	/*
	 * Extract from the device more type-specific information
	 */
	if (dev_type == ARPHRD_ETHER) {

		/*
		 * Ethernet - but what sub type?
		 */

#ifdef ECM_INTERFACE_VLAN_ENABLE
		/*
		 * VLAN?
		 */
		if (is_vlan_dev(dev)) {
			/*
			 * VLAN master
			 * GGG No locking needed here, ASSUMPTION is that real_dev is held for as long as we have dev.
			 */
			memcpy(type_info.vlan.address, dev->dev_addr, 6);
			type_info.vlan.vlan_tag = vlan_dev_vlan_id(dev);
			type_info.vlan.vlan_tpid = ETH_P_8021Q;
			DEBUG_TRACE("Net device: %p is VLAN, mac: %pM, vlan_id: %x vlan_tpid: %x\n",
					dev, type_info.vlan.address, type_info.vlan.vlan_tag, type_info.vlan.vlan_tpid);

			/*
			 * Establish this type of interface
			 */
			ii = ecm_interface_vlan_interface_establish(&type_info.vlan, dev_name, dev_interface_num, ae_interface_num, dev_mtu);
			goto identifier_update;
		}
#endif

		/*
		 * BRIDGE?
		 */
		if (ecm_front_end_is_bridge_device(dev)) {
			/*
			 * Bridge
			 */
			memcpy(type_info.bridge.address, dev->dev_addr, 6);

			DEBUG_TRACE("Net device: %p is BRIDGE, mac: %pM\n",
					dev, type_info.bridge.address);

			/*
			 * Establish this type of interface
			 */
			ii = ecm_interface_bridge_interface_establish(&type_info.bridge, dev_name, dev_interface_num, ae_interface_num, dev_mtu);
			goto identifier_update;
		}

#ifdef ECM_INTERFACE_BOND_ENABLE
		/*
		 * LAG?
		 */
		if (ecm_front_end_is_lag_master(dev)) {
			/*
			 * Link aggregation
			 */
			memcpy(type_info.lag.address, dev->dev_addr, 6);

			DEBUG_TRACE("Net device: %p is LAG, mac: %pM\n",
					dev, type_info.lag.address);

			/*
			 * Establish this type of interface
			 */
			ii = ecm_interface_lag_interface_establish(&type_info.lag, dev_name, dev_interface_num, ae_interface_num, dev_mtu);
			goto identifier_update;
		}
#endif

		/*
		 * ETHERNET!
		 * Just plain ethernet it seems
		 */
		memcpy(type_info.ethernet.address, dev->dev_addr, 6);
		DEBUG_TRACE("Net device: %p is ETHERNET, mac: %pM\n",
				dev, type_info.ethernet.address);

		/*
		 * Establish this type of interface
		 */
		ii = ecm_interface_ethernet_interface_establish(&type_info.ethernet, dev_name, dev_interface_num, ae_interface_num, dev_mtu);

identifier_update:
		if (ii) {
			/*
			 * An interface identifier/ifindex can be change after network restart. Below
			 * functtion will check interface_identifier present in 'ii' with new dev_interface_num.
			 * If differ then update new ifindex and update the interface identifier hash table.
			 */
			ecm_db_iface_identifier_hash_table_entry_check_and_update(ii, dev_interface_num);
		}

		return ii;
	}

	/*
	 * LOOPBACK?
	 */
	if (dev_type == ARPHRD_LOOPBACK) {
		DEBUG_TRACE("Net device: %p is LOOPBACK type: %d\n", dev, dev_type);
		type_info.loopback.os_specific_ident = dev_interface_num;
		ii = ecm_interface_loopback_interface_establish(&type_info.loopback, dev_name, dev_interface_num, ae_interface_num, dev_mtu);
		return ii;
	}

#ifdef ECM_INTERFACE_IPSEC_ENABLE
	/*
	 * IPSEC?
	 */
	if (dev_type == ECM_ARPHRD_IPSEC_TUNNEL_TYPE) {
		DEBUG_TRACE("Net device: %p is IPSec tunnel type: %d\n", dev, dev_type);
		type_info.ipsec_tunnel.os_specific_ident = dev_interface_num;

		// GGG TODO Flesh this out with tunnel endpoint addressing detail
		ii = ecm_interface_ipsec_tunnel_interface_establish(&type_info.ipsec_tunnel, dev_name, dev_interface_num, ae_interface_num, dev_mtu);
		return ii;
	}
#endif

#ifdef ECM_INTERFACE_SIT_ENABLE
#ifdef CONFIG_IPV6_SIT_6RD
	/*
	 * SIT (6-in-4)?
	 */
	if (dev_type == ARPHRD_SIT) {
		struct ip_tunnel *tunnel;
		struct ip_tunnel_6rd_parm *ip6rd;
		const struct iphdr  *tiph;

		DEBUG_TRACE("Net device: %p is SIT (6-in-4) type: %d\n", dev, dev_type);

		tunnel = (struct ip_tunnel*)netdev_priv(dev);
		ip6rd =  &tunnel->ip6rd;

		/*
		 * Get the Tunnel device IP header info
		 */
		tiph = &tunnel->parms.iph ;

		type_info.sit.prefixlen = ip6rd->prefixlen;
		type_info.sit.relay_prefix = ip6rd->relay_prefix;
		type_info.sit.relay_prefixlen = ip6rd->relay_prefixlen;
		ECM_NIN4_ADDR_TO_IP_ADDR(type_info.sit.saddr, tiph->saddr);
		ECM_NIN4_ADDR_TO_IP_ADDR(type_info.sit.daddr, tiph->daddr);
		type_info.sit.prefix[0] = ntohl(ip6rd->prefix.s6_addr32[0]);
		type_info.sit.prefix[1] = ntohl(ip6rd->prefix.s6_addr32[1]);
		type_info.sit.prefix[2] = ntohl(ip6rd->prefix.s6_addr32[2]);
		type_info.sit.prefix[3] = ntohl(ip6rd->prefix.s6_addr32[3]);
		type_info.sit.ttl = tiph->ttl;
		type_info.sit.tos = tiph->tos;

		ii = ecm_interface_sit_interface_establish(&type_info.sit, dev_name, dev_interface_num, ae_interface_num, dev_mtu);
		return ii;
	}
#endif
#endif

#ifdef ECM_INTERFACE_TUNIPIP6_ENABLE
#ifdef ECM_IPV6_ENABLE
	/*
	 * IPIP6 Tunnel?
	 */
	if (dev_type == ARPHRD_TUNNEL6) {
		struct ip6_tnl *tunnel;
		struct flowi6 *fl6;

		DEBUG_TRACE("Net device: %p is TUNIPIP6 type: %d\n", dev, dev_type);

		/*
		 * Get the tunnel device flow information (discover the output path of the tunnel)
		 */
		tunnel = (struct ip6_tnl *)netdev_priv(dev);
		fl6 = &tunnel->fl.u.ip6;

		ECM_NIN6_ADDR_TO_IP_ADDR(type_info.tunipip6.saddr, fl6->saddr);
		ECM_NIN6_ADDR_TO_IP_ADDR(type_info.tunipip6.daddr, fl6->daddr);
		type_info.tunipip6.hop_limit = tunnel->parms.hop_limit;
		type_info.tunipip6.flags = ntohl(tunnel->parms.flags);
		type_info.tunipip6.flowlabel = fl6->flowlabel;  /* flow Label In kernel is stored in big endian format */

		ii = ecm_interface_tunipip6_interface_establish(&type_info.tunipip6, dev_name, dev_interface_num, ae_interface_num, dev_mtu);
		return ii;
	}
#endif
#endif

	/*
	 * If this is NOT PPP then it is unknown to the ecm
	 */
	if (dev_type != ARPHRD_PPP) {
		DEBUG_TRACE("Net device: %p is UNKNOWN type: %d\n", dev, dev_type);
		type_info.unknown.os_specific_ident = dev_interface_num;

		/*
		 * Establish this type of interface
		 */
		ii = ecm_interface_unknown_interface_establish(&type_info.unknown, dev_name, dev_interface_num, ae_interface_num, dev_mtu);
		return ii;
	}

#ifndef ECM_INTERFACE_PPP_ENABLE
	/*
	 * PPP support is NOT provided for.
	 * Interface is therefore unknown
	 */
	DEBUG_TRACE("Net device: %p is UNKNOWN (PPP Unsupported) type: %d\n", dev, dev_type);
	type_info.unknown.os_specific_ident = dev_interface_num;

	/*
	 * Establish this type of interface
	 */
	ii = ecm_interface_unknown_interface_establish(&type_info.unknown, dev_name, dev_interface_num, ae_interface_num, dev_mtu);
	return ii;
#else
	/*
	 * PPP - but what is the channel type?
	 * First: If this is multi-link then we do not support it
	 */
	if (ppp_is_multilink(dev) > 0) {
		DEBUG_TRACE("Net device: %p is MULTILINK PPP - Unknown to the ECM\n", dev);
		type_info.unknown.os_specific_ident = dev_interface_num;

		/*
		 * Establish this type of interface
		 */
		ii = ecm_interface_unknown_interface_establish(&type_info.unknown, dev_name, dev_interface_num, ae_interface_num, dev_mtu);
		return ii;
	}

	DEBUG_TRACE("Net device: %p is PPP\n", dev);

	/*
	 * Get the PPP channel and then enquire what kind of channel it is
	 * NOTE: Not multilink so only one channel to get.
	 */
	channel_count = ppp_hold_channels(dev, ppp_chan, 1);
	if (channel_count != 1) {
		DEBUG_TRACE("Net device: %p PPP has %d channels - ECM cannot handle this (interface becomes Unknown type)\n",
				dev, channel_count);
		type_info.unknown.os_specific_ident = dev_interface_num;

		/*
		 * Establish this type of interface
		 */
		ii = ecm_interface_unknown_interface_establish(&type_info.unknown, dev_name, dev_interface_num, ae_interface_num, dev_mtu);
		return ii;
	}

	/*
	 * Get channel protocol type
	 * NOTE: Not all PPP channels support channel specific methods.
	 */
	channel_protocol = ppp_channel_get_protocol(ppp_chan[0]);
	if (channel_protocol != PX_PROTO_OE) {
		DEBUG_TRACE("Net device: %p PPP channel protocol: %d - Unknown to the ECM\n", dev, channel_protocol);
		type_info.unknown.os_specific_ident = dev_interface_num;

		/*
		 * Release the channel
		 */
		ppp_release_channels(ppp_chan, 1);

		/*
		 * Establish this type of interface
		 */
		ii = ecm_interface_unknown_interface_establish(&type_info.unknown, dev_name, dev_interface_num, ae_interface_num, dev_mtu);
		return ii;
	}

	/*
	 * PPPoE channel
	 */
	DEBUG_TRACE("Net device: %p PPP channel is PPPoE\n", dev);

	/*
	 * Get PPPoE session information and the underlying device it is using.
	 */
	pppoe_channel_addressing_get(ppp_chan[0], &addressing);
	type_info.pppoe.pppoe_session_id = (uint16_t)ntohs((uint16_t)addressing.pa.sid);
	memcpy(type_info.pppoe.remote_mac, addressing.pa.remote, ETH_ALEN);
	dev_put(addressing.dev);

	/*
	 * Release the channel.  Note that next_dev is still (correctly) held.
	 */
	ppp_release_channels(ppp_chan, 1);

	DEBUG_TRACE("Net device: %p PPPoE session: %x, remote mac: %pM\n",
			dev, type_info.pppoe.pppoe_session_id, type_info.pppoe.remote_mac);

	/*
	 * Establish this type of interface
	 */
	ii = ecm_interface_pppoe_interface_establish(&type_info.pppoe, dev_name, dev_interface_num, ae_interface_num, dev_mtu);
	return ii;
#endif
}
EXPORT_SYMBOL(ecm_interface_establish_and_ref);

#ifdef ECM_MULTICAST_ENABLE
/*
 * ecm_interface_multicast_heirarchy_construct_single()
 * 	Create and return an interface heirarchy for a single interface for a multicast connection
 *
 * 	src_addr	IP source address
 * 	dest_addr	IP Destination address/Group Address
 * 	interface	Pointer to a single multicast interface heirarchy
 *	given_dest_dev	Netdev pointer for destination interface
 *	br_slave_dev	Netdev pointer to a bridge slave device. It could be NULL in case of pure
 *			routed flow without any bridge interface in destination dev list.
 */
static uint32_t ecm_interface_multicast_heirarchy_construct_single(struct ecm_front_end_connection_instance *feci, ip_addr_t src_addr,
								   ip_addr_t dest_addr, struct ecm_db_iface_instance *interface,
								   struct net_device *given_dest_dev, struct net_device *br_slave_dev,
								   uint8_t *src_node_addr, bool is_routed, __be16 *layer4hdr)
{
	struct ecm_db_iface_instance *to_list_single[ECM_DB_IFACE_HEIRARCHY_MAX];
	struct ecm_db_iface_instance **ifaces;
	struct ecm_db_iface_instance *ii_temp;
	struct net_device *dest_dev;
	int32_t current_interface_index;
	int32_t interfaces_cnt = 0;
        int32_t dest_dev_type;

	dest_dev = given_dest_dev;
	dev_hold(dest_dev);
	dest_dev_type = dest_dev->type;
	current_interface_index = ECM_DB_IFACE_HEIRARCHY_MAX;

	while (current_interface_index > 0) {
		struct ecm_db_iface_instance *ii;
		struct net_device *next_dev;

		/*
		 * Get the ecm db interface instance for the device at hand
		 */
		ii = ecm_interface_establish_and_ref(feci, dest_dev);
		interfaces_cnt++;

		/*
		 * If the interface could not be established then we abort
		 */
		if (!ii) {
			DEBUG_WARN("Failed to establish interface: %p, name: %s\n", dest_dev, dest_dev->name);
			dev_put(dest_dev);

			/*
			 * Release the interfaces heirarchy we constructed to this point.
			 */
			ecm_db_multicast_copy_if_heirarchy(to_list_single, interface);
			ecm_db_connection_interfaces_deref(to_list_single, current_interface_index);
			return ECM_DB_IFACE_HEIRARCHY_MAX;
		}

		/*
		 * Record the interface instance into the *ifaces
		 */
		current_interface_index--;
		ii_temp = ecm_db_multicast_if_instance_get_at_index(interface, current_interface_index);
		ifaces = (struct ecm_db_iface_instance **)ii_temp;
		*ifaces = ii;

		/*
		 * Now we have to figure out what the next device will be (in the transmission path)
		 */
		do {
#ifdef ECM_INTERFACE_PPP_ENABLE
			int channel_count;
			struct ppp_channel *ppp_chan[1];
			int channel_protocol;
			struct pppoe_opt addressing;
#endif
			DEBUG_TRACE("Net device: %p is type: %d, name: %s\n", dest_dev, dest_dev_type, dest_dev->name);
			next_dev = NULL;

			if (dest_dev_type == ARPHRD_ETHER) {
				/*
				 * Ethernet - but what sub type?
				 */

				/*
				 * VLAN?
				 */
				if (is_vlan_dev(dest_dev)) {
					/*
					 * VLAN master
					 * No locking needed here, ASSUMPTION is that real_dev is held for as long as we have dev.
					 */
					next_dev = vlan_dev_real_dev(dest_dev);
					dev_hold(next_dev);
					DEBUG_TRACE("Net device: %p is VLAN, slave dev: %p (%s)\n",
							dest_dev, next_dev, next_dev->name);
					break;
				}

				/*
				 * BRIDGE?
				 */
				if (ecm_front_end_is_bridge_device(dest_dev)) {
					if (!ecm_front_end_is_bridge_port(br_slave_dev)) {
						DEBUG_ASSERT(NULL, "%p: expected only bridge slave here\n", interface);

						/*
						 * Release the interfaces heirarchy we constructed to this point.
						 */
						ecm_db_multicast_copy_if_heirarchy(to_list_single, interface);
						ecm_db_connection_interfaces_deref(to_list_single, current_interface_index);
						dev_put(dest_dev);
						return ECM_DB_IFACE_HEIRARCHY_MAX;
					}

					next_dev = br_slave_dev;
					if (!next_dev) {
						DEBUG_WARN("Unable to obtain output port \n");

						/*
						 * Release the interfaces heirarchy we constructed to this point.
						 */
						ecm_db_multicast_copy_if_heirarchy(to_list_single, interface);
						ecm_db_connection_interfaces_deref(to_list_single, current_interface_index);
						dev_put(dest_dev);
						return ECM_DB_IFACE_HEIRARCHY_MAX;
					}
					DEBUG_TRACE("Net device: %p is BRIDGE, next_dev: %p (%s)\n", dest_dev, next_dev, next_dev->name);
					dev_hold(next_dev);
					break;
				}

#ifdef ECM_INTERFACE_BOND_ENABLE
				/*
				 * LAG?
				 */
				if (ecm_front_end_is_lag_master(dest_dev)) {
					/*
					 * Link aggregation
					 * Figure out which slave device of the link aggregation will be used to reach the destination.
					 */
					uint32_t src_addr_32 = 0;
					uint32_t dest_addr_32 = 0;
					uint8_t src_mac_addr[ETH_ALEN];
					uint8_t dest_mac_addr[ETH_ALEN];

					memset(src_mac_addr, 0, ETH_ALEN);
					memset(dest_mac_addr, 0, ETH_ALEN);

					if (ECM_IP_ADDR_IS_V4(src_addr)) {
						ECM_IP_ADDR_TO_NIN4_ADDR(src_addr_32, src_addr);
						ECM_IP_ADDR_TO_NIN4_ADDR(dest_addr_32, dest_addr);
					}

					if (!is_routed) {
						memcpy(src_mac_addr, src_node_addr, ETH_ALEN);
					} else {
						struct net_device *dest_dev_master;

						/*
						 * Use appropriate source MAC address for routed packets
						 */
						dest_dev_master = ecm_interface_get_and_hold_dev_master(dest_dev);
						if (dest_dev_master) {
							memcpy(src_mac_addr, dest_dev_master->dev_addr, ETH_ALEN);
							dev_put(dest_dev_master);
						} else {
							memcpy(src_mac_addr, dest_dev->dev_addr, ETH_ALEN);
						}
					}

					/*
					 * Create Destination MAC address using IP multicast destination address
					 */
					ecm_translate_multicast_mac(dest_addr, dest_mac_addr);

					if (ECM_IP_ADDR_IS_V4(src_addr)) {
						next_dev = bond_get_tx_dev(NULL, src_mac_addr, dest_mac_addr,
									   &src_addr_32, &dest_addr_32,
									   htons((uint16_t)ETH_P_IP), dest_dev, layer4hdr);
					} else {
						next_dev = bond_get_tx_dev(NULL, src_mac_addr, dest_mac_addr,
									   src_addr, dest_addr,
									   htons((uint16_t)ETH_P_IPV6), dest_dev, NULL);
					}

					if (!(next_dev && netif_carrier_ok(next_dev))) {
						DEBUG_WARN("Unable to obtain LAG output slave device\n");
						dev_put(dest_dev);

						/*
						 * Release the interfaces heirarchy we constructed to this point.
						 */
						ecm_db_multicast_copy_if_heirarchy(to_list_single, interface);
						ecm_db_connection_interfaces_deref(to_list_single, current_interface_index);
						return ECM_DB_IFACE_HEIRARCHY_MAX;
					}

					dev_hold(next_dev);
					DEBUG_TRACE("Net device: %p is LAG, slave dev: %p (%s)\n", dest_dev, next_dev, next_dev->name);
					break;
				}
#endif

				/*
				 * ETHERNET!
				 * Just plain ethernet it seems.
				 */
				DEBUG_TRACE("Net device: %p is ETHERNET\n", dest_dev);
				break;
			}

			/*
			 * LOOPBACK?
			 */
			if (dest_dev_type == ARPHRD_LOOPBACK) {
				DEBUG_TRACE("Net device: %p is LOOPBACK type: %d\n", dest_dev, dest_dev_type);
				break;
			}

			/*
			 * IPSEC?
			 */
			if (dest_dev_type == ECM_ARPHRD_IPSEC_TUNNEL_TYPE) {
				DEBUG_TRACE("Net device: %p is IPSec tunnel type: %d\n", dest_dev, dest_dev_type);
				/*
				 * TODO Figure out the next device the tunnel is using...
				 */
				break;
			}

			/*
			 * SIT (6-in-4)?
			 */
			if (dest_dev_type == ARPHRD_SIT) {
				DEBUG_TRACE("Net device: %p is SIT (6-in-4) type: %d\n", dest_dev, dest_dev_type);
				/*
				 * TODO Figure out the next device the tunnel is using...
				 */
				break;
			}

			/*
			 * IPIP6 Tunnel?
			 */
			if (dest_dev_type == ARPHRD_TUNNEL6) {
				DEBUG_TRACE("Net device: %p is TUNIPIP6 type: %d\n", dest_dev, dest_dev_type);
				/*
				 * TODO Figure out the next device the tunnel is using...
				 */
				break;
			}

			/*
			 * If this is NOT PPP then it is unknown to the ecm and we cannot figure out it's next device.
			 */
			if (dest_dev_type != ARPHRD_PPP) {
				DEBUG_TRACE("Net device: %p is UNKNOWN type: %d\n", dest_dev, dest_dev_type);
				break;
			}

#ifndef ECM_INTERFACE_PPP_ENABLE
			DEBUG_TRACE("Net device: %p is UNKNOWN (PPP Unsupported) type: %d\n", dest_dev, dest_dev_type);
#else
			/*
			 * PPP - but what is the channel type?
			 * First: If this is multi-link then we do not support it
			 */
			if (ppp_is_multilink(dest_dev) > 0) {
				DEBUG_TRACE("Net device: %p is MULTILINK PPP - Unknown to the ECM\n", dest_dev);
				break;
			}

			DEBUG_TRACE("Net device: %p is PPP\n", dest_dev);

			/*
			 * Get the PPP channel and then enquire what kind of channel it is
			 * NOTE: Not multilink so only one channel to get.
			 */
			channel_count = ppp_hold_channels(dest_dev, ppp_chan, 1);
			if (channel_count != 1) {
				DEBUG_TRACE("Net device: %p PPP has %d channels - Unknown to the ECM\n",
						dest_dev, channel_count);
				break;
			}

			/*
			 * Get channel protocol type
			 * NOTE: Not all PPP channels support channel specific methods.
			 */
			channel_protocol = ppp_channel_get_protocol(ppp_chan[0]);
			if (channel_protocol != PX_PROTO_OE) {
				DEBUG_TRACE("Net device: %p PPP channel protocol: %d - Unknown to the ECM\n",
						dest_dev, channel_protocol);

				/*
				 * Release the channel
				 */
				ppp_release_channels(ppp_chan, 1);

				break;
			}

			/*
			 * PPPoE channel
			 */
			DEBUG_TRACE("Net device: %p PPP channel is PPPoE\n", dest_dev);

			/*
			 * Get PPPoE session information and the underlying device it is using.
			 */
			pppoe_channel_addressing_get(ppp_chan[0], &addressing);

			/*
			 * Copy the dev hold into this, we will release the hold later
			 */
			next_dev = addressing.dev;

			/*
			 * Release the channel.  Note that next_dev is still (correctly) held.
			 */
			ppp_release_channels(ppp_chan, 1);
#endif
		} while (false);

		/*
		 * No longer need dest_dev as it may become next_dev
		 */
		dev_put(dest_dev);

		/*
		 * Check out the next_dev, if any
		 */
		if (!next_dev) {
			int32_t i __attribute__((unused));
			DEBUG_INFO("Completed interface heirarchy construct with first interface @: %d\n", current_interface_index);
#if DEBUG_LEVEL > 1
			ecm_db_multicast_copy_if_heirarchy(to_list_single, interface);
			for (i = current_interface_index; i < ECM_DB_IFACE_HEIRARCHY_MAX; ++i) {
				DEBUG_TRACE("\tInterface @ %d: %p, type: %d, name: %s\n", \
						i, to_list_single[i], ecm_db_connection_iface_type_get(to_list_single[i]), \
						ecm_db_interface_type_to_string(ecm_db_connection_iface_type_get(to_list_single[i])));
			}
#endif
			return current_interface_index;
		}

		/*
		 * dest_dev becomes next_dev
		 */
		dest_dev = next_dev;
		dest_dev_type = dest_dev->type;
	}

	dev_put(dest_dev);

	ecm_db_multicast_copy_if_heirarchy(to_list_single, interface);
	ecm_db_connection_interfaces_deref(to_list_single, current_interface_index);
	return ECM_DB_IFACE_HEIRARCHY_MAX;
}

/*
 * ecm_interface_multicast_heirarchy_construct_routed()
 * 	Create destination interface heirarchy for a routed multicast connectiona
 *
 *	interfaces	Pointer to the 2-D array of multicast interface heirarchies
 *	in_dev		Pointer to the source netdev
 *	packet_src_addr	Source IP of the multicast flow
 *	packet_dest_addr Group(dest) IP of the multicast flow
 *	max_dst		Maximum number of netdev joined the multicast group
 *	dst_if_index_base An array of if index joined the multicast group
 *	interface_first_base An array of the index of the first interface in the list
 */
int32_t ecm_interface_multicast_heirarchy_construct_routed(struct ecm_front_end_connection_instance *feci,
								struct ecm_db_iface_instance *interfaces,
								struct net_device *in_dev,
								ip_addr_t packet_src_addr,
								ip_addr_t packet_dest_addr, uint8_t max_if,
								uint32_t *dst_if_index_base,
								uint32_t *interface_first_base,
								__be16 *layer4hdr)
{
	struct ecm_db_iface_instance *to_list_single[ECM_DB_IFACE_HEIRARCHY_MAX];
	struct ecm_db_iface_instance *ifaces;
	struct net_device *dest_dev = NULL;
	struct net_device *br_dev_src = NULL;
	uint32_t *dst_if_index;
	uint32_t *interface_first;
	uint32_t br_if;
	uint32_t valid_if;
	int32_t if_num;
	int32_t dest_dev_type;
	int if_index;
	int ii_cnt;
	int total_ii_count = 0;
	bool src_dev_is_bridge = false;

	DEBUG_TRACE("Construct interface heirarchy for dest_addr: " ECM_IP_ADDR_DOT_FMT " src_addr: " ECM_IP_ADDR_DOT_FMT "total destination ifs %d\n",
			ECM_IP_ADDR_TO_DOT(packet_dest_addr), ECM_IP_ADDR_TO_DOT(packet_src_addr), max_if);

	/*
	 * Check if the source net_dev is a bridge slave.
	 */
	if (in_dev) {
		if (ecm_front_end_is_bridge_port(in_dev)) {
			src_dev_is_bridge = true;
			br_dev_src = in_dev->master;

			/*
	 		 * The source net_dev found as bridge slave. In case of routed interface
			 * heirarchy MFC is not aware of any other bridge slave has joined the same
			 * multicast group as a destination interface. Therfore we assume there
			 * are bridge slaves present in multicast destination interface list
			 * and increase the max_if by one.
			 */
			max_if++;
		}
	}

	ii_cnt = 0;
	br_if = if_num = 0;

	/*
	 * This loop is for creating the destination interface hierarchy list.
	 * We take the destination interface array we got from MFC (in form of ifindex array)
	 * as input for this.
	 */
	for (if_index = 0, valid_if = 0; if_index < max_if; if_index++) {
		dst_if_index = ecm_db_multicast_if_first_get_at_index(dst_if_index_base, if_index);

		if (*dst_if_index == ECM_INTERFACE_LOOPBACK_DEV_INDEX) {
			continue;
		}

		dest_dev = dev_get_by_index(&init_net, *dst_if_index);
		if (!dest_dev) {
			if (!src_dev_is_bridge) {
				int i;

				/*
				 * If already constructed any interface heirarchies before hitting
				 * this error condition then Deref all interface heirarchies.
				 */
				if (valid_if > 0) {
					for (i = 0; i < valid_if; i++) {
						ifaces = ecm_db_multicast_if_heirarchy_get(interfaces, i);
						ecm_db_multicast_copy_if_heirarchy(to_list_single, ifaces);
						ecm_db_connection_interfaces_deref(to_list_single, interface_first_base[i]);
					}
				}

				/*
				 * If valid netdev not found, Return 0
				 */
				return 0;
			}
			dest_dev = br_dev_src;
			dev_hold(dest_dev);
		}

		dest_dev_type = dest_dev->type;

		if (ecm_front_end_is_bridge_device(dest_dev)) {
			struct net_device *mc_br_slave_dev = NULL;
			uint32_t mc_max_dst = ECM_DB_MULTICAST_IF_MAX;
			uint32_t mc_dst_if_index[ECM_DB_MULTICAST_IF_MAX];

			if (ECM_IP_ADDR_IS_V4(packet_src_addr)) {
				if_num = mc_bridge_ipv4_get_if(dest_dev, htonl((packet_src_addr[0])), htonl(packet_dest_addr[0]), mc_max_dst, mc_dst_if_index);
			} else {
				struct in6_addr origin6;
				struct in6_addr group6;
				ECM_IP_ADDR_TO_NIN6_ADDR(origin6, packet_src_addr);
				ECM_IP_ADDR_TO_NIN6_ADDR(group6, packet_dest_addr);
				if_num = mc_bridge_ipv6_get_if(dest_dev, &origin6, &group6, mc_max_dst, mc_dst_if_index);
			}

			if ((if_num < 0) || (if_num > ECM_DB_MULTICAST_IF_MAX)) {
				int i;
				DEBUG_WARN("MCS is not ready\n");

				/*
				 * If already constructed any interface heirarchies before hitting
				 * this error condition then Deref all interface heirarchies.
				 */
				if (valid_if > 0) {
					for (i = 0; i < valid_if; i++) {
						ifaces = ecm_db_multicast_if_heirarchy_get(interfaces, i);
						ecm_db_multicast_copy_if_heirarchy(to_list_single, ifaces);
						ecm_db_connection_interfaces_deref(to_list_single, interface_first_base[i]);
					}
				}

				dev_put(dest_dev);
				return 0;
			}

			for (br_if = 0; br_if < if_num; br_if++) {
				mc_br_slave_dev = dev_get_by_index(&init_net, mc_dst_if_index[br_if]);
				if (!mc_br_slave_dev) {
					continue;
				}

				if ((valid_if + br_if) > ECM_DB_MULTICAST_IF_MAX) {
					int i;

					/*
					 * If already constructed any interface heirarchies before hitting
					 * this error condition then Deref all interface heirarchies.
					 */
					for (i = 0; i < (valid_if + br_if); i++) {
						ifaces = ecm_db_multicast_if_heirarchy_get(interfaces, i);
						ecm_db_multicast_copy_if_heirarchy(to_list_single, ifaces);
						ecm_db_connection_interfaces_deref(to_list_single, interface_first_base[i]);
					}

					dev_put(dest_dev);
					dev_put(mc_br_slave_dev);
					return 0;
				}

				ifaces = ecm_db_multicast_if_heirarchy_get(interfaces, valid_if + br_if);
				/*
				 * Construct a single interface heirarchy of a multicast dev.
				 */
				ii_cnt = ecm_interface_multicast_heirarchy_construct_single(feci, packet_src_addr, packet_dest_addr, ifaces, dest_dev, mc_br_slave_dev, NULL, true, layer4hdr);
				if (ii_cnt == ECM_DB_IFACE_HEIRARCHY_MAX) {

					/*
					 * If already constructed any interface heirarchies before hitting
					 * this error condition then Deref all interface heirarchies.
					 */
					if ((valid_if + br_if) > 0) {
						int i;
						for (i = 0; i < (valid_if + br_if); i++) {
							ifaces = ecm_db_multicast_if_heirarchy_get(interfaces, i);
							ecm_db_multicast_copy_if_heirarchy(to_list_single, ifaces);
							ecm_db_connection_interfaces_deref(to_list_single, interface_first_base[i]);
						}
					}

					dev_put(dest_dev);
					dev_put(mc_br_slave_dev);
					return 0;
				}

				interface_first = ecm_db_multicast_if_first_get_at_index(interface_first_base, (valid_if + br_if));
				*interface_first = ii_cnt;
				total_ii_count += ii_cnt;
				dev_put(mc_br_slave_dev);
			}

			valid_if += br_if;

		} else {

			DEBUG_ASSERT(valid_if < ECM_DB_MULTICAST_IF_MAX, "Bad array index size %d\n", valid_if);
			ifaces = ecm_db_multicast_if_heirarchy_get(interfaces, valid_if);
			/*
			 * Construct a single interface heirarchy of a multicast dev.
			 */
			ii_cnt = ecm_interface_multicast_heirarchy_construct_single(feci, packet_src_addr, packet_dest_addr, ifaces, dest_dev, NULL, NULL, true, layer4hdr);
			if (ii_cnt == ECM_DB_IFACE_HEIRARCHY_MAX) {

				/*
				 * If already constructed any interface heirarchies before hitting
				 * this error condition then Deref all interface heirarchies.
				 */
				if (valid_if > 0) {
					int i;
					for (i = 0; i < valid_if; i++) {
						ifaces = ecm_db_multicast_if_heirarchy_get(interfaces, i);
						ecm_db_multicast_copy_if_heirarchy(to_list_single, ifaces);
						ecm_db_connection_interfaces_deref(to_list_single, interface_first_base[i]);
					}
				}

				dev_put(dest_dev);
				return 0;
			}

			interface_first = ecm_db_multicast_if_first_get_at_index(interface_first_base, valid_if);
			*interface_first = ii_cnt;
			total_ii_count += ii_cnt;
			valid_if++;
		}

		dev_put(dest_dev);
	}
	return total_ii_count;
}
EXPORT_SYMBOL(ecm_interface_multicast_heirarchy_construct_routed);

/*
 * ecm_interface_multicast_heirarchy_construct_bridged()
 * 	This function called when the Hyfi bridge snooper has IGMP/IMLD updates, this function
 * 	creates destination interface heirarchy for a bridged multicast connection.
 *
 *	interfaces	Pointer to the 2-D array of multicast interface heirarchies
 *	dest_dev	Pointer to the destination dev, here dest_dev is always a bridge type
 *	packet_src_addr	Source IP of the multicast flow
 *	packet_dest_addr Group(dest) IP of the multicast flow
 *	mc_max_dst	Maximum number of bridge slaves joined the multicast group
 *	mc_dst_if_index_base An array of if index joined the multicast group
 *	interface_first_base An array of the index of the first interface in the list
 */
int32_t ecm_interface_multicast_heirarchy_construct_bridged(struct ecm_front_end_connection_instance *feci,
						     struct ecm_db_iface_instance *interfaces, struct net_device *dest_dev,
						     ip_addr_t packet_src_addr, ip_addr_t packet_dest_addr, uint8_t mc_max_dst,
						     int *mc_dst_if_index_base, uint32_t *interface_first_base, uint8_t *src_node_addr,
						     __be16 *layer4hdr)
{
	struct ecm_db_iface_instance *to_list_single[ECM_DB_IFACE_HEIRARCHY_MAX];
	struct ecm_db_iface_instance *ifaces;
	struct net_device *mc_br_slave_dev = NULL;
	uint32_t *interface_first;
	int *mc_dst_if_index;
	int valid_if;
	int ii_cnt = 0;
        int br_if;
        int total_ii_cnt = 0;

	/*
	 * Go through the newly joined interface index one by one and
	 * create an interface heirarchy for each valid interface.
	 */
	for (br_if = 0, valid_if = 0; br_if < mc_max_dst; br_if++) {
		mc_dst_if_index = (int *)ecm_db_multicast_if_num_get_at_index(mc_dst_if_index_base, br_if);
		mc_br_slave_dev = dev_get_by_index(&init_net, *mc_dst_if_index);
		if (!mc_br_slave_dev) {

			/*
			 * If already constructed any interface heirarchies before hitting
			 * this error condition then Deref all interface heirarchies.
			 */
			if (valid_if > 0) {
				int i;
				for (i = 0; i < valid_if; i++) {
					ifaces = ecm_db_multicast_if_heirarchy_get(interfaces, i);
					ecm_db_multicast_copy_if_heirarchy(to_list_single, ifaces);
					ecm_db_connection_interfaces_deref(to_list_single, interface_first_base[i]);
				}
			}

			/*
			 * If valid netdev not found, Return 0
			 */
			return 0;
		}

		if (valid_if > ECM_DB_MULTICAST_IF_MAX) {
			int i;

			/*
			 * If already constructed any interface heirarchies before hitting
			 * this error condition then Deref all interface heirarchies.
			 */
			for (i = 0; i < valid_if; i++) {
				ifaces = ecm_db_multicast_if_heirarchy_get(interfaces, i);
				ecm_db_multicast_copy_if_heirarchy(to_list_single, ifaces);
				ecm_db_connection_interfaces_deref(to_list_single, interface_first_base[i]);
			}

			dev_put(mc_br_slave_dev);
			return 0;
		}

		ifaces = ecm_db_multicast_if_heirarchy_get(interfaces, valid_if);

		/*
		 * Construct a single interface heirarchy of a multicast dev.
		 */
		ii_cnt = ecm_interface_multicast_heirarchy_construct_single(feci, packet_src_addr, packet_dest_addr, ifaces, dest_dev, mc_br_slave_dev, src_node_addr, false, layer4hdr);
		if (ii_cnt == ECM_DB_IFACE_HEIRARCHY_MAX) {

			/*
			 * If already constructed any interface heirarchies before hitting
			 * this error condition then Deref all interface heirarchies.
			 */
			if (valid_if > 0) {
			int i;
				for (i = 0; i < valid_if; i++) {
					ifaces = ecm_db_multicast_if_heirarchy_get(interfaces, i);
					ecm_db_multicast_copy_if_heirarchy(to_list_single, ifaces);
					ecm_db_connection_interfaces_deref(to_list_single, interface_first_base[i]);
				}
			}

			dev_put(mc_br_slave_dev);
			return 0;
		}

		interface_first = ecm_db_multicast_if_first_get_at_index(interface_first_base, valid_if);
		*interface_first = ii_cnt;
		total_ii_cnt += ii_cnt;
		valid_if++;
		dev_put(mc_br_slave_dev);
	}

	return ii_cnt;
}
EXPORT_SYMBOL(ecm_interface_multicast_heirarchy_construct_bridged);
#endif

/*
 * ecm_interface_heirarchy_construct()
 *	Construct an interface heirarchy.
 *
 * Using the given addressing, locate the interface heirarchy used to emit packets to that destination.
 * This is the heirarchy of interfaces a packet would transit to emit from the device.
 *
 * We will use the given src/dest devices when is_routed is false.
 * When is_routed is true we will try routing tables first, failing back to any given.
 *
 * For example, with this network arrangement:
 *
 * PPPoE--VLAN--BRIDGE--BRIDGE_PORT(LAG_MASTER)--LAG_SLAVE_0--10.22.33.11
 *
 * Given the packet_dest_addr IP address 10.22.33.11 this will create an interface heirarchy (in interracfes[]) of:
 * LAG_SLAVE_0 @ [ECM_DB_IFACE_HEIRARCHY_MAX - 5]
 * LAG_MASTER @ [ECM_DB_IFACE_HEIRARCHY_MAX - 4]
 * BRIDGE @ [ECM_DB_IFACE_HEIRARCHY_MAX - 3]
 * VLAN @ [ECM_DB_IFACE_HEIRARCHY_MAX - 2]
 * PPPOE @ [ECM_DB_IFACE_HEIRARCHY_MAX - 1]
 * The value returned is (ECM_DB_IFACE_HEIRARCHY_MAX - 5)
 *
 * IMPORTANT: This function will return any known interfaces in the database, when interfaces do not exist in the database
 * they will be created and added automatically to the database.
 */
int32_t ecm_interface_heirarchy_construct(struct ecm_front_end_connection_instance *feci,
						struct ecm_db_iface_instance *interfaces[],
						ip_addr_t packet_src_addr,
						ip_addr_t packet_dest_addr,
						int ip_version, int packet_protocol,
						struct net_device *given_dest_dev,
						bool is_routed, struct net_device *given_src_dev,
						uint8_t *dest_node_addr, uint8_t *src_node_addr,
						__be16 *layer4hdr)
{
	int protocol;
	ip_addr_t src_addr;
	ip_addr_t dest_addr;
	struct net_device *dest_dev;
	char *dest_dev_name;
	int32_t dest_dev_type;
	struct net_device *src_dev;
	char *src_dev_name;
	int32_t src_dev_type;
	int32_t current_interface_index;
	bool from_local_addr;
	bool next_dest_addr_valid;
	bool next_dest_node_addr_valid;
	ip_addr_t next_dest_addr;
	uint8_t next_dest_node_addr[ETH_ALEN] = {0};

	/*
	 * Get a big endian of the IPv4 address we have been given as our starting point.
	 */
	protocol = packet_protocol;
	ECM_IP_ADDR_COPY(src_addr, packet_src_addr);
	ECM_IP_ADDR_COPY(dest_addr, packet_dest_addr);

	if (ip_version == 4) {
		DEBUG_TRACE("Construct interface heirarchy for from src_addr: " ECM_IP_ADDR_DOT_FMT " to dest_addr: " ECM_IP_ADDR_DOT_FMT ", protocol: %d\n",
				ECM_IP_ADDR_TO_DOT(src_addr), ECM_IP_ADDR_TO_DOT(dest_addr), protocol);
#ifdef ECM_IPV6_ENABLE
	} else if (ip_version == 6) {
		DEBUG_TRACE("Construct interface heirarchy for from src_addr: " ECM_IP_ADDR_OCTAL_FMT " to dest_addr: " ECM_IP_ADDR_OCTAL_FMT ", protocol: %d\n",
				ECM_IP_ADDR_TO_OCTAL(src_addr), ECM_IP_ADDR_TO_OCTAL(dest_addr), protocol);
#endif
	} else {
		DEBUG_WARN("Wrong IP protocol: %d\n", ip_version);
		return ECM_DB_IFACE_HEIRARCHY_MAX;
	}

	/*
	 * Get device to reach the given destination address.
	 * If the heirarchy is for a routed connection we must try route lookup first, falling back to any given_dest_dev.
	 * If the heirarchy is NOT for a routed connection we try the given_dest_dev first, followed by routed lookup.
	 */
	from_local_addr = false;
	if (is_routed) {
		dest_dev = ecm_interface_dev_find_by_addr(dest_addr, &from_local_addr);
		if (!dest_dev && given_dest_dev) {
			/*
			 * Fall back to any given
			 */
			dest_dev = given_dest_dev;
			dev_hold(dest_dev);
		}
	} else if (given_dest_dev) {
		dest_dev = given_dest_dev;
		dev_hold(dest_dev);
	} else {
		/*
		 * Fall back to routed look up
		 */
		dest_dev = ecm_interface_dev_find_by_addr(dest_addr, &from_local_addr);
	}

	/*
	 * GGG ALERT: If the address is a local address and protocol is an IP tunnel
	 * then this connection is a tunnel endpoint made to this device.
	 * In which case we circumvent all proper procedure and just hack the devices to make stuff work.
	 * GGG TODO THIS MUST BE FIXED - WE MUST USE THE INTERFACE HIERARCHY FOR ITS INTENDED PURPOSE TO
	 * PARSE THE DEVICES AND WORK OUT THE PROPER INTERFACES INVOLVED.
	 * E.G. IF WE TRIED TO RUN A TUNNEL OVER A VLAN OR QINQ THIS WILL BREAK AS WE DON'T DISCOVER THAT HIERARCHY
	 */
	if (dest_dev && from_local_addr) {
		if (((ip_version == 4) && (protocol == IPPROTO_IPV6)) ||
				((ip_version == 6) && (protocol == IPPROTO_IPIP))) {
			dev_put(dest_dev);
			dest_dev = given_dest_dev;
			if (dest_dev) {
				dev_hold(dest_dev);
				if (ip_version == 4) {
					DEBUG_TRACE("HACK: %s tunnel packet with dest_addr: " ECM_IP_ADDR_DOT_FMT " uses dev: %p(%s)\n", "IPV6", ECM_IP_ADDR_TO_DOT(dest_addr), dest_dev, dest_dev->name);
				} else {
					DEBUG_TRACE("HACK: %s tunnel packet with dest_addr: " ECM_IP_ADDR_OCTAL_FMT " uses dev: %p(%s)\n", "IPIP", ECM_IP_ADDR_TO_OCTAL(dest_addr), dest_dev, dest_dev->name);
				}
			}
		}
	}
	if (!dest_dev) {
		DEBUG_WARN("dest_addr: " ECM_IP_ADDR_OCTAL_FMT " - cannot locate device\n", ECM_IP_ADDR_TO_OCTAL(dest_addr));
		return ECM_DB_IFACE_HEIRARCHY_MAX;
	}
	dest_dev_name = dest_dev->name;
	dest_dev_type = dest_dev->type;

	/*
	 * Get device to reach the given source address.
	 * If the heirarchy is for a routed connection we must try route lookup first, falling back to any given_src_dev.
	 * If the heirarchy is NOT for a routed connection we try the given_src_dev first, followed by routed lookup.
	 */
	from_local_addr = false;
	if (is_routed) {
		src_dev = ecm_interface_dev_find_by_addr(src_addr, &from_local_addr);
		if (!src_dev && given_src_dev) {
			/*
			 * Fall back to any given
			 */
			src_dev = given_src_dev;
			dev_hold(src_dev);
		}
	} else if (given_src_dev) {
		src_dev = given_src_dev;
		dev_hold(src_dev);
	} else {
		/*
		 * Fall back to routed look up
		 */
		src_dev = ecm_interface_dev_find_by_addr(src_addr, &from_local_addr);
	}

	/*
	 * GGG ALERT: If the address is a local address and protocol is an IP tunnel
	 * then this connection is a tunnel endpoint made to this device.
	 * In which case we circumvent all proper procedure and just hack the devices to make stuff work.
	 * GGG TODO THIS MUST BE FIXED - WE MUST USE THE INTERFACE HIERARCHY FOR ITS INTENDED PURPOSE TO
	 * PARSE THE DEVICES AND WORK OUT THE PROPER INTERFACES INVOLVED.
	 * E.G. IF WE TRIED TO RUN A TUNNEL OVER A VLAN OR QINQ THIS WILL BREAK AS WE DON'T DISCOVER THAT HIERARCHY
	 */
	if (src_dev && from_local_addr) {
		if (((ip_version == 4) && (protocol == IPPROTO_IPV6)) ||
				((ip_version == 6) && (protocol == IPPROTO_IPIP))) {
			dev_put(src_dev);
			src_dev = given_src_dev;
			if (src_dev) {
				dev_hold(src_dev);
				if (ip_version == 4) {
					DEBUG_TRACE("HACK: %s tunnel packet with src_addr: " ECM_IP_ADDR_DOT_FMT " uses dev: %p(%s)\n", "IPV6", ECM_IP_ADDR_TO_DOT(src_addr), src_dev, src_dev->name);
				} else {
					DEBUG_TRACE("HACK: %s tunnel packet with src_addr: " ECM_IP_ADDR_OCTAL_FMT " uses dev: %p(%s)\n", "IPIP", ECM_IP_ADDR_TO_OCTAL(src_addr), src_dev, src_dev->name);
				}
			}
		}
	}

	if (!src_dev) {
		DEBUG_WARN("src_addr: " ECM_IP_ADDR_OCTAL_FMT " - cannot locate device\n", ECM_IP_ADDR_TO_OCTAL(src_addr));
		dev_put(dest_dev);
		return ECM_DB_IFACE_HEIRARCHY_MAX;
	}
	src_dev_name = src_dev->name;
	src_dev_type = src_dev->type;

	/*
	 * Check if source and dest dev are same.
	 * For the forwarded flows which involve tunnels this will happen when called from input hook.
	 */
	if (src_dev == dest_dev) {
		DEBUG_TRACE("Protocol is :%d source dev and dest dev are same\n", protocol);
		if (((ip_version == 4) && ((protocol == IPPROTO_IPV6) || (protocol == IPPROTO_ESP)))
				|| ((ip_version == 6) && (protocol == IPPROTO_IPIP))) {
			/*
			 * This happens from the input hook
			 * We do not want to create a connection entry for this
			 * GGG TODO YES WE DO.
			 * GGG TODO THIS CONCERNS ME AS THIS SHOULD BE CAUGHT MUCH
			 * EARLIER IN THE FRONT END IF POSSIBLE TO AVOID PERFORMANCE PENALTIES.
			 * WE HAVE DONE A TREMENDOUS AMOUT OF WORK TO GET TO THIS POINT.
			 * WE WILL ABORT HERE AND THIS WILL BE REPEATED FOR EVERY PACKET.
			 * IN KEEPING WITH THE ECM DESIGN IT IS BETTER TO CREATE A CONNECTION AND RECORD IN THE HIERARCHY
			 * ENOUGH INFORMATION TO ENSURE THAT ACCELERATION IS NOT BROKEN / DOES NOT OCCUR AT ALL.
			 * THAT WAY WE DO A HEAVYWEIGHT ESTABLISHING OF A CONNECTION ONCE AND NEVER AGAIN...
			 */
			dev_put(src_dev);
			dev_put(dest_dev);
			return ECM_DB_IFACE_HEIRARCHY_MAX;
		}
	}

	next_dest_addr_valid = true;
	next_dest_node_addr_valid = false;
	ECM_IP_ADDR_COPY(next_dest_addr, dest_addr);

	/*
	 * Iterate until we are done or get to the max number of interfaces we can record.
	 * NOTE: current_interface_index tracks the position of the first interface position in interfaces[]
	 * because we add from the end first_interface grows downwards.
	 */
	current_interface_index = ECM_DB_IFACE_HEIRARCHY_MAX;
	while (current_interface_index > 0) {
		struct ecm_db_iface_instance *ii;
		struct net_device *next_dev;

		/*
		 * Get the ecm db interface instance for the device at hand
		 */
		ii = ecm_interface_establish_and_ref(feci, dest_dev);

		/*
		 * If the interface could not be established then we abort
		 */
		if (!ii) {
			DEBUG_WARN("Failed to establish interface: %p, name: %s\n", dest_dev, dest_dev_name);
			dev_put(src_dev);
			dev_put(dest_dev);

			/*
			 * Release the interfaces heirarchy we constructed to this point.
			 */
			ecm_db_connection_interfaces_deref(interfaces, current_interface_index);
			return ECM_DB_IFACE_HEIRARCHY_MAX;
		}

		/*
		 * Record the interface instance into the interfaces[]
		 */
		current_interface_index--;
		interfaces[current_interface_index] = ii;

		/*
		 * Now we have to figure out what the next device will be (in the transmission path) the skb
		 * will use to emit to the destination address.
		 */
		do {
#ifdef ECM_INTERFACE_PPP_ENABLE
			int channel_count;
			struct ppp_channel *ppp_chan[1];
			int channel_protocol;
			struct pppoe_opt addressing;
#endif

			DEBUG_TRACE("Net device: %p is type: %d, name: %s\n", dest_dev, dest_dev_type, dest_dev_name);
			next_dev = NULL;

			if (dest_dev_type == ARPHRD_ETHER) {
				/*
				 * Ethernet - but what sub type?
				 */

#ifdef ECM_INTERFACE_VLAN_ENABLE
				/*
				 * VLAN?
				 */
				if (is_vlan_dev(dest_dev)) {
					/*
					 * VLAN master
					 * No locking needed here, ASSUMPTION is that real_dev is held for as long as we have dev.
					 */
					next_dev = vlan_dev_real_dev(dest_dev);
					dev_hold(next_dev);
					DEBUG_TRACE("Net device: %p is VLAN, slave dev: %p (%s)\n",
							dest_dev, next_dev, next_dev->name);
					break;
				}
#endif

				/*
				 * BRIDGE?
				 */
				if (ecm_front_end_is_bridge_device(dest_dev)) {
					/*
					 * Bridge
					 * Figure out which port device the skb will go to using the dest_addr.
					 */
					bool on_link;
					ip_addr_t gw_addr = ECM_IP_ADDR_NULL;
					uint8_t mac_addr[ETH_ALEN];

					if (next_dest_node_addr_valid) {
						memcpy(mac_addr, next_dest_node_addr, ETH_ALEN);
					} else if (!next_dest_addr_valid) {
						dev_put(src_dev);
						dev_put(dest_dev);

						/*
						 * Release the interfaces heirarchy we constructed to this point.
						 */
						ecm_db_connection_interfaces_deref(interfaces, current_interface_index);
						return ECM_DB_IFACE_HEIRARCHY_MAX;
					} else {
						if (!ecm_interface_mac_addr_get(next_dest_addr, mac_addr, &on_link, gw_addr)) {
							if (ip_version == 4) {
								DEBUG_WARN("Unable to obtain MAC address for " ECM_IP_ADDR_DOT_FMT "\n",
										ECM_IP_ADDR_TO_DOT(dest_addr));
								ecm_interface_send_arp_request(dest_dev, dest_addr, on_link, gw_addr);
							}
#ifdef ECM_IPV6_ENABLE
							if (ip_version == 6) {
								DEBUG_WARN("Unable to obtain MAC address for " ECM_IP_ADDR_OCTAL_FMT "\n",
										ECM_IP_ADDR_TO_OCTAL(dest_addr));
								ecm_interface_send_neighbour_solicitation(dest_dev, dest_addr);
							}
#endif
							dev_put(src_dev);
							dev_put(dest_dev);

							/*
							 * Release the interfaces heirarchy we constructed to this point.
							 */
							ecm_db_connection_interfaces_deref(interfaces, current_interface_index);
							return ECM_DB_IFACE_HEIRARCHY_MAX;
						}
					}
					next_dev = br_port_dev_get(dest_dev, mac_addr);
					if (!next_dev) {
						DEBUG_WARN("Unable to obtain output port for: %pM\n", mac_addr);
						dev_put(src_dev);
						dev_put(dest_dev);

						/*
						 * Release the interfaces heirarchy we constructed to this point.
						 */
						ecm_db_connection_interfaces_deref(interfaces, current_interface_index);
						return ECM_DB_IFACE_HEIRARCHY_MAX;
					}
					DEBUG_TRACE("Net device: %p is BRIDGE, next_dev: %p (%s)\n", dest_dev, next_dev, next_dev->name);
					break;
				}

#ifdef ECM_INTERFACE_BOND_ENABLE
				/*
				 * LAG?
				 */
				if (ecm_front_end_is_lag_master(dest_dev)) {
					/*
					 * Link aggregation
					 * Figure out whiich slave device of the link aggregation will be used to reach the destination.
					 */
					bool dest_on_link = false;
					ip_addr_t dest_gw_addr = ECM_IP_ADDR_NULL;
					uint32_t src_addr_32 = 0;
					uint32_t dest_addr_32 = 0;
					uint8_t src_mac_addr[ETH_ALEN];
					uint8_t dest_mac_addr[ETH_ALEN];
					struct net_device *master_dev = NULL;

					memset(src_mac_addr, 0, ETH_ALEN);
					memset(dest_mac_addr, 0, ETH_ALEN);

					if (ip_version == 4) {
						ECM_IP_ADDR_TO_NIN4_ADDR(src_addr_32, src_addr);
						ECM_IP_ADDR_TO_NIN4_ADDR(dest_addr_32, dest_addr);
					}

					if (!is_routed) {
						memcpy(src_mac_addr, src_node_addr, ETH_ALEN);
						memcpy(dest_mac_addr, dest_node_addr, ETH_ALEN);
					} else {
						struct net_device *dest_dev_master;

						/*
						 * Use appropriate source MAC address for routed packets
						 */
						dest_dev_master = ecm_interface_get_and_hold_dev_master(dest_dev);
						if (dest_dev_master) {
							memcpy(src_mac_addr, dest_dev_master->dev_addr, ETH_ALEN);
						} else {
							memcpy(src_mac_addr, dest_dev->dev_addr, ETH_ALEN);
						}

						/*
						 * Determine destination MAC address for this routed packet
						 */
						if (next_dest_node_addr_valid) {
							memcpy(dest_mac_addr, next_dest_node_addr, ETH_ALEN);
						} else if (!next_dest_addr_valid) {
							dev_put(src_dev);
							dev_put(dest_dev);
							if (dest_dev_master) {
								dev_put(dest_dev_master);
							}

							ecm_db_connection_interfaces_deref(interfaces, current_interface_index);
							return ECM_DB_IFACE_HEIRARCHY_MAX;
						} else {
							if (!ecm_interface_mac_addr_get(dest_addr, dest_mac_addr,
										&dest_on_link, dest_gw_addr)) {

								/*
								 * Find proper interfce from which to issue ARP
								 * or neighbour solicitation packet.
								 */
								if (dest_dev_master) {
									master_dev = dest_dev_master;
								} else {
									master_dev = dest_dev;
								}

								dev_hold(master_dev);

								if (dest_dev_master) {
									dev_put(dest_dev_master);
								}

								if (ip_version == 4) {
									DEBUG_WARN("Unable to obtain MAC address for " ECM_IP_ADDR_DOT_FMT "\n", ECM_IP_ADDR_TO_DOT(dest_addr));
									ecm_interface_send_arp_request(dest_dev, dest_addr, dest_on_link, dest_gw_addr);
								}
#ifdef ECM_IPV6_ENABLE
								if (ip_version == 6) {
									DEBUG_WARN("Unable to obtain MAC address for " ECM_IP_ADDR_OCTAL_FMT "\n",ECM_IP_ADDR_TO_OCTAL(dest_addr));
									ecm_interface_send_neighbour_solicitation(master_dev, dest_addr);
								}
#endif
								dev_put(src_dev);
								dev_put(dest_dev);
								dev_put(master_dev);
								ecm_db_connection_interfaces_deref(interfaces, current_interface_index);
								return ECM_DB_IFACE_HEIRARCHY_MAX;
							}
						}

						if (dest_dev_master) {
							dev_put(dest_dev_master);
						}
					}

					if (ip_version == 4) {
						next_dev = bond_get_tx_dev(NULL, src_mac_addr, dest_mac_addr,
										&src_addr_32, &dest_addr_32,
										htons((uint16_t)ETH_P_IP), dest_dev, layer4hdr);
					} else if (ip_version == 6) {
						next_dev = bond_get_tx_dev(NULL, src_mac_addr, dest_mac_addr,
										src_addr, dest_addr,
										htons((uint16_t)ETH_P_IPV6), dest_dev, NULL);
					}

					if (next_dev && netif_carrier_ok(next_dev)) {
						dev_hold(next_dev);
					} else {
						DEBUG_WARN("Unable to obtain LAG output slave device\n");
						dev_put(src_dev);
						dev_put(dest_dev);

						/*
						 * Release the interfaces heirarchy we constructed to this point.
						 */
						ecm_db_connection_interfaces_deref(interfaces, current_interface_index);
						return ECM_DB_IFACE_HEIRARCHY_MAX;
					}

					DEBUG_TRACE("Net device: %p is LAG, slave dev: %p (%s)\n", dest_dev, next_dev, next_dev->name);
					break;
				}
#endif

				/*
				 * ETHERNET!
				 * Just plain ethernet it seems.
				 */
				DEBUG_TRACE("Net device: %p is ETHERNET\n", dest_dev);
				break;
			}

			/*
			 * LOOPBACK?
			 */
			if (dest_dev_type == ARPHRD_LOOPBACK) {
				DEBUG_TRACE("Net device: %p is LOOPBACK type: %d\n", dest_dev, dest_dev_type);
				break;
			}

			/*
			 * IPSEC?
			 */
			if (dest_dev_type == ECM_ARPHRD_IPSEC_TUNNEL_TYPE) {
				DEBUG_TRACE("Net device: %p is IPSec tunnel type: %d\n", dest_dev, dest_dev_type);
				// GGG TODO Figure out the next device the tunnel is using...
				break;
			}

			/*
			 * SIT (6-in-4)?
			 */
			if (dest_dev_type == ARPHRD_SIT) {
				DEBUG_TRACE("Net device: %p is SIT (6-in-4) type: %d\n", dest_dev, dest_dev_type);
				// GGG TODO Figure out the next device the tunnel is using...
				break;
			}

			/*
			 * IPIP6 Tunnel?
			 */
			if (dest_dev_type == ARPHRD_TUNNEL6) {
				DEBUG_TRACE("Net device: %p is TUNIPIP6 type: %d\n", dest_dev, dest_dev_type);
				// GGG TODO Figure out the next device the tunnel is using...
				break;
			}

			/*
			 * If this is NOT PPP then it is unknown to the ecm and we cannot figure out it's next device.
			 */
			if (dest_dev_type != ARPHRD_PPP) {
				DEBUG_TRACE("Net device: %p is UNKNOWN type: %d\n", dest_dev, dest_dev_type);
				break;
			}

#ifndef ECM_INTERFACE_PPP_ENABLE
			DEBUG_TRACE("Net device: %p is UNKNOWN (PPP Unsupported) type: %d\n", dest_dev, dest_dev_type);
#else
			/*
			 * PPP - but what is the channel type?
			 * First: If this is multi-link then we do not support it
			 */
			if (ppp_is_multilink(dest_dev) > 0) {
				DEBUG_TRACE("Net device: %p is MULTILINK PPP - Unknown to the ECM\n", dest_dev);
				break;
			}

			DEBUG_TRACE("Net device: %p is PPP\n", dest_dev);

			/*
			 * Get the PPP channel and then enquire what kind of channel it is
			 * NOTE: Not multilink so only one channel to get.
			 */
			channel_count = ppp_hold_channels(dest_dev, ppp_chan, 1);
			if (channel_count != 1) {
				DEBUG_TRACE("Net device: %p PPP has %d channels - Unknown to the ECM\n",
						dest_dev, channel_count);
				break;
			}

			/*
			 * Get channel protocol type
			 * NOTE: Not all PPP channels support channel specific methods.
			 */
			channel_protocol = ppp_channel_get_protocol(ppp_chan[0]);
			if (channel_protocol != PX_PROTO_OE) {
				DEBUG_TRACE("Net device: %p PPP channel protocol: %d - Unknown to the ECM\n",
						dest_dev, channel_protocol);

				/*
				 * Release the channel
				 */
				ppp_release_channels(ppp_chan, 1);

				break;
			}

			/*
			 * PPPoE channel
			 */
			DEBUG_TRACE("Net device: %p PPP channel is PPPoE\n", dest_dev);

			/*
			 * Get PPPoE session information and the underlying device it is using.
			 */
			pppoe_channel_addressing_get(ppp_chan[0], &addressing);

			/*
			 * Copy the dev hold into this, we will release the hold later
			 */
			next_dev = addressing.dev;
			next_dest_addr_valid = false;
			next_dest_node_addr_valid = true;
			memcpy(next_dest_node_addr, addressing.pa.remote, ETH_ALEN);

			DEBUG_TRACE("Net device: %p, next device: %p (%s)\n", dest_dev, next_dev, next_dev->name);

			/*
			 * Release the channel.  Note that next_dev is still (correctly) held.
			 */
			ppp_release_channels(ppp_chan, 1);
#endif
		} while (false);

		/*
		 * No longer need dest_dev as it may become next_dev
		 */
		dev_put(dest_dev);

		/*
		 * Check out the next_dev, if any
		 */
		if (!next_dev) {
			int32_t i __attribute__((unused));
			DEBUG_INFO("Completed interface heirarchy construct with first interface @: %d\n", current_interface_index);
#if DEBUG_LEVEL > 1
			for (i = current_interface_index; i < ECM_DB_IFACE_HEIRARCHY_MAX; ++i) {
				DEBUG_TRACE("\tInterface @ %d: %p, type: %d, name: %s\n",
						i, interfaces[i], ecm_db_connection_iface_type_get(interfaces[i]), ecm_db_interface_type_to_string(ecm_db_connection_iface_type_get(interfaces[i])));

			}
#endif

			/*
			 * Release src_dev now
			 */
			dev_put(src_dev);
			return current_interface_index;
		}

		/*
		 * dest_dev becomes next_dev
		 */
		dest_dev = next_dev;
		dest_dev_name = dest_dev->name;
		dest_dev_type = dest_dev->type;
	}

	DEBUG_WARN("Too many interfaces: %d\n", current_interface_index);
	DEBUG_ASSERT(current_interface_index == 0, "Bad logic handling current_interface_index: %d\n", current_interface_index);
	dev_put(src_dev);
	dev_put(dest_dev);

	/*
	 * Release the interfaces heirarchy we constructed to this point.
	 */
	ecm_db_connection_interfaces_deref(interfaces, current_interface_index);
	return ECM_DB_IFACE_HEIRARCHY_MAX;
}
EXPORT_SYMBOL(ecm_interface_heirarchy_construct);

/*
 * ecm_interface_list_stats_update()
 *	Given an interface list, walk the interfaces and update the stats for certain types.
 */
static void ecm_interface_list_stats_update(int iface_list_first, struct ecm_db_iface_instance *iface_list[], uint8_t *mac_addr,
					bool is_mcast_flow, uint32_t tx_packets, uint32_t tx_bytes, uint32_t rx_packets, uint32_t rx_bytes)
{
	int list_index;

	for (list_index = iface_list_first; (list_index < ECM_DB_IFACE_HEIRARCHY_MAX); list_index++) {
		struct ecm_db_iface_instance *ii;
		ecm_db_iface_type_t ii_type;
		char *ii_name;
		struct net_device *dev;

		ii = iface_list[list_index];
		ii_type = ecm_db_connection_iface_type_get(ii);
		ii_name = ecm_db_interface_type_to_string(ii_type);
		DEBUG_TRACE("list_index: %d, ii: %p, type: %d (%s)\n", list_index, ii, ii_type, ii_name);

		/*
		 * Locate real device in system
		 */
		dev = dev_get_by_index(&init_net, ecm_db_iface_interface_identifier_get(ii));
		if (!dev) {
			DEBUG_WARN("Could not locate interface\n");
			continue;
		}
		DEBUG_TRACE("found dev: %p (%s)\n", dev, dev->name);

		if (likely(!is_mcast_flow)) {
			/*
			 * Refresh the bridge forward table entry if the port is a bridge port
			 * Note: A bridge port can be of different interface type, e.g VLAN, ethernet.
			 * This check, therefore, should be performed for all interface types.
			 */
			if (is_valid_ether_addr(mac_addr) && ecm_front_end_is_bridge_port(dev) && rx_packets) {
				DEBUG_TRACE("Update bridge fdb entry for mac: %pM\n", mac_addr);
				br_refresh_fdb_entry(dev, mac_addr);
			}
		}

		switch (ii_type) {
			struct rtnl_link_stats64 stats;

#ifdef ECM_INTERFACE_VLAN_ENABLE
			case ECM_DB_IFACE_TYPE_VLAN:
				DEBUG_INFO("VLAN\n");
				stats.rx_packets = rx_packets;
				stats.rx_bytes = rx_bytes;
				stats.tx_packets = tx_packets;
				stats.tx_bytes = tx_bytes;
				__vlan_dev_update_accel_stats(dev, &stats);
				break;
#endif
			case ECM_DB_IFACE_TYPE_BRIDGE:
				DEBUG_INFO("BRIDGE\n");
				stats.rx_packets = rx_packets;
				stats.rx_bytes = rx_bytes;
				stats.tx_packets = tx_packets;
				stats.tx_bytes = tx_bytes;
				br_dev_update_stats(dev, &stats);
				break;

#ifdef ECM_INTERFACE_PPP_ENABLE
			case ECM_DB_IFACE_TYPE_PPPOE:
				DEBUG_INFO("PPPOE\n");
				ppp_update_stats(dev, rx_packets, rx_bytes, tx_packets, tx_bytes);
				break;
#endif
			default:
				/*
				 * TODO: Extend it accordingly
				 */
				break;
		}

		dev_put(dev);
	}
}

/*
 * ecm_interface_stats_update()
 *	Using the interface lists for the given connection, update the interface statistics for each.
 *
 * 'from' here is wrt the connection 'from' side.  Likewise with 'to'.
 * TX is wrt what the interface has transmitted.  RX is what the interface has received.
 */
void ecm_interface_stats_update(struct ecm_db_connection_instance *ci,
						uint32_t from_tx_packets, uint32_t from_tx_bytes, uint32_t from_rx_packets, uint32_t from_rx_bytes,
						uint32_t to_tx_packets, uint32_t to_tx_bytes, uint32_t to_rx_packets, uint32_t to_rx_bytes)
{
	struct ecm_db_iface_instance *from_ifaces[ECM_DB_IFACE_HEIRARCHY_MAX];
	struct ecm_db_iface_instance *to_ifaces[ECM_DB_IFACE_HEIRARCHY_MAX];
	int from_ifaces_first;
	int to_ifaces_first;
	uint8_t mac_addr[ETH_ALEN];

	/*
	 * Iterate the 'from' side interfaces and update statistics and state for the real HLOS interfaces
	 * from_tx_packets / bytes: the amount transmitted by the 'from' interface
	 * from_rx_packets / bytes: the amount received by the 'from' interface
	 */
	DEBUG_INFO("%p: Update from interface stats\n", ci);
	from_ifaces_first = ecm_db_connection_from_interfaces_get_and_ref(ci, from_ifaces);
	ecm_db_connection_from_node_address_get(ci, mac_addr);
	ecm_interface_list_stats_update(from_ifaces_first, from_ifaces, mac_addr, false, from_tx_packets, from_tx_bytes, from_rx_packets, from_rx_bytes);
	ecm_db_connection_interfaces_deref(from_ifaces, from_ifaces_first);

	/*
	 * Iterate the 'to' side interfaces and update statistics and state for the real HLOS interfaces
	 * to_tx_packets / bytes: the amount transmitted by the 'to' interface
	 * to_rx_packets / bytes: the amount received by the 'to' interface
	 */
	DEBUG_INFO("%p: Update to interface stats\n", ci);
	to_ifaces_first = ecm_db_connection_to_interfaces_get_and_ref(ci, to_ifaces);
	ecm_db_connection_to_node_address_get(ci, mac_addr);
	ecm_interface_list_stats_update(to_ifaces_first, to_ifaces, mac_addr, false, to_tx_packets, to_tx_bytes, to_rx_packets, to_rx_bytes);
	ecm_db_connection_interfaces_deref(to_ifaces, to_ifaces_first);
}
EXPORT_SYMBOL(ecm_interface_stats_update);

#ifdef ECM_MULTICAST_ENABLE
/*
 * ecm_interface_multicast_stats_update()
 *	Using the interface lists for the given connection, update the interface statistics for each.
 *
 * 'from interface' here is the connection 'from' side.  Likewise with 'to interface'.
 * TX is wrt what the interface has transmitted.  RX is what the interface has received.
 */
void ecm_interface_multicast_stats_update(struct ecm_db_connection_instance *ci, uint32_t from_tx_packets, uint32_t from_tx_bytes,
				   uint32_t from_rx_packets, uint32_t from_rx_bytes, uint32_t to_tx_packets, uint32_t to_tx_bytes,
				   uint32_t to_rx_packets, uint32_t to_rx_bytes)
{
	struct ecm_db_iface_instance *from_ifaces[ECM_DB_IFACE_HEIRARCHY_MAX];
	struct ecm_db_iface_instance *to_list_single[ECM_DB_IFACE_HEIRARCHY_MAX];
	struct ecm_db_iface_instance *to_ifaces;
	struct ecm_db_iface_instance *ii_temp;
	int from_ifaces_first;
	int *to_ifaces_first;
	int if_index;
	int ret;
	uint8_t mac_addr[ETH_ALEN];

	/*
	 * Iterate the 'from' side interfaces and update statistics and state for the real HLOS interfaces
	 * from_tx_packets / bytes: the amount transmitted by the 'from' interface
	 * from_rx_packets / bytes: the amount received by the 'from' interface
	 */
	DEBUG_INFO("%p: Update from interface stats\n", ci);
	from_ifaces_first = ecm_db_connection_from_interfaces_get_and_ref(ci, from_ifaces);
	ecm_db_connection_from_node_address_get(ci, mac_addr);
	ecm_interface_list_stats_update(from_ifaces_first, from_ifaces, mac_addr, false, from_tx_packets, from_tx_bytes, from_rx_packets, from_rx_bytes);
	ecm_db_connection_interfaces_deref(from_ifaces, from_ifaces_first);

	/*
	 * Iterate the 'to' side interfaces and update statistics and state for the real HLOS interfaces
	 * to_tx_packets / bytes: the amount transmitted by the 'to' interface
	 * to_rx_packets / bytes: the amount received by the 'to' interface
	 */
	DEBUG_INFO("%p: Update to interface stats\n", ci);

	/*
	 * This function allocates the memory for temporary destination interface heirarchies.
	 * This memory needs to be free at the end.
	 */
	ret = ecm_db_multicast_connection_to_interfaces_get_and_ref_all(ci, &to_ifaces, &to_ifaces_first);
	if (ret == 0) {
		DEBUG_WARN("%p: Get and ref to all multicast detination interface heirarchies failed\n", ci);
		return;
	}

	for (if_index = 0; if_index < ECM_DB_MULTICAST_IF_MAX; if_index++) {
		if (to_ifaces_first[if_index] < ECM_DB_IFACE_HEIRARCHY_MAX) {
			ii_temp = ecm_db_multicast_if_heirarchy_get(to_ifaces, if_index);
			ecm_db_multicast_copy_if_heirarchy(to_list_single, ii_temp);
			ecm_interface_list_stats_update(to_ifaces_first[if_index], to_list_single, mac_addr, true, to_tx_packets, to_tx_bytes, to_rx_packets, to_rx_bytes);
		}
	}

	ecm_db_multicast_connection_to_interfaces_deref_all(to_ifaces, to_ifaces_first);
}
EXPORT_SYMBOL(ecm_interface_multicast_stats_update);
#endif

/*
 * ecm_interface_regenerate_connection()
 *	Re-generate a specific connection
 */
void ecm_interface_regenerate_connection(struct ecm_db_connection_instance *ci)
{
	struct ecm_front_end_connection_instance *feci;

	DEBUG_TRACE("Regenerate connection: %p\n", ci);

	/*
	 * Flag the connection as needing re-generation.
	 * Re-generation occurs when we next see traffic OR an acceleration engine sync for this connection.
	 * Refer to front end protocol specific process() functions.
	 */
	ecm_db_connection_regeneration_needed(ci);

	/*
	 * If the connection is accelerated then force deceleration.
	 * Under normal circumstances deceleration would occur on the next sync received,
	 * however, there is a situation where a sync may not occur if, say, a cable has been pulled.
	 * The acceleration engine would see no further traffic to trigger sending a sync and so
	 * re-generation would not occur.
	 * The connection would stall and no-regeneration would happen leaving the connection in bad state.
	 * NOTE: We can just call decelerate() upon the front end - if its not accelerated this will have no effect.
	 */
	feci = ecm_db_connection_front_end_get_and_ref(ci);
	feci->decelerate(feci);
	feci->deref(feci);
}

/*
 * ecm_interface_regenerate_connections()
 *	Cause regeneration of all connections that are using the specified interface.
 */
static void ecm_interface_regenerate_connections(struct ecm_db_iface_instance *ii)
{
#ifdef ECM_DB_XREF_ENABLE
	struct ecm_db_connection_instance *ci_from;
	struct ecm_db_connection_instance *ci_to;
	struct ecm_db_connection_instance *ci_from_nat;
	struct ecm_db_connection_instance *ci_to_nat;
	struct ecm_db_connection_instance *ci_mcast __attribute__ ((unused));
#endif

	DEBUG_TRACE("Regenerate connections using interface: %p\n", ii);

#ifndef ECM_DB_XREF_ENABLE
	/*
	 * An interface has changed, re-generate the connections to ensure all state is updated.
	 */
	ecm_db_regeneration_needed();
#else
	/*
	 * If the interface has NO connections then we re-generate all.
	 */
	ci_from = ecm_db_iface_connections_from_get_and_ref_first(ii);
	ci_to = ecm_db_iface_connections_to_get_and_ref_first(ii);
	ci_from_nat = ecm_db_iface_connections_nat_from_get_and_ref_first(ii);
	ci_to_nat = ecm_db_iface_connections_nat_to_get_and_ref_first(ii);
	if (!ci_from && !ci_to && !ci_from_nat && !ci_to_nat) {
		ecm_db_regeneration_needed();
		DEBUG_TRACE("%p: Regenerate (ALL) COMPLETE\n", ii);
		return;
	}

	/*
	 * Re-generate all connections associated with this interface
	 */
	DEBUG_TRACE("%p: Regenerate 'from' connections\n", ii);
	while (ci_from) {
		struct ecm_db_connection_instance *cin;
		cin = ecm_db_connection_iface_from_get_and_ref_next(ci_from);

		DEBUG_TRACE("%p: Regenerate: %p", ii, ci_from);
		ecm_db_connection_regeneration_needed(ci_from);
		ecm_db_connection_deref(ci_from);
		ci_from = cin;
	}

	DEBUG_TRACE("%p: Regenerate 'to' connections\n", ii);
	while (ci_to) {
		struct ecm_db_connection_instance *cin;
		cin = ecm_db_connection_iface_to_get_and_ref_next(ci_to);

		DEBUG_TRACE("%p: Regenerate: %p", ii, ci_to);
		ecm_db_connection_regeneration_needed(ci_to);
		ecm_db_connection_deref(ci_to);
		ci_to = cin;
	}

	/*
	 * GGG TODO These deprecated lists _nat_ lists will eventually be removed
	 */
	DEBUG_TRACE("%p: Regenerate 'from_nat' connections\n", ii);
	while (ci_from_nat) {
		struct ecm_db_connection_instance *cin;
		cin = ecm_db_connection_iface_nat_from_get_and_ref_next(ci_from_nat);

		DEBUG_TRACE("%p: Regenerate: %p", ii, ci_from_nat);
		ecm_db_connection_regeneration_needed(ci_from_nat);
		ecm_db_connection_deref(ci_from_nat);
		ci_from_nat = cin;
	}

	DEBUG_TRACE("%p: Regenerate 'to_nat' connections\n", ii);
	while (ci_to_nat) {
		struct ecm_db_connection_instance *cin;
		cin = ecm_db_connection_iface_nat_to_get_and_ref_next(ci_to_nat);

		DEBUG_TRACE("%p: Regenerate: %p", ii, ci_to_nat);
		ecm_db_connection_regeneration_needed(ci_to_nat);
		ecm_db_connection_deref(ci_to_nat);
		ci_to_nat = cin;
	}

#ifdef ECM_MULTICAST_ENABLE
	/*
	 * Multicasts would not have recorded in the lists above.
	 * Our only way to re-gen those is to iterate all multicasts.
	 * GGG TODO This will be optimised in a future release.
	 */
	ci_mcast = ecm_db_connections_get_and_ref_first();
	while (ci_mcast) {
		struct ecm_db_connection_instance *cin;

		/*
		 * Multicast and NOT flagged for re-gen?
		 */
		if (ecm_db_multicast_connection_to_interfaces_set_check(ci_mcast)
				&& ecm_db_connection_regeneration_required_peek(ci_mcast)) {
			ecm_db_connection_regeneration_needed(ci_mcast);
		}

		cin = ecm_db_connection_get_and_ref_next(ci_mcast);
		ecm_db_connection_deref(ci_mcast);
		ci_mcast = cin;
	}
#endif

#endif
	DEBUG_TRACE("%p: Regenerate COMPLETE\n", ii);
}

/*
 * ecm_interface_dev_regenerate_connections()
 *	Cause regeneration of all connections that are using the specified interface.
 */
void ecm_interface_dev_regenerate_connections(struct net_device *dev)
{
	struct ecm_db_iface_instance *ii;

	DEBUG_INFO("Regenerate connections for: %p (%s)\n", dev, dev->name);

	/*
	 * If the interface is known to us then we will get it returned by this
	 * function and process it accordingly.
	 */
	ii = ecm_db_iface_find_and_ref_by_interface_identifier(dev->ifindex);
	if (!ii) {
		DEBUG_WARN("%p: No interface instance could be established for this dev\n", dev);
		return;
	}
	ecm_interface_regenerate_connections(ii);
	DEBUG_TRACE("%p: Regenerate for %p: COMPLETE\n", dev, ii);
	ecm_db_iface_deref(ii);
}

/*
 * ecm_interface_mtu_change()
 *	MTU of interface has changed
 */
static void ecm_interface_mtu_change(struct net_device *dev)
{
	int mtu;
	struct ecm_db_iface_instance *ii;

	mtu = dev->mtu;
	DEBUG_INFO("%p (%s): MTU Change to: %d\n", dev, dev->name, mtu);

	/*
	 * Find the interface for the given device.
	 */
	ii = ecm_db_iface_find_and_ref_by_interface_identifier(dev->ifindex);
	if (!ii) {
		DEBUG_WARN("%p: No interface instance could be established for this dev\n", dev);
		return;
	}

	/*
	 * Change the mtu
	 */
	ecm_db_iface_mtu_reset(ii, mtu);
	DEBUG_TRACE("%p (%s): MTU Changed to: %d\n", dev, dev->name, mtu);
	if (netif_is_bond_slave(dev)) {
		struct net_device *master = NULL;
		master = ecm_interface_get_and_hold_dev_master(dev);
		DEBUG_ASSERT(master, "Expected a master\n");
		ecm_interface_dev_regenerate_connections(master);
		dev_put(master);
	} else {
		ecm_interface_regenerate_connections(ii);
	}

	DEBUG_TRACE("%p: Regenerate for %p: COMPLETE\n", dev, ii);
	ecm_db_iface_deref(ii);
}

/*
 * ecm_interface_netdev_notifier_callback()
 * 	Netdevice notifier callback to inform us of change of state of a netdevice
 */
static int ecm_interface_netdev_notifier_callback(struct notifier_block *this, unsigned long event, void *ptr)
{
#if (LINUX_VERSION_CODE <= KERNEL_VERSION(3, 10, 0))
	struct net_device *dev __attribute__ ((unused)) = (struct net_device *)ptr;
#else
	struct net_device *dev = netdev_notifier_info_to_dev(ptr);
#endif
	struct net_device *master = NULL;

	DEBUG_INFO("Net device notifier for: %p, name: %s, event: %lx\n", dev, dev->name, event);

	switch (event) {
	case NETDEV_DOWN:
		DEBUG_INFO("Net device: %p, DOWN\n", dev);
		if (netif_is_bond_slave(dev)) {
			master = ecm_interface_get_and_hold_dev_master(dev);
			DEBUG_ASSERT(master, "Expected a master\n");
			ecm_interface_dev_regenerate_connections(master);
			dev_put(master);
		} else {
			ecm_interface_dev_regenerate_connections(dev);
		}
		break;

	case NETDEV_CHANGE:
		DEBUG_INFO("Net device: %p, CHANGE\n", dev);
		if (!netif_carrier_ok(dev)) {
			DEBUG_INFO("Net device: %p, CARRIER BAD\n", dev);
			if (netif_is_bond_slave(dev)) {
				master = ecm_interface_get_and_hold_dev_master(dev);
				DEBUG_ASSERT(master, "Expected a master\n");
				ecm_interface_dev_regenerate_connections(master);
				dev_put(master);
			} else {
				ecm_interface_dev_regenerate_connections(dev);
			}
		}
		break;

	case NETDEV_CHANGEMTU:
		DEBUG_INFO("Net device: %p, MTU CHANGE\n", dev);
		ecm_interface_mtu_change(dev);
		break;

	default:
		DEBUG_TRACE("Net device: %p, UNHANDLED: %lx\n", dev, event);
		break;
	}

	return NOTIFY_DONE;
}

/*
 * struct notifier_block ecm_interface_netdev_notifier
 *	Registration for net device changes of state.
 */
static struct notifier_block ecm_interface_netdev_notifier __read_mostly = {
	.notifier_call		= ecm_interface_netdev_notifier_callback,
};
#if defined(ECM_DB_XREF_ENABLE) && defined(ECM_BAND_STEERING_ENABLE)
/*
 * ecm_interfae_node_br_fdb_notify_event()
 *	This is a call back for "bridge fdb update event/ageing timer expire
 *	event".
 */
static int ecm_interface_node_br_fdb_notify_event(struct notifier_block *nb,
					       unsigned long val,
					       void *data)
{
	uint8_t *mac =  (uint8_t *)data;
	struct ecm_db_node_instance *node = NULL;

	if(unlikely(!mac)) {
		DEBUG_WARN("mac address passed to ecm_interface_node_br_fdb_notify_event is null \n");
		return NOTIFY_DONE;
	}

	/*
	 * find node instance corresponding to mac address
	 */
	node = ecm_db_node_find_and_ref(mac);

	if(unlikely(!node)) {
		DEBUG_WARN("node address is null \n");
		return NOTIFY_DONE;
	}

	ecm_db_traverse_node_from_connection_list_and_decelerate(node);
	ecm_db_traverse_node_to_connection_list_and_decelerate(node);
	ecm_db_traverse_node_from_nat_connection_list_and_decelerate(node);
	ecm_db_traverse_node_to_nat_connection_list_and_decelerate(node);

	ecm_db_node_deref(node);

	return NOTIFY_DONE;
}

static struct notifier_block ecm_interface_node_br_fdb_update_nb = {
	.notifier_call = ecm_interface_node_br_fdb_notify_event,
};
#endif

#ifdef ECM_MULTICAST_ENABLE
/*
 * ecm_interface_multicast_find_outdated_iface_instances()
 *
 *	Called in the case of Routing/Bridging Multicast update events.
 *
 *	This function takes a list of ifindex for the connection which was received
 *	from MFC or bridge snooper, compares it against the existing list of interfaces
 *	in the DB connection, and extracts the list of those interfaces that have left
 *	the multicast group.
 *
 * 	ci		A DB connection instance.
 * 	mc_updates	Part of return Information. The function will mark the index of those
 * 			interfaces in the DB connection 'to_mcast_interfaces' array that have
 * 			left the group, in the mc_updates->if_leave_idx array. The caller uses this
 * 			information to delete those outdated interface heirarchies from the
 * 			connection.
 * 	is_bridged	True if the function called due to bridge multicast snooper update event.
 * 	dst_dev		Holds the netdevice ifindex number of the new list of interfaces as reported
 * 			by the update from MFC or Bridge snooper.
 *	max_to_dev	Size of the array 'dst_dev'
 *
 *	Return true if outdated interfaces found
 */
static bool ecm_interface_multicast_find_outdated_iface_instances(struct ecm_db_connection_instance *ci, struct ecm_multicast_if_update *mc_updates,
						   uint32_t flags, bool is_br_snooper, uint32_t *mc_dst_if_index, uint32_t max_to_dev)
{
	struct ecm_db_iface_instance *mc_ifaces;
	struct ecm_db_iface_instance *ii_temp;
	struct ecm_db_iface_instance *ii_single;
	struct ecm_db_iface_instance **ifaces;
	struct ecm_db_iface_instance *to_iface;
	int32_t *to_iface_first;
	int32_t *mc_ifaces_first;
	uint32_t *dst_if_index;
	ecm_db_iface_type_t ii_type;
	int32_t heirarchy_index;
	int32_t if_index;
	int32_t if_cnt = 0;
	int found = 0;
	int ii;
	int ret;
	int32_t ifaces_identifier;

	ret = ecm_db_multicast_connection_to_interfaces_get_and_ref_all(ci, &mc_ifaces, &mc_ifaces_first);
	if (ret == 0) {
		DEBUG_WARN("%p: multicast interfaces ref fail!\n", ci);
		return false;
	}

	/*
	 * Loop through the current interface list in the DB
	 * connection 'to_mcast_interfaces' array
	 */
	for (heirarchy_index = 0; heirarchy_index < ECM_DB_MULTICAST_IF_MAX; heirarchy_index++) {
		found = 0;
		to_iface_first = ecm_db_multicast_if_first_get_at_index(mc_ifaces_first, heirarchy_index);

		/*
		 * Invalid interface entry, skip
		 */
		if (*to_iface_first == ECM_DB_IFACE_HEIRARCHY_MAX) {
			continue;
		}

		ii_temp = ecm_db_multicast_if_heirarchy_get(mc_ifaces, heirarchy_index);
		ii_single = ecm_db_multicast_if_instance_get_at_index(ii_temp, ECM_DB_IFACE_HEIRARCHY_MAX - 1);
		ifaces = (struct ecm_db_iface_instance **)ii_single;
		to_iface = *ifaces;
		ii_type = ecm_db_connection_iface_type_get(to_iface);

		/*
		 * If the update was received from bridge snooper, do not consider entries in the
		 * interface list that are not part of a bridge.
		 */
		if (is_br_snooper && (ii_type != ECM_DB_IFACE_TYPE_BRIDGE)) {
			continue;
		}

		/*
		 * If the update was received from MFC, do not consider entries in the
		 * interface list that are part of a bridge. The bridge entries will be
		 * taken care by the Bridge Snooper Callback
		 */
		if (ii_type == ECM_DB_IFACE_TYPE_BRIDGE) {
			if (!is_br_snooper && !(flags & ECM_DB_MULTICAST_CONNECTION_BRIDGE_DEV_SET_FLAG)) {
				continue;
			}
		}

		/*
		 * Try to find a match in the newly received interface list, for any of
		 * the interface instance in the heirarchy. If found, it means that this
		 * interface has not left the group. If not found, it means that this
		 * interface has left the group.
		 */
		for (ii = ECM_DB_IFACE_HEIRARCHY_MAX - 1; ii >= *to_iface_first; ii--) {
			ii_single = ecm_db_multicast_if_instance_get_at_index(ii_temp, ii);
			ifaces = (struct ecm_db_iface_instance **)ii_single;
			to_iface = *ifaces;

			ii_type = ecm_db_connection_iface_type_get(to_iface);
			ifaces_identifier = ecm_db_iface_interface_identifier_get(to_iface);
			for (if_index = 0; if_index < max_to_dev; if_index++) {
				dst_if_index = ecm_db_multicast_if_num_get_at_index(mc_dst_if_index, if_index);
				if (*dst_if_index == ifaces_identifier) {
					found = 1;
					break;
				}
			}
			if (found) {
				break;
			}
		}

		/*
		 * We did not find a match for the interface in the present list. So mark
		 * if as one that has left the group.
		 */
		if (!found) {
			if_cnt++;
			mc_updates->if_leave_idx[heirarchy_index] = 1;
		}
	}

	ecm_db_multicast_connection_to_interfaces_deref_all(mc_ifaces, mc_ifaces_first);
	mc_updates->if_leave_cnt = if_cnt;
	return (if_cnt > 0);
}

/*
 * ecm_interface_multicast_find_new_iface_instances()
 *
 *	Called in the case of Routing/Bridging Multicast update events.
 *
 *	This function takes a list of ifindex for the connection which was received
 *	from MFC or bridge snooper, compares it against the existing list of interfaces
 *	in the DB connection, and extracts the list of the new joinees for the multicast
 *	group.
 *
 * 	ci		A DB connection instance.
 * 	mc_updates	Part of return Information. The function will mark the index of those
 * 			interfaces in the 'dst_dev' array that have joined the group, in the
 * 			mc_updates->if_join_idx array. The caller uses this information to add the new
 * 			interface heirarchies into the connection.
 * 	dst_dev		Holds the netdevice ifindex number of the new list of interfaces as reported
 * 			by the update from MFC or Bridge snooper.
 *	max_to_dev	Size of the array 'dst_dev'
 *
 *	Return true if new joinees found
 */
static bool ecm_interface_multicast_find_new_iface_instances(struct ecm_db_connection_instance *ci,
					struct ecm_multicast_if_update *mc_updates, uint32_t *mc_dst_if_index, uint32_t max_to_dev)
{
	struct ecm_db_iface_instance *mc_ifaces;
	struct ecm_db_iface_instance *ii_temp;
	struct ecm_db_iface_instance *ii_single;
	struct ecm_db_iface_instance **ifaces;
	int32_t *mc_ifaces_first;
	int32_t *to_list_first;
	int32_t heirarchy_index;
	int32_t if_index;
	int32_t if_cnt = 0;
	int found = 0;
	int ii;
	int ret;
	uint32_t *dst_if_index;
	int32_t ifaces_identifier;
	struct ecm_db_iface_instance *to_list;

	ret = ecm_db_multicast_connection_to_interfaces_get_and_ref_all(ci, &mc_ifaces, &mc_ifaces_first);
	if (ret == 0) {
		DEBUG_WARN("%p: multicast interfaces ref fail!\n", ci);
		return false;
	}

	/*
	 * Loop through the new interface list 'dst_dev'
	 */
	for (if_index = 0; if_index < max_to_dev; if_index++) {
		found = 0;
		dst_if_index = ecm_db_multicast_if_num_get_at_index(mc_dst_if_index, if_index);
		if (*dst_if_index == 0) {
			continue;
		}

		for (heirarchy_index = 0; heirarchy_index < ECM_DB_MULTICAST_IF_MAX ; heirarchy_index++) {
			to_list_first = ecm_db_multicast_if_first_get_at_index(mc_ifaces_first, heirarchy_index);

			/*
			 * Invalid interface entry, skip
			 */
			if (*to_list_first == ECM_DB_IFACE_HEIRARCHY_MAX) {
				continue;
			}

			ii_temp = ecm_db_multicast_if_heirarchy_get(mc_ifaces, heirarchy_index);

			/*
			 * Try to find a match for this ifindex (dst_dev[if_index]), in any of the
			 * interface instance in the heirarchy. If not found, it means that this
			 * ifindex has joined the group. If found, it means that this ifindex was
			 * already part of the list of destination interfaces.
			 */
			for (ii = ECM_DB_IFACE_HEIRARCHY_MAX - 1; ii >= *to_list_first; ii--) {
				ii_single = ecm_db_multicast_if_instance_get_at_index(ii_temp, ii);
				ifaces = (struct ecm_db_iface_instance **)ii_single;
				to_list = *ifaces;
				ifaces_identifier = ecm_db_iface_interface_identifier_get(to_list);
				if (*dst_if_index == ifaces_identifier) {
					found =  1;
					break;
				}
			}

			if (found) {
				break;
			}
		}

		/*
		 * We did not find a match for the interface in the present list. So mark
		 * it as one that has joined the group.
		 */
		if (!found) {

			/*
			 * Store the if index of the new joinee
			 */
			mc_updates->join_dev[if_cnt] = *dst_if_index;

			/*
			 * Identify a new vacant slot in the 'to_mcast_interfaces' to place
			 * the new interface
			 */
			for (heirarchy_index = 0; heirarchy_index < ECM_DB_MULTICAST_IF_MAX ; heirarchy_index++) {
				to_list_first = ecm_db_multicast_if_first_get_at_index(mc_ifaces_first, heirarchy_index);
				if (*to_list_first == ECM_DB_IFACE_HEIRARCHY_MAX) {
					mc_updates->if_join_idx[heirarchy_index] = 1;
					break;
				}
			}

			if_cnt++;
		}
	}

	ecm_db_multicast_connection_to_interfaces_deref_all(mc_ifaces, mc_ifaces_first);
	mc_updates->if_join_cnt = if_cnt;

	return (if_cnt > 0);
}

/*
 * ecm_interface_multicast_find_updates_to_iface_list()
 * 	Process IGMP/MLD updates either from MFC or bridge snooper. Identity the interfaces
 * 	that have left the group and new interfaces that have joined the group.
 *
 * The function returns true if there was any update necessary to the current destination
 * interface list
 */
bool ecm_interface_multicast_find_updates_to_iface_list(struct ecm_db_connection_instance *ci, struct ecm_multicast_if_update *mc_updates,
						uint32_t flags, bool is_br_snooper, uint32_t *mc_dst_if_index, uint32_t max_to_dev)
{
	bool join;
	bool leave;
	/*
	 * Find destination interfaces that have left the group
	 */
	leave = ecm_interface_multicast_find_outdated_iface_instances(ci, mc_updates, flags, is_br_snooper, mc_dst_if_index, max_to_dev);
	/*
	 * Find new destination interfaces that have joined the group
	 */
	join = ecm_interface_multicast_find_new_iface_instances(ci, mc_updates, mc_dst_if_index, max_to_dev);

	return (leave || join);
}
EXPORT_SYMBOL(ecm_interface_multicast_find_updates_to_iface_list);
#endif

/*
 * ecm_interface_init()
 */
int ecm_interface_init(void)
{
	int result;
	DEBUG_INFO("ECM Interface init\n");

	result = register_netdevice_notifier(&ecm_interface_netdev_notifier);
	if (result != 0) {
		DEBUG_ERROR("Failed to register netdevice notifier %d\n", result);
		return result;
	}
#if defined(ECM_DB_XREF_ENABLE) && defined(ECM_BAND_STEERING_ENABLE)
	/*
	 * register for bridge fdb database modificationevents
	 */
        br_fdb_update_register_notify(&ecm_interface_node_br_fdb_update_nb);
#endif
	return 0;
}
EXPORT_SYMBOL(ecm_interface_init);

/*
 * ecm_interface_exit()
 */
void ecm_interface_exit(void)
{
	DEBUG_INFO("ECM Interface exit\n");

	spin_lock_bh(&ecm_interface_lock);
	ecm_interface_terminate_pending  = true;
	spin_unlock_bh(&ecm_interface_lock);

	unregister_netdevice_notifier(&ecm_interface_netdev_notifier);
#if defined(ECM_DB_XREF_ENABLE) && defined(ECM_BAND_STEERING_ENABLE)
	/*
	 * unregister for bridge fdb update events
	 */
        br_fdb_update_unregister_notify(&ecm_interface_node_br_fdb_update_nb);
#endif
}
EXPORT_SYMBOL(ecm_interface_exit);
