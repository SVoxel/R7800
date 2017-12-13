#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "common.h"


/**
 * Record first NTP Sync Timestamp.
 *
 * It's a track record of the date/time on POT partition fot the first time
 * when the router power on and get time from NTP server.
 *
 * The first 4 bytes in the POT MTD with FIRST_NTP_TIME_OFFSET offset stores the
 * timestamp in the number of seconds since year 1970 in the GMT time.
 **/

void ntpst_usage(char *name)
{
	fprintf(stderr,
		"\nUsage: %s <get | set>\n"
		"\tget - display the contents of the NTP Sync Timestamp to 'stdout'.\n"
		"\tset - record now time to POT partition as a NTP Sync Timestamp,\n"
		"\t\tif thers is a NTP Sync Timestamp existed yet, just display\n"
		"\t\tit without any write action.\n"
		"\tclean - clean the result in POT partition.\n",
		name);
	exit(0);
}

int ntpst_func(int argc, char *argv[])
{
	int devfd, count, sign = 0; /* sign: 0 - get, 1 - set, 2 - clean */
	unsigned char buf[NAND_FLASH_PAGESIZE];
	time_t *ntptime = (time_t *)buf;
	char str[128];
	char timezone[64];
	char tz_env[64];

	if (2 == argc && !strcmp(argv[1], "get"))
		sign = 0;
	else if (2 == argc && !strcmp(argv[1], "set"))
		sign = 1;
	else if (2 == argc && !strcmp(argv[1], "clean"))
                sign = 2;
        else
		ntpst_usage(argv[0]);

	devfd = open(POT_MTD, O_RDWR | O_SYNC);
	if (devfd < 0) {
		dfp("can't open mtd device - %s\n", POT_MTD);
		return -1;
	}

	lseek(devfd, FIRST_NTP_TIME_OFFSET, SEEK_SET);
	read(devfd, buf, sizeof(buf));

	if (0 == sign) {
		if (0xFFFFFFFF == *ntptime) {
			fprintf(stderr, ":( ntpst - (offset 0x%08X bytes in POT partition) first NTP synchronized date/time: 00-00-00\n",
				FIRST_NTP_TIME_OFFSET);
		} else {
			memset(timezone, 0, sizeof(timezone));
			strcpy(timezone, config_get("time_zone"));
			sprintf(tz_env, "TZ=%s", timezone);
			putenv(tz_env);
			strftime(str, sizeof(str), "%T, %b %d, %Y",localtime(ntptime));

			fprintf(stderr, ":) ntpst - (offset 0x%08X bytes in POT partition) first NTP synchronized date/time: %s\n",
				FIRST_NTP_TIME_OFFSET, str);
		}
	} else {
                if (1 == sign) {
        		if (0xFFFFFFFF != *ntptime) {
        			strftime(str, sizeof(str), "%T, %b %d, %Y", localtime(ntptime));
        			fprintf(stderr, ":( ntpst - (offset 0x%08X bytes in POT partition) one NTP Sync Timestamp existed: %s\n",
        				FIRST_NTP_TIME_OFFSET, str);
        		} else {
        			*(time_t *)buf = time(NULL);
        			lseek(devfd, FIRST_NTP_TIME_OFFSET, SEEK_SET);
        			write(devfd, buf, sizeof(buf));
        			strftime(str, sizeof(str), "%T, %b %d, %Y", localtime(ntptime));
        			fprintf(stderr, ":) ntpst - (offset 0x%08X bytes in POT partition) NTP Sync Timestamp record success: %s\n",\
        				FIRST_NTP_TIME_OFFSET, str);
        		}
                } else {
	                unsigned char buf_tmp[NAND_FLASH_PAGESIZE];
	                unsigned char buf_tmp_1[NAND_FLASH_PAGESIZE];
                	struct mac_t {
                		unsigned char byte0;
                		unsigned char byte1;
                		unsigned char byte2;
                		unsigned char byte3;
                		unsigned char byte4;
                		unsigned char byte5;
                	} tmp, *mac;
                	memset(&tmp, 0xFFFFFFFF, sizeof(tmp));
                	mac = (struct mac_t *)buf_tmp;
                	lseek(devfd, FIRST_WIFISTATION_MAC_OFFSET, SEEK_SET);
                	read(devfd, buf_tmp, sizeof(buf_tmp_1));
                	struct net_t {
                		unsigned char byte0;
                		unsigned char byte1;
                	} nettmp, *netconn;
                	memset(&nettmp, 0xFFFF, sizeof(nettmp));
                	netconn = (struct net_t *)buf_tmp_1;
                	lseek(devfd, FIRST_INTERNET_CONNECT_OFFSET, SEEK_SET);
                	read(devfd, buf_tmp_1, sizeof(buf_tmp_1));
                        system("flash_erase /dev/mtd12 0x0040000 1");
        		if (memcmp(&tmp, mac, sizeof(tmp))) {
                       	        lseek(devfd, FIRST_WIFISTATION_MAC_OFFSET, SEEK_SET);
                               	write(devfd, buf_tmp, sizeof(buf_tmp));
                        }
        		if (memcmp(&nettmp, netconn, sizeof(nettmp))) {
                               	lseek(devfd, FIRST_INTERNET_CONNECT_OFFSET, SEEK_SET);
                               	write(devfd, buf_tmp_1, sizeof(buf_tmp_1));
                        }
			fprintf(stderr, ":( ntpst - (offset 0x%08X bytes in POT partition) first NTP synchronized date/time: 00-00-00\n",
				FIRST_NTP_TIME_OFFSET);
                }
	}

	close(devfd);
	return 0;
}


