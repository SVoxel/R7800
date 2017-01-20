/* Copyright (c) 2010-2011, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

/*
 * SDIO-Abstraction-Layer API.
 */

#ifndef __SDIO_AL__
#define __SDIO_AL__

#include <linux/mmc/card.h>

struct sdio_channel; /* Forward Declaration */


/**
 *  Channel Events.
 *  Available bytes notification.
 */
#define SDIO_EVENT_DATA_READ_AVAIL      0x01
#define SDIO_EVENT_DATA_WRITE_AVAIL     0x02


static int __maybe_unused sdio_open(const char *name, struct sdio_channel **ch,
		void *priv, void (*notify)(void *priv, unsigned channel_event))
{
	return -ENODEV;
}

static int __maybe_unused sdio_close(struct sdio_channel *ch)
{
	return -ENODEV;
}

static int __maybe_unused sdio_read(struct sdio_channel *ch, void *data,
						int len)
{
	return -ENODEV;
}

static int __maybe_unused sdio_write(struct sdio_channel *ch, const void *data,
						int len)
{
	return -ENODEV;
}

static int __maybe_unused sdio_write_avail(struct sdio_channel *ch)
{
	return -ENODEV;
}

static int __maybe_unused sdio_read_avail(struct sdio_channel *ch)
{
	return -ENODEV;
}

#endif /* __SDIO_AL__ */
