/*
**  igmpproxy - IGMP proxy based multicast router 
**  Copyright (C) 2005 Johnny Egeland <johnny@rlo.org>
**
**  This program is free software; you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation; either version 2 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
**
**----------------------------------------------------------------------------
**
**  This software is derived work from the following software. The original
**  source code has been modified from it's original state by the author
**  of igmpproxy.
**
**  smcroute 0.92 - Copyright (C) 2001 Carsten Schill <carsten@cschill.de>
**  - Licensed under the GNU General Public License, version 2
**  
**  mrouted 3.9-beta3 - COPYRIGHT 1989 by The Board of Trustees of 
**  Leland Stanford Junior University.
**  - Original license can be found in the "doc/mrouted-LINCESE" file.
**
*/
/**
*   igmp.h - Recieves IGMP requests, and handle them 
*            appropriately...
*/

#include "defs.h"

#ifndef USE_ATH_HEADER
extern int isAdressValidForIf( struct IfDesc* intrface, uint32 ipaddr );
extern int inetChksum(u_short *addr, u_int len);
#endif
 
// Globals                  
uint32     allhosts_group;          /* All hosts addr in net order */
uint32     allrouters_group;          /* All hosts addr in net order */
#if (SUPPORT_IGMPV3)
uint32	   all_group_igmpv3;
int 	   group_compatibilty_mode;		/* Group compatibility mode */
#endif
              
extern int MRouterFD;


/*
 * Open and initialize the igmp socket, and fill in the non-changing
 * IP header fields in the output packet buffer.
 */
void initIgmp() {
    struct ip *ip;

    recv_buf = malloc(RECV_BUF_SIZE);
    send_buf = malloc(RECV_BUF_SIZE);

    k_hdr_include(TRUE);    /* include IP header when sending */
    k_set_rcvbuf(256*1024,48*1024); /* lots of input buffering        */
    k_set_ttl(1);       /* restrict multicasts to one hop */
    k_set_loop(FALSE);      /* disable multicast loopback     */

    ip         = (struct ip *)send_buf;
    bzero(ip, sizeof(struct ip));
    /*
     * Fields zeroed that aren't filled in later:
     * - IP ID (let the kernel fill it in)
     * - Offset (we don't send fragments)
     * - Checksum (let the kernel fill it in)
     */
    ip->ip_v   = IPVERSION;
#if(SUPPORT_IGMPV3)
	ip->ip_hl  = (sizeof(struct ip) + IP_RA_OPTION_LEN)>> 2;
#else
    ip->ip_hl  = sizeof(struct ip) >> 2;
#endif
    ip->ip_tos = 0xc0;      /* Internet Control */
    ip->ip_ttl = MAXTTL;    /* applies to unicasts only */
    ip->ip_p   = IPPROTO_IGMP;

    allhosts_group   = htonl(INADDR_ALLHOSTS_GROUP);
    allrouters_group = htonl(INADDR_ALLRTRS_GROUP);
#if (SUPPORT_IGMPV3)
	all_group_igmpv3 = htonl(0xE0000016);
	group_compatibilty_mode = IGMPV3;
	//group_compatibilty_mode = IGMPV1_V2;
#endif
}

/**
*   Finds the textual name of the supplied IGMP request.
*/
char *igmpPacketKind(u_int type, u_int code) {
    static char unknown[20];

    switch (type) {
    case IGMP_MEMBERSHIP_QUERY:     return  "Membership query  ";
    case IGMP_V1_MEMBERSHIP_REPORT:  return "V1 member report  ";
    case IGMP_V2_MEMBERSHIP_REPORT:  return "V2 member report  ";
    case IGMP_V2_LEAVE_GROUP:        return "Leave message     ";
#if (SUPPORT_IGMPV3)
    case IGMP_V3_MEMBERSHIP_REPORT:  return "V3 member report  ";
#endif
    
    default:
        sprintf(unknown, "unk: 0x%02x/0x%02x    ", type, code);
        return unknown;
    }
}


/**
 * Process a newly received IGMP packet that is sitting in the input
 * packet buffer.
 */
