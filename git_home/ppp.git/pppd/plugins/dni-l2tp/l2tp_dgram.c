/***********************************************************************
*
* dgram.c
*
* Routines for manipulating L2TP datagrams.
*
* Copyright (C) 2002 by Roaring Penguin Software Inc.
*
* This software may be distributed under the terms of the GNU General
* Public License, Version 2, or (at your option) any later version.
*
* LIC: GPL
*
***********************************************************************/
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

#include "l2tp.h"
#include "l2tp_msg.h"

#define PULL_UINT16(buf, cursor, val) \
	do { \
		val = ((__u16) buf[cursor]) * 256 + (__u16) buf[cursor + 1]; \
		cursor += 2; \
	} while(0)

#define PUSH_UINT16(buf, cursor, val) \
	do { \
		buf[cursor] = val / 256; \
		buf[cursor + 1] = val & 0xFF; \
		cursor += 2; \
	} while (0)

#define GET_AVP_LEN(x) ((((x)[0] & 3) * 256) + (x)[1])

static void l2tp_dgram_init(struct l2tp_dgram *dgram)
{
	dgram->bits = 0;
	dgram->version = 2;
	dgram->length = 0;
	dgram->tid = 0;
	dgram->sid = 0;
	dgram->Ns = 0;
	dgram->Nr = 0;

	dgram->payload_len = 0;
	dgram->cursor = 0;
	dgram->alloc_len = MAX_PACKET_LEN;
}

/**********************************************************************
*	Init datagram; sets tid and sid fields; adds message_type AVP
***********************************************************************/
void l2tp_new_control(struct l2tp_dgram *dgram, __u16 msg_type, __u16 tid, __u16 sid)
{
	__u16 val;

	l2tp_dgram_init(dgram);

	dgram->bits = TYPE_BIT | LENGTH_BIT | SEQUENCE_BIT;
	dgram->tid = tid;
	dgram->sid = sid;
	dgram->msg_type = msg_type;

	if (msg_type != MESSAGE_ZLB) {
		val = htons(msg_type);

		l2tp_add_avp(dgram, MANDATORY, sizeof(val), AVP_MESSAGE_TYPE, &val);
	}
}

/**********************************************************************
*  Reads an L2TP datagram off the wire and puts it in dgram.  Adjusts
*  header fields to host byte order.  Keeps reading in a loop unless
*  we hit a control frame or EAGAIN.  This is more efficient than
*  returning to select loop each time if there's lots of traffic. And we only care
*  the control message.
***********************************************************************/
int l2tp_get_dgram(int fd, struct l2tp_dgram *dgram, struct sockaddr_in *from)
{
	int r, tries;
	__u16 cursor;
	socklen_t slen;
	__u16 vendor, avp_type, avp_len;
	__u8 *msg, buf[MAX_PACKET_LEN];

	tries = 5;
	slen = sizeof(*from);
	while (1) {
		if (--tries <= 0)
			return 0;
		r = recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr *)from, &slen);
		if (r <= 0)
			return 0;

		/* Check version; drop frame if not L2TP (ver = 2) */
		if ((buf[1] & VERSION_MASK) != 2)
			continue;

		/* A contrl frame -- break out of loop and handle control frame */
		if ((buf[0] & TYPE_BIT))
			break;
	}

	l2tp_dgram_init(dgram);
	dgram->bits = buf[0];
	dgram->version = buf[1];
	cursor = 2;

	/*
	  * RFC2661: 
	  *	The Length (L) bit MUST be set to 1 for control messages.
	  *	The Sequence (S) bit MUST be set to 1 for control messages.
	  *	The Offset (O) bit MUST be set to 0 (zero) for control messages.
	  */
	#define L2TP_CHK_BITS	(LENGTH_BIT |SEQUENCE_BIT |OFFSET_BIT)
	#define L2TP_VLD_BITS	(LENGTH_BIT |SEQUENCE_BIT)
	if ((dgram->bits & L2TP_CHK_BITS) != L2TP_VLD_BITS)
		return 0;

	PULL_UINT16(buf, cursor, dgram->length);
	if (dgram->length > r)
		return 0;

	PULL_UINT16(buf, cursor, dgram->tid);
	PULL_UINT16(buf, cursor, dgram->sid);
	PULL_UINT16(buf, cursor, dgram->Ns);
	PULL_UINT16(buf, cursor, dgram->Nr);
	if (cursor > dgram->length)
		return 0;

	dgram->payload_len = dgram->length - cursor;
	memcpy(dgram->data, buf + cursor, dgram->payload_len);

	/* Handle ZLB */
	if (dgram->payload_len == 0) {
		dgram->msg_type = MESSAGE_ZLB;
		return 1;
	}

	/*************/
	msg = l2tp_pull_avp(dgram, &avp_type, &avp_len, &vendor);
	if (msg == NULL ||avp_type != AVP_MESSAGE_TYPE ||
	    vendor != VENDOR_IETF ||avp_len != 2)	
		return 0;

	dgram->msg_type = ((__u16)msg[0]) * 256 + (__u16)msg[1];

	return 1;
}

