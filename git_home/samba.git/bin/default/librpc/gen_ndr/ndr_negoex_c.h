#include "librpc/rpc/dcerpc.h"
#include "bin/default/librpc/gen_ndr/negoex.h"
#ifndef _HEADER_RPC_negoex
#define _HEADER_RPC_negoex

extern const struct ndr_interface_table ndr_table_negoex;

struct tevent_req *dcerpc_decode_negoex_MESSAGE_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct decode_negoex_MESSAGE *r);
NTSTATUS dcerpc_decode_negoex_MESSAGE_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_decode_negoex_MESSAGE_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct decode_negoex_MESSAGE *r);
struct tevent_req *dcerpc_decode_negoex_MESSAGE_send(TALLOC_CTX *mem_ctx,
						     struct tevent_context *ev,
						     struct dcerpc_binding_handle *h,
						     struct negoex_MESSAGE_ARRAY _array /* [in]  */);
NTSTATUS dcerpc_decode_negoex_MESSAGE_recv(struct tevent_req *req,
					   TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_decode_negoex_MESSAGE(struct dcerpc_binding_handle *h,
				      TALLOC_CTX *mem_ctx,
				      struct negoex_MESSAGE_ARRAY _array /* [in]  */);

#endif /* _HEADER_RPC_negoex */
