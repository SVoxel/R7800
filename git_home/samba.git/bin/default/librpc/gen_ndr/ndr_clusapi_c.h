#include "librpc/rpc/dcerpc.h"
#include "bin/default/librpc/gen_ndr/clusapi.h"
#ifndef _HEADER_RPC_clusapi
#define _HEADER_RPC_clusapi

extern const struct ndr_interface_table ndr_table_clusapi;

struct tevent_req *dcerpc_clusapi_OpenCluster_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_OpenCluster *r);
NTSTATUS dcerpc_clusapi_OpenCluster_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_OpenCluster_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_OpenCluster *r);
struct tevent_req *dcerpc_clusapi_OpenCluster_send(TALLOC_CTX *mem_ctx,
						   struct tevent_context *ev,
						   struct dcerpc_binding_handle *h,
						   WERROR *_Status /* [out] [ref] */,
						   struct policy_handle *_Cluster /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_OpenCluster_recv(struct tevent_req *req,
					 TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_OpenCluster(struct dcerpc_binding_handle *h,
				    TALLOC_CTX *mem_ctx,
				    WERROR *_Status /* [out] [ref] */,
				    struct policy_handle *_Cluster /* [out] [ref] */);

struct tevent_req *dcerpc_clusapi_CloseCluster_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_CloseCluster *r);
NTSTATUS dcerpc_clusapi_CloseCluster_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_CloseCluster_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_CloseCluster *r);
struct tevent_req *dcerpc_clusapi_CloseCluster_send(TALLOC_CTX *mem_ctx,
						    struct tevent_context *ev,
						    struct dcerpc_binding_handle *h,
						    struct policy_handle *_Cluster /* [in,out] [ref] */);
NTSTATUS dcerpc_clusapi_CloseCluster_recv(struct tevent_req *req,
					  TALLOC_CTX *mem_ctx,
					  WERROR *result);
NTSTATUS dcerpc_clusapi_CloseCluster(struct dcerpc_binding_handle *h,
				     TALLOC_CTX *mem_ctx,
				     struct policy_handle *_Cluster /* [in,out] [ref] */,
				     WERROR *result);

struct tevent_req *dcerpc_clusapi_SetClusterName_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_SetClusterName *r);
NTSTATUS dcerpc_clusapi_SetClusterName_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_SetClusterName_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_SetClusterName *r);
struct tevent_req *dcerpc_clusapi_SetClusterName_send(TALLOC_CTX *mem_ctx,
						      struct tevent_context *ev,
						      struct dcerpc_binding_handle *h,
						      const char *_NewClusterName /* [in] [charset(UTF16),ref] */,
						      WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_SetClusterName_recv(struct tevent_req *req,
					    TALLOC_CTX *mem_ctx,
					    WERROR *result);
NTSTATUS dcerpc_clusapi_SetClusterName(struct dcerpc_binding_handle *h,
				       TALLOC_CTX *mem_ctx,
				       const char *_NewClusterName /* [in] [charset(UTF16),ref] */,
				       WERROR *_rpc_status /* [out] [ref] */,
				       WERROR *result);

struct tevent_req *dcerpc_clusapi_GetClusterName_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_GetClusterName *r);
NTSTATUS dcerpc_clusapi_GetClusterName_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_GetClusterName_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_GetClusterName *r);
struct tevent_req *dcerpc_clusapi_GetClusterName_send(TALLOC_CTX *mem_ctx,
						      struct tevent_context *ev,
						      struct dcerpc_binding_handle *h,
						      const char **_ClusterName /* [out] [charset(UTF16),ref] */,
						      const char **_NodeName /* [out] [charset(UTF16),ref] */);
NTSTATUS dcerpc_clusapi_GetClusterName_recv(struct tevent_req *req,
					    TALLOC_CTX *mem_ctx,
					    WERROR *result);
NTSTATUS dcerpc_clusapi_GetClusterName(struct dcerpc_binding_handle *h,
				       TALLOC_CTX *mem_ctx,
				       const char **_ClusterName /* [out] [charset(UTF16),ref] */,
				       const char **_NodeName /* [out] [charset(UTF16),ref] */,
				       WERROR *result);

struct tevent_req *dcerpc_clusapi_GetClusterVersion_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_GetClusterVersion *r);
NTSTATUS dcerpc_clusapi_GetClusterVersion_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_GetClusterVersion_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_GetClusterVersion *r);
struct tevent_req *dcerpc_clusapi_GetClusterVersion_send(TALLOC_CTX *mem_ctx,
							 struct tevent_context *ev,
							 struct dcerpc_binding_handle *h,
							 uint16_t *_lpwMajorVersion /* [out] [ref] */,
							 uint16_t *_lpwMinorVersion /* [out] [ref] */,
							 uint16_t *_lpwBuildNumber /* [out] [ref] */,
							 const char **_lpszVendorId /* [out] [charset(UTF16),ref] */,
							 const char **_lpszCSDVersion /* [out] [charset(UTF16),ref] */);
NTSTATUS dcerpc_clusapi_GetClusterVersion_recv(struct tevent_req *req,
					       TALLOC_CTX *mem_ctx,
					       WERROR *result);
NTSTATUS dcerpc_clusapi_GetClusterVersion(struct dcerpc_binding_handle *h,
					  TALLOC_CTX *mem_ctx,
					  uint16_t *_lpwMajorVersion /* [out] [ref] */,
					  uint16_t *_lpwMinorVersion /* [out] [ref] */,
					  uint16_t *_lpwBuildNumber /* [out] [ref] */,
					  const char **_lpszVendorId /* [out] [charset(UTF16),ref] */,
					  const char **_lpszCSDVersion /* [out] [charset(UTF16),ref] */,
					  WERROR *result);

struct tevent_req *dcerpc_clusapi_GetQuorumResource_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_GetQuorumResource *r);
NTSTATUS dcerpc_clusapi_GetQuorumResource_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_GetQuorumResource_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_GetQuorumResource *r);
struct tevent_req *dcerpc_clusapi_GetQuorumResource_send(TALLOC_CTX *mem_ctx,
							 struct tevent_context *ev,
							 struct dcerpc_binding_handle *h,
							 const char **_lpszResourceName /* [out] [charset(UTF16),ref] */,
							 const char **_lpszDeviceName /* [out] [charset(UTF16),ref] */,
							 uint32_t *_pdwMaxQuorumLogSize /* [out] [ref] */,
							 WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_GetQuorumResource_recv(struct tevent_req *req,
					       TALLOC_CTX *mem_ctx,
					       WERROR *result);
NTSTATUS dcerpc_clusapi_GetQuorumResource(struct dcerpc_binding_handle *h,
					  TALLOC_CTX *mem_ctx,
					  const char **_lpszResourceName /* [out] [charset(UTF16),ref] */,
					  const char **_lpszDeviceName /* [out] [charset(UTF16),ref] */,
					  uint32_t *_pdwMaxQuorumLogSize /* [out] [ref] */,
					  WERROR *_rpc_status /* [out] [ref] */,
					  WERROR *result);

struct tevent_req *dcerpc_clusapi_SetQuorumResource_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_SetQuorumResource *r);
NTSTATUS dcerpc_clusapi_SetQuorumResource_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_SetQuorumResource_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_SetQuorumResource *r);
struct tevent_req *dcerpc_clusapi_SetQuorumResource_send(TALLOC_CTX *mem_ctx,
							 struct tevent_context *ev,
							 struct dcerpc_binding_handle *h,
							 struct policy_handle _hResource /* [in]  */,
							 const char *_lpszDeviceName /* [in] [charset(UTF16),ref] */,
							 uint32_t _dwMaxQuorumLogSize /* [in]  */,
							 WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_SetQuorumResource_recv(struct tevent_req *req,
					       TALLOC_CTX *mem_ctx,
					       WERROR *result);
NTSTATUS dcerpc_clusapi_SetQuorumResource(struct dcerpc_binding_handle *h,
					  TALLOC_CTX *mem_ctx,
					  struct policy_handle _hResource /* [in]  */,
					  const char *_lpszDeviceName /* [in] [charset(UTF16),ref] */,
					  uint32_t _dwMaxQuorumLogSize /* [in]  */,
					  WERROR *_rpc_status /* [out] [ref] */,
					  WERROR *result);

struct tevent_req *dcerpc_clusapi_CreateEnum_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_CreateEnum *r);
NTSTATUS dcerpc_clusapi_CreateEnum_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_CreateEnum_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_CreateEnum *r);
struct tevent_req *dcerpc_clusapi_CreateEnum_send(TALLOC_CTX *mem_ctx,
						  struct tevent_context *ev,
						  struct dcerpc_binding_handle *h,
						  uint32_t _dwType /* [in]  */,
						  struct ENUM_LIST **_ReturnEnum /* [out] [ref] */,
						  WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_CreateEnum_recv(struct tevent_req *req,
					TALLOC_CTX *mem_ctx,
					WERROR *result);
NTSTATUS dcerpc_clusapi_CreateEnum(struct dcerpc_binding_handle *h,
				   TALLOC_CTX *mem_ctx,
				   uint32_t _dwType /* [in]  */,
				   struct ENUM_LIST **_ReturnEnum /* [out] [ref] */,
				   WERROR *_rpc_status /* [out] [ref] */,
				   WERROR *result);

struct tevent_req *dcerpc_clusapi_OpenResource_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_OpenResource *r);
NTSTATUS dcerpc_clusapi_OpenResource_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_OpenResource_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_OpenResource *r);
struct tevent_req *dcerpc_clusapi_OpenResource_send(TALLOC_CTX *mem_ctx,
						    struct tevent_context *ev,
						    struct dcerpc_binding_handle *h,
						    const char *_lpszResourceName /* [in] [charset(UTF16),ref] */,
						    WERROR *_Status /* [out] [ref] */,
						    WERROR *_rpc_status /* [out] [ref] */,
						    struct policy_handle *_hResource /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_OpenResource_recv(struct tevent_req *req,
					  TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_OpenResource(struct dcerpc_binding_handle *h,
				     TALLOC_CTX *mem_ctx,
				     const char *_lpszResourceName /* [in] [charset(UTF16),ref] */,
				     WERROR *_Status /* [out] [ref] */,
				     WERROR *_rpc_status /* [out] [ref] */,
				     struct policy_handle *_hResource /* [out] [ref] */);

struct tevent_req *dcerpc_clusapi_CreateResource_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_CreateResource *r);
NTSTATUS dcerpc_clusapi_CreateResource_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_CreateResource_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_CreateResource *r);
struct tevent_req *dcerpc_clusapi_CreateResource_send(TALLOC_CTX *mem_ctx,
						      struct tevent_context *ev,
						      struct dcerpc_binding_handle *h,
						      struct policy_handle _hGroup /* [in]  */,
						      const char *_lpszResourceName /* [in] [charset(UTF16),ref] */,
						      const char *_lpszResourceType /* [in] [charset(UTF16),ref] */,
						      enum clusapi_CreateResourceFlags _dwFlags /* [in]  */,
						      WERROR *_Status /* [out] [ref] */,
						      WERROR *_rpc_status /* [out] [ref] */,
						      struct policy_handle *_hResource /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_CreateResource_recv(struct tevent_req *req,
					    TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_CreateResource(struct dcerpc_binding_handle *h,
				       TALLOC_CTX *mem_ctx,
				       struct policy_handle _hGroup /* [in]  */,
				       const char *_lpszResourceName /* [in] [charset(UTF16),ref] */,
				       const char *_lpszResourceType /* [in] [charset(UTF16),ref] */,
				       enum clusapi_CreateResourceFlags _dwFlags /* [in]  */,
				       WERROR *_Status /* [out] [ref] */,
				       WERROR *_rpc_status /* [out] [ref] */,
				       struct policy_handle *_hResource /* [out] [ref] */);

struct tevent_req *dcerpc_clusapi_DeleteResource_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_DeleteResource *r);
NTSTATUS dcerpc_clusapi_DeleteResource_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_DeleteResource_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_DeleteResource *r);
struct tevent_req *dcerpc_clusapi_DeleteResource_send(TALLOC_CTX *mem_ctx,
						      struct tevent_context *ev,
						      struct dcerpc_binding_handle *h,
						      struct policy_handle _hResource /* [in]  */,
						      WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_DeleteResource_recv(struct tevent_req *req,
					    TALLOC_CTX *mem_ctx,
					    WERROR *result);
NTSTATUS dcerpc_clusapi_DeleteResource(struct dcerpc_binding_handle *h,
				       TALLOC_CTX *mem_ctx,
				       struct policy_handle _hResource /* [in]  */,
				       WERROR *_rpc_status /* [out] [ref] */,
				       WERROR *result);

struct tevent_req *dcerpc_clusapi_CloseResource_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_CloseResource *r);
NTSTATUS dcerpc_clusapi_CloseResource_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_CloseResource_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_CloseResource *r);
struct tevent_req *dcerpc_clusapi_CloseResource_send(TALLOC_CTX *mem_ctx,
						     struct tevent_context *ev,
						     struct dcerpc_binding_handle *h,
						     struct policy_handle *_Resource /* [in,out] [ref] */);
NTSTATUS dcerpc_clusapi_CloseResource_recv(struct tevent_req *req,
					   TALLOC_CTX *mem_ctx,
					   WERROR *result);
NTSTATUS dcerpc_clusapi_CloseResource(struct dcerpc_binding_handle *h,
				      TALLOC_CTX *mem_ctx,
				      struct policy_handle *_Resource /* [in,out] [ref] */,
				      WERROR *result);

