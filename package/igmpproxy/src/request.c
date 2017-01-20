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
*   request.c 
*
*   Functions for recieveing and processing IGMP requests.
*
*/

#include "defs.h"

#ifdef USE_ATH_HEADER
static int startupQueryCount = 0;
#endif
// Prototypes...
void sendGroupSpecificMemberQuery(void *argument);  
    
typedef struct {
    uint32      group;
    uint32      vifAddr;
    short       started;
} GroupVifDesc;

#if (SUPPURT_MCAST_TO_UNICAST)
inline int
join_group(unsigned long src, unsigned long group)
{
	char line[64];
	sprintf(line, "echo \"a %d.%d.%d.%d %d.%d.%d.%d\" > /proc/mcast", 
		NIPQUAD(src), NIPQUAD(group));
//	DPRINTF("%s\n",line);
	system(line);
	return 0;
}
#endif


/**
*   Handles incoming membership reports, and
*   appends them to the routing table.
*/
#ifdef USE_ATH_HEADER
void acceptGroupReport(uint32 port, uint32 src, uint32 group, uint8 type) {
#else
void acceptGroupReport(uint32 src, uint32 group, uint8 type) {
#endif
    struct IfDesc  *sourceVif;

    // Sanitycheck the group adress...
    if(!IN_MULTICAST( ntohl(group) )) {
        igmp_syslog(LOG_WARNING, 0, "The group address %s is not a valid Multicast group.",
            inetFmt(group, s1));
        return;
    }
	// add by Kane, skipped control group
	// ex. 224.0.0.x or 239.255.255.250
	if (mc_ctrl_packet(ntohl(group))) {
//		DPRINTF("%s :: skipped group [%x]\n", __FUNCTION__, group);
		return ;
	}
#ifdef USE_ATH_HEADER
    /* Bug 755: in other mode, if the bridge ip address and the frame from downsteam 
     * is not in the same network, all the join request will be accepted.
     * We don't care the source ip address in the same network or not.  */
// we do not accept the frame from downsteam is not in the same network to move the code segments to the insertRoute function
/*
	if(commonConfig.mode&0x01){
        	IF_DEBUG atlog(LOG_DEBUG, 0,
                     "Should insert group %s port %d to fdb table.",
                     inetFmt(group, s1),port);
        	if (port > 0) 
			insertFdb(group, port);
	}
*/
#endif
    // Find the interface on which the report was recieved.
    sourceVif = getIfByAddress( src );
    if(sourceVif == NULL) {
        igmp_syslog(LOG_WARNING, 0, "No interfaces found for source %s",
            inetFmt(src,s1));
        return;
    }

    if(sourceVif->InAdr.s_addr == src) {
        igmp_syslog(LOG_NOTICE, 0, "The IGMP message was from myself. Ignoring.");
        return;
    }

    // We have a IF so check that it's an downstream IF.
    if(sourceVif->state == IF_STATE_DOWNSTREAM) {
#ifdef USE_ATH_HEADER
		// The membership report was OK... Insert it into the route table..
	if(commonConfig.mode&0x02){
#endif
        IF_DEBUG igmp_syslog(LOG_DEBUG, 0, "Should insert group %s (from: %s) to route table. Vif Ix : %d",
            inetFmt(group,s1), inetFmt(src,s2), sourceVif->index);

#if (SUPPORT_IGMPV3)
		if ((type == IGMP_V1_MEMBERSHIP_REPORT) || (type == IGMP_V2_MEMBERSHIP_REPORT))
		{
    			group_compatibilty_mode = IGMPV1_V2;
		}
#endif

        // The membership report was OK... Insert it into the route table..
#if (IGMP_IMMEDIATE_LEAVE)
#ifdef USE_ATH_HEADER
		insertRoute(port, group, sourceVif->index, src);
#else
		insertRoute(group, sourceVif->index, src);
#endif
#else
        insertRoute(group, sourceVif->index);
#endif
#if (SUPPURT_MCAST_TO_UNICAST)
		join_group(src, group);
#endif
#ifdef USE_ATH_HEADER
	}
#endif
    } else {
        // Log the state of the interface the report was recieved on.
        igmp_syslog(LOG_INFO, 0, "Mebership report was recieved on %s. Ignoring.",
            sourceVif->state==IF_STATE_UPSTREAM?"the upstream interface":"a disabled interface");
    }

}

/**
*   Recieves and handles a group leave message.
*/
void acceptLeaveMessage(uint32 src, uint32 group) {
    struct IfDesc   *sourceVif;
    
    IF_DEBUG igmp_syslog(LOG_DEBUG, 0, "Got leave message from %s to %s. Starting last member detection.",
                 inetFmt(src, s1), inetFmt(group, s2));

    // Sanitycheck the group adress...
    if(!IN_MULTICAST( ntohl(group) )) {
        igmp_syslog(LOG_WARNING, 0, "The group address %s is not a valid Multicast group.",
            inetFmt(group, s1));
        return;
    }
	// add by Kane, skipped control group
	// ex. 224.0.0.x or 239.255.255.250
	if (mc_ctrl_packet(ntohl(group))) {
		return ;
	}
    // Find the interface on which the report was recieved.
    sourceVif = getIfByAddress( src );
    if(sourceVif == NULL) {
        igmp_syslog(LOG_WARNING, 0, "No interfaces found for source %s",
            inetFmt(src,s1));
        return;
    }

    // We have a IF so check that it's an downstream IF.
    if(sourceVif->state == IF_STATE_DOWNSTREAM) {

        GroupVifDesc   *gvDesc;
        gvDesc = (GroupVifDesc*) malloc(sizeof(GroupVifDesc));

        // Tell the route table that we are checking for remaining members...
#if (IGMP_IMMEDIATE_LEAVE)
        setRouteLastMemberMode(group, src);
#else
        setRouteLastMemberMode(group);
#endif
        // Call the group spesific membership querier...
        gvDesc->group = group;
        gvDesc->vifAddr = sourceVif->InAdr.s_addr;
        gvDesc->started = 0;

// modified by kane, to make igmp snooping in realtek board work smoothly
#if (!IGMP_IMMEDIATE_LEAVE)
        sendGroupSpecificMemberQuery(gvDesc);
#endif

    } else {
        // just ignore the leave request...
        IF_DEBUG igmp_syslog(LOG_DEBUG, 0, "The found if for %s was not downstream. Ignoring leave request.");
    }
}

/**
*   Sends a group specific member report query until the 
*   group times out...
*/
void sendGroupSpecificMemberQuery(void *argument) {
    struct  Config  *conf = getCommonConfig();

    // Cast argument to correct type...
    GroupVifDesc   *gvDesc = (GroupVifDesc*) argument;

    if(gvDesc->started) {
        // If aging returns false, we don't do any further action...
        if(!lastMemberGroupAge(gvDesc->group)) {
            return;
        }
    } else {
        gvDesc->started = 1;
    }

    // Send a group specific membership query...
    sendIgmp(gvDesc->vifAddr, gvDesc->group, 
             IGMP_MEMBERSHIP_QUERY,
             conf->lastMemberQueryInterval * IGMP_TIMER_SCALE, 
             gvDesc->group, 0);

    IF_DEBUG igmp_syslog(LOG_DEBUG, 0, "Sent membership query from %s to %s. Delay: %d",
        inetFmt(gvDesc->vifAddr,s1), inetFmt(gvDesc->group,s2),
        conf->lastMemberQueryInterval);

    // Set timeout for next round...
    timer_setTimer(conf->lastMemberQueryInterval, sendGroupSpecificMemberQuery, gvDesc);

}

#ifdef USE_ATH_HEADER
void setStartCount()
{
    struct Config *conf = getCommonConfig();
    startupQueryCount = conf->startupQueryCount;
}
#endif

/**
*   Sends a general membership query on downstream VIFs
*/
#ifndef USE_ATH_HEADER
extern struct RouteTable   *routing_table;
#endif
void sendGeneralMembershipQuery() {
    struct  Config  *conf = getCommonConfig();
    struct  IfDesc  *Dp;
    int             Ix;

    // Loop through all downstream vifs...
    for ( Ix = 0; Dp = getIfByIx( Ix ); Ix++ ) {
        if ( Dp->InAdr.s_addr && ! (Dp->Flags & IFF_LOOPBACK) ) {
            if(Dp->state == IF_STATE_DOWNSTREAM) {
                // Send the membership query...
                sendIgmp(Dp->InAdr.s_addr, allhosts_group, 
                         IGMP_MEMBERSHIP_QUERY,
                         conf->queryResponseInterval * IGMP_TIMER_SCALE, 0, 0);
                
                IF_DEBUG igmp_syslog(LOG_DEBUG, 0, "Sent membership query from %s to %s. Delay: %d",
                    inetFmt(Dp->InAdr.s_addr,s1), inetFmt(allhosts_group,s2),
                    conf->queryResponseInterval);
            }
        }
    }

#ifndef USE_ATH_HEADER
    // Install timer for aging active routes.
    timer_setTimer(conf->queryResponseInterval, ageActiveRoutes, NULL);

    // Install timer for next general query...
    if(conf->startupQueryCount>0) {
        // Use quick timer...
        timer_setTimer(conf->startupQueryInterval, sendGeneralMembershipQuery, NULL);
        // Decrease startup counter...
        conf->startupQueryCount--;
    } 
    else {
        // Use slow timer...
        timer_setTimer(conf->queryInterval, sendGeneralMembershipQuery, NULL);
    }
#endif

}
#define LOCAL_CONTROL_START 0xE0000000
#define LOCAL_CONTROL_END   0XE00000FF
#define SSDP    0XEFFFFFFA
int mc_ctrl_packet(uint32 group)
{
    if (group >= LOCAL_CONTROL_START && group <= LOCAL_CONTROL_END) {
        return 1;
    }
    else if (group == SSDP) {
        return 1;
    }
    return 0;
}

#ifdef USE_ATH_HEADER
/**
*   Sends a general membership query on downstream VIFs
*/
void
igmpTimerHandle()
{
    struct Config *conf = getCommonConfig();

    //if (commonConfig.mode&0x02)
        sendGeneralMembershipQuery();

    // Install timer for aging active routes.
    if (startupQueryCount == 0 || startupQueryCount == conf->startupQueryCount) {
        if (commonConfig.mode&0x02)
            timer_setTimer(conf->queryResponseInterval, ageActiveRoutes, NULL);

//        if (commonConfig.mode&0x01)
//            timer_setTimer(conf->queryResponseInterval, ageActiveFdbs, NULL);   
    }
    
    // Install timer for next general query...
    if (startupQueryCount > 0) {
        // Use quick timer...
        timer_setTimer(conf->startupQueryInterval, igmpTimerHandle,
                       NULL);
        // Decrease startup counter...
        startupQueryCount--;
    } else {   
        // Use slow timer...
        timer_setTimer(conf->queryInterval, igmpTimerHandle, NULL);
    }
} 
#endif

/* This function is used for BT IGMP to receive IGMP Query */
void accept_membership_query(uint32 src, uint32 dst, uint32 group, int code)
{
	struct  IfDesc  *Dp;
	int     Ix;

	if(src != htonl(get_if_ip(LAN_IF))) {
		for ( Ix = 0; Dp = getIfByIx( Ix ); Ix++ ) {
			if ( Dp->InAdr.s_addr && ! (Dp->Flags & IFF_LOOPBACK) ) {
				if(Dp->state == IF_STATE_DOWNSTREAM) {
					// Send the membership query...
					sendIgmp(Dp->InAdr.s_addr, dst, IGMP_MEMBERSHIP_QUERY, code, group, 0);

					IF_DEBUG igmp_syslog(LOG_DEBUG, 0, "Sent membership query from %s to %s.",
							inetFmt(Dp->InAdr.s_addr,s1), inetFmt(dst,s2));
				}
			}
		}
	}
}

