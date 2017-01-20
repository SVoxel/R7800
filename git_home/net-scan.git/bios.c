/*
  * NBTscan version 1.5.1. Copyright (C) 1999-2003 Alla Bezroutchko.
  * This is a free software and it comes with absolutely no warranty.
  * You can use, distribute and modify it under terms of GNU GPL.
  */

#include "netscan.h"

#define NB_DGRAM		137

int open_bios_socket(void)
{
	int s;
	struct sockaddr_in addr;
	
	s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (s < 0)
		return -1;
	
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0)
		return -1;
	
	return s;
}

static void mangle_qname(char *p) 
{
	int i;

	p[0] = 32;
	p[1] = (('*' >> 4) & 0x0F) + 'A';
	p[2] = ('*' & 0x0F) + 'A';
	for (i = 3; i < 33; i++)
		p[i] = 'A';
	p[i] = '\0';
}

void send_bios_query(int sock, struct in_addr dst_ip) 
{
	static uint16 xid = 0;
	struct sockaddr_in dest;
	struct nb_request req;

	memset(&dest, 0, sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_port = htons(NB_DGRAM);
	dest.sin_addr = dst_ip;

	xid++;
	
	req.xid = htons(xid);
	req.flags = htons(0x0010);
	req.questions = htons(1);
	req.answer = 0;
	req.authority = 0;
	req.additional = 0;
	mangle_qname(req.qname);
	req.qtype = htons(0x21);
	req.qclass = htons(0x01);

	sendto(sock, (char*)&req, sizeof(req), 0, (struct sockaddr *)&dest, sizeof(dest));
}