struct tevent_req *dcerpc_clusapi_GetResourceState_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_GetResourceState *r);
NTSTATUS dcerpc_clusapi_GetResourceState_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_GetResourceState_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_GetResourceState *r);
struct tevent_req *dcerpc_clusapi_GetResourceState_send(TALLOC_CTX *mem_ctx,
							struct tevent_context *ev,
							struct dcerpc_binding_handle *h,
							struct policy_handle _hResource /* [in]  */,
							enum clusapi_ClusterResourceState *_State /* [out] [ref] */,
							const char **_NodeName /* [out] [charset(UTF16),ref] */,
							const char **_GroupName /* [out] [charset(UTF16),ref] */,
							WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_GetResourceState_recv(struct tevent_req *req,
					      TALLOC_CTX *mem_ctx,
					      WERROR *result);
NTSTATUS dcerpc_clusapi_GetResourceState(struct dcerpc_binding_handle *h,
					 TALLOC_CTX *mem_ctx,
					 struct policy_handle _hResource /* [in]  */,
					 enum clusapi_ClusterResourceState *_State /* [out] [ref] */,
					 const char **_NodeName /* [out] [charset(UTF16),ref] */,
					 const char **_GroupName /* [out] [charset(UTF16),ref] */,
					 WERROR *_rpc_status /* [out] [ref] */,
					 WERROR *result);

struct tevent_req *dcerpc_clusapi_SetResourceName_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_SetResourceName *r);
NTSTATUS dcerpc_clusapi_SetResourceName_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_SetResourceName_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_SetResourceName *r);
struct tevent_req *dcerpc_clusapi_SetResourceName_send(TALLOC_CTX *mem_ctx,
						       struct tevent_context *ev,
						       struct dcerpc_binding_handle *h,
						       struct policy_handle _hResource /* [in]  */,
						       const char *_lpszResourceName /* [in] [charset(UTF16),ref] */,
						       WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_SetResourceName_recv(struct tevent_req *req,
					     TALLOC_CTX *mem_ctx,
					     WERROR *result);
NTSTATUS dcerpc_clusapi_SetResourceName(struct dcerpc_binding_handle *h,
					TALLOC_CTX *mem_ctx,
					struct policy_handle _hResource /* [in]  */,
					const char *_lpszResourceName /* [in] [charset(UTF16),ref] */,
					WERROR *_rpc_status /* [out] [ref] */,
					WERROR *result);

struct tevent_req *dcerpc_clusapi_GetResourceId_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_GetResourceId *r);
NTSTATUS dcerpc_clusapi_GetResourceId_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_GetResourceId_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_GetResourceId *r);
struct tevent_req *dcerpc_clusapi_GetResourceId_send(TALLOC_CTX *mem_ctx,
						     struct tevent_context *ev,
						     struct dcerpc_binding_handle *h,
						     struct policy_handle _hResource /* [in]  */,
						     const char **_pGuid /* [out] [charset(UTF16),ref] */,
						     WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_GetResourceId_recv(struct tevent_req *req,
					   TALLOC_CTX *mem_ctx,
					   WERROR *result);
NTSTATUS dcerpc_clusapi_GetResourceId(struct dcerpc_binding_handle *h,
				      TALLOC_CTX *mem_ctx,
				      struct policy_handle _hResource /* [in]  */,
				      const char **_pGuid /* [out] [charset(UTF16),ref] */,
				      WERROR *_rpc_status /* [out] [ref] */,
				      WERROR *result);

struct tevent_req *dcerpc_clusapi_GetResourceType_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_GetResourceType *r);
NTSTATUS dcerpc_clusapi_GetResourceType_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_GetResourceType_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_GetResourceType *r);
struct tevent_req *dcerpc_clusapi_GetResourceType_send(TALLOC_CTX *mem_ctx,
						       struct tevent_context *ev,
						       struct dcerpc_binding_handle *h,
						       struct policy_handle _hResource /* [in]  */,
						       const char **_lpszResourceType /* [out] [charset(UTF16),ref] */,
						       WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_GetResourceType_recv(struct tevent_req *req,
					     TALLOC_CTX *mem_ctx,
					     WERROR *result);
NTSTATUS dcerpc_clusapi_GetResourceType(struct dcerpc_binding_handle *h,
					TALLOC_CTX *mem_ctx,
					struct policy_handle _hResource /* [in]  */,
					const char **_lpszResourceType /* [out] [charset(UTF16),ref] */,
					WERROR *_rpc_status /* [out] [ref] */,
					WERROR *result);

struct tevent_req *dcerpc_clusapi_FailResource_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_FailResource *r);
NTSTATUS dcerpc_clusapi_FailResource_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_FailResource_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_FailResource *r);
struct tevent_req *dcerpc_clusapi_FailResource_send(TALLOC_CTX *mem_ctx,
						    struct tevent_context *ev,
						    struct dcerpc_binding_handle *h,
						    struct policy_handle _hResource /* [in]  */,
						    WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_FailResource_recv(struct tevent_req *req,
					  TALLOC_CTX *mem_ctx,
					  WERROR *result);
NTSTATUS dcerpc_clusapi_FailResource(struct dcerpc_binding_handle *h,
				     TALLOC_CTX *mem_ctx,
				     struct policy_handle _hResource /* [in]  */,
				     WERROR *_rpc_status /* [out] [ref] */,
				     WERROR *result);

struct tevent_req *dcerpc_clusapi_OnlineResource_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_OnlineResource *r);
NTSTATUS dcerpc_clusapi_OnlineResource_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_OnlineResource_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_OnlineResource *r);
struct tevent_req *dcerpc_clusapi_OnlineResource_send(TALLOC_CTX *mem_ctx,
						      struct tevent_context *ev,
						      struct dcerpc_binding_handle *h,
						      struct policy_handle _hResource /* [in]  */,
						      WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_OnlineResource_recv(struct tevent_req *req,
					    TALLOC_CTX *mem_ctx,
					    WERROR *result);
NTSTATUS dcerpc_clusapi_OnlineResource(struct dcerpc_binding_handle *h,
				       TALLOC_CTX *mem_ctx,
				       struct policy_handle _hResource /* [in]  */,
				       WERROR *_rpc_status /* [out] [ref] */,
				       WERROR *result);

struct tevent_req *dcerpc_clusapi_OfflineResource_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_OfflineResource *r);
NTSTATUS dcerpc_clusapi_OfflineResource_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_OfflineResource_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_OfflineResource *r);
struct tevent_req *dcerpc_clusapi_OfflineResource_send(TALLOC_CTX *mem_ctx,
						       struct tevent_context *ev,
						       struct dcerpc_binding_handle *h,
						       struct policy_handle _hResource /* [in]  */,
						       WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_OfflineResource_recv(struct tevent_req *req,
					     TALLOC_CTX *mem_ctx,
					     WERROR *result);
NTSTATUS dcerpc_clusapi_OfflineResource(struct dcerpc_binding_handle *h,
					TALLOC_CTX *mem_ctx,
					struct policy_handle _hResource /* [in]  */,
					WERROR *_rpc_status /* [out] [ref] */,
					WERROR *result);

struct tevent_req *dcerpc_clusapi_AddResourceDependency_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_AddResourceDependency *r);
NTSTATUS dcerpc_clusapi_AddResourceDependency_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_AddResourceDependency_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_AddResourceDependency *r);
struct tevent_req *dcerpc_clusapi_AddResourceDependency_send(TALLOC_CTX *mem_ctx,
							     struct tevent_context *ev,
							     struct dcerpc_binding_handle *h,
							     struct policy_handle _hResource /* [in]  */,
							     struct policy_handle _hDependsOn /* [in]  */,
							     WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_AddResourceDependency_recv(struct tevent_req *req,
						   TALLOC_CTX *mem_ctx,
						   WERROR *result);
NTSTATUS dcerpc_clusapi_AddResourceDependency(struct dcerpc_binding_handle *h,
					      TALLOC_CTX *mem_ctx,
					      struct policy_handle _hResource /* [in]  */,
					      struct policy_handle _hDependsOn /* [in]  */,
					      WERROR *_rpc_status /* [out] [ref] */,
					      WERROR *result);

struct tevent_req *dcerpc_clusapi_RemoveResourceDependency_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_RemoveResourceDependency *r);
NTSTATUS dcerpc_clusapi_RemoveResourceDependency_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_RemoveResourceDependency_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_RemoveResourceDependency *r);
struct tevent_req *dcerpc_clusapi_RemoveResourceDependency_send(TALLOC_CTX *mem_ctx,
								struct tevent_context *ev,
								struct dcerpc_binding_handle *h,
								struct policy_handle _hResource /* [in]  */,
								struct policy_handle _hDependsOn /* [in]  */,
								WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_RemoveResourceDependency_recv(struct tevent_req *req,
						      TALLOC_CTX *mem_ctx,
						      WERROR *result);
NTSTATUS dcerpc_clusapi_RemoveResourceDependency(struct dcerpc_binding_handle *h,
						 TALLOC_CTX *mem_ctx,
						 struct policy_handle _hResource /* [in]  */,
						 struct policy_handle _hDependsOn /* [in]  */,
						 WERROR *_rpc_status /* [out] [ref] */,
						 WERROR *result);

struct tevent_req *dcerpc_clusapi_CanResourceBeDependent_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_CanResourceBeDependent *r);
NTSTATUS dcerpc_clusapi_CanResourceBeDependent_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_CanResourceBeDependent_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_CanResourceBeDependent *r);
struct tevent_req *dcerpc_clusapi_CanResourceBeDependent_send(TALLOC_CTX *mem_ctx,
							      struct tevent_context *ev,
							      struct dcerpc_binding_handle *h,
							      struct policy_handle _hResource /* [in]  */,
							      struct policy_handle _hResourceDependent /* [in]  */,
							      WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_CanResourceBeDependent_recv(struct tevent_req *req,
						    TALLOC_CTX *mem_ctx,
						    WERROR *result);
NTSTATUS dcerpc_clusapi_CanResourceBeDependent(struct dcerpc_binding_handle *h,
					       TALLOC_CTX *mem_ctx,
					       struct policy_handle _hResource /* [in]  */,
					       struct policy_handle _hResourceDependent /* [in]  */,
					       WERROR *_rpc_status /* [out] [ref] */,
					       WERROR *result);

struct tevent_req *dcerpc_clusapi_CreateResEnum_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_CreateResEnum *r);
NTSTATUS dcerpc_clusapi_CreateResEnum_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_CreateResEnum_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_CreateResEnum *r);
struct tevent_req *dcerpc_clusapi_CreateResEnum_send(TALLOC_CTX *mem_ctx,
						     struct tevent_context *ev,
						     struct dcerpc_binding_handle *h,
						     struct policy_handle _hResource /* [in]  */,
						     uint32_t _dwType /* [in]  */,
						     struct ENUM_LIST **_ReturnEnum /* [out] [ref] */,
						     WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_CreateResEnum_recv(struct tevent_req *req,
					   TALLOC_CTX *mem_ctx,
					   WERROR *result);
NTSTATUS dcerpc_clusapi_CreateResEnum(struct dcerpc_binding_handle *h,
				      TALLOC_CTX *mem_ctx,
				      struct policy_handle _hResource /* [in]  */,
				      uint32_t _dwType /* [in]  */,
				      struct ENUM_LIST **_ReturnEnum /* [out] [ref] */,
				      WERROR *_rpc_status /* [out] [ref] */,
				      WERROR *result);

struct tevent_req *dcerpc_clusapi_AddResourceNode_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_AddResourceNode *r);
NTSTATUS dcerpc_clusapi_AddResourceNode_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_AddResourceNode_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_AddResourceNode *r);
struct tevent_req *dcerpc_clusapi_AddResourceNode_send(TALLOC_CTX *mem_ctx,
						       struct tevent_context *ev,
						       struct dcerpc_binding_handle *h,
						       struct policy_handle _hResource /* [in]  */,
						       struct policy_handle _hNode /* [in]  */,
						       WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_AddResourceNode_recv(struct tevent_req *req,
					     TALLOC_CTX *mem_ctx,
					     WERROR *result);
NTSTATUS dcerpc_clusapi_AddResourceNode(struct dcerpc_binding_handle *h,
					TALLOC_CTX *mem_ctx,
					struct policy_handle _hResource /* [in]  */,
					struct policy_handle _hNode /* [in]  */,
					WERROR *_rpc_status /* [out] [ref] */,
					WERROR *result);

struct tevent_req *dcerpc_clusapi_RemoveResourceNode_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_RemoveResourceNode *r);
NTSTATUS dcerpc_clusapi_RemoveResourceNode_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_RemoveResourceNode_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_RemoveResourceNode *r);
struct tevent_req *dcerpc_clusapi_RemoveResourceNode_send(TALLOC_CTX *mem_ctx,
							  struct tevent_context *ev,
							  struct dcerpc_binding_handle *h,
							  struct policy_handle _hResource /* [in]  */,
							  struct policy_handle _hNode /* [in]  */,
							  WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_RemoveResourceNode_recv(struct tevent_req *req,
						TALLOC_CTX *mem_ctx,
						WERROR *result);
NTSTATUS dcerpc_clusapi_RemoveResourceNode(struct dcerpc_binding_handle *h,
					   TALLOC_CTX *mem_ctx,
					   struct policy_handle _hResource /* [in]  */,
					   struct policy_handle _hNode /* [in]  */,
					   WERROR *_rpc_status /* [out] [ref] */,
					   WERROR *result);

struct tevent_req *dcerpc_clusapi_ChangeResourceGroup_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_ChangeResourceGroup *r);
NTSTATUS dcerpc_clusapi_ChangeResourceGroup_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_ChangeResourceGroup_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_ChangeResourceGroup *r);
struct tevent_req *dcerpc_clusapi_ChangeResourceGroup_send(TALLOC_CTX *mem_ctx,
							   struct tevent_context *ev,
							   struct dcerpc_binding_handle *h,
							   struct policy_handle _hResource /* [in]  */,
							   struct policy_handle _hGroup /* [in]  */,
							   WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_ChangeResourceGroup_recv(struct tevent_req *req,
						 TALLOC_CTX *mem_ctx,
						 WERROR *result);
NTSTATUS dcerpc_clusapi_ChangeResourceGroup(struct dcerpc_binding_handle *h,
					    TALLOC_CTX *mem_ctx,
					    struct policy_handle _hResource /* [in]  */,
					    struct policy_handle _hGroup /* [in]  */,
					    WERROR *_rpc_status /* [out] [ref] */,
					    WERROR *result);

struct tevent_req *dcerpc_clusapi_CreateResourceType_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_CreateResourceType *r);
NTSTATUS dcerpc_clusapi_CreateResourceType_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_CreateResourceType_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_CreateResourceType *r);
struct tevent_req *dcerpc_clusapi_CreateResourceType_send(TALLOC_CTX *mem_ctx,
							  struct tevent_context *ev,
							  struct dcerpc_binding_handle *h,
							  const char *_lpszTypeName /* [in] [charset(UTF16),ref] */,
							  const char *_lpszDisplayName /* [in] [charset(UTF16),ref] */,
							  const char *_lpszDllName /* [in] [charset(UTF16),ref] */,
							  uint32_t _dwLooksAlive /* [in]  */,
							  uint32_t _dwIsAlive /* [in]  */,
							  WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_CreateResourceType_recv(struct tevent_req *req,
						TALLOC_CTX *mem_ctx,
						WERROR *result);
NTSTATUS dcerpc_clusapi_CreateResourceType(struct dcerpc_binding_handle *h,
					   TALLOC_CTX *mem_ctx,
					   const char *_lpszTypeName /* [in] [charset(UTF16),ref] */,
					   const char *_lpszDisplayName /* [in] [charset(UTF16),ref] */,
					   const char *_lpszDllName /* [in] [charset(UTF16),ref] */,
					   uint32_t _dwLooksAlive /* [in]  */,
					   uint32_t _dwIsAlive /* [in]  */,
					   WERROR *_rpc_status /* [out] [ref] */,
					   WERROR *result);

struct tevent_req *dcerpc_clusapi_DeleteResourceType_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_DeleteResourceType *r);
NTSTATUS dcerpc_clusapi_DeleteResourceType_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_DeleteResourceType_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_DeleteResourceType *r);
struct tevent_req *dcerpc_clusapi_DeleteResourceType_send(TALLOC_CTX *mem_ctx,
							  struct tevent_context *ev,
							  struct dcerpc_binding_handle *h,
							  const char *_lpszTypeName /* [in] [charset(UTF16),ref] */,
							  WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_DeleteResourceType_recv(struct tevent_req *req,
						TALLOC_CTX *mem_ctx,
						WERROR *result);
NTSTATUS dcerpc_clusapi_DeleteResourceType(struct dcerpc_binding_handle *h,
					   TALLOC_CTX *mem_ctx,
					   const char *_lpszTypeName /* [in] [charset(UTF16),ref] */,
					   WERROR *_rpc_status /* [out] [ref] */,
					   WERROR *result);

struct tevent_req *dcerpc_clusapi_GetRootKey_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_GetRootKey *r);
NTSTATUS dcerpc_clusapi_GetRootKey_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_GetRootKey_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_GetRootKey *r);
struct tevent_req *dcerpc_clusapi_GetRootKey_send(TALLOC_CTX *mem_ctx,
						  struct tevent_context *ev,
						  struct dcerpc_binding_handle *h,
						  uint32_t _samDesired /* [in]  */,
						  WERROR *_Status /* [out] [ref] */,
						  WERROR *_rpc_status /* [out] [ref] */,
						  struct policy_handle *_phKey /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_GetRootKey_recv(struct tevent_req *req,
					TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_GetRootKey(struct dcerpc_binding_handle *h,
				   TALLOC_CTX *mem_ctx,
				   uint32_t _samDesired /* [in]  */,
				   WERROR *_Status /* [out] [ref] */,
				   WERROR *_rpc_status /* [out] [ref] */,
				   struct policy_handle *_phKey /* [out] [ref] */);

struct tevent_req *dcerpc_clusapi_CreateKey_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_CreateKey *r);
NTSTATUS dcerpc_clusapi_CreateKey_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_CreateKey_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_CreateKey *r);
struct tevent_req *dcerpc_clusapi_CreateKey_send(TALLOC_CTX *mem_ctx,
						 struct tevent_context *ev,
						 struct dcerpc_binding_handle *h,
						 struct policy_handle _hKey /* [in]  */,
						 const char *_lpSubKey /* [in] [charset(UTF16),ref] */,
						 uint32_t _dwOptions /* [in]  */,
						 uint32_t _samDesired /* [in]  */,
						 struct RPC_SECURITY_ATTRIBUTES *_lpSecurityAttributes /* [in] [unique] */,
						 uint32_t *_lpdwDisposition /* [out] [ref] */,
						 WERROR *_Status /* [out] [ref] */,
						 WERROR *_rpc_status /* [out] [ref] */,
						 struct policy_handle *_phKey /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_CreateKey_recv(struct tevent_req *req,
				       TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_CreateKey(struct dcerpc_binding_handle *h,
				  TALLOC_CTX *mem_ctx,
				  struct policy_handle _hKey /* [in]  */,
				  const char *_lpSubKey /* [in] [charset(UTF16),ref] */,
				  uint32_t _dwOptions /* [in]  */,
				  uint32_t _samDesired /* [in]  */,
				  struct RPC_SECURITY_ATTRIBUTES *_lpSecurityAttributes /* [in] [unique] */,
				  uint32_t *_lpdwDisposition /* [out] [ref] */,
				  WERROR *_Status /* [out] [ref] */,
				  WERROR *_rpc_status /* [out] [ref] */,
				  struct policy_handle *_phKey /* [out] [ref] */);

struct tevent_req *dcerpc_clusapi_OpenKey_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_OpenKey *r);
NTSTATUS dcerpc_clusapi_OpenKey_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_OpenKey_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_OpenKey *r);
struct tevent_req *dcerpc_clusapi_OpenKey_send(TALLOC_CTX *mem_ctx,
					       struct tevent_context *ev,
					       struct dcerpc_binding_handle *h,
					       struct policy_handle _hKey /* [in]  */,
					       const char *_lpSubKey /* [in] [charset(UTF16),ref] */,
					       uint32_t _samDesired /* [in]  */,
					       WERROR *_Status /* [out] [ref] */,
					       WERROR *_rpc_status /* [out] [ref] */,
					       struct policy_handle *_phKey /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_OpenKey_recv(struct tevent_req *req,
				     TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_OpenKey(struct dcerpc_binding_handle *h,
				TALLOC_CTX *mem_ctx,
				struct policy_handle _hKey /* [in]  */,
				const char *_lpSubKey /* [in] [charset(UTF16),ref] */,
				uint32_t _samDesired /* [in]  */,
				WERROR *_Status /* [out] [ref] */,
				WERROR *_rpc_status /* [out] [ref] */,
				struct policy_handle *_phKey /* [out] [ref] */);

struct tevent_req *dcerpc_clusapi_EnumKey_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_EnumKey *r);
NTSTATUS dcerpc_clusapi_EnumKey_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_EnumKey_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_EnumKey *r);
struct tevent_req *dcerpc_clusapi_EnumKey_send(TALLOC_CTX *mem_ctx,
					       struct tevent_context *ev,
					       struct dcerpc_binding_handle *h,
					       struct policy_handle _hKey /* [in]  */,
					       uint32_t _dwIndex /* [in]  */,
					       const char **_KeyName /* [out] [charset(UTF16),ref] */,
					       NTTIME *_lpftLastWriteTime /* [out] [ref] */,
					       WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_EnumKey_recv(struct tevent_req *req,
				     TALLOC_CTX *mem_ctx,
				     WERROR *result);
NTSTATUS dcerpc_clusapi_EnumKey(struct dcerpc_binding_handle *h,
				TALLOC_CTX *mem_ctx,
				struct policy_handle _hKey /* [in]  */,
				uint32_t _dwIndex /* [in]  */,
				const char **_KeyName /* [out] [charset(UTF16),ref] */,
				NTTIME *_lpftLastWriteTime /* [out] [ref] */,
				WERROR *_rpc_status /* [out] [ref] */,
				WERROR *result);

struct tevent_req *dcerpc_clusapi_SetValue_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_SetValue *r);
NTSTATUS dcerpc_clusapi_SetValue_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_SetValue_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_SetValue *r);
struct tevent_req *dcerpc_clusapi_SetValue_send(TALLOC_CTX *mem_ctx,
						struct tevent_context *ev,
						struct dcerpc_binding_handle *h,
						struct policy_handle _hKey /* [in]  */,
						const char *_lpValueName /* [in] [charset(UTF16),ref] */,
						enum winreg_Type _dwType /* [in]  */,
						uint8_t *_lpData /* [in] [ref,size_is(cbData)] */,
						uint32_t _cbData /* [in]  */,
						WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_SetValue_recv(struct tevent_req *req,
				      TALLOC_CTX *mem_ctx,
				      WERROR *result);
NTSTATUS dcerpc_clusapi_SetValue(struct dcerpc_binding_handle *h,
				 TALLOC_CTX *mem_ctx,
				 struct policy_handle _hKey /* [in]  */,
				 const char *_lpValueName /* [in] [charset(UTF16),ref] */,
				 enum winreg_Type _dwType /* [in]  */,
				 uint8_t *_lpData /* [in] [ref,size_is(cbData)] */,
				 uint32_t _cbData /* [in]  */,
				 WERROR *_rpc_status /* [out] [ref] */,
				 WERROR *result);

struct tevent_req *dcerpc_clusapi_DeleteValue_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_DeleteValue *r);
NTSTATUS dcerpc_clusapi_DeleteValue_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_DeleteValue_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_DeleteValue *r);
struct tevent_req *dcerpc_clusapi_DeleteValue_send(TALLOC_CTX *mem_ctx,
						   struct tevent_context *ev,
						   struct dcerpc_binding_handle *h,
						   struct policy_handle _hKey /* [in]  */,
						   const char *_lpValueName /* [in] [charset(UTF16),ref] */,
						   WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_DeleteValue_recv(struct tevent_req *req,
					 TALLOC_CTX *mem_ctx,
					 WERROR *result);
NTSTATUS dcerpc_clusapi_DeleteValue(struct dcerpc_binding_handle *h,
				    TALLOC_CTX *mem_ctx,
				    struct policy_handle _hKey /* [in]  */,
				    const char *_lpValueName /* [in] [charset(UTF16),ref] */,
				    WERROR *_rpc_status /* [out] [ref] */,
				    WERROR *result);

struct tevent_req *dcerpc_clusapi_QueryValue_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_QueryValue *r);
NTSTATUS dcerpc_clusapi_QueryValue_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_QueryValue_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_QueryValue *r);
struct tevent_req *dcerpc_clusapi_QueryValue_send(TALLOC_CTX *mem_ctx,
						  struct tevent_context *ev,
						  struct dcerpc_binding_handle *h,
						  struct policy_handle _hKey /* [in]  */,
						  const char *_lpValueName /* [in] [charset(UTF16),ref] */,
						  enum winreg_Type *_lpValueType /* [out] [ref] */,
						  uint8_t *_lpData /* [out] [ref,size_is(cbData)] */,
						  uint32_t _cbData /* [in]  */,
						  uint32_t *_lpcbRequired /* [out] [ref] */,
						  WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_QueryValue_recv(struct tevent_req *req,
					TALLOC_CTX *mem_ctx,
					WERROR *result);
NTSTATUS dcerpc_clusapi_QueryValue(struct dcerpc_binding_handle *h,
				   TALLOC_CTX *mem_ctx,
				   struct policy_handle _hKey /* [in]  */,
				   const char *_lpValueName /* [in] [charset(UTF16),ref] */,
				   enum winreg_Type *_lpValueType /* [out] [ref] */,
				   uint8_t *_lpData /* [out] [ref,size_is(cbData)] */,
				   uint32_t _cbData /* [in]  */,
				   uint32_t *_lpcbRequired /* [out] [ref] */,
				   WERROR *_rpc_status /* [out] [ref] */,
				   WERROR *result);

struct tevent_req *dcerpc_clusapi_DeleteKey_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_DeleteKey *r);
NTSTATUS dcerpc_clusapi_DeleteKey_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_DeleteKey_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_DeleteKey *r);
struct tevent_req *dcerpc_clusapi_DeleteKey_send(TALLOC_CTX *mem_ctx,
						 struct tevent_context *ev,
						 struct dcerpc_binding_handle *h,
						 struct policy_handle _hKey /* [in]  */,
						 const char *_lpSubKey /* [in] [charset(UTF16),ref] */,
						 WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_DeleteKey_recv(struct tevent_req *req,
				       TALLOC_CTX *mem_ctx,
				       WERROR *result);
NTSTATUS dcerpc_clusapi_DeleteKey(struct dcerpc_binding_handle *h,
				  TALLOC_CTX *mem_ctx,
				  struct policy_handle _hKey /* [in]  */,
				  const char *_lpSubKey /* [in] [charset(UTF16),ref] */,
				  WERROR *_rpc_status /* [out] [ref] */,
				  WERROR *result);

struct tevent_req *dcerpc_clusapi_EnumValue_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_EnumValue *r);
NTSTATUS dcerpc_clusapi_EnumValue_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_EnumValue_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_EnumValue *r);
struct tevent_req *dcerpc_clusapi_EnumValue_send(TALLOC_CTX *mem_ctx,
						 struct tevent_context *ev,
						 struct dcerpc_binding_handle *h,
						 struct policy_handle _hKey /* [in]  */,
						 uint32_t _dwIndex /* [in]  */,
						 const char **_lpValueName /* [out] [charset(UTF16),ref] */,
						 enum winreg_Type *_lpType /* [out] [ref] */,
						 uint8_t *_lpData /* [out] [ref,size_is(*lpcbData)] */,
						 uint32_t *_lpcbData /* [in,out] [ref] */,
						 uint32_t *_TotalSize /* [out] [ref] */,
						 WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_EnumValue_recv(struct tevent_req *req,
				       TALLOC_CTX *mem_ctx,
				       WERROR *result);
NTSTATUS dcerpc_clusapi_EnumValue(struct dcerpc_binding_handle *h,
				  TALLOC_CTX *mem_ctx,
				  struct policy_handle _hKey /* [in]  */,
				  uint32_t _dwIndex /* [in]  */,
				  const char **_lpValueName /* [out] [charset(UTF16),ref] */,
				  enum winreg_Type *_lpType /* [out] [ref] */,
				  uint8_t *_lpData /* [out] [ref,size_is(*lpcbData)] */,
				  uint32_t *_lpcbData /* [in,out] [ref] */,
				  uint32_t *_TotalSize /* [out] [ref] */,
				  WERROR *_rpc_status /* [out] [ref] */,
				  WERROR *result);

struct tevent_req *dcerpc_clusapi_CloseKey_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_CloseKey *r);
NTSTATUS dcerpc_clusapi_CloseKey_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_CloseKey_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_CloseKey *r);
struct tevent_req *dcerpc_clusapi_CloseKey_send(TALLOC_CTX *mem_ctx,
						struct tevent_context *ev,
						struct dcerpc_binding_handle *h,
						struct policy_handle *_pKey /* [in,out] [ref] */);
NTSTATUS dcerpc_clusapi_CloseKey_recv(struct tevent_req *req,
				      TALLOC_CTX *mem_ctx,
				      WERROR *result);
NTSTATUS dcerpc_clusapi_CloseKey(struct dcerpc_binding_handle *h,
				 TALLOC_CTX *mem_ctx,
				 struct policy_handle *_pKey /* [in,out] [ref] */,
				 WERROR *result);

struct tevent_req *dcerpc_clusapi_QueryInfoKey_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_QueryInfoKey *r);
NTSTATUS dcerpc_clusapi_QueryInfoKey_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_QueryInfoKey_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_QueryInfoKey *r);
struct tevent_req *dcerpc_clusapi_QueryInfoKey_send(TALLOC_CTX *mem_ctx,
						    struct tevent_context *ev,
						    struct dcerpc_binding_handle *h,
						    struct policy_handle _hKey /* [in]  */,
						    uint32_t *_lpcSubKeys /* [out] [ref] */,
						    uint32_t *_lpcbMaxSubKeyLen /* [out] [ref] */,
						    uint32_t *_lpcValues /* [out] [ref] */,
						    uint32_t *_lpcbMaxValueNameLen /* [out] [ref] */,
						    uint32_t *_lpcbMaxValueLen /* [out] [ref] */,
						    uint32_t *_lpcbSecurityDescriptor /* [out] [ref] */,
						    NTTIME *_lpftLastWriteTime /* [out] [ref] */,
						    WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_QueryInfoKey_recv(struct tevent_req *req,
					  TALLOC_CTX *mem_ctx,
					  WERROR *result);
NTSTATUS dcerpc_clusapi_QueryInfoKey(struct dcerpc_binding_handle *h,
				     TALLOC_CTX *mem_ctx,
				     struct policy_handle _hKey /* [in]  */,
				     uint32_t *_lpcSubKeys /* [out] [ref] */,
				     uint32_t *_lpcbMaxSubKeyLen /* [out] [ref] */,
				     uint32_t *_lpcValues /* [out] [ref] */,
				     uint32_t *_lpcbMaxValueNameLen /* [out] [ref] */,
				     uint32_t *_lpcbMaxValueLen /* [out] [ref] */,
				     uint32_t *_lpcbSecurityDescriptor /* [out] [ref] */,
				     NTTIME *_lpftLastWriteTime /* [out] [ref] */,
				     WERROR *_rpc_status /* [out] [ref] */,
				     WERROR *result);

struct tevent_req *dcerpc_clusapi_SetKeySecurity_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_SetKeySecurity *r);
NTSTATUS dcerpc_clusapi_SetKeySecurity_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_SetKeySecurity_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_SetKeySecurity *r);
struct tevent_req *dcerpc_clusapi_SetKeySecurity_send(TALLOC_CTX *mem_ctx,
						      struct tevent_context *ev,
						      struct dcerpc_binding_handle *h,
						      struct policy_handle _hKey /* [in]  */,
						      uint32_t _SecurityInformation /* [in]  */,
						      struct RPC_SECURITY_DESCRIPTOR *_pRpcSecurityDescriptor /* [in] [ref] */,
						      WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_SetKeySecurity_recv(struct tevent_req *req,
					    TALLOC_CTX *mem_ctx,
					    WERROR *result);
NTSTATUS dcerpc_clusapi_SetKeySecurity(struct dcerpc_binding_handle *h,
				       TALLOC_CTX *mem_ctx,
				       struct policy_handle _hKey /* [in]  */,
				       uint32_t _SecurityInformation /* [in]  */,
				       struct RPC_SECURITY_DESCRIPTOR *_pRpcSecurityDescriptor /* [in] [ref] */,
				       WERROR *_rpc_status /* [out] [ref] */,
				       WERROR *result);

struct tevent_req *dcerpc_clusapi_GetKeySecurity_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_GetKeySecurity *r);
NTSTATUS dcerpc_clusapi_GetKeySecurity_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_GetKeySecurity_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_GetKeySecurity *r);
struct tevent_req *dcerpc_clusapi_GetKeySecurity_send(TALLOC_CTX *mem_ctx,
						      struct tevent_context *ev,
						      struct dcerpc_binding_handle *h,
						      struct policy_handle _hKey /* [in]  */,
						      uint32_t _SecurityInformation /* [in]  */,
						      struct RPC_SECURITY_DESCRIPTOR *_pRpcSecurityDescriptor /* [in,out] [ref] */,
						      WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_GetKeySecurity_recv(struct tevent_req *req,
					    TALLOC_CTX *mem_ctx,
					    WERROR *result);
NTSTATUS dcerpc_clusapi_GetKeySecurity(struct dcerpc_binding_handle *h,
				       TALLOC_CTX *mem_ctx,
				       struct policy_handle _hKey /* [in]  */,
				       uint32_t _SecurityInformation /* [in]  */,
				       struct RPC_SECURITY_DESCRIPTOR *_pRpcSecurityDescriptor /* [in,out] [ref] */,
				       WERROR *_rpc_status /* [out] [ref] */,
				       WERROR *result);

struct tevent_req *dcerpc_clusapi_OpenGroup_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_OpenGroup *r);
NTSTATUS dcerpc_clusapi_OpenGroup_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_OpenGroup_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_OpenGroup *r);
struct tevent_req *dcerpc_clusapi_OpenGroup_send(TALLOC_CTX *mem_ctx,
						 struct tevent_context *ev,
						 struct dcerpc_binding_handle *h,
						 const char *_lpszGroupName /* [in] [charset(UTF16),ref] */,
						 WERROR *_Status /* [out] [ref] */,
						 WERROR *_rpc_status /* [out] [ref] */,
						 struct policy_handle *_hGroup /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_OpenGroup_recv(struct tevent_req *req,
				       TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_OpenGroup(struct dcerpc_binding_handle *h,
				  TALLOC_CTX *mem_ctx,
				  const char *_lpszGroupName /* [in] [charset(UTF16),ref] */,
				  WERROR *_Status /* [out] [ref] */,
				  WERROR *_rpc_status /* [out] [ref] */,
				  struct policy_handle *_hGroup /* [out] [ref] */);

struct tevent_req *dcerpc_clusapi_CreateGroup_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_CreateGroup *r);
NTSTATUS dcerpc_clusapi_CreateGroup_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_CreateGroup_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_CreateGroup *r);
struct tevent_req *dcerpc_clusapi_CreateGroup_send(TALLOC_CTX *mem_ctx,
						   struct tevent_context *ev,
						   struct dcerpc_binding_handle *h,
						   const char *_lpszGroupName /* [in] [charset(UTF16),ref] */,
						   WERROR *_Status /* [out] [ref] */,
						   WERROR *_rpc_status /* [out] [ref] */,
						   struct policy_handle *_hGroup /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_CreateGroup_recv(struct tevent_req *req,
					 TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_CreateGroup(struct dcerpc_binding_handle *h,
				    TALLOC_CTX *mem_ctx,
				    const char *_lpszGroupName /* [in] [charset(UTF16),ref] */,
				    WERROR *_Status /* [out] [ref] */,
				    WERROR *_rpc_status /* [out] [ref] */,
				    struct policy_handle *_hGroup /* [out] [ref] */);

struct tevent_req *dcerpc_clusapi_DeleteGroup_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_DeleteGroup *r);
NTSTATUS dcerpc_clusapi_DeleteGroup_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_DeleteGroup_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_DeleteGroup *r);
struct tevent_req *dcerpc_clusapi_DeleteGroup_send(TALLOC_CTX *mem_ctx,
						   struct tevent_context *ev,
						   struct dcerpc_binding_handle *h,
						   struct policy_handle _Group /* [in]  */,
						   uint8_t _force /* [in]  */,
						   WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_DeleteGroup_recv(struct tevent_req *req,
					 TALLOC_CTX *mem_ctx,
					 WERROR *result);
NTSTATUS dcerpc_clusapi_DeleteGroup(struct dcerpc_binding_handle *h,
				    TALLOC_CTX *mem_ctx,
				    struct policy_handle _Group /* [in]  */,
				    uint8_t _force /* [in]  */,
				    WERROR *_rpc_status /* [out] [ref] */,
				    WERROR *result);

struct tevent_req *dcerpc_clusapi_CloseGroup_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_CloseGroup *r);
NTSTATUS dcerpc_clusapi_CloseGroup_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_CloseGroup_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_CloseGroup *r);
struct tevent_req *dcerpc_clusapi_CloseGroup_send(TALLOC_CTX *mem_ctx,
						  struct tevent_context *ev,
						  struct dcerpc_binding_handle *h,
						  struct policy_handle *_Group /* [in,out] [ref] */);
