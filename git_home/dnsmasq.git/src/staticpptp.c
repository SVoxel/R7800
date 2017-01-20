/***
 * Add this file to support static pptp for Russian.
 *
 * As Netgear Spec request:
 * The user configured DNS server address(es) MUST be used by the router to query
 * the IP address of the PPTP server domain name, if a FQDN is used to specify the
 * server. For all other queries, including the queries relayed for a LAN PC, the router
 * MUST query the DNS server(s) with the following sequence:
 *     (i)  The domain name servers assigned from ISP through PPP/PPTP are queried.
 *     If there is a successful DNS answer received, no more domain name server is
 *     queried and the answered IP address is used.
 *     (ii)  User-configured domain name servers. If there is a successful DNS answer
 *     received and the resolved IP is on a subnet different from My IP Address, the
 *     router automatically adds a static host route to the resolved IP address and the
 *     gateway of the route is the Gateway IP Address, if it's Configured. In case there iis
 *     no Gateway IP Address configured, the static route is not added.
 */

#include "dnsmasq.h"

#ifdef SUP_STATIC_PPTP

#define DNS_TIMEOUT 1

void add_static_pptp_record(struct daemon *daemon, char *domain_name, void *packet, int plen)
{
  struct static_pptp_record *tmp_spr = NULL;
  struct static_pptp_record **d_spr = &daemon->sp_record;

  /* if this domain to added was exsited, just return and do nothing. */
  for (tmp_spr = *d_spr; tmp_spr; tmp_spr = tmp_spr->next) {
    if (0 == strcmp(tmp_spr->dname, domain_name))
      return;
  }

  /* add this record to daemon->sp_record. */
  tmp_spr = safe_malloc(sizeof(struct static_pptp_record));
  strcpy(tmp_spr->dname, domain_name);
  memcpy(tmp_spr->pkt, packet, plen);
  tmp_spr->len = plen;
  tmp_spr->t = dnsmasq_time();
  tmp_spr->next = *d_spr;
  *d_spr = tmp_spr;
}

void del_static_pptp_record(struct daemon *daemon, char *domain_name)
{
  struct static_pptp_record *tmp_spr = NULL;
  struct static_pptp_record *next_spr = NULL;
  struct static_pptp_record *new_spr = NULL;
  struct static_pptp_record **d_spr = &daemon->sp_record;

  if (NULL == *d_spr) { /* no any record */
    return;
  }
  for (tmp_spr = *d_spr; tmp_spr; tmp_spr = next_spr) {
    next_spr = tmp_spr->next;
    if (0 == strcmp(tmp_spr->dname, domain_name)) {
      free(tmp_spr);
      continue;
    }
    tmp_spr->next = new_spr;
    new_spr = tmp_spr;
  }
  *d_spr = new_spr;
}

int requery_by_static_pptp_server(struct daemon *daemon, struct static_pptp_record *sp_r)
{
  int rv = 0, rt = 0;
  struct serverfd *tmp_sfd = daemon->sp_sfd;

  /* send dns query to all dns server that get from stitic dns user specified */
  for (; tmp_sfd; tmp_sfd = tmp_sfd->next) {
    rv = sendto(daemon->sfds->fd, sp_r->pkt, sp_r->len, 0, &(tmp_sfd->source_addr.sa), sizeof(struct sockaddr));
    if (-1 == rv)
      rv--;
  }
  return rt;
}

void timer_check_static_pptp_query(struct daemon *daemon)
{
  struct static_pptp_record *tmp_spr = daemon->sp_record;
  time_t now = dnsmasq_time();

  /* check dns query record once a second, if record is not empty and some record
   * was sent in a second ago or more, we think for timeout, and requery it. */
  for (; tmp_spr; tmp_spr = tmp_spr->next) {
    if ((double)DNS_TIMEOUT < difftime(now, tmp_spr->t)) {
      if (0 == requery_by_static_pptp_server(daemon, tmp_spr))
        del_static_pptp_record(daemon, tmp_spr->dname);
    }
  }
}

int load_static_pptp_server(struct daemon *daemon)
{
  FILE *fp = NULL;
  char buf[128] = {0};
  char *sp = " \t\n\t";
  struct serverfd *tmp_sfd = NULL;

  /* no pptp resolv file was specified */
  if (NULL == daemon->pptp_resolv_file) {
    return 0;
  }
  /* detele and free all previous static dns server */
  while (NULL != (tmp_sfd = daemon->sp_sfd)) {
    daemon->sp_sfd = tmp_sfd->next;
    free(tmp_sfd);
  }

  /* read static dns server for static pptp(Russian) */
  if (NULL ==  (fp = fopen(daemon->pptp_resolv_file->name, "r"))) {
    return -1;
  }
  while (fgets(buf, sizeof(buf), fp)) {
    char *token = strtok(buf, sp);
    if (NULL == token || 0 != strcmp(token, "nameserver"))
      continue;
    token = strtok(NULL, sp);
    if (NULL != token) {
      tmp_sfd = safe_malloc(sizeof(struct serverfd));
      if (0 != inet_aton(token, &tmp_sfd->source_addr.in.sin_addr)) {
        printf("dnsmasq: using pptp server - %s\n", token);
        tmp_sfd->source_addr.in.sin_port = htons(NAMESERVER_PORT);
	tmp_sfd->source_addr.in.sin_family = AF_INET;
	tmp_sfd->next = daemon->sp_sfd;
	daemon->sp_sfd = tmp_sfd;
      } else {
        free(tmp_sfd);
      }
    }
  }

  fclose(fp);
  return 0;
}

