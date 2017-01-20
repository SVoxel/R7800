#include "defs.h"

struct source_set ZEROSET;

struct source_set_op
{
	void (*op)(struct source_set *, struct source_set *, struct source_set *, int);
	int inverse;
	int mode_change;
};

static struct source_set_op host_op[st_max][ev_max] = 
{
	/* none operation (initial)*/
	{{NULL, 0, 0}, {NULL, 0, 0}, {NULL, 0, 0}, {NULL, 0, 0}, {NULL, 0, 0}, {NULL, 0, 0}},
	/* IS_IN operation*/
	{{NULL, 0, 0}, {set_assign, 0, 0}, {set_assign, 0, 1}, {set_assign, 0, 0}, {set_assign, 0, 1}, {set_add, 0, 0}, {set_subtract, 0, 0}},
	/* IS_EX operation*/
	{{NULL, 0, 0}, {set_assign, 0, 1}, {set_assign, 0, 0}, {set_assign, 0, 1}, {set_assign, 0, 0}, {set_subtract, 0, 0}, {set_add, 0, 0}}
};

static struct source_set_op proxy_op[st_max][ev_max] =
{
	/* none operation (initial)*/
	{{NULL, 0, 0}, {NULL, 0, 0}, {NULL, 0, 0}, {NULL, 0, 0}, {NULL, 0, 0}, {NULL, 0, 0}},
	/* IS_IN operation*/
	{{NULL, 0, 0}, {set_add, 0, 0}, {set_subtract, 1, 1}, {set_add, 0, 0}, {set_subtract, 1, 1}, {NULL, 0, 0}, {NULL, 0, 0}},
	/* IS_EX operation*/
	{{NULL, 0, 0}, {set_subtract, 0, 0}, {set_intersection, 0, 0}, {set_subtract, 0, 0}, {set_intersection, 0, 0}, {NULL, 0, 0}, {NULL, 0, 0}}
};

/*
 * add_ebt_rule : add a ebtables' rule
 * port : insert to which chain
 * group : wanted multicast group address
 * newmode : include or exclude mode
 * newset : new source list set
*/
static inline void
add_ebt_rule(uint32 port, uint32 group, uint32 newmode, struct source_set *newset)
{
	char line[256];
	int i, len;
	int num = (newset->num == PORT_INIT_STATE)?0:newset->num;
	if (newset)
	{
		len = sprintf(line, "ebtables -I OUTPUT -o ethwan -p ipv4 --ip-dst %u.%u.%u.%u ", 
			NIPQUAD(group));
		if (num)
			len += sprintf(line+len, "%s --ip-msip ", (newmode == st_is_in)?"":"!");
		for (i = 0; i < num; i++)
			len += sprintf(line+len, "%u.%u.%u.%u%s", 
			NIPQUAD(newset->list[i]), (i == (num-1))?"":",");
		sprintf(line+len, " -j ACCEPT");
		DPRINTF("add_ebt_rule :: cmd [%s]\n", line);
		system(line);
	}
}

/*
 * del_ebt_rule : del a ebtables' rule
 * port : insert to which chain
 * group : wanted multicast group address
 * oldmode : include or exclude mode
 * oldset : old source list set
*/
static inline void
del_ebt_rule(uint32 port, uint32 group, uint32 oldmode, struct source_set *oldset)
{
	char line[256];
	int i, len;
	int num = (oldset->num == PORT_INIT_STATE)?0:oldset->num;
	if (oldset)
	{
		len = sprintf(line, "ebtables -D OUTPUT -o ethwan -p ipv4 --ip-dst %u.%u.%u.%u ", 
			NIPQUAD(group), (oldmode == st_is_in)?"":"!");
		if (num)
			len += sprintf(line+len, "%s --ip-msip ", (oldmode == st_is_in)?"":"!");
		for (i = 0; i < num; i++)
			len += sprintf(line+len, "%u.%u.%u.%u%s", 
			NIPQUAD(oldset->list[i]), (i != (num-1))?",":"");
		sprintf(line+len, " -j ACCEPT");
		DPRINTF("del_ebt_rule :: cmd [%s]\n", line);
		system(line);
	}
}

/*
 * update_ebt_rules : update a ebtables' rule
 * port : insert to which chain
 * group : wanted multicast group address
 * oldmode : include or exclude mode
 * oldset : old source list set
 * newmode : include or exclude mode
 * newset : new source list set
 * note : if our update procedure causes the media stream discontinuously, we may swap the update commands
*/
static inline void
update_ebt_rules(uint32 port, uint32 group, uint32 oldmode, struct source_set *oldset, uint32 newmode, struct source_set *newset)
{
	del_ebt_rule(port, group, oldmode, oldset);

	add_ebt_rule(port, group, newmode, newset);
}