NTSTATUS dcerpc_clusapi_CloseGroup_recv(struct tevent_req *req,
					TALLOC_CTX *mem_ctx,
					WERROR *result);
NTSTATUS dcerpc_clusapi_CloseGroup(struct dcerpc_binding_handle *h,
				   TALLOC_CTX *mem_ctx,
				   struct policy_handle *_Group /* [in,out] [ref] */,
				   WERROR *result);

struct tevent_req *dcerpc_clusapi_GetGroupState_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_GetGroupState *r);
NTSTATUS dcerpc_clusapi_GetGroupState_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_GetGroupState_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_GetGroupState *r);
struct tevent_req *dcerpc_clusapi_GetGroupState_send(TALLOC_CTX *mem_ctx,
						     struct tevent_context *ev,
						     struct dcerpc_binding_handle *h,
						     struct policy_handle _hGroup /* [in]  */,
						     enum clusapi_ClusterGroupState *_State /* [out] [ref] */,
						     const char **_NodeName /* [out] [charset(UTF16),ref] */,
						     WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_GetGroupState_recv(struct tevent_req *req,
					   TALLOC_CTX *mem_ctx,
					   WERROR *result);
NTSTATUS dcerpc_clusapi_GetGroupState(struct dcerpc_binding_handle *h,
				      TALLOC_CTX *mem_ctx,
				      struct policy_handle _hGroup /* [in]  */,
				      enum clusapi_ClusterGroupState *_State /* [out] [ref] */,
				      const char **_NodeName /* [out] [charset(UTF16),ref] */,
				      WERROR *_rpc_status /* [out] [ref] */,
				      WERROR *result);

struct tevent_req *dcerpc_clusapi_SetGroupName_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_SetGroupName *r);
NTSTATUS dcerpc_clusapi_SetGroupName_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_SetGroupName_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_SetGroupName *r);
struct tevent_req *dcerpc_clusapi_SetGroupName_send(TALLOC_CTX *mem_ctx,
						    struct tevent_context *ev,
						    struct dcerpc_binding_handle *h,
						    struct policy_handle _hGroup /* [in]  */,
						    const char *_lpszGroupName /* [in] [charset(UTF16),ref] */,
						    WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_SetGroupName_recv(struct tevent_req *req,
					  TALLOC_CTX *mem_ctx,
					  WERROR *result);
NTSTATUS dcerpc_clusapi_SetGroupName(struct dcerpc_binding_handle *h,
				     TALLOC_CTX *mem_ctx,
				     struct policy_handle _hGroup /* [in]  */,
				     const char *_lpszGroupName /* [in] [charset(UTF16),ref] */,
				     WERROR *_rpc_status /* [out] [ref] */,
				     WERROR *result);

struct tevent_req *dcerpc_clusapi_GetGroupId_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_GetGroupId *r);
NTSTATUS dcerpc_clusapi_GetGroupId_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_GetGroupId_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_GetGroupId *r);
struct tevent_req *dcerpc_clusapi_GetGroupId_send(TALLOC_CTX *mem_ctx,
						  struct tevent_context *ev,
						  struct dcerpc_binding_handle *h,
						  struct policy_handle _hGroup /* [in]  */,
						  const char **_pGuid /* [out] [charset(UTF16),ref] */,
						  WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_GetGroupId_recv(struct tevent_req *req,
					TALLOC_CTX *mem_ctx,
					WERROR *result);
NTSTATUS dcerpc_clusapi_GetGroupId(struct dcerpc_binding_handle *h,
				   TALLOC_CTX *mem_ctx,
				   struct policy_handle _hGroup /* [in]  */,
				   const char **_pGuid /* [out] [charset(UTF16),ref] */,
				   WERROR *_rpc_status /* [out] [ref] */,
				   WERROR *result);

struct tevent_req *dcerpc_clusapi_GetNodeId_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_GetNodeId *r);
NTSTATUS dcerpc_clusapi_GetNodeId_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_GetNodeId_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_GetNodeId *r);
struct tevent_req *dcerpc_clusapi_GetNodeId_send(TALLOC_CTX *mem_ctx,
						 struct tevent_context *ev,
						 struct dcerpc_binding_handle *h,
						 struct policy_handle _hNode /* [in]  */,
						 const char **_pGuid /* [out] [charset(UTF16),ref] */,
						 WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_GetNodeId_recv(struct tevent_req *req,
				       TALLOC_CTX *mem_ctx,
				       WERROR *result);
NTSTATUS dcerpc_clusapi_GetNodeId(struct dcerpc_binding_handle *h,
				  TALLOC_CTX *mem_ctx,
				  struct policy_handle _hNode /* [in]  */,
				  const char **_pGuid /* [out] [charset(UTF16),ref] */,
				  WERROR *_rpc_status /* [out] [ref] */,
				  WERROR *result);

struct tevent_req *dcerpc_clusapi_OnlineGroup_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_OnlineGroup *r);
NTSTATUS dcerpc_clusapi_OnlineGroup_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_OnlineGroup_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_OnlineGroup *r);
struct tevent_req *dcerpc_clusapi_OnlineGroup_send(TALLOC_CTX *mem_ctx,
						   struct tevent_context *ev,
						   struct dcerpc_binding_handle *h,
						   struct policy_handle _hGroup /* [in]  */,
						   WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_OnlineGroup_recv(struct tevent_req *req,
					 TALLOC_CTX *mem_ctx,
					 WERROR *result);
NTSTATUS dcerpc_clusapi_OnlineGroup(struct dcerpc_binding_handle *h,
				    TALLOC_CTX *mem_ctx,
				    struct policy_handle _hGroup /* [in]  */,
				    WERROR *_rpc_status /* [out] [ref] */,
				    WERROR *result);

struct tevent_req *dcerpc_clusapi_OfflineGroup_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_OfflineGroup *r);
NTSTATUS dcerpc_clusapi_OfflineGroup_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_OfflineGroup_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_OfflineGroup *r);
struct tevent_req *dcerpc_clusapi_OfflineGroup_send(TALLOC_CTX *mem_ctx,
						    struct tevent_context *ev,
						    struct dcerpc_binding_handle *h,
						    struct policy_handle _hGroup /* [in]  */,
						    WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_OfflineGroup_recv(struct tevent_req *req,
					  TALLOC_CTX *mem_ctx,
					  WERROR *result);
NTSTATUS dcerpc_clusapi_OfflineGroup(struct dcerpc_binding_handle *h,
				     TALLOC_CTX *mem_ctx,
				     struct policy_handle _hGroup /* [in]  */,
				     WERROR *_rpc_status /* [out] [ref] */,
				     WERROR *result);

struct tevent_req *dcerpc_clusapi_MoveGroup_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_MoveGroup *r);
NTSTATUS dcerpc_clusapi_MoveGroup_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_MoveGroup_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_MoveGroup *r);
struct tevent_req *dcerpc_clusapi_MoveGroup_send(TALLOC_CTX *mem_ctx,
						 struct tevent_context *ev,
						 struct dcerpc_binding_handle *h,
						 struct policy_handle _hGroup /* [in]  */,
						 WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_MoveGroup_recv(struct tevent_req *req,
				       TALLOC_CTX *mem_ctx,
				       WERROR *result);
NTSTATUS dcerpc_clusapi_MoveGroup(struct dcerpc_binding_handle *h,
				  TALLOC_CTX *mem_ctx,
				  struct policy_handle _hGroup /* [in]  */,
				  WERROR *_rpc_status /* [out] [ref] */,
				  WERROR *result);

struct tevent_req *dcerpc_clusapi_MoveGroupToNode_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_MoveGroupToNode *r);
NTSTATUS dcerpc_clusapi_MoveGroupToNode_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_MoveGroupToNode_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_MoveGroupToNode *r);
struct tevent_req *dcerpc_clusapi_MoveGroupToNode_send(TALLOC_CTX *mem_ctx,
						       struct tevent_context *ev,
						       struct dcerpc_binding_handle *h,
						       struct policy_handle _hGroup /* [in]  */,
						       struct policy_handle _hNode /* [in]  */,
						       WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_MoveGroupToNode_recv(struct tevent_req *req,
					     TALLOC_CTX *mem_ctx,
					     WERROR *result);
NTSTATUS dcerpc_clusapi_MoveGroupToNode(struct dcerpc_binding_handle *h,
					TALLOC_CTX *mem_ctx,
					struct policy_handle _hGroup /* [in]  */,
					struct policy_handle _hNode /* [in]  */,
					WERROR *_rpc_status /* [out] [ref] */,
					WERROR *result);

struct tevent_req *dcerpc_clusapi_CreateGroupResourceEnum_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_CreateGroupResourceEnum *r);
NTSTATUS dcerpc_clusapi_CreateGroupResourceEnum_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_CreateGroupResourceEnum_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_CreateGroupResourceEnum *r);
struct tevent_req *dcerpc_clusapi_CreateGroupResourceEnum_send(TALLOC_CTX *mem_ctx,
							       struct tevent_context *ev,
							       struct dcerpc_binding_handle *h,
							       struct policy_handle _hGroup /* [in]  */,
							       uint32_t _dwType /* [in]  */,
							       struct ENUM_LIST **_ReturnEnum /* [out] [ref] */,
							       WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_CreateGroupResourceEnum_recv(struct tevent_req *req,
						     TALLOC_CTX *mem_ctx,
						     WERROR *result);
NTSTATUS dcerpc_clusapi_CreateGroupResourceEnum(struct dcerpc_binding_handle *h,
						TALLOC_CTX *mem_ctx,
						struct policy_handle _hGroup /* [in]  */,
						uint32_t _dwType /* [in]  */,
						struct ENUM_LIST **_ReturnEnum /* [out] [ref] */,
						WERROR *_rpc_status /* [out] [ref] */,
						WERROR *result);

struct tevent_req *dcerpc_clusapi_SetGroupNodeList_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_SetGroupNodeList *r);
NTSTATUS dcerpc_clusapi_SetGroupNodeList_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_SetGroupNodeList_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_SetGroupNodeList *r);
struct tevent_req *dcerpc_clusapi_SetGroupNodeList_send(TALLOC_CTX *mem_ctx,
							struct tevent_context *ev,
							struct dcerpc_binding_handle *h,
							struct policy_handle _hGroup /* [in]  */,
							uint16_t *_multiSzNodeList /* [in] [size_is(cchListSize),unique] */,
							uint32_t _cchListSize /* [in]  */,
							WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_SetGroupNodeList_recv(struct tevent_req *req,
					      TALLOC_CTX *mem_ctx,
					      WERROR *result);
NTSTATUS dcerpc_clusapi_SetGroupNodeList(struct dcerpc_binding_handle *h,
					 TALLOC_CTX *mem_ctx,
					 struct policy_handle _hGroup /* [in]  */,
					 uint16_t *_multiSzNodeList /* [in] [size_is(cchListSize),unique] */,
					 uint32_t _cchListSize /* [in]  */,
					 WERROR *_rpc_status /* [out] [ref] */,
					 WERROR *result);

struct tevent_req *dcerpc_clusapi_CreateNotify_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_CreateNotify *r);
NTSTATUS dcerpc_clusapi_CreateNotify_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_CreateNotify_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_CreateNotify *r);
struct tevent_req *dcerpc_clusapi_CreateNotify_send(TALLOC_CTX *mem_ctx,
						    struct tevent_context *ev,
						    struct dcerpc_binding_handle *h,
						    WERROR *_Status /* [out] [ref] */,
						    WERROR *_rpc_status /* [out] [ref] */,
						    struct policy_handle *_hNotify /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_CreateNotify_recv(struct tevent_req *req,
					  TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_CreateNotify(struct dcerpc_binding_handle *h,
				     TALLOC_CTX *mem_ctx,
				     WERROR *_Status /* [out] [ref] */,
				     WERROR *_rpc_status /* [out] [ref] */,
				     struct policy_handle *_hNotify /* [out] [ref] */);

struct tevent_req *dcerpc_clusapi_CloseNotify_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_CloseNotify *r);
NTSTATUS dcerpc_clusapi_CloseNotify_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_CloseNotify_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_CloseNotify *r);
struct tevent_req *dcerpc_clusapi_CloseNotify_send(TALLOC_CTX *mem_ctx,
						   struct tevent_context *ev,
						   struct dcerpc_binding_handle *h,
						   struct policy_handle *_Notify /* [in,out] [ref] */);
NTSTATUS dcerpc_clusapi_CloseNotify_recv(struct tevent_req *req,
					 TALLOC_CTX *mem_ctx,
					 WERROR *result);
NTSTATUS dcerpc_clusapi_CloseNotify(struct dcerpc_binding_handle *h,
				    TALLOC_CTX *mem_ctx,
				    struct policy_handle *_Notify /* [in,out] [ref] */,
				    WERROR *result);

struct tevent_req *dcerpc_clusapi_AddNotifyCluster_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_AddNotifyCluster *r);
NTSTATUS dcerpc_clusapi_AddNotifyCluster_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_AddNotifyCluster_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_AddNotifyCluster *r);
struct tevent_req *dcerpc_clusapi_AddNotifyCluster_send(TALLOC_CTX *mem_ctx,
							struct tevent_context *ev,
							struct dcerpc_binding_handle *h,
							struct policy_handle _hNotify /* [in]  */,
							struct policy_handle _hCluster /* [in]  */,
							uint32_t _dwFilter /* [in]  */,
							uint32_t _dwNotifyKey /* [in]  */,
							WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_AddNotifyCluster_recv(struct tevent_req *req,
					      TALLOC_CTX *mem_ctx,
					      WERROR *result);
NTSTATUS dcerpc_clusapi_AddNotifyCluster(struct dcerpc_binding_handle *h,
					 TALLOC_CTX *mem_ctx,
					 struct policy_handle _hNotify /* [in]  */,
					 struct policy_handle _hCluster /* [in]  */,
					 uint32_t _dwFilter /* [in]  */,
					 uint32_t _dwNotifyKey /* [in]  */,
					 WERROR *_rpc_status /* [out] [ref] */,
					 WERROR *result);

struct tevent_req *dcerpc_clusapi_AddNotifyNode_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_AddNotifyNode *r);
NTSTATUS dcerpc_clusapi_AddNotifyNode_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_AddNotifyNode_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_AddNotifyNode *r);
struct tevent_req *dcerpc_clusapi_AddNotifyNode_send(TALLOC_CTX *mem_ctx,
						     struct tevent_context *ev,
						     struct dcerpc_binding_handle *h,
						     struct policy_handle _hNotify /* [in]  */,
						     struct policy_handle _hNode /* [in]  */,
						     uint32_t _dwFilter /* [in]  */,
						     uint32_t _dwNotifyKey /* [in]  */,
						     uint32_t *_dwStateSequence /* [out] [ref] */,
						     WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_AddNotifyNode_recv(struct tevent_req *req,
					   TALLOC_CTX *mem_ctx,
					   WERROR *result);
NTSTATUS dcerpc_clusapi_AddNotifyNode(struct dcerpc_binding_handle *h,
				      TALLOC_CTX *mem_ctx,
				      struct policy_handle _hNotify /* [in]  */,
				      struct policy_handle _hNode /* [in]  */,
				      uint32_t _dwFilter /* [in]  */,
				      uint32_t _dwNotifyKey /* [in]  */,
				      uint32_t *_dwStateSequence /* [out] [ref] */,
				      WERROR *_rpc_status /* [out] [ref] */,
				      WERROR *result);

struct tevent_req *dcerpc_clusapi_AddNotifyGroup_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_AddNotifyGroup *r);
NTSTATUS dcerpc_clusapi_AddNotifyGroup_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_AddNotifyGroup_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_AddNotifyGroup *r);
struct tevent_req *dcerpc_clusapi_AddNotifyGroup_send(TALLOC_CTX *mem_ctx,
						      struct tevent_context *ev,
						      struct dcerpc_binding_handle *h,
						      struct policy_handle _hNotify /* [in]  */,
						      struct policy_handle _hGroup /* [in]  */,
						      uint32_t _dwFilter /* [in]  */,
						      uint32_t _dwNotifyKey /* [in]  */,
						      uint32_t *_dwStateSequence /* [out] [ref] */,
						      WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_AddNotifyGroup_recv(struct tevent_req *req,
					    TALLOC_CTX *mem_ctx,
					    WERROR *result);
NTSTATUS dcerpc_clusapi_AddNotifyGroup(struct dcerpc_binding_handle *h,
				       TALLOC_CTX *mem_ctx,
				       struct policy_handle _hNotify /* [in]  */,
				       struct policy_handle _hGroup /* [in]  */,
				       uint32_t _dwFilter /* [in]  */,
				       uint32_t _dwNotifyKey /* [in]  */,
				       uint32_t *_dwStateSequence /* [out] [ref] */,
				       WERROR *_rpc_status /* [out] [ref] */,
				       WERROR *result);

struct tevent_req *dcerpc_clusapi_AddNotifyResource_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_AddNotifyResource *r);
NTSTATUS dcerpc_clusapi_AddNotifyResource_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_AddNotifyResource_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_AddNotifyResource *r);
struct tevent_req *dcerpc_clusapi_AddNotifyResource_send(TALLOC_CTX *mem_ctx,
							 struct tevent_context *ev,
							 struct dcerpc_binding_handle *h,
							 struct policy_handle _hNotify /* [in]  */,
							 struct policy_handle _hResource /* [in]  */,
							 uint32_t _dwFilter /* [in]  */,
							 uint32_t _dwNotifyKey /* [in]  */,
							 uint32_t *_dwStateSequence /* [out] [ref] */,
							 WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_AddNotifyResource_recv(struct tevent_req *req,
					       TALLOC_CTX *mem_ctx,
					       WERROR *result);
NTSTATUS dcerpc_clusapi_AddNotifyResource(struct dcerpc_binding_handle *h,
					  TALLOC_CTX *mem_ctx,
					  struct policy_handle _hNotify /* [in]  */,
					  struct policy_handle _hResource /* [in]  */,
					  uint32_t _dwFilter /* [in]  */,
					  uint32_t _dwNotifyKey /* [in]  */,
					  uint32_t *_dwStateSequence /* [out] [ref] */,
					  WERROR *_rpc_status /* [out] [ref] */,
					  WERROR *result);

struct tevent_req *dcerpc_clusapi_AddNotifyKey_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_AddNotifyKey *r);
NTSTATUS dcerpc_clusapi_AddNotifyKey_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_AddNotifyKey_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_AddNotifyKey *r);
struct tevent_req *dcerpc_clusapi_AddNotifyKey_send(TALLOC_CTX *mem_ctx,
						    struct tevent_context *ev,
						    struct dcerpc_binding_handle *h,
						    struct policy_handle _hNotify /* [in]  */,
						    struct policy_handle _hKey /* [in]  */,
						    uint32_t _dwNotifyKey /* [in]  */,
						    uint32_t _Filter /* [in]  */,
						    uint8_t _WatchSubTree /* [in]  */,
						    WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_AddNotifyKey_recv(struct tevent_req *req,
					  TALLOC_CTX *mem_ctx,
					  WERROR *result);
NTSTATUS dcerpc_clusapi_AddNotifyKey(struct dcerpc_binding_handle *h,
				     TALLOC_CTX *mem_ctx,
				     struct policy_handle _hNotify /* [in]  */,
				     struct policy_handle _hKey /* [in]  */,
				     uint32_t _dwNotifyKey /* [in]  */,
				     uint32_t _Filter /* [in]  */,
				     uint8_t _WatchSubTree /* [in]  */,
				     WERROR *_rpc_status /* [out] [ref] */,
				     WERROR *result);

struct tevent_req *dcerpc_clusapi_ReAddNotifyNode_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_ReAddNotifyNode *r);
NTSTATUS dcerpc_clusapi_ReAddNotifyNode_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_ReAddNotifyNode_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_ReAddNotifyNode *r);
struct tevent_req *dcerpc_clusapi_ReAddNotifyNode_send(TALLOC_CTX *mem_ctx,
						       struct tevent_context *ev,
						       struct dcerpc_binding_handle *h,
						       struct policy_handle _hNotify /* [in]  */,
						       struct policy_handle _hNode /* [in]  */,
						       uint32_t _dwFilter /* [in]  */,
						       uint32_t _dwNotifyKey /* [in]  */,
						       uint32_t _StateSequence /* [in]  */,
						       WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_ReAddNotifyNode_recv(struct tevent_req *req,
					     TALLOC_CTX *mem_ctx,
					     WERROR *result);
NTSTATUS dcerpc_clusapi_ReAddNotifyNode(struct dcerpc_binding_handle *h,
					TALLOC_CTX *mem_ctx,
					struct policy_handle _hNotify /* [in]  */,
					struct policy_handle _hNode /* [in]  */,
					uint32_t _dwFilter /* [in]  */,
					uint32_t _dwNotifyKey /* [in]  */,
					uint32_t _StateSequence /* [in]  */,
					WERROR *_rpc_status /* [out] [ref] */,
					WERROR *result);

struct tevent_req *dcerpc_clusapi_ReAddNotifyGroup_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_ReAddNotifyGroup *r);
NTSTATUS dcerpc_clusapi_ReAddNotifyGroup_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_ReAddNotifyGroup_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_ReAddNotifyGroup *r);
struct tevent_req *dcerpc_clusapi_ReAddNotifyGroup_send(TALLOC_CTX *mem_ctx,
							struct tevent_context *ev,
							struct dcerpc_binding_handle *h,
							struct policy_handle _hNotify /* [in]  */,
							struct policy_handle _hGroup /* [in]  */,
							uint32_t _dwFilter /* [in]  */,
							uint32_t _dwNotifyKey /* [in]  */,
							uint32_t _StateSequence /* [in]  */,
							WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_ReAddNotifyGroup_recv(struct tevent_req *req,
					      TALLOC_CTX *mem_ctx,
					      WERROR *result);
NTSTATUS dcerpc_clusapi_ReAddNotifyGroup(struct dcerpc_binding_handle *h,
					 TALLOC_CTX *mem_ctx,
					 struct policy_handle _hNotify /* [in]  */,
					 struct policy_handle _hGroup /* [in]  */,
					 uint32_t _dwFilter /* [in]  */,
					 uint32_t _dwNotifyKey /* [in]  */,
					 uint32_t _StateSequence /* [in]  */,
					 WERROR *_rpc_status /* [out] [ref] */,
					 WERROR *result);

struct tevent_req *dcerpc_clusapi_ReAddNotifyResource_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_ReAddNotifyResource *r);
NTSTATUS dcerpc_clusapi_ReAddNotifyResource_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_ReAddNotifyResource_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_ReAddNotifyResource *r);
struct tevent_req *dcerpc_clusapi_ReAddNotifyResource_send(TALLOC_CTX *mem_ctx,
							   struct tevent_context *ev,
							   struct dcerpc_binding_handle *h,
							   struct policy_handle _hNotify /* [in]  */,
							   struct policy_handle _hResource /* [in]  */,
							   uint32_t _dwFilter /* [in]  */,
							   uint32_t _dwNotifyKey /* [in]  */,
							   uint32_t _StateSequence /* [in]  */,
							   WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_ReAddNotifyResource_recv(struct tevent_req *req,
						 TALLOC_CTX *mem_ctx,
						 WERROR *result);
NTSTATUS dcerpc_clusapi_ReAddNotifyResource(struct dcerpc_binding_handle *h,
					    TALLOC_CTX *mem_ctx,
					    struct policy_handle _hNotify /* [in]  */,
					    struct policy_handle _hResource /* [in]  */,
					    uint32_t _dwFilter /* [in]  */,
					    uint32_t _dwNotifyKey /* [in]  */,
					    uint32_t _StateSequence /* [in]  */,
					    WERROR *_rpc_status /* [out] [ref] */,
					    WERROR *result);

struct tevent_req *dcerpc_clusapi_GetNotify_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_GetNotify *r);
NTSTATUS dcerpc_clusapi_GetNotify_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_GetNotify_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_GetNotify *r);
struct tevent_req *dcerpc_clusapi_GetNotify_send(TALLOC_CTX *mem_ctx,
						 struct tevent_context *ev,
						 struct dcerpc_binding_handle *h,
						 struct policy_handle _hNotify /* [in]  */,
						 uint32_t *_dwNotifyKey /* [out] [ref] */,
						 uint32_t *_dwFilter /* [out] [ref] */,
						 uint32_t *_dwStateSequence /* [out] [ref] */,
						 const char **_Name /* [out] [charset(UTF16),ref] */,
						 WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_GetNotify_recv(struct tevent_req *req,
				       TALLOC_CTX *mem_ctx,
				       WERROR *result);
NTSTATUS dcerpc_clusapi_GetNotify(struct dcerpc_binding_handle *h,
				  TALLOC_CTX *mem_ctx,
				  struct policy_handle _hNotify /* [in]  */,
				  uint32_t *_dwNotifyKey /* [out] [ref] */,
				  uint32_t *_dwFilter /* [out] [ref] */,
				  uint32_t *_dwStateSequence /* [out] [ref] */,
				  const char **_Name /* [out] [charset(UTF16),ref] */,
				  WERROR *_rpc_status /* [out] [ref] */,
				  WERROR *result);

struct tevent_req *dcerpc_clusapi_OpenNode_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_OpenNode *r);
NTSTATUS dcerpc_clusapi_OpenNode_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_OpenNode_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_OpenNode *r);
struct tevent_req *dcerpc_clusapi_OpenNode_send(TALLOC_CTX *mem_ctx,
						struct tevent_context *ev,
						struct dcerpc_binding_handle *h,
						const char *_lpszNodeName /* [in] [charset(UTF16),ref] */,
						WERROR *_Status /* [out] [ref] */,
						WERROR *_rpc_status /* [out] [ref] */,
						struct policy_handle *_hNode /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_OpenNode_recv(struct tevent_req *req,
				      TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_OpenNode(struct dcerpc_binding_handle *h,
				 TALLOC_CTX *mem_ctx,
				 const char *_lpszNodeName /* [in] [charset(UTF16),ref] */,
				 WERROR *_Status /* [out] [ref] */,
				 WERROR *_rpc_status /* [out] [ref] */,
				 struct policy_handle *_hNode /* [out] [ref] */);

struct tevent_req *dcerpc_clusapi_CloseNode_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_CloseNode *r);
NTSTATUS dcerpc_clusapi_CloseNode_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_CloseNode_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_CloseNode *r);
struct tevent_req *dcerpc_clusapi_CloseNode_send(TALLOC_CTX *mem_ctx,
						 struct tevent_context *ev,
						 struct dcerpc_binding_handle *h,
						 struct policy_handle *_Node /* [in,out] [ref] */);
