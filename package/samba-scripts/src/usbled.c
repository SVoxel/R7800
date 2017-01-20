/*
  * Run a process to set /proc/usbled/state periodically (every 1 second for example)
  * to the proper state.
  */
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>

int main(int argc, char **argv)
{
	struct timeval timo;

	daemon(1, 1);

	for (;;) {
		timo.tv_sec = 1;
		timo.tv_usec = 0;
		select(1, NULL, NULL, NULL, &timo);

		system("/sbin/ledcontrol -n usb -c green -s on");
	}

	return 0;
}