void acceptIgmp(int recvlen) {
    register uint32 src, dst, group;
    struct ip *ip;
    struct igmp *igmp;
#ifdef USE_ATH_HEADER
    int port = 0;
#endif
   
    int ipdatalen, iphdrlen, igmpdatalen;
#if (SUPPORT_IGMPV3)
	struct igmpv3_report *igmpv3;
#endif
#if (DNI_MULTI_LAN_SUPPORT)
	struct RouteTable *croute = NULL;
#endif
    if (recvlen < sizeof(struct ip)) {
        igmp_syslog(LOG_WARNING, 0,
            "received packet too short (%u bytes) for IP header", recvlen);
        return;
    }

    ip        = (struct ip *)recv_buf;
    src       = ip->ip_src.s_addr;
    dst       = ip->ip_dst.s_addr;

    //IF_DEBUG igmp_syslog(LOG_DEBUG, 0, "Got a IGMP request to process...");

    /* 
     * this is most likely a message from the kernel indicating that
     * a new src grp pair message has arrived and so, it would be 
     * necessary to install a route into the kernel for this.
     */
    if (ip->ip_p == 0) {
        if (src == 0 || dst == 0) {
            igmp_syslog(LOG_WARNING, 0, "kernel request not accurate");
        }
        else {
            struct IfDesc *checkVIF;
#ifdef USE_ATH_HEADER
	    // if snooping mode,return.
            if(commonConfig.mode==1) return;
#endif
            
            // Check if the source address matches a valid address on upstream vif.
            checkVIF = getIfByIx( upStreamVif );
            if(checkVIF == 0) {
                igmp_syslog(LOG_ERR, 0, "Upstream VIF was null.");
                return;
            } 
            else if(src == checkVIF->InAdr.s_addr) {
                igmp_syslog(LOG_NOTICE, 0, "Route activation request from %s for %s is from myself. Ignoring.",
                    inetFmt(src, s1), inetFmt(dst, s2));
                return;
            }
#if 0 //vicky
            else if(!isAdressValidForIf(checkVIF, src)) {
                igmp_syslog(LOG_WARNING, 0, "The source address %s for group %s, is not in any valid net for upstream VIF.",
                    inetFmt(src, s1), inetFmt(dst, s2));
                return;
            }
#endif
            
            // Activate the route.
            IF_DEBUG igmp_syslog(LOG_DEBUG, 0, "Route activate request from %s to %s",
                         inetFmt(src,s1), inetFmt(dst,s2));
            activateRoute(dst, src);
            

        }
        return;
    }

    iphdrlen  = ip->ip_hl << 2;
    ipdatalen = ntohs(ip->ip_len) - iphdrlen;

    if (iphdrlen + ipdatalen != recvlen) {
        igmp_syslog(LOG_WARNING, 0,
            "received packet from %s shorter (%u bytes) than hdr+data length (%u+%u)",
            inetFmt(src, s1), recvlen, iphdrlen, ipdatalen);
        return;
    }

    igmp        = (struct igmp *)(recv_buf + iphdrlen);
#if (!SUPPORT_IGMPV3)
    group       = igmp->igmp_group.s_addr;
#endif
#ifdef USE_ATH_HEADER
    if ((((char *) (&igmp->igmp_cksum))[0] == 0x7d) 
          && ((((char *) (&igmp->igmp_cksum))[1] & 0xf0) == 0x50))
        port = 1; //(uint32) ((char *) (&igmp->igmp_cksum))[1] & 0xf;
	if (port > (PORT_MAX_NUM+1))
	{
		DPRINTF("%s :: get port info [%d] greater than [%d]\n", __FUNCTION__, port, PORT_MAX_NUM+1);
		port = 0;
	}
#endif

#if (SUPPORT_IGMPV3)
	if(igmp->igmp_type == IGMP_V3_MEMBERSHIP_REPORT)
	{
		igmpdatalen = ipdatalen - IGMPV3_MINLEN;
	}
	else
	{
		igmpdatalen = ipdatalen - IGMP_MINLEN;
	}
#else
    igmpdatalen = ipdatalen - IGMP_MINLEN;
#endif
    if (igmpdatalen < 0) {
        igmp_syslog(LOG_WARNING, 0,
            "received IP data field too short (%u bytes) for IGMP, from %s",
            ipdatalen, inetFmt(src, s1));
        return;
    }

    igmp_syslog(LOG_NOTICE, 0, "RECV %s from %-15s to %s",
        igmpPacketKind(igmp->igmp_type, igmp->igmp_code),
        inetFmt(src, s1), inetFmt(dst, s2) );

#if (SUPPORT_IGMPV3)
    switch (igmp->igmp_type) {
    case IGMP_V1_MEMBERSHIP_REPORT:
    case IGMP_V2_MEMBERSHIP_REPORT:
    	group       = igmp->igmp_group.s_addr;
#ifdef USE_ATH_HEADER
        acceptGroupReport(port,src, group, igmp->igmp_type);
#else
        acceptGroupReport(src, group, igmp->igmp_type);
#endif
#if (DNI_MULTI_LAN_SUPPORT)
	if ((croute = findRoute(group)))
		update_all_ports(croute);
#endif
        return;
            
    case IGMP_V2_LEAVE_GROUP:
    	group       = igmp->igmp_group.s_addr;
        acceptLeaveMessage(src, group);
        return;

	case IGMP_V3_MEMBERSHIP_REPORT:
#if (DNI_MULTI_LAN_SUPPORT)
		processV3report(recv_buf + iphdrlen, src, port);
#else
		igmpv3       = (struct igmpv3_report *)(recv_buf + iphdrlen);
		group		 = igmpv3->grec[0].grec_mca;

		if (igmpv3->grec[0].grec_type == IGMPV3_CHANGE_TO_EXCLUDE)
		{
#ifdef USE_ATH_HEADER
			acceptGroupReport(port, src, group, igmp->igmp_type);
#else
			acceptGroupReport(src, group, igmp->igmp_type);
#endif
		}
		else if (igmpv3->grec[0].grec_type == IGMPV3_CHANGE_TO_INCLUDE)
		{
			acceptLeaveMessage(src, group);
		}
		else
		{
			int num_grec=0, pgr_data_len=0;
			struct igmpv3_grec *pgr = 0;

			for( num_grec = ntohs(igmpv3->ngrec); num_grec > 0; num_grec--)
			{
				pgr = (struct igmpv3_grec *)(recv_buf + iphdrlen + IGMP_MINLEN + pgr_data_len);

				if (pgr->grec_type == IGMPV3_MODE_IS_EXCLUDE)
				{
#ifdef USE_ATH_HEADER
					acceptGroupReport(port, src, pgr->grec_mca, igmp->igmp_type);
#else
					acceptGroupReport(src, pgr->grec_mca, igmp->igmp_type);
#endif
				}
				else if (pgr->grec_type == IGMPV3_MODE_IS_INCLUDE)
				{
					acceptLeaveMessage(src, pgr->grec_mca);
				}
				pgr_data_len = ((ntohs(igmpv3->ngrec) - num_grec + 1) * sizeof(struct igmpv3_grec)) - (ntohs(pgr->grec_nsrcs) * sizeof(struct in_addr));
			}
		}
#endif
        return;
    
    case IGMP_MEMBERSHIP_QUERY:
	if(strcmp(config_get("enable_bt_igmp"), "1") == 0) {
		group = igmp->igmp_group.s_addr;
		if(group == 0x0 && src != get_if_ip(LAN_IF))
			accept_membership_query(src, dst, group, igmp->igmp_code);
	}
        //accept_membership_query(src, dst, group, igmp->igmp_code);
        return;

    default:
        igmp_syslog(LOG_INFO, 0,
            "ignoring unknown IGMP message type %x from %s to %s",
            igmp->igmp_type, inetFmt(src, s1),
            inetFmt(dst, s2));
        return;
    }

#else
    switch (igmp->igmp_type) {
    case IGMP_V1_MEMBERSHIP_REPORT:
    case IGMP_V2_MEMBERSHIP_REPORT:
#ifdef USE_ATH_HEADER
		acceptGroupReport(port, src, group, igmp->igmp_type);
#else
		acceptGroupReport(src, group, igmp->igmp_type);
#endif
        return;
            
    case IGMP_V2_LEAVE_GROUP:
		acceptLeaveMessage(src, group);        
        return;
    
        /*
    case IGMP_MEMBERSHIP_QUERY:
        //accept_membership_query(src, dst, group, igmp->igmp_code);
        return;

    */

    default:
        igmp_syslog(LOG_INFO, 0,
            "ignoring unknown IGMP message type %x from %s to %s",
            igmp->igmp_type, inetFmt(src, s1),
            inetFmt(dst, s2));
        return;
    }
#endif
}


