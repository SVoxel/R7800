/*
 * include/linux/stopwatch.h
 *   Creates /proc/stopwatch/ and associated functions
 *
 * Copyright (c) 2014, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#ifndef _ASM_UBICOM32_STOPWATCH_H
#define _ASM_UBICOM32_STOPWATCH_H

#include <linux/seq_file.h>
#include <linux/smp.h>
#include <linux/time.h>

#define STOPWATCH_MICRO        	0x1
#define STOPWATCH_NANO 		0x2

/*
 * Create a false set of defines and functions enabling users
 * of the stopwatch to leave the code in place.
 */
#ifndef __STOPWATCH_USE__

struct stopwatch_instance;
#define DEFINE_STOPWATCH(x)
#define DEFINE_STOPWATCH_ARRAY(x,y)
#define INIT_STOPWATCH(x)
#define INIT_STOPWATCH_ARRAY(x,y)
#define STOPWATCH_START(x)
#define STOPWATCH_STOP(x)
#define STOPWATCH_INSTANCE_THIS_CPU(x)
#define STOPWATCH_INSTANCE_CPU(x,y)
#define stopwatch_register(name, count, show) (0)
#define stopwatch_unregister(name) (0)
#define stopwatch_show(instance, p, prec) (0)

#else /* __STOPWATCH_USE__ */

#define DEFINE_STOPWATCH(x) \
		DEFINE_PER_CPU(struct stopwatch_instance, __stop_watch_##x);

#define DEFINE_STOPWATCH_ARRAY(x, y) \
		DEFINE_PER_CPU(struct stopwatch_instance[y], __stop_watch_##x);

#define INIT_STOPWATCH(x) \
	do { \
		int cpu; \
		struct stopwatch_instance *si; \
		for_each_cpu(cpu, cpu_online_mask) { \
			si = &per_cpu(__stop_watch_##x, cpu); \
			memset(si, 0, sizeof(struct stopwatch_instance)); \
			spin_lock_init(&si->lock); \
		} \
	} while(0);

#define INIT_STOPWATCH_ARRAY(x, y) \
	do { \
		int cpu, i; \
		struct stopwatch_instance *si; \
		for_each_cpu(cpu, cpu_online_mask) { \
			for (i = 0; i < y; i++) { \
				si = &per_cpu(__stop_watch_##x[i], cpu); \
				memset(si, 0, sizeof(struct stopwatch_instance)); \
				spin_lock_init(&si->lock); \
			} \
		} \
	} while(0);

#define STOPWATCH_START(x) \
	do { \
		struct stopwatch_instance *si; \
		si = &__get_cpu_var(__stop_watch_##x); \
		getnstimeofday((struct timespec *)&si->last); \
	} while(0);

#define STOPWATCH_STOP(x) \
	do { \
		unsigned long flags; \
		struct timespec now; \
		unsigned long long delta; \
		struct stopwatch_instance *si; \
		\
		si = &__get_cpu_var(__stop_watch_##x); \
		getnstimeofday(&now); \
		delta = timespec_to_ns(&now) - \
				timespec_to_ns((struct timespec *)&si->last); \
		\
		spin_lock_irqsave(&si->lock, flags); \
		if (unlikely(si->count == 0)) { \
			si->min = delta; \
			si->max = delta; \
		} \
		else { \
			if (unlikely(si->min > delta))  \
				si->min = delta; \
			else if (unlikely(si->max < delta)) \
				si->max = delta; \
		} \
		\
		si->avg += delta; \
		si->count++; \
		spin_unlock_irqrestore(&si->lock, flags); \
	} while(0);

#define STOPWATCH_INSTANCE_THIS_CPU(x) (__get_cpu_var(__stop_watch_##x))
#define STOPWATCH_INSTANCE_CPU(x,y) (per_cpu(__stop_watch_##x, y))

struct stopwatch_instance {
	unsigned long long min;
	unsigned long long avg;
	unsigned long long max;
	unsigned long count;
	struct timespec last;
	spinlock_t lock;
};

typedef int (*stopwatch_show_t)(struct seq_file *f, void *v);

unsigned int stopwatch_register(const char *name, int count,
					stopwatch_show_t show);
void stopwatch_unregister(const char *name);
void stopwatch_show(struct stopwatch_instance *si, struct seq_file *p,
			int precision);

#endif /* __STOPWATCH_USE__ */
#endif /* _ASM_UBICOM32_STOPWATCH_H */
