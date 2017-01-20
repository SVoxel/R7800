#include<sys/types.h>
#include<sys/stat.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>

#include "pppnas.h"
#include "dniconfig.h"

extern void start_route(void);
extern void stop_route(void);

static char * pap_secrets[2] = {"/etc/ppp/pap-secrets", "/etc/ppp/pap1-secrets"};
static char * chap_secrets[2] = {"/etc/ppp/chap-secrets", "/etc/ppp/chap1-secrets"};
						
static void config_ppp_options(FILE * fp, char * mtu, char * usr, char * unit)
{
	fprintf(fp, "no-replace-dns\n");
	fprintf(fp, "noipdefault\n");
	fprintf(fp, "hide-password\n");
	fprintf(fp, "noauth\n");

	if ((!strcmp(unit, "1") && strcmp(nvram_get("wan_mulpppoe2_dns_assign"), "1")) ||
             (!strcmp(unit, "0") && strcmp(nvram_get("wan_mulpppoe1_dns_assign"), "1")))
                fprintf(fp, "usepeerdns\n");
	
	fprintf(fp, "mru %s\n", mtu);
	fprintf(fp, "mtu %s\n", mtu);
	fprintf(fp, "noaccomp\n");
	fprintf(fp, "default-asyncmap\n");
	fprintf(fp, "plugin rp-pppoe.so %s\n",WAN_ETH_NAME);

	if (strcmp(unit, "0") == 0 && strcmp(nvram_get("wan_mulpppoe1_wan_assign"), "1") == 0)
		fprintf(fp, "%s:\n", nvram_get("wan_mulpppoe1_ip"));

	if (strcmp(unit, "1") == 0 && strcmp(nvram_get("wan_mulpppoe2_wan_assign"), "1") == 0)
                fprintf(fp, "%s:\n", nvram_get("wan_mulpppoe2_ip"));
	
	fprintf(fp, "user %s\n", usr);
	fprintf(fp, "unit %s\n", unit);
}

static void config_uptime(int unit)
{
	FILE * fs, * fd;
	char time[16];
	char * uptime_files[2] = {
		"/etc/ppp/pppoe1-dial-up-time", 
		"/etc/ppp/pppoe2-dial-up-time"
	};

	if((fs = fopen("/proc/uptime", "r")) != NULL) {
		if((fd = fopen(uptime_files[unit], "w")) != NULL) {
			fscanf(fs, "%s %*s", time);
			fprintf(fd, "%s", time);
			fclose(fd);
		}

		fclose(fs);
	}
}

static void config_status(int unit, char * status)
{
	FILE * fd;
	char * status_files[2] = {
		"/etc/ppp/pppoe1-status", 
		"/etc/ppp/pppoe2-status"
	};

	if ((fd = fopen(status_files[unit], "w")) != NULL) {
		fprintf(fd, "%s", status);
		fclose(fd);
	}
}

static void config_secret_options(int unit, char *user, char *passwd)
{
	FILE * fp;
	
	if ((fp = fopen(pap_secrets[unit], "w")) != NULL) {
		fprintf(fp, "%s    *    %s\n", user, passwd);
		fclose(fp);
	}

	if ((fp = fopen(chap_secrets[unit], "w")) != NULL) {
		fprintf(fp, "%s    *    %s\n", user, passwd);
		fclose(fp);
	}
}

static int kill_pppd_by_pid(int unit)
{
	FILE *fp;
	int pid;
	char * pid_file[2] = {
		"/var/run/ppp0.pid", 
		"/var/run/ppp1.pid"
	};
	
	if ((fp = fopen(pid_file[unit], "r")) == NULL)
		return 0;

	fscanf(fp, "%d", &pid);
	fclose(fp);
	
	kill(pid, SIGTERM);
	
	return 1;
}

/* 
  * If ppd didn't get the IP, we will not find the related PID file. 
  * Let's try to kill them by command line. :-)
  */
