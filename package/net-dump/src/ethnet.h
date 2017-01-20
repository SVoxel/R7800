#ifndef _ETH_NET_H
#define _ETH_NET_H

#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/signal.h>
#include <sys/sysinfo.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <net/if_packet.h>
#include <netinet/ether.h>
#include <netpacket/packet.h>

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

#define ETH_HDRLEN	14	/* 6.DA + 6.SA + 2.TYPE */

#define TFTP_PORT	69

/* Different Router */
#define MODEL_NAME	"R7800"
#define DFT_FILENAME	MODEL_NAME "_pkt_debug.txt"
#define DFT_IFNAME	"brwan"

typedef unsigned int  uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;

typedef void (*put_fn) (char *, int , void *);

/* write to file per 2KB */
#define BUF_SIZE	0x2000	/* 8 KB for saving the raw of packets temporarily */
#define PER_2KB	0x0800
#define ADDR_ALIGN(addr) (char *)(((uint32)addr + 3) & ~3) /* align address to 4 bytes */

struct tftp_param
{
	uint16 port; /* network byte order */
	int socketfd;
	uint32 serverip;
	char *filename; /* if it is null, it means 'TFTP_WRQ' was xmited */
};

extern void tftp_put(char *buf, int len, void *param);
extern void file_put(char *buf, int len, void *param);
#endif

