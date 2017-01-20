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
*   mroute-api.c
*
*   This module contains the interface routines to the Linux mrouted API
*/


#define USE_LINUX_IN_H
#include "defs.h"
#ifdef USE_ATH_HEADER
#include <unistd.h>
#endif


// MAX_MC_VIFS from mclab.h must have same value as MAXVIFS from mroute.h
#if MAX_MC_VIFS != MAXVIFS
# error "constants don't match, correct mclab.h"
#endif
     
// need an IGMP socket as interface for the mrouted API
// - receives the IGMP messages
int         MRouterFD;        /* socket for all network I/O  */
char        *recv_buf;           /* input packet buffer         */
char        *send_buf;           /* output packet buffer        */
int	    recv_len;


// my internal virtual interfaces descriptor vector  
static struct VifDesc {
    struct IfDesc *IfDp;
} VifDescVc[ MAXVIFS ];



/*
** Initialises the mrouted API and locks it by this exclusively.
**     
** returns: - 0 if the functions succeeds     
**          - the errno value for non-fatal failure condition
*/
int enableMRouter()
{
    int Va = 1;

    if ( (MRouterFD  = socket(AF_INET, SOCK_RAW, IPPROTO_IGMP)) < 0 )
        igmp_syslog( LOG_ERR, errno, "IGMP socket open" );

    if ( setsockopt( MRouterFD, IPPROTO_IP, MRT_INIT, 
                     (void *)&Va, sizeof( Va ) ) )
        return errno;

    return 0;
}

/*
** Diables the mrouted API and relases by this the lock.
**          
*/
void disableMRouter()
{
    if ( setsockopt( MRouterFD, IPPROTO_IP, MRT_DONE, NULL, 0 ) 
         || close( MRouterFD )
       ) {
        MRouterFD = 0;
        igmp_syslog( LOG_ERR, errno, "MRT_DONE/close" );
    }

    MRouterFD = 0;
}

/*
** Adds the interface '*IfDp' as virtual interface to the mrouted API
** 
*/
void addVIF( struct IfDesc *IfDp )
{
    struct vifctl VifCtl;
    struct VifDesc *VifDp;

    /* search free VifDesc
     */
    for ( VifDp = VifDescVc; VifDp < VCEP( VifDescVc ); VifDp++ ) {
        if ( ! VifDp->IfDp )
            break;
    }

    /* no more space
     */
    if ( VifDp >= VCEP( VifDescVc ) )
        igmp_syslog( LOG_ERR, ENOMEM, "addVIF, out of VIF space" );

    VifDp->IfDp = IfDp;

    VifCtl.vifc_vifi  = VifDp - VifDescVc; 
    VifCtl.vifc_flags = 0;        /* no tunnel, no source routing, register ? */
    VifCtl.vifc_threshold  = VifDp->IfDp->threshold;    // Packet TTL must be at least 1 to pass them
    VifCtl.vifc_rate_limit = VifDp->IfDp->ratelimit;    // Ratelimit

    VifCtl.vifc_lcl_addr.s_addr = VifDp->IfDp->InAdr.s_addr;
    VifCtl.vifc_rmt_addr.s_addr = INADDR_ANY;

    // Set the index...
    VifDp->IfDp->index = VifCtl.vifc_vifi;

    igmp_syslog( LOG_NOTICE, 0, "adding VIF, Ix %d Fl 0x%x IP 0x%08x %s, Threshold: %d, Ratelimit: %d", 
         VifCtl.vifc_vifi, VifCtl.vifc_flags,  VifCtl.vifc_lcl_addr.s_addr, VifDp->IfDp->Name,
         VifCtl.vifc_threshold, VifCtl.vifc_rate_limit);

    IF_DEBUG {
        struct SubnetList *currSubnet;
        for(currSubnet = IfDp->allowednets; currSubnet; currSubnet = currSubnet->next) {
            igmp_syslog(LOG_DEBUG, 0, "        Network for [%s] : %s",
                IfDp->Name,
                inetFmts(currSubnet->subnet_addr, currSubnet->subnet_mask, s1));
        }
    }

    if ( setsockopt( MRouterFD, IPPROTO_IP, MRT_ADD_VIF, 
                     (char *)&VifCtl, sizeof( VifCtl ) ) )
        igmp_syslog( LOG_ERR, errno, "MRT_ADD_VIF" );

}