static void kill_pppd_by_cmd(int unit)
{
	int pid;
	DIR *dir;
	struct dirent *file;
	char *name, pppd_cmd[64], cmdline[64], buf[128];

	if ((dir = opendir("/proc")) == NULL)
		return; /*It is really so bad!*/

	sprintf(pppd_cmd, "pppoe-session%d", unit + 1);	
	
	for (;;) {
		int fd;
		ssize_t n;
	
		if ((file = readdir(dir)) == NULL) {
			closedir(dir);
			return;
		}
		
		name = file->d_name;
		if (!(*name >= '0' && *name <= '9'))
			continue;
		
		pid = atoi(name);
		sprintf(cmdline, "/proc/%d/cmdline", pid);

		if ((fd = open(cmdline, O_RDONLY)) < 0)
			continue;

		if ((n = read(fd, buf, sizeof(buf) -1)) < 1) {
			close(fd);
			continue;
		}
		
		close(fd);

		if (buf[n-1] == '\n')
			buf[--n] = 0;

		name = buf;
		while (n) {
			if (*name < ' ')  *name = ' ';

			name++; n--;
		}
		*name = 0;

		/* /usr/sbin/pppd call pppoe-session* updetach */
		if (buf[10] != 'p' ||buf[11] != 'p' || buf[12] != 'p' ||buf[13] != 'd')
			continue;

		if (strstr(&buf[14], pppd_cmd) == NULL)
			continue;
		
		kill(pid, SIGTERM); /* Yes, we got it! :) */

		closedir(dir);
		return;
	}
}

static void config_primary_options()
{
	FILE *fp;
	char * val;
	int idle;
	char mtu[8];
	char user[64], passwd[64];

	char * ip_up = "/etc/ppp/pppoe1-ip-up";
	char * ip_down = "/etc/ppp/pppoe1-ip-down";
	
	if ((fp = fopen("/etc/ppp/peers/pppoe-session1", "w")) == NULL)
		return;
	
	idle = atoi(nvram_get("wan_mulpppoe1_idletime"));
	
	if (idle > 0 && strcmp(nvram_get("wan_endis_dod"), "1") == 0) {
		fprintf(fp, "idle %d\n", idle);	
		fprintf(fp, "demand\n");
	} else {
		fprintf(fp, "persist\n");
	}

	fprintf(fp, "lcp-echo-interval 20\n");
	fprintf(fp, "lcp-echo-failure 3\n");

	val = nvram_get("wan_mulpppoe1_service");
	if (strcmp(val, ""))
		fprintf(fp, "remotename %s\n", val);

	fprintf(fp, "defaultroute\n");
	fprintf(fp, "ip-up-script %s\n", ip_up);
	fprintf(fp, "ip-down-script %s\n", ip_down);

	strcpy(mtu, nvram_get("wan_mulppp_mtu"));
	if (strcmp(mtu, "") == 0)
		sprintf(mtu, "1492");
	
	sprintf(user, "%s", nvram_get("wan_mulpppoe1_username"));
	sprintf(passwd, "%s", nvram_get("wan_mulpppoe1_passwd"));
	config_ppp_options(fp, mtu, user, "0");
	fclose(fp);

	if ((fp = fopen(ip_up, "w")) != NULL) {
		fprintf(fp, "#!/bin/sh\n");
		fprintf(fp, "/www/cgi-bin/firewall.sh start\n");
		fprintf(fp, "/usr/sbin/ppp-nas pppoe-session1 up\n");
		fprintf(fp, "/sbin/ledcontrol -n wan -c green -s on\n");
		fclose(fp);
		chmod(ip_up, 0731);
	}
	
	if ((fp = fopen(ip_down, "w")) != NULL) {
		fprintf(fp, "#!/bin/sh\n");
		fprintf(fp, "/usr/sbin/ppp-nas pppoe-session1 down\n");
		fprintf(fp, "/sbin/ledcontrol -n wan -c amber -s on\n");
		fclose(fp);
		chmod(ip_down, 0731);
	}
	
	config_secret_options(0, user, passwd);
}

