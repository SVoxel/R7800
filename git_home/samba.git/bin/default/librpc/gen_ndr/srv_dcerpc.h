#include "bin/default/librpc/gen_ndr/ndr_dcerpc.h"
#ifndef __SRV_DCERPC__
#define __SRV_DCERPC__
const struct api_struct *dcerpc_get_pipe_fns(int *n_fns);
struct rpc_srv_callbacks;
NTSTATUS rpc_dcerpc_init(const struct rpc_srv_callbacks *rpc_srv_cb);
NTSTATUS rpc_dcerpc_shutdown(void);
#endif /* __SRV_DCERPC__ */