static inline void
set_mcast_set(struct Listener *listener, uint32 group)
{
	char line[256], i, len;
	len = sprintf(line, "echo \"%u.%u.%u.%u %u.%u.%u.%u %d %d ", 
		NIPQUAD(listener->srcAddr), NIPQUAD(group), (listener->mode == st_is_in)?1:0, listener->source.num);
	for (i = 0; i < listener->source.num; i++)
		len += sprintf(line+len, "%u.%u.%u.%u ", NIPQUAD(listener->source.list[i]));
	sprintf(line+len, "\" > /proc/mcast_set");
	system(line);
}

/*
 * vaild_grec : to valid the group record
 * if the group record is valid/invalid, to return true/false.
 * if the multicast address field is not a multicast address ==> invalid
 * if the number of source is greater than 8 ==> invalid (for Netgear Spec.)
 * if the sources have more than one zero address or sources have multicast address ==> invalid
*/
static inline int
vaild_grec(struct igmpv3_grec *grec)
{
	int i, flag =0;

	if (!MULTICAST(grec->grec_mca) || 
		(grec->grec_nsrcs > 8)||
		(grec->grec_type > IGMPV3_BLOCK_OLD_SOURCE))
		return 0;
	for (i = 0; i < grec->grec_nsrcs; i++)
	{
		if (!grec->grec_src[i])
			flag++;
		if (MULTICAST(grec->grec_src[i]))
			flag += 2;
		if (flag > 1)
			return 0;
	}
	return 1;
}

/*
 * copy_source_list : copy group record to the temporary source set buffer
 * set : the source set storage buffer
 * grec : the group record
*/
static inline void
copy_source_list(struct source_set *set, struct igmpv3_grec *grec)
{
	int i;
	memset(set, 0, sizeof(struct source_set));
	set->num = grec->grec_nsrcs;
	for (i = 0; i < set->num; i++)
		set->list[i] = grec->grec_src[i];
}

/*
 * update_proxy_source_info : update the source set of the router
 * note : port 0 means wireless port,
 *          port1 ~ portN mean ethernet ports
 * to collect all ports source set information and to calculate the latest source set
 * if source set is changed, to use set_source_list to update the source set.
*/
static inline void
update_proxy_source_info(struct RouteTable *croute)
{
	int i;
	struct source_set_op *proxy;
	struct source_set tmp, tmp1;
	uint32 mode = croute->port_mode[0];

	memcpy(&tmp, &croute->port_source[0], sizeof(struct source_set));
	memset(&tmp1, 0, sizeof(struct source_set));
	for (i = 1; i <= PORT_MAX_NUM; i++)
	{
		proxy = &proxy_op[mode][croute->port_mode[i]];
		proxy->op(&tmp, &croute->port_source[i], &tmp1, proxy->inverse);
		if (proxy->mode_change)
			mode = (mode== st_is_in)?st_is_ex:st_is_in;
		memcpy(&tmp, &tmp1, sizeof(struct source_set));
	}

	if (mode != croute->mode ||
		set_comp(&croute->source, &tmp))
	{
		croute->mode = mode;
		memcpy(&croute->source, &tmp, sizeof(struct source_set));
		set_source_list(croute);
	}

}

/*
 * update_port_source_info : update the source set of the specific port
 * croute : multicast routing table info
 * port : come from which port
 * flag : call update_proxy_source_info or not
 * note : if the source set is changed to the include mode with an empty set, to delete the ebtables' rule
*/
static inline void
update_port_source_info(struct RouteTable *croute, uint32 port, uint32 flag)
{
	struct source_set tmp[3];
	uint32 i = 0, curr_state = st_is_in, next_state;
	struct Listener *listener = croute->listeners, *next;
	struct source_set_op *proxy;

	memset(&tmp[0], 0, sizeof(struct source_set));
	tmp[0].num = PORT_INIT_STATE;
	if (listener)
	{
		while (listener)
		{
			next = listener->nextlistener;
			if (listener->port == port)
			{
				if (tmp[0].num == PORT_INIT_STATE)
				{
					curr_state = listener->mode;
					memcpy(&tmp[0], &listener->source, sizeof(struct source_set));
				}
				else
				{
					memcpy(&tmp[1], &listener->source, sizeof(struct source_set));
					next_state = listener->mode;
					proxy = &proxy_op[curr_state][next_state];
					proxy->op(&tmp[0], &tmp[1], &tmp[2], proxy->inverse);
					memcpy(&tmp[0], &tmp[2], sizeof(struct source_set));
					if (proxy->mode_change)
						curr_state = (curr_state == st_is_in)?st_is_ex:st_is_in;
				}
			}
			listener = next;
		}
	}
	
	if (croute->port_mode[port] != curr_state
		||set_comp(&croute->port_source[port], &tmp[0]))
	{
		if (tmp[0].num == PORT_INIT_STATE)
		{
			del_ebt_rule(port, croute->group, croute->port_mode[port], &croute->port_source[port]);
			memcpy(&croute->port_source[port], tmp, sizeof(struct source_set));
			croute->port_mode[port] = st_is_in;
		}
		else
		{
			update_ebt_rules(port, croute->group, croute->port_mode[port], &croute->port_source[port], curr_state, &tmp[0]);
			memcpy(&croute->port_source[port], &tmp[0], sizeof(struct source_set));
			croute->port_mode[port] = curr_state;
		}
		if (flag)
			update_proxy_source_info(croute);
	}
}

