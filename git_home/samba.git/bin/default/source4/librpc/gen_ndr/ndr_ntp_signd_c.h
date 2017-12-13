#include "librpc/rpc/dcerpc.h"
#include "bin/default/source4/librpc/gen_ndr/ntp_signd.h"
#ifndef _HEADER_RPC_ntp_signd
#define _HEADER_RPC_ntp_signd

extern const struct ndr_interface_table ndr_table_ntp_signd;

struct tevent_req *dcerpc_decode_sign_requst_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct decode_sign_requst *r);
NTSTATUS dcerpc_decode_sign_requst_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_decode_sign_requst_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct decode_sign_requst *r);
struct tevent_req *dcerpc_decode_sign_requst_send(TALLOC_CTX *mem_ctx,
						  struct tevent_context *ev,
						  struct dcerpc_binding_handle *h,
						  struct sign_request _request /* [in]  */);
NTSTATUS dcerpc_decode_sign_requst_recv(struct tevent_req *req,
					TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_decode_sign_requst(struct dcerpc_binding_handle *h,
				   TALLOC_CTX *mem_ctx,
				   struct sign_request _request /* [in]  */);

#endif /* _HEADER_RPC_ntp_signd */
