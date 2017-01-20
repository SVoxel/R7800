/*
 *  Copyright (C) 2003 Glenn L. McGrath
 *  Copyright (C) 2003-2004 Erik Andersen
 *
 * Licensed under the GPL v2 or later, see the file LICENSE in this tarball.
 */

#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "busybox.h"

/* This might be useful elsewhere */
unsigned char *hash_bin_to_hex(unsigned char *hash_value,
				unsigned hash_length)
{
	/* xzalloc zero-terminates */
	char *hex_value = xzalloc((hash_length * 2) + 1);
	bin2hex(hex_value, (char*)hash_value, hash_length);
	return hex_value;
}

uint8_t *hash_file(const char *filename, hash_algo_t hash_algo)
{
	int src_fd, hash_len, count;
	union _ctx_ {
		sha1_ctx_t sha1;
		md5_ctx_t md5;
	} context;
	uint8_t *hash_value = NULL;
	RESERVE_CONFIG_UBUFFER(in_buf, 4096);
	void (*update)(const void*, size_t, void*);
	void (*final)(void*, void*);

	src_fd = STDIN_FILENO;
	if (NOT_LONE_DASH(filename)) {
		src_fd = open(filename, O_RDONLY);
		if (src_fd < 0) {
			bb_perror_msg("%s", filename);
			return NULL;
		}
	}

	/* figure specific hash algorithims */
	if (hash_algo==HASH_MD5) {
		md5_begin(&context.md5);
		update = (void (*)(const void*, size_t, void*))md5_hash;
		final = (void (*)(void*, void*))md5_end;
		hash_len = 16;
	} else if (hash_algo==HASH_SHA1) {
		sha1_begin(&context.sha1);
		update = (void (*)(const void*, size_t, void*))sha1_hash;
		final = (void (*)(void*, void*))sha1_end;
		hash_len = 20;
	} else {
		bb_error_msg_and_die("algorithm not supported");
	}

	while (0 < (count = safe_read(src_fd, in_buf, 4096))) {
		update(in_buf, count, &context);
	}

	if (count == 0) {
		final(in_buf, &context);
		hash_value = hash_bin_to_hex(in_buf, hash_len);
	}

	RELEASE_CONFIG_BUFFER(in_buf);

	if (src_fd != STDIN_FILENO) {
		close(src_fd);
	}

	return hash_value;
}


