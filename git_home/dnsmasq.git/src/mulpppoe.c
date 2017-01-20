/***
 * Add this file to support multi pppoe function.
 *
 */

#include "dnsmasq.h"
#include <net/route.h>


#define RECORD_FILE "/tmp/ppp1_dns_records.file"
#define PPP1_DNS_FILE "/etc/ppp/pppoe2-dns.conf" 

static void mul_pppoe_route_io(struct in_addr i_addr, int addflag);
static void mulppp_addrecord(char *dname, struct in_addr i_addr);

static void mulppp_addrecord(char *dname, struct in_addr i_addr)
{
	int ret = 0;
	FILE *fp = NULL, *ft = NULL, *fw = NULL;
	char rcstr[256] = {0}, line[256] = {0};

	snprintf(rcstr, 256, "#%s#%s#", dname, inet_ntoa(i_addr));
	if ((fp = fopen(RECORD_FILE, "r"))) {
		while (fgets(line, 256, fp)) {	
	                if (strstr(line, rcstr) != NULL) {
	                        ret = 1;
	                        break;
	                }
		}
		fclose(fp);
		if(ret) return;
	} else {
		if ((ft = fopen(RECORD_FILE, "w"))) 
			fclose(ft);
	}

	if (!(fw = fopen(RECORD_FILE, "a+"))) 
		return;
	strcat(rcstr, "\n");
	fputs(rcstr, fw);
	fclose(fw);
	mul_pppoe_route_io(i_addr, 1);
}

static int ignore_dnsserver_route(char *dstip) 
{
	FILE *fp = NULL, *ft = NULL;
	char line[128] = {0};
	int ret = 0;

	if (!(fp = fopen(PPP1_DNS_FILE, "r"))) 
		return 0;

	while(fgets(line, 127, fp)){
		if (strstr(line, dstip) != NULL) 
			ret = 1;
	}
	fclose(fp);
	if(ret) return ret;

	if (!(ft = fopen("/tmp/ppp1gw", "r"))) 
		return 0;

	while(fgets(line, 127, ft)){
		if (strstr(line, dstip) != NULL) 
			ret = 1;
	}
	fclose(ft);
	return ret;
}

static void mulppp_clearrecord(void)
{
	FILE *tfp = NULL, *fw = NULL;  
	char tbuf[256] = {0};
	int ret = 0;

	if (NULL == (tfp = fopen(RECORD_FILE, "r"))) 
		return;
	system("echo 1111 >> /tmp/kkk");
	system("ifconfig ppp1| grep \"P-t-P:\" | awk {'print$3'} | awk -F\":\" {'print$2'} > /tmp/ppp1gw");
	system("echo 1112 >> /tmp/kkk");
	while(fgets(tbuf, sizeof(tbuf), tfp)){
		struct in_addr taddr;
		char abuf[256] = {0};

		ret = sscanf(tbuf,"#%*[^#]#%[^#]", abuf);
		if(ret > 0 && !ignore_dnsserver_route(abuf)) {
			inet_aton(abuf, &taddr);
			mul_pppoe_route_io(taddr, 0);
		}
	}
	fclose(tfp);
	unlink(RECORD_FILE);
	if ((fw = fopen(RECORD_FILE, "w"))) 
		fclose(fw);
}

//ifdef SUP_MUL_PPPOE
static int check_dname_wildcard(struct dname_record *drecord)
{
  char *dnamep = drecord->dname;
  char *buf[128] = {0};
  if (drecord == NULL && dnamep == NULL)
  	return -1;
  if (*dnamep == '*'){
  	drecord->wildcard = 1;
	strcpy(buf, dnamep+1);
	strcpy(dnamep, buf);
  }
  else{
  	drecord->wildcard = 0;
  }
  return 0;
}

static void mul_pppoe_route_io(struct in_addr i_addr, int addflag)
{
  int sock_fd;
  struct rtentry rtp;

  struct sockaddr_in myaddr;
  struct sockaddr_in *my_addr;
  my_addr = &myaddr;
  char devi[] = "ppp1";

  sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sock_fd < 0 )
      goto ret;

  rtp.rt_flags = (RTF_UP|RTF_HOST);
  my_addr->sin_family = AF_INET;
  my_addr->sin_addr = i_addr;
  memcpy(&rtp.rt_dst, my_addr, sizeof(struct sockaddr));

  inet_aton("0.0.0.0",&my_addr->sin_addr);
  memcpy(&rtp.rt_gateway, my_addr, sizeof(struct sockaddr));

  inet_aton("255.255.255.255",&my_addr->sin_addr);
  memcpy(&rtp.rt_genmask, my_addr, sizeof(struct sockaddr));

  rtp.rt_metric = 1;
  rtp.rt_dev = devi;

  if(addflag) {
	  if(ioctl(sock_fd, SIOCADDRT, &rtp) < 0)
	      goto ret;
  } else {
	  if(ioctl(sock_fd, SIOCDELRT, &rtp) < 0)
	      goto ret;
  }