/**********************************************************************
*	Adjusts header fields from host byte order, then sends datagram
***********************************************************************/
int l2tp_send(struct l2tp_conn_mngr *conn, struct l2tp_dgram *dgram)
{
	int cursor = 2;
	socklen_t slen;
	size_t total_len;
	struct sockaddr_in *to;
	__u8 *len_ptr = NULL, buf[MAX_PACKET_LEN + 32];
    
	buf[0] = dgram->bits;
	buf[1] = dgram->version;

	if (dgram->bits & LENGTH_BIT) {
		len_ptr = buf + cursor;
		PUSH_UINT16(buf, cursor, dgram->length);
	}

	PUSH_UINT16(buf, cursor, dgram->tid);
	PUSH_UINT16(buf, cursor, dgram->sid);

	if (dgram->bits & SEQUENCE_BIT) {
		PUSH_UINT16(buf, cursor, dgram->Ns);
		PUSH_UINT16(buf, cursor, dgram->Nr);
	}

	total_len = cursor + dgram->payload_len;
	if (dgram->bits & LENGTH_BIT) {
		*len_ptr++ = total_len / 256;
		*len_ptr = total_len & 255;
	}

	memcpy(buf + cursor, dgram->data, dgram->payload_len);

	slen = sizeof(*to);
	to = &conn->peer_addr;
	return sendto(conn->ctrl_socket, buf, total_len, 0, (struct sockaddr const *)to, slen);
}

/**********************************************************************
* simple is the best, we never handle hidden AVP.
***********************************************************************/
int l2tp_add_avp(struct l2tp_dgram *dgram, int mandatory, __u16 len, __u16 type, void *val)
{
	len += 6; /* adjust from payload len to actual len */
	if ((dgram->cursor + len) > dgram->alloc_len)
		return -1;

	dgram->data[dgram->cursor] = (mandatory ? AVP_MANDATORY_BIT : 0); /* no hidden */
	dgram->data[dgram->cursor] |= (len >> 8);
	dgram->data[dgram->cursor+1] = (len & 0xFF);
	dgram->data[dgram->cursor+2] = 0;
	dgram->data[dgram->cursor+3] = 0; /* vendor is always 'VENDOR_IETF == 0' */
	dgram->data[dgram->cursor+4] = (type >> 8);
	dgram->data[dgram->cursor+5] = (type & 0xFF);

	if (len > 6)
		memcpy(dgram->data + dgram->cursor + 6, val, len - 6);

	dgram->cursor += len;
	dgram->payload_len = dgram->cursor;

	return 0;
}

/**********************************************************************
*	Pulls an AVP out of a received datagram.
***********************************************************************/
__u8 *l2tp_pull_avp(struct l2tp_dgram *dgram, __u16 *type, __u16 *len, __u16 *vendor)
{
	__u8 *ans;

	if (dgram->cursor >= dgram->payload_len)
		return NULL;

	if (dgram->data[dgram->cursor] & (AVP_RESERVED_BITS |AVP_HIDDEN_BIT))
		return NULL;

	*len = ((__u16) (dgram->data[dgram->cursor] & 3)) * 256 +
			dgram->data[dgram->cursor + 1];
	if (*len < 6 || (dgram->cursor + *len) > dgram->payload_len)
		return NULL;

	dgram->cursor += 2;
	PULL_UINT16(dgram->data, dgram->cursor, *vendor);
	PULL_UINT16(dgram->data, dgram->cursor, *type);

	*len -= 6; /* set len to length of value only */
	ans = dgram->data + dgram->cursor;
	dgram->cursor += *len;

	return ans;
}

