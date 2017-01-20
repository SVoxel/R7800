/* 
 *
 * Copyright (C) 1998 by Christopher Chan-Nui
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * 
 */

#include "config.h"
#include <stdio.h>
#include <netdb.h>
#include <ctype.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#ifdef HAVE_GETOPT_H
#include <getopt.h>
#endif
#include <errno.h>

#include <arpa/inet.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <netinet/ether.h>
#include <netpacket/packet.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#define _PATH_PROCNET_ARP "/proc/net/arp"

#define ARP_OPT_A (0x1)
#define ARP_OPT_p (0x2)
#define ARP_OPT_H (0x4)
#define ARP_OPT_t (0x8)
#define ARP_OPT_i (0x10)
#define ARP_OPT_a (0x20)
#define ARP_OPT_d (0x40)
#define ARP_OPT_n (0x80)        /* do not resolve addresses     */
#define ARP_OPT_D (0x100)       /* HW-address is devicename     */
#define ARP_OPT_s (0x200)
#define ARP_OPT_v (0x400 * DEBUG)       /* debugging output flag        */

#define DEV_NAME "eth1"

#if 0
#ifndef DEFAULTMAC
#define DEFAULTMAC "00a0c9852a5f"
#endif
#ifndef DEFAULTTARGET
#define DEFAULTTARGET "255.255.255.255"
#endif

static char *rcsid="@(#) $Id: wakelan.c,v 1.8 1998/08/30 05:04:28 channui Exp $";
static void *use_rcsid = (void *)((char *)&use_rcsid || (void *)&rcsid);
char *versionid = "1.0";

void usage(char *name) {
    printf ("Usage: %s [options] [mac] [broadcast] [port]\n"
	    "    -b addr    broadcast address\n"
	    "    -m mac     mac address of host\n"
	    "    -p port    UDP port to broadcast to\n"
	    "    -v[v]      version\n"
	    , name);
    exit (0);
}
#endif

int parse_mac(unsigned char *mac, char *str) {
    int i;
    int count;
    char c;
    unsigned char val;
    int colon_ok = 1;
    for (i = 0; i < 6; i++) {
	mac[i] = 0;
    }
    for (i = 0; i < 6; i++) {
	count = 0;
	val   = 0;
	do {
	    c = toupper(*str++);
	    if (c >= '0' && c <= '9') {
		val = (val * 16) + (c - '0');
	    } else if (c >= 'A' && c <= 'F') {
		val = (val * 16) + (c - 'A') + 10;
	    } else if (c == ':') {
		if (colon_ok || count-- != 0)
		    break;
	    } else if (c == '\0') {
		str--;
		break;
	    } else {
		return 0;
	    }
	    colon_ok=1;
	} while (++count < 2);
	colon_ok=(count<2);
	*mac++ = val;
    }
    if (*str)
	return 0;
    return 1;
}