/*
 * Construct an IGMP message in the output packet buffer.  The caller may
 * have already placed data in that buffer, of length 'datalen'.
 */
void buildIgmp(uint32 src, uint32 dst, int type, int code, uint32 group, int datalen) {
    struct ip *ip;
    struct igmp *igmp;
    extern int curttl;

    ip                      = (struct ip *)send_buf;
    ip->ip_src.s_addr       = src;
    ip->ip_dst.s_addr       = dst;
    ip->ip_len              = MIN_IP_HEADER_LEN + IGMP_MINLEN + datalen;
    ip->ip_len              = htons(ip->ip_len);

    if (IN_MULTICAST(ntohl(dst))) {
        ip->ip_ttl = curttl;
    } else {
        ip->ip_ttl = MAXTTL;
    }
    ip->ip_hl  = sizeof(struct ip) >> 2;	/* 20 octet*/

	igmp                    = (struct igmp *)(send_buf + MIN_IP_HEADER_LEN);
    igmp->igmp_type         = type;
    igmp->igmp_code         = code;
    igmp->igmp_group.s_addr = group;

    igmp->igmp_cksum        = 0;
    igmp->igmp_cksum        = inetChksum((u_short *)igmp,
                                         IGMP_MINLEN + datalen);
}

#if (SUPPORT_IGMPV3)
void buildIgmpv3(uint32 src, uint32 dst, int type, int code, uint32 group, int datalen) {
    struct ip *ip;
 	struct igmpv3_query *igmp;
    extern int curttl;
	//char ra_space[4]; /* IP options: Router Alert */


    ip                      = (struct ip *)send_buf;
    ip->ip_src.s_addr       = src;
    ip->ip_dst.s_addr       = dst;
    ip->ip_len              = MIN_IP_HEADER_LEN + IP_RA_OPTION_LEN + IGMPV3_MINLEN + datalen;
    ip->ip_len              = htons(ip->ip_len);
	/* IP options: Router Alert */
	((unsigned char*)&ip[1])[0] = IPOPT_RA;
	((unsigned char*)&ip[1])[1] = 4;
	((unsigned char*)&ip[1])[2] = 0;
	((unsigned char*)&ip[1])[3] = 0;
	
    if (IN_MULTICAST(ntohl(dst))) {
        ip->ip_ttl = curttl;
    } else {
        ip->ip_ttl = MAXTTL;
    }

    ip->ip_hl  = (sizeof(struct ip) + IP_RA_OPTION_LEN)>> 2;	
	
    igmp                    = (struct igmpv3_query *)(send_buf + MIN_IP_HEADER_LEN + IP_RA_OPTION_LEN);
    igmp->type         = type;
    igmp->code         = code;
    igmp->group		= group;

	igmp->qqic				= INTERVAL_QUERY;
	igmp->nsrcs				= 0; 
	igmp->qrv				= 2; 

    igmp->csum        = 0;
    igmp->csum        = inetChksum((u_short *)igmp,
                                         IGMPV3_MINLEN + datalen);
}

