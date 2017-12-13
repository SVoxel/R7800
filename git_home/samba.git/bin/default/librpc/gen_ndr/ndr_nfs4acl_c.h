#include "librpc/rpc/dcerpc.h"
#include "bin/default/librpc/gen_ndr/nfs4acl.h"
#ifndef _HEADER_RPC_nfs4acl_interface
#define _HEADER_RPC_nfs4acl_interface

extern const struct ndr_interface_table ndr_table_nfs4acl_interface;

struct tevent_req *dcerpc_nfs4acl_test_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct nfs4acl_test *r);
NTSTATUS dcerpc_nfs4acl_test_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_nfs4acl_test_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct nfs4acl_test *r);
struct tevent_req *dcerpc_nfs4acl_test_send(TALLOC_CTX *mem_ctx,
					    struct tevent_context *ev,
					    struct dcerpc_binding_handle *h,
					    struct nfs4acl _acl /* [in]  */);
NTSTATUS dcerpc_nfs4acl_test_recv(struct tevent_req *req,
				  TALLOC_CTX *mem_ctx,
				  NTSTATUS *result);
NTSTATUS dcerpc_nfs4acl_test(struct dcerpc_binding_handle *h,
			     TALLOC_CTX *mem_ctx,
			     struct nfs4acl _acl /* [in]  */,
			     NTSTATUS *result);

#endif /* _HEADER_RPC_nfs4acl_interface */
