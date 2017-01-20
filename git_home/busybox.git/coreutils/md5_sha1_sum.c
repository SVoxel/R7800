/* vi: set sw=4 ts=4: */
/*
 *  Copyright (C) 2003 Glenn L. McGrath
 *  Copyright (C) 2003-2004 Erik Andersen
 *
 * Licensed under the GPL v2 or later, see the file LICENSE in this tarball.
 */

#include "busybox.h"

#define FLAG_SILENT	1
#define FLAG_CHECK	2
#define FLAG_WARN	4

int md5_sha1_sum_main(int argc, char **argv)
{
	int return_value = EXIT_SUCCESS;
	uint8_t *hash_value;
	unsigned flags;
	hash_algo_t hash_algo = ENABLE_MD5SUM
		? (ENABLE_SHA1SUM ? (**argv=='m' ? HASH_MD5 : HASH_SHA1) : HASH_MD5)
		: HASH_SHA1;

	if (ENABLE_FEATURE_MD5_SHA1_SUM_CHECK)
		flags = getopt32(argc, argv, "scw");
	else optind = 1;

	if (ENABLE_FEATURE_MD5_SHA1_SUM_CHECK && !(flags & FLAG_CHECK)) {
		if (flags & FLAG_SILENT) {
			bb_error_msg_and_die
				("-%c is meaningful only when verifying checksums", 's');
		} else if (flags & FLAG_WARN) {
			bb_error_msg_and_die
				("-%c is meaningful only when verifying checksums", 'w');
		}
	}

	if (argc == optind) {
		argv[argc++] = "-";
	}

	if (ENABLE_FEATURE_MD5_SHA1_SUM_CHECK && (flags & FLAG_CHECK)) {
		FILE *pre_computed_stream;
		int count_total = 0;
		int count_failed = 0;
		char *file_ptr = argv[optind];
		char *line;

		if (optind + 1 != argc) {
			bb_error_msg_and_die
				("only one argument may be specified when using -c");
		}

		pre_computed_stream = stdin;
		if (NOT_LONE_DASH(file_ptr)) {
			pre_computed_stream = xfopen(file_ptr, "r");
		}

		while ((line = xmalloc_getline(pre_computed_stream)) != NULL) {
			char *filename_ptr;

			count_total++;
			filename_ptr = strstr(line, "  ");
			/* handle format for binary checksums */
			if (filename_ptr == NULL) {
				filename_ptr = strstr(line, " *");
			}
			if (filename_ptr == NULL) {
				if (flags & FLAG_WARN) {
					bb_error_msg("invalid format");
				}
				count_failed++;
				return_value = EXIT_FAILURE;
				free(line);
				continue;
			}
			*filename_ptr = '\0';
			filename_ptr += 2;

			hash_value = hash_file(filename_ptr, hash_algo);

			if (hash_value && (strcmp((char*)hash_value, line) == 0)) {
				if (!(flags & FLAG_SILENT))
					printf("%s: OK\n", filename_ptr);
			} else {
				if (!(flags & FLAG_SILENT))
					printf("%s: FAILED\n", filename_ptr);
				count_failed++;
				return_value = EXIT_FAILURE;
			}
			/* possible free(NULL) */
			free(hash_value);
			free(line);
		}
		if (count_failed && !(flags & FLAG_SILENT)) {
			bb_error_msg("WARNING: %d of %d computed checksums did NOT match",
						 count_failed, count_total);
		}
		/*
		if (fclose_if_not_stdin(pre_computed_stream) == EOF) {
			bb_perror_msg_and_die("cannot close file %s", file_ptr);
		}
		*/
	} else {
		while (optind < argc) {
			char *file_ptr = argv[optind++];

			hash_value = hash_file(file_ptr, hash_algo);
			if (hash_value == NULL) {
				return_value = EXIT_FAILURE;
			} else {
				printf("%s  %s\n", hash_value, file_ptr);
				free(hash_value);
			}
		}
	}
	return return_value;
}