void buildIgmpv3_report(uint32 src, uint32 dst, int type, uint32 group, int datalen) {
    struct ip *ip;
	struct igmpv3_report *igmp;
    extern int curttl;
	//char ra_space[4]; /* IP options: Router Alert */


    ip                      = (struct ip *)send_buf;
    ip->ip_src.s_addr       = src;
    ip->ip_dst.s_addr       = dst;
    ip->ip_len              = MIN_IP_HEADER_LEN + IP_RA_OPTION_LEN + IGMPV3_MINLEN + datalen;
    ip->ip_len              = htons(ip->ip_len);
	/* IP options: Router Alert */
	((unsigned char*)&ip[1])[0] = IPOPT_RA;
	((unsigned char*)&ip[1])[1] = 4;
	((unsigned char*)&ip[1])[2] = 0;
	((unsigned char*)&ip[1])[3] = 0;

    if (IN_MULTICAST(ntohl(dst))) {
        ip->ip_ttl = curttl;
    } else {
        ip->ip_ttl = MAXTTL;
    }

    ip->ip_hl  = (sizeof(struct ip) + IP_RA_OPTION_LEN)>> 2;

    igmp                    = (struct igmpv3_report *)(send_buf + MIN_IP_HEADER_LEN + IP_RA_OPTION_LEN);
    igmp->type         = type;
	igmp->resv1 = 0;
	igmp->resv2 = 0;
	igmp->ngrec = htons(1);
    igmp->grec->grec_mca		= group;
	igmp->grec->grec_type = IGMPV3_CHANGE_TO_EXCLUDE;
	igmp->grec->grec_auxwords = 0;
	igmp->grec->grec_nsrcs = 0;

    igmp->csum        = 0;
    igmp->csum        = inetChksum((u_short *)igmp,
                                         IGMPV3_MINLEN + datalen);
}
#endif

