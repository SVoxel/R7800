#include "librpc/rpc/dcerpc.h"
#include "bin/default/librpc/gen_ndr/security.h"
#ifndef _HEADER_RPC_security
#define _HEADER_RPC_security

extern const struct ndr_interface_table ndr_table_security;

struct tevent_req *dcerpc_decode_security_descriptor_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct decode_security_descriptor *r);
NTSTATUS dcerpc_decode_security_descriptor_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_decode_security_descriptor_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct decode_security_descriptor *r);
struct tevent_req *dcerpc_decode_security_descriptor_send(TALLOC_CTX *mem_ctx,
							  struct tevent_context *ev,
							  struct dcerpc_binding_handle *h,
							  struct security_descriptor _sd /* [in]  */);
NTSTATUS dcerpc_decode_security_descriptor_recv(struct tevent_req *req,
						TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_decode_security_descriptor(struct dcerpc_binding_handle *h,
					   TALLOC_CTX *mem_ctx,
					   struct security_descriptor _sd /* [in]  */);

struct tevent_req *dcerpc_decode_sec_desc_buf_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct decode_sec_desc_buf *r);
NTSTATUS dcerpc_decode_sec_desc_buf_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_decode_sec_desc_buf_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct decode_sec_desc_buf *r);
struct tevent_req *dcerpc_decode_sec_desc_buf_send(TALLOC_CTX *mem_ctx,
						   struct tevent_context *ev,
						   struct dcerpc_binding_handle *h,
						   struct sec_desc_buf _sd_buf /* [in]  */);
NTSTATUS dcerpc_decode_sec_desc_buf_recv(struct tevent_req *req,
					 TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_decode_sec_desc_buf(struct dcerpc_binding_handle *h,
				    TALLOC_CTX *mem_ctx,
				    struct sec_desc_buf _sd_buf /* [in]  */);

struct tevent_req *dcerpc_decode_security_token_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct decode_security_token *r);
NTSTATUS dcerpc_decode_security_token_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_decode_security_token_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct decode_security_token *r);
struct tevent_req *dcerpc_decode_security_token_send(TALLOC_CTX *mem_ctx,
						     struct tevent_context *ev,
						     struct dcerpc_binding_handle *h,
						     struct security_token _token /* [in]  */);
NTSTATUS dcerpc_decode_security_token_recv(struct tevent_req *req,
					   TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_decode_security_token(struct dcerpc_binding_handle *h,
				      TALLOC_CTX *mem_ctx,
				      struct security_token _token /* [in]  */);

struct tevent_req *dcerpc_decode_security_unix_token_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct decode_security_unix_token *r);
NTSTATUS dcerpc_decode_security_unix_token_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_decode_security_unix_token_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct decode_security_unix_token *r);
struct tevent_req *dcerpc_decode_security_unix_token_send(TALLOC_CTX *mem_ctx,
							  struct tevent_context *ev,
							  struct dcerpc_binding_handle *h,
							  struct security_unix_token _unix_token /* [in]  */);
NTSTATUS dcerpc_decode_security_unix_token_recv(struct tevent_req *req,
						TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_decode_security_unix_token(struct dcerpc_binding_handle *h,
					   TALLOC_CTX *mem_ctx,
					   struct security_unix_token _unix_token /* [in]  */);

#endif /* _HEADER_RPC_security */
