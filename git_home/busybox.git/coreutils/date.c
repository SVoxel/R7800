/* vi: set sw=4 ts=4: */
/*
 * Mini date implementation for busybox
 *
 * by Matthew Grant <grantma@anathoth.gen.nz>
 *
 * iso-format handling added by Robert Griebl <griebl@gmx.de>
 * bugfixes and cleanup by Bernhard Fischer
 *
 * Licensed under GPLv2 or later, see file LICENSE in this tarball for details.
*/

#include "busybox.h"

/* This 'date' command supports only 2 time setting formats,
   all the GNU strftime stuff (its in libc, lets use it),
   setting time using UTC and displaying it, as well as
   an RFC 2822 compliant date output for shell scripting
   mail commands */

/* Input parsing code is always bulky - used heavy duty libc stuff as
   much as possible, missed out a lot of bounds checking */

/* Default input handling to save surprising some people */


#define DATE_OPT_RFC2822	0x01
#define DATE_OPT_SET		0x02
#define DATE_OPT_UTC		0x04
#define DATE_OPT_DATE		0x08
#define DATE_OPT_REFERENCE	0x10
#define DATE_OPT_TIMESPEC	0x20
#define DATE_OPT_HINT		0x40

static void xputenv(char *s)
{
	if (putenv(s) != 0)
		bb_error_msg_and_die(bb_msg_memory_exhausted);
}

static void maybe_set_utc(int opt)
{
	if (opt & DATE_OPT_UTC)
		xputenv("TZ=UTC0");
}

static char * get_time_zone()
{
#define TIME_ZONE_FILE "/tmp/time_zone"
	int  i = 0;
	static char buf[256];
	FILE *fp;

	buf[0] = '\0';
	if (!(fp = fopen(TIME_ZONE_FILE, "r")))
		return buf;
	fgets(buf, sizeof(buf), fp);
	fclose(fp);

	i = 0;
	while (buf[i] != '\0' && buf[i] != '\r' && buf[i] != '\n')
		i++;
	buf[i] = '\0';

	return buf;
}

