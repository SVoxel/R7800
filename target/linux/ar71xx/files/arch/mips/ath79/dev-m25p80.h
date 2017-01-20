/*
 *  Copyright (C) 2009-2012 Gabor Juhos <juhosg@openwrt.org>
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 */

#ifndef _ATH79_DEV_M25P80_H
#define _ATH79_DEV_M25P80_H

#include <linux/spi/flash.h>

extern struct ath79_spi_platform_data ath79_spi_data;

void ath79_register_m25p80(struct flash_platform_data *pdata) __init;
void ath79_register_m25p80_multi(struct flash_platform_data *pdata) __init;
void ath79_init_m25p80_pdata(struct flash_platform_data *pdata);

#endif /* _ATH79_DEV_M25P80_H */
