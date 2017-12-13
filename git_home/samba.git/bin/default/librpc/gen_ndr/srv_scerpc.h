#include "bin/default/librpc/gen_ndr/ndr_scerpc.h"
#ifndef __SRV_SCERPC__
#define __SRV_SCERPC__
WERROR _scerpc_Unknown0(struct pipes_struct *p, struct scerpc_Unknown0 *r);
const struct api_struct *scerpc_get_pipe_fns(int *n_fns);
struct rpc_srv_callbacks;
NTSTATUS rpc_scerpc_init(const struct rpc_srv_callbacks *rpc_srv_cb);
NTSTATUS rpc_scerpc_shutdown(void);
#endif /* __SRV_SCERPC__ */
