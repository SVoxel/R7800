#include "bin/default/librpc/gen_ndr/ndr_nbt.h"
#ifndef __SRV_NBT__
#define __SRV_NBT__
void _decode_netlogon_samlogon_response_packet(struct pipes_struct *p, struct decode_netlogon_samlogon_response_packet *r);
void _decode_nbt_netlogon_packet(struct pipes_struct *p, struct decode_nbt_netlogon_packet *r);
const struct api_struct *nbt_get_pipe_fns(int *n_fns);
struct rpc_srv_callbacks;
NTSTATUS rpc_nbt_init(const struct rpc_srv_callbacks *rpc_srv_cb);
NTSTATUS rpc_nbt_shutdown(void);
#endif /* __SRV_NBT__ */
