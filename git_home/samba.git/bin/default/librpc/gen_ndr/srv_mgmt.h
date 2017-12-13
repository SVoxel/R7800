#include "bin/default/librpc/gen_ndr/ndr_mgmt.h"
#ifndef __SRV_MGMT__
#define __SRV_MGMT__
WERROR _mgmt_inq_if_ids(struct pipes_struct *p, struct mgmt_inq_if_ids *r);
WERROR _mgmt_inq_stats(struct pipes_struct *p, struct mgmt_inq_stats *r);
uint32_t _mgmt_is_server_listening(struct pipes_struct *p, struct mgmt_is_server_listening *r);
WERROR _mgmt_stop_server_listening(struct pipes_struct *p, struct mgmt_stop_server_listening *r);
WERROR _mgmt_inq_princ_name(struct pipes_struct *p, struct mgmt_inq_princ_name *r);
const struct api_struct *mgmt_get_pipe_fns(int *n_fns);
struct rpc_srv_callbacks;
NTSTATUS rpc_mgmt_init(const struct rpc_srv_callbacks *rpc_srv_cb);
NTSTATUS rpc_mgmt_shutdown(void);
#endif /* __SRV_MGMT__ */
