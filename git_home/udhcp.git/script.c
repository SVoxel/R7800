/* script.c
 *
 * Functions to call the DHCP client notification scripts 
 *
 * Russ Dill <Russ.Dill@asu.edu> July 2001
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#include "options.h"
#include "dhcpd.h"
#include "dhcpc.h"
#include "packet.h"
#include "options.h"
#include "debug.h"

/* get a rough idea of how long an option will be (rounding up...) */
static int max_option_length[] = {
	[OPTION_IP] =		sizeof("255.255.255.255 "),
	[OPTION_IP_PAIR] =	sizeof("255.255.255.255 ") * 2,
#if defined (RFC3442_121_SUPPORT) || defined (RFC3442_249_SUPPORT)
	[OPTION_IP_COMP] =	sizeof("255.255.255.255 ") * 3,
#endif
	[OPTION_STRING] =	1,
	[OPTION_BOOLEAN] =	sizeof("yes "),
	[OPTION_U8] =		sizeof("255 "),
	[OPTION_U16] =		sizeof("65535 "),
	[OPTION_S16] =		sizeof("-32768 "),
	[OPTION_U32] =		sizeof("4294967295 "),
	[OPTION_S32] =		sizeof("-2147483684 "),
	[OPTION_6RD] = 		sizeof("32 128 ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff 255.255.255.255 "),
};


static int upper_length(int length, struct dhcp_option *option)
{
	return max_option_length[option->flags & TYPE_MASK] *
	       (length / option_lengths[option->flags & TYPE_MASK]);
}


static int sprintip(char *dest, char *pre, unsigned char *ip) {
	return sprintf(dest, "%s%d.%d.%d.%d ", pre, ip[0], ip[1], ip[2], ip[3]);
}
#ifdef RFC2132_33_SUPPORT
/*
 * RFC2132: The code for this option is 33. The minimum length of this option is
 * 8, and the length MUST be a multiple of 8.
 *
 *  Code   Len         Destination 1           Router 1
 *  +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
 *  |  33 |  n  |  d1 |  d2 |  d3 |  d4 |  r1 |  r2 |  r3 |  r4 | ...
 *  +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
 */
static int sprintippair(char *dest, unsigned char *ippair)
{
     unsigned char *netmask;

      /*for Spec Rev10 P88: If the destination address is a network address based on
       *its address class, then the route is added as a network route instead of a host route */
     if(((ippair[0] & 0x80)== 0x00) && !(ippair[1]|ippair[2]|ippair[3]))
	netmask = "255.0.0.0";      /* IP class A network address */
     else if(((ippair[0] & 0xc0) == 0x80) && !(ippair[2]|ippair[3]))
	netmask = "255.255.0.0";    /* IP class B network address */
     else if(((ippair[0] & 0xe0) == 0xc0) && !(ippair[3]))
	netmask = "255.255.255.0";  /* IP class C network address */
     else
	netmask = "255.255.255.255";

     return sprintf(dest, "%d.%d.%d.%d/%s/%d.%d.%d.%d ",
            ippair[0], ippair[1], ippair[2], ippair[3],netmask,
            ippair[4], ippair[5], ippair[6], ippair[7]);
}
#endif

#if defined (RFC3442_121_SUPPORT) || defined (RFC3442_249_SUPPORT)
/*
 * RFC3442: Classless Route Option Format
 * The code for this option is 121, and its minimum length is 5 bytes. This option can contain one
 * or more static routes, each of which consists of a destination descriptor and the IP address of
 * the router that should be used to reach that destination.
 *
 * Code Len  Destination 1   Router 1            Destination 2   Router 2
 * +-----+---+----+-----+----+----+----+----+----+----+-----+----+----+----+----+----+
 * | 121 | n | d1 | ... | dN | r1 | r2 | r3 | r4 | d1 | ... | dN | r1 | r2 | r3 | r4 |
 * +-----+---+----+-----+----+----+----+----+----+----+-----+----+----+----+----+----+
 *
 * Destination descriptors describe the IP subnet number and subnet mask of a particular destination
 * using a compact encoding. This encoding consists of one octet describing the width of the subnet
 * mask, followed by all the significant octets of the subnet number. The width of the subnet mask
 * describes the number of one bits in the mask, so for example a subnet with a subnet number of
 * 10.0.127.0 and a netmask of 255.255.255.0 would have a subnet mask width of 24.
 * 
 * The significant portion of the subnet number is simply all of the octets of the subnet number where
 * the corresponding octet in the subnet mask is non-zero. The number of significant octets is the width
 * of the subnet mask divided by eight, rounding up, as shown in the following table:
 *
 * Width of subnet mask     Number of significant octets
 * 0				0
 * 1-8				1
 * 9-16				2
 * 17-24			3
 * 25-32			4
 *
 * The following table contains some examples of how various subnet number/mask combinations can be
 * encoded:
 *
 * Subnet number   Subnet mask      Destination descriptor
 * 0               0                0
 * 10.0.0.0        255.0.0.0        8.10
 * 10.0.0.0        255.255.255.0    24.10.0.0
 * 10.17.0.0       255.255.0.0      16.10.17
 * 10.27.129.0     255.255.255.0    24.10.27.129
 * 10.229.0.128    255.255.255.128  25.10.229.0.128
 * 10.198.122.47   255.255.255.255  32.10.198.122.47
 */
