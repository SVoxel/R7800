/* Qualcomm Secure Execution Environment Communicator (QSEECOM) driver
 *
 * Copyright (c) 2012, 2015, The Linux Foundation. All rights reserved.
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

/* Usage:
 *
 *(1) Step 1: To provide the sampleapp files to the kernel module
 *
 * cat /lib/firmware/sampleapp.mdt > /sys/firmware/mdt_file
 * cat /lib/firmware/sampleapp.b00 > /sys/firmware/seg0_file
 * cat /lib/firmware/sampleapp.b01 > /sys/firmware/seg1_file
 * cat /lib/firmware/sampleapp.b02 > /sys/firmware/seg2_file
 * cat /lib/firmware/sampleapp.b03 > /sys/firmware/seg3_file
 *
 *(2) Step 2: To start loading the sampleapp
 *
 * echo 1 > /sys/devices/system/tzapp/tzapp0/load_start
 *
 *(3) Step 3:
 *
 * To give input to Encryption:
 * echo '6bc1bee22e409f96' > /sys/devices/system/tzapp/tzapp0/encrypt
 *
 * To view encryption output:
 * cat /sys/devices/system/tzapp/tzapp0/encrypt
 *
 * To give input to Decryption:
 * echo `cat /sys/devices/system/tzapp/tzapp0/encrypt` >
 *  /sys/devices/system/tzapp/tzapp0/decrypt
 *
 * To view decryption output:
 * cat /sys/devices/system/tzapp/tzapp0/decrypt
 *
 * To give input to Multiplication op:
 * echo 100 > /sys/devices/system/tzapp/tzapp0/basic_data
 *
 * To view Secure Multiplication output:
 * cat /sys/devices/system/tzapp/tzapp0/basic_data
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/fcntl.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/highuid.h>
#include <linux/sysfs.h>
#include <linux/fs.h>
#include <linux/sysdev.h>
#include <linux/kobject.h>
#include "mach/scm.h"
#include <linux/gfp.h>
#include <linux/sysfs.h>

#define CLIENT_CMD1_BASIC_DATA	1
#define CLIENT_CMD8_RUN_CRYPTO_ENCRYPT	8
#define CLIENT_CMD9_RUN_CRYPTO_DECRYPT	9
#define MAX_APP_NAME_SIZE	32
#define MAX_INPUT_SIZE	4096

struct qsc_send_cmd {
	uint32_t cmd_id;
	uint32_t data;
	uint32_t data2;
	uint32_t len;
	uint32_t start_pkt;
	uint32_t end_pkt;
	uint32_t test_buf_size;
};

struct qsc_send_cmd_rsp {
	uint32_t data;
	int32_t status;
};

__packed struct qseecom_unload_app_ireq {
	uint32_t qsee_cmd_id;
	uint32_t  app_id;
};

enum qseecom_command_scm_resp_type {
	QSEOS_APP_ID = 0xEE01,
	QSEOS_LISTENER_ID
};

__packed struct qseecom_command_scm_resp {
	uint32_t result;
	enum qseecom_command_scm_resp_type resp_type;
	unsigned int data;
};

__packed struct qseecom_client_send_data_ireq {
	uint32_t qsee_cmd_id;
	uint32_t app_id;
	void *req_ptr;
	uint32_t req_len;
	void *rsp_ptr;	 /* First 4 bytes should always be the return status */
	uint32_t rsp_len;
};

__packed struct qseecom_load_app_ireq {
	uint32_t qsee_cmd_id;
	uint32_t mdt_len;		/* Length of the mdt file */
	uint32_t img_len;		/* Length of .bxx and .mdt files */
	uint32_t phy_addr;		/* phy addr of the start of image */
	char	 app_name[MAX_APP_NAME_SIZE];	/* application name*/
};

enum qseecom_qceos_cmd_status {
	QSEOS_RESULT_SUCCESS = 0,
	QSEOS_RESULT_INCOMPLETE,
	QSEOS_RESULT_FAILURE  = 0xFFFFFFFF
};

