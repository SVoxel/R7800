#include "defs.h"

static int bt_socket; //the socket for sending igmp packet to wan2
static int g_ifindex;

int get_if_mac(char mac[], char *ifname)
{
	int s = socket(AF_INET, SOCK_DGRAM, 0);
	if(s < 0) {
		perror("build socket fail.");
		return 1;
	}
	struct ifreq ifr;
	strcpy(ifr.ifr_name, ifname);
	ioctl(s, SIOCGIFHWADDR, &ifr);
	memcpy(mac, ifr.ifr_hwaddr.sa_data, 6);
	close(s);

	return 0;
}

int get_if_ip(char *ifname)
{
	int s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0) {
		perror("build socket fail.");
		return 0;
	}

	struct ifreq ifr;
	struct sockaddr_in *sin;
	strcpy(ifr.ifr_name, ifname);
	ioctl(s, SIOCGIFADDR, &ifr);
	sin = (struct sockaddr_in *)&ifr.ifr_addr;
	close(s);

	return sin->sin_addr.s_addr;
}


static int build_mac_hdr(struct ethhdr *eh)
{
	unsigned char dst[6] = {0x01, 0x00, 0x5e, 0x00, 0x00, 0x16};
	unsigned char src[6] = {0};
	get_if_mac(src, WAN_IF);
	
	memcpy(eh->h_dest, dst, 6);
	memcpy(eh->h_source, src, 6);
	eh->h_proto = htons(0x0800);

	return 0;
}

static int build_bt_igmp(char *igmp_buf, int size, char *bt_igmp)
{
	char *p = bt_igmp;
	struct ethhdr *eh = (struct ethhdr *)malloc(sizeof(struct ethhdr));
	memset(eh, 0, sizeof(struct ethhdr));
	build_mac_hdr(eh);
	
	memcpy(p, eh, sizeof(struct ethhdr));
	p += sizeof(struct ethhdr);

	memcpy(p, igmp_buf, size);
	struct iphdr *iph = (struct iphdr*)p;
	//iph->saddr = htonl(get_if_ip(LAN_IF));
	iph->saddr = 0x0A0A0A0A;
	iph->check = 0;
	iph->check = inetChksum((unsigned char *)p, iph->ihl*4);

	return 0;
}

void send_bt_igmp_report(char *igmp_buf, int size)
{
	struct iphdr *iph = (struct iphdr *)igmp_buf;
	struct igmphdr *igmph = (struct igmphdr*)(igmp_buf + iph->ihl*4);
	if(igmph->type != IGMP_V1_MEMBERSHIP_REPORT && igmph->type != IGMP_V2_MEMBERSHIP_REPORT &&igmph->type != IGMP_V2_LEAVE_GROUP && igmph->type != IGMP_V3_MEMBERSHIP_REPORT) { /* 0x16/0x22: join, 0x17: leave */
		perror("send bt igmp packet should be join/leave");
		return;
	}

	char *bt_igmp = (char *)malloc(sizeof(struct ethhdr) + size);
	memset(bt_igmp, 0, size + sizeof(struct ethhdr));
	build_bt_igmp(igmp_buf, size, bt_igmp);

	struct sockaddr_ll addr;
	memset(&addr, 0, sizeof(addr));
	addr.sll_protocol = htons(ETH_P_IP);
	addr.sll_ifindex = g_ifindex;
	addr.sll_family = AF_PACKET;
	memcpy(addr.sll_addr, ((struct ethhdr*)bt_igmp)->h_source, 6);

	/*
	int optval= 1;
	const int *poptval = &optval;
	setsockopt(bt_socket, IPPROTO_IP, IP_HDRINCL, poptval, sizeof(optval));
	*/

	if (sendto(bt_socket, bt_igmp, sizeof(struct ethhdr) + size, 0, &addr, sizeof(struct sockaddr_ll)) == -1) {
		perror("send bt igmp fail");
	}
}

void send_bt_igmp_query(char *igmp_buf, int size)
{
	struct iphdr *iph = (struct iphdr *)igmp_buf;
	struct igmp *igmp = (struct igmp*)(igmp_buf + iph->ihl*4);

	register uint32 src, dst, group;
	src = iph->saddr;
	dst = iph->daddr;
	group = igmp->igmp_group.s_addr;
	accept_membership_query(src, dst, group, igmp->igmp_code);
}

void enable_bt_igmp(int group)
{
	char cmd[126];
	sprintf(cmd, "echo %x > /proc/bt_igmp_group", group);
	system(cmd);
}

char *device;

