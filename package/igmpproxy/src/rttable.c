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
*   rttable.c 
*
*   Updates the routingtable according to 
*     recieved request.
*/

#include "defs.h"



                 
// Keeper for the routing table...
#ifdef USE_ATH_HEADER
static struct RouteTable   *routing_table;
#else
struct RouteTable   *routing_table;
#endif

// Prototypes
void logRouteTable(char *header);
int  internAgeRoute(struct RouteTable*  croute);

// Socket for sending join or leave requests.
int mcGroupSock = 0;

#if (0)
void v2_join_group(int port, uint32 mgroup)
{
	char line[64];
	if (port > 0)
	{
		sprintf(line,"ebtables -I port%d -p ipv4 --ip-dst %u.%u.%u.%u -j ACCEPT", port, NIPQUAD(mgroup));
		system(line);
	}
}

void v2_leave_group(int port, uint32 mgroup)
{
	char line[64];
	if (port > 0)
	{
		sprintf(line,"ebtables -D port%d -p ipv4 --ip-dst %u.%u.%u.%u -j ACCEPT", port, NIPQUAD(mgroup));
		system(line);	
	}
}

void v2_port_update (struct RouteTable *croute, int oldport, int newport)
{
	uint32 port[PORT_MAX_NUM+1];
	memcpy(port, croute->port, sizeof(uint32)*(PORT_MAX_NUM+1));
	port[newport]++;
	if (port[oldport]) port[oldport]--;
	if (port[oldport] == 0)
		v2_leave_group(oldport, croute->group);
	if (port[newport] == 1)
		v2_join_group(newport, croute->group);
	memcpy( croute->port, port, sizeof(uint32)*(PORT_MAX_NUM+1));
}
#endif

#ifdef USE_ATH_HEADER
int internUpdateKernelRoute(struct RouteTable *route, int activate);
static inline void 
update_port(struct RouteTable *croute, uint32 oldport, uint32 newport)
{
	uint32 i, ifbits = 0, port[PORT_MAX_NUM+1];
	memcpy(port, croute->port, sizeof(uint32)*(PORT_MAX_NUM+1));
	port[newport]++;
	if (port[oldport]) port[oldport]--;

	if ((port[newport] == 1) || (port[oldport] == 0))
	{
		DPRINTF("%s :: group [%x] port snoop status changed, to update the fdb table....\n", __FUNCTION__, croute->group);
		for (i = 1; i < PORT_MAX_NUM+1; i++)
		{
			if (port[i])
				ifbits |= (1<<i);
		}
		internUpdateFdb(croute->group, ifbits, 1);
	}
	memcpy( croute->port, port, sizeof(uint32)*(PORT_MAX_NUM+1));
}
#endif
/**
*   Function for retrieving the Multicast Group socket.
*/
int getMcGroupSock() {
    if( ! mcGroupSock ) {
        mcGroupSock = openUdpSocket( INADDR_ANY, 0 );;
    }
    return mcGroupSock;
}
 
/**
*   Initializes the routing table.
*/
void initRouteTable() {
    unsigned Ix;
    struct IfDesc *Dp;

    // Clear routing table...
    routing_table = NULL;

    // Join the all routers group on downstream vifs...
    for ( Ix = 0; Dp = getIfByIx( Ix ); Ix++ ) {
        // If this is a downstream vif, we should join the All routers group...
        if( Dp->InAdr.s_addr && ! (Dp->Flags & IFF_LOOPBACK) && Dp->state == IF_STATE_DOWNSTREAM) {
            IF_DEBUG igmp_syslog(LOG_DEBUG, 0, "Joining all-routers group %s on vif %s",
                         inetFmt(allrouters_group,s1),inetFmt(Dp->InAdr.s_addr,s2));

            joinMcGroup( getMcGroupSock(), Dp, allrouters_group );

#if (SUPPORT_IGMPV3)
            IF_DEBUG igmp_syslog(LOG_DEBUG, 0, "Joining all-routers group %s on vif %s",
                         inetFmt(all_group_igmpv3,s1),inetFmt(Dp->InAdr.s_addr,s2));

            joinMcGroup( getMcGroupSock(), Dp, all_group_igmpv3 );
#endif
        }
    }
}

