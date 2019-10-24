/* dnsmasq is Copyright (c) 2000-2018 Simon Kelley

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 dated June, 1991, or
   (at your option) version 3 dated 29 June, 2007.
 
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
     
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "dnsmasq.h"

static struct frec *lookup_frec(unsigned short id, void *hash);
static struct frec *lookup_frec_by_sender(unsigned short id,
					  union mysockaddr *addr,
					  void *hash);
static unsigned short get_id(void);
static void free_frec(struct frec *f);

#ifdef DNI_PARENTAL_CTL
static int trans_macaddr(char * mac, char *mac_p);
static int get_macaddr_by_ip(char *mac, char *ip);
static void get_deviceid_by_macaddr(char *macaddr, char *deviceid);
static void parse_parentalcontrol(struct dns_header *header, int *n, union mysockaddr ipaddr);
static int trans_deviceid(char *deviceid, char *deviceid_b);

#define PC_HTONS_CHARS(n) (unsigned char)((n) >> 8), (unsigned char)(n)
#endif

extern int extract_name(struct dns_header *header, size_t plen, unsigned char **pp, 
		 char *name, int isExtract, int extrabytes);

static struct server* get_first_server(struct daemon *daemon, unsigned short gotname, int type);
/* Send a UDP packet with its source address set as "source" 
   unless nowild is true, when we just send it with the kernel default */
int send_from(int fd, int nowild, char *packet, size_t len, 
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
	  struct in_pktinfo p;
	  p.ipi_ifindex = 0;
	  p.ipi_spec_dst = source->addr.addr4;
	  memcpy(CMSG_DATA(cmptr), &p, sizeof(p));
	  msg.msg_controllen = cmptr->cmsg_len = CMSG_LEN(sizeof(struct in_pktinfo));
	  cmptr->cmsg_level = IPPROTO_IP;
	  cmptr->cmsg_type = IP_PKTINFO;
#elif defined(IP_SENDSRCADDR)
	  memcpy(CMSG_DATA(cmptr), &(source->addr.addr4), sizeof(source->addr.addr4));
	  msg.msg_controllen = cmptr->cmsg_len = CMSG_LEN(sizeof(struct in_addr));
	  cmptr->cmsg_level = IPPROTO_IP;
	  cmptr->cmsg_type = IP_SENDSRCADDR;
#endif
	}
      else
#ifdef HAVE_IPV6
	{
	  struct in6_pktinfo p;
	  p.ipi6_ifindex = iface; /* Need iface for IPv6 to handle link-local addrs */
	  p.ipi6_addr = source->addr.addr6;
	  memcpy(CMSG_DATA(cmptr), &p, sizeof(p));
	  msg.msg_controllen = cmptr->cmsg_len = CMSG_LEN(sizeof(struct in6_pktinfo));
	  cmptr->cmsg_type = daemon->v6pktinfo;
	  cmptr->cmsg_level = IPPROTO_IPV6;
	}
#else
      (void)iface; /* eliminate warning */
#endif
    }
  
  while (retry_send(sendmsg(fd, &msg, 0)));

  /* If interface is still in DAD, EINVAL results - ignore that. */
  if (errno != 0 && errno != EINVAL)
    {
      my_syslog(LOG_ERR, _("failed to send packet: %s"), strerror(errno));
      return 0;
    }
  
  return 1;
}
          
static unsigned int search_servers(time_t now, struct all_addr **addrpp, unsigned int qtype,
				   char *qdomain, int *type, char **domain, int *norebind)
			      
