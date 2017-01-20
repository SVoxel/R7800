/* resolv.c: DNS Resolver
 *
 * Copyright (C) 1998  Kenneth Albanowski <kjahds@kjahds.com>,
 *                     The Silver Hammer Group, Ltd.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
*/

/* Simplied with dnsmasq for embedded system.  --- 2008-03-20 */
#include "httpd.h"

#define PKTSIZE		512
#define MAXRETRIES 	3
#define DNS_HFIXEDSZ		12	/* bytes of fixed data in header */
#define DNS_RRFIXEDSZ	10	/* bytes of fixed data in r record */
#define DNS_PORT	53
#define DNS_IPADDR	0x7F000001	/* local nameserver: 127.0.0.1 */
#define DNS_TIMEOUT	3

#define NS_T_A	1	/* Host address. */
#define NS_C_IN	1	/* Internet. */

/* Structs */
struct resolv_header 
{
	int id;
	int qr,opcode,aa,tc,rd,ra,rcode;
	int qdcount;
	int ancount;
	int nscount;
	int arcount;
};

struct resolv_question 
{
	char *dotted;
	int qtype;
	int qclass;
};

struct resolv_answer 
{
	int atype;
	int aclass;
	int rdlength;
	unsigned char *rdata;
};

/* 
  * Encode a dotted string into nameserver transport-level encoding.
  * and the dest is BIG enough. 
  * g e m i n i . t u c . n o a o . e d u
  * [6] g e m i n i [3] t u c [4] n o a o [3] e d u[0]
  */
static int __encode_dotted(const char *dotted, unsigned char *dest)
{
	char c;
	int used, len;
	unsigned char *plen;

	len = 0, used = 1;
	plen = &dest[0];
	
	while ((c = *dotted++) != '\0') {
		if (c == '.') {
			*plen = (unsigned char)len;
			len = 0, plen = &dest[used++];
			continue;
		}

		dest[used++] = (unsigned char)c;
		len++;
	}

	*plen = (unsigned char)len;
	dest[used++] = 0;

	return used;
}

static int __length_dotted(unsigned char *data, int offset)
{
	int len, orig = offset;
	
	while ((len = data[offset++])) {
		if ((len & 0xC0) == 0xC0) { /* compress */
			offset++;
			break;
		}

		offset += len;
	}

	return offset - orig;
}

/* The dest is BIG enough */
static int __encode_question(struct resolv_question *q, unsigned char *dest)
{
	int i;

	i = __encode_dotted(q->dotted, dest);
	
	dest += i;

	dest[0] = (q->qtype & 0xFF00) >> 8;
	dest[1] = (q->qtype & 0x00FF) >> 0;
	dest[2] = (q->qclass & 0xFF00) >> 8;
	dest[3] = (q->qclass & 0x00FF) >> 0;

	return i + 4;
}

static int __decode_answer(unsigned char *message, int offset,
			struct resolv_answer *a)
{
	int i;

	i = __length_dotted(message, offset);
	
	message += offset + i;

	a->atype = (message[0] << 8) |message[1]; 
	message += 2;
	a->aclass = (message[0] << 8) |message[1]; 
	message += 6; /* skip ttl */
	a->rdlength = (message[0] << 8) |message[1];
	message += 2;
	a->rdata = message;

	return i + DNS_RRFIXEDSZ + a->rdlength;
}

static int __length_question(unsigned char *message, int offset)
{
	int i;

	i = __length_dotted(message, offset);

	return i + 4;
}

static int __encode_header(struct resolv_header *h, unsigned char *dest)
{
	dest[0] = (h->id & 0xFF00) >> 8;
	dest[1] = (h->id & 0x00FF) >> 0;
	dest[2] = (h->qr ? 0x80 : 0) |
			((h->opcode & 0x0F) << 3) |
			(h->aa ? 0x04 : 0) |
			(h->tc ? 0x02 : 0) |
			(h->rd ? 0x01 : 0);
	dest[3] = (h->ra ? 0x80 : 0) | (h->rcode & 0x0F);
	dest[4] = (h->qdcount & 0xFF00) >> 8;
	dest[5] = (h->qdcount & 0x00FF) >> 0;
	dest[6] = (h->ancount & 0xFF00) >> 8;
	dest[7] = (h->ancount & 0x00FF) >> 0;
	dest[8] = (h->nscount & 0xFF00) >> 8;
	dest[9] = (h->nscount & 0x00FF) >> 0;
	dest[10] = (h->arcount & 0xFF00) >> 8;
	dest[11] = (h->arcount & 0x00FF) >> 0;

	return DNS_HFIXEDSZ;
}

