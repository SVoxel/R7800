#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "pppnas.h"

void if_fetch(char * ifname, struct net_iface * ife)
{
	int fd;
	struct ifreq ifr;

	memset(ife, 0, sizeof(struct net_iface));

	if ((fd = socket(AF_INET, SOCK_DGRAM, 0 )) < 0)
		return;

	memset(&ifr, 0, sizeof(ifr));
	ifr.ifr_addr.sa_family = AF_INET;

	strcpy(ifr.ifr_name, ifname);
	if (ioctl(fd, SIOCGIFINDEX, &ifr) == 0)
		ife->ifindex= ifr.ifr_ifindex;
	
	strcpy(ifr.ifr_name, ifname);
	if (ioctl(fd, SIOCGIFADDR, &ifr) == 0)
		ife->addr = ((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr;
	
	strcpy(ifr.ifr_name, ifname);
	if (ioctl(fd, SIOCGIFDSTADDR, &ifr) == 0)
		ife->dstaddr = ((struct sockaddr_in *)&ifr.ifr_dstaddr)->sin_addr;

	close(fd);
}

