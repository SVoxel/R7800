#include "bin/default/librpc/gen_ndr/ndr_security.h"
#ifndef __SRV_SECURITY__
#define __SRV_SECURITY__
void _decode_security_descriptor(struct pipes_struct *p, struct decode_security_descriptor *r);
void _decode_sec_desc_buf(struct pipes_struct *p, struct decode_sec_desc_buf *r);
void _decode_security_token(struct pipes_struct *p, struct decode_security_token *r);
void _decode_security_unix_token(struct pipes_struct *p, struct decode_security_unix_token *r);
const struct api_struct *security_get_pipe_fns(int *n_fns);
struct rpc_srv_callbacks;
NTSTATUS rpc_security_init(const struct rpc_srv_callbacks *rpc_srv_cb);
NTSTATUS rpc_security_shutdown(void);
#endif /* __SRV_SECURITY__ */
