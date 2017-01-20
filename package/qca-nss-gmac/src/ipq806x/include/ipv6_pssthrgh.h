#ifndef IPV6_PSSTHRGH_H
#define IPV6_PSSTHRGH_H

#define CONFIG_PSSTHRGH 1

struct dni_enet_info {
        struct net_device *real_dev;
        struct net_device_stats dev_stats;
};

#define DNI_ENET_INFO(x) ((struct dni_enet_info *)(x->ml_priv))

extern int ipv6_pssthrgh_enable;

#endif
