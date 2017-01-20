/* dnsmasq is Copyright (c) 2000 - 2005 Simon Kelley

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 dated June, 1991.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
*/

#include "dnsmasq.h"

static struct frec *frec_list = NULL;

static struct frec *lookup_frec(unsigned short id, unsigned int crc);
static struct frec *lookup_frec_by_sender(unsigned short id,
					  union mysockaddr *addr,
					  unsigned int crc);
static unsigned short get_id(int force, unsigned short force_id, unsigned int crc);

#ifdef DNI_PARENTAL_CTL
static int trans_macaddr(char * mac, char *mac_p);
static int get_macaddr_by_ip(char *mac, char *ip);
static void get_deviceid_by_macaddr(char *macaddr, char *deviceid);
static void parse_parentalcontrol(HEADER *header, int *n, union mysockaddr ipaddr);
static int trans_deviceid(char *deviceid, char *deviceid_b);

#define PC_HTONS_CHARS(n) (unsigned char)((n) >> 8), (unsigned char)(n)
#endif

extern int extract_name(HEADER *header, size_t plen, unsigned char **pp,
                       char *name, int isExtract);

/* Send a UDP packet with it's source address set as "source" 
   unless nowild is true, when we just send it with the kernel default */
static void send_from(int fd, int nowild, char *packet, size_t len, 
		      union mysockaddr *to, struct all_addr *source,
		      unsigned int iface)
{
  struct msghdr msg;
  struct iovec iov[1]; 
  union {
    struct cmsghdr align; /* this ensures alignment */
#if defined(HAVE_LINUX_NETWORK)
    char control[CMSG_SPACE(sizeof(struct in_pktinfo))];
#elif defined(IP_SENDSRCADDR)
    char control[CMSG_SPACE(sizeof(struct in_addr))];
#endif
#ifdef HAVE_IPV6
    char control6[CMSG_SPACE(sizeof(struct in6_pktinfo))];
#endif
  } control_u;
  
  iov[0].iov_base = packet;
  iov[0].iov_len = len;

  msg.msg_control = NULL;
  msg.msg_controllen = 0;
  msg.msg_flags = 0;
  msg.msg_name = to;
  msg.msg_namelen = sa_len(to);
  msg.msg_iov = iov;
  msg.msg_iovlen = 1;
  
  if (!nowild)
    {
      struct cmsghdr *cmptr;
      msg.msg_control = &control_u;
      msg.msg_controllen = sizeof(control_u);
      cmptr = CMSG_FIRSTHDR(&msg);

      if (to->sa.sa_family == AF_INET)
	{
#if defined(HAVE_LINUX_NETWORK)
	  struct in_pktinfo *pkt = (struct in_pktinfo *)CMSG_DATA(cmptr);
	  pkt->ipi_ifindex = 0;
	  pkt->ipi_spec_dst = source->addr.addr4;
	  msg.msg_controllen = cmptr->cmsg_len = CMSG_LEN(sizeof(struct in_pktinfo));
	  cmptr->cmsg_level = SOL_IP;
	  cmptr->cmsg_type = IP_PKTINFO;
#elif defined(IP_SENDSRCADDR)
	  struct in_addr *a = (struct in_addr *)CMSG_DATA(cmptr);
	  *a = source->addr.addr4;
	  msg.msg_controllen = cmptr->cmsg_len = CMSG_LEN(sizeof(struct in_addr));
	  cmptr->cmsg_level = IPPROTO_IP;
	  cmptr->cmsg_type = IP_SENDSRCADDR;
#endif
	}
      else
#ifdef HAVE_IPV6
	{
	  struct in6_pktinfo *pkt = (struct in6_pktinfo *)CMSG_DATA(cmptr);
	  pkt->ipi6_ifindex = iface; /* Need iface for IPv6 to handle link-local addrs */
	  pkt->ipi6_addr = source->addr.addr6;
	  msg.msg_controllen = cmptr->cmsg_len = CMSG_LEN(sizeof(struct in6_pktinfo));
	  cmptr->cmsg_type = IPV6_PKTINFO;
	  cmptr->cmsg_level = IPV6_LEVEL;
	}
#else
      iface = 0; /* eliminate warning */
#endif
    }
  
 retry:
  if (sendmsg(fd, &msg, 0) == -1)
    {
      /* certain Linux kernels seem to object to setting the source address in the IPv6 stack
	 by returning EINVAL from sendmsg. In that case, try again without setting the
	 source address, since it will nearly alway be correct anyway.  IPv6 stinks. */
      if (errno == EINVAL && msg.msg_controllen)
	{
	  msg.msg_controllen = 0;
	  goto retry;
	}
      if (retry_send())
	goto retry;
    }
}
          
static unsigned short search_servers(struct daemon *daemon, time_t now, struct all_addr **addrpp, 
				     unsigned short qtype, char *qdomain, int *type, char **domain)
			      
{
  /* If the query ends in the domain in one of our servers, set
     domain to point to that name. We find the largest match to allow both
     domain.org and sub.domain.org to exist. */
  
  unsigned int namelen = strlen(qdomain);
  unsigned int matchlen = 0;
  struct server *serv;
  unsigned short flags = 0;
  
  for (serv = daemon->servers; serv; serv=serv->next)
    /* domain matches take priority over NODOTS matches */
    if ((serv->flags & SERV_FOR_NODOTS) && *type != SERV_HAS_DOMAIN && !strchr(qdomain, '.') && namelen != 0)
      {
	unsigned short sflag = serv->addr.sa.sa_family == AF_INET ? F_IPV4 : F_IPV6; 
	*type = SERV_FOR_NODOTS;
	if (serv->flags & SERV_NO_ADDR)
	  flags = F_NXDOMAIN;
	else if (serv->flags & SERV_LITERAL_ADDRESS) 
	  { 
	    if (sflag & qtype)
	      {
		flags = sflag;
		if (serv->addr.sa.sa_family == AF_INET) 
		  *addrpp = (struct all_addr *)&serv->addr.in.sin_addr;
#ifdef HAVE_IPV6
		else
		  *addrpp = (struct all_addr *)&serv->addr.in6.sin6_addr;
#endif 
	      }
	    else if (!flags)
	      flags = F_NOERR;
	  } 
      }
    else if (serv->flags & SERV_HAS_DOMAIN)
      {
	unsigned int domainlen = strlen(serv->domain);
	char *matchstart = qdomain + namelen - domainlen;
	if (namelen >= domainlen &&
	    hostname_isequal(matchstart, serv->domain) &&
	    domainlen >= matchlen &&
	    (domainlen == 0 || namelen == domainlen || *(serv->domain) == '.' || *(matchstart-1) == '.' ))
	  {
	    unsigned short sflag = serv->addr.sa.sa_family == AF_INET ? F_IPV4 : F_IPV6;
	    *type = SERV_HAS_DOMAIN;
	    *domain = serv->domain;
	    matchlen = domainlen;
	    if (serv->flags & SERV_NO_ADDR)
	      flags = F_NXDOMAIN;
	    else if (serv->flags & SERV_LITERAL_ADDRESS)
	      {
		if ((sflag | F_QUERY ) & qtype)
		  {
		    flags = qtype & ~F_BIGNAME;
		    if (serv->addr.sa.sa_family == AF_INET) 
		      *addrpp = (struct all_addr *)&serv->addr.in.sin_addr;
#ifdef HAVE_IPV6
		    else
		      *addrpp = (struct all_addr *)&serv->addr.in6.sin6_addr;
#endif
		  }
		else if (!flags)
		  flags = F_NOERR;
	      }
	  } 
      }

  if (flags & ~(F_NOERR | F_NXDOMAIN)) /* flags set here means a literal found */
    {
      if (flags & F_QUERY)
	log_query(F_CONFIG | F_FORWARD | F_NEG, qdomain, NULL, 0, NULL, 0);
      else
	log_query(F_CONFIG | F_FORWARD | flags, qdomain, *addrpp, 0, NULL, 0);
    }
  else if (qtype && !(qtype & F_BIGNAME) && 
	   (daemon->options & OPT_NODOTS_LOCAL) && !strchr(qdomain, '.') && namelen != 0)
    /* don't forward simple names, make exception from NS queries and empty name. */
    flags = F_NXDOMAIN;
    
  if (flags == F_NXDOMAIN && check_for_local_domain(qdomain, now, daemon))
    flags = F_NOERR;

  if (flags == F_NXDOMAIN || flags == F_NOERR)
    log_query(F_CONFIG | F_FORWARD | F_NEG | qtype | (flags & F_NXDOMAIN), qdomain, NULL, 0, NULL, 0);

  return  flags;
}

