#include "bin/default/librpc/gen_ndr/ndr_named_pipe_auth.h"
#ifndef __SRV_NAMED_PIPE_AUTH__
#define __SRV_NAMED_PIPE_AUTH__
const struct api_struct *named_pipe_auth_get_pipe_fns(int *n_fns);
struct rpc_srv_callbacks;
NTSTATUS rpc_named_pipe_auth_init(const struct rpc_srv_callbacks *rpc_srv_cb);
NTSTATUS rpc_named_pipe_auth_shutdown(void);
#endif /* __SRV_NAMED_PIPE_AUTH__ */
