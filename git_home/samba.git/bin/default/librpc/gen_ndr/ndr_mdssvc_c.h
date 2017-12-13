#include "librpc/rpc/dcerpc.h"
#include "bin/default/librpc/gen_ndr/mdssvc.h"
#ifndef _HEADER_RPC_mdssvc
#define _HEADER_RPC_mdssvc

extern const struct ndr_interface_table ndr_table_mdssvc;

struct tevent_req *dcerpc_mdssvc_open_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct mdssvc_open *r);
NTSTATUS dcerpc_mdssvc_open_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_mdssvc_open_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct mdssvc_open *r);
struct tevent_req *dcerpc_mdssvc_open_send(TALLOC_CTX *mem_ctx,
					   struct tevent_context *ev,
					   struct dcerpc_binding_handle *h,
					   uint32_t *_device_id /* [in,out] [ref] */,
					   uint32_t *_unkn2 /* [in,out] [ref] */,
					   uint32_t *_unkn3 /* [in,out] [ref] */,
					   const char *_share_mount_path /* [in] [charset(UTF8),size_is(1025)] */,
					   const char *_share_name /* [in] [charset(UTF8),size_is(1025)] */,
					   const char *_share_path /* [out] [charset(UTF8),size_is(1025)] */,
					   struct policy_handle *_handle /* [out] [ref] */);
NTSTATUS dcerpc_mdssvc_open_recv(struct tevent_req *req,
				 TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_mdssvc_open(struct dcerpc_binding_handle *h,
			    TALLOC_CTX *mem_ctx,
			    uint32_t *_device_id /* [in,out] [ref] */,
			    uint32_t *_unkn2 /* [in,out] [ref] */,
			    uint32_t *_unkn3 /* [in,out] [ref] */,
			    const char *_share_mount_path /* [in] [charset(UTF8),size_is(1025)] */,
			    const char *_share_name /* [in] [charset(UTF8),size_is(1025)] */,
			    const char *_share_path /* [out] [charset(UTF8),size_is(1025)] */,
			    struct policy_handle *_handle /* [out] [ref] */);

struct tevent_req *dcerpc_mdssvc_unknown1_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct mdssvc_unknown1 *r);
NTSTATUS dcerpc_mdssvc_unknown1_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_mdssvc_unknown1_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct mdssvc_unknown1 *r);
struct tevent_req *dcerpc_mdssvc_unknown1_send(TALLOC_CTX *mem_ctx,
					       struct tevent_context *ev,
					       struct dcerpc_binding_handle *h,
					       struct policy_handle _handle /* [in]  */,
					       uint32_t _unkn1 /* [in]  */,
					       uint32_t _device_id /* [in]  */,
					       uint32_t _unkn3 /* [in]  */,
					       uint32_t _unkn4 /* [in]  */,
					       uint32_t _uid /* [in]  */,
					       uint32_t _gid /* [in]  */,
					       uint32_t *_status /* [out] [ref] */,
					       uint32_t *_flags /* [out] [ref] */,
					       uint32_t *_unkn7 /* [out] [ref] */);
