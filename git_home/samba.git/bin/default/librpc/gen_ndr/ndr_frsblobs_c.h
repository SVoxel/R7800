#include "librpc/rpc/dcerpc.h"
#include "bin/default/librpc/gen_ndr/frsblobs.h"
#ifndef _HEADER_RPC_frsblobs
#define _HEADER_RPC_frsblobs

extern const struct ndr_interface_table ndr_table_frsblobs;

struct tevent_req *dcerpc_decode_nt_backup_file_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct decode_nt_backup_file *r);
NTSTATUS dcerpc_decode_nt_backup_file_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_decode_nt_backup_file_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct decode_nt_backup_file *r);
struct tevent_req *dcerpc_decode_nt_backup_file_send(TALLOC_CTX *mem_ctx,
						     struct tevent_context *ev,
						     struct dcerpc_binding_handle *h,
						     struct bkup_NTBackupFile _file /* [in]  */);
NTSTATUS dcerpc_decode_nt_backup_file_recv(struct tevent_req *req,
					   TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_decode_nt_backup_file(struct dcerpc_binding_handle *h,
				      TALLOC_CTX *mem_ctx,
				      struct bkup_NTBackupFile _file /* [in]  */);

struct tevent_req *dcerpc_decode_frsrpc_StageHeader_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct decode_frsrpc_StageHeader *r);
NTSTATUS dcerpc_decode_frsrpc_StageHeader_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_decode_frsrpc_StageHeader_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct decode_frsrpc_StageHeader *r);
struct tevent_req *dcerpc_decode_frsrpc_StageHeader_send(TALLOC_CTX *mem_ctx,
							 struct tevent_context *ev,
							 struct dcerpc_binding_handle *h,
							 struct frsrpc_StageHeader _header /* [in]  */,
							 struct bkup_NTBackupFile _data /* [in]  */);
NTSTATUS dcerpc_decode_frsrpc_StageHeader_recv(struct tevent_req *req,
					       TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_decode_frsrpc_StageHeader(struct dcerpc_binding_handle *h,
					  TALLOC_CTX *mem_ctx,
					  struct frsrpc_StageHeader _header /* [in]  */,
					  struct bkup_NTBackupFile _data /* [in]  */);

struct tevent_req *dcerpc_decode_FileNetworkOpenInformation_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct decode_FileNetworkOpenInformation *r);
NTSTATUS dcerpc_decode_FileNetworkOpenInformation_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_decode_FileNetworkOpenInformation_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct decode_FileNetworkOpenInformation *r);
struct tevent_req *dcerpc_decode_FileNetworkOpenInformation_send(TALLOC_CTX *mem_ctx,
								 struct tevent_context *ev,
								 struct dcerpc_binding_handle *h,
								 struct fscc_FileNetworkOpenInformation _data /* [in]  */);
NTSTATUS dcerpc_decode_FileNetworkOpenInformation_recv(struct tevent_req *req,
						       TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_decode_FileNetworkOpenInformation(struct dcerpc_binding_handle *h,
						  TALLOC_CTX *mem_ctx,
						  struct fscc_FileNetworkOpenInformation _data /* [in]  */);

#endif /* _HEADER_RPC_frsblobs */