/* 
 * if_exist()
 * Check whether one network interface exist
 * return 1 if interface exist, and 0 otherwise.
 */
#ifdef BIND_SRVSOCK_TO_WAN
static int if_exist(char* ifname)
{
	int fd, ret = 0;
	struct ifreq ifr;

	if (!ifname || ifname[0] == '\0')
		return 0;

	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0)
		return 0;

	memset(&ifr, 0, sizeof(struct ifreq));
	ifr.ifr_addr.sa_family = AF_INET;
	strcpy(ifr.ifr_name, ifname);

	if (ioctl(fd, SIOCGIFINDEX, &ifr) == 0)
		ret = 1;

	close(fd);
	return ret;
}
#endif

//ifdef SUP_MUL_PPPOE
int mulpppoe_skip_dns(struct in_addr srv, char *name)
{
	FILE *fdns, *fdomain, *fp; 
	char buf[32] = {0}, buffer[512] = {0}, *p;
	int ppp1_dns = 0, ppp1_domain = 0, skip = 0, ppp0_dns = 0;
	if((fp = fopen("/etc/ppp/enable_ppp1", "r")) == NULL )
	{
		my_syslog(LOG_INFO, "ppp1 not enable, return");
		return 0;
	}
	fclose(fp);
	if((fdns = fopen("/etc/ppp/pppoe2-dns.conf", "r")) == NULL)
	{
		my_syslog(LOG_INFO, "Can't open dns config file of ppp1");
		ppp1_dns = -1;
	} else {
		while(fgets(buf, 32, fdns)) {
			my_syslog(LOG_INFO, "get ppp1 dns:%s", buf);
			if(inet_addr(buf) == srv.s_addr) {
				ppp1_dns = 1;
				break;
			}
		}
		fclose(fdns);
	}

	if((fdns = fopen("/etc/ppp/pppoe1-dns.conf", "r")) == NULL)
        {
                my_syslog(LOG_INFO, "Can't open dns config file of ppp0");
                ppp0_dns = -1;
        } else {
		memset(buf, 0, sizeof(buf));
                while(fgets(buf, 32, fdns)) {
                        my_syslog(LOG_INFO, "get ppp0 dns:%s", buf);
                        if(inet_addr(buf) == srv.s_addr) {
                                ppp0_dns = 1;
                                break;
                        }
                }
                fclose(fdns);
        }

	if((fdomain = fopen("/etc/ppp/pppoe2-domain.conf", "r")) == NULL)
        {
                my_syslog(LOG_INFO, "Can't open domain file of ppp1");
		ppp1_domain = -1;
        } else {
		while(fgets(buffer, 512, fdomain)) {
                	/* Remove \n */
	                buffer[strlen(buffer) - 1] = '\0';
			if(buffer[0] != '*') {
	        	        if(strstr(name, buffer)) {
        	        	    	 ppp1_domain = 1;
				}
	                } else {
				if(strstr(name, buffer + 1)){
					p = strstr(name, buffer + 1) + strlen(buffer + 1);
					if(p[0] == '\0'){
						ppp1_domain = 1;
					}
				}
			}
        	}
	        fclose(fdomain);
	}

	/* Fix Bug 38166 - [SQA-3006][Multi-PPPoE]Users cannot access Internet website
	 * after multi-PPPoE sessions connected and when session1 and session2 get the
	 * same DNS server.
	 */
	if(ppp0_dns == 1 && ppp1_dns == 1)
		return 0;
	/* visit policy domain */
	if(ppp1_domain == 1) {
		/* ppp0 dns or has not ppp1 dns */
		if(ppp1_dns != 1) {
			my_syslog(LOG_INFO, "return 1");
			return 1;
		}
	/* visit internet or has not ppp1 policy domain */
	} else {
		/* ppp1 dns */
		if(ppp1_dns == 1)
			return 1;
	}
	return 0;
}
//endif

static struct server* get_first_server(struct daemon *daemon, unsigned short gotname, int type)
{
  struct server *serv;
  
  for (serv = daemon->servers; serv; serv = serv->next) {
    if (gotname == F_IPV4 && serv->addr.sa.sa_family == AF_INET)
      break;
    else if (gotname == F_IPV6 && serv->addr.sa.sa_family == AF_INET6)
      break;
  }

  if (serv)
    return serv;
  if (type != 0 || !daemon->last_server || (daemon->options & OPT_ORDER))
    return daemon->servers;
  else
    return daemon->last_server;
}