/* 
 * Call build_igmp() to build an IGMP message in the output packet buffer.
 * Then send the message from the interface with IP address 'src' to
 * destination 'dst'.
 */
void sendIgmp(uint32 src, uint32 dst, int type, int code, uint32 group, int datalen) {
    struct sockaddr_in sdst;
    int setloop = 0, setigmpsource = 0;

#if (SUPPORT_IGMPV3)
	int igmp_min_len = 0;

	if(group_compatibilty_mode == IGMPV3)
	{
		igmp_min_len = IGMPV3_MINLEN;
		if (type == IGMP_V3_MEMBERSHIP_REPORT)
		{
			buildIgmpv3_report(src, dst, type, group, datalen);
		}
		else
			buildIgmpv3(src, dst, type, code, group, datalen);
	}
	else
	{	
		igmp_min_len = IGMP_MINLEN;
		buildIgmp(src, dst, type, code, group, datalen);
	}
#else
    buildIgmp(src, dst, type, code, group, datalen);
#endif

    if (IN_MULTICAST(ntohl(dst))) {
        k_set_if(src);
        setigmpsource = 1;
        if (type != IGMP_DVMRP || dst == allhosts_group) {
            setloop = 1;
            k_set_loop(TRUE);
        }
    }

    bzero(&sdst, sizeof(sdst));
    sdst.sin_family = AF_INET;
#ifdef HAVE_SA_LEN
    sdst.sin_len = sizeof(sdst);
#endif
    sdst.sin_addr.s_addr = dst;
    
#if (SUPPORT_IGMPV3)
    int ret = 0;	
    if(group_compatibilty_mode == IGMPV3)
	ret = sendto(MRouterFD, send_buf,
               MIN_IP_HEADER_LEN + IP_RA_OPTION_LEN + igmp_min_len + datalen, 0,
               (struct sockaddr *)&sdst, sizeof(sdst));
    else 
    	ret = sendto(MRouterFD, send_buf,
               MIN_IP_HEADER_LEN + IGMP_MINLEN + datalen, 0,
               (struct sockaddr *)&sdst, sizeof(sdst));
    if (ret < 0) {
#else
    if (sendto(MRouterFD, send_buf,
               MIN_IP_HEADER_LEN + IGMP_MINLEN + datalen, 0,
               (struct sockaddr *)&sdst, sizeof(sdst)) < 0) {
#endif
        if (errno == ENETDOWN)
            igmp_syslog(LOG_ERR, errno, "Sender VIF was down.");
        else
            igmp_syslog(LOG_INFO, errno,
                "sendto to %s on %s",
                inetFmt(dst, s1), inetFmt(src, s2));
    }

    if(setigmpsource) {
        if (setloop) {
            k_set_loop(FALSE);
        }
        // Restore original...
        k_set_if(INADDR_ANY);
    }

    IF_DEBUG igmp_syslog(LOG_DEBUG, 0, "SENT %s from %-15s to %s",
        igmpPacketKind(type, code), src == INADDR_ANY ? "INADDR_ANY" :
        inetFmt(src, s1), inetFmt(dst, s2));
}