static void config_second_options()
{
	FILE *fp;
	char * val;
	char mtu[8];
	char user[64], passwd[64];

	char * ip_up = "/etc/ppp/pppoe2-ip-up";
	char * ip_down = "/etc/ppp/pppoe2-ip-down";
	
	if ((fp = fopen("/etc/ppp/peers/pppoe-session2", "w")) == NULL)
		return;
	
	val = nvram_get("wan_mulpppoe2_servicename");
	if (strcmp(val, ""))
		fprintf(fp, "remotename %s\n", val);
	
	fprintf(fp, "nodefaultroute\n");
	fprintf(fp, "persist\n");
	fprintf(fp, "lcp-echo-interval 20\n");
	fprintf(fp, "lcp-echo-failure 3\n");
	fprintf(fp, "ip-up-script %s\n", ip_up);
	fprintf(fp, "ip-down-script %s\n", ip_down);
	fprintf(fp, "pap-file %s\n", pap_secrets[1]);
	fprintf(fp, "chap-file %s\n", chap_secrets[1]);

	strcpy(mtu, nvram_get("wan_mulppp_mtu"));
	if (strcmp(mtu, "") == 0)
		sprintf(mtu, "1492");
	
	sprintf(user, "%s", nvram_get("wan_mulpppoe2_username"));
	sprintf(passwd, "%s", nvram_get("wan_mulpppoe2_password")); 
	config_ppp_options(fp, mtu, user, "1");
	fclose(fp);
	
	if ((fp = fopen(ip_up, "w")) != NULL) {
		fprintf(fp, "#!/bin/sh\n");
		fprintf(fp, "/usr/sbin/net-wall start\n");
		fprintf(fp, "/usr/sbin/ppp-nas pppoe-session2 up\n");
		fclose(fp);
		chmod(ip_up, 0731);
	}
	
	if ((fp = fopen(ip_down, "w")) != NULL) {
		fprintf(fp, "#!/bin/sh\n");
		fprintf(fp, "/usr/sbin/ppp-nas pppoe-session2 down\n");
		fclose(fp);
		chmod(ip_down, 0731);
	}

	config_secret_options(1, user, passwd);
}

static void primary_session_up(void)
{
	FILE * fp;

	if ((fp = fopen("/etc/ppp/pppoe1-ip", "w")) != NULL) {
		struct net_iface iface;
		char ipaddr[16];

		if_fetch("ppp0", &iface);
		inet_ntop(AF_INET, &iface.addr, ipaddr, sizeof(ipaddr));
		
		fputs(ipaddr, fp);
		fclose(fp);
	}	
	
	config_uptime(0);

	config_status(0, "1");

	system("/sbin/check_route");
}

static void primary_session_down(void)
{
	config_status(0, "0");
}

static void start_primary_session(void)
{
	FILE *fp;

	if ((fp = fopen("/var/run/ppp1.pid", "r")) == NULL) {
		if ((fp = fopen("/tmp/resolv.conf", "w")) != NULL) {
			fputs("", fp);
			fclose(fp);
		}
	} else {
		fclose(fp);
	}

	config_primary_options();

	system("/usr/sbin/pppd call pppoe-session1 updetach");
}

static void stop_primary_session(void)
{
	if (kill_pppd_by_pid(0) == 0)
		kill_pppd_by_cmd(0);
}

int primary_session(char * cmd)
{
	if (strcmp(cmd, "restart") == 0) {
		stop_primary_session();
		sleep(2);
		start_primary_session();
	} 
	else if (strcmp(cmd, "start") == 0)
		start_primary_session();
	else if (strcmp(cmd, "stop") == 0)
		stop_primary_session();
	else if (strcmp(cmd, "up") == 0)
		primary_session_up();
	else if (strcmp(cmd, "down") == 0)
		primary_session_down();

	return 0;
}

static void second_session_up(void)
{
	start_route();
	
	config_uptime(1);

	config_status(1, "1");

	system("/sbin/check_route");
}

static void second_session_down(void)
{
	stop_route();
	
	config_status(1, "0");
}
 
static void start_second_session(void)
{
	FILE *fp;
	
	if (strcmp(nvram_get("wan_enable_session2"), "0") == 0)
		return;
	
	if ((fp = fopen("/var/run/ppp0.pid", "r")) == NULL) {
		if((fp = fopen("/tmp/resolv.conf", "w")) != NULL) {
			fputs("", fp);
			fclose(fp);
		}
	} else {
		fclose(fp);
	}

	config_second_options();
	
	system("/usr/sbin/pppd call pppoe-session2 updetach");
}

static void stop_second_session(void)
{
	if (kill_pppd_by_pid(1) == 0)
		kill_pppd_by_cmd(1);
}

int second_session(char * cmd)
{
	if (strcmp(cmd, "restart") == 0) {
		stop_second_session();
		sleep(2);
		start_second_session();
	}
	else if (strcmp(cmd, "start") == 0)
		start_second_session();
	else if(strcmp(cmd, "stop") == 0)
		stop_second_session();
	else if(strcmp(cmd, "up") == 0)
		second_session_up();
	else if(strcmp(cmd, "down") == 0)
		second_session_down();

	return 0;
}