/* returns new last_server */	
static void forward_query(struct daemon *daemon, int udpfd, union mysockaddr *udpaddr,
			  struct all_addr *dst_addr, unsigned int dst_iface,
			  HEADER *header, size_t plen, time_t now, struct frec *forward)
{
  char *domain = NULL;
  int type = 0;
  struct all_addr *addrp = NULL;
  unsigned int crc = questions_crc(header, plen, daemon->namebuff);
  unsigned short flags = 0;
  unsigned short gotname = extract_request(header, plen, daemon->namebuff, NULL);
  struct server *start = NULL;
#ifdef BIND_SRVSOCK_TO_WAN
  struct ifreq ifr;
#endif
    
  /* may be no servers available. */
  if (!daemon->servers)
    forward = NULL;
  else if (forward || (forward = lookup_frec_by_sender(ntohs(header->id), udpaddr, crc)))
    {
      /* retry on existing query, send to all available servers  */
      domain = forward->sentto->domain;
      if (!(daemon->options & OPT_ORDER))
	{
	  forward->forwardall = 1;
#ifdef DNI_IPV6_FEATURE
	  forward->time = now;
          if (2 == forward->fwd_sign)
	    forward->fwd_sign = 0;
#endif
	  daemon->last_server = NULL;
	}
      type = forward->sentto->flags & SERV_TYPE;
      if (!(start = forward->sentto->next))
	start = daemon->servers; /* at end of list, recycle */
      header->id = htons(forward->new_id);
    }
  else 
    {
      if (gotname)
	flags = search_servers(daemon, now, &addrp, gotname, daemon->namebuff, &type, &domain);
      
      if (!flags && !(forward = get_new_frec(daemon, now, NULL)))
	/* table full - server failure. */
	flags = F_NEG;
      
      if (forward)
	{
	  /* force unchanging id for signed packets */
	  int is_sign;
	  find_pseudoheader(header, plen, NULL, NULL, &is_sign);
	  
	  forward->source = *udpaddr;
	  forward->dest = *dst_addr;
	  forward->iface = dst_iface;
	  forward->orig_id = ntohs(header->id);
	  forward->new_id = get_id(is_sign, forward->orig_id, crc);
	  forward->fd = udpfd;
	  forward->crc = crc;
	  forward->forwardall = 0;
#ifdef DNI_IPV6_FEATURE
	  unsigned char *p = (unsigned char *)(header+1);
	  if (F_IPV4 == gotname || F_IPV6 == gotname)
	    {
	      memcpy(&forward->flags, (unsigned char *)header + 2, sizeof(forward->flags));
	      memset(forward->name, 0x00, sizeof(forward->name));
	      extract_name(header, plen, &p, forward->name, 1);
	      GETSHORT(forward->type, p);
	      GETSHORT(forward->class, p);
	    }
	  forward->fwd_sign = 0;
#else
	  unsigned char *p = (unsigned char *)(header+1);
	  if (F_IPV4 == gotname || F_IPV6 == gotname)
	    {
	      memset(forward->name, 0x00, sizeof(forward->name));
	      extract_name(header, plen, &p, forward->name, 1);
	    }
#endif
	  header->id = htons(forward->new_id);

#if 1
	  start = get_first_server(daemon, gotname, type);
#else		  
	  /* In strict_order mode, or when using domain specific servers
	     always try servers in the order specified in resolv.conf,
	     otherwise, use the one last known to work. */
	  
	  if (type != 0  || (daemon->options & OPT_ORDER))
	    start = daemon->servers;
	  else if (!(start = daemon->last_server))
	    {
	      start = daemon->servers;
	      forward->forwardall = 1;
	    }
#endif
	}
    }

  /* check for send errors here (no route to host) 
     if we fail to send to all nameservers, send back an error
     packet straight away (helps modem users when offline)  */
  
  if (!flags && forward)
    {
      struct server *firstsentto = start;
      int forwarded = 0;

#ifdef SUP_STATIC_PPTP
      if (1 == daemon->static_pptp_enable) {
        add_static_pptp_record(daemon, daemon->namebuff, header, plen);
      }
#endif

      while (1)
	{ 
	  /* only send to servers dealing with our domain.
	     domain may be NULL, in which case server->domain 
	     must be NULL also. */
	  
	  if (type == (start->flags & SERV_TYPE) &&
	      (type != SERV_HAS_DOMAIN || hostname_isequal(domain, start->domain)) &&
	      !(start->flags & SERV_LITERAL_ADDRESS))
	    {
#ifdef DNI_IPV6_FEATURE
	      if (!daemon->diff_svr || !(F_IPV6 == gotname || F_IPV4 == gotname))
		  ;
	      else if (F_IPV6 == gotname)
	        {
		  if (0 == forward->fwd_sign && AF_INET6 != start->addr.sa.sa_family)
	            goto again;
		  if (1 == forward->fwd_sign && AF_INET != start->addr.sa.sa_family)
		    goto again;
	        }
	      else
	        {
		  if (0 == forward->fwd_sign && AF_INET != start->addr.sa.sa_family)
		    goto again;
		  if (1 == forward->fwd_sign && AF_INET6 != start->addr.sa.sa_family)
		    goto again;
	        }
#endif
#ifdef BIND_SRVSOCK_TO_WAN
	      /* To fix bug 22747, bind server socket to WAN interface if hasn't been bind successfully */
	      if (bind_wan_success == 0 && if_exist(wan_ifname) == 1)
		{
		  memset(&ifr, 0, sizeof(ifr));
		  strcpy(ifr.ifr_name, wan_ifname);
		  if(setsockopt(start->sfd->fd, SOL_SOCKET, SO_BINDTODEVICE, (char *)&ifr, sizeof(ifr)) == 0)
		    {
		      my_syslog(LOG_INFO, _("Bind server socket to interface %s successfully"), wan_ifname);
		      bind_wan_success = 1;
		    }
		  else
		    my_syslog(LOG_INFO, _("Bind server socket to interface %s failed, reason: %m"), wan_ifname);
		}
#endif
//ifdef SUP_MUL_PPPOE
             my_syslog(LOG_INFO, "#####query domain is:%s\n", forward->name);
             /* Check if not match interface, if yes, skip, bellow for example
              * www.baidu.com dns but server is on ppp1, skip
              * www.flets.com dns but server is on ppp0, skip
              */
             if(mulpppoe_skip_dns(start->addr.in.sin_addr, forward->name)) {
                  if(!(start = start->next))
                        start = daemon->servers;
                  if (start == firstsentto)
                        break;
                  continue;
                }
//endif
	      if (sendto(start->sfd->fd, (char *)header, plen, 0,
			 &start->addr.sa,
			 sa_len(&start->addr)) == -1)
		{
#ifdef BIND_SRVSOCK_TO_WAN
		  int errsendto = errno;
		  if (if_exist(wan_ifname) == 1)
		      my_syslog(LOG_INFO, "Fail to forward qurey to upstream server: %s", strerror(errsendto));
		  /* 
		   * To fix bug 22747, sendto() will fail with ENODEV if WAN interface disappear for some reason,
		   * and will still fail with ENODEV even though WAN interface appear (because index of virtual interface
		   * such as ppp0 in kernel will be changed after virtual interface disappear/appear)
		   * so, when this issue happen, we must re-bind again if WAN interface still exist,
		   * and cancel bind if WAN interface does not exist.
		   */
		  if (errsendto == ENODEV && bind_wan_success == 1)
		    {
		      if (if_exist(wan_ifname) == 1)
			{
			  /* re-bind again */
			  memset(&ifr, 0, sizeof(ifr));
			  strcpy(ifr.ifr_name, wan_ifname);
			  if (setsockopt(start->sfd->fd, SOL_SOCKET, SO_BINDTODEVICE, (char *)&ifr, sizeof(ifr)) == 0)
			    {
			      my_syslog(LOG_INFO, _("Re-bind server socket to interface %s successfully"), wan_ifname);
			    }
			  else
			    {
			      my_syslog(LOG_INFO, _("Re-bind server socket to interface %s failed, reason: %m"), wan_ifname);
			    }
			}
		      else
			{
			  /* cancel bind */
			  memset(&ifr, 0, sizeof(ifr));
			  strcpy(ifr.ifr_name, "");
			  if (setsockopt(start->sfd->fd, SOL_SOCKET, SO_BINDTODEVICE, (char *)&ifr, sizeof(ifr)) == 0)
			    {
			      my_syslog(LOG_INFO, _("Cancel bind server socket to interface %s successfully"), wan_ifname);
			      bind_wan_success = 0;
			    }
			}
		    }
#endif
		  if (retry_send())
		    continue;
		}
	      else
		{
		  /* Keep info in case we want to re-send this packet */
		  daemon->srv_save = start;
		  daemon->packet_len = plen;
		  
		  if (!gotname)
		    strcpy(daemon->namebuff, "query");
		  if (start->addr.sa.sa_family == AF_INET)
		    log_query(F_SERVER | F_IPV4 | F_FORWARD, daemon->namebuff, 
			      (struct all_addr *)&start->addr.in.sin_addr, 0,
			      NULL, 0); 
#ifdef HAVE_IPV6
		  else
		    log_query(F_SERVER | F_IPV6 | F_FORWARD, daemon->namebuff, 
			      (struct all_addr *)&start->addr.in6.sin6_addr, 0,
			      NULL, 0);
#endif 
		  forwarded = 1;
		  forward->sentto = start;
		  if (!forward->forwardall) 
		    break;
		  forward->forwardall++;
		}
	    } 
	  
#ifdef DNI_IPV6_FEATURE
again:
#endif
	  if (!(start = start->next))
 	    start = daemon->servers;
	  
	  if (start == firstsentto)
	    break;
	}
      
#ifdef DNI_IPV6_FEATURE
      if (!daemon->diff_svr || !(F_IPV6 == gotname || F_IPV4 == gotname))
        forward->fwd_sign = 2;
      else
        forward->fwd_sign++;
      if (forwarded)
        {
          return;
        }
      else if (1 == forward->fwd_sign)
        {
	  return;
        }
#else
      if (forwarded)
	  return;
#endif
      
      /* could not send on, prepare to return */ 
      header->id = htons(forward->orig_id);
      forward->sentto = NULL; /* cancel */
    }	  
  
  /* could not send on, return empty answer or address if known for whole domain */
  if (udpfd != -1)
    {
      plen = setup_reply(header, plen, addrp, flags, daemon->local_ttl);
      send_from(udpfd, daemon->options & OPT_NOWILD, (char *)header, plen, udpaddr, dst_addr, dst_iface);
    }

  return;
}