enum qseecom_qceos_cmd_id {
	QSEOS_APP_START_COMMAND      = 0x01,
	QSEOS_APP_SHUTDOWN_COMMAND,
	QSEOS_APP_LOOKUP_COMMAND,
	QSEOS_REGISTER_LISTENER,
	QSEOS_DEREGISTER_LISTENER,
	QSEOS_CLIENT_SEND_DATA_COMMAND,
	QSEOS_LISTENER_DATA_RSP_COMMAND,
	QSEOS_LOAD_EXTERNAL_ELF_COMMAND,
	QSEOS_UNLOAD_EXTERNAL_ELF_COMMAND,
	QSEOS_CMD_MAX	  = 0xEFFFFFFF
};

static uint32_t qsee_app_id;
static void *qsee_sbuffer;
static int32_t basic_output;
static int enc_len;
static int dec_len;
static int basic_data_len;
static int mdt_size;
static int seg0_size;
static int seg1_size;
static int seg2_size;
static int seg3_size;
static uint8_t *mdt_file;
static uint8_t *seg0_file;
static uint8_t *seg1_file;
static uint8_t *seg2_file;
static uint8_t *seg3_file;

/*
 * Array Length is 4096 bytes, since 4MB is the max input size
 * that can be passed to SCM call
 */
static uint8_t encrypt_text[MAX_INPUT_SIZE];
static uint8_t decrypt_text[MAX_INPUT_SIZE];

static ssize_t mdt_write(struct file *filp, struct kobject *kobj,
	struct bin_attribute *bin_attr,
	char *buf, loff_t pos, size_t count)
{
	/*
	 * Position '0' means new file being written,
	 * Hence allocate new memory after freeing already allocated mem if any
	 */
	if (pos == 0) {
		kfree(mdt_file);
		mdt_file = kzalloc((count) * sizeof(uint8_t), GFP_KERNEL);
	} else {
		mdt_file = krealloc(mdt_file,
			(pos + count) * sizeof(uint8_t), GFP_KERNEL);
	}

	if (!mdt_file)
		return -ENOMEM;

	memcpy((mdt_file + pos), buf, count);
	mdt_size = pos + count;
	return count;
}

static ssize_t seg0_write(struct file *filp, struct kobject *kobj,
	struct bin_attribute *bin_attr,
	char *buf, loff_t pos, size_t count)
{
	if (pos == 0) {
		kfree(seg0_file);
		seg0_file = kzalloc((count) * sizeof(uint8_t), GFP_KERNEL);
	} else {
		seg0_file = krealloc(seg0_file, (pos + count) * sizeof(uint8_t),
					GFP_KERNEL);
	}

	if (!seg0_file)
		return -ENOMEM;

	memcpy((seg0_file + pos), buf, count);
	seg0_size = pos + count;
	return count;
}

static ssize_t seg1_write(struct file *filp, struct kobject *kobj,
	struct bin_attribute *bin_attr,
	char *buf, loff_t pos, size_t count)
{
	if (pos == 0) {
		kfree(seg1_file);
		seg1_file = kzalloc((count) * sizeof(uint8_t), GFP_KERNEL);
	} else {
		seg1_file = krealloc(seg1_file, (pos + count) * sizeof(uint8_t),
					GFP_KERNEL);
	}

	if (!seg1_file)
		return -ENOMEM;

	memcpy((seg1_file + pos), buf, count);
	seg1_size = pos + count;
	return count;
}

static ssize_t seg2_write(struct file *filp, struct kobject *kobj,
	struct bin_attribute *bin_attr,
	char *buf, loff_t pos, size_t count)
{
	if (pos == 0) {
		kfree(seg2_file);
		seg2_file = kzalloc((count) * sizeof(uint8_t), GFP_KERNEL);
	} else {
		seg2_file = krealloc(seg2_file, (pos + count) * sizeof(uint8_t),
					GFP_KERNEL);
	}

	if (!seg2_file)
		return -ENOMEM;

	memcpy((seg2_file + pos), buf, count);
	seg2_size = pos + count;
	return count;
}

