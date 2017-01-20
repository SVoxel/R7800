#include <stdio.h>
#include <stddef.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

/*
 * dni_system
 * output: the file that the output will be redirected to, or fill with NULL if you don't want to redirect the output
 * cmd: the absolute path of the command (executable binary or script)
 * ...: the argument strings passed to the command, must end with NULL
 * example 1: dni_system(NULL, "/bin/ls", NULL);
 * example 2: dni_system("/tmp/result", "/bin/ls", "-l", NULL);
 */

int dni_system(const char *output, const char *cmd, ...)
{
#define MAX_ARG 100
	int wait_val, pid;
	__sighandler_t save_quit, save_int, save_chld;

	save_quit = signal(SIGQUIT, SIG_IGN);
	save_int = signal(SIGINT, SIG_IGN);
	save_chld = signal(SIGCHLD, SIG_DFL);

	if ((pid = vfork()) < 0) {
		signal(SIGQUIT, save_quit);
		signal(SIGINT, save_int);
		signal(SIGCHLD, save_chld);
		return -1;
	}
	if (pid == 0) {
		int fd, i = 0;
		char *cmdpath;
		va_list args;
		char *argv[MAX_ARG] = {NULL};
		char *p;

		signal(SIGQUIT, SIG_DFL);
		signal(SIGINT, SIG_DFL);
		signal(SIGCHLD, SIG_DFL);

		argv[0] = cmdpath = (char *)cmd;

		va_start(args, cmd);
		for (i = 1;;) {
			p = va_arg(args, char *);
			if (p == NULL)
				break;
			if (i < MAX_ARG)
				argv[i++] = p;
			else {
				printf("warning: drop some argv!\n");
				break;
			}
		}
		va_end(args);

		if (output) {
			if ((fd = open(output, O_RDWR | O_CREAT, 0666)) < 0) {
				printf("can not open %s %s\n", output, strerror(errno));
				_exit(127);
			}
			dup2(fd, STDOUT_FILENO);
			dup2(fd, STDERR_FILENO);
			close(fd);
		}

		execve(cmdpath, (char *const *)argv, __environ);
		_exit(127);
	}
	/* Signals are not absolutly guarenteed with vfork */
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, SIG_IGN);

	if (wait4(pid, &wait_val, 0, 0) == -1)
		wait_val = -1;

	signal(SIGQUIT, save_quit);
	signal(SIGINT, save_int);
	signal(SIGCHLD, save_chld);
	return wait_val;
}
