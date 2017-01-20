/*
 * Copyright (c) 2012, The Linux Foundation. All rights reserved.
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
#include <linux/kernel.h>
#include <linux/workqueue.h>
#include <linux/percpu.h>
#include <linux/cpu.h>
#include <linux/smp.h>
#include <linux/sysfs.h>
#include <linux/sysdev.h>

#include <mach/scm.h>

#define CPU_CONFIG_CMD			(5)
#define CPU_CONFIG_QUERY_CMD 		(6)
#define TZBSP_BUILD_VER_QUERY_CMD	(4)
#define TZBSP_MAJOR_VERSION(x)		(((x) >> 16) & 0xFFFF)
#define TZBSP_MINOR_VERSION(x)		((x) & 0xFFFF)
#define TZBSP_BUILD_ID_LEN		(32)

static int query_cpu_config(void)
{
	struct cpu_config_query_req_resp {
		u32	id;
		u32	arg0;
		u32	arg1;
		u32	arg2;
	} request;
	struct cpu_config_query_resp {
		u32	ret0;
		u32	ret1;
		u32	ret2;
		u32	ret3;
	} response = {0};
	int ret;

	request.id = 1;
	ret = scm_call(SCM_SVC_BOOT, CPU_CONFIG_QUERY_CMD, &request,
			sizeof(request), &response, sizeof(response));
	return ret ? : response.ret0;
}

static void set_cpu_config(int enable)
{
	struct cpu_config_req {
		u32	id;
		u32	arg0;
		u32	arg1;
		u32	arg2;
	} request;

	request.id = 1;
	request.arg0 = enable;
	scm_call(SCM_SVC_BOOT, CPU_CONFIG_CMD, &request, sizeof(request),
			NULL, 0);
}

void enable_cpu_config(struct work_struct *work)
{
	set_cpu_config(1);
}

void disable_cpu_config(struct work_struct *work)
{
	set_cpu_config(0);
}

int cpu_config_on_each_cpu(bool enable)
{
	work_func_t func = enable ? enable_cpu_config : disable_cpu_config;
	return schedule_on_each_cpu(func);
}

static ssize_t show_cpuctl(struct sysdev_class *class,
		struct sysdev_class_attribute *attr, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%d\n", query_cpu_config());
}

static ssize_t store_cpuctl(struct sysdev_class *class,
		struct sysdev_class_attribute *attr, const char *buf,
		size_t count)
{
	unsigned val;
	int ret;

	ret = kstrtouint(buf, 10, &val);
	if (ret < 0)
		return ret;
	ret = cpu_config_on_each_cpu(val);
	if (ret < 0)
		return ret;

	return count;
}

static SYSDEV_CLASS_ATTR(cpuctl, 0600, show_cpuctl, store_cpuctl);

static ssize_t show_tzbspver(struct sysdev_class *class,
		struct sysdev_class_attribute *attr, char *buf)
{
	u32 ver = scm_get_version();
	return snprintf(buf, PAGE_SIZE, "%u.%u\n", TZBSP_MAJOR_VERSION(ver),
			TZBSP_MINOR_VERSION(ver));
}

static ssize_t show_tzbsp_build_rev(struct sysdev_class *class,
		struct sysdev_class_attribute *attr, char *buf)
{
	int ret;

	ret = scm_call(SCM_SVC_INFO, TZBSP_BUILD_VER_QUERY_CMD, NULL,
			0, buf, TZBSP_BUILD_ID_LEN);

	if (ret)
		pr_err("%s: could not query build revision ret = %d.\n",
			__func__, ret);

	return strnlen(buf, TZBSP_BUILD_ID_LEN);

}
static SYSDEV_CLASS_ATTR(tzbsp_build_rev, 0600, show_tzbsp_build_rev, NULL);

static SYSDEV_CLASS_ATTR(tzbsp_version, 0600, show_tzbspver, NULL);

static struct attribute *cpu_subsys_attributes[] = {
	&attr_cpuctl.attr,
	&attr_tzbsp_version.attr,
	&attr_tzbsp_build_rev.attr,
	NULL
};

static struct attribute_group cpu_subsys_attr_group = {
	.attrs = cpu_subsys_attributes,
};

static int __init init_scm_cpu(void)
{
	return sysfs_create_group(&cpu_subsys.dev_root->kobj,
			&cpu_subsys_attr_group);
}
module_init(init_scm_cpu);
