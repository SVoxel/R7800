#ifndef _PLC_DEVS_H
#define _PLC_DEVS_H

/* ---------------files for plc devices--------------- */
#define PLC_DEV_FILE		"/tmp/plc_dev_file"
#define PLC_RULES			"/tmp/rules.txt"
#define PLC_VLAN_QOS		"/tmp/plc_vlan_tos_file"


/*  ---------------plc variables --------------- */
/* config_plc_dev_led_control */
extern char *led_mac;
extern char *led_set_status;

/* config_plc_dev_reset */
extern char *reset_mac;

/* config_plc_dev_config */
extern int mac_select_num;
extern char *dev_config_info;

/* config_plc_qos_setting */
extern char *plc_qos_all_mac;
/* ------------------------------------------- */

/* -----------------plc_devs.c----------------- */
extern char *get_string_segment(char * str, int seg_num, char * delim);
extern void plc_rules_write(FILE *filep, char *qos_name, char *rules_type);
extern void plc_rules_read(void);
extern void plc_rules_file_update(void);


/* ---------------plc_devs_action.c--------------- */
extern void cmd_plc_led_set(char *led_mac,  char *led_set_status);
extern void cmd_plc_reset(char * reset_mac);
extern void cmd_plc_set_keys(int mac_select_num, char * dev_config_info);
extern void cmd_plc_qos_set(char * plc_qos_all_mac);


/* ---------------plc_devs_asp.c--------------- */
extern void asp_plc_devs_detection(FILE * fp, char * * argv);
extern void asp_plc_qos_mac_items(FILE *fp, char **argv);
extern void asp_plc_qos_port_items(FILE *fp, char **argv);
extern void asp_plc_scan_devs(FILE *fp, char **argv);
extern void asp_plc_availible_devs(FILE *fp, char **argv);

/* -------------function from asp.c--------------- */
extern void list_nat_items(FILE *fp, char *nat_prefix, char *array_prefix, char *num_prefix);
extern char *sed_items(char *sed);

/* ---------------plc_devs_apply.c--------------- */
#ifndef CGI_STRUCT
#define CGI_STRUCT
struct cgi_struct
{
	char *name;
	char *value;
};
#endif

extern void config_plc_avail_dev(struct cgi_struct *cgi, int num);
extern void config_plc_dev_led_control(struct cgi_struct *cgi, int num);
extern void config_plc_dev_reset(struct cgi_struct *cgi, int num);
extern void config_plc_dev_config(struct cgi_struct *cgi, int num);
extern void config_plc_qos_setting(struct cgi_struct *cgi, int num);
extern void config_plc_qos_dev_mac_config(struct cgi_struct *cgi, int num);
extern void config_plc_qos_dev_mac_delete(struct cgi_struct *cgi, int num);
extern void config_plc_qos_dev_port_config(struct cgi_struct *cgi, int num);
extern void config_plc_qos_dev_port_delete(struct cgi_struct *cgi, int num);
extern void config_plc_qos_mac_add(struct cgi_struct *cgi, int num);
extern void config_plc_qos_mac_edit(struct cgi_struct *cgi, int num);
extern void config_plc_qos_port_add(struct cgi_struct *cgi, int num);
extern void config_plc_qos_port_edit(struct cgi_struct *cgi, int num);

/* -------------function from apply.c ------------------ */
#ifndef CGI_SET
#define CGI_SET
struct cgi_set
{
	char *cgi_name;	/* the item name on form page */
	char *cfg_name;	/* the item name in configuring */
};
#endif

extern int add_items(char *prefix, char *value);
extern int del_items(char *prefix, char *del);
extern void del_all_items(char *prefix);
extern char *cgi_value(char *name, struct cgi_struct *cgi, int num);
extern void do_setting(struct cgi_set *cfg, struct cgi_struct *cgi, int num);

#endif