static size_t process_reply(struct daemon *daemon, HEADER *header, time_t now, 
			    struct server *server, size_t n)
{
  unsigned char *pheader, *sizep;
  int munged = 0, is_sign;
  size_t plen; 

  /* If upstream is advertising a larger UDP packet size
	 than we allow, trim it so that we don't get overlarge
	 requests for the client. We can't do this for signed packets. */

  if ((pheader = find_pseudoheader(header, n, &plen, &sizep, &is_sign)) && !is_sign)
    {
      unsigned short udpsz;
      unsigned char *psave = sizep;
      
      GETSHORT(udpsz, sizep);
      if (udpsz > daemon->edns_pktsz)
	PUTSHORT(daemon->edns_pktsz, psave);
    }

  if (header->opcode != QUERY || (header->rcode != NOERROR && header->rcode != NXDOMAIN))
    return n;
  
  /* Complain loudly if the upstream server is non-recursive. */
  if (!header->ra && header->rcode == NOERROR && ntohs(header->ancount) == 0 &&
      server && !(server->flags & SERV_WARNED_RECURSIVE))
    {
      prettyprint_addr(&server->addr, daemon->namebuff);
      my_syslog(LOG_WARNING, _("nameserver %s refused to do a recursive query"), daemon->namebuff);
      if (!(daemon->options & OPT_LOG))
	server->flags |= SERV_WARNED_RECURSIVE;
    }  
    
  if (daemon->bogus_addr && header->rcode != NXDOMAIN &&
      check_for_bogus_wildcard(header, n, daemon->namebuff, daemon->bogus_addr, now))
    {
      munged = 1;
      header->rcode = NXDOMAIN;
      header->aa = 0;
    }
  else 
    {
      if (header->rcode == NXDOMAIN && 
	  extract_request(header, n, daemon->namebuff, NULL) &&
	  check_for_local_domain(daemon->namebuff, now, daemon))
	{
	  /* if we forwarded a query for a locally known name (because it was for 
	     an unknown type) and the answer is NXDOMAIN, convert that to NODATA,
	     since we know that the domain exists, even if upstream doesn't */
	  munged = 1;
	  header->aa = 1;
	  header->rcode = NOERROR;
	}
      
      extract_addresses(header, n, daemon->namebuff, now, daemon);
    }
  
  /* do this after extract_addresses. Ensure NODATA reply and remove
     nameserver info. */
  
  if (munged)
    {
      header->ancount = htons(0);
      header->nscount = htons(0);
      header->arcount = htons(0);
    }
  
  /* the bogus-nxdomain stuff, doctor and NXDOMAIN->NODATA munging can all elide
     sections of the packet. Find the new length here and put back pseudoheader
     if it was removed. */
  return resize_packet(header, n, pheader, plen);
}