ret:
    if (sock_fd >= 0)
        close(sock_fd);
}

static int load_mul_pppoe_config(char *filename)
{
  FILE *fp = NULL;
  char buf[128] = {0};
  char *sp = " \t\n\t";
  struct dname_record *tmp_rc = NULL;
  struct dname_record *next_rc = NULL;
  if (NULL == (fp = fopen(filename, "r"))) {
      goto err;
  }
  if (dname_list != NULL)
  	for (tmp_rc = dname_list; tmp_rc; tmp_rc = next_rc) {
		next_rc = tmp_rc->next;
		free(tmp_rc);
	}
  dname_list = NULL;
  while (fgets(buf, sizeof(buf), fp)) {
      char *name = strtok(buf, sp);
      if (NULL == name)
         continue;
      tmp_rc = safe_malloc(sizeof(struct dname_record));
      strcpy(tmp_rc->dname, name);
      check_dname_wildcard(tmp_rc);
      my_syslog(LOG_INFO, _("mul_pppoe_config, dname: %s wildcard:%d"), tmp_rc->dname, tmp_rc->wildcard);
      tmp_rc->next = dname_list;
      dname_list = tmp_rc;
  }
  mulppp_clearrecord();
  //system("/sbin/mul_pppoe_dns clear_all_record");
  fclose(fp);
  return 0;

err:
  if (fp)
     fclose(fp);
  return -1;
}

static int inc_check_list( char *dname)
{
	struct dname_record *tmp_rc = NULL;
	char *tmpr = NULL;
	if (dname_list == NULL && dname == NULL)
		return -1;
	for (tmp_rc = dname_list; tmp_rc; tmp_rc = tmp_rc->next) {
		if (tmp_rc->wildcard == 0){
			if (tmp_rc->dname != NULL && strstr(tmp_rc->dname, dname))
				return 0;
		}
		else{
			if (tmp_rc->dname != NULL && (tmpr = strstr(dname, tmp_rc->dname)) != NULL)
				if (0 == strcmp(tmp_rc->dname, tmpr))
					return 0;
		}
	}
	return -1;
}

static void update_config_check(void)
{
	struct stat statbuf_mp;
	if (NULL != dname_check_file && -1 != stat(dname_check_file, &statbuf_mp)) {
		if (statbuf_mp.st_mtime != dname_save_time) {
			if (0 == load_mul_pppoe_config(dname_check_file))
				dname_save_time = statbuf_mp.st_mtime;
		}
	}
}

static int mul_pppoe_function_check(void)
{
	FILE *fp = NULL;
	if((fp = fopen("/etc/ppp/enable_ppp1", "r")) == NULL ){
		my_syslog(LOG_INFO, "ppp1 not enable, return");
		return -1;
	}
	fclose(fp);
	return 0;
}

void check_mul_pppoe_record(HEADER *header, int plen)
{
  unsigned char dname[MAXDNAME] = {0};
  unsigned short int qtype;
  int addrcount = 0;
  int i;
  struct in_addr i_addr[128];
  char *cmd[128] = {'\0'};

  if (mul_pppoe_function_check() == -1)
   return;

  if (NULL == header)
   return;
  if (header->opcode != QUERY || header->rcode != NOERROR)
   return;
  /* it must only one query in this packet*/
  if (ntohs(header->qdcount) != 1)
   return;
  /* it's not answer in this dns reply packet */
  if (ntohs(header->ancount) <= 0)
   return;
  /* get the query dname and query type */
  if (extract_request(header, plen, dname, &qtype) == 0)
   return;
  if (qtype != T_A)
   return;

  update_config_check();
  if (inc_check_list(dname) == -1)
   return;
  my_syslog(LOG_INFO, "start to get_resolve_address");

  get_resolve_address(&addrcount, i_addr, header, plen);

  my_syslog(LOG_INFO, "get addrcount is %d", addrcount);

  if (addrcount == 0)
   return;
  for (i = 0; i < addrcount; i++)
  {
   //sprintf(cmd, "/sbin/mul_pppoe_dns add_record %s %s", dname, inet_ntoa(i_addr[i]));
   //system(cmd);
   mulppp_addrecord(dname, i_addr[i]);
  }
  return;
}
//endif