void pcap_handle(u_char* user,const struct pcap_pkthdr* header,const u_char* pkt_data)
{
    struct iphdr *ip_header;
    int eth_len;
    if(strcmp(device,PPP_IF) == 0) { //PPP
	//fprintf(stderr, "pcap_handle2: the package should be PPP\n");
	//return;
	eth_len = 16; //it is for linux link capture;
	ip_header = (struct iphdr*)(pkt_data+eth_len);
    } else {
	eth_len = 14;
	ip_header = (struct iphdr*)(pkt_data+eth_len);
    }
    struct igmphdr *igmp_header = (struct igmphdr*)(pkt_data + eth_len + ip_header->ihl*4);
    if(igmp_header->type == IGMP_V1_MEMBERSHIP_REPORT || igmp_header->type == IGMP_V2_MEMBERSHIP_REPORT || igmp_header->type == IGMP_V2_LEAVE_GROUP || igmp_header->type == IGMP_V3_MEMBERSHIP_REPORT) {
	send_bt_igmp_report(pkt_data+eth_len, (int)header->len-eth_len);
	return;
    } else if(igmp_header->type == IGMP_MEMBERSHIP_QUERY) {
	send_bt_igmp_query(pkt_data+eth_len, (int)header->len-eth_len);
	return;
    }
}



int pcap_ppp()
{
    char *wan_proto = config_get("wan_proto");
    if(!strcmp(wan_proto, "pppoe") || !strcmp(wan_proto, "l2tp") || !strcmp(wan_proto, "pptp"))
	device = PPP_IF;
    else
	device = WAN_IF;
    char errbuf[1024];
    pcap_t *phandle;
    
    bpf_u_int32 ipaddress,ipmask;
    struct bpf_program fcode;
    int datalink;
    
    /*
    if((device=pcap_lookupdev(errbuf))==NULL){
        perror(errbuf);
        return 1;
    }
    else
        printf("device: %s\n",device);
    */
    
	bt_socket = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_IP));
	if(bt_socket < 0) {
		perror("build bt socket fail.");
		return;
	}

	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	struct ifreq req;
	strcpy(req.ifr_name, WAN_IF);

	ioctl(sock, SIOCGIFINDEX, &req);
	g_ifindex = req.ifr_ifindex;

    phandle=pcap_open_live(device,800,0,500,errbuf);
    if(phandle==NULL){
        perror(errbuf);
        return 1;
    }
    
    if(pcap_lookupnet(device,&ipaddress,&ipmask,errbuf)==-1){
        perror(errbuf);
        return 1;
    }
    else{
        char ip[INET_ADDRSTRLEN],mask[INET_ADDRSTRLEN];
        if(inet_ntop(AF_INET,&ipaddress,ip,sizeof(ip))==NULL)
            perror("inet_ntop error");
        else if(inet_ntop(AF_INET,&ipmask,mask,sizeof(mask))==NULL)
            perror("inet_ntop error");
        printf("IP address: %s, Network Mask: %s\n",ip,mask);
    }
    
#if	0
    int flag=1;
    while(flag){
        //input the design filter
        printf("Input packet Filter: ");
        char filterString[1024];
        scanf("%s",filterString);
        
        if(pcap_compile(phandle,&fcode,filterString,0,ipmask)==-1)
            fprintf(stderr,"pcap_compile: %s,please input again....\n",pcap_geterr(phandle));
        else
            flag=0;
    }
#else
    int wan_ip;
    wan_ip = get_if_ip(device);
    char filterString[1024];
    sprintf(filterString, "((igmp[0]==0x22||igmp[0]==0x17||igmp[0]==0x16)&&ip[12:4]==0x%x)||(igmp[0]==0x11&&ip[12:4]!=0x%x&&ip[12:4]!=0x%x)",htonl(wan_ip), htonl(wan_ip), htonl(get_if_ip(LAN_IF))); //filter IGMP report or Query packet.
    fprintf(stderr, "filterString: %s\n", filterString);
    if(pcap_compile(phandle,&fcode,filterString,0,ipmask)==-1) {
	fprintf(stderr,"pcap_compile: %s\n",pcap_geterr(phandle));
	fprintf(stderr, "filterString: %s\n", filterString);
	return 1;
    }
#endif
    
    if(pcap_setfilter(phandle,&fcode)==-1){
        fprintf(stderr,"pcap_setfilter: %s\n",pcap_geterr(phandle));
        return 1;
    }
    
    if((datalink=pcap_datalink(phandle))==-1){
        fprintf(stderr,"pcap_datalink: %s\n",pcap_geterr(phandle));
        return 1;
    }
    
    printf("datalink= %d\n",datalink);

//    daemon(0,0);    
    pcap_loop(phandle,-1,pcap_handle,NULL);
    
	close(sock);
	close(bt_socket);

    return 0;
}

void fork_pcap_ppp()
{
	pid_t pid;
	if((pid = fork()) < 0) {
		fprintf(stderr, "fork error\n");
	} else if(pid == 0) {
		if((pid = fork()) < 0) {
			fprintf(stderr, "fork error\n");
		} else if(pid > 0) {
			exit(0);
		}
		pcap_ppp();
		exit(0);
	}
	if(waitpid(pid, NULL, 0) != pid)
		fprintf(stderr, "waitpid error\n");
}
