#include "bin/default/librpc/gen_ndr/ndr_frsblobs.h"
#ifndef __SRV_FRSBLOBS__
#define __SRV_FRSBLOBS__
void _decode_nt_backup_file(struct pipes_struct *p, struct decode_nt_backup_file *r);
void _decode_frsrpc_StageHeader(struct pipes_struct *p, struct decode_frsrpc_StageHeader *r);
void _decode_FileNetworkOpenInformation(struct pipes_struct *p, struct decode_FileNetworkOpenInformation *r);
const struct api_struct *frsblobs_get_pipe_fns(int *n_fns);
struct rpc_srv_callbacks;
NTSTATUS rpc_frsblobs_init(const struct rpc_srv_callbacks *rpc_srv_cb);
NTSTATUS rpc_frsblobs_shutdown(void);
#endif /* __SRV_FRSBLOBS__ */
