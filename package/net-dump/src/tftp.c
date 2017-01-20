/* -------------------------------------------------------------------------
 * tftp.c
 *
 * A simple tftp client for busybox.
 * Tries to follow RFC1350.
 * Only "octet" mode supported.
 * Optional blocksize negotiation (RFC2347 + RFC2348)
 *
 * Copyright (C) 2001 Magnus Damm <damm@opensource.se>
 *
 * Parts of the code based on:
 *
 * atftp:  Copyright (C) 2000 Jean-Pierre Lefebvre <helix@step.polymtl.ca>
 *                        and Remi Lefebvre <remi@debian.org>
 *
 * utftp:  Copyright (C) 1999 Uwe Ohse <uwe@ohse.de>
 *
 * Licensed under GPLv2 or later, see file LICENSE in this tarball for details.
 *
 * Modified for net-dump using by haiyue @ Delta Networks Inc. 2008-02-24
 * ------------------------------------------------------------------------- */

#include "ethnet.h"

#define TFTP_BLOCKSIZE	512	  /* according to RFC 1350, don't change */
#define TFTP_TIMEOUT		5	  /* seconds */
#define TFTP_NUM_RETRIES	5 /* number of retries */

/* opcodes we support */
#define TFTP_RRQ		1
#define TFTP_WRQ	2
#define TFTP_DATA	3
#define TFTP_ACK		4
#define TFTP_ERROR	5
#define TFTP_OACK	6

void tftp_put(char *buf, int len, void *param)
{
	/***************************************************/
	static uint16 block_nr;
	uint16 opcode, rbuf[2]; /* rbuf: 2.opcode + 2.block_nr = 4 may be OK. */

#define OP_CODE() (rbuf[0])

	int num;
	int i, xlen, ret, timeout;
	socklen_t fromlen;
	fd_set rfds;	
	struct timeval tv;
	struct sockaddr_in dest, from;
	struct tftp_param *tparam;
	char *cp, *xbuf, req[TFTP_BLOCKSIZE];

	i = 0;
	tparam = (struct tftp_param *)param;
	opcode = tparam->filename != 0 ? TFTP_WRQ : TFTP_DATA;

	memset(&dest, 0, sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_addr.s_addr = tparam->serverip;

	if (len < PER_2KB) /* the last sending ... */
		num = (len + TFTP_BLOCKSIZE) / TFTP_BLOCKSIZE;
	else
		num = PER_2KB / TFTP_BLOCKSIZE; /* len MUST be equ PER_2KB */

	while (1) {
		if (opcode == TFTP_DATA) {
			if (i == num)
				break;
			xbuf = &buf[i * TFTP_BLOCKSIZE] - 4;
			i++;
		} else {
			xbuf = req;
		}
		
		cp = xbuf;
		*((uint16*)cp) = htons(opcode);
		cp += 2;

		/* mostly is TFTP_DATA */
		if (opcode == TFTP_DATA) {
			int dlen = len < TFTP_BLOCKSIZE ? len : TFTP_BLOCKSIZE;
			len -= dlen;
			block_nr++;
			*((uint16*)cp) = htons(block_nr);
			cp += (2 + dlen);
		} else if (opcode == TFTP_WRQ) {
			int nlen = strlen(tparam->filename) + 1;
			strncpy(cp, tparam->filename, nlen);
			cp += nlen;
			
			memcpy(cp, "octet", sizeof("octet"));
			cp += sizeof("octet");
		}

		/* send packet */
		timeout = TFTP_NUM_RETRIES;
		xlen = cp - xbuf;
		dest.sin_port = tparam->port;
		do {
			ret = sendto(tparam->socketfd, xbuf, xlen, 0,
					(struct sockaddr const *)&dest, sizeof(dest));
			if (ret < 0)
				return;

			/* receive packet */
		recv_again:
			tv.tv_sec = TFTP_TIMEOUT;
			tv.tv_usec = 0;

			FD_ZERO(&rfds);
			FD_SET(tparam->socketfd, &rfds);
			ret = select(tparam->socketfd + 1, &rfds, NULL, NULL, &tv);
				
			switch (ret) {
			case 1:
				fromlen = sizeof(from);			
				if (recvfrom(tparam->socketfd, rbuf, sizeof(rbuf), 0, 
						(struct sockaddr *)&from, &fromlen) < 0)
					return;
				/* Our first query went to port 69
				 * but reply will come from different one.
				 * Remember and use this new port */
				tparam->port = from.sin_port;
				timeout = 0;
				break;
			case 0:
				if (--timeout == 0)
					return;
				break;
			default:
				goto recv_again;
				break;
			}
		} while(timeout);

		if (ntohs(OP_CODE()) != TFTP_ACK)
			return;

		opcode = TFTP_DATA;
	}

	tparam->filename = 0;
}