NTSTATUS dcerpc_clusapi_CloseNode_recv(struct tevent_req *req,
				       TALLOC_CTX *mem_ctx,
				       WERROR *result);
NTSTATUS dcerpc_clusapi_CloseNode(struct dcerpc_binding_handle *h,
				  TALLOC_CTX *mem_ctx,
				  struct policy_handle *_Node /* [in,out] [ref] */,
				  WERROR *result);

struct tevent_req *dcerpc_clusapi_GetNodeState_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_GetNodeState *r);
NTSTATUS dcerpc_clusapi_GetNodeState_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_GetNodeState_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_GetNodeState *r);
struct tevent_req *dcerpc_clusapi_GetNodeState_send(TALLOC_CTX *mem_ctx,
						    struct tevent_context *ev,
						    struct dcerpc_binding_handle *h,
						    struct policy_handle _hNode /* [in]  */,
						    enum clusapi_ClusterNodeState *_State /* [out] [ref] */,
						    WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_GetNodeState_recv(struct tevent_req *req,
					  TALLOC_CTX *mem_ctx,
					  WERROR *result);
NTSTATUS dcerpc_clusapi_GetNodeState(struct dcerpc_binding_handle *h,
				     TALLOC_CTX *mem_ctx,
				     struct policy_handle _hNode /* [in]  */,
				     enum clusapi_ClusterNodeState *_State /* [out] [ref] */,
				     WERROR *_rpc_status /* [out] [ref] */,
				     WERROR *result);

struct tevent_req *dcerpc_clusapi_PauseNode_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_PauseNode *r);
NTSTATUS dcerpc_clusapi_PauseNode_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_PauseNode_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_PauseNode *r);
struct tevent_req *dcerpc_clusapi_PauseNode_send(TALLOC_CTX *mem_ctx,
						 struct tevent_context *ev,
						 struct dcerpc_binding_handle *h,
						 struct policy_handle _hNode /* [in]  */,
						 WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_PauseNode_recv(struct tevent_req *req,
				       TALLOC_CTX *mem_ctx,
				       WERROR *result);
NTSTATUS dcerpc_clusapi_PauseNode(struct dcerpc_binding_handle *h,
				  TALLOC_CTX *mem_ctx,
				  struct policy_handle _hNode /* [in]  */,
				  WERROR *_rpc_status /* [out] [ref] */,
				  WERROR *result);

struct tevent_req *dcerpc_clusapi_ResumeNode_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_ResumeNode *r);
NTSTATUS dcerpc_clusapi_ResumeNode_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_ResumeNode_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_ResumeNode *r);
struct tevent_req *dcerpc_clusapi_ResumeNode_send(TALLOC_CTX *mem_ctx,
						  struct tevent_context *ev,
						  struct dcerpc_binding_handle *h,
						  struct policy_handle _hNode /* [in]  */,
						  WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_ResumeNode_recv(struct tevent_req *req,
					TALLOC_CTX *mem_ctx,
					WERROR *result);
NTSTATUS dcerpc_clusapi_ResumeNode(struct dcerpc_binding_handle *h,
				   TALLOC_CTX *mem_ctx,
				   struct policy_handle _hNode /* [in]  */,
				   WERROR *_rpc_status /* [out] [ref] */,
				   WERROR *result);

struct tevent_req *dcerpc_clusapi_EvictNode_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_EvictNode *r);
NTSTATUS dcerpc_clusapi_EvictNode_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_EvictNode_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_EvictNode *r);
struct tevent_req *dcerpc_clusapi_EvictNode_send(TALLOC_CTX *mem_ctx,
						 struct tevent_context *ev,
						 struct dcerpc_binding_handle *h,
						 struct policy_handle _hNode /* [in]  */,
						 WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_EvictNode_recv(struct tevent_req *req,
				       TALLOC_CTX *mem_ctx,
				       WERROR *result);
NTSTATUS dcerpc_clusapi_EvictNode(struct dcerpc_binding_handle *h,
				  TALLOC_CTX *mem_ctx,
				  struct policy_handle _hNode /* [in]  */,
				  WERROR *_rpc_status /* [out] [ref] */,
				  WERROR *result);

struct tevent_req *dcerpc_clusapi_NodeResourceControl_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_NodeResourceControl *r);
NTSTATUS dcerpc_clusapi_NodeResourceControl_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_NodeResourceControl_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_NodeResourceControl *r);
struct tevent_req *dcerpc_clusapi_NodeResourceControl_send(TALLOC_CTX *mem_ctx,
							   struct tevent_context *ev,
							   struct dcerpc_binding_handle *h,
							   struct policy_handle _hResource /* [in]  */,
							   struct policy_handle _hNode /* [in]  */,
							   uint32_t _dwControlCode /* [in]  */,
							   uint8_t *_lpInBuffer /* [in] [size_is(nInBufferSize),unique] */,
							   uint32_t _nInBufferSize /* [in]  */,
							   uint8_t *_lpOutBuffer /* [out] [length_is(*lpBytesReturned),ref,size_is(nOutBufferSize)] */,
							   uint32_t _nOutBufferSize /* [in]  */,
							   uint32_t *_lpBytesReturned /* [out] [ref] */,
							   uint32_t *_lpcbRequired /* [out] [ref] */,
							   WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_NodeResourceControl_recv(struct tevent_req *req,
						 TALLOC_CTX *mem_ctx,
						 WERROR *result);
NTSTATUS dcerpc_clusapi_NodeResourceControl(struct dcerpc_binding_handle *h,
					    TALLOC_CTX *mem_ctx,
					    struct policy_handle _hResource /* [in]  */,
					    struct policy_handle _hNode /* [in]  */,
					    uint32_t _dwControlCode /* [in]  */,
					    uint8_t *_lpInBuffer /* [in] [size_is(nInBufferSize),unique] */,
					    uint32_t _nInBufferSize /* [in]  */,
					    uint8_t *_lpOutBuffer /* [out] [length_is(*lpBytesReturned),ref,size_is(nOutBufferSize)] */,
					    uint32_t _nOutBufferSize /* [in]  */,
					    uint32_t *_lpBytesReturned /* [out] [ref] */,
					    uint32_t *_lpcbRequired /* [out] [ref] */,
					    WERROR *_rpc_status /* [out] [ref] */,
					    WERROR *result);

struct tevent_req *dcerpc_clusapi_ResourceControl_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_ResourceControl *r);
NTSTATUS dcerpc_clusapi_ResourceControl_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_ResourceControl_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_ResourceControl *r);
struct tevent_req *dcerpc_clusapi_ResourceControl_send(TALLOC_CTX *mem_ctx,
						       struct tevent_context *ev,
						       struct dcerpc_binding_handle *h,
						       struct policy_handle _hResource /* [in]  */,
						       enum clusapi_ResourceControlCode _dwControlCode /* [in]  */,
						       uint8_t *_lpInBuffer /* [in] [size_is(nInBufferSize),unique] */,
						       uint32_t _nInBufferSize /* [in]  */,
						       uint8_t *_lpOutBuffer /* [out] [length_is(*lpBytesReturned),ref,size_is(nOutBufferSize)] */,
						       uint32_t _nOutBufferSize /* [in]  */,
						       uint32_t *_lpBytesReturned /* [out] [ref] */,
						       uint32_t *_lpcbRequired /* [out] [ref] */,
						       WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_ResourceControl_recv(struct tevent_req *req,
					     TALLOC_CTX *mem_ctx,
					     WERROR *result);
NTSTATUS dcerpc_clusapi_ResourceControl(struct dcerpc_binding_handle *h,
					TALLOC_CTX *mem_ctx,
					struct policy_handle _hResource /* [in]  */,
					enum clusapi_ResourceControlCode _dwControlCode /* [in]  */,
					uint8_t *_lpInBuffer /* [in] [size_is(nInBufferSize),unique] */,
					uint32_t _nInBufferSize /* [in]  */,
					uint8_t *_lpOutBuffer /* [out] [length_is(*lpBytesReturned),ref,size_is(nOutBufferSize)] */,
					uint32_t _nOutBufferSize /* [in]  */,
					uint32_t *_lpBytesReturned /* [out] [ref] */,
					uint32_t *_lpcbRequired /* [out] [ref] */,
					WERROR *_rpc_status /* [out] [ref] */,
					WERROR *result);

struct tevent_req *dcerpc_clusapi_NodeResourceTypeControl_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_NodeResourceTypeControl *r);
NTSTATUS dcerpc_clusapi_NodeResourceTypeControl_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_NodeResourceTypeControl_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_NodeResourceTypeControl *r);
struct tevent_req *dcerpc_clusapi_NodeResourceTypeControl_send(TALLOC_CTX *mem_ctx,
							       struct tevent_context *ev,
							       struct dcerpc_binding_handle *h,
							       struct policy_handle _hCluster /* [in]  */,
							       const char *_lpszResourceTypeName /* [in] [charset(UTF16),ref] */,
							       struct policy_handle _hNode /* [in]  */,
							       uint32_t _dwControlCode /* [in]  */,
							       uint8_t *_lpInBuffer /* [in] [size_is(nInBufferSize),unique] */,
							       uint32_t _nInBufferSize /* [in]  */,
							       uint8_t *_lpOutBuffer /* [out] [length_is(*lpBytesReturned),ref,size_is(nOutBufferSize)] */,
							       uint32_t _nOutBufferSize /* [in]  */,
							       uint32_t *_lpBytesReturned /* [out] [ref] */,
							       uint32_t *_lpcbRequired /* [out] [ref] */,
							       WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_NodeResourceTypeControl_recv(struct tevent_req *req,
						     TALLOC_CTX *mem_ctx,
						     WERROR *result);
NTSTATUS dcerpc_clusapi_NodeResourceTypeControl(struct dcerpc_binding_handle *h,
						TALLOC_CTX *mem_ctx,
						struct policy_handle _hCluster /* [in]  */,
						const char *_lpszResourceTypeName /* [in] [charset(UTF16),ref] */,
						struct policy_handle _hNode /* [in]  */,
						uint32_t _dwControlCode /* [in]  */,
						uint8_t *_lpInBuffer /* [in] [size_is(nInBufferSize),unique] */,
						uint32_t _nInBufferSize /* [in]  */,
						uint8_t *_lpOutBuffer /* [out] [length_is(*lpBytesReturned),ref,size_is(nOutBufferSize)] */,
						uint32_t _nOutBufferSize /* [in]  */,
						uint32_t *_lpBytesReturned /* [out] [ref] */,
						uint32_t *_lpcbRequired /* [out] [ref] */,
						WERROR *_rpc_status /* [out] [ref] */,
						WERROR *result);

struct tevent_req *dcerpc_clusapi_ResourceTypeControl_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_ResourceTypeControl *r);
NTSTATUS dcerpc_clusapi_ResourceTypeControl_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_ResourceTypeControl_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_ResourceTypeControl *r);
struct tevent_req *dcerpc_clusapi_ResourceTypeControl_send(TALLOC_CTX *mem_ctx,
							   struct tevent_context *ev,
							   struct dcerpc_binding_handle *h,
							   struct policy_handle _hCluster /* [in]  */,
							   const char *_lpszResourceTypeName /* [in] [charset(UTF16),ref] */,
							   enum clusapi_ResourceTypeControlCode _dwControlCode /* [in]  */,
							   uint8_t *_lpInBuffer /* [in] [size_is(nInBufferSize),unique] */,
							   uint32_t _nInBufferSize /* [in]  */,
							   uint8_t *_lpOutBuffer /* [out] [length_is(*lpBytesReturned),ref,size_is(nOutBufferSize)] */,
							   uint32_t _nOutBufferSize /* [in]  */,
							   uint32_t *_lpBytesReturned /* [out] [ref] */,
							   uint32_t *_lpcbRequired /* [out] [ref] */,
							   WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_ResourceTypeControl_recv(struct tevent_req *req,
						 TALLOC_CTX *mem_ctx,
						 WERROR *result);
NTSTATUS dcerpc_clusapi_ResourceTypeControl(struct dcerpc_binding_handle *h,
					    TALLOC_CTX *mem_ctx,
					    struct policy_handle _hCluster /* [in]  */,
					    const char *_lpszResourceTypeName /* [in] [charset(UTF16),ref] */,
					    enum clusapi_ResourceTypeControlCode _dwControlCode /* [in]  */,
					    uint8_t *_lpInBuffer /* [in] [size_is(nInBufferSize),unique] */,
					    uint32_t _nInBufferSize /* [in]  */,
					    uint8_t *_lpOutBuffer /* [out] [length_is(*lpBytesReturned),ref,size_is(nOutBufferSize)] */,
					    uint32_t _nOutBufferSize /* [in]  */,
					    uint32_t *_lpBytesReturned /* [out] [ref] */,
					    uint32_t *_lpcbRequired /* [out] [ref] */,
					    WERROR *_rpc_status /* [out] [ref] */,
					    WERROR *result);

struct tevent_req *dcerpc_clusapi_NodeGroupControl_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_NodeGroupControl *r);
NTSTATUS dcerpc_clusapi_NodeGroupControl_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_NodeGroupControl_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_NodeGroupControl *r);
struct tevent_req *dcerpc_clusapi_NodeGroupControl_send(TALLOC_CTX *mem_ctx,
							struct tevent_context *ev,
							struct dcerpc_binding_handle *h,
							struct policy_handle _hGroup /* [in]  */,
							struct policy_handle _hNode /* [in]  */,
							uint32_t _dwControlCode /* [in]  */,
							uint8_t *_lpInBuffer /* [in] [size_is(nInBufferSize),unique] */,
							uint32_t _nInBufferSize /* [in]  */,
							uint8_t *_lpOutBuffer /* [out] [length_is(*lpBytesReturned),ref,size_is(nOutBufferSize)] */,
							uint32_t _nOutBufferSize /* [in]  */,
							uint32_t *_lpBytesReturned /* [out] [ref] */,
							uint32_t *_lpcbRequired /* [out] [ref] */,
							WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_NodeGroupControl_recv(struct tevent_req *req,
					      TALLOC_CTX *mem_ctx,
					      WERROR *result);
NTSTATUS dcerpc_clusapi_NodeGroupControl(struct dcerpc_binding_handle *h,
					 TALLOC_CTX *mem_ctx,
					 struct policy_handle _hGroup /* [in]  */,
					 struct policy_handle _hNode /* [in]  */,
					 uint32_t _dwControlCode /* [in]  */,
					 uint8_t *_lpInBuffer /* [in] [size_is(nInBufferSize),unique] */,
					 uint32_t _nInBufferSize /* [in]  */,
					 uint8_t *_lpOutBuffer /* [out] [length_is(*lpBytesReturned),ref,size_is(nOutBufferSize)] */,
					 uint32_t _nOutBufferSize /* [in]  */,
					 uint32_t *_lpBytesReturned /* [out] [ref] */,
					 uint32_t *_lpcbRequired /* [out] [ref] */,
					 WERROR *_rpc_status /* [out] [ref] */,
					 WERROR *result);

struct tevent_req *dcerpc_clusapi_GroupControl_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_GroupControl *r);
NTSTATUS dcerpc_clusapi_GroupControl_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_GroupControl_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_GroupControl *r);
struct tevent_req *dcerpc_clusapi_GroupControl_send(TALLOC_CTX *mem_ctx,
						    struct tevent_context *ev,
						    struct dcerpc_binding_handle *h,
						    struct policy_handle _hGroup /* [in]  */,
						    enum clusapi_GroupControlCode _dwControlCode /* [in]  */,
						    uint8_t *_lpInBuffer /* [in] [size_is(nInBufferSize),unique] */,
						    uint32_t _nInBufferSize /* [in]  */,
						    uint8_t *_lpOutBuffer /* [out] [length_is(*lpBytesReturned),ref,size_is(nOutBufferSize)] */,
						    uint32_t _nOutBufferSize /* [in]  */,
						    uint32_t *_lpBytesReturned /* [out] [ref] */,
						    uint32_t *_lpcbRequired /* [out] [ref] */,
						    WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_GroupControl_recv(struct tevent_req *req,
					  TALLOC_CTX *mem_ctx,
					  WERROR *result);
NTSTATUS dcerpc_clusapi_GroupControl(struct dcerpc_binding_handle *h,
				     TALLOC_CTX *mem_ctx,
				     struct policy_handle _hGroup /* [in]  */,
				     enum clusapi_GroupControlCode _dwControlCode /* [in]  */,
				     uint8_t *_lpInBuffer /* [in] [size_is(nInBufferSize),unique] */,
				     uint32_t _nInBufferSize /* [in]  */,
				     uint8_t *_lpOutBuffer /* [out] [length_is(*lpBytesReturned),ref,size_is(nOutBufferSize)] */,
				     uint32_t _nOutBufferSize /* [in]  */,
				     uint32_t *_lpBytesReturned /* [out] [ref] */,
				     uint32_t *_lpcbRequired /* [out] [ref] */,
				     WERROR *_rpc_status /* [out] [ref] */,
				     WERROR *result);

struct tevent_req *dcerpc_clusapi_NodeNodeControl_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_NodeNodeControl *r);
NTSTATUS dcerpc_clusapi_NodeNodeControl_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_NodeNodeControl_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_NodeNodeControl *r);
struct tevent_req *dcerpc_clusapi_NodeNodeControl_send(TALLOC_CTX *mem_ctx,
						       struct tevent_context *ev,
						       struct dcerpc_binding_handle *h,
						       struct policy_handle _hNode /* [in]  */,
						       struct policy_handle _hHostNode /* [in]  */,
						       uint32_t _dwControlCode /* [in]  */,
						       uint8_t *_lpInBuffer /* [in] [size_is(nInBufferSize),unique] */,
						       uint32_t _nInBufferSize /* [in]  */,
						       uint8_t *_lpOutBuffer /* [out] [length_is(*lpBytesReturned),ref,size_is(nOutBufferSize)] */,
						       uint32_t _nOutBufferSize /* [in]  */,
						       uint32_t *_lpBytesReturned /* [out] [ref] */,
						       uint32_t *_lpcbRequired /* [out] [ref] */,
						       WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_NodeNodeControl_recv(struct tevent_req *req,
					     TALLOC_CTX *mem_ctx,
					     WERROR *result);
NTSTATUS dcerpc_clusapi_NodeNodeControl(struct dcerpc_binding_handle *h,
					TALLOC_CTX *mem_ctx,
					struct policy_handle _hNode /* [in]  */,
					struct policy_handle _hHostNode /* [in]  */,
					uint32_t _dwControlCode /* [in]  */,
					uint8_t *_lpInBuffer /* [in] [size_is(nInBufferSize),unique] */,
					uint32_t _nInBufferSize /* [in]  */,
					uint8_t *_lpOutBuffer /* [out] [length_is(*lpBytesReturned),ref,size_is(nOutBufferSize)] */,
					uint32_t _nOutBufferSize /* [in]  */,
					uint32_t *_lpBytesReturned /* [out] [ref] */,
					uint32_t *_lpcbRequired /* [out] [ref] */,
					WERROR *_rpc_status /* [out] [ref] */,
					WERROR *result);

struct tevent_req *dcerpc_clusapi_NodeControl_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_NodeControl *r);
NTSTATUS dcerpc_clusapi_NodeControl_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_NodeControl_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_NodeControl *r);
struct tevent_req *dcerpc_clusapi_NodeControl_send(TALLOC_CTX *mem_ctx,
						   struct tevent_context *ev,
						   struct dcerpc_binding_handle *h,
						   struct policy_handle _hNode /* [in]  */,
						   enum clusapi_NodeControlCode _dwControlCode /* [in]  */,
						   uint8_t *_lpInBuffer /* [in] [size_is(nInBufferSize),unique] */,
						   uint32_t _nInBufferSize /* [in]  */,
						   uint8_t *_lpOutBuffer /* [out] [length_is(*lpBytesReturned),ref,size_is(nOutBufferSize)] */,
						   uint32_t _nOutBufferSize /* [in]  */,
						   uint32_t *_lpBytesReturned /* [out] [ref] */,
						   uint32_t *_lpcbRequired /* [out] [ref] */,
						   WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_NodeControl_recv(struct tevent_req *req,
					 TALLOC_CTX *mem_ctx,
					 WERROR *result);
NTSTATUS dcerpc_clusapi_NodeControl(struct dcerpc_binding_handle *h,
				    TALLOC_CTX *mem_ctx,
				    struct policy_handle _hNode /* [in]  */,
				    enum clusapi_NodeControlCode _dwControlCode /* [in]  */,
				    uint8_t *_lpInBuffer /* [in] [size_is(nInBufferSize),unique] */,
				    uint32_t _nInBufferSize /* [in]  */,
				    uint8_t *_lpOutBuffer /* [out] [length_is(*lpBytesReturned),ref,size_is(nOutBufferSize)] */,
				    uint32_t _nOutBufferSize /* [in]  */,
				    uint32_t *_lpBytesReturned /* [out] [ref] */,
				    uint32_t *_lpcbRequired /* [out] [ref] */,
				    WERROR *_rpc_status /* [out] [ref] */,
				    WERROR *result);

struct tevent_req *dcerpc_Opnum80NotUsedOnWire_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct Opnum80NotUsedOnWire *r);
NTSTATUS dcerpc_Opnum80NotUsedOnWire_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_Opnum80NotUsedOnWire_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct Opnum80NotUsedOnWire *r);
struct tevent_req *dcerpc_Opnum80NotUsedOnWire_send(TALLOC_CTX *mem_ctx,
						    struct tevent_context *ev,
						    struct dcerpc_binding_handle *h);
