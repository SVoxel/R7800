#include "bin/default/librpc/gen_ndr/ndr_fscc.h"
#ifndef __SRV_FSCC__
#define __SRV_FSCC__
const struct api_struct *fscc_get_pipe_fns(int *n_fns);
struct rpc_srv_callbacks;
NTSTATUS rpc_fscc_init(const struct rpc_srv_callbacks *rpc_srv_cb);
NTSTATUS rpc_fscc_shutdown(void);
#endif /* __SRV_FSCC__ */