int load_static_pptp_config(struct daemon *daemon)
{
  FILE *fp = NULL;
  char buf[128] = {0};
  char *sp = " \t\n\t";
  char *iface = "interface";
  char *myip = "myip";
  char *gateway = "gateway";
  char *netmask = "netmask";
  char *resolv = "resolv";

  /* no static pptp config file was specified */
  if (NULL == daemon->static_pptp_conf)
    goto err;
  if (NULL == (fp = fopen(daemon->static_pptp_conf, "r"))) {
    goto err;
  }
  while (fgets(buf, sizeof(buf), fp)) {
    char *name = strtok(buf, sp);
    char *value = strtok(NULL, sp);
    if (NULL == name || NULL == value)
      continue;
    if (0 == strcmp(name, iface)) {
      daemon->wan_iface = safe_malloc(strlen(value) + 1);
      strcpy(daemon->wan_iface, value);
      daemon->wan_iface[strlen(value)] = 0;
    } else if (0 == strcmp(name, myip)) {
      if (0 == inet_aton(value, &daemon->myip))
        goto err;
    } else if (0 == strcmp(name, gateway)) {
      if (0 != inet_aton(value, &daemon->gw))
        daemon->record_static_route += 1;
    } else if (0 == strcmp(name, netmask)) {
      if (0 != inet_aton(value, &daemon->nm))
        daemon->record_static_route += 1;
    } else if (0 == strcmp(name, resolv)) {
      daemon->pptp_resolv_file = safe_malloc(sizeof(struct resolvc));
      daemon->pptp_resolv_file->name = safe_malloc(strlen(value) + 1);
      strcpy(daemon->pptp_resolv_file->name, value);
      daemon->pptp_resolv_file->name[strlen(value)] = 0;
      daemon->pptp_resolv_file->mtime = dnsmasq_time();
    } else {
      goto err;
    }
  }
  fclose(fp);
  return 0;
err:
  if (fp)
    fclose(fp);
  if (daemon->wan_iface)
    free(daemon->wan_iface);
  if (daemon->pptp_resolv_file->name)
    free(daemon->pptp_resolv_file->name);
  if (daemon->pptp_resolv_file)
    free(daemon->pptp_resolv_file);
  return -1;
}

/**
 * check if one host route was exsited in system, return 0 if no and -1 if yes.
 */
int check_static_host_route_for_pptp(struct in_addr addr, char *if_name)
{
  char iface[64]={0};
  unsigned long int d;

  FILE *fp = NULL;
  if (NULL == (fp = fopen("/proc/net/route", "r")))
    return -1;
  if (fscanf(fp, "%*[^\n]\n") < 0) { /* Skip the first line. */
    fclose(fp);
    return -1;
  }
  while (1) {
    int r;
    memset(iface, 0x00, sizeof(iface));
    d = 0;
    r = fscanf(fp, "%s%lx%*lx%*X%*d%*d%*d%*lx%*d%*d%*d\n", iface, &d);
    if ((r < 0) && feof(fp))
      break;
    if ((addr.s_addr == d) && (0 == strcmp(iface, if_name))) {
      fclose(fp);
      return -1;
    }
  }
  fclose(fp);
  return 0;
}

int add_static_host_route_for_pptp(struct in_addr addr, struct in_addr gateway, char *if_name)
{
  int sock = -1;
  struct rtentry rt;

  if ((INADDR_NONE == addr.s_addr) || (INADDR_NONE == gateway.s_addr) || (NULL == if_name))
    return -1;
  if (0 != check_static_host_route_for_pptp(addr, if_name)) /* host route already exsited */
    return -1;

  /* add a host route */
  sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (sock < 0) {
    return -1;
  }
  memset(&rt, 0x00, sizeof(struct rtentry));
  ((struct sockaddr_in *)&rt.rt_dst)->sin_addr = addr;
  ((struct sockaddr_in *)&rt.rt_genmask)->sin_addr.s_addr = INADDR_BROADCAST;
  ((struct sockaddr_in *)&rt.rt_gateway)->sin_addr = gateway;
  rt.rt_flags = RTF_UP | RTF_HOST | RTF_GATEWAY;
  rt.rt_metric = 1;
  rt.rt_dev = if_name;
  rt.rt_dst.sa_family = AF_INET;
  rt.rt_gateway.sa_family = AF_INET;
  rt.rt_genmask.sa_family = AF_INET;
  ioctl(sock, SIOCADDRT, &rt);
  close(sock);

  return 0;
}

void check_static_pptp(void *packet, int plen, struct daemon *daemon)
{
  HEADER *header = NULL;
  unsigned char *aw = NULL;
  unsigned short int type;
  unsigned short int class;
  int len;
  struct in_addr i_addr;

  if (NULL == packet)
    return;
  header = (HEADER *)packet;
  /* gateway or netmask was not specified, will not add staitc route */
  if (2 != daemon->record_static_route)
    return;
  /* it's not answer in this dns reply packet */
  if (header->ancount <= 0)
    return;

  aw = ex_skip_questions(header, plen);
  if (NULL == aw)
    return;
  aw += 2; /* skip domain name */
  GETSHORT(type, aw);
  if (T_A != type)
    return;
  GETSHORT(class, aw);
  aw += 4; /* skip ttl */
  GETSHORT(len, aw);
  GETLONG(i_addr.s_addr, aw);

  /* if ip address in dns reply packet is in a diffrent subnet from MyIP, add static route */
  if ((i_addr.s_addr & daemon->nm.s_addr) != (daemon->myip.s_addr & daemon->nm.s_addr)) {
    add_static_host_route_for_pptp(i_addr, daemon->gw, daemon->wan_iface);
  }
}
#endif