static ssize_t seg3_write(struct file *filp, struct kobject *kobj,
	struct bin_attribute *bin_attr,
	char *buf, loff_t pos, size_t count)
{
	if (pos == 0) {
		kfree(seg3_file);
		seg3_file = kzalloc((count) * sizeof(uint8_t), GFP_KERNEL);
	} else {
		seg3_file = krealloc(seg3_file, (pos + count) * sizeof(uint8_t),
					GFP_KERNEL);
	}

	if (!seg3_file)
		return -ENOMEM;

	memcpy((seg3_file + pos), buf, count);
	seg3_size = pos + count;
	return count;
}

struct bin_attribute mdt_attr = {
	.attr = {.name = "mdt_file", .mode = 0444},
	.write = mdt_write,
};

struct bin_attribute seg0_attr = {
	.attr = {.name = "seg0_file", .mode = 0444},
	.write = seg0_write,
};

struct bin_attribute seg1_attr = {
	.attr = {.name = "seg1_file", .mode = 0444},
	.write = seg1_write,
};

struct bin_attribute seg2_attr = {
	.attr = {.name = "seg2_file", .mode = 0444},
	.write = seg2_write,
};

struct bin_attribute seg3_attr = {
	.attr = {.name = "seg3_file", .mode = 0444},
	.write = seg3_write,
};

static int qseecom_unload_app(void)
{
	struct qseecom_unload_app_ireq req;
	struct qseecom_command_scm_resp resp;
	int ret;

	req.qsee_cmd_id = QSEOS_APP_SHUTDOWN_COMMAND;
	req.app_id = qsee_app_id;

	/* SCM_CALL to unload the app */
	ret = scm_call(SCM_SVC_TZSCHEDULER, 1,	&req,
			sizeof(struct qseecom_unload_app_ireq),
			&resp, sizeof(resp));
	if (ret)
		pr_err("scm_call to unload app (id = %d) failed\n", req.app_id);

	pr_info("App id %d now unloaded\n", req.app_id);
	return 0;
}

static int tzapp_test(void *input, void *output, int input_len, int option)
{
	int ret = 0;

	struct qseecom_client_send_data_ireq send_data_req;
	struct qseecom_command_scm_resp resp;
	struct qsc_send_cmd *msgreq;	 /* request data sent to QSEE */
	struct qsc_send_cmd_rsp *msgrsp; /* response data sent from QSEE */
	struct page *pg_tmp;
	unsigned long pg_addr;

	/*
	 * Using alloc_pages to avoid colliding with input pointer's
	 * allocated page, since qsee_register_shared_buffer() in sampleapp
	 * checks if input ptr is in secure area. Page where msgreq/msgrsp
	 * is allocated is added to blacklisted area by sampleapp and added
	 * as secure memory region, hence input data (shared buffer)
	 * cannot be in that secure memory region
	 */
	pg_tmp = alloc_page(GFP_KERNEL);
	if (!pg_tmp)
		return -ENOMEM;

	/*
	 * Getting virtual page address. pg_tmp will be pointing to
	 * first page structure
	 */
	msgreq = page_address(pg_tmp);
	if (!msgreq) {
		pr_err("Unable to allocate memory\n");
		return -ENOMEM;
	}
	/* pg_addr for passing to free_page */
	pg_addr = (unsigned long) msgreq;

	msgrsp = (struct qsc_send_cmd_rsp *)((uint8_t *) msgreq +
						sizeof(struct qsc_send_cmd));
	if (!msgrsp) {
		kfree(msgreq);
		pr_err("Unable to allocate memory\n");
		return -ENOMEM;
	}

	/*
	 * option = 1 -> Basic Multiplication, option = 2 -> Encryption,
	 * option = 3 -> Decryption
	 */
	switch (option) {
	case 1:
		msgreq->cmd_id = CLIENT_CMD1_BASIC_DATA;
		msgreq->data = *((uint32_t *)input);
		break;
	case 2:
		msgreq->cmd_id = CLIENT_CMD8_RUN_CRYPTO_ENCRYPT;
		break;
	case 3:
		msgreq->cmd_id = CLIENT_CMD9_RUN_CRYPTO_DECRYPT;
		break;
	default:
		pr_err("\n Invalid Option");
		goto fn_exit;
	}
	if (option != 1) {
		msgreq->data = virt_to_phys(input);
		msgreq->data2 = virt_to_phys(output);
		msgreq->test_buf_size = input_len;
		msgreq->len = input_len;
	}

	send_data_req.qsee_cmd_id = QSEOS_CLIENT_SEND_DATA_COMMAND;
	send_data_req.app_id = qsee_app_id;
	send_data_req.req_ptr = (void *)virt_to_phys(msgreq);
	send_data_req.req_len = sizeof(struct qsc_send_cmd);
	send_data_req.rsp_ptr = (void *)virt_to_phys(msgrsp);
	send_data_req.rsp_len = sizeof(struct qsc_send_cmd_rsp);

	ret = scm_call(SCM_SVC_TZSCHEDULER, 1, (const void *) &send_data_req,
					sizeof(send_data_req),
					&resp, sizeof(resp));
	if (ret) {
		pr_err("qseecom_scm_call failed with err: %d\n", ret);
		goto fn_exit;
	}

	if (resp.result == QSEOS_RESULT_INCOMPLETE) {
		pr_err("Result incomplete\n");
		ret = -EINVAL;
		goto fn_exit;
	} else {
		if (resp.result != QSEOS_RESULT_SUCCESS) {
			pr_err("Response result %d not supported\n",
							resp.result);
			ret = -EINVAL;
			goto fn_exit;
		}
	}
	if (option == 1) {
		if (msgrsp->status) {
			pr_err("Input size exceeded supported range\n");
			ret = -EINVAL;
		}
		basic_output = msgrsp->data;
	}
fn_exit:
	free_page(pg_addr);
	return ret;
}

