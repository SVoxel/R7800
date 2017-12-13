#include "bin/default/librpc/gen_ndr/ndr_bkupblobs.h"
#ifndef __SRV_BKUP__
#define __SRV_BKUP__
const struct api_struct *bkup_get_pipe_fns(int *n_fns);
struct rpc_srv_callbacks;
NTSTATUS rpc_bkup_init(const struct rpc_srv_callbacks *rpc_srv_cb);
NTSTATUS rpc_bkup_shutdown(void);
#endif /* __SRV_BKUP__ */
