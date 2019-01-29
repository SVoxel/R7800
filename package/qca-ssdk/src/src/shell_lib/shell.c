/*
 * Copyright (c) 2013, The Linux Foundation. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "shell.h"
#include "shell_io.h"
#include "shell_sw.h"
#include "shell_config.h"
#include "api_access.h"

a_uint32_t ioctl_buf[IOCTL_BUF_SIZE];
a_uint32_t ioctl_argp[CMDSTR_ARGS_MAX*4];

void
cmd_print_error(sw_error_t rtn)
{

}

void
cmd_print(char *fmt, ...)
{

}

static sw_error_t
cmd_input_parser(a_uint32_t *arg_val, a_uint32_t arg_index, sw_api_param_t *pp)
{
    a_int16_t i;
    a_uint32_t *pbuf;
    a_uint16_t rtn_size = 1;
    sw_api_param_t *pptmp = NULL;;

    pbuf = ioctl_buf + rtn_size;    /*reserve for return value */

    for (i = 0; i < arg_index; i++)
    {
        pptmp = pp + i;
        if (pptmp->param_type & SW_PARAM_PTR)
        {
            pbuf += (pptmp->data_size + 3) / 4;
        }
    }

    if (pptmp == NULL)
        return SW_BAD_PTR;

    if ((pbuf - ioctl_buf + (pptmp->data_size + 3) / 4) > (IOCTL_BUF_SIZE/4))
    {
        return SW_NO_RESOURCE;
    }

    *arg_val = (a_uint32_t) pbuf;

    return SW_OK;
}

static sw_error_t
cmd_api_func(sw_api_func_t *fp, a_uint32_t nr_param, a_uint32_t * args)
{
    a_uint32_t *p = &args[2];
    sw_error_t rv;
    sw_error_t(*func) ();

    func = fp->func;

    switch (nr_param)
    {
        case 0:
            rv = (func) ();
            break;
        case 1:
            rv = (func) (p[0]);
            break;
        case 2:
            rv = (func) (p[0], p[1]);
            break;
        case 3:
            rv = (func) (p[0], p[1], p[2]);
            break;
        case 4:
            rv = (func) (p[0], p[1], p[2], p[3]);
            break;
        case 5:
            rv = (func) (p[0], p[1], p[2], p[3], p[4]);
            break;
        case 6:
            rv = (func) (p[0], p[1], p[2], p[3], p[4], p[5]);
            break;
        case 7:
            rv = (func) (p[0], p[1], p[2], p[3], p[4], p[5], p[6]);
            break;
        case 8:
            rv = (func) (p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);
            break;
        case 9:
            rv = (func) (p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8]);
            break;
        case 10:
            rv = (func) (p[0], p[1], p[2], p[3], p[4], p[5],
                         p[6], p[7], p[8], p[9]);
            break;
        default:
            rv = SW_OUT_OF_RANGE;
    }

    *(a_uint32_t *) args[1] = rv;

    return rv;
}

static sw_error_t
cmd_api_output(sw_api_param_t *pp, a_uint32_t nr_param, a_uint32_t * args)
{
    a_uint16_t i;
    a_uint32_t *pbuf;
    a_uint16_t rtn_size = 1;
    sw_error_t rtn = (sw_error_t) (*ioctl_buf);
    sw_api_param_t *pptmp = NULL;

    if (rtn != SW_OK)
    {
        cmd_print_error(rtn);
        return rtn;
    }

    pbuf = ioctl_buf + rtn_size;
    for (i = 0; i < nr_param; i++)
    {
        pptmp = pp + i;
        if (pptmp->param_type & SW_PARAM_PTR)
        {

            if (pptmp->param_type & SW_PARAM_OUT)
            {

                sw_data_type_t *data_type;
                if (!(data_type = cmd_data_type_find(pptmp->data_type)))
                    return SW_NO_SUCH;

                if (data_type->show_func)
                {
                    data_type->show_func(pptmp->param_name, pbuf, pptmp->data_size);
                }
                else
                {
                    dprintf("\n Error, not define output print function!");
                }
            }

            if ((pbuf - ioctl_buf +
                    (pptmp->data_size + 3) / 4) > (IOCTL_BUF_SIZE/4))
                return SW_NO_RESOURCE;

            pbuf += (pptmp->data_size + 3) / 4;

        }
    }
    return SW_OK;
}

void
cmd_strtol(char *str, a_uint32_t * arg_val)
{
    if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X'))
        sscanf(str, "%x", arg_val);
    else
        sscanf(str, "%d", arg_val);
}

