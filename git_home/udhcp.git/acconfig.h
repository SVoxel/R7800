/*
 * DNI udhcp config.h
 */
#ifndef _DNI_UDHCP_CONFIG_H
#define _DNI_UDHCP_CONFIG_H

@TOP@
/*
 * Netgear router spec V1.6 4.1 : Netgear router SHOULD support fixed IP
 * settings via editing the Address Reservation Table to specific clients
 */
#undef DHCPD_STATIC_LEASE

/*
 * The name of the connected PC can't be got by NETBIOS query sometimes,
 * and the PC host name is sent in DHCP Request packet, list all DHCP clients
 * host names for showing attached devices name if unknown by 'net-scan'.
 */
#undef DHCPD_SHOW_HOSTNAME

/*
 * Netgear router spec V1.6 4.4 WAN/LAN IP conflict detection.
 * when wan port in dhcp client mode get ip, do IP conflict detection
 */
#undef WAN_LAN_IPCONFLICT

/*
 * NETGEAR router spec V1.6, chapter 5.1 Classless Route Option support in dhcpc
 * option 121 (RFC 3442)
 */
#undef RFC3442_121_SUPPORT

/*
 * NETGEAR router spec V1.6, chapter 5.1 Classless Route Option support in dhcpc
 * option 33 (RFC 2132) support
 */
#undef RFC2132_33_SUPPORT

/*
 * check whether the request IP is the same with sever's IP
 */
#undef DHCPD_CHECK_SERVER_IP

/*
 * NETGEAR Router Spec V1.8, chapter 16.2.3 TR-69
 * If TR_069 function is enabled, and the WAN type is DHCP mode, then the CPE should
 * include the string "dslforum.org"(all lower case) any where in the Vendor Class
 * Identifier (DHCP option 60) to let DHCP server to identify this device as supporting
 * this method ......
 * ...
 *
 * If selected, udhcpc can change the Vendor ID by "-V [YOU_ID]"
 */
#undef SUPPORT_OPTION_60

/*
 * NETGEAR Router Spec V1.8, chapter 16.2.3 TR-69
 * ...
 * and obtain the parameter ManagementServer.URL in option 43 and recognize it as ACS URL
 * address and query it IP address via DNS again
 */
#undef SUPPORT_OPTION_43

/*
 * NETGEAR router spec V1.8,chapter 5.1 Classless Route Option support in dhcpc
 * The client receives one or more DHCP OFFER messages from one or more servers.
 * The client may choose to wait for multiple responses,like to select the information
 * including old IP address,DNS primary & secondary,gateway address,The waiting time
 * is 1 second,For the first time to get an address after the router reboots,there is no
 * preference.
 */
#undef DHCPC_CHOOSE_OLDIP

/*
 * In RFC2131, Page.9-10,
 * ...
 * The 'options' field is now variable length. A DHCP client must be
 * prepared to receive DHCP messages with an 'options' field of at least
 * length 312 octets.  This requirement implies that a DHCP client must
 * be prepared to receive a message of up to 576 octets, the minimum IP
 * datagram size an IP host must be prepared to accept [3].  ...
 * ...
 *
 * Some DHCP server will send a packet larger than 576 octets(which include IP,
 * UDP header, not include Ethernet header), but the 'struct udp_dhcp_packet'
 * size is 576 in udhcp code. If we simple enlarge the option buffer size, the
 * packet which is larger than 576 octets can be received. It seems fixed this
 * bugs, but cause another defect, that the dhcp packets which router send are
 * larger than 576 octets also, which makes many DHCP server can't handle the
 * larger DHCP packet and lead to DHCP process fail if using DHCP type to get
 * WAN IP.
 *
 * Consider the packet size of almost all of DHCP packet Router send is less
 * than 576 octets, so we can dynamic resize the DHCP packet according to it's
 * real payload size and make sure their size are not larger than 576 octets
 * before send out.
 */
#undef DHCP_PACKET_RESIZE

/*
 * Define RFC3442_249_SUPPORT to 1 Support option 249 (RFC 3442) - Microsoft Classless Static Route in dhcpc
 */
#undef RFC3442_249_SUPPORT
@BOTTOM@

#endif
