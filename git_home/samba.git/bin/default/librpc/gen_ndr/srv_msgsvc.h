#include "bin/default/librpc/gen_ndr/ndr_msgsvc.h"
#ifndef __SRV_MSGSVC__
#define __SRV_MSGSVC__
void _NetrMessageNameAdd(struct pipes_struct *p, struct NetrMessageNameAdd *r);
void _NetrMessageNameEnum(struct pipes_struct *p, struct NetrMessageNameEnum *r);
void _NetrMessageNameGetInfo(struct pipes_struct *p, struct NetrMessageNameGetInfo *r);
void _NetrMessageNameDel(struct pipes_struct *p, struct NetrMessageNameDel *r);
const struct api_struct *msgsvc_get_pipe_fns(int *n_fns);
struct rpc_srv_callbacks;
NTSTATUS rpc_msgsvc_init(const struct rpc_srv_callbacks *rpc_srv_cb);
NTSTATUS rpc_msgsvc_shutdown(void);
#endif /* __SRV_MSGSVC__ */
#ifndef __SRV_MSGSVCSEND__
#define __SRV_MSGSVCSEND__
void _NetrSendMessage(struct pipes_struct *p, struct NetrSendMessage *r);
const struct api_struct *msgsvcsend_get_pipe_fns(int *n_fns);
struct rpc_srv_callbacks;
NTSTATUS rpc_msgsvcsend_init(const struct rpc_srv_callbacks *rpc_srv_cb);
NTSTATUS rpc_msgsvcsend_shutdown(void);
#endif /* __SRV_MSGSVCSEND__ */
