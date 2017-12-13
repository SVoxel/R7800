#include "librpc/rpc/dcerpc.h"
#include "bin/default/source4/librpc/gen_ndr/winsrepl.h"
#ifndef _HEADER_RPC_wrepl
#define _HEADER_RPC_wrepl

extern const struct ndr_interface_table ndr_table_wrepl;

struct tevent_req *dcerpc_decode_winsrepl_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct decode_winsrepl *r);
NTSTATUS dcerpc_decode_winsrepl_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_decode_winsrepl_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct decode_winsrepl *r);
struct tevent_req *dcerpc_decode_winsrepl_send(TALLOC_CTX *mem_ctx,
					       struct tevent_context *ev,
					       struct dcerpc_binding_handle *h,
					       struct wrepl_wrap _p /* [in]  */);
NTSTATUS dcerpc_decode_winsrepl_recv(struct tevent_req *req,
				     TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_decode_winsrepl(struct dcerpc_binding_handle *h,
				TALLOC_CTX *mem_ctx,
				struct wrepl_wrap _p /* [in]  */);

#endif /* _HEADER_RPC_wrepl */