static sw_error_t
cmd_parse_api(char **cmd_str, a_uint32_t * arg_val)
{
    char *tmp_str;
    a_uint32_t arg_index, arg_start = 2, reserve_index = 1; /*reserve for dev_id */
    a_uint32_t last_param_in = 0;
    a_uint32_t *temp;
    void *pentry;
    sw_api_param_t *pptmp = NULL;
    sw_api_t sw_api;
    a_uint32_t ignorecnt = 0;
    sw_api.api_id = arg_val[0];
    SW_RTN_ON_ERROR(sw_api_get(&sw_api));

    /*set device id */
    arg_val[arg_start] = get_devid();

    for (arg_index = reserve_index; arg_index < sw_api.api_nr; arg_index++)
    {
        tmp_str = NULL;
        pptmp = sw_api.api_pp + arg_index;

        if (!(pptmp->param_type & SW_PARAM_IN))
        {
            ignorecnt++;
        }

        if (pptmp->param_type & SW_PARAM_IN)
        {
            tmp_str = cmd_str[arg_index - reserve_index - ignorecnt];
            last_param_in = arg_index;
            if((pptmp->api_id == 314) && last_param_in == 2) last_param_in = 4;//SW_API_FDB_EXTEND_NEXT wr
            if((pptmp->api_id == 327) && last_param_in == 2) last_param_in = 4;//SW_API_FDB_EXTEND_FIRST wr
        }
        temp = &arg_val[arg_start + arg_index];

        sw_data_type_t *data_type;
        if (!(data_type = cmd_data_type_find(pptmp->data_type)))
            return SW_NO_SUCH;

        pentry = temp;
        if (pptmp->param_type & SW_PARAM_PTR)
        {
            if (cmd_input_parser(temp, arg_index, sw_api.api_pp) != SW_OK)
                return SW_NO_RESOURCE;

            pentry = (void *) *temp;
        }

        if (pptmp->param_type & SW_PARAM_IN)
        {
#if 1
            if(pptmp->param_type & SW_PARAM_PTR)   //quiet mode
            {
                if(!get_talk_mode())
                    set_full_cmdstrp((char **)(cmd_str + (last_param_in - reserve_index)));
            }
#endif
            /*check and convert input param */
            if (data_type->param_check != NULL)
            {
                if (data_type->param_check(tmp_str, pentry, pptmp->data_size) != SW_OK)
                    return SW_BAD_PARAM;
            }
        }
    }

    /*superfluous args */
    /*
       if(cmd_str[last_param_in] != NULL)
       return SW_BAD_PARAM;
     */

    return SW_OK;
}

/*user command api*/
sw_error_t
cmd_exec_api(a_uint32_t *arg_val)
{
    sw_error_t rv;
    sw_api_t sw_api;

    sw_api.api_id = arg_val[0];
    SW_RTN_ON_ERROR(sw_api_get(&sw_api));

    /*save cmd return value */
    arg_val[1] = (a_uint32_t) ioctl_buf;
    /*save set device id */
    arg_val[2] = get_devid();

    rv = cmd_api_func(sw_api.api_fp, sw_api.api_nr, arg_val);
    SW_RTN_ON_ERROR(rv);

    rv = cmd_api_output(sw_api.api_pp, sw_api.api_nr, arg_val);
    SW_RTN_ON_ERROR(rv);

    return rv;
}


void
cmd_print_usage (int cmd_index, int cmd_index_sub)
{
    if(GCMD_NAME(cmd_index))
        dprintf("usage: %s", GCMD_NAME(cmd_index));

    if (GCMD_SUB_NAME(cmd_index, cmd_index_sub))
        dprintf(" %s", GCMD_SUB_NAME(cmd_index, cmd_index_sub));

    if(GCMD_SUB_ACT(cmd_index, cmd_index_sub) && GCMD_SUB_USAGE(cmd_index, cmd_index_sub))
        dprintf(" %s %s\n\n", GCMD_SUB_ACT(cmd_index, cmd_index_sub),
                GCMD_SUB_USAGE(cmd_index, cmd_index_sub));
}
/*
    main function
    input args:
        arg_val[0] = cmd_num
        arg_val[1] = rtn_code
        arg_val[2] = dev_id
        arg_val[3] = dbg_cmd_num or other
*/