static int32_t copy_files(int *img_size)
{
	uint8_t *buf;

	if (mdt_file && seg0_file && seg1_file && seg2_file && seg3_file) {
		*img_size = mdt_size + seg0_size + seg1_size
				+ seg2_size + seg3_size;

		qsee_sbuffer = kzalloc(*img_size, GFP_KERNEL);
		if (!qsee_sbuffer) {
			pr_err("Error: qsee_sbuffer alloc failed\n");
			return -ENOMEM;
		}
		buf = qsee_sbuffer;

		memcpy(buf, mdt_file, mdt_size);
		buf += mdt_size;
		memcpy(buf, seg0_file, seg0_size);
		buf += seg0_size;
		memcpy(buf, seg1_file, seg1_size);
		buf += seg1_size;
		memcpy(buf, seg2_file, seg2_size);
		buf += seg2_size;
		memcpy(buf, seg3_file, seg3_size);
	} else {
		pr_err("\nSampleapp file Inputs not provided\n");
		return -EINVAL;
	}
	return 0;
}

static int load_app(void)
{
	struct qseecom_load_app_ireq load_req;
	struct qseecom_command_scm_resp resp;
	int ret;
	int img_size;

	ret = copy_files(&img_size);
	if (ret) {
		pr_err("Copying Files failed\n");
		return ret;
	}

	/* Populate the structure for sending scm call to load image */
	strlcpy(load_req.app_name, "sampleapp", sizeof("sampleapp"));
	load_req.qsee_cmd_id = QSEOS_APP_START_COMMAND;
	load_req.mdt_len = mdt_size;
	load_req.img_len = img_size;
	load_req.phy_addr = virt_to_phys(qsee_sbuffer);

	/* SCM_CALL to load the app and get the app_id back */
	ret = scm_call(SCM_SVC_TZSCHEDULER, 1,	&load_req,
		sizeof(struct qseecom_load_app_ireq),
		&resp, sizeof(resp));
	if (ret) {
		pr_err("SCM_CALL to load app failed\n");
		return ret;
	}

	if (resp.result == QSEOS_RESULT_FAILURE) {
		pr_err("SCM_CALL rsp.result is QSEOS_RESULT_FAILURE\n");
		return -EFAULT;
	}

	if (resp.result == QSEOS_RESULT_INCOMPLETE)
		pr_err("Process_incomplete_cmd ocurred\n");

	if (resp.result != QSEOS_RESULT_SUCCESS) {
		pr_err("scm_call failed resp.result unknown, %d\n",
				resp.result);
		return -EFAULT;
	}

	pr_info("\n Loaded Sampleapp Succesfully!\n");

	qsee_app_id = resp.data;
	return 0;
}

