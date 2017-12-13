#include "bin/default/librpc/gen_ndr/ndr_frsrpc.h"
#ifndef __SRV_FRSRPC__
#define __SRV_FRSRPC__
WERROR _frsrpc_FrsSendCommPkt(struct pipes_struct *p, struct frsrpc_FrsSendCommPkt *r);
WERROR _frsrpc_FrsVerifyPromotionParent(struct pipes_struct *p, struct frsrpc_FrsVerifyPromotionParent *r);
WERROR _frsrpc_FrsStartPromotionParent(struct pipes_struct *p, struct frsrpc_FrsStartPromotionParent *r);
WERROR _frsrpc_FrsNOP(struct pipes_struct *p, struct frsrpc_FrsNOP *r);
const struct api_struct *frsrpc_get_pipe_fns(int *n_fns);
struct rpc_srv_callbacks;
NTSTATUS rpc_frsrpc_init(const struct rpc_srv_callbacks *rpc_srv_cb);
NTSTATUS rpc_frsrpc_shutdown(void);
#endif /* __SRV_FRSRPC__ */