NTSTATUS dcerpc_Opnum80NotUsedOnWire_recv(struct tevent_req *req,
					  TALLOC_CTX *mem_ctx,
					  WERROR *result);
NTSTATUS dcerpc_Opnum80NotUsedOnWire(struct dcerpc_binding_handle *h,
				     TALLOC_CTX *mem_ctx,
				     WERROR *result);

struct tevent_req *dcerpc_clusapi_OpenNetwork_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_OpenNetwork *r);
NTSTATUS dcerpc_clusapi_OpenNetwork_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_OpenNetwork_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_OpenNetwork *r);
struct tevent_req *dcerpc_clusapi_OpenNetwork_send(TALLOC_CTX *mem_ctx,
						   struct tevent_context *ev,
						   struct dcerpc_binding_handle *h,
						   const char *_lpszNetworkName /* [in] [charset(UTF16),ref] */,
						   WERROR *_Status /* [out] [ref] */,
						   WERROR *_rpc_status /* [out] [ref] */,
						   struct policy_handle *_hNetwork /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_OpenNetwork_recv(struct tevent_req *req,
					 TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_OpenNetwork(struct dcerpc_binding_handle *h,
				    TALLOC_CTX *mem_ctx,
				    const char *_lpszNetworkName /* [in] [charset(UTF16),ref] */,
				    WERROR *_Status /* [out] [ref] */,
				    WERROR *_rpc_status /* [out] [ref] */,
				    struct policy_handle *_hNetwork /* [out] [ref] */);

struct tevent_req *dcerpc_clusapi_CloseNetwork_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_CloseNetwork *r);
NTSTATUS dcerpc_clusapi_CloseNetwork_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_CloseNetwork_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_CloseNetwork *r);
struct tevent_req *dcerpc_clusapi_CloseNetwork_send(TALLOC_CTX *mem_ctx,
						    struct tevent_context *ev,
						    struct dcerpc_binding_handle *h,
						    struct policy_handle *_Network /* [in,out] [ref] */);
NTSTATUS dcerpc_clusapi_CloseNetwork_recv(struct tevent_req *req,
					  TALLOC_CTX *mem_ctx,
					  WERROR *result);
NTSTATUS dcerpc_clusapi_CloseNetwork(struct dcerpc_binding_handle *h,
				     TALLOC_CTX *mem_ctx,
				     struct policy_handle *_Network /* [in,out] [ref] */,
				     WERROR *result);

struct tevent_req *dcerpc_clusapi_GetNetworkState_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_GetNetworkState *r);
NTSTATUS dcerpc_clusapi_GetNetworkState_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_GetNetworkState_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_GetNetworkState *r);
struct tevent_req *dcerpc_clusapi_GetNetworkState_send(TALLOC_CTX *mem_ctx,
						       struct tevent_context *ev,
						       struct dcerpc_binding_handle *h,
						       struct policy_handle _hNetwork /* [in]  */,
						       enum clusapi_ClusterNetworkState *_State /* [out] [ref] */,
						       WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_GetNetworkState_recv(struct tevent_req *req,
					     TALLOC_CTX *mem_ctx,
					     WERROR *result);
NTSTATUS dcerpc_clusapi_GetNetworkState(struct dcerpc_binding_handle *h,
					TALLOC_CTX *mem_ctx,
					struct policy_handle _hNetwork /* [in]  */,
					enum clusapi_ClusterNetworkState *_State /* [out] [ref] */,
					WERROR *_rpc_status /* [out] [ref] */,
					WERROR *result);

struct tevent_req *dcerpc_clusapi_SetNetworkName_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_SetNetworkName *r);
NTSTATUS dcerpc_clusapi_SetNetworkName_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_SetNetworkName_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_SetNetworkName *r);
struct tevent_req *dcerpc_clusapi_SetNetworkName_send(TALLOC_CTX *mem_ctx,
						      struct tevent_context *ev,
						      struct dcerpc_binding_handle *h,
						      struct policy_handle _hNetwork /* [in]  */,
						      const char *_lpszNetworkName /* [in] [charset(UTF16),ref] */,
						      WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_SetNetworkName_recv(struct tevent_req *req,
					    TALLOC_CTX *mem_ctx,
					    WERROR *result);
NTSTATUS dcerpc_clusapi_SetNetworkName(struct dcerpc_binding_handle *h,
				       TALLOC_CTX *mem_ctx,
				       struct policy_handle _hNetwork /* [in]  */,
				       const char *_lpszNetworkName /* [in] [charset(UTF16),ref] */,
				       WERROR *_rpc_status /* [out] [ref] */,
				       WERROR *result);

struct tevent_req *dcerpc_clusapi_CreateNetworkEnum_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_CreateNetworkEnum *r);
NTSTATUS dcerpc_clusapi_CreateNetworkEnum_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_CreateNetworkEnum_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_CreateNetworkEnum *r);
struct tevent_req *dcerpc_clusapi_CreateNetworkEnum_send(TALLOC_CTX *mem_ctx,
							 struct tevent_context *ev,
							 struct dcerpc_binding_handle *h,
							 struct policy_handle _hNetwork /* [in]  */,
							 uint32_t _dwType /* [in]  */,
							 struct ENUM_LIST **_ReturnEnum /* [out] [ref] */,
							 WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_CreateNetworkEnum_recv(struct tevent_req *req,
					       TALLOC_CTX *mem_ctx,
					       WERROR *result);
NTSTATUS dcerpc_clusapi_CreateNetworkEnum(struct dcerpc_binding_handle *h,
					  TALLOC_CTX *mem_ctx,
					  struct policy_handle _hNetwork /* [in]  */,
					  uint32_t _dwType /* [in]  */,
					  struct ENUM_LIST **_ReturnEnum /* [out] [ref] */,
					  WERROR *_rpc_status /* [out] [ref] */,
					  WERROR *result);

struct tevent_req *dcerpc_clusapi_GetNetworkId_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_GetNetworkId *r);
NTSTATUS dcerpc_clusapi_GetNetworkId_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_GetNetworkId_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_GetNetworkId *r);
struct tevent_req *dcerpc_clusapi_GetNetworkId_send(TALLOC_CTX *mem_ctx,
						    struct tevent_context *ev,
						    struct dcerpc_binding_handle *h,
						    struct policy_handle _hNetwork /* [in]  */,
						    const char **_pGuid /* [out] [charset(UTF16),ref] */,
						    WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_GetNetworkId_recv(struct tevent_req *req,
					  TALLOC_CTX *mem_ctx,
					  WERROR *result);
NTSTATUS dcerpc_clusapi_GetNetworkId(struct dcerpc_binding_handle *h,
				     TALLOC_CTX *mem_ctx,
				     struct policy_handle _hNetwork /* [in]  */,
				     const char **_pGuid /* [out] [charset(UTF16),ref] */,
				     WERROR *_rpc_status /* [out] [ref] */,
				     WERROR *result);

struct tevent_req *dcerpc_clusapi_SetNetworkPriorityOrder_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_SetNetworkPriorityOrder *r);
NTSTATUS dcerpc_clusapi_SetNetworkPriorityOrder_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_SetNetworkPriorityOrder_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_SetNetworkPriorityOrder *r);
struct tevent_req *dcerpc_clusapi_SetNetworkPriorityOrder_send(TALLOC_CTX *mem_ctx,
							       struct tevent_context *ev,
							       struct dcerpc_binding_handle *h,
							       uint32_t _NetworkCount /* [in] [range(0,1000)] */,
							       const char *_NetworkIdList /* [in] [charset(UTF16),size_is(NetworkCount)] */,
							       WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_SetNetworkPriorityOrder_recv(struct tevent_req *req,
						     TALLOC_CTX *mem_ctx,
						     WERROR *result);
NTSTATUS dcerpc_clusapi_SetNetworkPriorityOrder(struct dcerpc_binding_handle *h,
						TALLOC_CTX *mem_ctx,
						uint32_t _NetworkCount /* [in] [range(0,1000)] */,
						const char *_NetworkIdList /* [in] [charset(UTF16),size_is(NetworkCount)] */,
						WERROR *_rpc_status /* [out] [ref] */,
						WERROR *result);

struct tevent_req *dcerpc_clusapi_NodeNetworkControl_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_NodeNetworkControl *r);
NTSTATUS dcerpc_clusapi_NodeNetworkControl_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_NodeNetworkControl_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_NodeNetworkControl *r);
struct tevent_req *dcerpc_clusapi_NodeNetworkControl_send(TALLOC_CTX *mem_ctx,
							  struct tevent_context *ev,
							  struct dcerpc_binding_handle *h,
							  struct policy_handle _hNetwork /* [in]  */,
							  struct policy_handle _hNode /* [in]  */,
							  uint32_t _dwControlCode /* [in]  */,
							  uint8_t *_lpInBuffer /* [in] [size_is(nInBufferSize),unique] */,
							  uint32_t _nInBufferSize /* [in]  */,
							  uint8_t *_lpOutBuffer /* [out] [length_is(*lpBytesReturned),ref,size_is(nOutBufferSize)] */,
							  uint32_t _nOutBufferSize /* [in]  */,
							  uint32_t *_lpBytesReturned /* [out] [ref] */,
							  uint32_t *_lpcbRequired /* [out] [ref] */,
							  WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_NodeNetworkControl_recv(struct tevent_req *req,
						TALLOC_CTX *mem_ctx,
						WERROR *result);
NTSTATUS dcerpc_clusapi_NodeNetworkControl(struct dcerpc_binding_handle *h,
					   TALLOC_CTX *mem_ctx,
					   struct policy_handle _hNetwork /* [in]  */,
					   struct policy_handle _hNode /* [in]  */,
					   uint32_t _dwControlCode /* [in]  */,
					   uint8_t *_lpInBuffer /* [in] [size_is(nInBufferSize),unique] */,
					   uint32_t _nInBufferSize /* [in]  */,
					   uint8_t *_lpOutBuffer /* [out] [length_is(*lpBytesReturned),ref,size_is(nOutBufferSize)] */,
					   uint32_t _nOutBufferSize /* [in]  */,
					   uint32_t *_lpBytesReturned /* [out] [ref] */,
					   uint32_t *_lpcbRequired /* [out] [ref] */,
					   WERROR *_rpc_status /* [out] [ref] */,
					   WERROR *result);

struct tevent_req *dcerpc_clusapi_NetworkControl_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_NetworkControl *r);
NTSTATUS dcerpc_clusapi_NetworkControl_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_NetworkControl_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_NetworkControl *r);
struct tevent_req *dcerpc_clusapi_NetworkControl_send(TALLOC_CTX *mem_ctx,
						      struct tevent_context *ev,
						      struct dcerpc_binding_handle *h,
						      struct policy_handle _hNetwork /* [in]  */,
						      uint32_t _dwControlCode /* [in]  */,
						      uint8_t *_lpInBuffer /* [in] [size_is(nInBufferSize),unique] */,
						      uint32_t _nInBufferSize /* [in]  */,
						      uint8_t *_lpOutBuffer /* [out] [length_is(*lpBytesReturned),ref,size_is(nOutBufferSize)] */,
						      uint32_t _nOutBufferSize /* [in]  */,
						      uint32_t *_lpBytesReturned /* [out] [ref] */,
						      uint32_t *_lpcbRequired /* [out] [ref] */,
						      WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_NetworkControl_recv(struct tevent_req *req,
					    TALLOC_CTX *mem_ctx,
					    WERROR *result);
NTSTATUS dcerpc_clusapi_NetworkControl(struct dcerpc_binding_handle *h,
				       TALLOC_CTX *mem_ctx,
				       struct policy_handle _hNetwork /* [in]  */,
				       uint32_t _dwControlCode /* [in]  */,
				       uint8_t *_lpInBuffer /* [in] [size_is(nInBufferSize),unique] */,
				       uint32_t _nInBufferSize /* [in]  */,
				       uint8_t *_lpOutBuffer /* [out] [length_is(*lpBytesReturned),ref,size_is(nOutBufferSize)] */,
				       uint32_t _nOutBufferSize /* [in]  */,
				       uint32_t *_lpBytesReturned /* [out] [ref] */,
				       uint32_t *_lpcbRequired /* [out] [ref] */,
				       WERROR *_rpc_status /* [out] [ref] */,
				       WERROR *result);

struct tevent_req *dcerpc_clusapi_AddNotifyNetwork_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_AddNotifyNetwork *r);
NTSTATUS dcerpc_clusapi_AddNotifyNetwork_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_AddNotifyNetwork_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_AddNotifyNetwork *r);
struct tevent_req *dcerpc_clusapi_AddNotifyNetwork_send(TALLOC_CTX *mem_ctx,
							struct tevent_context *ev,
							struct dcerpc_binding_handle *h,
							struct policy_handle _hNotify /* [in]  */,
							struct policy_handle _hNetwork /* [in]  */,
							uint32_t _dwFilter /* [in]  */,
							uint32_t _dwNotifyKey /* [in]  */,
							uint32_t *_dwStateSequence /* [out] [ref] */,
							WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_AddNotifyNetwork_recv(struct tevent_req *req,
					      TALLOC_CTX *mem_ctx,
					      WERROR *result);
NTSTATUS dcerpc_clusapi_AddNotifyNetwork(struct dcerpc_binding_handle *h,
					 TALLOC_CTX *mem_ctx,
					 struct policy_handle _hNotify /* [in]  */,
					 struct policy_handle _hNetwork /* [in]  */,
					 uint32_t _dwFilter /* [in]  */,
					 uint32_t _dwNotifyKey /* [in]  */,
					 uint32_t *_dwStateSequence /* [out] [ref] */,
					 WERROR *_rpc_status /* [out] [ref] */,
					 WERROR *result);

struct tevent_req *dcerpc_clusapi_ReAddNotifyNetwork_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_ReAddNotifyNetwork *r);
NTSTATUS dcerpc_clusapi_ReAddNotifyNetwork_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_ReAddNotifyNetwork_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_ReAddNotifyNetwork *r);
struct tevent_req *dcerpc_clusapi_ReAddNotifyNetwork_send(TALLOC_CTX *mem_ctx,
							  struct tevent_context *ev,
							  struct dcerpc_binding_handle *h,
							  struct policy_handle _hNotify /* [in]  */,
							  struct policy_handle _hNetwork /* [in]  */,
							  uint32_t _dwFilter /* [in]  */,
							  uint32_t _dwNotifyKey /* [in]  */,
							  uint32_t _StateSequence /* [in]  */,
							  WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_ReAddNotifyNetwork_recv(struct tevent_req *req,
						TALLOC_CTX *mem_ctx,
						WERROR *result);
NTSTATUS dcerpc_clusapi_ReAddNotifyNetwork(struct dcerpc_binding_handle *h,
					   TALLOC_CTX *mem_ctx,
					   struct policy_handle _hNotify /* [in]  */,
					   struct policy_handle _hNetwork /* [in]  */,
					   uint32_t _dwFilter /* [in]  */,
					   uint32_t _dwNotifyKey /* [in]  */,
					   uint32_t _StateSequence /* [in]  */,
					   WERROR *_rpc_status /* [out] [ref] */,
					   WERROR *result);

struct tevent_req *dcerpc_clusapi_OpenNetInterface_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_OpenNetInterface *r);
NTSTATUS dcerpc_clusapi_OpenNetInterface_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_OpenNetInterface_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_OpenNetInterface *r);
struct tevent_req *dcerpc_clusapi_OpenNetInterface_send(TALLOC_CTX *mem_ctx,
							struct tevent_context *ev,
							struct dcerpc_binding_handle *h,
							const char *_lpszNetInterfaceName /* [in] [charset(UTF16),ref] */,
							WERROR *_Status /* [out] [ref] */,
							WERROR *_rpc_status /* [out] [ref] */,
							struct policy_handle *_hNetInterface /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_OpenNetInterface_recv(struct tevent_req *req,
					      TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_OpenNetInterface(struct dcerpc_binding_handle *h,
					 TALLOC_CTX *mem_ctx,
					 const char *_lpszNetInterfaceName /* [in] [charset(UTF16),ref] */,
					 WERROR *_Status /* [out] [ref] */,
					 WERROR *_rpc_status /* [out] [ref] */,
					 struct policy_handle *_hNetInterface /* [out] [ref] */);

struct tevent_req *dcerpc_clusapi_CloseNetInterface_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_CloseNetInterface *r);
NTSTATUS dcerpc_clusapi_CloseNetInterface_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_CloseNetInterface_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_CloseNetInterface *r);
struct tevent_req *dcerpc_clusapi_CloseNetInterface_send(TALLOC_CTX *mem_ctx,
							 struct tevent_context *ev,
							 struct dcerpc_binding_handle *h,
							 struct policy_handle *_NetInterface /* [in,out] [ref] */);
NTSTATUS dcerpc_clusapi_CloseNetInterface_recv(struct tevent_req *req,
					       TALLOC_CTX *mem_ctx,
					       WERROR *result);
NTSTATUS dcerpc_clusapi_CloseNetInterface(struct dcerpc_binding_handle *h,
					  TALLOC_CTX *mem_ctx,
					  struct policy_handle *_NetInterface /* [in,out] [ref] */,
					  WERROR *result);

