#include "bin/default/librpc/gen_ndr/ndr_notify.h"
#ifndef __SRV_NOTIFY__
#define __SRV_NOTIFY__
const struct api_struct *notify_get_pipe_fns(int *n_fns);
struct rpc_srv_callbacks;
NTSTATUS rpc_notify_init(const struct rpc_srv_callbacks *rpc_srv_cb);
NTSTATUS rpc_notify_shutdown(void);
#endif /* __SRV_NOTIFY__ */
