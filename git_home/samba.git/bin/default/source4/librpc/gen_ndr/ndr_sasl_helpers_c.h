#include "librpc/rpc/dcerpc.h"
#include "bin/default/source4/librpc/gen_ndr/sasl_helpers.h"
#ifndef _HEADER_RPC_sasl_helpers
#define _HEADER_RPC_sasl_helpers

extern const struct ndr_interface_table ndr_table_sasl_helpers;

struct tevent_req *dcerpc_decode_saslauthd_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct decode_saslauthd *r);
NTSTATUS dcerpc_decode_saslauthd_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_decode_saslauthd_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct decode_saslauthd *r);
struct tevent_req *dcerpc_decode_saslauthd_send(TALLOC_CTX *mem_ctx,
						struct tevent_context *ev,
						struct dcerpc_binding_handle *h,
						struct saslauthdRequest _req /* [in]  */);
NTSTATUS dcerpc_decode_saslauthd_recv(struct tevent_req *req,
				      TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_decode_saslauthd(struct dcerpc_binding_handle *h,
				 TALLOC_CTX *mem_ctx,
				 struct saslauthdRequest _req /* [in]  */);

#endif /* _HEADER_RPC_sasl_helpers */