/* sets new last_server */
void reply_query(struct serverfd *sfd, struct daemon *daemon, time_t now)
{
  /* packet from peer server, extract data for cache, and send to
     original requester */
  HEADER *header;
  union mysockaddr serveraddr;
  struct frec *forward;
  socklen_t addrlen = sizeof(serveraddr);
  ssize_t n = recvfrom(sfd->fd, daemon->packet, daemon->edns_pktsz, 0, &serveraddr.sa, &addrlen);
  size_t nn;

  /* packet buffer overwritten */
  daemon->srv_save = NULL;
  
  /* Determine the address of the server replying  so that we can mark that as good */
  serveraddr.sa.sa_family = sfd->source_addr.sa.sa_family;
#ifdef HAVE_IPV6
  if (serveraddr.sa.sa_family == AF_INET6)
    serveraddr.in6.sin6_flowinfo = 0;
#endif
  
  header = (HEADER *)daemon->packet;
  
  if (n >= (int)sizeof(HEADER) && header->qr && 
      (forward = lookup_frec(ntohs(header->id), questions_crc(header, n, daemon->namebuff))))
    {
      struct server *server = forward->sentto;
      
#ifdef DNI_IPV6_FEATURE
      if ((header->rcode == SERVFAIL || header->rcode == REFUSED ||
	   header->rcode == NXDOMAIN || header->rcode == NOTIMP ||
	   header->rcode == FORMERR) && (1 == forward->fwd_sign))
        {
	  unsigned char *pheader;
	  size_t plen;
	  int is_sign;
	  /* recreate query from reply */
	  pheader = find_pseudoheader(header, (size_t)n, &plen, NULL, &is_sign);
	  header->ancount = htons(0);
	  header->nscount = htons(0);
	  header->arcount = htons(0);
	  if ((nn = resize_packet(header, (size_t)n, pheader, plen)))
	    {
	       header->qr = 0;
	       header->tc = 0;
	       forward_query(daemon, forward->fd, &forward->source,
                 &forward->dest, forward->iface, header, nn, now, forward);
	       return;
	    }
        }
#endif
      /* If try-all-ns are set, treat NXDOMAIN as a failed request */
      if ((daemon->options & OPT_TRY_ALL_NS) && header->rcode == NXDOMAIN)
        {
          char strserverip[ADDRSTRLEN];
          inet_ntop(serveraddr.sa.sa_family, serveraddr.sa.sa_family == AF_INET ? &serveraddr.in.sin_addr : &serveraddr.in6.sin6_addr, strserverip, ADDRSTRLEN);
          my_syslog(LOG_INFO, "get reply with response code NXDOMAIN from %s for domain %s, treat as SERVFAIL\n", strserverip, forward->name);
          header->rcode = SERVFAIL;
        }

      if ((header->rcode == SERVFAIL || header->rcode == REFUSED) &&
	  !(daemon->options & OPT_ORDER) &&
	  forward->forwardall == 0)
	/* for broken servers, attempt to send to another one. */
	{
	  unsigned char *pheader;
	  size_t plen;
	  int is_sign;
	  
	  /* recreate query from reply */
	  pheader = find_pseudoheader(header, (size_t)n, &plen, NULL, &is_sign);
	  if (!is_sign)
	    {
	      header->ancount = htons(0);
	      header->nscount = htons(0);
	      header->arcount = htons(0);
	      if ((nn = resize_packet(header, (size_t)n, pheader, plen)))
		{
		  header->qr = 0;
		  header->tc = 0;
		  forward_query(daemon, -1, NULL, NULL, 0, header, nn, now, forward);
		  return;
		}
	    }
	}   
      
      if ((forward->sentto->flags & SERV_TYPE) == 0)
	{
	  if (header->rcode == SERVFAIL || header->rcode == REFUSED)
	    server = NULL;
	  else
	    {
	      struct server *last_server;
	      /* find good server by address if possible, otherwise assume the last one we sent to */ 
	      for (last_server = daemon->servers; last_server; last_server = last_server->next)
		if (!(last_server->flags & (SERV_LITERAL_ADDRESS | SERV_HAS_DOMAIN | SERV_FOR_NODOTS | SERV_NO_ADDR)) &&
		    sockaddr_isequal(&last_server->addr, &serveraddr))
		  {
		    server = last_server;
		    break;
		  }
	    } 
	  daemon->last_server = server;
	}
      
      /* If the answer is an error, keep the forward record in place in case
	 we get a good reply from another server. Kill it when we've
	 had replies from all to avoid filling the forwarding table when
	 everything is broken */
      if (forward->forwardall == 0 || --forward->forwardall == 1 || 
	  (header->rcode != REFUSED && header->rcode != SERVFAIL))
	{
	  if ((nn = process_reply(daemon, header, now, server, (size_t)n)))
	    {
//ifdef SUP_MUL_PPPOE
	      if (header->rcode == NOERROR) { /* No error occurred */
	          check_mul_pppoe_record(header, nn);
	      }
//endif
	      header->id = htons(forward->orig_id);
	      header->ra = 1; /* recursion if available */
	      send_from(forward->fd, daemon->options & OPT_NOWILD, daemon->packet, nn, 
			&forward->source, &forward->dest, forward->iface);
#ifdef SUP_STATIC_PPTP
	      if (1 == daemon->static_pptp_enable) {
	        if (header->rcode == NOERROR) { /* No error occurred */
	          char namebuff[512] = {0};
		  extract_request(header, nn, namebuff, NULL);
		  del_static_pptp_record(daemon, namebuff);
		  /* add static route */
		  struct serverfd *tmp_sp;
		  for (tmp_sp = daemon->sp_sfd; tmp_sp; tmp_sp = tmp_sp->next) {
		    if (serveraddr.in.sin_addr.s_addr == tmp_sp->source_addr.in.sin_addr.s_addr) {
		      check_static_pptp(daemon->packet, nn, daemon);
		      break;
		    }
		  }
		}
	      }
#endif
	    }
	  forward->sentto = NULL; /* cancel */
	}
    }
}