static void __decode_header(struct resolv_header *h, unsigned char *data)
{
	h->id = (data[0] << 8) | data[1];
	h->qr = (data[2] & 0x80) ? 1 : 0;
	h->opcode = (data[2] >> 3) & 0x0F;
	h->aa = (data[2] & 0x04) ? 1 : 0;
	h->tc = (data[2] & 0x02) ? 1 : 0;
	h->rd = (data[2] & 0x01) ? 1 : 0;
	h->ra = (data[3] & 0x80) ? 1 : 0;
	h->rcode = data[3] & 0x0F;
	h->qdcount = (data[4] << 8) | data[5];
	h->ancount = (data[6] << 8) | data[7];
	h->nscount = (data[8] << 8) | data[9];
	h->arcount = (data[10] << 8) | data[11];
}

#define OPEN_UDP(fd) (fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))
#define SOCKTYPE(dest) ((struct sockaddr const *)(dest))

/* modified from uclib function '__dns_lookup', just inet type */
static unsigned int dns_lookup(char *name)
{
	int retries = -1;
	int local_id = 1313;
	unsigned int ipaddr = 0;
	int i, j, len, fd, pos;
	fd_set readable;
	struct timeval timeo;
	struct sockaddr_in dest;
	struct resolv_header h;
	struct resolv_question q;
	struct resolv_answer ma;
	unsigned char packet[PKTSIZE];
	
	fd = -1;
	memset(&dest, 0, sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_port = htons(DNS_PORT);
	dest.sin_addr.s_addr = htonl(DNS_IPADDR); /* use '127.0.0.1' as nameserver, it's dnsmasq */
		
	while (++retries < MAXRETRIES) {
		if (fd < 0 && OPEN_UDP(fd) < 0)
			continue;

		memset(packet, 0, PKTSIZE);
		
		memset(&h, 0, sizeof(h));
		h.id = ++local_id;
		h.qdcount = 1;
		h.rd = 1;
		i = __encode_header(&h, packet);

		q.dotted = name;
		q.qtype = NS_T_A;
		q.qclass = NS_C_IN;
		j = __encode_question(&q, packet + i);
	
		len = i + j;
		if (sendto(fd, packet, len, 0, SOCKTYPE(&dest), sizeof(dest)) < 1)
			continue;

		FD_ZERO(&readable);
		FD_SET(fd, &readable);
		timeo.tv_sec = DNS_TIMEOUT;
		timeo.tv_usec = 0;
		if (select(fd + 1, &readable, NULL, NULL, &timeo) < 1)
			continue;			

		len = recvfrom(fd, packet, sizeof(packet), 0, NULL, 0);
		if (len < DNS_HFIXEDSZ)
			continue;

		__decode_header(&h, packet);
		if (h.id != local_id ||!h.qr ||h.rcode ||h.ancount < 1)
			continue;
		
		pos = DNS_HFIXEDSZ;
		for (j = 0; j < h.qdcount; j++)
			pos += __length_question(packet, pos);
		
		for (j = 0; j < h.ancount; j++, pos += i) {
			i = __decode_answer(packet, pos, &ma);
			if (ma.atype != NS_T_A ||ma.aclass != NS_C_IN)
				continue;

			unsigned char *p = ma.rdata;
			ipaddr = (p[0] << 24) |(p[1] << 16) |(p[2] << 8) |(p[3]);
			goto ret;
		}
	}

ret:
	if (fd >= 0)
		close(fd);

	return ipaddr;
}

unsigned int resolve_dns(char *host)
{
	unsigned int ipaddr;

	ipaddr = inet_addr(host);
	if (ipaddr == INADDR_NONE)
		ipaddr = dns_lookup(host);

	return ipaddr;
}

