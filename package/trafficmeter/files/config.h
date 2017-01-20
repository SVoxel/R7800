#ifndef __CONFIG_H__
#define __CONFIG_H__

/* which mtd used record traffic meter data */
#define TRAFFIC_METER_MTD "/dev/mtd16"
#define TRAFFIC_METER_MTD_DONGLE "/dev/mtd18"

/* WAN interface name of router */
#define WAN_DHCP_IFNAME "brwan"
/* PPP interface name of router */
#define WAN_PPP_IFNAME "ppp0"
#define WAN_ETH_STATUS "/tmp/port_status"
#define WAN_PPP_STATUS "/tmp/ppp/ppp0-status"

/* output files, like as status, statistics, date and volume left, warning messages,
 *    and config file of traffic meter. */
#define TMP_TRAFFIC_WARNINGMSG "/tmp/traffic_meter/traffic_meter_warning_message"
#define TMP_TRAFFIC_CONFIG "/tmp/traffic_meter/traffic_meter.conf"
#define TMP_TRAFFIC_LEFT "/tmp/traffic_meter/date_and_volume_left"
#define TMP_TRAFFIC_STATISTICS "/tmp/traffic_meter/traffic_statistics"
#define TMP_TRAFFIC_STATUS "/tmp/traffic_meter/traffic_status"

/* support new feature of NTP described in traffic meter spec 1.9 */
#define NTP_FEATURE
/*For accurate control*/
#define ACCURATE_CONTROL
/* When users change the timezone, the traffic statistics numbers should be adjusted accordingly */
#define UPDATE_STATISTICS
/* The traffic meter function should be disabled automatically when change connection time control */
#define AUTO_DISABLE

/* enable changes for nand flash access */
#define NAND_FLASH_SUPPORT 1
#if NAND_FLASH_SUPPORT
#define NAND_FLASH_BLOCKSIZE 0x1f000
#define NAND_FLASH_PAGESIZE  2048
#define MAX_RECORD_6HOURS 1
#endif

#endif