/**
*   Internal function to send join or leave requests for
*   a specified route upstream...
*/
void sendJoinLeaveUpstream(struct RouteTable* route, int join) {
    struct IfDesc*      upstrIf;
    
    // Get the upstream VIF...
    upstrIf = getIfByIx( upStreamVif );
    if(upstrIf == NULL) {
        igmp_syslog(LOG_ERR, 0 ,"FATAL: Unable to get Upstream IF.");
    }
    
    IF_DEBUG {
        igmp_syslog(LOG_DEBUG, 0, "Upstream IF addr  : %s", inetFmt(upstrIf->InAdr.s_addr,s1));
        igmp_syslog(LOG_DEBUG, 0, "Upstream IF state : %d", upstrIf->state);
        igmp_syslog(LOG_DEBUG, 0, "Upstream IF index : %d", upstrIf->index);
    }

    // Send join or leave request...
    if(join) {

        // Only join a group if there are listeners downstream...
        if(route->vifBits > 0) {
            IF_DEBUG igmp_syslog(LOG_DEBUG, 0, "Joining group %s upstream on IF address %s",
                         inetFmt(route->group, s1), 
                         inetFmt(upstrIf->InAdr.s_addr, s2));

            //k_join(route->group, upstrIf->InAdr.s_addr);
            joinMcGroup( getMcGroupSock(), upstrIf, route->group );

            route->upstrState = ROUTESTATE_JOINED;
        } else IF_DEBUG {
            igmp_syslog(LOG_DEBUG, 0, "No downstream listeners for group %s. No join sent.",
                inetFmt(route->group, s1));
        }

    } else {
        // Only leave if group is not left already...
        if(route->upstrState != ROUTESTATE_NOTJOINED) {
            IF_DEBUG igmp_syslog(LOG_DEBUG, 0, "Leaving group %s upstream on IF address %s",
                         inetFmt(route->group, s1), 
                         inetFmt(upstrIf->InAdr.s_addr, s2));
            
			leaveMcGroup( getMcGroupSock(), upstrIf, route->group );

            route->upstrState = ROUTESTATE_NOTJOINED;
        }
    }
}

/**
*   Clear all routes from routing table, and alerts Leaves upstream.
*/
void clearAllRoutes() {
    struct RouteTable   *croute, *remainroute;

    // Loop through all routes...
    for(croute = routing_table; croute; croute = remainroute) {

        remainroute = croute->nextroute;

        // Log the cleanup in debugmode...
        IF_DEBUG igmp_syslog(LOG_DEBUG, 0, "Removing route entry for %s",
                     inetFmt(croute->group, s1));

        // Uninstall current route
        if(!internUpdateKernelRoute(croute, 0)) {
            igmp_syslog(LOG_WARNING, 0, "The removal from Kernel failed.");
        }

        // Send Leave message upstream.
        sendJoinLeaveUpstream(croute, 0);

        // Clear memory, and set pointer to next route...
        free(croute);
    }
    routing_table = NULL;

    // Send a notice that the routing table is empty...
    igmp_syslog(LOG_NOTICE, 0, "All routes removed. Routing table is empty.");
}
                 
/**
*   Private access function to find a route from a given 
*   Route Descriptor.
*/
struct RouteTable *findRoute(uint32 group) {
    struct RouteTable*  croute;

    for(croute = routing_table; croute; croute = croute->nextroute) {
        if(croute->group == group) {
            return croute;
        }
    }

    return NULL;
}

#if (SUPPURT_MCAST_TO_UNICAST)
static inline int
leave_group(unsigned long src, unsigned long group)
{
	char line[64];
	sprintf(line, "echo \"d %d.%d.%d.%d %d.%d.%d.%d\" > /proc/mcast", 
		NIPQUAD(src), NIPQUAD(group));
//	DPRINTF("%s\n",line);
	system(line);
	return 0;
}
#endif

#if (IGMP_IMMEDIATE_LEAVE)
void update_listener(struct RouteTable*  croute)
{
	struct Listener *listener;
	struct Listener *nlistener = NULL;
	uint32 now = GetSysUpTime();
	listener = croute->listeners;

	while (listener)
	{
		nlistener = listener->nextlistener;
		if (now - listener->timeout >= IGMP_MBR_TIMEOUT)
			setRouteLastMemberMode(croute->group, listener->srcAddr);
		listener = nlistener;
	}
}