/**
 * Record MAC address of the first Wi-Fi STA that connects to the router after it came out from the factory.
 *
 * the first 6 bytes in the POT partition with FIRST_WIFISTATION_MAC_OFFSET offset stores MAC address of the
 * first connected wifi station in the process of user using DUT.
 *
 * Basically it works this way:
 * Every time when there is a Wi-Fi STA connected to a router since it boots up, 
 * the router checks whether there is a MAC address stored on the 6 bytes (i.e. whether the first byte of the 6 bytes is 0xff).
 * If the 6 bytes do not record a MAC address information, the router stores the MAC address of this Wi-Fi STA to the 6 bytes.
 * Or else no action is performed.
 */

void stamac_usage(char *name)
{
	printf("\nUsage: %s <get | set <mac address>>\n"
		"\tget - display the MAC address of the first Wi-Fi STA that connects to\n"
		"\t\tthe router after it came out from the factory to 'stdout'.\n"
		"\tset xx:xx:xx:xx:xx:xx - record specified MAC address to POT partition,\n"
		"\t\tif thers is a MAC address existed yet, just display\n"
		"\t\tit without any write action.\n"
		"\tclean - clean the result in POT partition.\n",
		name);
	exit(0);
}

int stamac_func(int argc, char *argv[])
{
	int devfd, count, sign = 0;	/* sign: 0 - get, 1 - set, 2 - clean */
	unsigned char buf[NAND_FLASH_PAGESIZE];
	struct mac_t {
		unsigned char byte0;
		unsigned char byte1;
		unsigned char byte2;
		unsigned char byte3;
		unsigned char byte4;
		unsigned char byte5;
	} tmp, *mac;
	unsigned int gmac[6];

	if (argc == 2 && !strcmp(argv[1], "get"))
		sign = 0;
	else if (argc == 3 && !strcmp(argv[1], "set"))
		sign = 1;
	else if (argc == 2 && !strcmp(argv[1], "clean"))
		sign = 2;
	else
		stamac_usage(argv[0]);

	memset(&tmp, 0xFFFFFFFF, sizeof(tmp));
	mac = (struct mac_t *)buf;

	devfd = open(POT_MTD, O_RDWR | O_SYNC);
	if (0 > devfd) {
		printf("stamac: open mtd POT error!\n");
		return -1;
	}

#define MAC_FMT	"%02x:%02x:%02x:%02x:%02x:%02x"
#define MAC_VAL	mac->byte0, mac->byte1, mac->byte2, mac->byte3, mac->byte4, mac->byte5
	lseek(devfd, FIRST_WIFISTATION_MAC_OFFSET, SEEK_SET);
	read(devfd, buf, sizeof(buf));
	if (0 == sign) {
		if (!memcmp(&tmp, mac, sizeof(tmp)))
			fprintf(stderr, ":( stamac - (offset 0x%08X bytes in POT partition) 1st connected STA's MAC: 00-00-00-00-00-00\n",
				FIRST_WIFISTATION_MAC_OFFSET);
		else
			fprintf(stderr, ":) stamac - (offset 0x%08X bytes in POT partition) 1st connected STA's MAC: "MAC_FMT"\n",
				FIRST_WIFISTATION_MAC_OFFSET, MAC_VAL);
	} else {
                if (1 == sign) {
        		if (memcmp(&tmp, mac, sizeof(tmp))) {
        			fprintf(stderr, ":( stamac - (offset 0x%08X bytes in POT partition) one 1st connected STA's MAC exsited: "MAC_FMT"\n",
        				FIRST_WIFISTATION_MAC_OFFSET, MAC_VAL);
        		} else {
        			if (6 == sscanf(argv[2], MAC_FMT, &gmac[0], &gmac[1], &gmac[2], &gmac[3], &gmac[4], &gmac[5])) {
        				for (count = 0; count < 6; count++)	/* valid MAC address ? */
        				{
        					buf[count] = (unsigned char)gmac[count];
        					if (buf[count] > 0xFF || buf[count] < 0x00)
        						break;
        				}

        				if (count == 6) {
        					lseek(devfd, FIRST_WIFISTATION_MAC_OFFSET, SEEK_SET);
        					write(devfd, buf, sizeof(buf));
        					fprintf(stderr, ":) stamac - (offset 0x%08X bytes in POT partition) 1st connected STA's MAC record success: "MAC_FMT"\n",
        						FIRST_WIFISTATION_MAC_OFFSET, MAC_VAL);
        				} else {
        					fprintf(stderr, ":( stamac - (offset 0x%08X bytes in POT partition) sorry, invalied MAC address can't be stored: %s\n",
						FIRST_WIFISTATION_MAC_OFFSET, argv[2]);
        				}
        			} else {
        				fprintf(stderr, ":( stamac - (offset 0x%08X bytes in POT partition) sorry, invalied MAC address can't be stored: %s\n",
	        				FIRST_WIFISTATION_MAC_OFFSET, argv[2]);
        			}
        		}
                } else {
	                unsigned char buf_tmp[NAND_FLASH_PAGESIZE];
	                unsigned char buf_tmp_1[NAND_FLASH_PAGESIZE];
	                time_t *ntptime = (time_t *)buf_tmp;
	                lseek(devfd, FIRST_NTP_TIME_OFFSET, SEEK_SET);
                	read(devfd, buf_tmp, sizeof(buf_tmp));
                	struct net_t {
                		unsigned char byte0;
                		unsigned char byte1;
                	} nettmp, *netconn;
                	memset(&nettmp, 0xFFFF, sizeof(nettmp));
                	netconn = (struct net_t *)buf_tmp_1;
                	lseek(devfd, FIRST_INTERNET_CONNECT_OFFSET, SEEK_SET);
                	read(devfd, buf_tmp_1, sizeof(buf_tmp_1));
                        system("flash_erase /dev/mtd12 0x0040000 1");
                        if (0xFFFFFFFF != *ntptime) {
                               	lseek(devfd, FIRST_NTP_TIME_OFFSET, SEEK_SET);
                               	write(devfd, buf_tmp, sizeof(buf_tmp));
                        }
        		if (memcmp(&nettmp, netconn, sizeof(nettmp))) {
                               	lseek(devfd, FIRST_INTERNET_CONNECT_OFFSET, SEEK_SET);
                               	write(devfd, buf_tmp_1, sizeof(buf_tmp_1));
                        }
			fprintf(stderr, ":( stamac - (offset 0x%08X bytes in POT partition) 1st connected STA's MAC: 00-00-00-00-00-00\n",
				FIRST_WIFISTATION_MAC_OFFSET);
                }
        }
#undef MAC_FMT
#undef MAC_VAL

	close(devfd);
	return 0;
}
void netconn_usage(char *name)
{
	printf("\nUsage: %s <get | set connect to internet or not>\n"
		"\tget - display it connect to internet or not\n"
		"\t\t after it came out from the factory to 'stdout'.\n"
		"\tset xxxx - record the result to POT partition,\n"
		"\t\tif thers is a result existed yet, just display\n"
		"\t\tit without any write action.\n"
		"\tclean - clean the result in POT partition.\n",
		name);
	exit(0);
}