/*
** Adds the multicast routed '*Dp' to the kernel routes
**
** returns: - 0 if the function succeeds
**          - the errno value for non-fatal failure condition
*/
int addMRoute( struct MRouteDesc *Dp )
{
    struct mfcctl CtlReq;

    CtlReq.mfcc_origin    = Dp->OriginAdr;
    CtlReq.mfcc_mcastgrp  = Dp->McAdr;
    CtlReq.mfcc_parent    = Dp->InVif;

    /* copy the TTL vector
     */
    if (    sizeof( CtlReq.mfcc_ttls ) != sizeof( Dp->TtlVc ) 
            || VCMC( CtlReq.mfcc_ttls ) != VCMC( Dp->TtlVc )
       )
        igmp_syslog( LOG_ERR, 0, "data types doesn't match in " __FILE__ ", source adaption needed !" );

    memcpy( CtlReq.mfcc_ttls, Dp->TtlVc, sizeof( CtlReq.mfcc_ttls ) );

    {
        char FmtBuO[ 32 ], FmtBuM[ 32 ];

        igmp_syslog( LOG_NOTICE, 0, "Adding MFC: %s -> %s, InpVIf: %d", 
             fmtInAdr( FmtBuO, CtlReq.mfcc_origin ), 
             fmtInAdr( FmtBuM, CtlReq.mfcc_mcastgrp ),
             CtlReq.mfcc_parent == ALL_VIFS ? -1 : CtlReq.mfcc_parent
           );
    }

    if ( setsockopt( MRouterFD, IPPROTO_IP, MRT_ADD_MFC,
                     (void *)&CtlReq, sizeof( CtlReq ) ) )
        igmp_syslog( LOG_WARNING, errno, "MRT_ADD_MFC" );

        return 0;
}

/*
** Removes the multicast routed '*Dp' from the kernel routes
**
** returns: - 0 if the function succeeds
**          - the errno value for non-fatal failure condition
*/
int delMRoute( struct MRouteDesc *Dp )
{
    struct mfcctl CtlReq;

    CtlReq.mfcc_origin    = Dp->OriginAdr;
    CtlReq.mfcc_mcastgrp  = Dp->McAdr;
    CtlReq.mfcc_parent    = Dp->InVif;

    /* clear the TTL vector
     */
    memset( CtlReq.mfcc_ttls, 0, sizeof( CtlReq.mfcc_ttls ) );

    {
        char FmtBuO[ 32 ], FmtBuM[ 32 ];

        igmp_syslog( LOG_NOTICE, 0, "Removing MFC: %s -> %s, InpVIf: %d", 
             fmtInAdr( FmtBuO, CtlReq.mfcc_origin ), 
             fmtInAdr( FmtBuM, CtlReq.mfcc_mcastgrp ),
             CtlReq.mfcc_parent == ALL_VIFS ? -1 : CtlReq.mfcc_parent
           );
    }

    if ( setsockopt( MRouterFD, IPPROTO_IP, MRT_DEL_MFC,
                     (void *)&CtlReq, sizeof( CtlReq ) ) )
        igmp_syslog( LOG_WARNING, errno, "MRT_DEL_MFC" );
    return 0;
}

/*
** Returns for the virtual interface index for '*IfDp'
**
** returns: - the vitrual interface index if the interface is registered
**          - -1 if no virtual interface exists for the interface 
**          
*/
int getVifIx( struct IfDesc *IfDp )
{
    struct VifDesc *Dp;

    for ( Dp = VifDescVc; Dp < VCEP( VifDescVc ); Dp++ )
        if ( Dp->IfDp == IfDp )
            return Dp - VifDescVc;

    return -1;
}

#if (DNI_MULTI_LAN_SUPPORT)
int add_source_list(uint32 mgroup, uint32 src_ip)
{
	struct ip_data
	{
		unsigned short family;
		unsigned long ip;
	} group, src;
	union
	{
		struct group_req gr;
		struct group_source_req gsr;
	} opt;
	int ret = 0, optlen;

	memset(&opt, 0, sizeof(opt));
	group.family = src.family = AF_INET;
	group.ip = mgroup;
	src.ip = src_ip;
	memcpy(&opt.gsr.gsr_source, &src, 8);
	memcpy(&opt.gsr.gsr_group, &group, 8);
	optlen = sizeof(opt.gsr);
	if ((ret =setsockopt(getMcGroupSock(), IPPROTO_IP, MCAST_JOIN_SOURCE_GROUP, (void *)&opt, optlen)) < 0)
	{
		DPRINTF("%s :: setsockopt : MCAST_JOIN_SOURCE_GROUP error [%d]\n", __FUNCTION__, ret);
		ret = 1;
	}
	return ret;
}

int set_source_list(struct RouteTable *croute)
{
	struct ip_msfilter *imsfp = NULL;
	int i, ret = 0, num = croute->source.num;
	struct IfDesc *vif;

	imsfp = (struct ip_msfilter *)malloc(IP_MSFILTER_SIZE(num));
	if (!imsfp)
	{
		DPRINTF("%s :: out of memory...\n", __FUNCTION__);
		return 1;
	}
	memset (imsfp, 0, IP_MSFILTER_SIZE(num));
	imsfp->imsf_fmode = (croute->mode == st_is_in)?1:0;
	imsfp->imsf_numsrc = num;
	imsfp->imsf_multiaddr = croute->group;
	for (i = 0; i < num; i++)
		imsfp->imsf_slist[i] = croute->source.list[i];

	vif = getIfByIx( upStreamVif );
	imsfp->imsf_interface = htonl(vif->InAdr.s_addr);
	i = IP_MSFILTER_SIZE(num);
	if ((ret = setsockopt(getMcGroupSock(), IPPROTO_IP, IP_MSFILTER, imsfp,i)) < 0 )
	{
		DPRINTF("%s :: setsockopt : IP_MSFILTER error [%d]\n", __FUNCTION__, ret);
		ret = 1;
	}
	free(imsfp);
	return ret;
}
#endif