void check_mbr_timeout(void)
{
	struct RouteTable*  croute = routing_table;
	struct RouteTable*  nroute = NULL;

	while(croute)
	{
		nroute = croute->nextroute;
		update_listener(croute);
		croute = nroute;
	}

	timer_setTimer( INTERVAL_QUERY_RESPONSE, check_mbr_timeout, NULL);
}

struct Listener *findListener(struct RouteTable*  croute, uint32 src) 
{
	struct Listener*  listener;
	
    for(listener = croute->listeners; listener; listener = listener->nextlistener) {
        if(listener->srcAddr == src) {
            listener->timeout = GetSysUpTime();
            return listener;
        }
    }
    
    return NULL;
}
#ifdef USE_ATH_HEADER
struct Listener* insertListener(uint32 port, struct RouteTable*  croute, uint32 src) {
#else
void insertListener(struct RouteTable*  croute, uint32 src) {
#endif
	struct Listener*  newlistener;
	
	newlistener = (struct Listener*)malloc(sizeof(struct Listener));
	newlistener->nextlistener = NULL;
	newlistener->srcAddr = src;
	newlistener->timeout = GetSysUpTime();
#ifdef USE_ATH_HEADER
	newlistener->port = port;
#if !(DNI_MULTI_LAN_SUPPORT)
	update_port(croute, 0, port);
#endif
#endif
	if(croute->listeners == NULL)
	{
		croute->listeners = newlistener;
	}
	else
	{
	    struct Listener*  listener;

	    listener = croute->listeners;
	
		for(listener = croute->listeners; listener; listener = listener->nextlistener) 
		{
			if(listener->nextlistener == NULL)
			{
				listener->nextlistener = newlistener;
				break;
			}
		}
	}
#if (DNI_MULTI_LAN_SUPPORT)&&(SUPPORT_IGMPV3)
	newlistener->mode = st_is_ex;
	newlistener->version = IGMP_V2_MEMBERSHIP_REPORT;
	memcpy(&newlistener->source, &ZEROSET, sizeof(ZEROSET));
#endif
	return newlistener;
}

int removeListener(struct RouteTable*  croute, uint32 src) 
{
    struct Listener*  listener;
	struct Listener*  prevlistener;
    int result = 0;
    
	if(croute->listeners == NULL)
		return 0;	

	
	prevlistener = NULL;
	listener = croute->listeners;
    while(listener != NULL)
    {
		if(listener->srcAddr == src) 
            break;

      	prevlistener = listener;
      	listener = listener->nextlistener;
    }
// modified by Kane
// to avoid some abnormal conditions
// if client A joined the group G and client B leaved the group G without joining,
// the igmpproxy will be crashed
    if (listener == NULL)
    {
        DPRINTF("%s :: listener is null ... group [%x] has entry [%d]\n", __FUNCTION__,
			croute->group, (croute->listeners)?1:0);
        if (croute->listeners) // there are some clients joined the group
            return 1;
        else
            return 0; // there is no client joined the group
    }
#ifdef USE_ATH_HEADER
#if !(DNI_MULTI_LAN_SUPPORT)
	update_port(croute, listener->port, 0);
#endif
#endif
    // Free the memory
    if((prevlistener == NULL) && (listener->nextlistener == NULL))
    {
    	 croute->listeners = NULL;
    }
    else if((prevlistener == NULL) && (listener->nextlistener != NULL))
    {
    	 croute->listeners = listener->nextlistener;
        result = 1;
    }
    else
    {
         prevlistener->nextlistener = listener->nextlistener;
         result = 1;
    }
	
	free(listener);
	return result;
}

#endif

/**
*   Adds a specified route to the routingtable.
*   If the route already exists, the existing route 
*   is updated...
*/
#if (IGMP_IMMEDIATE_LEAVE)
#ifdef USE_ATH_HEADER
int insertRoute(uint32 port, uint32 group, int ifx, uint32 src) {
#else
int insertRoute(uint32 group, int ifx, uint32 src) {
#endif
#else
int insertRoute(uint32 group, int ifx) {
#endif
    
    struct Config *conf = getCommonConfig();
    struct RouteTable*  croute;
    //int result = 1;
#ifdef USE_ATH_HEADER
	int i;
#endif
    // Sanitycheck the group adress...
    if( ! IN_MULTICAST( ntohl(group) )) {
        igmp_syslog(LOG_WARNING, 0, "The group address %s is not a valid Multicast group. Table insert failed.",
            inetFmt(group, s1));
        return 0;
    }

    // Santiycheck the VIF index...
    //if(ifx < 0 || ifx >= MAX_MC_VIFS) {
    if(ifx >= MAX_MC_VIFS) {
        igmp_syslog(LOG_WARNING, 0, "The VIF Ix %d is out of range (0-%d). Table insert failed.",ifx,MAX_MC_VIFS);
        return 0;
    }

    // Try to find an existing route for this group...
    croute = findRoute(group);
    if(croute==NULL) {
        struct RouteTable*  newroute;

        IF_DEBUG igmp_syslog(LOG_DEBUG, 0, "No existing route for %s. Create new.",
                     inetFmt(group, s1));


        // Create and initialize the new route table entry..
        newroute = (struct RouteTable*)malloc(sizeof(struct RouteTable));
        // Insert the route desc and clear all pointers...
        newroute->group      = group;
        newroute->originAddr = 0;
        newroute->nextroute  = NULL;
        newroute->prevroute  = NULL;

        // The group is not joined initially.
        newroute->upstrState = ROUTESTATE_NOTJOINED;

        // The route is not active yet, so the age is unimportant.
        newroute->ageValue    = conf->robustnessValue;
        newroute->ageActivity = 0;
#ifdef USE_ATH_HEADER
	for (i = 0; i <= PORT_MAX_NUM; i++)
		newroute->port[i] = 0;
	newroute->port[port]++;
#endif
#if (DNI_MULTI_LAN_SUPPORT)&&(SUPPORT_IGMPV3)
	memset(&newroute->source, &ZEROSET, sizeof(ZEROSET));
	for (i = 0; i <= PORT_MAX_NUM; i++)
	{
		memcpy(&newroute->port_source[i], &ZEROSET, sizeof(ZEROSET));
		newroute->port_source[i].num = PORT_INIT_STATE;
		newroute->port_mode[i] = st_is_in;
	}
	newroute->mode = 0;
#endif
#if (IGMP_IMMEDIATE_LEAVE)
		newroute->listeners = NULL;
		if(src != 0)
		{
			struct Listener*  newlistener;

	        	newlistener = (struct Listener*)malloc(sizeof(struct Listener));
	        	newlistener->nextlistener = NULL;
	        	newlistener->srcAddr = src;
			newroute->listeners = newlistener;
#ifdef USE_ATH_HEADER
			newlistener->port = port;
#endif
#if (DNI_MULTI_LAN_SUPPORT)&&(SUPPORT_IGMPV3)
			newlistener->mode = st_is_ex;
			newlistener->version = IGMP_V2_MEMBERSHIP_REPORT;
			memcpy(&newlistener->source, &ZEROSET, sizeof(ZEROSET));
#endif
		}
#endif
        
        BIT_ZERO(newroute->ageVifBits);     // Initially we assume no listeners.

        // Set the listener flag...
        BIT_ZERO(newroute->vifBits);    // Initially no listeners...
        if(ifx >= 0) {
            BIT_SET(newroute->vifBits, ifx);
        }

        // Check if there is a table already....
        if(routing_table == NULL) {
            // No location set, so insert in on the table top.
            routing_table = newroute;
            IF_DEBUG igmp_syslog(LOG_DEBUG, 0, "No routes in table. Insert at beginning.");
        } else {

            IF_DEBUG igmp_syslog(LOG_DEBUG, 0, "Found existing routes. Find insert location.");

            // Check if the route could be inserted at the beginning...
            if(routing_table->group > group) {
                IF_DEBUG igmp_syslog(LOG_DEBUG, 0, "Inserting at beginning, before route %s",inetFmt(routing_table->group,s1));

                // Insert at beginning...
                newroute->nextroute = routing_table;
                newroute->prevroute = NULL;
                routing_table = newroute;

                // If the route has a next node, the previous pointer must be updated.
                if(newroute->nextroute != NULL) {
                    newroute->nextroute->prevroute = newroute;
                }

            } else {

                // Find the location which is closest to the route.
                for( croute = routing_table; croute->nextroute != NULL; croute = croute->nextroute ) {
                    // Find insert position.
                    if(croute->nextroute->group > group) {
                        break;
                    }
                }

                IF_DEBUG igmp_syslog(LOG_DEBUG, 0, "Inserting after route %s",inetFmt(croute->group,s1));
                
                // Insert after current...
                newroute->nextroute = croute->nextroute;
                newroute->prevroute = croute;
                if(croute->nextroute != NULL) {
                    croute->nextroute->prevroute = newroute; 
                }
                croute->nextroute = newroute;
            }
        }

        // Set the new route as the current...
        croute = newroute;

        // Log the cleanup in debugmode...
        igmp_syslog(LOG_INFO, 0, "Inserted route table entry for %s on VIF #%d",
            inetFmt(croute->group, s1),ifx);
#ifdef USE_ATH_HEADER
	if(commonConfig.mode&0x01){
        	IF_DEBUG atlog(LOG_DEBUG, 0,
                     "Should insert group %s port %d to fdb table.",
                     inetFmt(group, s1),port);
#if !(DNI_MULTI_LAN_SUPPORT)
        	if (port > 0) 
			insertFdb(group, port);
#endif
	}
#endif
    } else if(ifx >= 0) {

        // The route exists already, so just update it.
        BIT_SET(croute->vifBits, ifx);
        
        // Register the VIF activity for the aging routine
        BIT_SET(croute->ageVifBits, ifx);

        // Log the cleanup in debugmode...
        igmp_syslog(LOG_INFO, 0, "Updated route entry for %s on VIF #%d",
            inetFmt(croute->group, s1), ifx);

#if (IGMP_IMMEDIATE_LEAVE)
		if(src != 0)
		{
#ifdef USE_ATH_HEADER
			struct Listener *mbr;
			if((mbr = findListener(croute, src)) == NULL)
			{
				insertListener(port, croute, src);
			}
			else
			{
#if !(DNI_MULTI_LAN_SUPPORT)
				// if we found the client switch the port, try to update the snooping status.
				if(mbr->port != port)
				{
					update_port(croute, mbr->port, port);
					mbr->port = port;
				}
#endif
			}
#else
			if(findListener(croute, src) == NULL)
			{
				insertListener(croute, src);
			}

#endif
			update_listener(croute);
		}
#endif

        // If the route is active, it must be reloaded into the Kernel..
        if(croute->originAddr != 0) {
            // Update route in kernel...
            if(!internUpdateKernelRoute(croute, 1)) {
                igmp_syslog(LOG_WARNING, 0, "The insertion into Kernel failed.");
                return 0;
            }
        }
    }

    // Send join message upstream, if the route has no joined flag...
    if(croute->upstrState != ROUTESTATE_JOINED) {
	// Send Join request upstream
	sendJoinLeaveUpstream(croute, 1);
    }
	//send join report if there are listeners downstream in case some unfriendly video server provider periodically need IGMP report for giving video stream.
    else if ( !strcmp(config_get("enable_bt_igmp"), "1") && croute->originAddr != 0 && croute->vifBits > 0)
    {
	struct IfDesc*      upstrIf;
	upstrIf = getIfByIx( upStreamVif );
	sendIgmp(upstrIf->InAdr.s_addr, all_group_igmpv3, IGMP_V3_MEMBERSHIP_REPORT, 0, group, sizeof(struct igmpv3_grec));
	//sendIgmp(upstrIf->InAdr.s_addr, allrouters_group, IGMP_V2_MEMBERSHIP_REPORT, 0, group, 0); // send IGMPV2 join report
    }

    IF_DEBUG logRouteTable("Insert Route");

    return 1;
}

/**
*   Activates a passive group. If the group is already
*   activated, it's reinstalled in the kernel. If
*   the route is activated, no originAddr is needed.
*/
int activateRoute(uint32 group, uint32 originAddr) {
    struct RouteTable*  croute;
    int result = 0;

    // Find the requested route.
    croute = findRoute(group);
    if(croute == NULL) {
        IF_DEBUG igmp_syslog(LOG_DEBUG, 0, "No table entry for %s [From: %s]. Inserting route.",
            inetFmt(group, s1),inetFmt(originAddr, s2));

        // Insert route, but no interfaces have yet requested it downstream.
#if (IGMP_IMMEDIATE_LEAVE)
#ifdef USE_ATH_HEADER
        insertRoute(0, group, -1, 0);
#else
        insertRoute(group, -1, 0);
#endif
#else
        insertRoute(group, -1);
#endif
        // Retrieve the route from table...
        croute = findRoute(group);
    }

    if(croute != NULL) {
        // If the origin address is set, update the route data.
        if(originAddr > 0) {
            if(croute->originAddr > 0 && croute->originAddr!=originAddr) {
                igmp_syslog(LOG_WARNING, 0, "The origin for route %s changed from %s to %s",
                    inetFmt(croute->group, s1),
                    inetFmt(croute->originAddr, s2),
                    inetFmt(originAddr, s3));
            }
            croute->originAddr = originAddr;
        }

        // Only update kernel table if there are listeners !
        if(croute->vifBits > 0) {
            result = internUpdateKernelRoute(croute, 1);
        }
    }
    IF_DEBUG logRouteTable("Activate Route");

    return result;
}


/**
*   This function loops through all routes, and updates the age 
*   of any active routes.
*/
void ageActiveRoutes() {
    struct RouteTable   *croute, *nroute;
    
    IF_DEBUG igmp_syslog(LOG_DEBUG, 0, "Aging routes in table.");

    // Scan all routes...
    for( croute = routing_table; croute != NULL; croute = nroute ) {
        
        // Keep the next route (since current route may be removed)...
        nroute = croute->nextroute;
        // Run the aging round algorithm.
        if(croute->upstrState != ROUTESTATE_CHECK_LAST_MEMBER) {
            // Only age routes if Last member probe is not active...
            internAgeRoute(croute);
        }
    }
    IF_DEBUG logRouteTable("Age active routes");
}

/**
*   Should be called when a leave message is recieved, to
*   mark a route for the last member probe state.
*/
#if (IGMP_IMMEDIATE_LEAVE)
void setRouteLastMemberMode(uint32 group, uint32 src) {
#else
void setRouteLastMemberMode(uint32 group) {
#endif
    struct Config       *conf = getCommonConfig();
    struct RouteTable   *croute;

    croute = findRoute(group);
    if(croute!=NULL) {
        // Check for fast leave mode...
        if(croute->upstrState == ROUTESTATE_JOINED && conf->fastUpstreamLeave) {
#if (IGMP_IMMEDIATE_LEAVE)
            if( removeListener(croute, src) == 0 )
            {
	            BIT_ZERO(croute->ageVifBits);

	            // No activity was registered within the timelimit, so remove the route.
	            removeRoute(croute);
		}
		else
		{
#if (DNI_MULTI_LAN_SUPPORT)
			update_all_ports(croute);
#endif
			if(croute->listeners == NULL)
			{
				// Send a leave message right away..
				sendJoinLeaveUpstream(croute, 0);            
			}
		}
#else
            // Send a leave message right away..
            sendJoinLeaveUpstream(croute, 0);
#endif
#if (SUPPURT_MCAST_TO_UNICAST)
		leave_group(src, group);
#endif
        }
        // Set the routingstate to Last member check...
#if (!IGMP_IMMEDIATE_LEAVE)
        croute->upstrState = ROUTESTATE_CHECK_LAST_MEMBER;
        // Set the count value for expiring... (-1 since first aging)
        croute->ageValue = conf->lastMemberQueryCount;
#endif

    }
}


/**
*   Ages groups in the last member check state. If the
*   route is not found, or not in this state, 0 is returned.
*/
int lastMemberGroupAge(uint32 group) {
    struct Config       *conf = NULL;
    struct RouteTable   *croute;

    conf = getCommonConfig();

    croute = findRoute(group);
    if(croute!=NULL) {
        if(croute->upstrState == ROUTESTATE_CHECK_LAST_MEMBER) {
            return !internAgeRoute(croute);
        } else {
            return 0;
        }
    }
    return 0;
}

/**
*   Remove a specified route. Returns 1 on success,
*   and 0 if route was not found.
*/
int removeRoute(struct RouteTable*  croute) {
    struct Config       *conf = getCommonConfig();
    int result = 1;
    
    // If croute is null, no routes was found.
    if(croute==NULL) {
        return 0;
    }
#if (IGMP_IMMEDIATE_LEAVE)
    if (croute->listeners)
    {
         struct Listener  *listener, *nextlistener;
         DPRINTF("%s :: to remove entry [%x]\n", __FUNCTION__, croute->group);
	  listener = croute->listeners;
	  nextlistener = listener->nextlistener;
	  while (nextlistener)
	  {
#if (SUPPURT_MCAST_TO_UNICAST)
		leave_group(listener->srcAddr, croute->group);
#endif
	  	free(listener);
		listener = nextlistener;
		nextlistener = listener->nextlistener;
	  }
#if (SUPPURT_MCAST_TO_UNICAST)
	leave_group(listener->srcAddr, croute->group);
#endif
	  free(listener);
	  croute->listeners = NULL;
    }
#if (DNI_MULTI_LAN_SUPPORT)
	update_all_ports(croute);
#endif
#endif
    // Log the cleanup in debugmode...
    IF_DEBUG igmp_syslog(LOG_DEBUG, 0, "Removed route entry for %s from table.",
                 inetFmt(croute->group, s1));

    //BIT_ZERO(croute->vifBits);

    // Uninstall current route from kernel
    if(!internUpdateKernelRoute(croute, 0)) {
        igmp_syslog(LOG_WARNING, 0, "The removal from Kernel failed.");
        result = 0;
    }

    // Send Leave request upstream if group is joined
    if(croute->upstrState == ROUTESTATE_JOINED || 
       (croute->upstrState == ROUTESTATE_CHECK_LAST_MEMBER && !conf->fastUpstreamLeave)) 
    {
        sendJoinLeaveUpstream(croute, 0);
    }

    // Update pointers...
    if(croute->prevroute == NULL) {
        // Topmost node...
        if(croute->nextroute != NULL) {
            croute->nextroute->prevroute = NULL;
        }
        routing_table = croute->nextroute;

    } else {
        croute->prevroute->nextroute = croute->nextroute;
        if(croute->nextroute != NULL) {
            croute->nextroute->prevroute = croute->prevroute;
        }
    }
    // Free the memory, and set the route to NULL...
    free(croute);
    croute = NULL;

    IF_DEBUG logRouteTable("Remove route");

    return result;
}


/**
*   Ages a specific route
*/
int internAgeRoute(struct RouteTable*  croute) {
    struct Config *conf = getCommonConfig();
    int result = 0;

    // Drop age by 1.
    croute->ageValue--;
    // Check if there has been any activity...
    if( croute->ageVifBits > 0 && croute->ageActivity == 0 ) {
        // There was some activity, check if all registered vifs responded.
        if(croute->vifBits == croute->ageVifBits) {
            // Everything is in perfect order, so we just update the route age.
            croute->ageValue = conf->robustnessValue;
            //croute->ageActivity = 0;
        } else {
            // One or more VIF has not gotten any response.
            croute->ageActivity++;
            // Update the actual bits for the route...
            croute->vifBits = croute->ageVifBits;
        }
    } 
    // Check if there have been activity in aging process...
    else if( croute->ageActivity > 0 ) {
        // If the bits are different in this round, we must
        if(croute->vifBits != croute->ageVifBits) {
            // Or the bits together to insure we don't lose any listeners.
            croute->vifBits |= croute->ageVifBits;

            // Register changes in this round as well..
            croute->ageActivity++;
        }
    }

    // If the aging counter has reached zero, its time for updating...
    if(croute->ageValue == 0) {
        // Check for activity in the aging process,
        if(croute->ageActivity>0) {
            
            IF_DEBUG igmp_syslog(LOG_DEBUG, 0, "Updating route after aging : %s",
                         inetFmt(croute->group,s1));
            
            // Just update the routing settings in kernel...
            internUpdateKernelRoute(croute, 1);
    
            // We append the activity counter to the age, and continue...
            croute->ageValue = croute->ageActivity;
            croute->ageActivity = 0;
        } else {

            IF_DEBUG igmp_syslog(LOG_DEBUG, 0, "Removing group %s. Died of old age.",
                         inetFmt(croute->group,s1));

            // No activity was registered within the timelimit, so remove the route.
            removeRoute(croute);
        }
        // Tell that the route was updated...
        result = 1;
    }
    // The aging vif bits must be reset for each round...
    BIT_ZERO(croute->ageVifBits);

    return result;
}

/**
*   Updates the Kernel routing table. If activate is 1, the route
*   is (re-)activated. If activate is false, the route is removed.
*/
int internUpdateKernelRoute(struct RouteTable *route, int activate) {
    struct   MRouteDesc     mrDesc;
    struct   IfDesc         *Dp;
    unsigned                Ix;
    
    if(route->originAddr>0) {

        // Build route descriptor from table entry...
        // Set the source address and group address...
        mrDesc.McAdr.s_addr     = route->group;
        mrDesc.OriginAdr.s_addr = route->originAddr;
        // clear output interfaces 
        memset( mrDesc.TtlVc, 0, sizeof( mrDesc.TtlVc ) );
    
        IF_DEBUG igmp_syslog(LOG_DEBUG, 0, "Vif bits : 0x%08x", route->vifBits);

        // Set the TTL's for the route descriptor...
        for ( Ix = 0; Dp = getIfByIx( Ix ); Ix++ ) {
            if(Dp->state == IF_STATE_UPSTREAM) {
                //IF_DEBUG igmp_syslog(LOG_DEBUG, 0, "Identified VIF #%d as upstream.", Dp->index);
                mrDesc.InVif = Dp->index;
            }
            else if(BIT_TST(route->vifBits, Dp->index)) {
                IF_DEBUG igmp_syslog(LOG_DEBUG, 0, "Setting TTL for Vif %d to %d", Dp->index, Dp->threshold);
                mrDesc.TtlVc[ Dp->index ] = Dp->threshold;
            }
        }
    
        // Do the actual Kernel route update...
        if(activate) {
            // Add route in kernel...
            addMRoute( &mrDesc );
        } else {
            // Delete the route from Kernel...
#ifndef USE_ATH_HEADER
            mrDesc.OriginAdr.s_addr = 0;
#endif
            delMRoute( &mrDesc );
        }

    } else {
        igmp_syslog(LOG_NOTICE, 0, "Route is not active. No kernel updates done.");
    }

    return 1;
}

/**
*   Debug function that writes the routing table entries
*   to the igmp_syslog.
*/
void logRouteTable(char *header) {
    IF_DEBUG  {
        struct RouteTable*  croute = routing_table;
        unsigned            rcount = 0;
    
        igmp_syslog(LOG_DEBUG, 0, "\nCurrent routing table (%s);\n-----------------------------------------------------\n", header);
        if(croute==NULL) {
            igmp_syslog(LOG_DEBUG, 0, "No routes in table...");
        } else {
            do {
                /*
                igmp_syslog(LOG_DEBUG, 0, "#%d: Src: %s, Dst: %s, Age:%d, St: %s, Prev: 0x%08x, T: 0x%08x, Next: 0x%08x",
                    rcount, inetFmt(croute->originAddr, s1), inetFmt(croute->group, s2),
                    croute->ageValue,(croute->originAddr>0?"A":"I"),
                    croute->prevroute, croute, croute->nextroute);
                */
                igmp_syslog(LOG_DEBUG, 0, "#%d: Src: %s, Dst: %s, Age:%d, St: %s, OutVifs: 0x%08x",
                    rcount, inetFmt(croute->originAddr, s1), inetFmt(croute->group, s2),
                    croute->ageValue,(croute->originAddr>0?"A":"I"),
                    croute->vifBits);

                croute = croute->nextroute; 
        
                rcount++;
            } while ( croute != NULL );
        }
    
        igmp_syslog(LOG_DEBUG, 0, "\n-----------------------------------------------------\n");
    }
}
