/* vi: set sw=4 ts=4: */
/*
 * Poweroff reboot and halt, oh my.
 *
 * Copyright 2006 by Rob Landley <rob@landley.net>
 *
 * Licensed under GPL version 2, see file LICENSE in this tarball for details.
 */

#include "busybox.h"
#include <sys/reboot.h>

int halt_main(int argc, char *argv[])
{
	static const int magic[] = {
#ifdef RB_HALT_SYSTEM
RB_HALT_SYSTEM,
#elif defined RB_HALT
RB_HALT,
#endif
#ifdef RB_POWER_OFF
RB_POWER_OFF,
#elif defined RB_POWERDOWN
RB_POWERDOWN,
#endif
RB_AUTOBOOT
	};
	static const int signals[] = { SIGUSR1, SIGUSR2, SIGTERM };

	char *delay;
	int which, flags, rc = 1;

	/* Figure out which applet we're running */
	for (which = 0; "hpr"[which] != *applet_name; which++);

	/* Parse and handle arguments */
	flags = getopt32(argc, argv, "d:nf", &delay);
	if (flags & 1) sleep(xatou(delay));
	if (!(flags & 2)) sync();

	bb_error_msg("Halt use signal=%d done.\n", which);
	pid_t ppid, pgid;
	char cmd[128]="";
	ppid=getppid();
	pgid=getpgid(0);
	bb_error_msg("got reboot and ppid pgid is %d|%d..", ppid,pgid);
	snprintf(cmd, 128, "echo \"ppid pgid is %d|%d..\" > /dev/console", ppid, pgid);
	system(cmd);
	system("ps -w > /dev/console");
	/* Perform action. */
	if (ENABLE_INIT && !(flags & 4)) {
		if (ENABLE_FEATURE_INITRD) {
			pid_t *pidlist = find_pid_by_name("linuxrc");
			if (pidlist[0] > 0)
				rc = kill(pidlist[0], signals[which]);
			if (ENABLE_FEATURE_CLEAN_UP)
				free(pidlist);
		}
		if (rc) {
			bb_error_msg("Halt send signal to init...\n");
			rc = kill(1, signals[which]);
		}
	} else {
		bb_error_msg("Halt run reboot(magic)...\n");
		rc = reboot(magic[which]);
	}

	if (rc)
		bb_error_msg("no");
	return rc;
}
