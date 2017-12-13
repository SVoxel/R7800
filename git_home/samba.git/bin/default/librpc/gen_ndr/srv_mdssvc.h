#include "bin/default/librpc/gen_ndr/ndr_mdssvc.h"
#ifndef __SRV_MDSSVC__
#define __SRV_MDSSVC__
void _mdssvc_open(struct pipes_struct *p, struct mdssvc_open *r);
void _mdssvc_unknown1(struct pipes_struct *p, struct mdssvc_unknown1 *r);
void _mdssvc_cmd(struct pipes_struct *p, struct mdssvc_cmd *r);
void _mdssvc_close(struct pipes_struct *p, struct mdssvc_close *r);
const struct api_struct *mdssvc_get_pipe_fns(int *n_fns);
struct rpc_srv_callbacks;
NTSTATUS rpc_mdssvc_init(const struct rpc_srv_callbacks *rpc_srv_cb);
NTSTATUS rpc_mdssvc_shutdown(void);
#endif /* __SRV_MDSSVC__ */
