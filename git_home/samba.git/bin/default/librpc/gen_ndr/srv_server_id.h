#include "bin/default/librpc/gen_ndr/ndr_server_id.h"
#ifndef __SRV_SERVER_ID__
#define __SRV_SERVER_ID__
const struct api_struct *server_id_get_pipe_fns(int *n_fns);
struct rpc_srv_callbacks;
NTSTATUS rpc_server_id_init(const struct rpc_srv_callbacks *rpc_srv_cb);
NTSTATUS rpc_server_id_shutdown(void);
#endif /* __SRV_SERVER_ID__ */