/* To show basic multiplication output */
static ssize_t
show_basic_output(struct sys_device *dev, struct sysdev_attribute *attr,
					char *buf)
{
	return snprintf(buf, (basic_data_len + 1), "%u", basic_output);
}

/* Basic multiplication App*/
static ssize_t
store_basic_input(struct sys_device *dev, struct sysdev_attribute *attr,
					const char *buf, size_t count)
{
	uint32_t basic_input = 0;
	uint32_t ret = 0;

	basic_data_len = count;
	if ((count - 1) == 0) {
		pr_err("\n Input cannot be NULL!");
		return -EINVAL;
	}
	if (kstrtouint(buf, 10, &basic_input))
		pr_err("\n Please enter a valid unsigned integer");
	else
		ret = tzapp_test(&basic_input, NULL, 0, 1);
	return ret ? ret : count;
}

/* To show encrypted plain text*/
static ssize_t
show_encrypt_output(struct sys_device *dev, struct sysdev_attribute *attr,
					char *buf)
{
	return snprintf(buf, (enc_len + 1), "%s", (char *) encrypt_text);
}

/* To Encrypt input plain text */
static ssize_t
store_encrypt_input(struct sys_device *dev, struct sysdev_attribute *attr,
		const char *buf, size_t count)
{
	int32_t ret = -EINVAL;
	uint8_t *input_pt;
	uint8_t *output_pt;

	enc_len = count - 1;
	if (enc_len == 0) {
		pr_err("\n Input cannot be NULL!");
		return -EINVAL;
	}
	if ((enc_len % 16 != 0) && (enc_len <= MAX_INPUT_SIZE)) {
		pr_err("\n Input Length must be multiple of 16 & < 4096 bytes");
		return -EINVAL;
	}

	input_pt = kzalloc(enc_len * sizeof(uint8_t *), GFP_KERNEL);
	if (!input_pt)
		return -ENOMEM;
	memcpy(input_pt, buf, count);

	output_pt = kzalloc(enc_len * sizeof(uint8_t *), GFP_KERNEL);
	if (!output_pt) {
		kfree(input_pt);
		return -ENOMEM;
	}

	ret = tzapp_test((uint8_t *)input_pt, (uint8_t *)output_pt, enc_len, 2);

	if (!ret)
		memcpy(encrypt_text, output_pt, enc_len);

	kfree(input_pt);
	kfree(output_pt);
	return count;
}

/* To show decrypted cipher text */
static ssize_t
show_decrypt_output(struct sys_device *dev, struct sysdev_attribute *attr,
		 char *buf)
{
	return snprintf(buf, (dec_len + 1), "%s", (char *) decrypt_text);
}

/* To decrypt input cipher text */
static ssize_t
store_decrypt_input(struct sys_device *dev, struct sysdev_attribute *attr,
		const char *buf, size_t count)
{
	int32_t ret = -EINVAL;
	uint8_t *input_pt;
	uint8_t *output_pt;

	dec_len = count - 1;
	if (dec_len == 0) {
		pr_err("\n Input cannot be NULL!");
		return -EINVAL;
	}

	if ((dec_len % 16 != 0) && (dec_len <= MAX_INPUT_SIZE)) {
		pr_err("\n Input Length must be multiple of 16 & < 4096 bytes");
		return -EINVAL;
	}

	input_pt = kzalloc(dec_len * sizeof(uint8_t *), GFP_KERNEL);
	if (!input_pt)
		return -ENOMEM;
	memcpy(input_pt, buf, dec_len);

	output_pt = kzalloc(dec_len * sizeof(uint8_t *), GFP_KERNEL);
	if (!output_pt) {
		kfree(input_pt);
		return -ENOMEM;
	}

	ret = tzapp_test((uint8_t *)input_pt, (uint8_t *)output_pt, dec_len, 3);
	if (!ret)
		memcpy(decrypt_text, output_pt, dec_len);

	kfree(input_pt);
	kfree(output_pt);
	return count;
}

