#include "librpc/rpc/dcerpc.h"
#include "bin/default/librpc/gen_ndr/witness.h"
#ifndef _HEADER_RPC_witness
#define _HEADER_RPC_witness

extern const struct ndr_interface_table ndr_table_witness;

struct tevent_req *dcerpc_witness_GetInterfaceList_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct witness_GetInterfaceList *r);
NTSTATUS dcerpc_witness_GetInterfaceList_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_witness_GetInterfaceList_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct witness_GetInterfaceList *r);
struct tevent_req *dcerpc_witness_GetInterfaceList_send(TALLOC_CTX *mem_ctx,
							struct tevent_context *ev,
							struct dcerpc_binding_handle *h,
							struct witness_interfaceList **_interface_list /* [out] [ref] */);
NTSTATUS dcerpc_witness_GetInterfaceList_recv(struct tevent_req *req,
					      TALLOC_CTX *mem_ctx,
					      WERROR *result);
NTSTATUS dcerpc_witness_GetInterfaceList(struct dcerpc_binding_handle *h,
					 TALLOC_CTX *mem_ctx,
					 struct witness_interfaceList **_interface_list /* [out] [ref] */,
					 WERROR *result);

struct tevent_req *dcerpc_witness_Register_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct witness_Register *r);
NTSTATUS dcerpc_witness_Register_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_witness_Register_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct witness_Register *r);
struct tevent_req *dcerpc_witness_Register_send(TALLOC_CTX *mem_ctx,
						struct tevent_context *ev,
						struct dcerpc_binding_handle *h,
						struct policy_handle *_context_handle /* [out] [ref] */,
						enum witness_version _version /* [in]  */,
						const char *_net_name /* [in] [charset(UTF16),unique] */,
						const char *_ip_address /* [in] [charset(UTF16),unique] */,
						const char *_client_computer_name /* [in] [charset(UTF16),unique] */);
NTSTATUS dcerpc_witness_Register_recv(struct tevent_req *req,
				      TALLOC_CTX *mem_ctx,
				      WERROR *result);
NTSTATUS dcerpc_witness_Register(struct dcerpc_binding_handle *h,
				 TALLOC_CTX *mem_ctx,
				 struct policy_handle *_context_handle /* [out] [ref] */,
				 enum witness_version _version /* [in]  */,
				 const char *_net_name /* [in] [charset(UTF16),unique] */,
				 const char *_ip_address /* [in] [charset(UTF16),unique] */,
				 const char *_client_computer_name /* [in] [charset(UTF16),unique] */,
				 WERROR *result);

struct tevent_req *dcerpc_witness_UnRegister_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct witness_UnRegister *r);
NTSTATUS dcerpc_witness_UnRegister_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_witness_UnRegister_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct witness_UnRegister *r);
struct tevent_req *dcerpc_witness_UnRegister_send(TALLOC_CTX *mem_ctx,
						  struct tevent_context *ev,
						  struct dcerpc_binding_handle *h,
						  struct policy_handle _context_handle /* [in]  */);
NTSTATUS dcerpc_witness_UnRegister_recv(struct tevent_req *req,
					TALLOC_CTX *mem_ctx,
					WERROR *result);
NTSTATUS dcerpc_witness_UnRegister(struct dcerpc_binding_handle *h,
				   TALLOC_CTX *mem_ctx,
				   struct policy_handle _context_handle /* [in]  */,
				   WERROR *result);

struct tevent_req *dcerpc_witness_AsyncNotify_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct witness_AsyncNotify *r);
NTSTATUS dcerpc_witness_AsyncNotify_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_witness_AsyncNotify_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct witness_AsyncNotify *r);
struct tevent_req *dcerpc_witness_AsyncNotify_send(TALLOC_CTX *mem_ctx,
						   struct tevent_context *ev,
						   struct dcerpc_binding_handle *h,
						   struct policy_handle _context_handle /* [in]  */,
						   struct witness_notifyResponse **_response /* [out] [ref] */);
NTSTATUS dcerpc_witness_AsyncNotify_recv(struct tevent_req *req,
					 TALLOC_CTX *mem_ctx,
					 WERROR *result);
NTSTATUS dcerpc_witness_AsyncNotify(struct dcerpc_binding_handle *h,
				    TALLOC_CTX *mem_ctx,
				    struct policy_handle _context_handle /* [in]  */,
				    struct witness_notifyResponse **_response /* [out] [ref] */,
				    WERROR *result);

struct tevent_req *dcerpc_witness_RegisterEx_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct witness_RegisterEx *r);
NTSTATUS dcerpc_witness_RegisterEx_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_witness_RegisterEx_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct witness_RegisterEx *r);
struct tevent_req *dcerpc_witness_RegisterEx_send(TALLOC_CTX *mem_ctx,
						  struct tevent_context *ev,
						  struct dcerpc_binding_handle *h,
						  struct policy_handle *_context_handle /* [out] [ref] */,
						  enum witness_version _version /* [in]  */,
						  const char *_net_name /* [in] [charset(UTF16),unique] */,
						  const char *_share_name /* [in] [charset(UTF16),unique] */,
						  const char *_ip_address /* [in] [charset(UTF16),unique] */,
						  const char *_client_computer_name /* [in] [charset(UTF16),unique] */,
						  uint32_t _flags /* [in]  */,
						  uint32_t _timeout /* [in]  */);
NTSTATUS dcerpc_witness_RegisterEx_recv(struct tevent_req *req,
					TALLOC_CTX *mem_ctx,
					WERROR *result);
NTSTATUS dcerpc_witness_RegisterEx(struct dcerpc_binding_handle *h,
				   TALLOC_CTX *mem_ctx,
				   struct policy_handle *_context_handle /* [out] [ref] */,
				   enum witness_version _version /* [in]  */,
				   const char *_net_name /* [in] [charset(UTF16),unique] */,
				   const char *_share_name /* [in] [charset(UTF16),unique] */,
				   const char *_ip_address /* [in] [charset(UTF16),unique] */,
				   const char *_client_computer_name /* [in] [charset(UTF16),unique] */,
				   uint32_t _flags /* [in]  */,
				   uint32_t _timeout /* [in]  */,
				   WERROR *result);

#endif /* _HEADER_RPC_witness */
