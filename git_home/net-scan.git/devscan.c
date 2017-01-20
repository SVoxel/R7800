#include "netscan.h"

#define DEV_FILE		"/tmp/netscan/ip_name_list"
#define HDR_SIZE		sizeof(struct nb_response_header)

void get_dev_name(char *buf, int len, struct in_addr from)
{
	uint16 num;
	uint8 *p, *e;
	FILE *fp;
	struct nb_response_header *resp;
	char *tmp;

	if (len < HDR_SIZE)
		return;
	
	resp = (struct nb_response_header *)buf;
	num = resp->num_names;
	p = (uint8*)&buf[HDR_SIZE];
	e = p + (num * 18);
	for (; p < e; p += 18)
		if (p[15] == 0 && (p[16] & 0x80) == 0)
			break;
	if (p == e)
		return;

	if ((fp = fopen(DEV_FILE, "w")) == 0) 
		return;

	tmp = (char *)p;
	fprintf(fp, "%s %s\n", inet_ntoa(from), 
		tmp[0] == '\0' ? "&lt;unknown&gt;" : tmp);

	fclose(fp);
}

int main(int argc, char **argv)
{
	char host[32];
	int bios_sock;
	fd_set readset;
		
	struct in_addr send_ip;
	struct sockaddr_in bios_from;					
	socklen_t bios_len = sizeof(bios_from);
	char bios_packet[1024];
	struct timeval to;
	int ret, blen = 0;

	if (argc != 2)
		exit (1);
	
	bios_sock = open_bios_socket();
	if (bios_sock < 0)
		return -1;

	FD_ZERO(&readset);
	FD_SET(bios_sock, &readset);

	strcpy(host, argv[1]);
	send_ip.s_addr = inet_addr(host);
	send_bios_query(bios_sock, send_ip);

	to.tv_sec = 1;
	to.tv_usec = 0;
	ret = select(bios_sock + 1, &readset, 0, 0, &to);
	if (ret <= 0) 
		return -1;

	if (FD_ISSET(bios_sock, &readset))
		blen = recvfrom(bios_sock, bios_packet, sizeof(bios_packet), 0, 
			(struct sockaddr *) &bios_from, &bios_len);
	
	if (blen > 0 && send_ip.s_addr == bios_from.sin_addr.s_addr)
		get_dev_name(bios_packet, blen, bios_from.sin_addr);
	
	return 0;
}

