#include "bin/default/librpc/gen_ndr/ndr_dbgidl.h"
#ifndef __SRV_DBGIDL__
#define __SRV_DBGIDL__
void _dummy_dbgidl(struct pipes_struct *p, struct dummy_dbgidl *r);
const struct api_struct *dbgidl_get_pipe_fns(int *n_fns);
struct rpc_srv_callbacks;
NTSTATUS rpc_dbgidl_init(const struct rpc_srv_callbacks *rpc_srv_cb);
NTSTATUS rpc_dbgidl_shutdown(void);
#endif /* __SRV_DBGIDL__ */
