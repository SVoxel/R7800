#include "librpc/rpc/dcerpc.h"
#include "bin/default/source4/librpc/gen_ndr/winstation.h"
#ifndef _HEADER_RPC_winstation
#define _HEADER_RPC_winstation

extern const struct ndr_interface_table ndr_table_winstation;

struct tevent_req *dcerpc_winstation_foo_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winstation_foo *r);
NTSTATUS dcerpc_winstation_foo_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winstation_foo_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winstation_foo *r);
struct tevent_req *dcerpc_winstation_foo_send(TALLOC_CTX *mem_ctx,
					      struct tevent_context *ev,
					      struct dcerpc_binding_handle *h);
NTSTATUS dcerpc_winstation_foo_recv(struct tevent_req *req,
				    TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winstation_foo(struct dcerpc_binding_handle *h,
			       TALLOC_CTX *mem_ctx);

#endif /* _HEADER_RPC_winstation */