{
  /* If the query ends in the domain in one of our servers, set
     domain to point to that name. We find the largest match to allow both
     domain.org and sub.domain.org to exist. */
  
  unsigned int namelen = strlen(qdomain);
  unsigned int matchlen = 0;
  struct server *serv;
  unsigned int flags = 0;
  static struct all_addr zero;
  
  for (serv = daemon->servers; serv; serv=serv->next)
    if (qtype == F_DNSSECOK && !(serv->flags & SERV_DO_DNSSEC))
      continue;
    /* domain matches take priority over NODOTS matches */
    else if ((serv->flags & SERV_FOR_NODOTS) && *type != SERV_HAS_DOMAIN && !strchr(qdomain, '.') && namelen != 0)
      {
	unsigned int sflag = serv->addr.sa.sa_family == AF_INET ? F_IPV4 : F_IPV6; 
	*type = SERV_FOR_NODOTS;
	if (serv->flags & SERV_NO_ADDR)
	  flags = F_NXDOMAIN;
	else if (serv->flags & SERV_LITERAL_ADDRESS)
	  { 
	    /* literal address = '#' -> return all-zero address for IPv4 and IPv6 */
	    if ((serv->flags & SERV_USE_RESOLV) && (qtype & (F_IPV6 | F_IPV4)))
	      {
		memset(&zero, 0, sizeof(zero));
		flags = qtype;
		*addrpp = &zero;
	      }
	    else if (sflag & qtype)
	      {
		flags = sflag;
		if (serv->addr.sa.sa_family == AF_INET) 
		  *addrpp = (struct all_addr *)&serv->addr.in.sin_addr;
#ifdef HAVE_IPV6
		else
		  *addrpp = (struct all_addr *)&serv->addr.in6.sin6_addr;
#endif 
	      }
	    else if (!flags || (flags & F_NXDOMAIN))
	      flags = F_NOERR;
	  } 
      }
    else if (serv->flags & SERV_HAS_DOMAIN)
      {
	unsigned int domainlen = strlen(serv->domain);
	char *matchstart = qdomain + namelen - domainlen;
	if (namelen >= domainlen &&
	    hostname_isequal(matchstart, serv->domain) &&
	    (domainlen == 0 || namelen == domainlen || *(matchstart-1) == '.' ))
	  {
	    if ((serv->flags & SERV_NO_REBIND) && norebind)	
	      *norebind = 1;
	    else
	      {
		unsigned int sflag = serv->addr.sa.sa_family == AF_INET ? F_IPV4 : F_IPV6;
		/* implement priority rules for --address and --server for same domain.
		   --address wins if the address is for the correct AF
		   --server wins otherwise. */
		if (domainlen != 0 && domainlen == matchlen)
		  {
		    if ((serv->flags & SERV_LITERAL_ADDRESS))
		      {
			if (!(sflag & qtype) && flags == 0)
			  continue;
		      }
		    else
		      {
			if (flags & (F_IPV4 | F_IPV6))
			  continue;
		      }
		  }
		
		if (domainlen >= matchlen)
		  {
		    *type = serv->flags & (SERV_HAS_DOMAIN | SERV_USE_RESOLV | SERV_NO_REBIND | SERV_DO_DNSSEC);
		    *domain = serv->domain;
		    matchlen = domainlen;
		    if (serv->flags & SERV_NO_ADDR)
		      flags = F_NXDOMAIN;
		    else if (serv->flags & SERV_LITERAL_ADDRESS)
		      {
			 /* literal address = '#' -> return all-zero address for IPv4 and IPv6 */
			if ((serv->flags & SERV_USE_RESOLV) && (qtype & (F_IPV6 | F_IPV4)))
			  {			    
			    memset(&zero, 0, sizeof(zero));
			    flags = qtype;
			    *addrpp = &zero;
			  }
			else if (sflag & qtype)
			  {
			    flags = sflag;
			    if (serv->addr.sa.sa_family == AF_INET) 
			      *addrpp = (struct all_addr *)&serv->addr.in.sin_addr;
#ifdef HAVE_IPV6
			    else
			      *addrpp = (struct all_addr *)&serv->addr.in6.sin6_addr;
#endif
			  }
			else if (!flags || (flags & F_NXDOMAIN))
			  flags = F_NOERR;
		      }
		    else
		      flags = 0;
		  } 
	      }
	  }
      }
  
  if (flags == 0 && !(qtype & (F_QUERY | F_DNSSECOK)) && 
      option_bool(OPT_NODOTS_LOCAL) && !strchr(qdomain, '.') && namelen != 0)
    /* don't forward A or AAAA queries for simple names, except the empty name */
    flags = F_NOERR;
  
  if (flags == F_NXDOMAIN && check_for_local_domain(qdomain, now))
    flags = F_NOERR;

  if (flags)
    {
       if (flags == F_NXDOMAIN || flags == F_NOERR)
	 log_query(flags | qtype | F_NEG | F_CONFIG | F_FORWARD, qdomain, NULL, NULL);
       else
	 {
	   /* handle F_IPV4 and F_IPV6 set on ANY query to 0.0.0.0/:: domain. */
	   if (flags & F_IPV4)
	     log_query((flags | F_CONFIG | F_FORWARD) & ~F_IPV6, qdomain, *addrpp, NULL);
#ifdef HAVE_IPV6
	   if (flags & F_IPV6)
	     log_query((flags | F_CONFIG | F_FORWARD) & ~F_IPV4, qdomain, *addrpp, NULL);
#endif
	 }
    }
  else if ((*type) & SERV_USE_RESOLV)
    {
      *type = 0; /* use normal servers for this domain */
      *domain = NULL;
    }
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

#ifdef SUP_MUL_PPPOE
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
			if(!strncmp(inet_ntoa(srv), buf, strlen(buf)-1)){
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
			if(!strncmp(inet_ntoa(srv), buf, strlen(buf)-1)){
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
		/* ppp1 domain skip ppp0 dns */
		if(ppp0_dns == 1) {
		   	my_syslog(LOG_INFO, "return 1");
			return 1;
		}
	}
	return 0;
}
#endif											+

static int forward_query(int udpfd, union mysockaddr *udpaddr,
			 struct all_addr *dst_addr, unsigned int dst_iface,
			 struct dns_header *header, size_t plen, time_t now, 
			 struct frec *forward, int ad_reqd, int do_bit)
{
  char *domain = NULL;
  int type = SERV_DO_DNSSEC, norebind = 0;
  struct all_addr *addrp = NULL;
  unsigned int flags = 0;
  struct server *start = NULL;
#ifdef HAVE_DNSSEC
  void *hash = hash_questions(header, plen, daemon->namebuff);
  int do_dnssec = 0;
#else
  unsigned int crc = questions_crc(header, plen, daemon->namebuff);
  void *hash = &crc;
#endif
  unsigned int gotname = extract_request(header, plen, daemon->namebuff, NULL);
  unsigned char *oph = find_pseudoheader(header, plen, NULL, NULL, NULL, NULL);
  (void)do_bit;

#ifdef BIND_SRVSOCK_TO_WAN
 struct ifreq ifr;
#endif

  /* may be no servers available. */
  if (forward || (hash && (forward = lookup_frec_by_sender(ntohs(header->id), udpaddr, hash))))
    {
      /* If we didn't get an answer advertising a maximal packet in EDNS,
	 fall back to 1280, which should work everywhere on IPv6.
	 If that generates an answer, it will become the new default
	 for this server */
      forward->flags |= FREC_TEST_PKTSZ;
      
#ifdef HAVE_DNSSEC
      /* If we've already got an answer to this query, but we're awaiting keys for validation,
	 there's no point retrying the query, retry the key query instead...... */
      if (forward->blocking_query)
	{
	  int fd, is_sign;
	  unsigned char *pheader;
	  
	  forward->flags &= ~FREC_TEST_PKTSZ;
	  
	  while (forward->blocking_query)
	    forward = forward->blocking_query;
	   
	  blockdata_retrieve(forward->stash, forward->stash_len, (void *)header);
	  plen = forward->stash_len;
	  
	  forward->flags |= FREC_TEST_PKTSZ;
	  if (find_pseudoheader(header, plen, NULL, &pheader, &is_sign, NULL) && !is_sign)
	    PUTSHORT(SAFE_PKTSZ, pheader);
	  
	  if (forward->sentto->addr.sa.sa_family == AF_INET) 
	    log_query(F_NOEXTRA | F_DNSSEC | F_IPV4, "retry", (struct all_addr *)&forward->sentto->addr.in.sin_addr, "dnssec");
#ifdef HAVE_IPV6
	  else
	    log_query(F_NOEXTRA | F_DNSSEC | F_IPV6, "retry", (struct all_addr *)&forward->sentto->addr.in6.sin6_addr, "dnssec");
#endif
  
	  if (forward->sentto->sfd)
	    fd = forward->sentto->sfd->fd;
	  else
	    {
#ifdef HAVE_IPV6
	      if (forward->sentto->addr.sa.sa_family == AF_INET6)
		fd = forward->rfd6->fd;
	      else
#endif
		fd = forward->rfd4->fd;
	    }
	  
	  while (retry_send(sendto(fd, (char *)header, plen, 0,
				   &forward->sentto->addr.sa,
				   sa_len(&forward->sentto->addr))));
	  
	  return 1;
	}
#endif

      /* retry on existing query, send to all available servers  */
      domain = forward->sentto->domain;
      forward->sentto->failed_queries++;
      if (!option_bool(OPT_ORDER))
	{
	  forward->forwardall = 1;
#ifdef DNI_IPV6_FEATURE
	  forward->time = now;
	  if (2 == forward->fwd_sign)
		  forward->fwd_sign = 0;
#else
	  unsigned char *p = (unsigned char *)(header+1);
	  if (F_IPV4 == gotname || F_IPV6 == gotname)
	  {
		  memset(forward->name, 0x00, sizeof(forward->name));
		  extract_name(header, plen, &p, forward->name, 1, 4);
	  }
#endif
	  daemon->last_server = NULL;
	}
      type = forward->sentto->flags & SERV_TYPE;
#ifdef HAVE_DNSSEC
      do_dnssec = forward->sentto->flags & SERV_DO_DNSSEC;
#endif

      if (!(start = forward->sentto->next))
	start = daemon->servers; /* at end of list, recycle */
      header->id = htons(forward->new_id);
    }
  else 
    {
      if (gotname)
	flags = search_servers(now, &addrp, gotname, daemon->namebuff, &type, &domain, &norebind);
      
#ifdef HAVE_DNSSEC
      do_dnssec = type & SERV_DO_DNSSEC;
#endif
      type &= ~SERV_DO_DNSSEC;      

      if (daemon->servers && !flags)
	forward = get_new_frec(now, NULL, 0);
      /* table full - flags == 0, return REFUSED */
      
      if (forward)
	{
	  forward->source = *udpaddr;
	  forward->dest = *dst_addr;
	  forward->iface = dst_iface;
	  forward->orig_id = ntohs(header->id);
	  forward->new_id = get_id();
	  forward->fd = udpfd;
	  memcpy(forward->hash, hash, HASH_SIZE);
	  forward->forwardall = 0;
#ifdef DNI_IPV6_FEATURE
	  unsigned char *p = (unsigned char *)(header+1);
	  if (F_IPV4 == gotname || F_IPV6 == gotname)
	  {
		  memcpy(&forward->flags, (unsigned char *)header + 2, sizeof(forward->flags));
		  memset(forward->name, 0x00, sizeof(forward->name));
		  extract_name(header, plen, &p, forward->name, 1, 4);
		  GETSHORT(forward->type, p);
		  GETSHORT(forward->class, p);
	  }
	  forward->fwd_sign = 0;
#endif
	  forward->flags = 0;
	  if (norebind)
	    forward->flags |= FREC_NOREBIND;
	  if (header->hb4 & HB4_CD)
	    forward->flags |= FREC_CHECKING_DISABLED;
	  if (ad_reqd)
	    forward->flags |= FREC_AD_QUESTION;
#ifdef HAVE_DNSSEC
	  forward->work_counter = DNSSEC_WORK;
	  if (do_bit)
	    forward->flags |= FREC_DO_QUESTION;
#endif
	  
	  header->id = htons(forward->new_id);
	 
#if 1
	  start = get_first_server(daemon, gotname, type);
#else

	  /* In strict_order mode, always try servers in the order 
	     specified in resolv.conf, if a domain is given 
	     always try all the available servers,
	     otherwise, use the one last known to work. */
	  
	  if (type == 0)
	    {
	      if (option_bool(OPT_ORDER))
		start = daemon->servers;
	      else if (!(start = daemon->last_server) ||
		       daemon->forwardcount++ > FORWARD_TEST ||
		       difftime(now, daemon->forwardtime) > FORWARD_TIME)
		{
		  start = daemon->servers;
		  forward->forwardall = 1;
		  daemon->forwardcount = 0;
		  daemon->forwardtime = now;
		}
	    }
	  else
	    {
	      start = daemon->servers;
	      if (!option_bool(OPT_ORDER))
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
      int subnet, forwarded = 0;
      size_t edns0_len;
      unsigned char *pheader;
      
      /* If a query is retried, use the log_id for the retry when logging the answer. */
      forward->log_id = daemon->log_id;
      
      plen = add_edns0_config(header, plen, ((unsigned char *)header) + PACKETSZ, &forward->source, now, &subnet);
      
      if (subnet)
	forward->flags |= FREC_HAS_SUBNET;
      
#ifdef HAVE_DNSSEC
      if (option_bool(OPT_DNSSEC_VALID) && do_dnssec)
	{
	  plen = add_do_bit(header, plen, ((unsigned char *) header) + PACKETSZ);
	 	      
	  /* For debugging, set Checking Disabled, otherwise, have the upstream check too,
	     this allows it to select auth servers when one is returning bad data. */
	  if (option_bool(OPT_DNSSEC_DEBUG))
	    header->hb4 |= HB4_CD;

	}
#endif

      if (find_pseudoheader(header, plen, &edns0_len, &pheader, NULL, NULL))
	{
	  /* If there wasn't a PH before, and there is now, we added it. */
	  if (!oph)
	    forward->flags |= FREC_ADDED_PHEADER;

	  /* If we're sending an EDNS0 with any options, we can't recreate the query from a reply. */
	  if (edns0_len > 11)
	    forward->flags |= FREC_HAS_EXTRADATA;

	  /* Reduce udp size on retransmits. */
	  if (forward->flags & FREC_TEST_PKTSZ)
	    PUTSHORT(SAFE_PKTSZ, pheader);
	}
      
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
	      !(start->flags & (SERV_LITERAL_ADDRESS | SERV_LOOP)))
	    {
	      int fd;

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
 
	      /* find server socket to use, may need to get random one. */
	      if (start->sfd)
		fd = start->sfd->fd;
	      else 
		{
#ifdef HAVE_IPV6
		  if (start->addr.sa.sa_family == AF_INET6)
		    {
		      if (!forward->rfd6 &&
			  !(forward->rfd6 = allocate_rfd(AF_INET6)))
			break;
		      daemon->rfd_save = forward->rfd6;
		      fd = forward->rfd6->fd;
		    }
		  else
#endif
		    {
		      if (!forward->rfd4 &&
			  !(forward->rfd4 = allocate_rfd(AF_INET)))
			break;
		      daemon->rfd_save = forward->rfd4;
		      fd = forward->rfd4->fd;
		    }

#ifdef HAVE_CONNTRACK
		  /* Copy connection mark of incoming query to outgoing connection. */
		  if (option_bool(OPT_CONNTRACK))
		    {
		      unsigned int mark;
		      if (get_incoming_mark(&forward->source, &forward->dest, 0, &mark))
			setsockopt(fd, SOL_SOCKET, SO_MARK, &mark, sizeof(unsigned int));
		    }
#endif
		}
	      
#ifdef HAVE_DNSSEC
	      if (option_bool(OPT_DNSSEC_VALID) && (forward->flags & FREC_ADDED_PHEADER))
		{
		  /* Difficult one here. If our client didn't send EDNS0, we will have set the UDP
		     packet size to 512. But that won't provide space for the RRSIGS in many cases.
		     The RRSIGS will be stripped out before the answer goes back, so the packet should
		     shrink again. So, if we added a do-bit, bump the udp packet size to the value
		     known to be OK for this server. We check returned size after stripping and set
		     the truncated bit if it's still too big. */		  
		  unsigned char *pheader;
		  int is_sign;
		  if (find_pseudoheader(header, plen, NULL, &pheader, &is_sign, NULL) && !is_sign)
		    PUTSHORT(start->edns_pktsz, pheader);
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

#ifdef SUP_MUL_PPPOE
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
#endif

		  int sendto_ret = 0;
		  
		  sendto_ret = sendto(fd, (char *)header, plen, 0, &start->addr.sa, sa_len(&start->addr));
	      
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

		  if (retry_send(sendto_ret))
		continue;
	    
	      if (errno == 0)
		{
#ifdef HAVE_DUMPFILE
		  dump_packet(DUMP_UP_QUERY, (void *)header, plen, NULL, &start->addr);
#endif
		  
		  /* Keep info in case we want to re-send this packet */
		  daemon->srv_save = start;
		  daemon->packet_len = plen;
		  
		  if (!gotname)
		    strcpy(daemon->namebuff, "query");
		  if (start->addr.sa.sa_family == AF_INET)
		    log_query(F_SERVER | F_IPV4 | F_FORWARD, daemon->namebuff, 
			      (struct all_addr *)&start->addr.in.sin_addr, NULL); 
#ifdef HAVE_IPV6
		  else
		    log_query(F_SERVER | F_IPV6 | F_FORWARD, daemon->namebuff, 
			      (struct all_addr *)&start->addr.in6.sin6_addr, NULL);
#endif 
		  start->queries++;
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
      if (forwarded || 1 == forward->fwd_sign)
	return 1;
#else
      if (forwarded)
	      return 1;
#endif
      /* could not send on, prepare to return */ 
      header->id = htons(forward->orig_id);
      free_frec(forward); /* cancel */
    }	  
  
  /* could not send on, return empty answer or address if known for whole domain */
  if (udpfd != -1)
    {
      plen = setup_reply(header, plen, addrp, flags, daemon->local_ttl);
      if (oph)
	plen = add_pseudoheader(header, plen, ((unsigned char *) header) + PACKETSZ, daemon->edns_pktsz, 0, NULL, 0, do_bit, 0);
      send_from(udpfd, option_bool(OPT_NOWILD) || option_bool(OPT_CLEVERBIND), (char *)header, plen, udpaddr, dst_addr, dst_iface);
	}

  return 0;
}

static size_t process_reply(struct dns_header *header, time_t now, struct server *server, size_t n, int check_rebind, 
			    int no_cache, int cache_secure, int bogusanswer, int ad_reqd, int do_bit, int added_pheader, 
			    int check_subnet, union mysockaddr *query_source)
{
  unsigned char *pheader, *sizep;
  char **sets = 0;
  int munged = 0, is_sign;
  unsigned int rcode = RCODE(header);
  size_t plen; 
  
  (void)ad_reqd;
  (void)do_bit;
  (void)bogusanswer;

#ifdef HAVE_IPSET
  if (daemon->ipsets && extract_request(header, n, daemon->namebuff, NULL))
    {
      /* Similar algorithm to search_servers. */
      struct ipsets *ipset_pos;
      unsigned int namelen = strlen(daemon->namebuff);
      unsigned int matchlen = 0;
      for (ipset_pos = daemon->ipsets; ipset_pos; ipset_pos = ipset_pos->next) 
	{
	  unsigned int domainlen = strlen(ipset_pos->domain);
	  char *matchstart = daemon->namebuff + namelen - domainlen;
	  if (namelen >= domainlen && hostname_isequal(matchstart, ipset_pos->domain) &&
	      (domainlen == 0 || namelen == domainlen || *(matchstart - 1) == '.' ) &&
	      domainlen >= matchlen) 
	    {
	      matchlen = domainlen;
	      sets = ipset_pos->sets;
	    }
	}
    }
#endif
  
  if ((pheader = find_pseudoheader(header, n, &plen, &sizep, &is_sign, NULL)))
    {
      /* Get extended RCODE. */
      rcode |= sizep[2] << 4;

      if (check_subnet && !check_source(header, plen, pheader, query_source))
	{
	  my_syslog(LOG_WARNING, _("discarding DNS reply: subnet option mismatch"));
	  return 0;
	}
      
      if (!is_sign)
	{
	  if (added_pheader)
	    {
	      /* client didn't send EDNS0, we added one, strip it off before returning answer. */
	      n = rrfilter(header, n, 0);
	      pheader = NULL;
	    }
	  else
	    {
	      unsigned short udpsz;

	      /* If upstream is advertising a larger UDP packet size
		 than we allow, trim it so that we don't get overlarge
		 requests for the client. We can't do this for signed packets. */
	      GETSHORT(udpsz, sizep);
	      if (udpsz > daemon->edns_pktsz)
		{
		  sizep -= 2;
		  PUTSHORT(daemon->edns_pktsz, sizep);
		}

#ifdef HAVE_DNSSEC
	      /* If the client didn't set the do bit, but we did, reset it. */
	      if (option_bool(OPT_DNSSEC_VALID) && !do_bit)
		{
		  unsigned short flags;
		  sizep += 2; /* skip RCODE */
		  GETSHORT(flags, sizep);
		  flags &= ~0x8000;
		  sizep -= 2;
		  PUTSHORT(flags, sizep);
		}
#endif
	    }
	}
    }
  
  /* RFC 4035 sect 4.6 para 3 */
  if (!is_sign && !option_bool(OPT_DNSSEC_PROXY))
     header->hb4 &= ~HB4_AD;
  
  if (OPCODE(header) != QUERY)
    return resize_packet(header, n, pheader, plen);

  if (rcode != NOERROR && rcode != NXDOMAIN)
    {
      struct all_addr a;
      a.addr.rcode.rcode = rcode;
      log_query(F_UPSTREAM | F_RCODE, "error", &a, NULL);
      
      return resize_packet(header, n, pheader, plen);
    }
  
  /* Complain loudly if the upstream server is non-recursive. */
  if (!(header->hb4 & HB4_RA) && rcode == NOERROR &&
      server && !(server->flags & SERV_WARNED_RECURSIVE))
    {
      prettyprint_addr(&server->addr, daemon->namebuff);
      my_syslog(LOG_WARNING, _("nameserver %s refused to do a recursive query"), daemon->namebuff);
      if (!option_bool(OPT_LOG))
	server->flags |= SERV_WARNED_RECURSIVE;
    }  

  if (daemon->bogus_addr && rcode != NXDOMAIN &&
      check_for_bogus_wildcard(header, n, daemon->namebuff, daemon->bogus_addr, now))
    {
      munged = 1;
      SET_RCODE(header, NXDOMAIN);
      header->hb3 &= ~HB3_AA;
      cache_secure = 0;
    }
  else 
    {
      int doctored = 0;
      
      if (rcode == NXDOMAIN && 
	  extract_request(header, n, daemon->namebuff, NULL) &&
	  check_for_local_domain(daemon->namebuff, now))
	{
	  /* if we forwarded a query for a locally known name (because it was for 
	     an unknown type) and the answer is NXDOMAIN, convert that to NODATA,
	     since we know that the domain exists, even if upstream doesn't */
	  munged = 1;
	  header->hb3 |= HB3_AA;
	  SET_RCODE(header, NOERROR);
	  cache_secure = 0;
	}
      
      if (extract_addresses(header, n, daemon->namebuff, now, sets, is_sign, check_rebind, no_cache, cache_secure, &doctored))
	{
	  my_syslog(LOG_WARNING, _("possible DNS-rebind attack detected: %s"), daemon->namebuff);
	  munged = 1;
	  cache_secure = 0;
	}

      if (doctored)
	cache_secure = 0;
    }
  
#ifdef HAVE_DNSSEC
  if (bogusanswer && !(header->hb4 & HB4_CD) && !option_bool(OPT_DNSSEC_DEBUG))
    {
      /* Bogus reply, turn into SERVFAIL */
      SET_RCODE(header, SERVFAIL);
      munged = 1;
    }

  if (option_bool(OPT_DNSSEC_VALID))
    {
      header->hb4 &= ~HB4_AD;
      
      if (!(header->hb4 & HB4_CD) && ad_reqd && cache_secure)
	header->hb4 |= HB4_AD;
      
      /* If the requestor didn't set the DO bit, don't return DNSSEC info. */
      if (!do_bit)
	n = rrfilter(header, n, 1);
    }
#endif

  /* do this after extract_addresses. Ensure NODATA reply and remove
     nameserver info. */
  
  if (munged)
    {
      header->ancount = htons(0);
      header->nscount = htons(0);
      header->arcount = htons(0);
      header->hb3 &= ~HB3_TC;
    }
  
  /* the bogus-nxdomain stuff, doctor and NXDOMAIN->NODATA munging can all elide
     sections of the packet. Find the new length here and put back pseudoheader
     if it was removed. */
  return resize_packet(header, n, pheader, plen);
}

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

/* sets new last_server */
void reply_query(int fd, int family, time_t now)
{
  /* packet from peer server, extract data for cache, and send to
     original requester */
  struct dns_header *header;
  union mysockaddr serveraddr;
  struct frec *forward;
  socklen_t addrlen = sizeof(serveraddr);
  ssize_t n = recvfrom(fd, daemon->packet, daemon->packet_buff_sz, 0, &serveraddr.sa, &addrlen);
  size_t nn;
  struct server *server;
  void *hash;
#ifndef HAVE_DNSSEC
  unsigned int crc;
#endif

  /* packet buffer overwritten */
  daemon->srv_save = NULL;
  
  /* Determine the address of the server replying  so that we can mark that as good */
  serveraddr.sa.sa_family = family;
#ifdef HAVE_IPV6
  if (serveraddr.sa.sa_family == AF_INET6)
    serveraddr.in6.sin6_flowinfo = 0;
#endif
  
  header = (struct dns_header *)daemon->packet;

  if (n < (int)sizeof(struct dns_header) || !(header->hb3 & HB3_QR))
    return;
 
  /* spoof check: answer must come from known server, */
  for (server = daemon->servers; server; server = server->next)
    if (!(server->flags & (SERV_LITERAL_ADDRESS | SERV_NO_ADDR)) &&
	sockaddr_isequal(&server->addr, &serveraddr))
      break;
  
  if (!server)
    return;

  /* If sufficient time has elapsed, try and expand UDP buffer size again. */
  if (difftime(now, server->pktsz_reduced) > UDP_TEST_TIME)
    server->edns_pktsz = daemon->edns_pktsz;

#ifdef HAVE_DNSSEC
  hash = hash_questions(header, n, daemon->namebuff);
#else
  hash = &crc;
  crc = questions_crc(header, n, daemon->namebuff);
#endif
  
  if (!(forward = lookup_frec(ntohs(header->id), hash)))
    return;

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
	       forward_query(forward->fd, &forward->source,
                 &forward->dest, forward->iface, header, nn, now, forward, forward->flags & FREC_AD_QUESTION, forward->flags & FREC_DO_QUESTION);
	       return;
	    }
        }
#endif
  
  /* If try-all-ns are set, treat NXDOMAIN as a failed request */
  HEADER *header_t = header;
  if ((daemon->options & OPT_TRY_ALL_NS) && header_t->rcode == NXDOMAIN)
  {
	  char strserverip[ADDRSTRLEN];
	  inet_ntop(serveraddr.sa.sa_family, serveraddr.sa.sa_family == AF_INET ? &serveraddr.in.sin_addr : &serveraddr.in6.sin6_addr, strserverip, ADDRSTRLEN);
	  my_syslog(LOG_INFO, "get reply with response code NXDOMAIN from %s for domain --, treat as SERVFAIL\n", strserverip);
	  header_t->rcode = SERVFAIL;
  }

#ifdef HAVE_DUMPFILE
  dump_packet((forward->flags & (FREC_DNSKEY_QUERY | FREC_DS_QUERY)) ? DUMP_SEC_REPLY : DUMP_UP_REPLY,
	      (void *)header, n, &serveraddr, NULL);
#endif

  /* log_query gets called indirectly all over the place, so 
     pass these in global variables - sorry. */
  daemon->log_display_id = forward->log_id;
  daemon->log_source_addr = &forward->source;
  
  if (daemon->ignore_addr && RCODE(header) == NOERROR &&
      check_for_ignored_address(header, n, daemon->ignore_addr))
    return;

  /* Note: if we send extra options in the EDNS0 header, we can't recreate
     the query from the reply. */
  if ((RCODE(header) == REFUSED || RCODE(header) == SERVFAIL) &&
      forward->forwardall == 0 &&
      !(forward->flags & FREC_HAS_EXTRADATA))
    /* for broken servers, attempt to send to another one. */
    {
      unsigned char *pheader;
      size_t plen;
      int is_sign;

#ifdef HAVE_DNSSEC
      /* For DNSSEC originated queries, just retry the query to the same server. */
      if (forward->flags & (FREC_DNSKEY_QUERY | FREC_DS_QUERY))
	{
	  struct server *start;
	  
	  blockdata_retrieve(forward->stash, forward->stash_len, (void *)header);
	  plen = forward->stash_len;

	  forward->forwardall = 2; /* only retry once */
	  start = forward->sentto;

	  /* for non-domain specific servers, see if we can find another to try. */
	  if ((forward->sentto->flags & SERV_TYPE) == 0)
	    while (1)
	      {
		if (!(start = start->next))
		  start = daemon->servers;
		if (start == forward->sentto)
		  break;
		
		if ((start->flags & SERV_TYPE) == 0 &&
		    (start->flags & SERV_DO_DNSSEC))
		  break;
	      }
	    
	  
	  if (start->sfd)
	    fd = start->sfd->fd;
	  else
	    {
#ifdef HAVE_IPV6
	      if (start->addr.sa.sa_family == AF_INET6)
		{
		  /* may have changed family */
		  if (!forward->rfd6)
		    forward->rfd6 = allocate_rfd(AF_INET6);
		  fd = forward->rfd6->fd;
		}
	      else
#endif
		{
		  /* may have changed family */
		  if (!forward->rfd4)
		    forward->rfd4 = allocate_rfd(AF_INET);
		  fd = forward->rfd4->fd;
		}
	    }
	
	  while (retry_send(sendto(fd, (char *)header, plen, 0,
				   &start->addr.sa,
				   sa_len(&start->addr))));
	  
	  if (start->addr.sa.sa_family == AF_INET) 
	    log_query(F_NOEXTRA | F_DNSSEC | F_IPV4, "retry", (struct all_addr *)&start->addr.in.sin_addr, "dnssec");
#ifdef HAVE_IPV6
	  else
	    log_query(F_NOEXTRA | F_DNSSEC | F_IPV6, "retry", (struct all_addr *)&start->addr.in6.sin6_addr, "dnssec");
#endif
	  
	  return;
	}
#endif
      
      /* In strict order mode, there must be a server later in the chain
	 left to send to, otherwise without the forwardall mechanism,
	 code further on will cycle around the list forwever if they
	 all return REFUSED. Note that server is always non-NULL before 
	 this executes. */
      if (option_bool(OPT_ORDER))
	for (server = forward->sentto->next; server; server = server->next)
	  if (!(server->flags & (SERV_LITERAL_ADDRESS | SERV_HAS_DOMAIN | SERV_FOR_NODOTS | SERV_NO_ADDR | SERV_LOOP)))
	    break;

      /* recreate query from reply */
      pheader = find_pseudoheader(header, (size_t)n, &plen, NULL, &is_sign, NULL);
      if (!is_sign && server)
	{
	  header->ancount = htons(0);
	  header->nscount = htons(0);
	  header->arcount = htons(0);
	  if ((nn = resize_packet(header, (size_t)n, pheader, plen)))
	    {
	      header->hb3 &= ~(HB3_QR | HB3_AA | HB3_TC);
	      header->hb4 &= ~(HB4_RA | HB4_RCODE | HB4_CD | HB4_AD);
	      if (forward->flags & FREC_CHECKING_DISABLED)
		header->hb4 |= HB4_CD;
	      if (forward->flags & FREC_AD_QUESTION)
		header->hb4 |= HB4_AD;
	      if (forward->flags & FREC_DO_QUESTION)
		add_do_bit(header, nn,  (unsigned char *)pheader + plen);
	      forward_query(-1, NULL, NULL, 0, header, nn, now, forward, forward->flags & FREC_AD_QUESTION, forward->flags & FREC_DO_QUESTION);
	      return;
	    }
	}
    }   
   
  server = forward->sentto;
  if ((forward->sentto->flags & SERV_TYPE) == 0)
    {
      if (RCODE(header) == REFUSED)
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
      if (!option_bool(OPT_ALL_SERVERS))
	daemon->last_server = server;
    }
 
  /* We tried resending to this server with a smaller maximum size and got an answer.
     Make that permanent. To avoid reduxing the packet size for a single dropped packet,
     only do this when we get a truncated answer, or one larger than the safe size. */
  if (server && server->edns_pktsz > SAFE_PKTSZ && (forward->flags & FREC_TEST_PKTSZ) && 
      ((header->hb3 & HB3_TC) || n >= SAFE_PKTSZ))
    {
      server->edns_pktsz = SAFE_PKTSZ;
      server->pktsz_reduced = now;
      prettyprint_addr(&server->addr, daemon->addrbuff);
      my_syslog(LOG_WARNING, _("reducing DNS packet size for nameserver %s to %d"), daemon->addrbuff, SAFE_PKTSZ);
    }

    
  /* If the answer is an error, keep the forward record in place in case
     we get a good reply from another server. Kill it when we've
     had replies from all to avoid filling the forwarding table when
     everything is broken */
  if (forward->forwardall == 0 || --forward->forwardall == 1 ||
      (RCODE(header) != REFUSED && RCODE(header) != SERVFAIL))
    {
      int check_rebind = 0, no_cache_dnssec = 0, cache_secure = 0, bogusanswer = 0;
      
      if (option_bool(OPT_NO_REBIND))
	check_rebind = !(forward->flags & FREC_NOREBIND);
      
      /*   Don't cache replies where DNSSEC validation was turned off, either
	   the upstream server told us so, or the original query specified it.  */
      if ((header->hb4 & HB4_CD) || (forward->flags & FREC_CHECKING_DISABLED))
	no_cache_dnssec = 1;
      
#ifdef HAVE_DNSSEC
      if (server && (server->flags & SERV_DO_DNSSEC) && 
	  option_bool(OPT_DNSSEC_VALID) && !(forward->flags & FREC_CHECKING_DISABLED))
	{
	  int status = 0;

	  /* We've had a reply already, which we're validating. Ignore this duplicate */
	  if (forward->blocking_query)
	    return;
	  
	   /* Truncated answer can't be validated.
	      If this is an answer to a DNSSEC-generated query, we still
	      need to get the client to retry over TCP, so return
	      an answer with the TC bit set, even if the actual answer fits.
	   */
	  if (header->hb3 & HB3_TC)
	    status = STAT_TRUNCATED;
	  
	  while (1)
	    {
	      /* As soon as anything returns BOGUS, we stop and unwind, to do otherwise
		 would invite infinite loops, since the answers to DNSKEY and DS queries
		 will not be cached, so they'll be repeated. */
	      if (status != STAT_BOGUS && status != STAT_TRUNCATED && status != STAT_ABANDONED)
		{
		  if (forward->flags & FREC_DNSKEY_QUERY)
		    status = dnssec_validate_by_ds(now, header, n, daemon->namebuff, daemon->keyname, forward->class);
		  else if (forward->flags & FREC_DS_QUERY)
		    status = dnssec_validate_ds(now, header, n, daemon->namebuff, daemon->keyname, forward->class);
		  else
		    status = dnssec_validate_reply(now, header, n, daemon->namebuff, daemon->keyname, &forward->class, 
						   !option_bool(OPT_DNSSEC_IGN_NS) && (server->flags & SERV_DO_DNSSEC),
						   NULL, NULL);
#ifdef HAVE_DUMPFILE
		  if (status == STAT_BOGUS)
		    dump_packet((forward->flags & (FREC_DNSKEY_QUERY | FREC_DS_QUERY)) ? DUMP_SEC_BOGUS : DUMP_BOGUS,
				header, (size_t)n, &serveraddr, NULL);
#endif
		}
	      
	      /* Can't validate, as we're missing key data. Put this
		 answer aside, whilst we get that. */     
	      if (status == STAT_NEED_DS || status == STAT_NEED_KEY)
		{
		  struct frec *new, *orig;
		  
		  /* Free any saved query */
		  if (forward->stash)
		    blockdata_free(forward->stash);
		  
		  /* Now save reply pending receipt of key data */
		  if (!(forward->stash = blockdata_alloc((char *)header, n)))
		    return;
		  forward->stash_len = n;
		  
		  /* Find the original query that started it all.... */
		  for (orig = forward; orig->dependent; orig = orig->dependent);
		  
		  if (--orig->work_counter == 0 || !(new = get_new_frec(now, NULL, 1)))
		    status = STAT_ABANDONED;
		  else
		    {
		      int querytype, fd, type = SERV_DO_DNSSEC;
		      struct frec *next = new->next;
		      char *domain;
		      
		      *new = *forward; /* copy everything, then overwrite */
		      new->next = next;
		      new->blocking_query = NULL;

		      /* Find server to forward to. This will normally be the 
			 same as for the original query, but may be another if
			 servers for domains are involved. */		      
		      if (search_servers(now, NULL, F_DNSSECOK, daemon->keyname, &type, &domain, NULL) == 0)
			{
			  struct server *start = server, *new_server = NULL;
			  
			  while (1)
			    {
			      if (type == (start->flags & (SERV_TYPE | SERV_DO_DNSSEC)) &&
				  ((type & SERV_TYPE) != SERV_HAS_DOMAIN || hostname_isequal(domain, start->domain)) &&
				  !(start->flags & (SERV_LITERAL_ADDRESS | SERV_LOOP)))
				{
				  new_server = start;
				  if (server == start)
				    {
				      new_server = NULL;
				      break;
				    }
				}
			      
			      if (!(start = start->next))
				start = daemon->servers;
			      if (start == server)
				break;
			    }
			  
			  if (new_server)
			    server = new_server;
			}
		      
		      new->sentto = server;
		      new->rfd4 = NULL;
#ifdef HAVE_IPV6
		      new->rfd6 = NULL;
#endif
		      new->flags &= ~(FREC_DNSKEY_QUERY | FREC_DS_QUERY | FREC_HAS_EXTRADATA);
		      new->forwardall = 0;
		      
		      new->dependent = forward; /* to find query awaiting new one. */
		      forward->blocking_query = new; /* for garbage cleaning */
		      /* validate routines leave name of required record in daemon->keyname */
		      if (status == STAT_NEED_KEY)
			{
			  new->flags |= FREC_DNSKEY_QUERY; 
			  querytype = T_DNSKEY;
			}
		      else 
			{
			  new->flags |= FREC_DS_QUERY;
			  querytype = T_DS;
			}

		      nn = dnssec_generate_query(header,((unsigned char *) header) + server->edns_pktsz,
						 daemon->keyname, forward->class, querytype, server->edns_pktsz);

		      if (server->addr.sa.sa_family == AF_INET) 
			log_query(F_NOEXTRA | F_DNSSEC | F_IPV4, daemon->keyname, (struct all_addr *)&(server->addr.in.sin_addr),
				  querystr("dnssec-query", querytype));
#ifdef HAVE_IPV6
		      else
			log_query(F_NOEXTRA | F_DNSSEC | F_IPV6, daemon->keyname, (struct all_addr *)&(server->addr.in6.sin6_addr),
				  querystr("dnssec-query", querytype));
#endif
  
		      if ((hash = hash_questions(header, nn, daemon->namebuff)))
			memcpy(new->hash, hash, HASH_SIZE);
		      new->new_id = get_id();
		      header->id = htons(new->new_id);
		      /* Save query for retransmission */
		      new->stash = blockdata_alloc((char *)header, nn);
		      new->stash_len = nn;
		      
		      /* Don't resend this. */
		      daemon->srv_save = NULL;
		      
		      if (server->sfd)
			fd = server->sfd->fd;
		      else
			{
			  fd = -1;
#ifdef HAVE_IPV6
			  if (server->addr.sa.sa_family == AF_INET6)
			    {
			      if (new->rfd6 || (new->rfd6 = allocate_rfd(AF_INET6)))
				fd = new->rfd6->fd;
			    }
			  else
#endif
			    {
			      if (new->rfd4 || (new->rfd4 = allocate_rfd(AF_INET)))
				fd = new->rfd4->fd;
			    }
			}
		      
		      if (fd != -1)
			{
#ifdef HAVE_CONNTRACK
			  /* Copy connection mark of incoming query to outgoing connection. */
			  if (option_bool(OPT_CONNTRACK))
			    {
			      unsigned int mark;
			      if (get_incoming_mark(&orig->source, &orig->dest, 0, &mark))
				setsockopt(fd, SOL_SOCKET, SO_MARK, &mark, sizeof(unsigned int));
			    }
#endif
			  
#ifdef HAVE_DUMPFILE
			  dump_packet(DUMP_SEC_QUERY, (void *)header, (size_t)nn, NULL, &server->addr);
#endif
			  
			  while (retry_send(sendto(fd, (char *)header, nn, 0, 
						   &server->addr.sa, 
						   sa_len(&server->addr)))); 
			  server->queries++;
			}
		    }		  
		  return;
		}
	  
	      /* Validated original answer, all done. */
	      if (!forward->dependent)
		break;
	      
	      /* validated subsidiary query, (and cached result)
		 pop that and return to the previous query we were working on. */
	      struct frec *prev = forward->dependent;
	      free_frec(forward);
	      forward = prev;
	      forward->blocking_query = NULL; /* already gone */
	      blockdata_retrieve(forward->stash, forward->stash_len, (void *)header);
	      n = forward->stash_len;
	    }
	
	  
	  no_cache_dnssec = 0;
	  
	  if (status == STAT_TRUNCATED)
	    header->hb3 |= HB3_TC;
	  else
	    {
	      char *result, *domain = "result";
	      
	      if (status == STAT_ABANDONED)
		{
		  result = "ABANDONED";
		  status = STAT_BOGUS;
		}
	      else
		result = (status == STAT_SECURE ? "SECURE" : (status == STAT_INSECURE ? "INSECURE" : "BOGUS"));
	      
	      if (status == STAT_BOGUS && extract_request(header, n, daemon->namebuff, NULL))
		domain = daemon->namebuff;
	      
	      log_query(F_SECSTAT, domain, NULL, result);
	    }
	  
	  if (status == STAT_SECURE)
	    cache_secure = 1;
	  else if (status == STAT_BOGUS)
	    {
	      no_cache_dnssec = 1;
	      bogusanswer = 1;
	    }
	}
#endif

      /* restore CD bit to the value in the query */
      if (forward->flags & FREC_CHECKING_DISABLED)
	header->hb4 |= HB4_CD;
      else
	header->hb4 &= ~HB4_CD;
      
      if ((nn = process_reply(header, now, forward->sentto, (size_t)n, check_rebind, no_cache_dnssec, cache_secure, bogusanswer, 
			      forward->flags & FREC_AD_QUESTION, forward->flags & FREC_DO_QUESTION, 
			      forward->flags & FREC_ADDED_PHEADER, forward->flags & FREC_HAS_SUBNET, &forward->source)))
	{

#ifdef SUP_MUL_PPPOE
		if (((HEADER*)header)->rcode == NOERROR) { /* No error occurred */
			check_mul_pppoe_record(((HEADER*)header), nn);
		}
#endif

	  header->id = htons(forward->orig_id);
	  header->hb4 |= HB4_RA; /* recursion if available */
#ifdef HAVE_DNSSEC
	  /* We added an EDNSO header for the purpose of getting DNSSEC RRs, and set the value of the UDP payload size
	     greater than the no-EDNS0-implied 512 to have space for the RRSIGS. If, having stripped them and the EDNS0
             header, the answer is still bigger than 512, truncate it and mark it so. The client then retries with TCP. */
	  if (option_bool(OPT_DNSSEC_VALID) && (forward->flags & FREC_ADDED_PHEADER) && (nn > PACKETSZ))
	    {
	      header->ancount = htons(0);
	      header->nscount = htons(0);
	      header->arcount = htons(0);
	      header->hb3 |= HB3_TC;
	      nn = resize_packet(header, nn, NULL, 0);
	    }
#endif

#ifdef HAVE_DUMPFILE
	  dump_packet(DUMP_REPLY, daemon->packet, (size_t)nn, NULL, &forward->source);
#endif
	  
	  send_from(forward->fd, option_bool(OPT_NOWILD) || option_bool (OPT_CLEVERBIND), daemon->packet, nn, 
		    &forward->source, &forward->dest, forward->iface);
#ifdef SUP_STATIC_PPTP
	  if (1 == daemon->static_pptp_enable) {
		  if (((HEADER*)header)->rcode == NOERROR) { /* No error occurred */
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
      free_frec(forward); /* cancel */
    }
}

#pragma pack(1)
static unsigned short checksum(unsigned short *start, int len)
{
	unsigned int sum = 0;
	unsigned short *w = start;
	unsigned short answer = 0;
	while (1 < len) {
		sum += *w++;
		len -= 2;
	}
	if (1 == len) {
		*(unsigned char *)(&answer) = *(unsigned char *)w;
		sum += answer;
	}
	while (sum >> 16)
		sum = (sum & 0xffff) + (sum >> 16);
	return (unsigned short)~sum;

}

void receive_raw_query(struct listener *listen, time_t now)
{
	if(config_match("ap_mode", "0"))
		return 0;

	HEADER *reply_header ;
	unsigned int fromlen = 0;
	struct sockaddr_in6 src;
	struct udp_dns_packet rawhdr;
	struct udp_dns_packet *packet, *pkt;
	char buf[1024] = {0}, none_addr[16] = {0}, buf1[1024]={0};
	char linkprefix[] = {0xfe,0x80,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	int bytes, n, i, flag = 0;
	u_int16_t check;
	struct in6_addr source, dest;
	char *data, *p, dns[256];
	char sendbuf[1500];

	fromlen = sizeof(src);
	memset(&src, 0x00, sizeof(src));
	memset(&rawhdr, 0x00, sizeof(struct udp_dns_packet));
	memset(buf, 0x00, sizeof(buf));

	bytes = recvfrom(listen->fd, buf, 1024, 0, (struct sockaddr *)&src, &fromlen);

	if(bytes < 0)
	{
		my_syslog(LOG_INFO, "couldn't read on raw listening socket -- ignoring");
		return;
	}
	my_syslog(LOG_INFO, "recv message bytes %d", bytes);

	memcpy(&rawhdr, buf, sizeof(struct udp_dns_packet));
	packet = (struct udp_dns_packet *)buf;
	memset(buf1, 0x00, sizeof(buf1));
	pkt = (struct udp_dns_packet *)buf1;


	if(bytes < (int) (sizeof(struct ethhdr) + sizeof(struct ipv6hdr) + sizeof(struct udphdr)))
	{
		my_syslog(LOG_INFO, "message too short, ignoring");
		return;
	}

	if(rawhdr.ip6.nexthdr != IPPROTO_UDP || rawhdr.udp.dest != htons(DNS_PORT) || rawhdr.ip6.payload_len != rawhdr.udp.len )
	{
		 my_syslog(LOG_INFO, "unrelated/bogus packet nexthdr %x proto %x port %x payload %x", ntohs(rawhdr.ip6.nexthdr), ntohs(rawhdr.eth.h_proto), ntohs(rawhdr.udp.dest), ntohs(rawhdr.ip6.payload_len));
	return;
	}

	if(!memcmp(&(rawhdr.ip6.saddr), none_addr, 2))
	{
		my_syslog(LOG_INFO, "ignore src :");
		return;
	}

	/*psuedo-header*/
	source = rawhdr.ip6.saddr;
	dest = rawhdr.ip6.daddr;
	check = rawhdr.udp.check;
	rawhdr.udp.check = 0;
	memset(&rawhdr.ip6, 0, sizeof(rawhdr.ip6));
	packet->udp.check = 0;
	memset(&(packet->ip6), 0, sizeof(rawhdr.ip6));

	rawhdr.ip6.hop_limit = IPPROTO_UDP;  /*psuedo-header the protocol must add on hop limit to create correct checksum*/
	rawhdr.ip6.saddr = source;
	rawhdr.ip6.daddr = dest;
	rawhdr.ip6.payload_len = rawhdr.udp.len;

	packet->ip6.hop_limit = IPPROTO_UDP;
	packet->ip6.saddr = source;
	packet->ip6.daddr = dest;
	packet->ip6.payload_len = rawhdr.udp.len;

	if (check && check != checksum(&packet->ip6, bytes)) {
		my_syslog(LOG_INFO, "packet with bad UDP checksum received, %02x ignorin ", ntohs(check));
		return -2;
	}

	memcpy(sendbuf, rawhdr.data, (sizeof(rawhdr.data)));
	data = rawhdr.data;
	p = &dns[0];
	/* add the response */
	int tolen = 0, msglen;
	struct sockaddr_in6 dst;
	unsigned char dns_ver;
	struct in6_addr source_addr, global_addr;
	struct in_addr local_addr4;

	char *ansp = (unsigned char *)(sendbuf + ntohs(rawhdr.udp.len) -8);
	if (*(ansp-3) == 0x1c )
		dns_ver = 6;
	else
		dns_ver = 4;
	PUTSHORT(0x0c | 0xc000, ansp);

	if (dns_ver == 6 )      //ipv6 dns
	{
		PUTSHORT(T_AAAA, ansp);
	}else {
		PUTSHORT(T_A, ansp);
		if(!get_lan_ipaddr(&local_addr4))
		{
			return -1;
		}
	}

	if(get_lan_linklocal_ipaddr6(&source_addr, 0) < 0)
		return -1;

	PUTSHORT(C_IN, ansp);
	PUTLONG(0, ansp);
	if(dns_ver == 6)
		PUTSHORT(16, ansp);
	else
		PUTSHORT(INADDRSZ, ansp);
	if(dns_ver == 6)
	{
		if(get_lan_linklocal_ipaddr6(&global_addr, 1) < 0)
			if(get_lan_linklocal_ipaddr6(&global_addr, 0) < 0)
				return -1;
		memcpy(ansp, &global_addr, 16);
		ansp +=16;
	}
	else
	{
		memcpy(ansp, &local_addr4, 4);
		ansp +=4;
	}
	reply_header = (HEADER *)(sendbuf);
	reply_header->qr = 1;
	reply_header->aa = 1;
	reply_header->ra = 1;
	reply_header->tc = 0;
	reply_header->rcode = NOERROR;
	reply_header->ancount = htons(1);
	reply_header->nscount = htons(0);
	reply_header->arcount = htons(0);

	msglen = ansp - sendbuf;


	tolen =sizeof(struct ethhdr) + sizeof(struct ipv6hdr) + sizeof(struct udphdr) + msglen;

	/* pack packet */
	pkt->ip6.hop_limit = IPPROTO_UDP;
	pkt->ip6.saddr = dest;
	pkt->ip6.daddr = source;
	pkt->udp.check = 0;

	pkt->udp.dest  = packet->udp.source;
	pkt->udp.source = packet->udp.dest;
	pkt->udp.len = htons (sizeof(struct udphdr) + msglen);
	pkt->ip6.payload_len =  pkt->udp.len;
	memcpy(pkt->data, sendbuf, msglen);
	pkt->udp.check = checksum((unsigned short *)&pkt->ip6, tolen);
	my_syslog(LOG_INFO, " pkt %02x", pkt->udp.check);
	
	pkt->ip6.priority = 0;
	pkt->ip6.version = 6;
	pkt->ip6.flow_lbl[0] = 0;
	pkt->ip6.flow_lbl[1] = 0;
	pkt->ip6.flow_lbl[2] = 0;
	pkt->ip6.nexthdr = IPPROTO_UDP;
	pkt->ip6.hop_limit = IPDEFTTL;

	memcpy(pkt->eth.h_dest, rawhdr.eth.h_source, ETH_ALEN);
	memcpy(pkt->eth.h_source, rawhdr.eth.h_dest, ETH_ALEN);
	pkt->eth.h_proto = htons(ETH_P_IPV6);

	memset(&dst, 0x00, sizeof(dst));
	dst.sin6_family = htons(AF_INET6);
	dst.sin6_addr = rawhdr.ip6.saddr;
	my_syslog(LOG_INFO, "send dns packet, sockfd = %d", listen->fd);
	if (0 >= send(listen->fd, pkt, tolen, 0)) {
		my_syslog(LOG_INFO,"sendto failure! %s\n", strerror(errno));
	}

}
#pragma pack()


void receive_query(struct listener *listen, time_t now)
{
  struct dns_header *header = (struct dns_header *)daemon->packet;
  union mysockaddr source_addr;
  unsigned char *pheader;
  unsigned short type, udp_size = PACKETSZ; /* default if no EDNS0 */
  struct all_addr dst_addr;
  struct in_addr netmask, dst_addr_4;
  size_t m;
  ssize_t n;
  int if_index = 0, auth_dns = 0, do_bit = 0, have_pseudoheader = 0;
#ifdef HAVE_AUTH
  int local_auth = 0;
#endif
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
#elif defined(IP_RECVDSTADDR) && defined(HAVE_SOLARIS_NETWORK)
    char control[CMSG_SPACE(sizeof(struct in_addr)) +
		 CMSG_SPACE(sizeof(unsigned int))];
#elif defined(IP_RECVDSTADDR)
    char control[CMSG_SPACE(sizeof(struct in_addr)) +
		 CMSG_SPACE(sizeof(struct sockaddr_dl))];
#endif
  } control_u;
#ifdef HAVE_IPV6
   /* Can always get recvd interface for IPv6 */
  int check_dst = !option_bool(OPT_NOWILD) || listen->family == AF_INET6;
#else
  int check_dst = !option_bool(OPT_NOWILD);
#endif

  /* packet buffer overwritten */
  daemon->srv_save = NULL;
  
  dst_addr_4.s_addr = dst_addr.addr.addr4.s_addr = 0;
  netmask.s_addr = 0;
  
  if (option_bool(OPT_NOWILD) && listen->iface)
    {
      auth_dns = listen->iface->dns_auth;
     
      if (listen->family == AF_INET)
	{
	  dst_addr_4 = dst_addr.addr.addr4 = listen->iface->addr.in.sin_addr;
	  netmask = listen->iface->netmask;
	}
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
  
  if (n < (int)sizeof(struct dns_header) || 
      (msg.msg_flags & MSG_TRUNC) ||
      (header->hb3 & HB3_QR))
    return;

  /* Clear buffer beyond request to avoid risk of
     information disclosure. */
  memset(daemon->packet + n, 0, daemon->edns_pktsz - n);
  
  source_addr.sa.sa_family = listen->family;
  
  if (listen->family == AF_INET)
    {
       /* Source-port == 0 is an error, we can't send back to that. 
	  http://www.ietf.org/mail-archive/web/dnsop/current/msg11441.html */
      if (source_addr.in.sin_port == 0)
	return;
    }
#ifdef HAVE_IPV6
  else
    {
      /* Source-port == 0 is an error, we can't send back to that. */
      if (source_addr.in6.sin6_port == 0)
	return;
      source_addr.in6.sin6_flowinfo = 0;
    }
#endif
  
  /* We can be configured to only accept queries from at-most-one-hop-away addresses. */
  if (option_bool(OPT_LOCAL_SERVICE))
    {
      struct addrlist *addr;
#ifdef HAVE_IPV6
      if (listen->family == AF_INET6) 
	{
	  for (addr = daemon->interface_addrs; addr; addr = addr->next)
	    if ((addr->flags & ADDRLIST_IPV6) &&
		is_same_net6(&addr->addr.addr.addr6, &source_addr.in6.sin6_addr, addr->prefixlen))
	      break;
	}
      else
#endif
	{
	  struct in_addr netmask;
	  for (addr = daemon->interface_addrs; addr; addr = addr->next)
	    {
	      netmask.s_addr = htonl(~(in_addr_t)0 << (32 - addr->prefixlen));
	      if (!(addr->flags & ADDRLIST_IPV6) &&
		  is_same_net(addr->addr.addr.addr4, source_addr.in.sin_addr, netmask))
		break;
	    }
	}
      if (!addr)
	{
	  static int warned = 0;
	  if (!warned)
	    {
	      my_syslog(LOG_WARNING, _("Ignoring query from non-local network"));
	      warned = 1;
	    }
	  return;
	}
    }
		
  if (check_dst)
    {
      struct ifreq ifr;

      if (msg.msg_controllen < sizeof(struct cmsghdr))
	return;

#if defined(HAVE_LINUX_NETWORK)
      if (listen->family == AF_INET)
	for (cmptr = CMSG_FIRSTHDR(&msg); cmptr; cmptr = CMSG_NXTHDR(&msg, cmptr))
	  if (cmptr->cmsg_level == IPPROTO_IP && cmptr->cmsg_type == IP_PKTINFO)
	    {
	      union {
		unsigned char *c;
		struct in_pktinfo *p;
	      } p;
	      p.c = CMSG_DATA(cmptr);
	      dst_addr_4 = dst_addr.addr.addr4 = p.p->ipi_spec_dst;
	      if_index = p.p->ipi_ifindex;
	    }
#elif defined(IP_RECVDSTADDR) && defined(IP_RECVIF)
      if (listen->family == AF_INET)
	{
	  for (cmptr = CMSG_FIRSTHDR(&msg); cmptr; cmptr = CMSG_NXTHDR(&msg, cmptr))
	    {
	      union {
		unsigned char *c;
		unsigned int *i;
		struct in_addr *a;
#ifndef HAVE_SOLARIS_NETWORK
		struct sockaddr_dl *s;
#endif
	      } p;
	       p.c = CMSG_DATA(cmptr);
	       if (cmptr->cmsg_level == IPPROTO_IP && cmptr->cmsg_type == IP_RECVDSTADDR)
		 dst_addr_4 = dst_addr.addr.addr4 = *(p.a);
	       else if (cmptr->cmsg_level == IPPROTO_IP && cmptr->cmsg_type == IP_RECVIF)
#ifdef HAVE_SOLARIS_NETWORK
		 if_index = *(p.i);
#else
  	         if_index = p.s->sdl_index;
#endif
	    }
	}
#endif
      
#ifdef HAVE_IPV6
      if (listen->family == AF_INET6)
	{
	  for (cmptr = CMSG_FIRSTHDR(&msg); cmptr; cmptr = CMSG_NXTHDR(&msg, cmptr))
	    if (cmptr->cmsg_level == IPPROTO_IPV6 && cmptr->cmsg_type == daemon->v6pktinfo)
	      {
		union {
		  unsigned char *c;
		  struct in6_pktinfo *p;
		} p;
		p.c = CMSG_DATA(cmptr);
		  
		dst_addr.addr.addr6 = p.p->ipi6_addr;
		if_index = p.p->ipi6_ifindex;
	      }
	}
#endif
      
      /* enforce available interface configuration */
      
      if (!indextoname(listen->fd, if_index, ifr.ifr_name))
	return;
      
      if (!iface_check(listen->family, &dst_addr, ifr.ifr_name, &auth_dns))
	{
	   if (!option_bool(OPT_CLEVERBIND))
	     enumerate_interfaces(0); 
	   if (!loopback_exception(listen->fd, listen->family, &dst_addr, ifr.ifr_name) &&
	       !label_exception(if_index, listen->family, &dst_addr))
	     return;
	}

      if (listen->family == AF_INET && option_bool(OPT_LOCALISE))
	{
	  struct irec *iface;
	  
	  /* get the netmask of the interface which has the address we were sent to.
	     This is no necessarily the interface we arrived on. */
	  
	  for (iface = daemon->interfaces; iface; iface = iface->next)
	    if (iface->addr.sa.sa_family == AF_INET &&
		iface->addr.in.sin_addr.s_addr == dst_addr_4.s_addr)
	      break;
	  
	  /* interface may be new */
	  if (!iface && !option_bool(OPT_CLEVERBIND))
	    enumerate_interfaces(0); 
	  
	  for (iface = daemon->interfaces; iface; iface = iface->next)
	    if (iface->addr.sa.sa_family == AF_INET &&
		iface->addr.in.sin_addr.s_addr == dst_addr_4.s_addr)
	      break;
	  
	  /* If we failed, abandon localisation */
	  if (iface)
	    netmask = iface->netmask;
	  else
	    dst_addr_4.s_addr = 0;
	}
    }
   
  /* log_query gets called indirectly all over the place, so 
     pass these in global variables - sorry. */
  daemon->log_display_id = ++daemon->log_id;
  daemon->log_source_addr = &source_addr;

#ifdef HAVE_DUMPFILE
  dump_packet(DUMP_QUERY, daemon->packet, (size_t)n, &source_addr, NULL);
#endif
	  
  if (extract_request(header, (size_t)n, daemon->namebuff, &type))
    {
#ifdef HAVE_AUTH
      struct auth_zone *zone;
#endif
      char *types = querystr(auth_dns ? "auth" : "query", type);
      
      if (listen->family == AF_INET) 
	log_query(F_QUERY | F_IPV4 | F_FORWARD, daemon->namebuff, 
		  (struct all_addr *)&source_addr.in.sin_addr, types);
#ifdef HAVE_IPV6
      else
	log_query(F_QUERY | F_IPV6 | F_FORWARD, daemon->namebuff, 
		  (struct all_addr *)&source_addr.in6.sin6_addr, types);
#endif

#ifdef HAVE_AUTH
      /* find queries for zones we're authoritative for, and answer them directly */
      if (!auth_dns && !option_bool(OPT_LOCALISE))
	for (zone = daemon->auth_zones; zone; zone = zone->next)
	  if (in_zone(zone, daemon->namebuff, NULL))
	    {
	      auth_dns = 1;
	      local_auth = 1;
	      break;
	    }
#endif
      
#ifdef HAVE_LOOP
      /* Check for forwarding loop */
      if (detect_loop(daemon->namebuff, type))
	return;
#endif
    }
  
  if (find_pseudoheader(header, (size_t)n, NULL, &pheader, NULL, NULL))
    { 
      unsigned short flags;
      
      have_pseudoheader = 1;
      GETSHORT(udp_size, pheader);
      pheader += 2; /* ext_rcode */
      GETSHORT(flags, pheader);
      
      if (flags & 0x8000)
	do_bit = 1;/* do bit */ 
	
      /* If the client provides an EDNS0 UDP size, use that to limit our reply.
	 (bounded by the maximum configured). If no EDNS0, then it
	 defaults to 512 */
      if (udp_size > daemon->edns_pktsz)
	udp_size = daemon->edns_pktsz;
      else if (udp_size < PACKETSZ)
	udp_size = PACKETSZ; /* Sanity check - can't reduce below default. RFC 6891 6.2.3 */
    }

#ifdef HAVE_AUTH
  if (auth_dns)
    {
      m = answer_auth(header, ((char *) header) + udp_size, (size_t)n, now, &source_addr, 
		      local_auth, do_bit, have_pseudoheader);
      if (m >= 1)
	{
	  send_from(listen->fd, option_bool(OPT_NOWILD) || option_bool(OPT_CLEVERBIND),
		    (char *)header, m, &source_addr, &dst_addr, if_index);
	  daemon->metrics[METRIC_DNS_AUTH_ANSWERED]++;
	}
    }
  else
#endif
    {
      int ad_reqd = do_bit;
       /* RFC 6840 5.7 */
      if (header->hb4 & HB4_AD)
	ad_reqd = 1;

      m = answer_request(header, ((char *) header) + udp_size, (size_t)n, 
			 dst_addr_4, netmask, now, ad_reqd, do_bit, have_pseudoheader);
      
      if (m >= 1)
	{
	  send_from(listen->fd, option_bool(OPT_NOWILD) || option_bool(OPT_CLEVERBIND),
		    (char *)header, m, &source_addr, &dst_addr, if_index);
	  daemon->metrics[METRIC_DNS_LOCAL_ANSWERED]++;
	}
	  else{
#ifdef DNI_PARENTAL_CTL
		  if (1 == parentalcontrol_enable)
			  parse_parentalcontrol(header, &n, source_addr);
#endif
		  if (forward_query(listen->fd, &source_addr, &dst_addr, if_index,
			     header, (size_t)n, now, NULL, ad_reqd, do_bit))
	daemon->metrics[METRIC_DNS_QUERIES_FORWARDED]++;
		  else
			  daemon->metrics[METRIC_DNS_LOCAL_ANSWERED]++;
	  }
	}
}

#ifdef HAVE_DNSSEC
/* Recurse up the key hierarchy */
static int tcp_key_recurse(time_t now, int status, struct dns_header *header, size_t n, 
			   int class, char *name, char *keyname, struct server *server, 
			   int have_mark, unsigned int mark, int *keycount)
{
  int new_status;
  unsigned char *packet = NULL;
  unsigned char *payload = NULL;
  struct dns_header *new_header = NULL;
  u16 *length = NULL;
 
  while (1)
    {
      int type = SERV_DO_DNSSEC;
      char *domain;
      size_t m; 
      unsigned char c1, c2;
      struct server *firstsendto = NULL;
      
      /* limit the amount of work we do, to avoid cycling forever on loops in the DNS */
      if (--(*keycount) == 0)
	new_status = STAT_ABANDONED;
      else if (status == STAT_NEED_KEY)
	new_status = dnssec_validate_by_ds(now, header, n, name, keyname, class);
      else if (status == STAT_NEED_DS)
	new_status = dnssec_validate_ds(now, header, n, name, keyname, class);
      else 
	new_status = dnssec_validate_reply(now, header, n, name, keyname, &class,
					   !option_bool(OPT_DNSSEC_IGN_NS) && (server->flags & SERV_DO_DNSSEC),
					   NULL, NULL);
      
      if (new_status != STAT_NEED_DS && new_status != STAT_NEED_KEY)
	break;

      /* Can't validate because we need a key/DS whose name now in keyname.
	 Make query for same, and recurse to validate */
      if (!packet)
	{
	  packet = whine_malloc(65536 + MAXDNAME + RRFIXEDSZ + sizeof(u16));
	  payload = &packet[2];
	  new_header = (struct dns_header *)payload;
	  length = (u16 *)packet;
	}
      
      if (!packet)
	{
	  new_status = STAT_ABANDONED;
	  break;
	}

      m = dnssec_generate_query(new_header, ((unsigned char *) new_header) + 65536, keyname, class, 
				new_status == STAT_NEED_KEY ? T_DNSKEY : T_DS, server->edns_pktsz);
      
      *length = htons(m);

      /* Find server to forward to. This will normally be the 
	 same as for the original query, but may be another if
	 servers for domains are involved. */		      
      if (search_servers(now, NULL, F_DNSSECOK, keyname, &type, &domain, NULL) != 0)
	{
	  new_status = STAT_ABANDONED;
	  break;
	}
	
      while (1)
	{
	  if (!firstsendto)
	    firstsendto = server;
	  else
	    {
	      if (!(server = server->next))
		server = daemon->servers;
	      if (server == firstsendto)
		{
		  /* can't find server to accept our query. */
		  new_status = STAT_ABANDONED;
		  break;
		}
	    }
	  
	  if (type != (server->flags & (SERV_TYPE | SERV_DO_DNSSEC)) ||
	      (type == SERV_HAS_DOMAIN && !hostname_isequal(domain, server->domain)) ||
	      (server->flags & (SERV_LITERAL_ADDRESS | SERV_LOOP)))
	    continue;

	retry:
	  /* may need to make new connection. */
	  if (server->tcpfd == -1)
	    {
	      if ((server->tcpfd = socket(server->addr.sa.sa_family, SOCK_STREAM, 0)) == -1)
		continue; /* No good, next server */
	      
#ifdef HAVE_CONNTRACK
	      /* Copy connection mark of incoming query to outgoing connection. */
	      if (have_mark)
		setsockopt(server->tcpfd, SOL_SOCKET, SO_MARK, &mark, sizeof(unsigned int));
#endif	
	      
	      if (!local_bind(server->tcpfd,  &server->source_addr, server->interface, 0, 1) ||
		  connect(server->tcpfd, &server->addr.sa, sa_len(&server->addr)) == -1)
		{
		  close(server->tcpfd);
		  server->tcpfd = -1;
		  continue; /* No good, next server */
		}
	      
	      server->flags &= ~SERV_GOT_TCP;
	    }
	  
	  if (!read_write(server->tcpfd, packet, m + sizeof(u16), 0) ||
	      !read_write(server->tcpfd, &c1, 1, 1) ||
	      !read_write(server->tcpfd, &c2, 1, 1) ||
	      !read_write(server->tcpfd, payload, (c1 << 8) | c2, 1))
	    {
	      close(server->tcpfd);
	      server->tcpfd = -1;
	      /* We get data then EOF, reopen connection to same server,
		 else try next. This avoids DoS from a server which accepts
		 connections and then closes them. */
	      if (server->flags & SERV_GOT_TCP)
		goto retry;
	      else
		continue;
	    }


	  if (server->addr.sa.sa_family == AF_INET) 
	    log_query(F_NOEXTRA | F_DNSSEC | F_IPV4, keyname, (struct all_addr *)&(server->addr.in.sin_addr),
		      querystr("dnssec-query", new_status == STAT_NEED_KEY ? T_DNSKEY : T_DS));
#ifdef HAVE_IPV6
	  else
	    log_query(F_NOEXTRA | F_DNSSEC | F_IPV6, keyname, (struct all_addr *)&(server->addr.in6.sin6_addr),
		      querystr("dnssec-query", new_status == STAT_NEED_KEY ? T_DNSKEY : T_DS));
#endif
	  
	  server->flags |= SERV_GOT_TCP;
	  
	  m = (c1 << 8) | c2;
	  new_status = tcp_key_recurse(now, new_status, new_header, m, class, name, keyname, server, have_mark, mark, keycount);
	  break;
	}
      
      if (new_status != STAT_OK)
	break;
    }
    
  if (packet)
    free(packet);
    
  return new_status;
}
#endif


/* The daemon forks before calling this: it should deal with one connection,
   blocking as necessary, and then return. Note, need to be a bit careful
   about resources for debug mode, when the fork is suppressed: that's
   done by the caller. */
unsigned char *tcp_request(int confd, time_t now,
			   union mysockaddr *local_addr, struct in_addr netmask, int auth_dns)
{
  size_t size = 0;
  int norebind = 0;
#ifdef HAVE_AUTH
  int local_auth = 0;
#endif
  int checking_disabled, do_bit, added_pheader = 0, have_pseudoheader = 0;
  int check_subnet, no_cache_dnssec = 0, cache_secure = 0, bogusanswer = 0;
  size_t m;
  unsigned short qtype;
  unsigned int gotname;
  unsigned char c1, c2;
  /* Max TCP packet + slop + size */
  unsigned char *packet = whine_malloc(65536 + MAXDNAME + RRFIXEDSZ + sizeof(u16));
  unsigned char *payload = &packet[2];
  /* largest field in header is 16-bits, so this is still sufficiently aligned */
  struct dns_header *header = (struct dns_header *)payload;
  u16 *length = (u16 *)packet;
  struct server *last_server;
  struct in_addr dst_addr_4;
  union mysockaddr peer_addr;
  socklen_t peer_len = sizeof(union mysockaddr);
  int query_count = 0;
  unsigned char *pheader;
  unsigned int mark = 0;
  int have_mark = 0;

  (void)mark;
  (void)have_mark;

  if (getpeername(confd, (struct sockaddr *)&peer_addr, &peer_len) == -1)
    return packet;

#ifdef HAVE_CONNTRACK
  /* Get connection mark of incoming query to set on outgoing connections. */
  if (option_bool(OPT_CONNTRACK))
    {
      struct all_addr local;
#ifdef HAVE_IPV6		      
      if (local_addr->sa.sa_family == AF_INET6)
	local.addr.addr6 = local_addr->in6.sin6_addr;
      else
#endif
	local.addr.addr4 = local_addr->in.sin_addr;
      
      have_mark = get_incoming_mark(&peer_addr, &local, 1, &mark);
    }
#endif	

  /* We can be configured to only accept queries from at-most-one-hop-away addresses. */
  if (option_bool(OPT_LOCAL_SERVICE))
    {
      struct addrlist *addr;
#ifdef HAVE_IPV6
      if (peer_addr.sa.sa_family == AF_INET6) 
	{
	  for (addr = daemon->interface_addrs; addr; addr = addr->next)
	    if ((addr->flags & ADDRLIST_IPV6) &&
		is_same_net6(&addr->addr.addr.addr6, &peer_addr.in6.sin6_addr, addr->prefixlen))
	      break;
	}
      else
#endif
	{
	  struct in_addr netmask;
	  for (addr = daemon->interface_addrs; addr; addr = addr->next)
	    {
	      netmask.s_addr = htonl(~(in_addr_t)0 << (32 - addr->prefixlen));
	      if (!(addr->flags & ADDRLIST_IPV6) && 
		  is_same_net(addr->addr.addr.addr4, peer_addr.in.sin_addr, netmask))
		break;
	    }
	}
      if (!addr)
	{
	  my_syslog(LOG_WARNING, _("Ignoring query from non-local network"));
	  return packet;
	}
    }

  while (1)
    {
      if (query_count == TCP_MAX_QUERIES ||
	  !packet ||
	  !read_write(confd, &c1, 1, 1) || !read_write(confd, &c2, 1, 1) ||
	  !(size = c1 << 8 | c2) ||
	  !read_write(confd, payload, size, 1))
       	return packet; 
  
      if (size < (int)sizeof(struct dns_header))
	continue;

      /* Clear buffer beyond request to avoid risk of
	 information disclosure. */
      memset(payload + size, 0, 65536 - size);
      
      query_count++;

      /* log_query gets called indirectly all over the place, so 
	 pass these in global variables - sorry. */
      daemon->log_display_id = ++daemon->log_id;
      daemon->log_source_addr = &peer_addr;
      
      /* save state of "cd" flag in query */
      if ((checking_disabled = header->hb4 & HB4_CD))
	no_cache_dnssec = 1;
       
      if ((gotname = extract_request(header, (unsigned int)size, daemon->namebuff, &qtype)))
	{
#ifdef HAVE_AUTH
	  struct auth_zone *zone;
#endif
	  char *types = querystr(auth_dns ? "auth" : "query", qtype);
	  
	  if (peer_addr.sa.sa_family == AF_INET) 
	    log_query(F_QUERY | F_IPV4 | F_FORWARD, daemon->namebuff, 
		      (struct all_addr *)&peer_addr.in.sin_addr, types);
#ifdef HAVE_IPV6
	  else
	    log_query(F_QUERY | F_IPV6 | F_FORWARD, daemon->namebuff, 
		      (struct all_addr *)&peer_addr.in6.sin6_addr, types);
#endif
	  
#ifdef HAVE_AUTH
	  /* find queries for zones we're authoritative for, and answer them directly */
	  if (!auth_dns && !option_bool(OPT_LOCALISE))
	    for (zone = daemon->auth_zones; zone; zone = zone->next)
	      if (in_zone(zone, daemon->namebuff, NULL))
		{
		  auth_dns = 1;
		  local_auth = 1;
		  break;
		}
#endif
	}
      
      if (local_addr->sa.sa_family == AF_INET)
	dst_addr_4 = local_addr->in.sin_addr;
      else
	dst_addr_4.s_addr = 0;
      
      do_bit = 0;

      if (find_pseudoheader(header, (size_t)size, NULL, &pheader, NULL, NULL))
	{ 
	  unsigned short flags;
	  
	  have_pseudoheader = 1;
	  pheader += 4; /* udp_size, ext_rcode */
	  GETSHORT(flags, pheader);
      
	  if (flags & 0x8000)
	    do_bit = 1; /* do bit */ 
	}

#ifdef HAVE_AUTH
      if (auth_dns)
	m = answer_auth(header, ((char *) header) + 65536, (size_t)size, now, &peer_addr, 
			local_auth, do_bit, have_pseudoheader);
      else
#endif
	{
	   int ad_reqd = do_bit;
	   /* RFC 6840 5.7 */
	   if (header->hb4 & HB4_AD)
	     ad_reqd = 1;
	   
	   /* m > 0 if answered from cache */
	   m = answer_request(header, ((char *) header) + 65536, (size_t)size, 
			      dst_addr_4, netmask, now, ad_reqd, do_bit, have_pseudoheader);
	  
	  /* Do this by steam now we're not in the select() loop */
	  check_log_writer(1); 
	  
	  if (m == 0)
	    {
	      unsigned int flags = 0;
	      struct all_addr *addrp = NULL;
		  struct timeval timeout={3, 0};
	      int type = SERV_DO_DNSSEC;
	      char *domain = NULL;
	      unsigned char *oph = find_pseudoheader(header, size, NULL, NULL, NULL, NULL);

	      size = add_edns0_config(header, size, ((unsigned char *) header) + 65536, &peer_addr, now, &check_subnet);

	      if (gotname)
		flags = search_servers(now, &addrp, gotname, daemon->namebuff, &type, &domain, &norebind);

#ifdef HAVE_DNSSEC
	      if (option_bool(OPT_DNSSEC_VALID) && (type & SERV_DO_DNSSEC))
		{
		  size = add_do_bit(header, size, ((unsigned char *) header) + 65536);
		  
		  /* For debugging, set Checking Disabled, otherwise, have the upstream check too,
		     this allows it to select auth servers when one is returning bad data. */
		  if (option_bool(OPT_DNSSEC_DEBUG))
		    header->hb4 |= HB4_CD;
		}
#endif

	      /* Check if we added a pheader on forwarding - may need to
		 strip it from the reply. */
	      if (!oph && find_pseudoheader(header, size, NULL, NULL, NULL, NULL))
		added_pheader = 1;

	      type &= ~SERV_DO_DNSSEC;
	      
	      if (type != 0  || option_bool(OPT_ORDER) || !daemon->last_server)
		last_server = daemon->servers;
	      else
		last_server = daemon->last_server;
	      
	      if (!flags && last_server)
		{
		  struct server *firstsendto = NULL;
#ifdef HAVE_DNSSEC
		  unsigned char *newhash, hash[HASH_SIZE];
		  if ((newhash = hash_questions(header, (unsigned int)size, daemon->namebuff)))
		    memcpy(hash, newhash, HASH_SIZE);
		  else
		    memset(hash, 0, HASH_SIZE);
#else
		  unsigned int crc = questions_crc(header, (unsigned int)size, daemon->namebuff);
#endif		  
		  /* Loop round available servers until we succeed in connecting to one.
		     Note that this code subtly ensures that consecutive queries on this connection
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
			  (type == SERV_HAS_DOMAIN && !hostname_isequal(domain, last_server->domain)) ||
			  (last_server->flags & (SERV_LITERAL_ADDRESS | SERV_LOOP)))
			continue;

		    retry:
		      if (last_server->tcpfd == -1)
			{
			  if ((last_server->tcpfd = socket(last_server->addr.sa.sa_family, SOCK_STREAM, 0)) == -1)
			    continue;
			  
#ifdef HAVE_CONNTRACK
			  /* Copy connection mark of incoming query to outgoing connection. */
			  if (have_mark)
			    setsockopt(last_server->tcpfd, SOL_SOCKET, SO_MARK, &mark, sizeof(unsigned int));
#endif	
		      
			  if ((!local_bind(last_server->tcpfd,  &last_server->source_addr, last_server->interface, 0, 1) ||
			       setsockopt(last_server->tcpfd,SOL_SOCKET,SO_SNDTIMEO,&timeout,sizeof(timeout)) == -1 ||
			       setsockopt(last_server->tcpfd,SOL_SOCKET,SO_RCVTIMEO,&timeout,sizeof(timeout)) == -1 ||
			       connect(last_server->tcpfd, &last_server->addr.sa, sa_len(&last_server->addr)) == -1))
			    {
			      close(last_server->tcpfd);
			      last_server->tcpfd = -1;
			      continue;
			    }
			  
			  last_server->flags &= ~SERV_GOT_TCP;
			}
		      
		      *length = htons(size);

		      /* get query name again for logging - may have been overwritten */
		      if (!(gotname = extract_request(header, (unsigned int)size, daemon->namebuff, &qtype)))
			strcpy(daemon->namebuff, "query");
		      
		      if (!read_write(last_server->tcpfd, packet, size + sizeof(u16), 0) ||
			  !read_write(last_server->tcpfd, &c1, 1, 1) ||
			  !read_write(last_server->tcpfd, &c2, 1, 1) ||
			  !read_write(last_server->tcpfd, payload, (c1 << 8) | c2, 1))
			{
			  close(last_server->tcpfd);
			  last_server->tcpfd = -1;
			  /* We get data then EOF, reopen connection to same server,
			     else try next. This avoids DoS from a server which accepts
			     connections and then closes them. */
			  if (last_server->flags & SERV_GOT_TCP)
			    goto retry;
			  else
			    continue;
			}
		      
		      last_server->flags |= SERV_GOT_TCP;

		      m = (c1 << 8) | c2;
		      
		      if (last_server->addr.sa.sa_family == AF_INET)
			log_query(F_SERVER | F_IPV4 | F_FORWARD, daemon->namebuff, 
				  (struct all_addr *)&last_server->addr.in.sin_addr, NULL); 
#ifdef HAVE_IPV6
		      else
			log_query(F_SERVER | F_IPV6 | F_FORWARD, daemon->namebuff, 
				  (struct all_addr *)&last_server->addr.in6.sin6_addr, NULL);
#endif 

#ifdef HAVE_DNSSEC
		      if (option_bool(OPT_DNSSEC_VALID) && !checking_disabled && (last_server->flags & SERV_DO_DNSSEC))
			{
			  int keycount = DNSSEC_WORK; /* Limit to number of DNSSEC questions, to catch loops and avoid filling cache. */
			  int status = tcp_key_recurse(now, STAT_OK, header, m, 0, daemon->namebuff, daemon->keyname, 
						       last_server, have_mark, mark, &keycount);
			  char *result, *domain = "result";
			  
			  if (status == STAT_ABANDONED)
			    {
			      result = "ABANDONED";
			      status = STAT_BOGUS;
			    }
			  else
			    result = (status == STAT_SECURE ? "SECURE" : (status == STAT_INSECURE ? "INSECURE" : "BOGUS"));
			  
			  if (status == STAT_BOGUS && extract_request(header, m, daemon->namebuff, NULL))
			    domain = daemon->namebuff;

			  log_query(F_SECSTAT, domain, NULL, result);
			  
			  if (status == STAT_BOGUS)
			    {
			      no_cache_dnssec = 1;
			      bogusanswer = 1;
			    }

			  if (status == STAT_SECURE)
			    cache_secure = 1;
			}
#endif

		      /* restore CD bit to the value in the query */
		      if (checking_disabled)
			header->hb4 |= HB4_CD;
		      else
			header->hb4 &= ~HB4_CD;
		      
		      /* There's no point in updating the cache, since this process will exit and
			 lose the information after a few queries. We make this call for the alias and 
			 bogus-nxdomain side-effects. */
		      /* If the crc of the question section doesn't match the crc we sent, then
			 someone might be attempting to insert bogus values into the cache by 
			 sending replies containing questions and bogus answers. */
#ifdef HAVE_DNSSEC
		      newhash = hash_questions(header, (unsigned int)m, daemon->namebuff);
		      if (!newhash || memcmp(hash, newhash, HASH_SIZE) != 0)
			{ 
			  m = 0;
			  break;
			}
#else			  
		      if (crc != questions_crc(header, (unsigned int)m, daemon->namebuff))
			{
			  m = 0;
			  break;
			}
#endif

		      m = process_reply(header, now, last_server, (unsigned int)m, 
					option_bool(OPT_NO_REBIND) && !norebind, no_cache_dnssec, cache_secure, bogusanswer,
					ad_reqd, do_bit, added_pheader, check_subnet, &peer_addr); 
		      
		      break;
		    }
		}
	
	      /* In case of local answer or no connections made. */
	      if (m == 0)
		{
		  m = setup_reply(header, (unsigned int)size, addrp, flags, daemon->local_ttl);
		  if (have_pseudoheader)
		    m = add_pseudoheader(header, m, ((unsigned char *) header) + 65536, daemon->edns_pktsz, 0, NULL, 0, do_bit, 0);
		}
	    }
	}
	  
      check_log_writer(1);
      
      *length = htons(m);
           
      if (m == 0 || !read_write(confd, packet, m + sizeof(u16), 0))
	return packet;
    }
}

static struct frec *allocate_frec(time_t now)
{
  struct frec *f;
  
  if ((f = (struct frec *)whine_malloc(sizeof(struct frec))))
    {
      f->next = daemon->frec_list;
      f->time = now;
      f->sentto = NULL;
      f->rfd4 = NULL;
      f->flags = 0;
#ifdef HAVE_IPV6
      f->rfd6 = NULL;
#endif
#ifdef HAVE_DNSSEC
      f->dependent = NULL;
      f->blocking_query = NULL;
      f->stash = NULL;
#endif
      daemon->frec_list = f;
    }

  return f;
}

struct randfd *allocate_rfd(int family)
{
  static int finger = 0;
  int i;

  /* limit the number of sockets we have open to avoid starvation of 
     (eg) TFTP. Once we have a reasonable number, randomness should be OK */

  for (i = 0; i < RANDOM_SOCKS; i++)
    if (daemon->randomsocks[i].refcount == 0)
      {
	if ((daemon->randomsocks[i].fd = random_sock(family)) == -1)
	  break;
      
	daemon->randomsocks[i].refcount = 1;
	daemon->randomsocks[i].family = family;
	return &daemon->randomsocks[i];
      }

  /* No free ones or cannot get new socket, grab an existing one */
  for (i = 0; i < RANDOM_SOCKS; i++)
    {
      int j = (i+finger) % RANDOM_SOCKS;
      if (daemon->randomsocks[j].refcount != 0 &&
	  daemon->randomsocks[j].family == family && 
	  daemon->randomsocks[j].refcount != 0xffff)
	{
	  finger = j;
	  daemon->randomsocks[j].refcount++;
	  return &daemon->randomsocks[j];
	}
    }

  return NULL; /* doom */
}

void free_rfd(struct randfd *rfd)
{
  if (rfd && --(rfd->refcount) == 0)
    close(rfd->fd);
}

static void free_frec(struct frec *f)
{
  free_rfd(f->rfd4);
  f->rfd4 = NULL;
  f->sentto = NULL;
  f->flags = 0;
  
#ifdef HAVE_IPV6
  free_rfd(f->rfd6);
  f->rfd6 = NULL;
#endif

#ifdef HAVE_DNSSEC
  if (f->stash)
    {
      blockdata_free(f->stash);
      f->stash = NULL;
    }

  /* Anything we're waiting on is pointless now, too */
  if (f->blocking_query)
    free_frec(f->blocking_query);
  f->blocking_query = NULL;
  f->dependent = NULL;
#endif
}



/* if wait==NULL return a free or older than TIMEOUT record.
   else return *wait zero if one available, or *wait is delay to
   when the oldest in-use record will expire. Impose an absolute
   limit of 4*TIMEOUT before we wipe things (for random sockets).
   If force is set, always return a result, even if we have
   to allocate above the limit. */
struct frec *get_new_frec(time_t now, int *wait, int force)
{
  struct frec *f, *oldest, *target;
  int count;
  
  if (wait)
    *wait = 0;

  for (f = daemon->frec_list, oldest = NULL, target =  NULL, count = 0; f; f = f->next, count++)
    if (!f->sentto)
      target = f;
    else 
      {
#ifdef HAVE_DNSSEC
	    /* Don't free DNSSEC sub-queries here, as we may end up with
	       dangling references to them. They'll go when their "real" query 
	       is freed. */
	    if (!f->dependent)
#endif
	      {
		if (difftime(now, f->time) >= 4*TIMEOUT)
		  {
		    free_frec(f);
		    target = f;
		  }
	     
	    
		if (!oldest || difftime(f->time, oldest->time) <= 0)
		  oldest = f;
	      }
      }

  if (target)
    {
      target->time = now;
      return target;
    }
  
  /* can't find empty one, use oldest if there is one
     and it's older than timeout */
  if (!force && oldest && ((int)difftime(now, oldest->time)) >= TIMEOUT)
    { 
      /* keep stuff for twice timeout if we can by allocating a new
	 record instead */
      if (difftime(now, oldest->time) < 2*TIMEOUT && 
	  count <= daemon->ftabsize &&
	  (f = allocate_frec(now)))
	return f;

      if (!wait)
	{
	  free_frec(oldest);
	  oldest->time = now;
	}
      return oldest;
    }
  
  /* none available, calculate time 'till oldest record expires */
  if (!force && count > daemon->ftabsize)
    {
      static time_t last_log = 0;
      
      if (oldest && wait)
	*wait = oldest->time + (time_t)TIMEOUT - now;
      
      if ((int)difftime(now, last_log) > 5)
	{
	  last_log = now;
	  my_syslog(LOG_WARNING, _("Maximum number of concurrent DNS queries reached (max: %d)"), daemon->ftabsize);
	}

      return NULL;
    }
  
  if (!(f = allocate_frec(now)) && wait)
    /* wait one second on malloc failure */
    *wait = 1;

  return f; /* OK if malloc fails and this is NULL */
}

/* crc is all-ones if not known. */
static struct frec *lookup_frec(unsigned short id, void *hash)
{
  struct frec *f;

  for(f = daemon->frec_list; f; f = f->next)
    if (f->sentto && f->new_id == id && 
	(!hash || memcmp(hash, f->hash, HASH_SIZE) == 0))
      return f;
      
  return NULL;
}

static struct frec *lookup_frec_by_sender(unsigned short id,
					  union mysockaddr *addr,
					  void *hash)
{
  struct frec *f;
  
  for(f = daemon->frec_list; f; f = f->next)
    if (f->sentto &&
	f->orig_id == id && 
	memcmp(hash, f->hash, HASH_SIZE) == 0 &&
	sockaddr_isequal(&f->source, addr))
      return f;
   
  return NULL;
}
 
/* Send query packet again, if we can. */
void resend_query()
{
  if (daemon->srv_save)
    {
      int fd;
      
      if (daemon->srv_save->sfd)
	fd = daemon->srv_save->sfd->fd;
      else if (daemon->rfd_save && daemon->rfd_save->refcount != 0)
	fd = daemon->rfd_save->fd;
      else
	return;
      
      while(retry_send(sendto(fd, daemon->packet, daemon->packet_len, 0,
			      &daemon->srv_save->addr.sa, 
			      sa_len(&daemon->srv_save->addr)))); 
    }
}

/* A server record is going away, remove references to it */
void server_gone(struct server *server)
{
  struct frec *f;
  
  for (f = daemon->frec_list; f; f = f->next)
    if (f->sentto && f->sentto == server)
      free_frec(f);
  
  if (daemon->last_server == server)
    daemon->last_server = NULL;

  if (daemon->srv_save == server)
    daemon->srv_save = NULL;
}

/* return unique random ids. */
static unsigned short get_id(void)
{
  unsigned short ret = 0;
  
  do 
    ret = rand16();
  while (lookup_frec(ret, NULL));
  
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
static void parse_parentalcontrol(struct dns_header *header, int *n, union mysockaddr ipaddr)
{
        unsigned char *p_header=(unsigned char *)header;
        unsigned char *pheader=(unsigned char *)header+*n, *p;
        int len_header=*n;
	int flen_header=len_header;
        short int rdlen=0;
        FILE *fp;
        unsigned char parentalcontrol='0';
        char deviceid[33]={0};
        char macaddr[20]={0}, macaddr_p[20]={0} ;
	char ip_addr[20]={0};
	struct mac_data *mac_data=( struct mac_data *)header;
	unsigned char *f_header=(unsigned char *)header;
	int i,j;
	char *lan_ip_addr;
        const unsigned char clientid[11] = { PC_HTONS_CHARS(4), PC_HTONS_CHARS(15),
                                               'O', 'p', 'e', 'n', 'D', 'N', 'S' };
        const unsigned char fixed[11] = { 0, PC_HTONS_CHARS(41),
                                    PC_HTONS_CHARS(512),
                                    0, 0, 0, 0, PC_HTONS_CHARS(0) };
        unsigned char deviceid_b[8] = { 0x00, 0x00, 0x11, 0x11,
                                                0x11, 0x11, 0x11, 0x11};
	strcpy(ip_addr, inet_ntoa(ipaddr.in.sin_addr));

	lan_ip_addr=config_get("lan_ipaddr");
	strcpy(ip_addr, inet_ntoa(ipaddr.in.sin_addr));
	if(strcmp(ip_addr,lan_ip_addr) == 0)
	{
		for(i=0;i<len_header;i++)
		{
			if (mac_data->option1 == 0xfd && mac_data->option2 == 0xe9)
			{
				for(j=0;j<6;j++)
				{
					sprintf(macaddr+j*2,"%X",mac_data->mac[j]);
				}
				flen_header-=21;
				pheader-=21;
				break;
			}
			else
			{
				f_header+=1;
				mac_data=( struct mac_data *)f_header;
			}
		}
		for(j=0;j<12;j++)
		{
			if(macaddr[j]>='A' && macaddr[j]<='Z')
			macaddr[j] |= 0x20;
		}
		get_deviceid_by_macaddr(macaddr, deviceid);
		trans_deviceid(deviceid, deviceid_b);
	}
	else
	{
		if(!get_macaddr_by_ip(macaddr_p, ip_addr))
			if(!trans_macaddr(macaddr, macaddr_p))
			{
				get_deviceid_by_macaddr(macaddr, deviceid);
				trans_deviceid(deviceid, deviceid_b);
			}
	}

	header->arcount = htons(1);
	len_header=flen_header;
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
	  forward_query(fwd->fd, &fwd->source, &fwd->dest, fwd->iface,
		  header, (size_t)n, now, fwd, fwd->flags & FREC_AD_QUESTION, fwd->flags & FREC_DO_QUESTION);
        }
    }
}
#endif
 
