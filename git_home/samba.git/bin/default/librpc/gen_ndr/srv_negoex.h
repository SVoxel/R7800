#include "bin/default/librpc/gen_ndr/ndr_negoex.h"
#ifndef __SRV_NEGOEX__
#define __SRV_NEGOEX__
void _decode_negoex_MESSAGE(struct pipes_struct *p, struct decode_negoex_MESSAGE *r);
const struct api_struct *negoex_get_pipe_fns(int *n_fns);
struct rpc_srv_callbacks;
NTSTATUS rpc_negoex_init(const struct rpc_srv_callbacks *rpc_srv_cb);
NTSTATUS rpc_negoex_shutdown(void);
#endif /* __SRV_NEGOEX__ */
