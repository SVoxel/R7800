#ifndef BT_IGMP_H
#define BT_IGMP_H

void send_bt_igmp(char *igmp_buf, int size);
int get_if_mac(char mac[], char *ifname);
int get_if_ip(char *ifname);
//void accept_membership_query(uint32 src, uint32 dst, uint32 group, int code);

#endif //BT_IGMP_H