struct tevent_req *dcerpc_clusapi_GetNetInterfaceState_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_GetNetInterfaceState *r);
NTSTATUS dcerpc_clusapi_GetNetInterfaceState_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_GetNetInterfaceState_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_GetNetInterfaceState *r);
struct tevent_req *dcerpc_clusapi_GetNetInterfaceState_send(TALLOC_CTX *mem_ctx,
							    struct tevent_context *ev,
							    struct dcerpc_binding_handle *h,
							    struct policy_handle _hNetInterface /* [in]  */,
							    enum clusapi_ClusterNetInterfaceState *_State /* [out] [ref] */,
							    WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_GetNetInterfaceState_recv(struct tevent_req *req,
						  TALLOC_CTX *mem_ctx,
						  WERROR *result);
NTSTATUS dcerpc_clusapi_GetNetInterfaceState(struct dcerpc_binding_handle *h,
					     TALLOC_CTX *mem_ctx,
					     struct policy_handle _hNetInterface /* [in]  */,
					     enum clusapi_ClusterNetInterfaceState *_State /* [out] [ref] */,
					     WERROR *_rpc_status /* [out] [ref] */,
					     WERROR *result);

struct tevent_req *dcerpc_clusapi_GetNetInterface_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_GetNetInterface *r);
NTSTATUS dcerpc_clusapi_GetNetInterface_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_GetNetInterface_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_GetNetInterface *r);
struct tevent_req *dcerpc_clusapi_GetNetInterface_send(TALLOC_CTX *mem_ctx,
						       struct tevent_context *ev,
						       struct dcerpc_binding_handle *h,
						       const char *_lpszNodeName /* [in] [charset(UTF16),ref] */,
						       const char *_lpszNetworkName /* [in] [charset(UTF16),ref] */,
						       const char **_lppszInterfaceName /* [out] [charset(UTF16),ref] */,
						       WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_GetNetInterface_recv(struct tevent_req *req,
					     TALLOC_CTX *mem_ctx,
					     WERROR *result);
NTSTATUS dcerpc_clusapi_GetNetInterface(struct dcerpc_binding_handle *h,
					TALLOC_CTX *mem_ctx,
					const char *_lpszNodeName /* [in] [charset(UTF16),ref] */,
					const char *_lpszNetworkName /* [in] [charset(UTF16),ref] */,
					const char **_lppszInterfaceName /* [out] [charset(UTF16),ref] */,
					WERROR *_rpc_status /* [out] [ref] */,
					WERROR *result);

struct tevent_req *dcerpc_clusapi_GetNetInterfaceId_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_GetNetInterfaceId *r);
NTSTATUS dcerpc_clusapi_GetNetInterfaceId_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_GetNetInterfaceId_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_GetNetInterfaceId *r);
struct tevent_req *dcerpc_clusapi_GetNetInterfaceId_send(TALLOC_CTX *mem_ctx,
							 struct tevent_context *ev,
							 struct dcerpc_binding_handle *h,
							 struct policy_handle _hNetInterface /* [in]  */,
							 const char **_pGuid /* [out] [charset(UTF16),ref] */,
							 WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_GetNetInterfaceId_recv(struct tevent_req *req,
					       TALLOC_CTX *mem_ctx,
					       WERROR *result);
NTSTATUS dcerpc_clusapi_GetNetInterfaceId(struct dcerpc_binding_handle *h,
					  TALLOC_CTX *mem_ctx,
					  struct policy_handle _hNetInterface /* [in]  */,
					  const char **_pGuid /* [out] [charset(UTF16),ref] */,
					  WERROR *_rpc_status /* [out] [ref] */,
					  WERROR *result);

struct tevent_req *dcerpc_clusapi_NodeNetInterfaceControl_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_NodeNetInterfaceControl *r);
NTSTATUS dcerpc_clusapi_NodeNetInterfaceControl_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_NodeNetInterfaceControl_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_NodeNetInterfaceControl *r);
struct tevent_req *dcerpc_clusapi_NodeNetInterfaceControl_send(TALLOC_CTX *mem_ctx,
							       struct tevent_context *ev,
							       struct dcerpc_binding_handle *h,
							       struct policy_handle _hNetInterface /* [in]  */,
							       struct policy_handle _hNode /* [in]  */,
							       uint32_t _dwControlCode /* [in]  */,
							       uint8_t *_lpInBuffer /* [in] [size_is(nInBufferSize),unique] */,
							       uint32_t _nInBufferSize /* [in]  */,
							       uint8_t *_lpOutBuffer /* [out] [length_is(*lpBytesReturned),ref,size_is(nOutBufferSize)] */,
							       uint32_t _nOutBufferSize /* [in]  */,
							       uint32_t *_lpBytesReturned /* [out] [ref] */,
							       uint32_t *_lpcbRequired /* [out] [ref] */,
							       WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_NodeNetInterfaceControl_recv(struct tevent_req *req,
						     TALLOC_CTX *mem_ctx,
						     WERROR *result);
NTSTATUS dcerpc_clusapi_NodeNetInterfaceControl(struct dcerpc_binding_handle *h,
						TALLOC_CTX *mem_ctx,
						struct policy_handle _hNetInterface /* [in]  */,
						struct policy_handle _hNode /* [in]  */,
						uint32_t _dwControlCode /* [in]  */,
						uint8_t *_lpInBuffer /* [in] [size_is(nInBufferSize),unique] */,
						uint32_t _nInBufferSize /* [in]  */,
						uint8_t *_lpOutBuffer /* [out] [length_is(*lpBytesReturned),ref,size_is(nOutBufferSize)] */,
						uint32_t _nOutBufferSize /* [in]  */,
						uint32_t *_lpBytesReturned /* [out] [ref] */,
						uint32_t *_lpcbRequired /* [out] [ref] */,
						WERROR *_rpc_status /* [out] [ref] */,
						WERROR *result);

struct tevent_req *dcerpc_clusapi_NetInterfaceControl_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_NetInterfaceControl *r);
NTSTATUS dcerpc_clusapi_NetInterfaceControl_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_NetInterfaceControl_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_NetInterfaceControl *r);
struct tevent_req *dcerpc_clusapi_NetInterfaceControl_send(TALLOC_CTX *mem_ctx,
							   struct tevent_context *ev,
							   struct dcerpc_binding_handle *h,
							   struct policy_handle _hNetInterface /* [in]  */,
							   uint32_t _dwControlCode /* [in]  */,
							   uint8_t *_lpInBuffer /* [in] [size_is(nInBufferSize),unique] */,
							   uint32_t _nInBufferSize /* [in]  */,
							   uint8_t *_lpOutBuffer /* [out] [length_is(*lpBytesReturned),ref,size_is(nOutBufferSize)] */,
							   uint32_t _nOutBufferSize /* [in]  */,
							   uint32_t *_lpBytesReturned /* [out] [ref] */,
							   uint32_t *_lpcbRequired /* [out] [ref] */,
							   WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_NetInterfaceControl_recv(struct tevent_req *req,
						 TALLOC_CTX *mem_ctx,
						 WERROR *result);
NTSTATUS dcerpc_clusapi_NetInterfaceControl(struct dcerpc_binding_handle *h,
					    TALLOC_CTX *mem_ctx,
					    struct policy_handle _hNetInterface /* [in]  */,
					    uint32_t _dwControlCode /* [in]  */,
					    uint8_t *_lpInBuffer /* [in] [size_is(nInBufferSize),unique] */,
					    uint32_t _nInBufferSize /* [in]  */,
					    uint8_t *_lpOutBuffer /* [out] [length_is(*lpBytesReturned),ref,size_is(nOutBufferSize)] */,
					    uint32_t _nOutBufferSize /* [in]  */,
					    uint32_t *_lpBytesReturned /* [out] [ref] */,
					    uint32_t *_lpcbRequired /* [out] [ref] */,
					    WERROR *_rpc_status /* [out] [ref] */,
					    WERROR *result);

struct tevent_req *dcerpc_clusapi_AddNotifyNetInterface_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_AddNotifyNetInterface *r);
NTSTATUS dcerpc_clusapi_AddNotifyNetInterface_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_AddNotifyNetInterface_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_AddNotifyNetInterface *r);
struct tevent_req *dcerpc_clusapi_AddNotifyNetInterface_send(TALLOC_CTX *mem_ctx,
							     struct tevent_context *ev,
							     struct dcerpc_binding_handle *h,
							     struct policy_handle _hNotify /* [in]  */,
							     struct policy_handle _hNetInterface /* [in]  */,
							     uint32_t _dwFilter /* [in]  */,
							     uint32_t _dwNotifyKey /* [in]  */,
							     uint32_t *_dwStateSequence /* [out] [ref] */,
							     WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_AddNotifyNetInterface_recv(struct tevent_req *req,
						   TALLOC_CTX *mem_ctx,
						   WERROR *result);
NTSTATUS dcerpc_clusapi_AddNotifyNetInterface(struct dcerpc_binding_handle *h,
					      TALLOC_CTX *mem_ctx,
					      struct policy_handle _hNotify /* [in]  */,
					      struct policy_handle _hNetInterface /* [in]  */,
					      uint32_t _dwFilter /* [in]  */,
					      uint32_t _dwNotifyKey /* [in]  */,
					      uint32_t *_dwStateSequence /* [out] [ref] */,
					      WERROR *_rpc_status /* [out] [ref] */,
					      WERROR *result);

struct tevent_req *dcerpc_clusapi_ReAddNotifyNetInterface_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_ReAddNotifyNetInterface *r);
NTSTATUS dcerpc_clusapi_ReAddNotifyNetInterface_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_ReAddNotifyNetInterface_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_ReAddNotifyNetInterface *r);
struct tevent_req *dcerpc_clusapi_ReAddNotifyNetInterface_send(TALLOC_CTX *mem_ctx,
							       struct tevent_context *ev,
							       struct dcerpc_binding_handle *h,
							       struct policy_handle _hNotify /* [in]  */,
							       struct policy_handle _hNetInterface /* [in]  */,
							       uint32_t _dwFilter /* [in]  */,
							       uint32_t _dwNotifyKey /* [in]  */,
							       uint32_t _StateSequence /* [in]  */,
							       WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_ReAddNotifyNetInterface_recv(struct tevent_req *req,
						     TALLOC_CTX *mem_ctx,
						     WERROR *result);
NTSTATUS dcerpc_clusapi_ReAddNotifyNetInterface(struct dcerpc_binding_handle *h,
						TALLOC_CTX *mem_ctx,
						struct policy_handle _hNotify /* [in]  */,
						struct policy_handle _hNetInterface /* [in]  */,
						uint32_t _dwFilter /* [in]  */,
						uint32_t _dwNotifyKey /* [in]  */,
						uint32_t _StateSequence /* [in]  */,
						WERROR *_rpc_status /* [out] [ref] */,
						WERROR *result);

struct tevent_req *dcerpc_clusapi_CreateNodeEnum_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_CreateNodeEnum *r);
NTSTATUS dcerpc_clusapi_CreateNodeEnum_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_CreateNodeEnum_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_CreateNodeEnum *r);
struct tevent_req *dcerpc_clusapi_CreateNodeEnum_send(TALLOC_CTX *mem_ctx,
						      struct tevent_context *ev,
						      struct dcerpc_binding_handle *h,
						      struct policy_handle _hNode /* [in]  */,
						      uint32_t _dwType /* [in]  */,
						      struct ENUM_LIST **_ReturnEnum /* [out] [ref] */,
						      WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_CreateNodeEnum_recv(struct tevent_req *req,
					    TALLOC_CTX *mem_ctx,
					    WERROR *result);
NTSTATUS dcerpc_clusapi_CreateNodeEnum(struct dcerpc_binding_handle *h,
				       TALLOC_CTX *mem_ctx,
				       struct policy_handle _hNode /* [in]  */,
				       uint32_t _dwType /* [in]  */,
				       struct ENUM_LIST **_ReturnEnum /* [out] [ref] */,
				       WERROR *_rpc_status /* [out] [ref] */,
				       WERROR *result);

struct tevent_req *dcerpc_clusapi_GetClusterVersion2_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_GetClusterVersion2 *r);
NTSTATUS dcerpc_clusapi_GetClusterVersion2_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_GetClusterVersion2_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_GetClusterVersion2 *r);
struct tevent_req *dcerpc_clusapi_GetClusterVersion2_send(TALLOC_CTX *mem_ctx,
							  struct tevent_context *ev,
							  struct dcerpc_binding_handle *h,
							  uint16_t *_lpwMajorVersion /* [out] [ref] */,
							  uint16_t *_lpwMinorVersion /* [out] [ref] */,
							  uint16_t *_lpwBuildNumber /* [out] [ref] */,
							  const char **_lpszVendorId /* [out] [charset(UTF16),ref] */,
							  const char **_lpszCSDVersion /* [out] [charset(UTF16),ref] */,
							  struct CLUSTER_OPERATIONAL_VERSION_INFO **_ppClusterOpVerInfo /* [out] [ref] */,
							  WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_GetClusterVersion2_recv(struct tevent_req *req,
						TALLOC_CTX *mem_ctx,
						WERROR *result);
NTSTATUS dcerpc_clusapi_GetClusterVersion2(struct dcerpc_binding_handle *h,
					   TALLOC_CTX *mem_ctx,
					   uint16_t *_lpwMajorVersion /* [out] [ref] */,
					   uint16_t *_lpwMinorVersion /* [out] [ref] */,
					   uint16_t *_lpwBuildNumber /* [out] [ref] */,
					   const char **_lpszVendorId /* [out] [charset(UTF16),ref] */,
					   const char **_lpszCSDVersion /* [out] [charset(UTF16),ref] */,
					   struct CLUSTER_OPERATIONAL_VERSION_INFO **_ppClusterOpVerInfo /* [out] [ref] */,
					   WERROR *_rpc_status /* [out] [ref] */,
					   WERROR *result);

struct tevent_req *dcerpc_clusapi_CreateResTypeEnum_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_CreateResTypeEnum *r);
NTSTATUS dcerpc_clusapi_CreateResTypeEnum_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_CreateResTypeEnum_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_CreateResTypeEnum *r);
struct tevent_req *dcerpc_clusapi_CreateResTypeEnum_send(TALLOC_CTX *mem_ctx,
							 struct tevent_context *ev,
							 struct dcerpc_binding_handle *h,
							 const char *_lpszTypeName /* [in] [charset(UTF16),ref] */,
							 uint32_t _dwType /* [in]  */,
							 struct ENUM_LIST **_ReturnEnum /* [out] [ref] */,
							 WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_CreateResTypeEnum_recv(struct tevent_req *req,
					       TALLOC_CTX *mem_ctx,
					       WERROR *result);
NTSTATUS dcerpc_clusapi_CreateResTypeEnum(struct dcerpc_binding_handle *h,
					  TALLOC_CTX *mem_ctx,
					  const char *_lpszTypeName /* [in] [charset(UTF16),ref] */,
					  uint32_t _dwType /* [in]  */,
					  struct ENUM_LIST **_ReturnEnum /* [out] [ref] */,
					  WERROR *_rpc_status /* [out] [ref] */,
					  WERROR *result);

struct tevent_req *dcerpc_clusapi_BackupClusterDatabase_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_BackupClusterDatabase *r);
NTSTATUS dcerpc_clusapi_BackupClusterDatabase_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_BackupClusterDatabase_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_BackupClusterDatabase *r);
struct tevent_req *dcerpc_clusapi_BackupClusterDatabase_send(TALLOC_CTX *mem_ctx,
							     struct tevent_context *ev,
							     struct dcerpc_binding_handle *h,
							     const char *_lpszPathName /* [in] [charset(UTF16),ref] */,
							     WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_BackupClusterDatabase_recv(struct tevent_req *req,
						   TALLOC_CTX *mem_ctx,
						   WERROR *result);
NTSTATUS dcerpc_clusapi_BackupClusterDatabase(struct dcerpc_binding_handle *h,
					      TALLOC_CTX *mem_ctx,
					      const char *_lpszPathName /* [in] [charset(UTF16),ref] */,
					      WERROR *_rpc_status /* [out] [ref] */,
					      WERROR *result);

struct tevent_req *dcerpc_clusapi_NodeClusterControl_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_NodeClusterControl *r);
NTSTATUS dcerpc_clusapi_NodeClusterControl_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_NodeClusterControl_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_NodeClusterControl *r);
struct tevent_req *dcerpc_clusapi_NodeClusterControl_send(TALLOC_CTX *mem_ctx,
							  struct tevent_context *ev,
							  struct dcerpc_binding_handle *h,
							  struct policy_handle _hCluster /* [in]  */,
							  struct policy_handle _hHostNode /* [in]  */,
							  uint32_t _dwControlCode /* [in]  */,
							  uint8_t *_lpInBuffer /* [in] [size_is(nInBufferSize),unique] */,
							  uint32_t _nInBufferSize /* [in]  */,
							  uint8_t *_lpOutBuffer /* [out] [length_is(*lpBytesReturned),ref,size_is(nOutBufferSize)] */,
							  uint32_t _nOutBufferSize /* [in]  */,
							  uint32_t *_lpBytesReturned /* [out] [ref] */,
							  uint32_t *_lpcbRequired /* [out] [ref] */,
							  WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_NodeClusterControl_recv(struct tevent_req *req,
						TALLOC_CTX *mem_ctx,
						WERROR *result);
NTSTATUS dcerpc_clusapi_NodeClusterControl(struct dcerpc_binding_handle *h,
					   TALLOC_CTX *mem_ctx,
					   struct policy_handle _hCluster /* [in]  */,
					   struct policy_handle _hHostNode /* [in]  */,
					   uint32_t _dwControlCode /* [in]  */,
					   uint8_t *_lpInBuffer /* [in] [size_is(nInBufferSize),unique] */,
					   uint32_t _nInBufferSize /* [in]  */,
					   uint8_t *_lpOutBuffer /* [out] [length_is(*lpBytesReturned),ref,size_is(nOutBufferSize)] */,
					   uint32_t _nOutBufferSize /* [in]  */,
					   uint32_t *_lpBytesReturned /* [out] [ref] */,
					   uint32_t *_lpcbRequired /* [out] [ref] */,
					   WERROR *_rpc_status /* [out] [ref] */,
					   WERROR *result);

struct tevent_req *dcerpc_clusapi_ClusterControl_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_ClusterControl *r);
NTSTATUS dcerpc_clusapi_ClusterControl_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_ClusterControl_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_ClusterControl *r);
struct tevent_req *dcerpc_clusapi_ClusterControl_send(TALLOC_CTX *mem_ctx,
						      struct tevent_context *ev,
						      struct dcerpc_binding_handle *h,
						      struct policy_handle _hCluster /* [in]  */,
						      enum clusapi_ClusterControlCode _dwControlCode /* [in]  */,
						      uint8_t *_lpInBuffer /* [in] [size_is(nInBufferSize),unique] */,
						      uint32_t _nInBufferSize /* [in]  */,
						      uint8_t *_lpOutBuffer /* [out] [length_is(*lpBytesReturned),ref,size_is(nOutBufferSize)] */,
						      uint32_t _nOutBufferSize /* [in]  */,
						      uint32_t *_lpBytesReturned /* [out] [ref] */,
						      uint32_t *_lpcbRequired /* [out] [ref] */,
						      WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_ClusterControl_recv(struct tevent_req *req,
					    TALLOC_CTX *mem_ctx,
					    WERROR *result);
NTSTATUS dcerpc_clusapi_ClusterControl(struct dcerpc_binding_handle *h,
				       TALLOC_CTX *mem_ctx,
				       struct policy_handle _hCluster /* [in]  */,
				       enum clusapi_ClusterControlCode _dwControlCode /* [in]  */,
				       uint8_t *_lpInBuffer /* [in] [size_is(nInBufferSize),unique] */,
				       uint32_t _nInBufferSize /* [in]  */,
				       uint8_t *_lpOutBuffer /* [out] [length_is(*lpBytesReturned),ref,size_is(nOutBufferSize)] */,
				       uint32_t _nOutBufferSize /* [in]  */,
				       uint32_t *_lpBytesReturned /* [out] [ref] */,
				       uint32_t *_lpcbRequired /* [out] [ref] */,
				       WERROR *_rpc_status /* [out] [ref] */,
				       WERROR *result);

struct tevent_req *dcerpc_clusapi_UnblockGetNotifyCall_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_UnblockGetNotifyCall *r);
NTSTATUS dcerpc_clusapi_UnblockGetNotifyCall_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_UnblockGetNotifyCall_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_UnblockGetNotifyCall *r);
struct tevent_req *dcerpc_clusapi_UnblockGetNotifyCall_send(TALLOC_CTX *mem_ctx,
							    struct tevent_context *ev,
							    struct dcerpc_binding_handle *h,
							    struct policy_handle _hNotify /* [in]  */);
NTSTATUS dcerpc_clusapi_UnblockGetNotifyCall_recv(struct tevent_req *req,
						  TALLOC_CTX *mem_ctx,
						  WERROR *result);
NTSTATUS dcerpc_clusapi_UnblockGetNotifyCall(struct dcerpc_binding_handle *h,
					     TALLOC_CTX *mem_ctx,
					     struct policy_handle _hNotify /* [in]  */,
					     WERROR *result);

struct tevent_req *dcerpc_clusapi_SetServiceAccountPassword_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_SetServiceAccountPassword *r);
NTSTATUS dcerpc_clusapi_SetServiceAccountPassword_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_SetServiceAccountPassword_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_SetServiceAccountPassword *r);
struct tevent_req *dcerpc_clusapi_SetServiceAccountPassword_send(TALLOC_CTX *mem_ctx,
								 struct tevent_context *ev,
								 struct dcerpc_binding_handle *h,
								 const char *_lpszNewPassword /* [in] [charset(UTF16),ref] */,
								 enum IDL_CLUSTER_SET_PASSWORD_FLAGS _dwFlags /* [in]  */,
								 struct IDL_CLUSTER_SET_PASSWORD_STATUS *_ReturnStatusBufferPtr /* [out] [length_is(*SizeReturned),size_is(ReturnStatusBufferSize)] */,
								 uint32_t _ReturnStatusBufferSize /* [in] [range(0,(64*1024))] */,
								 uint32_t *_SizeReturned /* [out] [ref] */,
								 uint32_t *_ExpectedBufferSize /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_SetServiceAccountPassword_recv(struct tevent_req *req,
						       TALLOC_CTX *mem_ctx,
						       WERROR *result);
NTSTATUS dcerpc_clusapi_SetServiceAccountPassword(struct dcerpc_binding_handle *h,
						  TALLOC_CTX *mem_ctx,
						  const char *_lpszNewPassword /* [in] [charset(UTF16),ref] */,
						  enum IDL_CLUSTER_SET_PASSWORD_FLAGS _dwFlags /* [in]  */,
						  struct IDL_CLUSTER_SET_PASSWORD_STATUS *_ReturnStatusBufferPtr /* [out] [length_is(*SizeReturned),size_is(ReturnStatusBufferSize)] */,
						  uint32_t _ReturnStatusBufferSize /* [in] [range(0,(64*1024))] */,
						  uint32_t *_SizeReturned /* [out] [ref] */,
						  uint32_t *_ExpectedBufferSize /* [out] [ref] */,
						  WERROR *result);

struct tevent_req *dcerpc_clusapi_SetResourceDependencyExpression_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_SetResourceDependencyExpression *r);
NTSTATUS dcerpc_clusapi_SetResourceDependencyExpression_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_SetResourceDependencyExpression_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_SetResourceDependencyExpression *r);
struct tevent_req *dcerpc_clusapi_SetResourceDependencyExpression_send(TALLOC_CTX *mem_ctx,
								       struct tevent_context *ev,
								       struct dcerpc_binding_handle *h,
								       struct policy_handle _hResource /* [in]  */,
								       const char *_lpszDependencyExpression /* [in] [charset(UTF16),unique] */,
								       WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_SetResourceDependencyExpression_recv(struct tevent_req *req,
							     TALLOC_CTX *mem_ctx,
							     WERROR *result);
NTSTATUS dcerpc_clusapi_SetResourceDependencyExpression(struct dcerpc_binding_handle *h,
							TALLOC_CTX *mem_ctx,
							struct policy_handle _hResource /* [in]  */,
							const char *_lpszDependencyExpression /* [in] [charset(UTF16),unique] */,
							WERROR *_rpc_status /* [out] [ref] */,
							WERROR *result);

struct tevent_req *dcerpc_clusapi_GetResourceDependencyExpression_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_GetResourceDependencyExpression *r);
NTSTATUS dcerpc_clusapi_GetResourceDependencyExpression_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_GetResourceDependencyExpression_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_GetResourceDependencyExpression *r);
struct tevent_req *dcerpc_clusapi_GetResourceDependencyExpression_send(TALLOC_CTX *mem_ctx,
								       struct tevent_context *ev,
								       struct dcerpc_binding_handle *h,
								       struct policy_handle _hResource /* [in]  */,
								       const char **_lpszDependencyExpression /* [out] [charset(UTF16),ref] */,
								       WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_GetResourceDependencyExpression_recv(struct tevent_req *req,
							     TALLOC_CTX *mem_ctx,
							     WERROR *result);
NTSTATUS dcerpc_clusapi_GetResourceDependencyExpression(struct dcerpc_binding_handle *h,
							TALLOC_CTX *mem_ctx,
							struct policy_handle _hResource /* [in]  */,
							const char **_lpszDependencyExpression /* [out] [charset(UTF16),ref] */,
							WERROR *_rpc_status /* [out] [ref] */,
							WERROR *result);

struct tevent_req *dcerpc_Opnum111NotUsedOnWire_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct Opnum111NotUsedOnWire *r);
NTSTATUS dcerpc_Opnum111NotUsedOnWire_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_Opnum111NotUsedOnWire_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct Opnum111NotUsedOnWire *r);
struct tevent_req *dcerpc_Opnum111NotUsedOnWire_send(TALLOC_CTX *mem_ctx,
						     struct tevent_context *ev,
						     struct dcerpc_binding_handle *h);
NTSTATUS dcerpc_Opnum111NotUsedOnWire_recv(struct tevent_req *req,
					   TALLOC_CTX *mem_ctx,
					   WERROR *result);
NTSTATUS dcerpc_Opnum111NotUsedOnWire(struct dcerpc_binding_handle *h,
				      TALLOC_CTX *mem_ctx,
				      WERROR *result);

