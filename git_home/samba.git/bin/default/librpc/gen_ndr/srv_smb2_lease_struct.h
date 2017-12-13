#include "bin/default/librpc/gen_ndr/ndr_smb2_lease_struct.h"
#ifndef __SRV_SMB2_LEASE_STRUCT__
#define __SRV_SMB2_LEASE_STRUCT__
const struct api_struct *smb2_lease_struct_get_pipe_fns(int *n_fns);
struct rpc_srv_callbacks;
NTSTATUS rpc_smb2_lease_struct_init(const struct rpc_srv_callbacks *rpc_srv_cb);
NTSTATUS rpc_smb2_lease_struct_shutdown(void);
#endif /* __SRV_SMB2_LEASE_STRUCT__ */