NTSTATUS dcerpc_mdssvc_unknown1_recv(struct tevent_req *req,
				     TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_mdssvc_unknown1(struct dcerpc_binding_handle *h,
				TALLOC_CTX *mem_ctx,
				struct policy_handle _handle /* [in]  */,
				uint32_t _unkn1 /* [in]  */,
				uint32_t _device_id /* [in]  */,
				uint32_t _unkn3 /* [in]  */,
				uint32_t _unkn4 /* [in]  */,
				uint32_t _uid /* [in]  */,
				uint32_t _gid /* [in]  */,
				uint32_t *_status /* [out] [ref] */,
				uint32_t *_flags /* [out] [ref] */,
				uint32_t *_unkn7 /* [out] [ref] */);

struct tevent_req *dcerpc_mdssvc_cmd_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct mdssvc_cmd *r);
NTSTATUS dcerpc_mdssvc_cmd_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_mdssvc_cmd_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct mdssvc_cmd *r);
struct tevent_req *dcerpc_mdssvc_cmd_send(TALLOC_CTX *mem_ctx,
					  struct tevent_context *ev,
					  struct dcerpc_binding_handle *h,
					  struct policy_handle _handle /* [in]  */,
					  uint32_t _unkn1 /* [in]  */,
					  uint32_t _device_id /* [in]  */,
					  uint32_t _unkn3 /* [in]  */,
					  uint32_t _unkn4 /* [in]  */,
					  uint32_t _flags /* [in]  */,
					  struct mdssvc_blob _request_blob /* [in]  */,
					  uint32_t _unkn5 /* [in]  */,
					  uint32_t _max_fragment_size1 /* [in]  */,
					  uint32_t _unkn6 /* [in]  */,
					  uint32_t _max_fragment_size2 /* [in]  */,
					  uint32_t _unkn7 /* [in]  */,
					  uint32_t _unkn8 /* [in]  */,
					  uint32_t *_status /* [out] [ref] */,
					  struct mdssvc_blob *_response_blob /* [out] [ref] */,
					  uint32_t *_unkn9 /* [out] [ref] */);
NTSTATUS dcerpc_mdssvc_cmd_recv(struct tevent_req *req,
				TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_mdssvc_cmd(struct dcerpc_binding_handle *h,
			   TALLOC_CTX *mem_ctx,
			   struct policy_handle _handle /* [in]  */,
			   uint32_t _unkn1 /* [in]  */,
			   uint32_t _device_id /* [in]  */,
			   uint32_t _unkn3 /* [in]  */,
			   uint32_t _unkn4 /* [in]  */,
			   uint32_t _flags /* [in]  */,
			   struct mdssvc_blob _request_blob /* [in]  */,
			   uint32_t _unkn5 /* [in]  */,
			   uint32_t _max_fragment_size1 /* [in]  */,
			   uint32_t _unkn6 /* [in]  */,
			   uint32_t _max_fragment_size2 /* [in]  */,
			   uint32_t _unkn7 /* [in]  */,
			   uint32_t _unkn8 /* [in]  */,
			   uint32_t *_status /* [out] [ref] */,
			   struct mdssvc_blob *_response_blob /* [out] [ref] */,
			   uint32_t *_unkn9 /* [out] [ref] */);

struct tevent_req *dcerpc_mdssvc_close_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct mdssvc_close *r);
NTSTATUS dcerpc_mdssvc_close_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_mdssvc_close_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct mdssvc_close *r);
struct tevent_req *dcerpc_mdssvc_close_send(TALLOC_CTX *mem_ctx,
					    struct tevent_context *ev,
					    struct dcerpc_binding_handle *h,
					    struct policy_handle _in_handle /* [in]  */,
					    uint32_t _unkn1 /* [in]  */,
					    uint32_t _device_id /* [in]  */,
					    uint32_t _unkn2 /* [in]  */,
					    uint32_t _unkn3 /* [in]  */,
					    struct policy_handle *_out_handle /* [out] [ref] */,
					    uint32_t *_status /* [out] [ref] */);
NTSTATUS dcerpc_mdssvc_close_recv(struct tevent_req *req,
				  TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_mdssvc_close(struct dcerpc_binding_handle *h,
			     TALLOC_CTX *mem_ctx,
			     struct policy_handle _in_handle /* [in]  */,
			     uint32_t _unkn1 /* [in]  */,
			     uint32_t _device_id /* [in]  */,
			     uint32_t _unkn2 /* [in]  */,
			     uint32_t _unkn3 /* [in]  */,
			     struct policy_handle *_out_handle /* [out] [ref] */,
			     uint32_t *_status /* [out] [ref] */);

#endif /* _HEADER_RPC_mdssvc */