static ssize_t
store_load_start(struct sys_device *dev, struct sysdev_attribute *attr,
		const char *buf, size_t count)
{
	int load_cmd;

	if (kstrtouint(buf, 10, &load_cmd)) {
		pr_err("\n Provide valid integer input!");
		pr_err("Echo 1 to start loading app");
		return -EINVAL;
	}
	if (load_cmd == 1)
		load_app();
	else
		pr_info("\nEcho 1 to start loading app");

	return count;
}

static struct sysdev_attribute tzapp_attr[] = {
	_SYSDEV_ATTR(encrypt, 0666, show_encrypt_output,
					store_encrypt_input),
	_SYSDEV_ATTR(decrypt, 0666, show_decrypt_output,
					store_decrypt_input),
	_SYSDEV_ATTR(basic_data, 0666, show_basic_output,
					store_basic_input),
	_SYSDEV_ATTR(load_start, 0666, NULL,
					store_load_start),

};

static struct sysdev_class tzapp_sysdev_class = {
	.name = "tzapp",
};

static struct sys_device tzapp_sys_device = {
	.id = 0,
	.cls = &tzapp_sysdev_class,
};

static int __init tzapp_create_files(struct sys_device *dev,
					struct sysdev_attribute files[],
					int size)
{
	int i;

	for (i = 0; i < size; i++) {
		int err = sysdev_create_file(dev, &files[i]);
		if (err) {
			pr_err("%s: Error in sysdev_create_file (%s)=%d\n",
				__func__, files[i].attr.name, err);
			return err;
		}
	}
	return 0;
}

static int __init tzapp_init_sysdev(void)
{
	int err;

	err = sysdev_class_register(&tzapp_sysdev_class);
	if (err) {
		pr_err("%s: sysdev_class_register fail (%d)\n",
			__func__, err);
		return err;
	}
	err = sysdev_register(&tzapp_sys_device);
	if (err) {
		pr_err("%s: sysdev_register fail (%d)\n",
			__func__, err);
	}

	return tzapp_create_files(&tzapp_sys_device, tzapp_attr,
				ARRAY_SIZE(tzapp_attr));
}

static int __init qseecom_init(void)
{
	sysfs_create_bin_file(firmware_kobj, &mdt_attr);
	sysfs_create_bin_file(firmware_kobj, &seg0_attr);
	sysfs_create_bin_file(firmware_kobj, &seg1_attr);
	sysfs_create_bin_file(firmware_kobj, &seg2_attr);
	sysfs_create_bin_file(firmware_kobj, &seg3_attr);

	if (!tzapp_init_sysdev())
		pr_info("\nLoaded Module Successfully!\n");
	else
		pr_info("\nFailed to load module\n");
	return 0;
}

static void __exit qseecom_exit(void)
{
	qseecom_unload_app();

	sysfs_remove_bin_file(firmware_kobj, &mdt_attr);
	sysfs_remove_bin_file(firmware_kobj, &seg0_attr);
	sysfs_remove_bin_file(firmware_kobj, &seg1_attr);
	sysfs_remove_bin_file(firmware_kobj, &seg2_attr);
	sysfs_remove_bin_file(firmware_kobj, &seg3_attr);

	sysdev_class_unregister(&tzapp_sysdev_class);
	sysdev_unregister(&tzapp_sys_device);

	kfree(mdt_file);
	kfree(seg0_file);
	kfree(seg1_file);
	kfree(seg2_file);
	kfree(seg3_file);
	kfree(qsee_sbuffer);
}
MODULE_LICENSE("GPL v2");
module_init(qseecom_init);
module_exit(qseecom_exit);