struct tevent_req *dcerpc_clusapi_GetResourceNetworkName_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_GetResourceNetworkName *r);
NTSTATUS dcerpc_clusapi_GetResourceNetworkName_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_GetResourceNetworkName_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_GetResourceNetworkName *r);
struct tevent_req *dcerpc_clusapi_GetResourceNetworkName_send(TALLOC_CTX *mem_ctx,
							      struct tevent_context *ev,
							      struct dcerpc_binding_handle *h,
							      struct policy_handle _hResource /* [in]  */,
							      const char **_lpszName /* [out] [charset(UTF16),ref] */,
							      WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_GetResourceNetworkName_recv(struct tevent_req *req,
						    TALLOC_CTX *mem_ctx,
						    WERROR *result);
NTSTATUS dcerpc_clusapi_GetResourceNetworkName(struct dcerpc_binding_handle *h,
					       TALLOC_CTX *mem_ctx,
					       struct policy_handle _hResource /* [in]  */,
					       const char **_lpszName /* [out] [charset(UTF16),ref] */,
					       WERROR *_rpc_status /* [out] [ref] */,
					       WERROR *result);

struct tevent_req *dcerpc_clusapi_ExecuteBatch_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_ExecuteBatch *r);
NTSTATUS dcerpc_clusapi_ExecuteBatch_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_ExecuteBatch_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_ExecuteBatch *r);
struct tevent_req *dcerpc_clusapi_ExecuteBatch_send(TALLOC_CTX *mem_ctx,
						    struct tevent_context *ev,
						    struct dcerpc_binding_handle *h,
						    struct policy_handle _hKey /* [in]  */,
						    uint32_t _cbData /* [in]  */,
						    uint8_t *_lpData /* [in] [ref,size_is(cbData)] */,
						    int32_t *_pdwFailedCommand /* [out] [ref] */,
						    WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_ExecuteBatch_recv(struct tevent_req *req,
					  TALLOC_CTX *mem_ctx,
					  WERROR *result);
NTSTATUS dcerpc_clusapi_ExecuteBatch(struct dcerpc_binding_handle *h,
				     TALLOC_CTX *mem_ctx,
				     struct policy_handle _hKey /* [in]  */,
				     uint32_t _cbData /* [in]  */,
				     uint8_t *_lpData /* [in] [ref,size_is(cbData)] */,
				     int32_t *_pdwFailedCommand /* [out] [ref] */,
				     WERROR *_rpc_status /* [out] [ref] */,
				     WERROR *result);

struct tevent_req *dcerpc_clusapi_CreateBatchPort_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_CreateBatchPort *r);
NTSTATUS dcerpc_clusapi_CreateBatchPort_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_CreateBatchPort_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_CreateBatchPort *r);
struct tevent_req *dcerpc_clusapi_CreateBatchPort_send(TALLOC_CTX *mem_ctx,
						       struct tevent_context *ev,
						       struct dcerpc_binding_handle *h,
						       struct policy_handle _hKey /* [in]  */,
						       struct policy_handle *_phBatchPort /* [out] [ref] */,
						       WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_CreateBatchPort_recv(struct tevent_req *req,
					     TALLOC_CTX *mem_ctx,
					     WERROR *result);
NTSTATUS dcerpc_clusapi_CreateBatchPort(struct dcerpc_binding_handle *h,
					TALLOC_CTX *mem_ctx,
					struct policy_handle _hKey /* [in]  */,
					struct policy_handle *_phBatchPort /* [out] [ref] */,
					WERROR *_rpc_status /* [out] [ref] */,
					WERROR *result);

struct tevent_req *dcerpc_clusapi_GetBatchNotification_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_GetBatchNotification *r);
NTSTATUS dcerpc_clusapi_GetBatchNotification_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_GetBatchNotification_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_GetBatchNotification *r);
struct tevent_req *dcerpc_clusapi_GetBatchNotification_send(TALLOC_CTX *mem_ctx,
							    struct tevent_context *ev,
							    struct dcerpc_binding_handle *h,
							    struct policy_handle _hBatchNotify /* [in]  */,
							    uint32_t *_cbData /* [out] [ref] */,
							    uint8_t **_lpData /* [out] [ref,size_is(,*cbData)] */);
NTSTATUS dcerpc_clusapi_GetBatchNotification_recv(struct tevent_req *req,
						  TALLOC_CTX *mem_ctx,
						  WERROR *result);
NTSTATUS dcerpc_clusapi_GetBatchNotification(struct dcerpc_binding_handle *h,
					     TALLOC_CTX *mem_ctx,
					     struct policy_handle _hBatchNotify /* [in]  */,
					     uint32_t *_cbData /* [out] [ref] */,
					     uint8_t **_lpData /* [out] [ref,size_is(,*cbData)] */,
					     WERROR *result);

struct tevent_req *dcerpc_clusapi_CloseBatchPort_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_CloseBatchPort *r);
NTSTATUS dcerpc_clusapi_CloseBatchPort_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_CloseBatchPort_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_CloseBatchPort *r);
struct tevent_req *dcerpc_clusapi_CloseBatchPort_send(TALLOC_CTX *mem_ctx,
						      struct tevent_context *ev,
						      struct dcerpc_binding_handle *h,
						      struct policy_handle *_phBatchPort /* [in,out] [ref] */);
NTSTATUS dcerpc_clusapi_CloseBatchPort_recv(struct tevent_req *req,
					    TALLOC_CTX *mem_ctx,
					    WERROR *result);
NTSTATUS dcerpc_clusapi_CloseBatchPort(struct dcerpc_binding_handle *h,
				       TALLOC_CTX *mem_ctx,
				       struct policy_handle *_phBatchPort /* [in,out] [ref] */,
				       WERROR *result);