/*command string lookup*/
a_uint32_t
cmd_lookup(char **cmd_str, int *cmd_index, int *cmd_index_sub)
{
    a_uint32_t no, sub_no;
    a_uint32_t cmd_deepth = 0;

    *cmd_index = GCMD_DESC_NO_MATCH;
    *cmd_index_sub = GCMD_DESC_NO_MATCH;

    if (cmd_str[0] == NULL)
        return cmd_deepth;

    for (no = 0; GCMD_DESC_VALID(no); no++)
    {
        if (strcasecmp(cmd_str[0], GCMD_NAME(no)))
            continue;

        for (sub_no = 0; GCMD_SUB_DESC_VALID(no, sub_no); sub_no++)
        {
            if (cmd_str[1] != NULL && cmd_str[2] != NULL)
            {

                if (GCMD_SUB_NAME(no, sub_no) && GCMD_SUB_ACT(no, sub_no)
                        && !strcasecmp(cmd_str[1], GCMD_SUB_NAME(no, sub_no))
                        && !strcasecmp(cmd_str[2], GCMD_SUB_ACT(no, sub_no)))
                {
                    *cmd_index = no;
                    *cmd_index_sub = sub_no;
                    cmd_deepth = 3;
                    return cmd_deepth;
                }

                else if (!GCMD_SUB_NAME(no, sub_no) && GCMD_SUB_ACT(no, sub_no)
                         && !strcasecmp(cmd_str[1], GCMD_SUB_ACT(no, sub_no)))
                {
                    *cmd_index = no;
                    *cmd_index_sub = sub_no;
                    cmd_deepth = 2;
                    return cmd_deepth;
                }
            }
            else if (cmd_str[1] != NULL && cmd_str[2] == NULL)
            {

                if (!GCMD_SUB_NAME(no, sub_no) && GCMD_SUB_ACT(no, sub_no)
                        && !strcasecmp(cmd_str[1], GCMD_SUB_ACT(no, sub_no)))
                {
                    *cmd_index = no;
                    *cmd_index_sub = sub_no;
                    cmd_deepth = 2;
                    return cmd_deepth;
                }
            }
        }
    }

    return cmd_deepth;
}

static a_uint32_t *
cmd_parse(char *cmd_str, int *cmd_index, int *cmd_index_sub)
{
    int cmd_nr = 0;
    a_uint32_t *arg_val = ioctl_argp;
    char *tmp_str[CMDSTR_ARGS_MAX];
    int rtn_code;

    if (cmd_str == NULL)
        return NULL;

    memset(arg_val, 0, CMDSTR_ARGS_MAX * sizeof (a_uint32_t));

    /* split string into array */
    if ((tmp_str[cmd_nr] = (void *) strsep(&cmd_str, " ")) == NULL)
        return NULL;

    /*handle help */
    if (!strcasecmp(tmp_str[cmd_nr], "help"))
    {
        dprintf("input ? get help\n\n");
        return NULL;
    }

    while (tmp_str[cmd_nr])
    {
        if (++cmd_nr == 3)
            break;

        tmp_str[cmd_nr] = (void *) strsep(&cmd_str, " ");
    }

    /*commond string lookup */
    int cmd_depth = cmd_lookup(tmp_str, cmd_index, cmd_index_sub);

    if (*cmd_index == GCMD_DESC_NO_MATCH || *cmd_index_sub == GCMD_DESC_NO_MATCH)
    {
        dprintf("invalid or incomplete command.\n\n");
        return NULL;
    }

    /*parse param */
    cmd_nr = 0;
    if (cmd_depth == 2)
    {
        tmp_str[cmd_nr] = tmp_str[2];
        cmd_nr++;
    }

    tmp_str[cmd_nr] = (void *) strsep(&cmd_str, " ");
    while (tmp_str[cmd_nr])
    {
        if (++cmd_nr == CMDSTR_ARGS_MAX)
            break;
        tmp_str[cmd_nr] = (void *) strsep(&cmd_str, " ");
    }

    arg_val[0] = GCMD_SUB_API(*cmd_index, *cmd_index_sub);
    arg_val[1] = (a_uint32_t) ioctl_buf;

    if (arg_val[0] < SW_API_MAX)
    {
        /*api command parse */
        rtn_code = cmd_parse_api(tmp_str, arg_val);

    }
    else if (arg_val[0] > SW_API_MAX)
    {
    }
    else
    {
        rtn_code = SW_BAD_PARAM;
    }

    if(rtn_code != SW_OK)
    {
        cmd_print_error(rtn_code);

        if(rtn_code == SW_BAD_PARAM)
            cmd_print_usage(*cmd_index, *cmd_index_sub);

        return NULL;
    }

    return arg_val;
}

static int
cmd_exec(a_uint32_t *arg_val, int cmd_index, int cmd_index_sub)
{
    a_uint32_t api_id = arg_val[0];
    sw_error_t rtn;

    if( api_id < SW_API_MAX )
    {
        rtn = cmd_exec_api(arg_val);

    }
    else if ((api_id > SW_API_MAX ) && (api_id < SW_CMD_MAX))
    {
    }
    else
    {
        rtn = SW_BAD_PARAM;
    }

    if(rtn != SW_OK)
        cmd_print_error(rtn);
    else
        dprintf("\noperate done.\n\n");

    return 0;
}

int
cmd_run_one(char *cmd_str)
{
    a_uint32_t *arg_list;
    int cmd_index = 0, cmd_index_sub = 0;

    if ((arg_list = cmd_parse(cmd_str, &cmd_index, &cmd_index_sub)) != NULL)
    {
        cmd_exec(arg_list, cmd_index, cmd_index_sub);
    }

    return SW_OK;
}

/* Dummy function to avoid linker complaints */
void __aeabi_unwind_cpp_pr0(void)
{
};
void __aeabi_unwind_cpp_pr1(void)
{
};