#if 0
int main (int argc, char *argv[]) {
    int sock;
    int optval = 1;
    int version =0;
    int i, j, c, rc;
    char msg[1024];
    int  msglen = 0;
    struct sockaddr_in bcast;
    struct hostent *he;
    struct in_addr inaddr;
    unsigned char macaddr[6];
    char *mac    = DEFAULTMAC;
    char *target = DEFAULTTARGET;
    short bport = htons(32767);

    while ((c = getopt(argc, argv, "hvp:m:b:")) != EOF) {
	switch (c) {
	    case 'b': target = optarg;             break;
	    case 'm': mac    = optarg;             break;
	    case 'p': bport = htons(atoi(optarg)); break;
	    case 'v': version++;                   break;
	    case 'h':
	    case '?':
		usage(argv[0]);
	}
    }

    if (version) {
        printf ("Version: %s\n", versionid);
        if (version > 1) {
            printf ("  RCSID: %s\n", rcsid);
        }
        exit (0);
    }

    if (argv[optind] != NULL) {
	mac = argv[optind++];
    }
    if (argv[optind] != NULL) {
	target = argv[optind++];
    }
    if (argv[optind] != NULL) {
	bport = htons(atoi(argv[optind++]));
    }
    if (argv[optind] != NULL) {
	usage(argv[0]);
    }

    if (!parse_mac(macaddr, mac)) {
	printf ("Illegal MAC address '%s'\n", mac);
	exit (1);
    }

    if (!inet_aton(target, &inaddr)) {
	he = gethostbyname(target);
	inaddr = *(struct in_addr *)he->h_addr_list[0];
    }

    for (i = 0; i < 6; i++) {
	msg[msglen++] = 0xff;
    }
    for (i = 0; i < 16; i++) {
	for (j = 0; j < sizeof(macaddr); j++) {
	    msg[msglen++] = macaddr[j];
	}
    }

    memset(&bcast, 0, sizeof(bcast));
    bcast.sin_family      = AF_INET;
    bcast.sin_addr.s_addr = inaddr.s_addr;
    bcast.sin_port        = bport;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
	printf ("Can't allocate socket\n");
	exit (1);
    }
    if ((rc=setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval))) < 0) {
	printf ("Can't socket option SO_BROADCAST: rc = %d, errno=%s(%d)\n",
		rc, strerror(errno), errno);
	exit (1);
    }
    sendto(sock, &msg, msglen, 0, (struct sockaddr *)&bcast, sizeof(bcast));
    return 0;
}
#else
int send_wol (char *target, char *mac)
{
    int sock;
    int optval = 1;
    //int version =0;
    int i, j, rc;
    char msg[1024];
    int  msglen = 0;
    struct sockaddr_in bcast;
    struct hostent *he;
    struct in_addr inaddr;
    unsigned char macaddr[6];
    short bport = htons(32767);

    if (!parse_mac(macaddr, mac)) {
	printf ("Illegal MAC address '%s'\n", mac);
	exit (1);
    }

    if (!inet_aton(target, &inaddr)) {
	he = gethostbyname(target);
	inaddr = *(struct in_addr *)he->h_addr_list[0];
    }

    for (i = 0; i < 6; i++) {
	msg[msglen++] = 0xff;
    }
    for (i = 0; i < 16; i++) {
	for (j = 0; j < sizeof(macaddr); j++) {
	    msg[msglen++] = macaddr[j];
	}
    }

    memset(&bcast, 0, sizeof(bcast));
    bcast.sin_family      = AF_INET;
    bcast.sin_addr.s_addr = inaddr.s_addr;
    bcast.sin_port        = bport;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
	printf ("Can't allocate socket\n");
	return -1;
    }
    if ((rc=setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval))) < 0) {
	printf ("Can't socket option SO_BROADCAST: rc = %d, errno=%s(%d)\n",
		rc, strerror(errno), errno);
	return -1;
    }
    sendto(sock, &msg, msglen, 0, (struct sockaddr *)&bcast, sizeof(bcast));
    return 0;
}
#endif

int get_bcast_addr(char *ifname, char *bcast_addr)
{
        struct ifreq ifr;
        int fd;

        fd = socket(AF_INET, SOCK_DGRAM, 0);
        if (fd < 0) {
                printf("Unable to open the socket\n");
                return -1;
        }
        strncpy(ifr.ifr_name, ifname, sizeof(ifr.ifr_name));
        memset(bcast_addr, 0, 16);
        if (ioctl(fd, SIOCGIFBRDADDR, &ifr) >= 0)
                sprintf(bcast_addr, "%s", inet_ntoa(((struct sockaddr_in *)&ifr.ifr_broadaddr)->sin_addr));

        close(fd);

        return 0;
}

int main()
{
        //char host[100];
        char ip[100];
        char hwa[100];
        char mask[100];
        char line[200];
        char dev[100];
        int type, flags;
        FILE *fp;
        int num;
        //char cmd[200];
        char mac[16];
        char bcast_addr[16];
        int hwa_idx;
        int mac_idx;

        //host[0] = '\0';

        /* Open the PROCps kernel table. */
        fp = fopen(_PATH_PROCNET_ARP, "r");
        if (fp == NULL) {
                printf("Unable to open the file\n");
                return 0;
        }

        /* get Bcast addr for 'br0' */
        get_bcast_addr(DEV_NAME, (char *)bcast_addr);

        /* Bypass header -- read until newline */
        if (fgets(line, sizeof(line), fp) != (char *) NULL) {
                mask[0] = '-'; mask[1] = '\0';
                dev[0] = '-'; dev[1] = '\0';

                /* Read the ARP cache entries. */
                for (; fgets(line, sizeof(line), fp);) {
                        num = sscanf(line, "%s 0x%x 0x%x %100s %100s %100s\n",
                                                 ip, &type, &flags, hwa, mask, dev);
                        if (num < 4)
                                break;

//                      printf("ip: %s, hwa: %s, dev: %s\n", ip, hwa, dev);

                        if (!strcmp(dev, DEV_NAME)) {
                                memset(mac, 0, 16);
                                mac_idx = 0;
                                /* exclude ':' from the 'hwa' */
                                for (hwa_idx = 0; hwa_idx <= 17; hwa_idx += 3, mac_idx += 2) {
                                        memcpy(mac + mac_idx, hwa + hwa_idx, 2);
                                }
//                              printf("mac: %s\n", mac);
                                /* Trigger WOL for the mac */
				send_wol(bcast_addr, mac);
//                              sprintf(cmd, "/usr/sbin/wol -b %s -m %s", bcast_addr, mac);
//                              printf("cmd: %s\n", cmd);
//                                system(cmd);
                        }
                }
        }

        fclose(fp);

        return 0;
}
