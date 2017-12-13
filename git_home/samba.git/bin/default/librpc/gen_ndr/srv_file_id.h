#include "bin/default/librpc/gen_ndr/ndr_file_id.h"
#ifndef __SRV_FILE_ID__
#define __SRV_FILE_ID__
const struct api_struct *file_id_get_pipe_fns(int *n_fns);
struct rpc_srv_callbacks;
NTSTATUS rpc_file_id_init(const struct rpc_srv_callbacks *rpc_srv_cb);
NTSTATUS rpc_file_id_shutdown(void);
#endif /* __SRV_FILE_ID__ */
