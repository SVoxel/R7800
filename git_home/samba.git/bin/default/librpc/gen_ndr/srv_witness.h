#include "bin/default/librpc/gen_ndr/ndr_witness.h"
#ifndef __SRV_WITNESS__
#define __SRV_WITNESS__
WERROR _witness_GetInterfaceList(struct pipes_struct *p, struct witness_GetInterfaceList *r);
WERROR _witness_Register(struct pipes_struct *p, struct witness_Register *r);
WERROR _witness_UnRegister(struct pipes_struct *p, struct witness_UnRegister *r);
WERROR _witness_AsyncNotify(struct pipes_struct *p, struct witness_AsyncNotify *r);
WERROR _witness_RegisterEx(struct pipes_struct *p, struct witness_RegisterEx *r);
const struct api_struct *witness_get_pipe_fns(int *n_fns);
struct rpc_srv_callbacks;
NTSTATUS rpc_witness_init(const struct rpc_srv_callbacks *rpc_srv_cb);
NTSTATUS rpc_witness_shutdown(void);
#endif /* __SRV_WITNESS__ */