static inline int mask_octet_len(int masklen)
{
	return ((masklen + 7) / 8);
}

static inline unsigned int inet_make_mask(int logmask)
{
	if (logmask)
		return htonl(~((1 << (32 - logmask)) - 1));
	return 0;
}

#define NIPQUAD(addr) \
	((unsigned char *)&addr)[0], \
	((unsigned char *)&addr)[1], \
	((unsigned char *)&addr)[2], \
	((unsigned char *)&addr)[3]

/* Format as "subnet/mask/gateway" */
static int sprintipcomp(char *dest, unsigned char *ipcomp, int *optlen)
{
	int masklen, octlen;
	unsigned int subnet, netmask;

	masklen	= *ipcomp++;
	octlen	= mask_octet_len(masklen);
	*optlen	= 1 + octlen + 4;

	subnet = 0;
	memcpy(&subnet, ipcomp, octlen);

	netmask = inet_make_mask(masklen);
	ipcomp += octlen;

	return sprintf(dest, "%u.%u.%u.%u/%u.%u.%u.%u/%u.%u.%u.%u ",
			NIPQUAD(subnet), NIPQUAD(netmask),
			ipcomp[0], ipcomp[1], ipcomp[2], ipcomp[3]);
}
#endif

const char bb_hexdigits_upcase[] = "0123456789ABCDEF";

/* Emit a string of hex representation of bytes */
char* bin2hex(char *p, const char *cp, int count)
{
    while (count) {
        unsigned char c = *cp++;
        /* put lowercase hex digits */
        *p++ = 0x20 | bb_hexdigits_upcase[c >> 4];
        *p++ = 0x20 | bb_hexdigits_upcase[c & 0xf];
        count--;
    }
    return p;
}

/* note: ip is a pointer to an IP in network order, possibly misaliged */
static int sprint_nip(char *dest, const char *pre, const uint8_t *ip)
{
    return sprintf(dest, "%s%u.%u.%u.%u", pre, ip[0], ip[1], ip[2], ip[3]);
}

/* note: ip is a pointer to an IPv6 in network order, possibly misaliged */
int sprint_nip6(char *dest, /*const char *pre,*/ const uint8_t *ip)
{
    char hexstrbuf[16 * 2];
    bin2hex(hexstrbuf, (void*)ip, 16);
    return sprintf(dest, /* "%s" */
        "%.4s:%.4s:%.4s:%.4s:%.4s:%.4s:%.4s:%.4s",
        /* pre, */
        hexstrbuf + 0 * 4,
        hexstrbuf + 1 * 4,
        hexstrbuf + 2 * 4,
        hexstrbuf + 3 * 4,
        hexstrbuf + 4 * 4,
        hexstrbuf + 5 * 4,
        hexstrbuf + 6 * 4,
        hexstrbuf + 7 * 4
    );
}

/* Fill dest with the text of option 'option'. */
static void fill_options(char *dest, unsigned char *option, struct dhcp_option *type_p)
{
	int type, optlen;
	u_int16_t val_u16;
	int16_t val_s16;
	u_int32_t val_u32;
	int32_t val_s32;
	int len = option[OPT_LEN - 2];

	dest += sprintf(dest, "%s=", type_p->name);

	type = type_p->flags & TYPE_MASK;
	optlen = option_lengths[type];
	for(;;) {
		switch (type) {
#if defined (RFC3442_121_SUPPORT) || defined (RFC3442_249_SUPPORT)
		case OPTION_IP_COMP:
			dest += sprintipcomp(dest, option, &optlen);
			break;
#endif
		case OPTION_IP_PAIR:
#ifdef RFC2132_33_SUPPORT
                        dest += sprintippair(dest, option);
                        break;
#else
			dest += sprintip(dest, "", option);
			*(dest++) = '/';
			option += 4;
			optlen = 4;
#endif
		case OPTION_IP:	/* Works regardless of host byte order. */
			dest += sprintip(dest, "", option);
 			break;
		case OPTION_BOOLEAN:
			dest += sprintf(dest, *option ? "yes " : "no ");
			break;
		case OPTION_U8:
			dest += sprintf(dest, "%u ", *option);
			break;
		case OPTION_U16:
			memcpy(&val_u16, option, 2);
			dest += sprintf(dest, "%u ", ntohs(val_u16));
			break;
		case OPTION_S16:
			memcpy(&val_s16, option, 2);
			dest += sprintf(dest, "%d ", ntohs(val_s16));
			break;
		case OPTION_U32:
			memcpy(&val_u32, option, 4);
			dest += sprintf(dest, "%lu ", (unsigned long) ntohl(val_u32));
			break;
		case OPTION_S32:
			memcpy(&val_s32, option, 4);
			dest += sprintf(dest, "%ld ", (long) ntohl(val_s32));
			break;
		case OPTION_STRING:
			memcpy(dest, option, len);
			dest[len] = '\0';
			return;	 /* Short circuit this case */
		case OPTION_6RD:
			if (len >= (1 + 1 + 16 + 4) && option[0] <= 32
				&& (option[1] + 32 - option[0]) <= 128
			) {
				/* IPv4MaskLen */
		                dest += sprintf(dest, "%u ", *option++);
                		/* 6rdPrefixLen */
		                dest += sprintf(dest, "%u ", *option++);
                		/* 6rdPrefix */
		                dest += sprint_nip6(dest, /* "", */ option);
                		option += 16;
				len -= 1 + 1 + 16 + 4;
				/* "+ 4" above corresponds to the length of IPv4 addr
		                 * we consume in the loop below */
				while (1) {
                	       /* 6rdBRIPv4Address(es) */
		                    dest += sprint_nip(dest, " ", option);
                		    option += 4;
		                    len -= 4; /* do we have yet another 4+ bytes? */
                		    if (len < 0)
		                        break; /* no */
                		}
			}
			return;
		}
		option += optlen;
		len -= optlen;
		if (len <= 0) break;
	}
}