/*
 * update_all_ports : update all ports information
 * croute : multicast routing table info
 * If the clients join(leave) or the port status is changed,
 * we will call update_all_ports.
 * And to avoid the update_proxy_source_info is called frequently
 * so we will call update_proxy_source_info after all ports are updated.
*/
void
update_all_ports(struct RouteTable *croute)
{
	int i;
	if (mc_ctrl_packet(ntohl(croute->group)))
		return;

	for (i = 0; i <= PORT_MAX_NUM; i++)
		update_port_source_info(croute, i, (i == PORT_MAX_NUM)?1:0);
}

/*
 * add_listener : get a new client
 * port : come from which port
 * croute : multicast routing table info
 * src : the ip address of the new client
 * tmp1 : the new source set
 * mode : include or exclude mode
*/
static inline void
add_listener(uint32 port, struct RouteTable *croute, uint32 src, struct source_set *tmp1, int mode) 
{
	struct Listener *listener = NULL;
	struct source_set_op *proxy;
	struct source_set tmp2;

	listener = (struct Listener *)insertListener(port, croute, src);
	if (!listener)
		return;

	listener->mode = mode;
	listener->port = port;
	listener->version = IGMP_V3_MEMBERSHIP_REPORT;
	memcpy(&listener->source, tmp1, sizeof(struct source_set));

	update_port_source_info(croute, port, 1);
	if (!port)
	{
		join_group(src, croute->group);
		set_mcast_set(listener, croute->group);
	}
}

/*
 * create_croute : create a new multicast routing table entry
 * group : multicast group address
 * src : the ip address of the new client
 * port : come from which port
 * grec_type : the type of group record
 * source : the new source set
 * note : In order to pass the cdrouter test case (case 335 ~ case 343).
 *           If we get any source address, to call add_source_list first before update the source list.
*/
static inline void
create_croute(uint32 group, uint32 src, uint32 port, uint8 grec_type, struct source_set *source)
{
	struct RouteTable *croute = NULL;
	struct Listener *listener = NULL;
	uint32 mode;
	acceptGroupReport(port, src, group, IGMP_V3_MEMBERSHIP_REPORT);
	croute = (struct RouteTable *)findRoute(group);

	if (!croute)
		return;
	memcpy(&croute->source, source, sizeof(struct source_set));
	mode = (grec_type == IGMPV3_MODE_IS_INCLUDE || 
		grec_type == IGMPV3_CHANGE_TO_INCLUDE||
		grec_type == IGMPV3_ALLOW_NEW_SOURCE)?st_is_in:st_is_ex;
	listener = (struct Listener *)findListener(croute, src);
	if (listener)
	{
		memcpy(&listener->source, source, sizeof(struct source_set));
		listener->mode = mode;
		listener->port = port;
		listener->version = IGMP_V3_MEMBERSHIP_REPORT;
		if (listener->source.num)
			add_source_list(croute->group, listener->source.list[0]);
	}
	update_port_source_info(croute, port, 1);
	if (!port)
		set_mcast_set(listener, croute->group);
}