void receive_query(struct listener *listen, struct daemon *daemon, time_t now)
{
  HEADER *header = (HEADER *)daemon->packet;
  union mysockaddr source_addr;
  unsigned short type;
  struct all_addr dst_addr;
  struct in_addr netmask, dst_addr_4;
  size_t m;
  ssize_t n;
  int if_index = 0;
  struct iovec iov[1];
  struct msghdr msg;
  struct cmsghdr *cmptr;
  union {
    struct cmsghdr align; /* this ensures alignment */
#ifdef HAVE_IPV6
    char control6[CMSG_SPACE(sizeof(struct in6_pktinfo))];
#endif
#if defined(HAVE_LINUX_NETWORK)
    char control[CMSG_SPACE(sizeof(struct in_pktinfo))];
#elif defined(IP_RECVDSTADDR)
    char control[CMSG_SPACE(sizeof(struct in_addr)) +
		 CMSG_SPACE(sizeof(struct sockaddr_dl))];
#endif
  } control_u;
  
  /* packet buffer overwritten */
  daemon->srv_save = NULL;
  
  if (listen->family == AF_INET && (daemon->options & OPT_NOWILD))
    {
      dst_addr_4 = listen->iface->addr.in.sin_addr;
      netmask = listen->iface->netmask;
    }
  else
    {
      dst_addr_4.s_addr = 0;
      netmask.s_addr = 0;
    }

  iov[0].iov_base = daemon->packet;
  iov[0].iov_len = daemon->edns_pktsz;
    
  msg.msg_control = control_u.control;
  msg.msg_controllen = sizeof(control_u);
  msg.msg_flags = 0;
  msg.msg_name = &source_addr;
  msg.msg_namelen = sizeof(source_addr);
  msg.msg_iov = iov;
  msg.msg_iovlen = 1;
  
  if ((n = recvmsg(listen->fd, &msg, 0)) == -1)
    return;
  
  if (n < (int)sizeof(HEADER) || 
      (msg.msg_flags & MSG_TRUNC) ||
      header->qr)
    return;
  
  source_addr.sa.sa_family = listen->family;
#ifdef HAVE_IPV6
  if (listen->family == AF_INET6)
    source_addr.in6.sin6_flowinfo = 0;
#endif
  
  if (!(daemon->options & OPT_NOWILD))
    {
      struct ifreq ifr;

      if (msg.msg_controllen < sizeof(struct cmsghdr))
	return;

#if defined(HAVE_LINUX_NETWORK)
      if (listen->family == AF_INET)
	for (cmptr = CMSG_FIRSTHDR(&msg); cmptr; cmptr = CMSG_NXTHDR(&msg, cmptr))
	  if (cmptr->cmsg_level == SOL_IP && cmptr->cmsg_type == IP_PKTINFO)
	    {
	      dst_addr_4 = dst_addr.addr.addr4 = ((struct in_pktinfo *)CMSG_DATA(cmptr))->ipi_spec_dst;
	      if_index = ((struct in_pktinfo *)CMSG_DATA(cmptr))->ipi_ifindex;
	    }
#elif defined(IP_RECVDSTADDR) && defined(IP_RECVIF)
      if (listen->family == AF_INET)
	{
	  for (cmptr = CMSG_FIRSTHDR(&msg); cmptr; cmptr = CMSG_NXTHDR(&msg, cmptr))
	    if (cmptr->cmsg_level == IPPROTO_IP && cmptr->cmsg_type == IP_RECVDSTADDR)
	      dst_addr_4 = dst_addr.addr.addr4 = *((struct in_addr *)CMSG_DATA(cmptr));
	    else if (cmptr->cmsg_level == IPPROTO_IP && cmptr->cmsg_type == IP_RECVIF)
	      if_index = ((struct sockaddr_dl *)CMSG_DATA(cmptr))->sdl_index;
	}
#endif
      
#ifdef HAVE_IPV6
      if (listen->family == AF_INET6)
	{
	  for (cmptr = CMSG_FIRSTHDR(&msg); cmptr; cmptr = CMSG_NXTHDR(&msg, cmptr))
	    if (cmptr->cmsg_level == IPV6_LEVEL && cmptr->cmsg_type == IPV6_PKTINFO)
	      {
		dst_addr.addr.addr6 = ((struct in6_pktinfo *)CMSG_DATA(cmptr))->ipi6_addr;
		if_index =((struct in6_pktinfo *)CMSG_DATA(cmptr))->ipi6_ifindex;
	      }
	}
#endif
      
      /* enforce available interface configuration */
      
      if (if_index == 0)
	return;
      
#ifdef SIOCGIFNAME
      ifr.ifr_ifindex = if_index;
      if (ioctl(listen->fd, SIOCGIFNAME, &ifr) == -1)
	return;
#else
      if (!if_indextoname(if_index, ifr.ifr_name))
	return;
#endif
      
      if (!iface_check(daemon, listen->family, &dst_addr, &ifr, &if_index))
	return;
      
      if (listen->family == AF_INET &&
	  (daemon->options & OPT_LOCALISE) &&
	  ioctl(listen->fd, SIOCGIFNETMASK, &ifr) == -1)
	return;
      
      netmask = ((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr;
    }
  
  if (extract_request(header, (size_t)n, daemon->namebuff, &type))
    {
      if (listen->family == AF_INET) 
	log_query(F_QUERY | F_IPV4 | F_FORWARD, daemon->namebuff, 
		  (struct all_addr *)&source_addr.in.sin_addr, type, NULL, 0);
#ifdef HAVE_IPV6
      else
	log_query(F_QUERY | F_IPV6 | F_FORWARD, daemon->namebuff, 
		  (struct all_addr *)&source_addr.in6.sin6_addr, type, NULL, 0);
#endif
    }

#ifdef HAVE_IPV6
  if((m = answer_request (header, \
			  ((char *) header) + PACKETSZ, \
			  (size_t)n, \
			  daemon, \
			  dst_addr_4, \
			  netmask, \
			  now) ) == -1)
    return; //We would do nothing for this request.
#else
  m = answer_request (header, ((char *) header) + PACKETSZ, (size_t)n, daemon, 
		      dst_addr_4, netmask, now);
#endif
  if (m >= 1)
    send_from(listen->fd, daemon->options & OPT_NOWILD, (char *)header, m, &source_addr, &dst_addr, if_index);
  else
#ifdef DNI_PARENTAL_CTL
  {
    if (1 == parentalcontrol_enable)
        parse_parentalcontrol(header, &n, source_addr);
#endif
    forward_query(daemon, listen->fd, &source_addr, &dst_addr, if_index,
		  header, (size_t)n, now, NULL);
#ifdef DNI_PARENTAL_CTL
  }
#endif
}

/* The daemon forks before calling this: it should deal with one connection,
   blocking as neccessary, and then return. Note, need to be a bit careful
   about resources for debug mode, when the fork is suppressed: that's
   done by the caller. */
unsigned char *tcp_request(struct daemon *daemon, int confd, time_t now,
			   struct in_addr local_addr, struct in_addr netmask)
{
  int size = 0;
  size_t m;
  unsigned short qtype, gotname;
  unsigned char c1, c2;
  /* Max TCP packet + slop */
  unsigned char *packet = malloc(65536 + MAXDNAME + RRFIXEDSZ);
  HEADER *header;
  struct server *last_server;
  
  while (1)
    {
      if (!packet ||
	  !read_write(confd, &c1, 1, 1) || !read_write(confd, &c2, 1, 1) ||
	  !(size = c1 << 8 | c2) ||
	  !read_write(confd, packet, size, 1))
       	return packet; 
  
      if (size < (int)sizeof(HEADER))
	continue;
      
      header = (HEADER *)packet;
      
      if ((gotname = extract_request(header, (unsigned int)size, daemon->namebuff, &qtype)))
	{
	  union mysockaddr peer_addr;
	  socklen_t peer_len = sizeof(union mysockaddr);
	  
	  if (getpeername(confd, (struct sockaddr *)&peer_addr, &peer_len) != -1)
	    {
	      if (peer_addr.sa.sa_family == AF_INET) 
		log_query(F_QUERY | F_IPV4 | F_FORWARD, daemon->namebuff, 
			  (struct all_addr *)&peer_addr.in.sin_addr, qtype, NULL, 0);
#ifdef HAVE_IPV6
	      else
		log_query(F_QUERY | F_IPV6 | F_FORWARD, daemon->namebuff, 
			  (struct all_addr *)&peer_addr.in6.sin6_addr, qtype, NULL, 0);
#endif
	    }
	}
      
      /* m > 0 if answered from cache */
      m = answer_request(header, ((char *) header) + 65536, (unsigned int)size, daemon, 
			 local_addr, netmask, now);
      
      if (m == 0)
	{
	  unsigned short flags = 0;
	  struct all_addr *addrp = NULL;
	  struct timeval timeout={3, 0};
	  int type = 0;
	  char *domain = NULL;
	  
	  if (gotname)
	    flags = search_servers(daemon, now, &addrp, gotname, daemon->namebuff, &type, &domain);
	  
	  if (type != 0  || (daemon->options & OPT_ORDER) || !daemon->last_server)
	    last_server = daemon->servers;
	  else
	    last_server = daemon->last_server;
      
	  if (!flags && last_server)
	    {
	      struct server *firstsendto = NULL;
	      unsigned int crc = questions_crc(header, (unsigned int)size, daemon->namebuff);

	      /* Loop round available servers until we succeed in connecting to one.
	         Note that this code subtley ensures that consecutive queries on this connection
	         which can go to the same server, do so. */
	      while (1) 
		{
		  if (!firstsendto)
		    firstsendto = last_server;
		  else
		    {
		      if (!(last_server = last_server->next))
			last_server = daemon->servers;
		      
		      if (last_server == firstsendto)
			break;
		    }
	      
		  /* server for wrong domain */
		  if (type != (last_server->flags & SERV_TYPE) ||
		      (type == SERV_HAS_DOMAIN && !hostname_isequal(domain, last_server->domain)))
		    continue;
		  
		  if ((last_server->tcpfd == -1) &&
		      (last_server->tcpfd = socket(last_server->addr.sa.sa_family, SOCK_STREAM, 0)) != -1 &&
		      (setsockopt(last_server->tcpfd,SOL_SOCKET,SO_SNDTIMEO,&timeout,sizeof(timeout)) == -1 ||
		       setsockopt(last_server->tcpfd,SOL_SOCKET,SO_RCVTIMEO,&timeout,sizeof(timeout)) == -1 ||
		       connect(last_server->tcpfd, &last_server->addr.sa, sa_len(&last_server->addr)) == -1 ))
		    {
		      close(last_server->tcpfd);
		      last_server->tcpfd = -1;
		    }
		  
		  if (last_server->tcpfd == -1)	
		    continue;
		  
		  c1 = size >> 8;
		  c2 = size;
		  
		  if (!read_write(last_server->tcpfd, &c1, 1, 0) ||
		      !read_write(last_server->tcpfd, &c2, 1, 0) ||
		      !read_write(last_server->tcpfd, packet, size, 0) ||
		      !read_write(last_server->tcpfd, &c1, 1, 1) ||
		      !read_write(last_server->tcpfd, &c2, 1, 1))
		    {
		      close(last_server->tcpfd);
		      last_server->tcpfd = -1;
		      continue;
		    } 
	      
		  m = (c1 << 8) | c2;
		  if (!read_write(last_server->tcpfd, packet, m, 1))
		    return packet;
		  
		  if (!gotname)
		    strcpy(daemon->namebuff, "query");
		  if (last_server->addr.sa.sa_family == AF_INET)
		    log_query(F_SERVER | F_IPV4 | F_FORWARD, daemon->namebuff, 
			      (struct all_addr *)&last_server->addr.in.sin_addr, 0, NULL, 0); 
#ifdef HAVE_IPV6
		  else
		    log_query(F_SERVER | F_IPV6 | F_FORWARD, daemon->namebuff, 
			      (struct all_addr *)&last_server->addr.in6.sin6_addr, 0, NULL, 0);
#endif 
		  
		  /* There's no point in updating the cache, since this process will exit and
		     lose the information after a few queries. We make this call for the alias and 
		     bogus-nxdomain side-effects. */
		  /* If the crc of the question section doesn't match the crc we sent, then
		     someone might be attempting to insert bogus values into the cache by 
		     sending replies containing questions and bogus answers. */
		  if (crc == questions_crc(header, (unsigned int)m, daemon->namebuff))
		    m = process_reply(daemon, header, now, last_server, (unsigned int)m);
		  
		  break;
		}
	    }
	  
	  /* In case of local answer or no connections made. */
	  if (m == 0)
	    m = setup_reply(header, (unsigned int)size, addrp, flags, daemon->local_ttl);
	}
      
      c1 = m>>8;
      c2 = m;
      if (!read_write(confd, &c1, 1, 0) ||
	  !read_write(confd, &c2, 1, 0) || 
	  !read_write(confd, packet, m, 0))
	return packet;
    }
}

static struct frec *allocate_frec(time_t now)
{
  struct frec *f;
  
  if ((f = (struct frec *)malloc(sizeof(struct frec))))
    {
      f->next = frec_list;
      f->time = now;
      f->sentto = NULL;
      frec_list = f;
    }

  return f;
}

/* if wait==NULL return a free or older than TIMEOUT record.
   else return *wait zero if one available, or *wait is delay to
   when the oldest in-use record will expire. */
struct frec *get_new_frec(struct daemon *daemon, time_t now, int *wait)
{
  struct frec *f, *oldest;
  int count;
  
  if (wait)
    *wait = 0;

  for (f = frec_list, oldest = NULL, count = 0; f; f = f->next, count++)
    if (!f->sentto)
      {
	f->time = now;
	return f;
      }
    else if (!oldest || difftime(f->time, oldest->time) <= 0)
      oldest = f;
  
  /* can't find empty one, use oldest if there is one
     and it's older than timeout */
  if (oldest && ((int)difftime(now, oldest->time)) >= TIMEOUT)
    { 
      /* keep stuff for twice timeout if we can by allocating a new
	 record instead */
      if (difftime(now, oldest->time) < 2*TIMEOUT && 
	  count <= daemon->ftabsize &&
	  (f = allocate_frec(now)))
	return f;

      if (!wait)
	{
	  oldest->sentto = 0;
	  oldest->time = now;
	}
      return oldest;
    }
  
  /* none available, calculate time 'till oldest record expires */
  if (count > daemon->ftabsize)
    {
      if (oldest && wait)
	*wait = oldest->time + (time_t)TIMEOUT - now;
      return NULL;
    }
  
  if (!(f = allocate_frec(now)) && wait)
    /* wait one second on malloc failure */
    *wait = 1;

  return f; /* OK if malloc fails and this is NULL */
}
 
/* crc is all-ones if not known. */
static struct frec *lookup_frec(unsigned short id, unsigned int crc)
{
  struct frec *f;

  for(f = frec_list; f; f = f->next)
    if (f->sentto && f->new_id == id && 
	(f->crc == crc || crc == 0xffffffff))
      return f;
      
  return NULL;
}

static struct frec *lookup_frec_by_sender(unsigned short id,
					  union mysockaddr *addr,
					  unsigned int crc)
{
  struct frec *f;
  
  for(f = frec_list; f; f = f->next)
    if (f->sentto &&
	f->orig_id == id && 
	f->crc == crc &&
	sockaddr_isequal(&f->source, addr))
      return f;
   
  return NULL;
}

/* A server record is going away, remove references to it */
void server_gone(struct daemon *daemon, struct server *server)
{
  struct frec *f;
  
  for (f = frec_list; f; f = f->next)
    if (f->sentto && f->sentto == server)
      f->sentto = NULL;
  
  if (daemon->last_server == server)
    daemon->last_server = NULL;

  if (daemon->srv_save == server)
    daemon->srv_save = NULL;
}

/* return unique random ids.
   For signed packets we can't change the ID without breaking the
   signing, so we keep the same one. In this case force is set, and this
   routine degenerates into killing any conflicting forward record. */
static unsigned short get_id(int force, unsigned short force_id, unsigned int crc)
{
  unsigned short ret = 0;
  
  if (force)
    {
      struct frec *f = lookup_frec(force_id, crc);
      if (f)
	f->sentto = NULL; /* free */
      ret = force_id;
    }
  else do 
    ret = rand16();
  while (lookup_frec(ret, crc));
  
  return ret;
}

#ifdef DNI_PARENTAL_CTL
/*********************************
 * transform the the mac address from the formation AA:BB:CC:DD:EE:FF
 * assigned by mac_p to the formation AABBCCDDEEFF assigned by mac
 ********************************/
static int trans_macaddr(char * mac, char *mac_p)
{
	int i = 0;
	#define MAC_LEN 17

	if ((NULL == mac) || (NULL == mac_p) || (MAC_LEN != strlen(mac_p)))
		return -1;

	while (1) {
		*mac++ = *mac_p++;
		*mac++ = *mac_p++;
		if (6 <= ++i)
			break;
		mac_p++;
	}
	*mac = '\0';
        return 0;
}

/***********************************
 * get the mac address that is corresponding the specified ip address
 * from the arp buffer file "/proc/net/arp"
 **********************************/
static int get_macaddr_by_ip(char *mac, char *ip)
{
        char buf[512];
        FILE *fp=NULL;
        char str_addr[20], str_mac[20];

	if ((NULL == mac) || (NULL == ip))
		return -1;

        if(!(fp=fopen("/proc/net/arp", "r")))
                return -1;

        while(fgets(buf, 512, fp))
        {
		memset(str_addr, 0x00, sizeof(str_addr));
		memset(str_mac, 0x00, sizeof(str_mac));
		sscanf(buf, "%s %*s %*s %s %*s %*s", str_addr, str_mac);
		if(!strcasecmp(ip, str_addr))
		{
			strcpy(mac, str_mac);
			break;
		}
	}
	if(fp)
		fclose(fp);
        return 0;
}

/************************************
 * get the deviceID that is corresponding of specified mac address
 * from the ParentalControl_table which record all pairs about
 * the mac addresses and deviceID setted by ParentalControl SOAP APIs.
 * if there is not the record of this mac address, then assign the
 * deviceID of the especial mac address "default", and if the "default"
 * is not existed too, then assign "0000111111111111".
 ***********************************/
static void get_deviceid_by_macaddr(char *macaddr, char *deviceid)
{
        char default_deviceid[33]={0};
        char macaddr_tmp[13]={0}, deviceid_tmp[33]={0};
        char *pos=NULL;
        char *delimiter = ",";
        int count=0;
	char pc_table[2000] = "0,";
	FILE *fp = NULL;

	if (NULL != (fp = fopen(pc_table_file, "r"))) {
		fgets(pc_table, sizeof(pc_table), fp);
		fclose(fp);
	}

        if(!strcmp("0,", pc_table))
        {
                strcpy(deviceid, "0000111111111111");
                return ;
        }
        pos = strtok(pc_table, delimiter); //read count value of record.
        count = atoi(pc_table);
        while(0 < count--)
        {
                pos = strtok(NULL, delimiter);
                sscanf(pos, "%s %s", macaddr_tmp, deviceid_tmp);
                if(!strcasecmp(macaddr_tmp, macaddr))
                {
                        strcpy(deviceid, deviceid_tmp);
                        break;
                }
                else if(!strcasecmp(macaddr_tmp, "default"))
                {
                        strcpy(default_deviceid, deviceid_tmp);
                }
        }

        if(strcmp(deviceid, ""))
                return ;
        if(strcmp(default_deviceid, ""))
        {
                strcpy(deviceid, default_deviceid);
                return;
        }
        strcpy(deviceid, "0000111111111111");

}

/*****************************************
 * if ParentalControl feature was enable, that means the value of
 * /tmp/configs/ParentalControl is 1, lookup the right deviceID and
 * add it to the end of DNS query packet with the relevant Options.
 * if ParentalControl feature was disable, just return and do nothing.
 ****************************************/
static void parse_parentalcontrol(HEADER *header, int *n, union mysockaddr ipaddr)
{
        unsigned char *p_header=(unsigned char *)header;
        unsigned char *pheader=(unsigned char *)header+*n, *p;
        int len_header=*n;
        short int rdlen=0;
        FILE *fp;
        unsigned char parentalcontrol='0';
        char deviceid[33]={0};
        char macaddr[20]={0}, macaddr_p[20]={0} ;
	char ip_addr[20]={0};
        const unsigned char clientid[11] = { PC_HTONS_CHARS(4), PC_HTONS_CHARS(15),
                                               'O', 'p', 'e', 'n', 'D', 'N', 'S' };
        const unsigned char fixed[11] = { 0, PC_HTONS_CHARS(41),
                                    PC_HTONS_CHARS(512),
                                    0, 0, 0, 0, PC_HTONS_CHARS(0) };
        unsigned char deviceid_b[8] = { 0x00, 0x00, 0x11, 0x11,
                                                0x11, 0x11, 0x11, 0x11};
	strcpy(ip_addr, inet_ntoa(ipaddr.in.sin_addr));
        if(!get_macaddr_by_ip(macaddr_p, ip_addr))
                if(!trans_macaddr(macaddr, macaddr_p))
                {
                        get_deviceid_by_macaddr(macaddr, deviceid);
                        trans_deviceid(deviceid, deviceid_b);
                }
        header->arcount = htons(1);

        memcpy(p_header+len_header, fixed, sizeof(fixed));
        len_header+=sizeof(fixed);
        memcpy(p_header+len_header, clientid, sizeof(clientid));
        len_header+=sizeof(clientid);
        memcpy(p_header+len_header, deviceid_b, sizeof(deviceid_b));
        len_header+=sizeof(deviceid_b);


        p = pheader + 9;
        memcpy(&rdlen, p, sizeof(rdlen));
        rdlen=ntohs(rdlen);
        rdlen+=sizeof(clientid) + sizeof(deviceid_b);
        rdlen=htons(rdlen);
        p = pheader + 9;
        memcpy(p, &rdlen, sizeof(rdlen));


        *n=len_header;
}

/***********************************
 * transform the string of deviceid to the hexadecimal formation.
 * for instance, transform the string "1234567890abcdef" to the
 * hexadecimal digit 0x12 0x34 0x56 0x78 0x90 0xAB 0xCD 0xEF, and
 * copy this hex digit to the memory assigned by deviceid_b, then
 * the string have changed to binay code.
 **********************************/
static int trans_deviceid(char *deviceid, char *deviceid_b)
{
        int i=0;

	if ((NULL == deviceid) || (NULL == deviceid_b))
		return -1;

        if(16 != strlen(deviceid))
                return -1;
        if(!strncmp(deviceid, "0000111111111111", 16))
                return 0;
        for(i=0; i<16; i++)
        {
		if (('0' <= *(deviceid + i)) && ('9' >= *(deviceid + i))) {
			*(deviceid+i) -= '0';
		} else if (('a' <= *(deviceid + i)) && ('f' >= *(deviceid + i))) {
			*(deviceid+i) -= ('a' - 0xA);
		} else if (('A' <= *(deviceid + i)) && ('F' >= *(deviceid + i))) {
			*(deviceid+i) -= ('A' - 0xA);
		} else {
			return -1;
		}
        }
        for(i=0; i < 8; i++)
                *(deviceid_b+i)=((*(deviceid+i*2+0))<<4)|(*(deviceid+i*2+1));
        return 0;
}
#endif

#ifdef DNI_IPV6_FEATURE
int transmit_name(char *name, unsigned char **ret)
{
  unsigned char *p = NULL, *q = NULL, *start = NULL;
  int n = 0;
  if (!ret)
    return -1;
  start = p = q = *ret;
  while (0 != *name)
    {
      p = q++;
      n = 0;
      while('.' != *name && 0 != *name)
        {
	  *q++ = *name++;
	  n++;
        }
      *p = (unsigned char)(n);
      name++;
    }
  *q = 0;
  *ret = q + 1;
  return ((*ret) - start);
}

#define DNI_DNS_QUERY_TIMEOUT 3
void check_timeout_forward(struct daemon *daemon, time_t now)
{
  struct frec *fwd = NULL;
  HEADER *header = NULL;
  int n = 0;
  unsigned char *p = NULL;

  for (fwd = frec_list; fwd; fwd = fwd->next)
    {
      if (fwd->sentto && 1 == fwd->fwd_sign && (int)difftime(now, fwd->time) >= DNI_DNS_QUERY_TIMEOUT)
        {
	  header = (HEADER *)daemon->packet;
	  p = (unsigned char *)header;
	  header->id = htons(fwd->new_id);
	  memcpy(p + 2, &fwd->flags, sizeof(fwd->flags));
	  header->qdcount = 1;
	  header->ancount = 0;
	  header->nscount = 0;
	  header->arcount = 0;
	  p += sizeof(HEADER);
	  n += sizeof(HEADER);
	  n += transmit_name(fwd->name, &p);
	  PUTSHORT(fwd->type, p);
	  n += sizeof(fwd->type);
	  PUTSHORT(fwd->class, p);
	  n += sizeof(fwd->class);
	  forward_query(daemon, fwd->fd, &fwd->source, &fwd->dest, fwd->iface,
		  header, (size_t)n, now, fwd);
        }
    }
}
#endif