struct tevent_req *dcerpc_clusapi_OpenClusterEx_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_OpenClusterEx *r);
NTSTATUS dcerpc_clusapi_OpenClusterEx_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_OpenClusterEx_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_OpenClusterEx *r);
struct tevent_req *dcerpc_clusapi_OpenClusterEx_send(TALLOC_CTX *mem_ctx,
						     struct tevent_context *ev,
						     struct dcerpc_binding_handle *h,
						     uint32_t _dwDesiredAccess /* [in]  */,
						     uint32_t *_lpdwGrantedAccess /* [out] [ref] */,
						     WERROR *_Status /* [out] [ref] */,
						     struct policy_handle *_hCluster /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_OpenClusterEx_recv(struct tevent_req *req,
					   TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_OpenClusterEx(struct dcerpc_binding_handle *h,
				      TALLOC_CTX *mem_ctx,
				      uint32_t _dwDesiredAccess /* [in]  */,
				      uint32_t *_lpdwGrantedAccess /* [out] [ref] */,
				      WERROR *_Status /* [out] [ref] */,
				      struct policy_handle *_hCluster /* [out] [ref] */);

struct tevent_req *dcerpc_clusapi_OpenNodeEx_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_OpenNodeEx *r);
NTSTATUS dcerpc_clusapi_OpenNodeEx_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_OpenNodeEx_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_OpenNodeEx *r);
struct tevent_req *dcerpc_clusapi_OpenNodeEx_send(TALLOC_CTX *mem_ctx,
						  struct tevent_context *ev,
						  struct dcerpc_binding_handle *h,
						  const char *_lpszNodeName /* [in] [charset(UTF16),ref] */,
						  uint32_t _dwDesiredAccess /* [in]  */,
						  uint32_t *_lpdwGrantedAccess /* [out] [ref] */,
						  WERROR *_Status /* [out] [ref] */,
						  WERROR *_rpc_status /* [out] [ref] */,
						  struct policy_handle *_hNode /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_OpenNodeEx_recv(struct tevent_req *req,
					TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_OpenNodeEx(struct dcerpc_binding_handle *h,
				   TALLOC_CTX *mem_ctx,
				   const char *_lpszNodeName /* [in] [charset(UTF16),ref] */,
				   uint32_t _dwDesiredAccess /* [in]  */,
				   uint32_t *_lpdwGrantedAccess /* [out] [ref] */,
				   WERROR *_Status /* [out] [ref] */,
				   WERROR *_rpc_status /* [out] [ref] */,
				   struct policy_handle *_hNode /* [out] [ref] */);

struct tevent_req *dcerpc_clusapi_OpenGroupEx_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_OpenGroupEx *r);
NTSTATUS dcerpc_clusapi_OpenGroupEx_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_OpenGroupEx_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_OpenGroupEx *r);
struct tevent_req *dcerpc_clusapi_OpenGroupEx_send(TALLOC_CTX *mem_ctx,
						   struct tevent_context *ev,
						   struct dcerpc_binding_handle *h,
						   const char *_lpszGroupName /* [in] [charset(UTF16),ref] */,
						   uint32_t _dwDesiredAccess /* [in]  */,
						   uint32_t *_lpdwGrantedAccess /* [out] [ref] */,
						   WERROR *_Status /* [out] [ref] */,
						   WERROR *_rpc_status /* [out] [ref] */,
						   struct policy_handle *_hGroup /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_OpenGroupEx_recv(struct tevent_req *req,
					 TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_OpenGroupEx(struct dcerpc_binding_handle *h,
				    TALLOC_CTX *mem_ctx,
				    const char *_lpszGroupName /* [in] [charset(UTF16),ref] */,
				    uint32_t _dwDesiredAccess /* [in]  */,
				    uint32_t *_lpdwGrantedAccess /* [out] [ref] */,
				    WERROR *_Status /* [out] [ref] */,
				    WERROR *_rpc_status /* [out] [ref] */,
				    struct policy_handle *_hGroup /* [out] [ref] */);

struct tevent_req *dcerpc_clusapi_OpenResourceEx_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_OpenResourceEx *r);
NTSTATUS dcerpc_clusapi_OpenResourceEx_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_OpenResourceEx_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_OpenResourceEx *r);
struct tevent_req *dcerpc_clusapi_OpenResourceEx_send(TALLOC_CTX *mem_ctx,
						      struct tevent_context *ev,
						      struct dcerpc_binding_handle *h,
						      const char *_lpszResourceName /* [in] [charset(UTF16),ref] */,
						      uint32_t _dwDesiredAccess /* [in]  */,
						      uint32_t *_lpdwGrantedAccess /* [out] [ref] */,
						      WERROR *_Status /* [out] [ref] */,
						      WERROR *_rpc_status /* [out] [ref] */,
						      struct policy_handle *_hResource /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_OpenResourceEx_recv(struct tevent_req *req,
					    TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_OpenResourceEx(struct dcerpc_binding_handle *h,
				       TALLOC_CTX *mem_ctx,
				       const char *_lpszResourceName /* [in] [charset(UTF16),ref] */,
				       uint32_t _dwDesiredAccess /* [in]  */,
				       uint32_t *_lpdwGrantedAccess /* [out] [ref] */,
				       WERROR *_Status /* [out] [ref] */,
				       WERROR *_rpc_status /* [out] [ref] */,
				       struct policy_handle *_hResource /* [out] [ref] */);

struct tevent_req *dcerpc_clusapi_OpenNetworkEx_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_OpenNetworkEx *r);
NTSTATUS dcerpc_clusapi_OpenNetworkEx_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_OpenNetworkEx_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_OpenNetworkEx *r);
struct tevent_req *dcerpc_clusapi_OpenNetworkEx_send(TALLOC_CTX *mem_ctx,
						     struct tevent_context *ev,
						     struct dcerpc_binding_handle *h,
						     const char *_lpszNetworkName /* [in] [charset(UTF16),ref] */,
						     uint32_t _dwDesiredAccess /* [in]  */,
						     uint32_t *_lpdwGrantedAccess /* [out] [ref] */,
						     WERROR *_Status /* [out] [ref] */,
						     WERROR *_rpc_status /* [out] [ref] */,
						     struct policy_handle *_hNetwork /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_OpenNetworkEx_recv(struct tevent_req *req,
					   TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_OpenNetworkEx(struct dcerpc_binding_handle *h,
				      TALLOC_CTX *mem_ctx,
				      const char *_lpszNetworkName /* [in] [charset(UTF16),ref] */,
				      uint32_t _dwDesiredAccess /* [in]  */,
				      uint32_t *_lpdwGrantedAccess /* [out] [ref] */,
				      WERROR *_Status /* [out] [ref] */,
				      WERROR *_rpc_status /* [out] [ref] */,
				      struct policy_handle *_hNetwork /* [out] [ref] */);

struct tevent_req *dcerpc_clusapi_OpenNetInterfaceEx_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_OpenNetInterfaceEx *r);
NTSTATUS dcerpc_clusapi_OpenNetInterfaceEx_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_OpenNetInterfaceEx_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_OpenNetInterfaceEx *r);
struct tevent_req *dcerpc_clusapi_OpenNetInterfaceEx_send(TALLOC_CTX *mem_ctx,
							  struct tevent_context *ev,
							  struct dcerpc_binding_handle *h,
							  const char *_lpszNetInterfaceName /* [in] [charset(UTF16),ref] */,
							  uint32_t _dwDesiredAccess /* [in]  */,
							  uint32_t *_lpdwGrantedAccess /* [out] [ref] */,
							  WERROR *_Status /* [out] [ref] */,
							  WERROR *_rpc_status /* [out] [ref] */,
							  struct policy_handle *_hNetInterface /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_OpenNetInterfaceEx_recv(struct tevent_req *req,
						TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_OpenNetInterfaceEx(struct dcerpc_binding_handle *h,
					   TALLOC_CTX *mem_ctx,
					   const char *_lpszNetInterfaceName /* [in] [charset(UTF16),ref] */,
					   uint32_t _dwDesiredAccess /* [in]  */,
					   uint32_t *_lpdwGrantedAccess /* [out] [ref] */,
					   WERROR *_Status /* [out] [ref] */,
					   WERROR *_rpc_status /* [out] [ref] */,
					   struct policy_handle *_hNetInterface /* [out] [ref] */);

struct tevent_req *dcerpc_clusapi_ChangeCsvState_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_ChangeCsvState *r);
NTSTATUS dcerpc_clusapi_ChangeCsvState_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_ChangeCsvState_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_ChangeCsvState *r);
struct tevent_req *dcerpc_clusapi_ChangeCsvState_send(TALLOC_CTX *mem_ctx,
						      struct tevent_context *ev,
						      struct dcerpc_binding_handle *h,
						      struct policy_handle _hResource /* [in]  */,
						      uint32_t _dwState /* [in]  */,
						      WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_ChangeCsvState_recv(struct tevent_req *req,
					    TALLOC_CTX *mem_ctx,
					    WERROR *result);
NTSTATUS dcerpc_clusapi_ChangeCsvState(struct dcerpc_binding_handle *h,
				       TALLOC_CTX *mem_ctx,
				       struct policy_handle _hResource /* [in]  */,
				       uint32_t _dwState /* [in]  */,
				       WERROR *_rpc_status /* [out] [ref] */,
				       WERROR *result);

struct tevent_req *dcerpc_clusapi_CreateNodeEnumEx_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_CreateNodeEnumEx *r);
NTSTATUS dcerpc_clusapi_CreateNodeEnumEx_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_CreateNodeEnumEx_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_CreateNodeEnumEx *r);
struct tevent_req *dcerpc_clusapi_CreateNodeEnumEx_send(TALLOC_CTX *mem_ctx,
							struct tevent_context *ev,
							struct dcerpc_binding_handle *h,
							struct policy_handle _hNode /* [in]  */,
							uint32_t _dwType /* [in]  */,
							uint32_t _dwOptions /* [in]  */,
							struct ENUM_LIST **_ReturnIdEnum /* [out] [ref] */,
							struct ENUM_LIST **_ReturnNameEnum /* [out] [ref] */,
							WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_CreateNodeEnumEx_recv(struct tevent_req *req,
					      TALLOC_CTX *mem_ctx,
					      WERROR *result);
NTSTATUS dcerpc_clusapi_CreateNodeEnumEx(struct dcerpc_binding_handle *h,
					 TALLOC_CTX *mem_ctx,
					 struct policy_handle _hNode /* [in]  */,
					 uint32_t _dwType /* [in]  */,
					 uint32_t _dwOptions /* [in]  */,
					 struct ENUM_LIST **_ReturnIdEnum /* [out] [ref] */,
					 struct ENUM_LIST **_ReturnNameEnum /* [out] [ref] */,
					 WERROR *_rpc_status /* [out] [ref] */,
					 WERROR *result);

struct tevent_req *dcerpc_clusapi_CreateEnumEx_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_CreateEnumEx *r);
NTSTATUS dcerpc_clusapi_CreateEnumEx_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_CreateEnumEx_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_CreateEnumEx *r);
struct tevent_req *dcerpc_clusapi_CreateEnumEx_send(TALLOC_CTX *mem_ctx,
						    struct tevent_context *ev,
						    struct dcerpc_binding_handle *h,
						    struct policy_handle _hCluster /* [in]  */,
						    uint32_t _dwType /* [in]  */,
						    uint32_t _dwOptions /* [in]  */,
						    struct ENUM_LIST **_ReturnIdEnum /* [out] [ref] */,
						    struct ENUM_LIST **_ReturnNameEnum /* [out] [ref] */,
						    WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_CreateEnumEx_recv(struct tevent_req *req,
					  TALLOC_CTX *mem_ctx,
					  WERROR *result);
NTSTATUS dcerpc_clusapi_CreateEnumEx(struct dcerpc_binding_handle *h,
				     TALLOC_CTX *mem_ctx,
				     struct policy_handle _hCluster /* [in]  */,
				     uint32_t _dwType /* [in]  */,
				     uint32_t _dwOptions /* [in]  */,
				     struct ENUM_LIST **_ReturnIdEnum /* [out] [ref] */,
				     struct ENUM_LIST **_ReturnNameEnum /* [out] [ref] */,
				     WERROR *_rpc_status /* [out] [ref] */,
				     WERROR *result);

struct tevent_req *dcerpc_clusapi_PauseNodeEx_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_PauseNodeEx *r);
NTSTATUS dcerpc_clusapi_PauseNodeEx_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_PauseNodeEx_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_PauseNodeEx *r);
struct tevent_req *dcerpc_clusapi_PauseNodeEx_send(TALLOC_CTX *mem_ctx,
						   struct tevent_context *ev,
						   struct dcerpc_binding_handle *h,
						   struct policy_handle _hNode /* [in]  */,
						   uint8_t _bDrainNode /* [in]  */,
						   uint32_t _dwPauseFlags /* [in]  */,
						   WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_PauseNodeEx_recv(struct tevent_req *req,
					 TALLOC_CTX *mem_ctx,
					 WERROR *result);
NTSTATUS dcerpc_clusapi_PauseNodeEx(struct dcerpc_binding_handle *h,
				    TALLOC_CTX *mem_ctx,
				    struct policy_handle _hNode /* [in]  */,
				    uint8_t _bDrainNode /* [in]  */,
				    uint32_t _dwPauseFlags /* [in]  */,
				    WERROR *_rpc_status /* [out] [ref] */,
				    WERROR *result);

struct tevent_req *dcerpc_clusapi_PauseNodeWithDrainTarget_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_PauseNodeWithDrainTarget *r);
NTSTATUS dcerpc_clusapi_PauseNodeWithDrainTarget_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_PauseNodeWithDrainTarget_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_PauseNodeWithDrainTarget *r);
struct tevent_req *dcerpc_clusapi_PauseNodeWithDrainTarget_send(TALLOC_CTX *mem_ctx,
								struct tevent_context *ev,
								struct dcerpc_binding_handle *h,
								struct policy_handle _hNode /* [in]  */,
								uint32_t _dwPauseFlags /* [in]  */,
								struct policy_handle _hNodeDrainTarget /* [in]  */,
								WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_PauseNodeWithDrainTarget_recv(struct tevent_req *req,
						      TALLOC_CTX *mem_ctx,
						      WERROR *result);
NTSTATUS dcerpc_clusapi_PauseNodeWithDrainTarget(struct dcerpc_binding_handle *h,
						 TALLOC_CTX *mem_ctx,
						 struct policy_handle _hNode /* [in]  */,
						 uint32_t _dwPauseFlags /* [in]  */,
						 struct policy_handle _hNodeDrainTarget /* [in]  */,
						 WERROR *_rpc_status /* [out] [ref] */,
						 WERROR *result);

struct tevent_req *dcerpc_clusapi_ResumeNodeEx_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_ResumeNodeEx *r);
NTSTATUS dcerpc_clusapi_ResumeNodeEx_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_ResumeNodeEx_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_ResumeNodeEx *r);
struct tevent_req *dcerpc_clusapi_ResumeNodeEx_send(TALLOC_CTX *mem_ctx,
						    struct tevent_context *ev,
						    struct dcerpc_binding_handle *h,
						    struct policy_handle _hNode /* [in]  */,
						    uint32_t _dwResumeFailbackType /* [in]  */,
						    uint32_t _dwResumeFlagsReserved /* [in]  */,
						    WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_ResumeNodeEx_recv(struct tevent_req *req,
					  TALLOC_CTX *mem_ctx,
					  WERROR *result);
NTSTATUS dcerpc_clusapi_ResumeNodeEx(struct dcerpc_binding_handle *h,
				     TALLOC_CTX *mem_ctx,
				     struct policy_handle _hNode /* [in]  */,
				     uint32_t _dwResumeFailbackType /* [in]  */,
				     uint32_t _dwResumeFlagsReserved /* [in]  */,
				     WERROR *_rpc_status /* [out] [ref] */,
				     WERROR *result);

struct tevent_req *dcerpc_clusapi_CreateGroupEx_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_CreateGroupEx *r);
NTSTATUS dcerpc_clusapi_CreateGroupEx_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_CreateGroupEx_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_CreateGroupEx *r);
struct tevent_req *dcerpc_clusapi_CreateGroupEx_send(TALLOC_CTX *mem_ctx,
						     struct tevent_context *ev,
						     struct dcerpc_binding_handle *h,
						     const char *_lpszGroupName /* [in] [charset(UTF16),ref] */,
						     struct CLUSTER_CREATE_GROUP_INFO_RPC *_pGroupInfo /* [in] [unique] */,
						     WERROR *_Status /* [out] [ref] */,
						     WERROR *_rpc_status /* [out] [ref] */,
						     struct policy_handle *_hGroup /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_CreateGroupEx_recv(struct tevent_req *req,
					   TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_CreateGroupEx(struct dcerpc_binding_handle *h,
				      TALLOC_CTX *mem_ctx,
				      const char *_lpszGroupName /* [in] [charset(UTF16),ref] */,
				      struct CLUSTER_CREATE_GROUP_INFO_RPC *_pGroupInfo /* [in] [unique] */,
				      WERROR *_Status /* [out] [ref] */,
				      WERROR *_rpc_status /* [out] [ref] */,
				      struct policy_handle *_hGroup /* [out] [ref] */);

struct tevent_req *dcerpc_clusapi_OnlineGroupEx_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_OnlineGroupEx *r);
NTSTATUS dcerpc_clusapi_OnlineGroupEx_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_OnlineGroupEx_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_OnlineGroupEx *r);
struct tevent_req *dcerpc_clusapi_OnlineGroupEx_send(TALLOC_CTX *mem_ctx,
						     struct tevent_context *ev,
						     struct dcerpc_binding_handle *h,
						     struct policy_handle _hGroup /* [in]  */,
						     uint32_t _dwOnlineFlags /* [in]  */,
						     uint8_t *_lpInBuffer /* [in] [ref,size_is(cbInBufferSize)] */,
						     uint32_t _cbInBufferSize /* [in]  */,
						     WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_OnlineGroupEx_recv(struct tevent_req *req,
					   TALLOC_CTX *mem_ctx,
					   WERROR *result);
NTSTATUS dcerpc_clusapi_OnlineGroupEx(struct dcerpc_binding_handle *h,
				      TALLOC_CTX *mem_ctx,
				      struct policy_handle _hGroup /* [in]  */,
				      uint32_t _dwOnlineFlags /* [in]  */,
				      uint8_t *_lpInBuffer /* [in] [ref,size_is(cbInBufferSize)] */,
				      uint32_t _cbInBufferSize /* [in]  */,
				      WERROR *_rpc_status /* [out] [ref] */,
				      WERROR *result);

struct tevent_req *dcerpc_clusapi_OfflineGroupEx_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_OfflineGroupEx *r);
NTSTATUS dcerpc_clusapi_OfflineGroupEx_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_OfflineGroupEx_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_OfflineGroupEx *r);
struct tevent_req *dcerpc_clusapi_OfflineGroupEx_send(TALLOC_CTX *mem_ctx,
						      struct tevent_context *ev,
						      struct dcerpc_binding_handle *h,
						      struct policy_handle _hGroup /* [in]  */,
						      uint32_t _dwOfflineFlags /* [in]  */,
						      uint8_t *_lpInBuffer /* [in] [ref,size_is(cbInBufferSize)] */,
						      uint32_t _cbInBufferSize /* [in]  */,
						      WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_OfflineGroupEx_recv(struct tevent_req *req,
					    TALLOC_CTX *mem_ctx,
					    WERROR *result);
NTSTATUS dcerpc_clusapi_OfflineGroupEx(struct dcerpc_binding_handle *h,
				       TALLOC_CTX *mem_ctx,
				       struct policy_handle _hGroup /* [in]  */,
				       uint32_t _dwOfflineFlags /* [in]  */,
				       uint8_t *_lpInBuffer /* [in] [ref,size_is(cbInBufferSize)] */,
				       uint32_t _cbInBufferSize /* [in]  */,
				       WERROR *_rpc_status /* [out] [ref] */,
				       WERROR *result);

struct tevent_req *dcerpc_clusapi_MoveGroupEx_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_MoveGroupEx *r);
NTSTATUS dcerpc_clusapi_MoveGroupEx_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_MoveGroupEx_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_MoveGroupEx *r);
struct tevent_req *dcerpc_clusapi_MoveGroupEx_send(TALLOC_CTX *mem_ctx,
						   struct tevent_context *ev,
						   struct dcerpc_binding_handle *h,
						   struct policy_handle _hGroup /* [in]  */,
						   uint32_t _dwMoveFlags /* [in]  */,
						   uint8_t *_lpInBuffer /* [in] [ref,size_is(cbInBufferSize)] */,
						   uint32_t _cbInBufferSize /* [in]  */,
						   WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_MoveGroupEx_recv(struct tevent_req *req,
					 TALLOC_CTX *mem_ctx,
					 WERROR *result);
NTSTATUS dcerpc_clusapi_MoveGroupEx(struct dcerpc_binding_handle *h,
				    TALLOC_CTX *mem_ctx,
				    struct policy_handle _hGroup /* [in]  */,
				    uint32_t _dwMoveFlags /* [in]  */,
				    uint8_t *_lpInBuffer /* [in] [ref,size_is(cbInBufferSize)] */,
				    uint32_t _cbInBufferSize /* [in]  */,
				    WERROR *_rpc_status /* [out] [ref] */,
				    WERROR *result);

struct tevent_req *dcerpc_clusapi_MoveGroupToNodeEx_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_MoveGroupToNodeEx *r);
NTSTATUS dcerpc_clusapi_MoveGroupToNodeEx_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_MoveGroupToNodeEx_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_MoveGroupToNodeEx *r);
struct tevent_req *dcerpc_clusapi_MoveGroupToNodeEx_send(TALLOC_CTX *mem_ctx,
							 struct tevent_context *ev,
							 struct dcerpc_binding_handle *h,
							 struct policy_handle _hGroup /* [in]  */,
							 struct policy_handle _hNode /* [in]  */,
							 uint32_t _dwMoveFlags /* [in]  */,
							 uint8_t *_lpInBuffer /* [in] [ref,size_is(cbInBufferSize)] */,
							 uint32_t _cbInBufferSize /* [in]  */,
							 WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_MoveGroupToNodeEx_recv(struct tevent_req *req,
					       TALLOC_CTX *mem_ctx,
					       WERROR *result);
NTSTATUS dcerpc_clusapi_MoveGroupToNodeEx(struct dcerpc_binding_handle *h,
					  TALLOC_CTX *mem_ctx,
					  struct policy_handle _hGroup /* [in]  */,
					  struct policy_handle _hNode /* [in]  */,
					  uint32_t _dwMoveFlags /* [in]  */,
					  uint8_t *_lpInBuffer /* [in] [ref,size_is(cbInBufferSize)] */,
					  uint32_t _cbInBufferSize /* [in]  */,
					  WERROR *_rpc_status /* [out] [ref] */,
					  WERROR *result);

struct tevent_req *dcerpc_clusapi_CancelClusterGroupOperation_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_CancelClusterGroupOperation *r);
NTSTATUS dcerpc_clusapi_CancelClusterGroupOperation_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_CancelClusterGroupOperation_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_CancelClusterGroupOperation *r);
struct tevent_req *dcerpc_clusapi_CancelClusterGroupOperation_send(TALLOC_CTX *mem_ctx,
								   struct tevent_context *ev,
								   struct dcerpc_binding_handle *h,
								   struct policy_handle _hGroup /* [in]  */,
								   uint32_t _dwCancelFlags /* [in]  */,
								   WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_CancelClusterGroupOperation_recv(struct tevent_req *req,
							 TALLOC_CTX *mem_ctx,
							 WERROR *result);
NTSTATUS dcerpc_clusapi_CancelClusterGroupOperation(struct dcerpc_binding_handle *h,
						    TALLOC_CTX *mem_ctx,
						    struct policy_handle _hGroup /* [in]  */,
						    uint32_t _dwCancelFlags /* [in]  */,
						    WERROR *_rpc_status /* [out] [ref] */,
						    WERROR *result);

struct tevent_req *dcerpc_clusapi_OnlineResourceEx_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_OnlineResourceEx *r);
NTSTATUS dcerpc_clusapi_OnlineResourceEx_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_OnlineResourceEx_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_OnlineResourceEx *r);
struct tevent_req *dcerpc_clusapi_OnlineResourceEx_send(TALLOC_CTX *mem_ctx,
							struct tevent_context *ev,
							struct dcerpc_binding_handle *h,
							struct policy_handle _hResource /* [in]  */,
							uint32_t _dwOnlineFlags /* [in]  */,
							uint8_t *_lpInBuffer /* [in] [ref,size_is(cbInBufferSize)] */,
							uint32_t _cbInBufferSize /* [in]  */,
							WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_OnlineResourceEx_recv(struct tevent_req *req,
					      TALLOC_CTX *mem_ctx,
					      WERROR *result);
NTSTATUS dcerpc_clusapi_OnlineResourceEx(struct dcerpc_binding_handle *h,
					 TALLOC_CTX *mem_ctx,
					 struct policy_handle _hResource /* [in]  */,
					 uint32_t _dwOnlineFlags /* [in]  */,
					 uint8_t *_lpInBuffer /* [in] [ref,size_is(cbInBufferSize)] */,
					 uint32_t _cbInBufferSize /* [in]  */,
					 WERROR *_rpc_status /* [out] [ref] */,
					 WERROR *result);

struct tevent_req *dcerpc_clusapi_OfflineResourceEx_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_OfflineResourceEx *r);
NTSTATUS dcerpc_clusapi_OfflineResourceEx_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_OfflineResourceEx_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_OfflineResourceEx *r);
struct tevent_req *dcerpc_clusapi_OfflineResourceEx_send(TALLOC_CTX *mem_ctx,
							 struct tevent_context *ev,
							 struct dcerpc_binding_handle *h,
							 struct policy_handle _hResource /* [in]  */,
							 uint32_t _dwOfflineFlags /* [in]  */,
							 uint8_t *_lpInBuffer /* [in] [ref,size_is(cbInBufferSize)] */,
							 uint32_t _cbInBufferSize /* [in]  */,
							 WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_OfflineResourceEx_recv(struct tevent_req *req,
					       TALLOC_CTX *mem_ctx,
					       WERROR *result);
NTSTATUS dcerpc_clusapi_OfflineResourceEx(struct dcerpc_binding_handle *h,
					  TALLOC_CTX *mem_ctx,
					  struct policy_handle _hResource /* [in]  */,
					  uint32_t _dwOfflineFlags /* [in]  */,
					  uint8_t *_lpInBuffer /* [in] [ref,size_is(cbInBufferSize)] */,
					  uint32_t _cbInBufferSize /* [in]  */,
					  WERROR *_rpc_status /* [out] [ref] */,
					  WERROR *result);

struct tevent_req *dcerpc_clusapi_CreateNotifyV2_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_CreateNotifyV2 *r);
NTSTATUS dcerpc_clusapi_CreateNotifyV2_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_CreateNotifyV2_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_CreateNotifyV2 *r);
struct tevent_req *dcerpc_clusapi_CreateNotifyV2_send(TALLOC_CTX *mem_ctx,
						      struct tevent_context *ev,
						      struct dcerpc_binding_handle *h,
						      WERROR *_rpc_error /* [out] [ref] */,
						      WERROR *_rpc_status /* [out] [ref] */,
						      struct policy_handle *_hNotify /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_CreateNotifyV2_recv(struct tevent_req *req,
					    TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_CreateNotifyV2(struct dcerpc_binding_handle *h,
				       TALLOC_CTX *mem_ctx,
				       WERROR *_rpc_error /* [out] [ref] */,
				       WERROR *_rpc_status /* [out] [ref] */,
				       struct policy_handle *_hNotify /* [out] [ref] */);

struct tevent_req *dcerpc_clusapi_AddNotifyV2_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_AddNotifyV2 *r);
NTSTATUS dcerpc_clusapi_AddNotifyV2_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_AddNotifyV2_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_AddNotifyV2 *r);
struct tevent_req *dcerpc_clusapi_AddNotifyV2_send(TALLOC_CTX *mem_ctx,
						   struct tevent_context *ev,
						   struct dcerpc_binding_handle *h,
						   struct policy_handle _hNotify /* [in]  */,
						   struct policy_handle _hObject /* [in]  */,
						   struct NOTIFY_FILTER_AND_TYPE_RPC _filter /* [in]  */,
						   uint32_t _dwNotifyKey /* [in]  */,
						   uint32_t _dwVersion /* [in]  */,
						   uint8_t _isTargetedAtObject /* [in]  */,
						   WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_AddNotifyV2_recv(struct tevent_req *req,
					 TALLOC_CTX *mem_ctx,
					 WERROR *result);
NTSTATUS dcerpc_clusapi_AddNotifyV2(struct dcerpc_binding_handle *h,
				    TALLOC_CTX *mem_ctx,
				    struct policy_handle _hNotify /* [in]  */,
				    struct policy_handle _hObject /* [in]  */,
				    struct NOTIFY_FILTER_AND_TYPE_RPC _filter /* [in]  */,
				    uint32_t _dwNotifyKey /* [in]  */,
				    uint32_t _dwVersion /* [in]  */,
				    uint8_t _isTargetedAtObject /* [in]  */,
				    WERROR *_rpc_status /* [out] [ref] */,
				    WERROR *result);

struct tevent_req *dcerpc_clusapi_GetNotifyV2_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_GetNotifyV2 *r);
NTSTATUS dcerpc_clusapi_GetNotifyV2_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_GetNotifyV2_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_GetNotifyV2 *r);
struct tevent_req *dcerpc_clusapi_GetNotifyV2_send(TALLOC_CTX *mem_ctx,
						   struct tevent_context *ev,
						   struct dcerpc_binding_handle *h,
						   struct policy_handle _hNotify /* [in]  */,
						   struct NOTIFICATION_RPC **_Notifications /* [out] [ref,size_is(,*dwNumNotifications)] */,
						   uint32_t *_dwNumNotifications /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_GetNotifyV2_recv(struct tevent_req *req,
					 TALLOC_CTX *mem_ctx,
					 WERROR *result);
NTSTATUS dcerpc_clusapi_GetNotifyV2(struct dcerpc_binding_handle *h,
				    TALLOC_CTX *mem_ctx,
				    struct policy_handle _hNotify /* [in]  */,
				    struct NOTIFICATION_RPC **_Notifications /* [out] [ref,size_is(,*dwNumNotifications)] */,
				    uint32_t *_dwNumNotifications /* [out] [ref] */,
				    WERROR *result);

struct tevent_req *dcerpc_clusapi_QueryAllValues_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_QueryAllValues *r);
NTSTATUS dcerpc_clusapi_QueryAllValues_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_QueryAllValues_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_QueryAllValues *r);
struct tevent_req *dcerpc_clusapi_QueryAllValues_send(TALLOC_CTX *mem_ctx,
						      struct tevent_context *ev,
						      struct dcerpc_binding_handle *h,
						      struct policy_handle _hKey /* [in]  */,
						      uint32_t *_pcbData /* [out] [ref] */,
						      uint8_t **_ppData /* [out] [ref,size_is(1,*pcbData)] */,
						      WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_QueryAllValues_recv(struct tevent_req *req,
					    TALLOC_CTX *mem_ctx,
					    WERROR *result);
NTSTATUS dcerpc_clusapi_QueryAllValues(struct dcerpc_binding_handle *h,
				       TALLOC_CTX *mem_ctx,
				       struct policy_handle _hKey /* [in]  */,
				       uint32_t *_pcbData /* [out] [ref] */,
				       uint8_t **_ppData /* [out] [ref,size_is(1,*pcbData)] */,
				       WERROR *_rpc_status /* [out] [ref] */,
				       WERROR *result);

struct tevent_req *dcerpc_clusapi_StmFindDisk_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_StmFindDisk *r);
NTSTATUS dcerpc_clusapi_StmFindDisk_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_StmFindDisk_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_StmFindDisk *r);
struct tevent_req *dcerpc_clusapi_StmFindDisk_send(TALLOC_CTX *mem_ctx,
						   struct tevent_context *ev,
						   struct dcerpc_binding_handle *h,
						   struct policy_handle _hCluster /* [in]  */,
						   uint32_t _dwFlags /* [in]  */,
						   struct CLUSTER_DISKID *_pDiskId /* [in,out] [unique] */,
						   uint8_t *_pUniqueId /* [in] [size_is(uniqueIdSize),unique] */,
						   uint32_t _uniqueIdSize /* [in]  */,
						   const char **_ppszDeviceName /* [out] [charset(UTF16),ref] */,
						   WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_StmFindDisk_recv(struct tevent_req *req,
					 TALLOC_CTX *mem_ctx,
					 WERROR *result);
NTSTATUS dcerpc_clusapi_StmFindDisk(struct dcerpc_binding_handle *h,
				    TALLOC_CTX *mem_ctx,
				    struct policy_handle _hCluster /* [in]  */,
				    uint32_t _dwFlags /* [in]  */,
				    struct CLUSTER_DISKID *_pDiskId /* [in,out] [unique] */,
				    uint8_t *_pUniqueId /* [in] [size_is(uniqueIdSize),unique] */,
				    uint32_t _uniqueIdSize /* [in]  */,
				    const char **_ppszDeviceName /* [out] [charset(UTF16),ref] */,
				    WERROR *_rpc_status /* [out] [ref] */,
				    WERROR *result);

struct tevent_req *dcerpc_clusapi_ClusterMrr_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_ClusterMrr *r);
NTSTATUS dcerpc_clusapi_ClusterMrr_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_ClusterMrr_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_ClusterMrr *r);
struct tevent_req *dcerpc_clusapi_ClusterMrr_send(TALLOC_CTX *mem_ctx,
						  struct tevent_context *ev,
						  struct dcerpc_binding_handle *h,
						  struct policy_handle _hCluster /* [in]  */,
						  uint8_t _fExcludeSelf /* [in]  */,
						  uint64_t _nodeSet /* [in]  */,
						  uint32_t _dwTimeout /* [in]  */,
						  uint32_t _dwComponent /* [in]  */,
						  uint32_t _dwControlCode /* [in]  */,
						  uint32_t _inSize /* [in]  */,
						  uint8_t *_pInData /* [in] [size_is(inSize),unique] */,
						  struct CLUSTER_MRR_RESPONSE **_ppInfo /* [out] [ref,size_is(1,1)] */,
						  WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_ClusterMrr_recv(struct tevent_req *req,
					TALLOC_CTX *mem_ctx,
					WERROR *result);
NTSTATUS dcerpc_clusapi_ClusterMrr(struct dcerpc_binding_handle *h,
				   TALLOC_CTX *mem_ctx,
				   struct policy_handle _hCluster /* [in]  */,
				   uint8_t _fExcludeSelf /* [in]  */,
				   uint64_t _nodeSet /* [in]  */,
				   uint32_t _dwTimeout /* [in]  */,
				   uint32_t _dwComponent /* [in]  */,
				   uint32_t _dwControlCode /* [in]  */,
				   uint32_t _inSize /* [in]  */,
				   uint8_t *_pInData /* [in] [size_is(inSize),unique] */,
				   struct CLUSTER_MRR_RESPONSE **_ppInfo /* [out] [ref,size_is(1,1)] */,
				   WERROR *_rpc_status /* [out] [ref] */,
				   WERROR *result);

struct tevent_req *dcerpc_clusapi_CreateGroupEnum_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_CreateGroupEnum *r);
NTSTATUS dcerpc_clusapi_CreateGroupEnum_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_CreateGroupEnum_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_CreateGroupEnum *r);
struct tevent_req *dcerpc_clusapi_CreateGroupEnum_send(TALLOC_CTX *mem_ctx,
						       struct tevent_context *ev,
						       struct dcerpc_binding_handle *h,
						       struct policy_handle _hCluster /* [in]  */,
						       uint8_t *_pProperties /* [in] [size_is(cbProperties),unique] */,
						       uint32_t _cbProperties /* [in]  */,
						       uint8_t *_pRoProperties /* [in] [size_is(cbRoProperties),unique] */,
						       uint32_t _cbRoProperties /* [in]  */,
						       struct GROUP_ENUM_LIST **_ppResultList /* [out] [ref] */,
						       WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_CreateGroupEnum_recv(struct tevent_req *req,
					     TALLOC_CTX *mem_ctx,
					     WERROR *result);
NTSTATUS dcerpc_clusapi_CreateGroupEnum(struct dcerpc_binding_handle *h,
					TALLOC_CTX *mem_ctx,
					struct policy_handle _hCluster /* [in]  */,
					uint8_t *_pProperties /* [in] [size_is(cbProperties),unique] */,
					uint32_t _cbProperties /* [in]  */,
					uint8_t *_pRoProperties /* [in] [size_is(cbRoProperties),unique] */,
					uint32_t _cbRoProperties /* [in]  */,
					struct GROUP_ENUM_LIST **_ppResultList /* [out] [ref] */,
					WERROR *_rpc_status /* [out] [ref] */,
					WERROR *result);

struct tevent_req *dcerpc_clusapi_CreateResourceEnum_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_CreateResourceEnum *r);
NTSTATUS dcerpc_clusapi_CreateResourceEnum_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_CreateResourceEnum_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_CreateResourceEnum *r);
struct tevent_req *dcerpc_clusapi_CreateResourceEnum_send(TALLOC_CTX *mem_ctx,
							  struct tevent_context *ev,
							  struct dcerpc_binding_handle *h,
							  struct policy_handle _hCluster /* [in]  */,
							  uint8_t *_pProperties /* [in] [size_is(cbProperties),unique] */,
							  uint32_t _cbProperties /* [in]  */,
							  uint8_t *_pRoProperties /* [in] [size_is(cbRoProperties),unique] */,
							  uint32_t _cbRoProperties /* [in]  */,
							  struct RESOURCE_ENUM_LIST **_ppResultList /* [out] [ref] */,
							  WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_CreateResourceEnum_recv(struct tevent_req *req,
						TALLOC_CTX *mem_ctx,
						WERROR *result);
NTSTATUS dcerpc_clusapi_CreateResourceEnum(struct dcerpc_binding_handle *h,
					   TALLOC_CTX *mem_ctx,
					   struct policy_handle _hCluster /* [in]  */,
					   uint8_t *_pProperties /* [in] [size_is(cbProperties),unique] */,
					   uint32_t _cbProperties /* [in]  */,
					   uint8_t *_pRoProperties /* [in] [size_is(cbRoProperties),unique] */,
					   uint32_t _cbRoProperties /* [in]  */,
					   struct RESOURCE_ENUM_LIST **_ppResultList /* [out] [ref] */,
					   WERROR *_rpc_status /* [out] [ref] */,
					   WERROR *result);

struct tevent_req *dcerpc_clusapi_ExecuteReadBatch_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_ExecuteReadBatch *r);
NTSTATUS dcerpc_clusapi_ExecuteReadBatch_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_ExecuteReadBatch_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_ExecuteReadBatch *r);
struct tevent_req *dcerpc_clusapi_ExecuteReadBatch_send(TALLOC_CTX *mem_ctx,
							struct tevent_context *ev,
							struct dcerpc_binding_handle *h,
							struct policy_handle _hKey /* [in]  */,
							uint32_t _cbInData /* [in]  */,
							uint8_t *_lpInData /* [in] [ref,size_is(cbInData)] */,
							uint32_t *_cbOutData /* [out] [ref] */,
							uint8_t **_lpOutData /* [out] [ref,size_is(,*cbOutData)] */,
							WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_ExecuteReadBatch_recv(struct tevent_req *req,
					      TALLOC_CTX *mem_ctx,
					      WERROR *result);
NTSTATUS dcerpc_clusapi_ExecuteReadBatch(struct dcerpc_binding_handle *h,
					 TALLOC_CTX *mem_ctx,
					 struct policy_handle _hKey /* [in]  */,
					 uint32_t _cbInData /* [in]  */,
					 uint8_t *_lpInData /* [in] [ref,size_is(cbInData)] */,
					 uint32_t *_cbOutData /* [out] [ref] */,
					 uint8_t **_lpOutData /* [out] [ref,size_is(,*cbOutData)] */,
					 WERROR *_rpc_status /* [out] [ref] */,
					 WERROR *result);

struct tevent_req *dcerpc_clusapi_RestartResource_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_RestartResource *r);
NTSTATUS dcerpc_clusapi_RestartResource_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_RestartResource_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_RestartResource *r);
struct tevent_req *dcerpc_clusapi_RestartResource_send(TALLOC_CTX *mem_ctx,
						       struct tevent_context *ev,
						       struct dcerpc_binding_handle *h,
						       struct policy_handle _hResource /* [in]  */,
						       uint32_t _dwFlags /* [in]  */,
						       WERROR *_rpc_status /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_RestartResource_recv(struct tevent_req *req,
					     TALLOC_CTX *mem_ctx,
					     WERROR *result);
NTSTATUS dcerpc_clusapi_RestartResource(struct dcerpc_binding_handle *h,
					TALLOC_CTX *mem_ctx,
					struct policy_handle _hResource /* [in]  */,
					uint32_t _dwFlags /* [in]  */,
					WERROR *_rpc_status /* [out] [ref] */,
					WERROR *result);

struct tevent_req *dcerpc_clusapi_GetNotifyAsync_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct clusapi_GetNotifyAsync *r);
NTSTATUS dcerpc_clusapi_GetNotifyAsync_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_clusapi_GetNotifyAsync_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct clusapi_GetNotifyAsync *r);
struct tevent_req *dcerpc_clusapi_GetNotifyAsync_send(TALLOC_CTX *mem_ctx,
						      struct tevent_context *ev,
						      struct dcerpc_binding_handle *h,
						      struct policy_handle _hNotify /* [in]  */,
						      struct NOTIFICATION_DATA_ASYNC_RPC **_Notifications /* [out] [ref,size_is(,*dwNumNotifications)] */,
						      uint32_t *_dwNumNotifications /* [out] [ref] */);
NTSTATUS dcerpc_clusapi_GetNotifyAsync_recv(struct tevent_req *req,
					    TALLOC_CTX *mem_ctx,
					    WERROR *result);
NTSTATUS dcerpc_clusapi_GetNotifyAsync(struct dcerpc_binding_handle *h,
				       TALLOC_CTX *mem_ctx,
				       struct policy_handle _hNotify /* [in]  */,
				       struct NOTIFICATION_DATA_ASYNC_RPC **_Notifications /* [out] [ref,size_is(,*dwNumNotifications)] */,
				       uint32_t *_dwNumNotifications /* [out] [ref] */,
				       WERROR *result);

#endif /* _HEADER_RPC_clusapi */