/*
 * processV3report : IGMPV3 main function
 * buf : igmp packet
 * src : the ip address of the new client
 * port : come from which port
*/
void 
processV3report(char *buf, uint32 src, uint32 port)
{
	struct igmpv3_report *igmpv3 = (struct igmpv3_report *)buf;
	uint16 i, num = igmpv3->ngrec;
	struct igmpv3_grec *grec = NULL;
	uint32 group, mode, port_change = 0;
	struct RouteTable *croute = NULL;
	struct source_set tmp1, tmp2;
	struct Listener *listener = NULL;
	struct source_set_op *host;
	struct IfDesc  *sourceVif;
	char *pdata = (char *)igmpv3->grec;

	for (i = 0;(i < num) && pdata; i++, pdata += grec->grec_nsrcs*4 + 8)
	{
		grec = (struct igmpv3_grec *)pdata;
		// check group record is valid or not
		if (!vaild_grec(grec))
			return;

		group = grec->grec_mca;
		if (mc_ctrl_packet(ntohl(group)))
			continue;
		croute = (struct RouteTable *)findRoute(group);
		copy_source_list(&tmp1, grec);
		if (!croute)
		{
			// if the multicast group entry is not existed and group record type is not block old sources,
			// to add a new multicast group entry
			if (grec->grec_type < IGMPV3_BLOCK_OLD_SOURCE)
				create_croute(group, src, port, grec->grec_type, &tmp1);
		}
		else
		{
			// it should not be happened
			// to make sure all router mode will be include, exclude or not set.
			if (croute->mode > 2)
				croute->mode = 0;

			listener = (struct Listener *)findListener(croute, src);
			// it should not be happened
			if (croute->mode == st_none)
			{
				switch (grec->grec_type)
				{
					case ev_is_in:
					case ev_to_in:
					case ev_allow:
						// skipped leave message
						if (!memcmp(&tmp1, &ZEROSET, sizeof(struct source_set)))
							break;
					case ev_is_ex:
					case ev_to_ex:
						create_croute(group, src, port, grec->grec_type, &tmp1);
					case ev_block:
					default:
						break;
				}
				continue;
			}
			sourceVif = getIfByAddress( src );
			if(!sourceVif)
				return;
			// to update kernel multicast route table
			insertRoute(port, group, sourceVif->index, src);
			if (listener)
			{
				// to check port is changed or not
				if (listener->port != port)
					port_change = 1;
				host = &host_op[listener->mode][grec->grec_type];
				host->op(&listener->source, &tmp1, &tmp2, host->inverse);
				switch (FSM(listener->mode, grec->grec_type))
				{
					case FSM(st_is_in, ev_is_in):
					case FSM(st_is_in, ev_to_in):
					case FSM(st_is_in, ev_allow):
						if (port_change || memcmp(&listener->source, &tmp2, sizeof(struct source_set)))
						{
							// if the source set is become include mode with an empty set,
							// to call acceptLeaveMessage
							if (tmp2.num)
							{
								memcpy(&listener->source, &tmp2, sizeof(struct source_set));
								listener->port = port;
								if (port_change)
									update_all_ports(croute);
								else
									update_port_source_info(croute, port, 1);
								if (!port)
									set_mcast_set(listener, group);
							}
							else
								acceptLeaveMessage(src, group);
						}
						break;
					case FSM(st_is_in, ev_is_ex):
					case FSM(st_is_in, ev_to_ex):
						listener->mode = st_is_ex;
						memcpy(&listener->source, &tmp2, sizeof(struct source_set));
						listener->port = port;
						if (port_change)
							update_all_ports(croute);
						else
							update_port_source_info(croute, port, 1);
						if (!port)
							set_mcast_set(listener, group);
						break;
					case FSM(st_is_in, ev_block):
						if (tmp2.num)
						{
							if (port_change || memcmp(&listener->source, &tmp2, sizeof(struct source_set)))
							{
								memcpy(&listener->source, &tmp2, sizeof(struct source_set));
								listener->port = port;
								if (port_change)
									update_all_ports(croute);
								else
									update_port_source_info(croute, port, 1);
								if (!port)
									set_mcast_set(listener, group);
							}
						}
						else
							acceptLeaveMessage(src, group);
						break;
					case FSM(st_is_ex, ev_is_in):
					case FSM(st_is_ex, ev_to_in):
						if (tmp2.num)
						{
							listener->port = port;
							listener->mode = st_is_in;
							if (memcmp(&listener->source, &tmp2, sizeof(struct source_set)))
								memcpy(&listener->source, &tmp2, sizeof(struct source_set));
							if (port_change)
								update_all_ports(croute);
							else
								update_port_source_info(croute, port, 1);
							if (!port)
								set_mcast_set(listener, group);
						}
						else
							acceptLeaveMessage(src, group);
						break;
					case FSM(st_is_ex, ev_is_ex):
					case FSM(st_is_ex, ev_to_ex):
					case FSM(st_is_ex, ev_allow):
					case FSM(st_is_ex, ev_block):
						if (port_change || memcmp(&listener->source, &tmp2, sizeof(struct source_set)))
						{
							listener->port = port;
							memcpy(&listener->source, &tmp2, sizeof(struct source_set));
							if (port_change)
								update_all_ports(croute);
							else
								update_port_source_info(croute, port, 1);
							if (!port)
								set_mcast_set(listener, group);
						}
						break;
					default:
						DPRINTF("%s :: unknown state [%d] or unknown event [%d]", __FUNCTION__, croute->mode, grec->grec_type);
						break;
				}
			}
			else
			{
				switch (grec->grec_type)
				{
					case ev_is_in:
					case ev_to_in:
					case ev_allow:
						mode = st_is_in;
						break;
					case ev_is_ex:
					case ev_to_ex:
						mode = st_is_ex;
						break;
					// if an unknown client send a block type message,
					// to skip it
					case ev_block:
					default:
						continue;
				}
				add_listener(port, croute, src, &tmp1, mode);
			}
		}
	}
}