static char *find_env(const char *prefix, char *defaultstr)
{
	extern char **environ;
	char **ptr;
	const int len = strlen(prefix);

	for (ptr = environ; *ptr != NULL; ptr++) {
		if (strncmp(prefix, *ptr, len) == 0)
			return *ptr;
	}
	return defaultstr;
}


/* put all the paramaters into an environment */
static char **fill_envp(struct dhcpMessage *packet)
{
	int num_options = 0;
	int i, j;
	char **envp;
	unsigned char *temp;
	char over = 0;

	if (packet == NULL)
		num_options = 0;
	else {
		for (i = 0; options[i].code; i++)
			if (get_option(packet, options[i].code))
				num_options++;
		if (packet->siaddr) num_options++;
		if ((temp = get_option(packet, DHCP_OPTION_OVER)))
			over = *temp;
		if (!(over & FILE_FIELD) && packet->file[0]) num_options++;
		if (!(over & SNAME_FIELD) && packet->sname[0]) num_options++;		
	}
	
	envp = xmalloc((num_options + 5) * sizeof(char *));
	envp[0] = xmalloc(sizeof("interface=") + strlen(client_config.interface));
	sprintf(envp[0], "interface=%s", client_config.interface);
	envp[1] = find_env("PATH", "PATH=/bin:/usr/bin:/sbin:/usr/sbin");
	envp[2] = find_env("HOME", "HOME=/");

	if (packet == NULL) {
		envp[3] = NULL;
		return envp;
	}

	envp[3] = xmalloc(sizeof("ip=255.255.255.255"));
	sprintip(envp[3], "ip=", (unsigned char *) &packet->yiaddr);
	for (i = 0, j = 4; options[i].code; i++) {
		if ((temp = get_option(packet, options[i].code))) {
			envp[j] = xmalloc(upper_length(temp[OPT_LEN - 2], &options[i]) + strlen(options[i].name) + 2);
#ifdef RFC2132_33_SUPPORT
			if(options[i].code == 0x21) {
				int tlen = upper_length(temp[OPT_LEN - 2], &options[i]) + strlen(options[i].name) + 2;
				envp[j] = realloc(envp[j],tlen + (temp[OPT_LEN - 2]/8)*sizeof("255.255.255.255/"));
			}
#endif
			fill_options(envp[j], temp, &options[i]);
			j++;
		}
	}
	if (packet->siaddr) {
		envp[j] = xmalloc(sizeof("siaddr=255.255.255.255"));
		sprintip(envp[j++], "siaddr=", (unsigned char *) &packet->siaddr);
	}
	if (!(over & FILE_FIELD) && packet->file[0]) {
		/* watch out for invalid packets */
		packet->file[sizeof(packet->file) - 1] = '\0';
		envp[j] = xmalloc(sizeof("boot_file=") + strlen(packet->file));
		sprintf(envp[j++], "boot_file=%s", packet->file);
	}
	if (!(over & SNAME_FIELD) && packet->sname[0]) {
		/* watch out for invalid packets */
		packet->sname[sizeof(packet->sname) - 1] = '\0';
		envp[j] = xmalloc(sizeof("sname=") + strlen(packet->sname));
		sprintf(envp[j++], "sname=%s", packet->sname);
	}	
	envp[j] = NULL;
	return envp;
}


/* Call a script with a par file and env vars */
void run_script(struct dhcpMessage *packet, const char *name)
{
	int pid;
	char **envp;

	if (client_config.script == NULL)
		return;

	/* call script */
	pid = fork();
	if (pid) {
		waitpid(pid, NULL, 0);
		return;
	} else if (pid == 0) {
		envp = fill_envp(packet);
		
		/* close fd's? */
		
		/* exec script */
		DEBUG(LOG_INFO, "execle'ing %s", client_config.script);
		execle(client_config.script, client_config.script,
		       name, NULL, envp);
		LOG(LOG_ERR, "script %s failed: %s",
		    client_config.script, strerror(errno));
		exit(1);
	}			
}