int date_main(int argc, char **argv)
{
	time_t tm;
	struct tm tm_time;
	unsigned opt;
	int ifmt = -1;
	char *date_str = NULL;
	char *date_fmt = NULL;
	char *filename = NULL;
	char *time_zone = NULL;
	char *isofmt_arg;
	char *hintfmt_arg;

	opt_complementary = "?:d--s:s--d"
		USE_FEATURE_DATE_ISOFMT(":R--I:I--R");
	opt = getopt32(argc, argv, "Rs:ud:r:"
			USE_FEATURE_DATE_ISOFMT("I::D:"),
			&date_str, &date_str, &filename
			USE_FEATURE_DATE_ISOFMT(, &isofmt_arg, &hintfmt_arg));
	maybe_set_utc(opt);

	if (ENABLE_FEATURE_DATE_ISOFMT && (opt & DATE_OPT_TIMESPEC)) {
		if (!isofmt_arg) {
			ifmt = 0; /* default is date */
		} else {
			const char * const isoformats[] =
				{"date", "hours", "minutes", "seconds"};

			for (ifmt = 0; ifmt < 4; ifmt++)
				if (!strcmp(isofmt_arg, isoformats[ifmt])) {
					break;
				}
			if (ifmt == 4) /* parse error */
				bb_show_usage();
		}
	}

	/* XXX, date_fmt == NULL from this always */
	if ((date_fmt == NULL) && (optind < argc) && (argv[optind][0] == '+')) {
		date_fmt = &argv[optind][1];	/* Skip over the '+' */
	} else if (date_str == NULL) {
		opt |= DATE_OPT_SET;
		date_str = argv[optind];
	}

	/* Now we have parsed all the information except the date format
	   which depends on whether the clock is being set or read */

	if (filename) {
		struct stat statbuf;
		xstat(filename, &statbuf);
		tm = statbuf.st_mtime;
	} else {
		time(&tm);
		time_zone = get_time_zone();
		if (*time_zone != '\0') {
			setenv("TZ", time_zone, 1);
		}
	}
	/* Get local time value according to time zone setting */
	localtime_r(&tm, &tm_time);

	/* Zero out fields - take her back to midnight! */
	if (date_str != NULL) {
		tm_time.tm_sec = 0;
		tm_time.tm_min = 0;
		tm_time.tm_hour = 0;

		/* Process any date input to UNIX time since 1 Jan 1970 */
		if (ENABLE_FEATURE_DATE_ISOFMT && (opt & DATE_OPT_HINT)) {
			strptime(date_str, hintfmt_arg, &tm_time);
		} else if (strchr(date_str, ':') != NULL) {
			/* Parse input and assign appropriately to tm_time */

			if (sscanf(date_str, "%d:%d:%d", &tm_time.tm_hour, &tm_time.tm_min,
								 &tm_time.tm_sec) == 3) {
				/* no adjustments needed */
			} else if (sscanf(date_str, "%d:%d", &tm_time.tm_hour,
										&tm_time.tm_min) == 2) {
				/* no adjustments needed */
			} else if (sscanf(date_str, "%d.%d-%d:%d:%d", &tm_time.tm_mon,
								&tm_time.tm_mday, &tm_time.tm_hour,
								&tm_time.tm_min, &tm_time.tm_sec) == 5) {
				/* Adjust dates from 1-12 to 0-11 */
				tm_time.tm_mon -= 1;
			} else if (sscanf(date_str, "%d.%d-%d:%d", &tm_time.tm_mon,
								&tm_time.tm_mday,
								&tm_time.tm_hour, &tm_time.tm_min) == 4) {
				/* Adjust dates from 1-12 to 0-11 */
				tm_time.tm_mon -= 1;
			} else if (sscanf(date_str, "%d.%d.%d-%d:%d:%d", &tm_time.tm_year,
								&tm_time.tm_mon, &tm_time.tm_mday,
								&tm_time.tm_hour, &tm_time.tm_min,
									&tm_time.tm_sec) == 6) {
				tm_time.tm_year -= 1900;	/* Adjust years */
				tm_time.tm_mon -= 1;	/* Adjust dates from 1-12 to 0-11 */
			} else if (sscanf(date_str, "%d.%d.%d-%d:%d", &tm_time.tm_year,
								&tm_time.tm_mon, &tm_time.tm_mday,
								&tm_time.tm_hour, &tm_time.tm_min) == 5) {
				tm_time.tm_year -= 1900;	/* Adjust years */
				tm_time.tm_mon -= 1;	/* Adjust dates from 1-12 to 0-11 */
			} else {
				bb_error_msg_and_die(bb_msg_invalid_date, date_str);
			}
		} else {
			int nr;
			char *cp;

			nr = sscanf(date_str, "%2d%2d%2d%2d%d", &tm_time.tm_mon,
						&tm_time.tm_mday, &tm_time.tm_hour, &tm_time.tm_min,
						&tm_time.tm_year);

			if (nr < 4 || nr > 5) {
				bb_error_msg_and_die(bb_msg_invalid_date, date_str);
			}

			cp = strchr(date_str, '.');
			if (cp) {
				nr = sscanf(cp + 1, "%2d", &tm_time.tm_sec);
				if (nr != 1) {
					bb_error_msg_and_die(bb_msg_invalid_date, date_str);
				}
			}

			/* correct for century  - minor Y2K problem here? */
			if (tm_time.tm_year >= 1900) {
				tm_time.tm_year -= 1900;
			}
			/* adjust date */
			tm_time.tm_mon -= 1;
		}

		/* Correct any day of week and day of year etc. fields */
		tm_time.tm_isdst = -1;	/* Be sure to recheck dst. */
		tm = mktime(&tm_time);
		if (tm < 0) {
			bb_error_msg_and_die(bb_msg_invalid_date, date_str);
		}
		maybe_set_utc(opt);

		/* if setting time, set it */
		if ((opt & DATE_OPT_SET) && stime(&tm) < 0) {
			bb_perror_msg("cannot set date");
		}
	}

	/* Display output */

	/* Deal with format string */

	if (date_fmt == NULL) {
		int i;
		date_fmt = xzalloc(32);
		if (ENABLE_FEATURE_DATE_ISOFMT && ifmt >= 0) {
			strcpy(date_fmt, "%Y-%m-%d");
			if (ifmt > 0) {
				i = 8;
				date_fmt[i++] = 'T';
				date_fmt[i++] = '%';
				date_fmt[i++] = 'H';
				if (ifmt > 1) {
					date_fmt[i++] = ':';
					date_fmt[i++] = '%';
					date_fmt[i++] = 'M';
				}
				if (ifmt > 2) {
					date_fmt[i++] = ':';
					date_fmt[i++] = '%';
					date_fmt[i++] = 'S';
				}
format_utc:
				date_fmt[i++] = '%';
				date_fmt[i] = (opt & DATE_OPT_UTC) ? 'Z' : 'z';
			}
		} else if (opt & DATE_OPT_RFC2822) {
			/* Undo busybox.c for date -R */
			if (ENABLE_LOCALE_SUPPORT)
				setlocale(LC_TIME, "C");
			strcpy(date_fmt, "%a, %d %b %Y %H:%M:%S ");
			i = 22;
			goto format_utc;
		} else /* default case */
			date_fmt = "%a %b %e %H:%M:%S %Z %Y";
	}

	if (*date_fmt == '\0') {
		/* With no format string, just print a blank line */
		*bb_common_bufsiz1 = 0;
	} else {
		/* Handle special conversions */

		if (strncmp(date_fmt, "%f", 2) == 0) {
			date_fmt = "%Y.%m.%d-%H:%M:%S";
		}

		/* Generate output string */
		strftime(bb_common_bufsiz1, 200, date_fmt, &tm_time);
	}
	puts(bb_common_bufsiz1);

	return EXIT_SUCCESS;
}
