/* 
 * NETGEAR SPEC:
 * 8.8 Attached Devices
 * We only show IP address, device name and MAC address on the attached devices list.
 *** How to build the attached device table?
 * The attached device table should contain IP address, device name and MAC address.
 * 1. Listen all the ARP packets (including request and reply, not from router itself which means
 * don't record sender MAC/IP address of router) on the LAN and record all sender MAC
 * address, sender IP address and target IP address in the attached ARP device table.
 * 2. Once we receive any reply packet, we should send out name query to that client to get device
 * name.
 * Note: The NetBIOS is only supported by Microsoft Windows system or Linux with Samba. That's
 * why we use this method to build initial table.
 *** How to validate the attached device table?
 * When customer clicks the "Refresh" button, the following steps should be implemented.
 * 1. Send ARP packets to all IP addresses belong to our attach device table. Wait 3 seconds to
 * collect all replies. If any client doesn't reply, then remove it from our attach device table.
 * 2. Please follow "how to build the attached device table" to add any new device in the network
 * to our attach device table.
 * 3. Update the attach device table to the GUI.
 *
 * refer to arping.c - Ping hosts by ARP requests/replies
 *         BY
 * Author:	Alexey Kuznetsov <kuznet@ms2.inr.ac.ru>
 * Busybox port: Nick Fedchik <nick@fedchik.org.ua>
 *
 * net-scan is not so good, but at least can work. :) --- 20071217
 */

#include "netscan.h"

static int sigval;

void signal_pending(int sig)
{
	sigval = sig;
}

void do_signal(int arp_sock, struct sockaddr *me)
{
	struct itimerval tv;
	if (sigval == 0)
		return;

	if (sigval == SIGUSR1) {
		/* To fix bug 22146, call reset_arp_table to set active status of all nodes in the arp_tbl to 0 in the parent process */
		reset_arp_table();
		scan_arp_table(arp_sock, me);
	} else if (sigval == SIGALRM) {
		show_arp_table();
	}

	sigval = 0;
}

int main(int argc, char **argv)
{
	int arp_sock;
	int bios_sock;
	int max_sock;

	fd_set readset;
	struct sigaction sa;
	struct sockaddr me;
	
	arp_sock = open_arp_socket(&me);
	bios_sock = open_bios_socket();
	
	if (arp_sock < 0 ||bios_sock < 0)
		exit(1);

	//unlink(ARP_FILE);

	printf("The attached devices demo is Running ...\n");
	daemon(1, 1);

	memset(&sa, 0, sizeof(sa));
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = signal_pending;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGALRM, &sa, NULL);
	
	if (bios_sock > arp_sock)
		max_sock = bios_sock + 1;
	else
		max_sock = arp_sock + 1;
	
	while (1) {
		int ret;
		int alen = 0;
		int blen = 0;
		
		struct in_addr send_ip;
		struct sockaddr arp_from;
		struct sockaddr_in bios_from;
		
		socklen_t arp_len = sizeof(arp_from);			
		socklen_t bios_len = sizeof(bios_from);

		struct arpmsg	arp_packet;
		char bios_packet[1024];

		FD_ZERO(&readset);
		FD_SET(arp_sock, &readset);
		FD_SET(bios_sock, &readset);

		ret = select(max_sock, &readset, 0, 0, 0);
		if (ret <= 0) {
			if (ret == -1 && errno == EINTR)
				do_signal(arp_sock, &me);
			continue;
		}
				
		if (FD_ISSET(arp_sock, &readset))
			alen = recvfrom(arp_sock, &arp_packet, sizeof(arp_packet), 0, 
				(struct sockaddr *) &arp_from, &arp_len);
		if (FD_ISSET(bios_sock, &readset))
			blen = recvfrom(bios_sock, bios_packet, sizeof(bios_packet), 0, 
				(struct sockaddr *) &bios_from, &bios_len);
		
		/* min arp packet length: 14 ethernet + 28 arp header = 42 */
		if (alen >= 42 && recv_arp_pack(&arp_packet, &send_ip))
			send_bios_query(bios_sock, send_ip);
		if (blen > 0)
			recv_bios_pack(bios_packet, blen, bios_from.sin_addr);

		do_signal(arp_sock, &me);
	}

	return 0;
}