int netconn_func(int argc, char *argv[])
{
	int devfd, count, sign = 0;	/* sign: 0 - get, 1 - set, 2 - clean */
	unsigned char buf[NAND_FLASH_PAGESIZE];
	struct net_t {
		unsigned char byte0;
		unsigned char byte1;
	} nettmp, *netconn;

	if (argc == 2 && !strcmp(argv[1], "get"))
		sign = 0;
	else if (argc == 2 && !strcmp(argv[1], "set"))
		sign = 1;
	else if (argc == 2 && !strcmp(argv[1], "clean"))
		sign = 2;
	else
		netconn_usage(argv[0]);

	memset(&nettmp, 0xFFFF, sizeof(nettmp));
	netconn = (struct net_t *)buf;
	devfd = open(POT_MTD, O_RDWR | O_SYNC);
	if (0 > devfd) {
		printf("netconn: open mtd POT error!\n");
		return -1;
	}

	lseek(devfd, FIRST_INTERNET_CONNECT_OFFSET, SEEK_SET);
	read(devfd, buf, sizeof(buf));
	if (0 == sign) {
		if (!memcmp(&nettmp, netconn, sizeof(nettmp))) {
			fprintf(stderr, ":( netconn - (offset 0x%08X bytes in POT partition) Ever connected to Internet (Y/N): No\n",
				FIRST_INTERNET_CONNECT_OFFSET);
		} else {
			fprintf(stderr, ":) netconn - (offset 0x%08X bytes in POT partition) Ever connected to Internet (Y/N): Yes\n",
				FIRST_INTERNET_CONNECT_OFFSET);
		}
	} else {
                if (1 == sign) {
        		if (memcmp(&nettmp, netconn, sizeof(nettmp))) {
        			fprintf(stderr, ":( netconn - (offset 0x%08X bytes in POT partition) one netconn result existed: Yes\n",
        				FIRST_INTERNET_CONNECT_OFFSET);
        		} else {
                                        for (count = 0; count < 2; count++)
                                        {
                                                buf[count] = 0x00;
                                        }
                                        if (count == 2) {
                        			lseek(devfd, FIRST_INTERNET_CONNECT_OFFSET, SEEK_SET);
                        			write(devfd, buf, sizeof(buf));
                        			fprintf(stderr, ":) netconn - (offset 0x%08X bytes in POT partition) set netconn result success: Yes\n",\
                        				FIRST_INTERNET_CONNECT_OFFSET);
                                        }
        		}
                } else {
	                unsigned char buf_tmp[NAND_FLASH_PAGESIZE];
	                unsigned char buf_tmp_1[NAND_FLASH_PAGESIZE];
	                time_t *ntptime = (time_t *)buf_tmp;
	                lseek(devfd, FIRST_NTP_TIME_OFFSET, SEEK_SET);
                	read(devfd, buf_tmp, sizeof(buf_tmp));
                	struct mac_t {
                		unsigned char byte0;
                		unsigned char byte1;
                		unsigned char byte2;
                		unsigned char byte3;
                		unsigned char byte4;
                		unsigned char byte5;
                	} tmp, *mac;
                	memset(&tmp, 0xFFFFFFFF, sizeof(tmp));
                	mac = (struct mac_t *)buf_tmp_1;
                	lseek(devfd, FIRST_WIFISTATION_MAC_OFFSET, SEEK_SET);
                	read(devfd, buf_tmp_1, sizeof(buf_tmp_1));
                        system("flash_erase /dev/mtd12 0x0040000 1");
                        if (0xFFFFFFFF != *ntptime) {
                               	lseek(devfd, FIRST_NTP_TIME_OFFSET, SEEK_SET);
                               	write(devfd, buf_tmp, sizeof(buf_tmp));
                        }
        		if (memcmp(&tmp, mac, sizeof(tmp))) {
                       	        lseek(devfd, FIRST_WIFISTATION_MAC_OFFSET, SEEK_SET);
                               	write(devfd, buf_tmp_1, sizeof(buf_tmp_1));
                        }
       			fprintf(stderr, ":( netconn - (offset 0x%08X bytes in POT partition) set netconn result: No\n",
       				FIRST_INTERNET_CONNECT_OFFSET);
                }
	}

	close(devfd);
	return 0;
}
