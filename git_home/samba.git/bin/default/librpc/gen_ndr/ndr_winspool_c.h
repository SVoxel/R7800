#include "librpc/rpc/dcerpc.h"
#include "bin/default/librpc/gen_ndr/winspool.h"
#ifndef _HEADER_RPC_iremotewinspool
#define _HEADER_RPC_iremotewinspool

extern const struct ndr_interface_table ndr_table_iremotewinspool;

struct tevent_req *dcerpc_winspool_AsyncOpenPrinter_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncOpenPrinter *r);
NTSTATUS dcerpc_winspool_AsyncOpenPrinter_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncOpenPrinter_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncOpenPrinter *r);
struct tevent_req *dcerpc_winspool_AsyncOpenPrinter_send(TALLOC_CTX *mem_ctx,
							 struct tevent_context *ev,
							 struct dcerpc_binding_handle *h,
							 const char *_pPrinterName /* [in] [charset(UTF16),unique] */,
							 struct policy_handle *_pHandle /* [out] [ref] */,
							 const char *_pDatatype /* [in] [charset(UTF16),unique] */,
							 struct spoolss_DevmodeContainer *_pDevModeContainer /* [in] [ref] */,
							 uint32_t _AccessRequired /* [in]  */,
							 struct spoolss_UserLevelCtr *_pClientInfo /* [in] [ref] */);
NTSTATUS dcerpc_winspool_AsyncOpenPrinter_recv(struct tevent_req *req,
					       TALLOC_CTX *mem_ctx,
					       WERROR *result);
NTSTATUS dcerpc_winspool_AsyncOpenPrinter(struct dcerpc_binding_handle *h,
					  TALLOC_CTX *mem_ctx,
					  const char *_pPrinterName /* [in] [charset(UTF16),unique] */,
					  struct policy_handle *_pHandle /* [out] [ref] */,
					  const char *_pDatatype /* [in] [charset(UTF16),unique] */,
					  struct spoolss_DevmodeContainer *_pDevModeContainer /* [in] [ref] */,
					  uint32_t _AccessRequired /* [in]  */,
					  struct spoolss_UserLevelCtr *_pClientInfo /* [in] [ref] */,
					  WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncAddPrinter_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncAddPrinter *r);
NTSTATUS dcerpc_winspool_AsyncAddPrinter_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncAddPrinter_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncAddPrinter *r);
struct tevent_req *dcerpc_winspool_AsyncAddPrinter_send(TALLOC_CTX *mem_ctx,
							struct tevent_context *ev,
							struct dcerpc_binding_handle *h,
							const char *_pName /* [in] [charset(UTF16),unique] */,
							struct spoolss_SetPrinterInfoCtr *_pPrinterContainer /* [in] [ref] */,
							struct spoolss_DevmodeContainer *_pDevModeContainer /* [in] [ref] */,
							struct sec_desc_buf *_pSecurityContainer /* [in] [ref] */,
							struct spoolss_UserLevelCtr *_pClientInfo /* [in] [ref] */,
							struct policy_handle *_pHandle /* [out] [ref] */);
NTSTATUS dcerpc_winspool_AsyncAddPrinter_recv(struct tevent_req *req,
					      TALLOC_CTX *mem_ctx,
					      WERROR *result);
NTSTATUS dcerpc_winspool_AsyncAddPrinter(struct dcerpc_binding_handle *h,
					 TALLOC_CTX *mem_ctx,
					 const char *_pName /* [in] [charset(UTF16),unique] */,
					 struct spoolss_SetPrinterInfoCtr *_pPrinterContainer /* [in] [ref] */,
					 struct spoolss_DevmodeContainer *_pDevModeContainer /* [in] [ref] */,
					 struct sec_desc_buf *_pSecurityContainer /* [in] [ref] */,
					 struct spoolss_UserLevelCtr *_pClientInfo /* [in] [ref] */,
					 struct policy_handle *_pHandle /* [out] [ref] */,
					 WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncSetJob_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncSetJob *r);
NTSTATUS dcerpc_winspool_AsyncSetJob_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncSetJob_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncSetJob *r);
struct tevent_req *dcerpc_winspool_AsyncSetJob_send(TALLOC_CTX *mem_ctx,
						    struct tevent_context *ev,
						    struct dcerpc_binding_handle *h,
						    struct policy_handle _hPrinter /* [in]  */,
						    uint32_t _JobId /* [in]  */,
						    struct spoolss_JobInfoContainer *_pJobContainer /* [in] [unique] */,
						    uint32_t _Command /* [in]  */);
NTSTATUS dcerpc_winspool_AsyncSetJob_recv(struct tevent_req *req,
					  TALLOC_CTX *mem_ctx,
					  WERROR *result);
NTSTATUS dcerpc_winspool_AsyncSetJob(struct dcerpc_binding_handle *h,
				     TALLOC_CTX *mem_ctx,
				     struct policy_handle _hPrinter /* [in]  */,
				     uint32_t _JobId /* [in]  */,
				     struct spoolss_JobInfoContainer *_pJobContainer /* [in] [unique] */,
				     uint32_t _Command /* [in]  */,
				     WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncGetJob_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncGetJob *r);
NTSTATUS dcerpc_winspool_AsyncGetJob_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncGetJob_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncGetJob *r);
struct tevent_req *dcerpc_winspool_AsyncGetJob_send(TALLOC_CTX *mem_ctx,
						    struct tevent_context *ev,
						    struct dcerpc_binding_handle *h,
						    struct policy_handle _hPrinter /* [in]  */,
						    uint32_t _JobId /* [in]  */,
						    uint32_t _Level /* [in]  */,
						    uint8_t *_pJob /* [in,out] [size_is(cbBuf),unique] */,
						    uint32_t _cbBuf /* [in]  */,
						    uint32_t *_pcbNeeded /* [out] [ref] */);
NTSTATUS dcerpc_winspool_AsyncGetJob_recv(struct tevent_req *req,
					  TALLOC_CTX *mem_ctx,
					  WERROR *result);
NTSTATUS dcerpc_winspool_AsyncGetJob(struct dcerpc_binding_handle *h,
				     TALLOC_CTX *mem_ctx,
				     struct policy_handle _hPrinter /* [in]  */,
				     uint32_t _JobId /* [in]  */,
				     uint32_t _Level /* [in]  */,
				     uint8_t *_pJob /* [in,out] [size_is(cbBuf),unique] */,
				     uint32_t _cbBuf /* [in]  */,
				     uint32_t *_pcbNeeded /* [out] [ref] */,
				     WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncEnumJobs_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncEnumJobs *r);
NTSTATUS dcerpc_winspool_AsyncEnumJobs_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncEnumJobs_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncEnumJobs *r);
struct tevent_req *dcerpc_winspool_AsyncEnumJobs_send(TALLOC_CTX *mem_ctx,
						      struct tevent_context *ev,
						      struct dcerpc_binding_handle *h,
						      struct policy_handle _hPrinter /* [in]  */,
						      uint32_t _FirstJob /* [in]  */,
						      uint32_t _NoJobs /* [in]  */,
						      uint32_t _Level /* [in]  */,
						      uint8_t *_pJob /* [in,out] [size_is(cbBuf),unique] */,
						      uint32_t _cbBuf /* [in]  */,
						      uint32_t *_pcbNeeded /* [out] [ref] */,
						      uint32_t *_pcReturned /* [out] [ref] */);
NTSTATUS dcerpc_winspool_AsyncEnumJobs_recv(struct tevent_req *req,
					    TALLOC_CTX *mem_ctx,
					    WERROR *result);
NTSTATUS dcerpc_winspool_AsyncEnumJobs(struct dcerpc_binding_handle *h,
				       TALLOC_CTX *mem_ctx,
				       struct policy_handle _hPrinter /* [in]  */,
				       uint32_t _FirstJob /* [in]  */,
				       uint32_t _NoJobs /* [in]  */,
				       uint32_t _Level /* [in]  */,
				       uint8_t *_pJob /* [in,out] [size_is(cbBuf),unique] */,
				       uint32_t _cbBuf /* [in]  */,
				       uint32_t *_pcbNeeded /* [out] [ref] */,
				       uint32_t *_pcReturned /* [out] [ref] */,
				       WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncAddJob_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncAddJob *r);
NTSTATUS dcerpc_winspool_AsyncAddJob_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncAddJob_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncAddJob *r);
struct tevent_req *dcerpc_winspool_AsyncAddJob_send(TALLOC_CTX *mem_ctx,
						    struct tevent_context *ev,
						    struct dcerpc_binding_handle *h,
						    struct policy_handle _hPrinter /* [in]  */,
						    uint32_t _Level /* [in]  */,
						    uint8_t *_pAddJob /* [in,out] [size_is(cbBuf),unique] */,
						    uint32_t _cbBuf /* [in]  */,
						    uint32_t *_pcbNeeded /* [out] [ref] */);
NTSTATUS dcerpc_winspool_AsyncAddJob_recv(struct tevent_req *req,
					  TALLOC_CTX *mem_ctx,
					  WERROR *result);
NTSTATUS dcerpc_winspool_AsyncAddJob(struct dcerpc_binding_handle *h,
				     TALLOC_CTX *mem_ctx,
				     struct policy_handle _hPrinter /* [in]  */,
				     uint32_t _Level /* [in]  */,
				     uint8_t *_pAddJob /* [in,out] [size_is(cbBuf),unique] */,
				     uint32_t _cbBuf /* [in]  */,
				     uint32_t *_pcbNeeded /* [out] [ref] */,
				     WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncScheduleJob_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncScheduleJob *r);
NTSTATUS dcerpc_winspool_AsyncScheduleJob_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncScheduleJob_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncScheduleJob *r);
struct tevent_req *dcerpc_winspool_AsyncScheduleJob_send(TALLOC_CTX *mem_ctx,
							 struct tevent_context *ev,
							 struct dcerpc_binding_handle *h,
							 struct policy_handle _hPrinter /* [in]  */,
							 uint32_t _JobId /* [in]  */);
NTSTATUS dcerpc_winspool_AsyncScheduleJob_recv(struct tevent_req *req,
					       TALLOC_CTX *mem_ctx,
					       WERROR *result);
NTSTATUS dcerpc_winspool_AsyncScheduleJob(struct dcerpc_binding_handle *h,
					  TALLOC_CTX *mem_ctx,
					  struct policy_handle _hPrinter /* [in]  */,
					  uint32_t _JobId /* [in]  */,
					  WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncDeletePrinter_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncDeletePrinter *r);
NTSTATUS dcerpc_winspool_AsyncDeletePrinter_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncDeletePrinter_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncDeletePrinter *r);
struct tevent_req *dcerpc_winspool_AsyncDeletePrinter_send(TALLOC_CTX *mem_ctx,
							   struct tevent_context *ev,
							   struct dcerpc_binding_handle *h,
							   struct policy_handle _hPrinter /* [in]  */);
NTSTATUS dcerpc_winspool_AsyncDeletePrinter_recv(struct tevent_req *req,
						 TALLOC_CTX *mem_ctx,
						 WERROR *result);
NTSTATUS dcerpc_winspool_AsyncDeletePrinter(struct dcerpc_binding_handle *h,
					    TALLOC_CTX *mem_ctx,
					    struct policy_handle _hPrinter /* [in]  */,
					    WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncSetPrinter_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncSetPrinter *r);
NTSTATUS dcerpc_winspool_AsyncSetPrinter_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncSetPrinter_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncSetPrinter *r);
struct tevent_req *dcerpc_winspool_AsyncSetPrinter_send(TALLOC_CTX *mem_ctx,
							struct tevent_context *ev,
							struct dcerpc_binding_handle *h,
							struct policy_handle _hPrinter /* [in]  */,
							struct spoolss_SetPrinterInfoCtr *_pPrinterContainer /* [in] [ref] */,
							struct spoolss_DevmodeContainer *_pDevModeContainer /* [in] [ref] */,
							struct sec_desc_buf *_pSecurityContainer /* [in] [ref] */,
							uint32_t _Command /* [in]  */);
NTSTATUS dcerpc_winspool_AsyncSetPrinter_recv(struct tevent_req *req,
					      TALLOC_CTX *mem_ctx,
					      WERROR *result);
NTSTATUS dcerpc_winspool_AsyncSetPrinter(struct dcerpc_binding_handle *h,
					 TALLOC_CTX *mem_ctx,
					 struct policy_handle _hPrinter /* [in]  */,
					 struct spoolss_SetPrinterInfoCtr *_pPrinterContainer /* [in] [ref] */,
					 struct spoolss_DevmodeContainer *_pDevModeContainer /* [in] [ref] */,
					 struct sec_desc_buf *_pSecurityContainer /* [in] [ref] */,
					 uint32_t _Command /* [in]  */,
					 WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncGetPrinter_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncGetPrinter *r);
NTSTATUS dcerpc_winspool_AsyncGetPrinter_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncGetPrinter_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncGetPrinter *r);
struct tevent_req *dcerpc_winspool_AsyncGetPrinter_send(TALLOC_CTX *mem_ctx,
							struct tevent_context *ev,
							struct dcerpc_binding_handle *h,
							struct policy_handle _hPrinter /* [in]  */,
							uint32_t _Level /* [in]  */,
							uint8_t *_pPrinter /* [in,out] [size_is(cbBuf),unique] */,
							uint32_t _cbBuf /* [in]  */,
							uint32_t *_pcbNeeded /* [out] [ref] */);
NTSTATUS dcerpc_winspool_AsyncGetPrinter_recv(struct tevent_req *req,
					      TALLOC_CTX *mem_ctx,
					      WERROR *result);
NTSTATUS dcerpc_winspool_AsyncGetPrinter(struct dcerpc_binding_handle *h,
					 TALLOC_CTX *mem_ctx,
					 struct policy_handle _hPrinter /* [in]  */,
					 uint32_t _Level /* [in]  */,
					 uint8_t *_pPrinter /* [in,out] [size_is(cbBuf),unique] */,
					 uint32_t _cbBuf /* [in]  */,
					 uint32_t *_pcbNeeded /* [out] [ref] */,
					 WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncStartDocPrinter_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncStartDocPrinter *r);
NTSTATUS dcerpc_winspool_AsyncStartDocPrinter_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncStartDocPrinter_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncStartDocPrinter *r);
struct tevent_req *dcerpc_winspool_AsyncStartDocPrinter_send(TALLOC_CTX *mem_ctx,
							     struct tevent_context *ev,
							     struct dcerpc_binding_handle *h,
							     struct policy_handle _hPrinter /* [in]  */,
							     struct spoolss_DocumentInfoCtr *_pDocInfoContainer /* [in] [ref] */,
							     uint32_t *_pJobId /* [out] [ref] */);
NTSTATUS dcerpc_winspool_AsyncStartDocPrinter_recv(struct tevent_req *req,
						   TALLOC_CTX *mem_ctx,
						   WERROR *result);
NTSTATUS dcerpc_winspool_AsyncStartDocPrinter(struct dcerpc_binding_handle *h,
					      TALLOC_CTX *mem_ctx,
					      struct policy_handle _hPrinter /* [in]  */,
					      struct spoolss_DocumentInfoCtr *_pDocInfoContainer /* [in] [ref] */,
					      uint32_t *_pJobId /* [out] [ref] */,
					      WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncStartPagePrinter_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncStartPagePrinter *r);
NTSTATUS dcerpc_winspool_AsyncStartPagePrinter_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncStartPagePrinter_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncStartPagePrinter *r);
struct tevent_req *dcerpc_winspool_AsyncStartPagePrinter_send(TALLOC_CTX *mem_ctx,
							      struct tevent_context *ev,
							      struct dcerpc_binding_handle *h,
							      struct policy_handle _hPrinter /* [in]  */);
NTSTATUS dcerpc_winspool_AsyncStartPagePrinter_recv(struct tevent_req *req,
						    TALLOC_CTX *mem_ctx,
						    WERROR *result);
NTSTATUS dcerpc_winspool_AsyncStartPagePrinter(struct dcerpc_binding_handle *h,
					       TALLOC_CTX *mem_ctx,
					       struct policy_handle _hPrinter /* [in]  */,
					       WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncWritePrinter_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncWritePrinter *r);
NTSTATUS dcerpc_winspool_AsyncWritePrinter_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncWritePrinter_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncWritePrinter *r);
struct tevent_req *dcerpc_winspool_AsyncWritePrinter_send(TALLOC_CTX *mem_ctx,
							  struct tevent_context *ev,
							  struct dcerpc_binding_handle *h,
							  struct policy_handle _hPrinter /* [in]  */,
							  uint8_t *_pBuf /* [in] [ref,size_is(cbBuf)] */,
							  uint32_t _cbBuf /* [in]  */,
							  uint32_t *_pcWritten /* [out] [ref] */);
NTSTATUS dcerpc_winspool_AsyncWritePrinter_recv(struct tevent_req *req,
						TALLOC_CTX *mem_ctx,
						WERROR *result);
NTSTATUS dcerpc_winspool_AsyncWritePrinter(struct dcerpc_binding_handle *h,
					   TALLOC_CTX *mem_ctx,
					   struct policy_handle _hPrinter /* [in]  */,
					   uint8_t *_pBuf /* [in] [ref,size_is(cbBuf)] */,
					   uint32_t _cbBuf /* [in]  */,
					   uint32_t *_pcWritten /* [out] [ref] */,
					   WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncEndPagePrinter_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncEndPagePrinter *r);
NTSTATUS dcerpc_winspool_AsyncEndPagePrinter_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncEndPagePrinter_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncEndPagePrinter *r);
struct tevent_req *dcerpc_winspool_AsyncEndPagePrinter_send(TALLOC_CTX *mem_ctx,
							    struct tevent_context *ev,
							    struct dcerpc_binding_handle *h,
							    struct policy_handle _hPrinter /* [in]  */);
NTSTATUS dcerpc_winspool_AsyncEndPagePrinter_recv(struct tevent_req *req,
						  TALLOC_CTX *mem_ctx,
						  WERROR *result);
NTSTATUS dcerpc_winspool_AsyncEndPagePrinter(struct dcerpc_binding_handle *h,
					     TALLOC_CTX *mem_ctx,
					     struct policy_handle _hPrinter /* [in]  */,
					     WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncEndDocPrinter_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncEndDocPrinter *r);
NTSTATUS dcerpc_winspool_AsyncEndDocPrinter_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncEndDocPrinter_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncEndDocPrinter *r);
struct tevent_req *dcerpc_winspool_AsyncEndDocPrinter_send(TALLOC_CTX *mem_ctx,
							   struct tevent_context *ev,
							   struct dcerpc_binding_handle *h,
							   struct policy_handle _hPrinter /* [in]  */);
NTSTATUS dcerpc_winspool_AsyncEndDocPrinter_recv(struct tevent_req *req,
						 TALLOC_CTX *mem_ctx,
						 WERROR *result);
NTSTATUS dcerpc_winspool_AsyncEndDocPrinter(struct dcerpc_binding_handle *h,
					    TALLOC_CTX *mem_ctx,
					    struct policy_handle _hPrinter /* [in]  */,
					    WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncAbortPrinter_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncAbortPrinter *r);
NTSTATUS dcerpc_winspool_AsyncAbortPrinter_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncAbortPrinter_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncAbortPrinter *r);
struct tevent_req *dcerpc_winspool_AsyncAbortPrinter_send(TALLOC_CTX *mem_ctx,
							  struct tevent_context *ev,
							  struct dcerpc_binding_handle *h,
							  struct policy_handle _hPrinter /* [in]  */);
NTSTATUS dcerpc_winspool_AsyncAbortPrinter_recv(struct tevent_req *req,
						TALLOC_CTX *mem_ctx,
						WERROR *result);
NTSTATUS dcerpc_winspool_AsyncAbortPrinter(struct dcerpc_binding_handle *h,
					   TALLOC_CTX *mem_ctx,
					   struct policy_handle _hPrinter /* [in]  */,
					   WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncGetPrinterData_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncGetPrinterData *r);
NTSTATUS dcerpc_winspool_AsyncGetPrinterData_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncGetPrinterData_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncGetPrinterData *r);
struct tevent_req *dcerpc_winspool_AsyncGetPrinterData_send(TALLOC_CTX *mem_ctx,
							    struct tevent_context *ev,
							    struct dcerpc_binding_handle *h,
							    struct policy_handle _hPrinter /* [in]  */,
							    const char *_pValueName /* [in] [charset(UTF16),ref] */,
							    uint32_t *_pType /* [out] [ref] */,
							    uint8_t *_pData /* [out] [ref,size_is(nSize)] */,
							    uint32_t _nSize /* [in]  */,
							    uint32_t *_pcbNeeded /* [out] [ref] */);
NTSTATUS dcerpc_winspool_AsyncGetPrinterData_recv(struct tevent_req *req,
						  TALLOC_CTX *mem_ctx,
						  WERROR *result);
NTSTATUS dcerpc_winspool_AsyncGetPrinterData(struct dcerpc_binding_handle *h,
					     TALLOC_CTX *mem_ctx,
					     struct policy_handle _hPrinter /* [in]  */,
					     const char *_pValueName /* [in] [charset(UTF16),ref] */,
					     uint32_t *_pType /* [out] [ref] */,
					     uint8_t *_pData /* [out] [ref,size_is(nSize)] */,
					     uint32_t _nSize /* [in]  */,
					     uint32_t *_pcbNeeded /* [out] [ref] */,
					     WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncGetPrinterDataEx_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncGetPrinterDataEx *r);
NTSTATUS dcerpc_winspool_AsyncGetPrinterDataEx_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncGetPrinterDataEx_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncGetPrinterDataEx *r);
struct tevent_req *dcerpc_winspool_AsyncGetPrinterDataEx_send(TALLOC_CTX *mem_ctx,
							      struct tevent_context *ev,
							      struct dcerpc_binding_handle *h,
							      struct policy_handle _hPrinter /* [in]  */,
							      const char *_pKeyName /* [in] [charset(UTF16),ref] */,
							      const char *_pValueName /* [in] [charset(UTF16),ref] */,
							      uint32_t *_pType /* [out] [ref] */,
							      uint8_t *_pData /* [out] [ref,size_is(nSize)] */,
							      uint32_t _nSize /* [in]  */,
							      uint32_t *_pcbNeeded /* [out] [ref] */);
NTSTATUS dcerpc_winspool_AsyncGetPrinterDataEx_recv(struct tevent_req *req,
						    TALLOC_CTX *mem_ctx,
						    WERROR *result);
NTSTATUS dcerpc_winspool_AsyncGetPrinterDataEx(struct dcerpc_binding_handle *h,
					       TALLOC_CTX *mem_ctx,
					       struct policy_handle _hPrinter /* [in]  */,
					       const char *_pKeyName /* [in] [charset(UTF16),ref] */,
					       const char *_pValueName /* [in] [charset(UTF16),ref] */,
					       uint32_t *_pType /* [out] [ref] */,
					       uint8_t *_pData /* [out] [ref,size_is(nSize)] */,
					       uint32_t _nSize /* [in]  */,
					       uint32_t *_pcbNeeded /* [out] [ref] */,
					       WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncSetPrinterData_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncSetPrinterData *r);
NTSTATUS dcerpc_winspool_AsyncSetPrinterData_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncSetPrinterData_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncSetPrinterData *r);
struct tevent_req *dcerpc_winspool_AsyncSetPrinterData_send(TALLOC_CTX *mem_ctx,
							    struct tevent_context *ev,
							    struct dcerpc_binding_handle *h,
							    struct policy_handle _hPrinter /* [in]  */,
							    const char *_pValueName /* [in] [charset(UTF16),ref] */,
							    uint32_t _Type /* [in]  */,
							    uint8_t *_pData /* [in] [ref,size_is(cbData)] */,
							    uint32_t _cbData /* [in]  */);
NTSTATUS dcerpc_winspool_AsyncSetPrinterData_recv(struct tevent_req *req,
						  TALLOC_CTX *mem_ctx,
						  WERROR *result);
NTSTATUS dcerpc_winspool_AsyncSetPrinterData(struct dcerpc_binding_handle *h,
					     TALLOC_CTX *mem_ctx,
					     struct policy_handle _hPrinter /* [in]  */,
					     const char *_pValueName /* [in] [charset(UTF16),ref] */,
					     uint32_t _Type /* [in]  */,
					     uint8_t *_pData /* [in] [ref,size_is(cbData)] */,
					     uint32_t _cbData /* [in]  */,
					     WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncSetPrinterDataEx_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncSetPrinterDataEx *r);
NTSTATUS dcerpc_winspool_AsyncSetPrinterDataEx_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncSetPrinterDataEx_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncSetPrinterDataEx *r);
struct tevent_req *dcerpc_winspool_AsyncSetPrinterDataEx_send(TALLOC_CTX *mem_ctx,
							      struct tevent_context *ev,
							      struct dcerpc_binding_handle *h,
							      struct policy_handle _hPrinter /* [in]  */,
							      const char *_pKeyName /* [in] [charset(UTF16),ref] */,
							      const char *_pValueName /* [in] [charset(UTF16),ref] */,
							      uint32_t _Type /* [in]  */,
							      uint8_t *_pData /* [in] [ref,size_is(cbData)] */,
							      uint32_t _cbData /* [in]  */);
NTSTATUS dcerpc_winspool_AsyncSetPrinterDataEx_recv(struct tevent_req *req,
						    TALLOC_CTX *mem_ctx,
						    WERROR *result);
NTSTATUS dcerpc_winspool_AsyncSetPrinterDataEx(struct dcerpc_binding_handle *h,
					       TALLOC_CTX *mem_ctx,
					       struct policy_handle _hPrinter /* [in]  */,
					       const char *_pKeyName /* [in] [charset(UTF16),ref] */,
					       const char *_pValueName /* [in] [charset(UTF16),ref] */,
					       uint32_t _Type /* [in]  */,
					       uint8_t *_pData /* [in] [ref,size_is(cbData)] */,
					       uint32_t _cbData /* [in]  */,
					       WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncClosePrinter_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncClosePrinter *r);
NTSTATUS dcerpc_winspool_AsyncClosePrinter_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncClosePrinter_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncClosePrinter *r);
struct tevent_req *dcerpc_winspool_AsyncClosePrinter_send(TALLOC_CTX *mem_ctx,
							  struct tevent_context *ev,
							  struct dcerpc_binding_handle *h,
							  struct policy_handle *_phPrinter /* [in,out] [ref] */);
NTSTATUS dcerpc_winspool_AsyncClosePrinter_recv(struct tevent_req *req,
						TALLOC_CTX *mem_ctx,
						WERROR *result);
NTSTATUS dcerpc_winspool_AsyncClosePrinter(struct dcerpc_binding_handle *h,
					   TALLOC_CTX *mem_ctx,
					   struct policy_handle *_phPrinter /* [in,out] [ref] */,
					   WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncAddForm_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncAddForm *r);
NTSTATUS dcerpc_winspool_AsyncAddForm_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncAddForm_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncAddForm *r);
struct tevent_req *dcerpc_winspool_AsyncAddForm_send(TALLOC_CTX *mem_ctx,
						     struct tevent_context *ev,
						     struct dcerpc_binding_handle *h,
						     struct policy_handle _hPrinter /* [in]  */,
						     struct spoolss_AddFormInfoCtr *_pFormInfoContainer /* [in] [ref] */);
NTSTATUS dcerpc_winspool_AsyncAddForm_recv(struct tevent_req *req,
					   TALLOC_CTX *mem_ctx,
					   WERROR *result);
NTSTATUS dcerpc_winspool_AsyncAddForm(struct dcerpc_binding_handle *h,
				      TALLOC_CTX *mem_ctx,
				      struct policy_handle _hPrinter /* [in]  */,
				      struct spoolss_AddFormInfoCtr *_pFormInfoContainer /* [in] [ref] */,
				      WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncDeleteForm_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncDeleteForm *r);
NTSTATUS dcerpc_winspool_AsyncDeleteForm_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncDeleteForm_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncDeleteForm *r);
struct tevent_req *dcerpc_winspool_AsyncDeleteForm_send(TALLOC_CTX *mem_ctx,
							struct tevent_context *ev,
							struct dcerpc_binding_handle *h,
							struct policy_handle _hPrinter /* [in]  */,
							const char *_pFormName /* [in] [charset(UTF16),ref] */);
NTSTATUS dcerpc_winspool_AsyncDeleteForm_recv(struct tevent_req *req,
					      TALLOC_CTX *mem_ctx,
					      WERROR *result);
NTSTATUS dcerpc_winspool_AsyncDeleteForm(struct dcerpc_binding_handle *h,
					 TALLOC_CTX *mem_ctx,
					 struct policy_handle _hPrinter /* [in]  */,
					 const char *_pFormName /* [in] [charset(UTF16),ref] */,
					 WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncGetForm_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncGetForm *r);
NTSTATUS dcerpc_winspool_AsyncGetForm_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncGetForm_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncGetForm *r);
struct tevent_req *dcerpc_winspool_AsyncGetForm_send(TALLOC_CTX *mem_ctx,
						     struct tevent_context *ev,
						     struct dcerpc_binding_handle *h,
						     struct policy_handle _hPrinter /* [in]  */,
						     const char *_pFormName /* [in] [charset(UTF16),ref] */,
						     uint32_t _Level /* [in]  */,
						     uint8_t *_pForm /* [in,out] [size_is(cbBuf),unique] */,
						     uint32_t _cbBuf /* [in]  */,
						     uint32_t *_pcbNeeded /* [out] [ref] */);
NTSTATUS dcerpc_winspool_AsyncGetForm_recv(struct tevent_req *req,
					   TALLOC_CTX *mem_ctx,
					   WERROR *result);
NTSTATUS dcerpc_winspool_AsyncGetForm(struct dcerpc_binding_handle *h,
				      TALLOC_CTX *mem_ctx,
				      struct policy_handle _hPrinter /* [in]  */,
				      const char *_pFormName /* [in] [charset(UTF16),ref] */,
				      uint32_t _Level /* [in]  */,
				      uint8_t *_pForm /* [in,out] [size_is(cbBuf),unique] */,
				      uint32_t _cbBuf /* [in]  */,
				      uint32_t *_pcbNeeded /* [out] [ref] */,
				      WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncSetForm_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncSetForm *r);
NTSTATUS dcerpc_winspool_AsyncSetForm_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncSetForm_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncSetForm *r);
struct tevent_req *dcerpc_winspool_AsyncSetForm_send(TALLOC_CTX *mem_ctx,
						     struct tevent_context *ev,
						     struct dcerpc_binding_handle *h,
						     struct policy_handle _hPrinter /* [in]  */,
						     const char *_pFormName /* [in] [charset(UTF16),ref] */,
						     struct spoolss_AddFormInfoCtr *_pFormInfoContainer /* [in] [ref] */);
NTSTATUS dcerpc_winspool_AsyncSetForm_recv(struct tevent_req *req,
					   TALLOC_CTX *mem_ctx,
					   WERROR *result);
NTSTATUS dcerpc_winspool_AsyncSetForm(struct dcerpc_binding_handle *h,
				      TALLOC_CTX *mem_ctx,
				      struct policy_handle _hPrinter /* [in]  */,
				      const char *_pFormName /* [in] [charset(UTF16),ref] */,
				      struct spoolss_AddFormInfoCtr *_pFormInfoContainer /* [in] [ref] */,
				      WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncEnumForms_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncEnumForms *r);
NTSTATUS dcerpc_winspool_AsyncEnumForms_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncEnumForms_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncEnumForms *r);
struct tevent_req *dcerpc_winspool_AsyncEnumForms_send(TALLOC_CTX *mem_ctx,
						       struct tevent_context *ev,
						       struct dcerpc_binding_handle *h,
						       struct policy_handle _hPrinter /* [in]  */,
						       uint32_t _Level /* [in]  */,
						       uint8_t *_pForm /* [in,out] [size_is(cbBuf),unique] */,
						       uint32_t _cbBuf /* [in]  */,
						       uint32_t *_pcbNeeded /* [out] [ref] */,
						       uint32_t *_pcReturned /* [out] [ref] */);
NTSTATUS dcerpc_winspool_AsyncEnumForms_recv(struct tevent_req *req,
					     TALLOC_CTX *mem_ctx,
					     WERROR *result);
NTSTATUS dcerpc_winspool_AsyncEnumForms(struct dcerpc_binding_handle *h,
					TALLOC_CTX *mem_ctx,
					struct policy_handle _hPrinter /* [in]  */,
					uint32_t _Level /* [in]  */,
					uint8_t *_pForm /* [in,out] [size_is(cbBuf),unique] */,
					uint32_t _cbBuf /* [in]  */,
					uint32_t *_pcbNeeded /* [out] [ref] */,
					uint32_t *_pcReturned /* [out] [ref] */,
					WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncGetPrinterDriver_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncGetPrinterDriver *r);
NTSTATUS dcerpc_winspool_AsyncGetPrinterDriver_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncGetPrinterDriver_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncGetPrinterDriver *r);
struct tevent_req *dcerpc_winspool_AsyncGetPrinterDriver_send(TALLOC_CTX *mem_ctx,
							      struct tevent_context *ev,
							      struct dcerpc_binding_handle *h,
							      struct policy_handle _hPrinter /* [in]  */,
							      const char *_pEnvironment /* [in] [charset(UTF16),unique] */,
							      uint32_t _Level /* [in]  */,
							      uint8_t *_pDriver /* [in,out] [size_is(cbBuf),unique] */,
							      uint32_t _cbBuf /* [in]  */,
							      uint32_t *_pcbNeeded /* [out] [ref] */,
							      uint32_t _dwClientMajorVersion /* [in]  */,
							      uint32_t _dwClientMinorVersion /* [in]  */,
							      uint32_t *_pdwServerMaxVersion /* [out] [ref] */,
							      uint32_t *_pdwServerMinVersion /* [out] [ref] */);
NTSTATUS dcerpc_winspool_AsyncGetPrinterDriver_recv(struct tevent_req *req,
						    TALLOC_CTX *mem_ctx,
						    WERROR *result);
NTSTATUS dcerpc_winspool_AsyncGetPrinterDriver(struct dcerpc_binding_handle *h,
					       TALLOC_CTX *mem_ctx,
					       struct policy_handle _hPrinter /* [in]  */,
					       const char *_pEnvironment /* [in] [charset(UTF16),unique] */,
					       uint32_t _Level /* [in]  */,
					       uint8_t *_pDriver /* [in,out] [size_is(cbBuf),unique] */,
					       uint32_t _cbBuf /* [in]  */,
					       uint32_t *_pcbNeeded /* [out] [ref] */,
					       uint32_t _dwClientMajorVersion /* [in]  */,
					       uint32_t _dwClientMinorVersion /* [in]  */,
					       uint32_t *_pdwServerMaxVersion /* [out] [ref] */,
					       uint32_t *_pdwServerMinVersion /* [out] [ref] */,
					       WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncEnumPrinterData_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncEnumPrinterData *r);
NTSTATUS dcerpc_winspool_AsyncEnumPrinterData_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncEnumPrinterData_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncEnumPrinterData *r);
struct tevent_req *dcerpc_winspool_AsyncEnumPrinterData_send(TALLOC_CTX *mem_ctx,
							     struct tevent_context *ev,
							     struct dcerpc_binding_handle *h,
							     struct policy_handle _hPrinter /* [in]  */,
							     uint32_t _dwIndex /* [in]  */,
							     uint16_t *_pValueName /* [out] [ref,size_is(cbValueName/2)] */,
							     uint32_t _cbValueName /* [in]  */,
							     uint32_t *_pcbValueName /* [out] [ref] */,
							     uint32_t *_pType /* [out] [ref] */,
							     uint8_t *_pData /* [out] [ref,size_is(cbData)] */,
							     uint32_t _cbData /* [in]  */,
							     uint32_t *_pcbData /* [out] [ref] */);
NTSTATUS dcerpc_winspool_AsyncEnumPrinterData_recv(struct tevent_req *req,
						   TALLOC_CTX *mem_ctx,
						   WERROR *result);
NTSTATUS dcerpc_winspool_AsyncEnumPrinterData(struct dcerpc_binding_handle *h,
					      TALLOC_CTX *mem_ctx,
					      struct policy_handle _hPrinter /* [in]  */,
					      uint32_t _dwIndex /* [in]  */,
					      uint16_t *_pValueName /* [out] [ref,size_is(cbValueName/2)] */,
					      uint32_t _cbValueName /* [in]  */,
					      uint32_t *_pcbValueName /* [out] [ref] */,
					      uint32_t *_pType /* [out] [ref] */,
					      uint8_t *_pData /* [out] [ref,size_is(cbData)] */,
					      uint32_t _cbData /* [in]  */,
					      uint32_t *_pcbData /* [out] [ref] */,
					      WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncEnumPrinterDataEx_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncEnumPrinterDataEx *r);
NTSTATUS dcerpc_winspool_AsyncEnumPrinterDataEx_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncEnumPrinterDataEx_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncEnumPrinterDataEx *r);
struct tevent_req *dcerpc_winspool_AsyncEnumPrinterDataEx_send(TALLOC_CTX *mem_ctx,
							       struct tevent_context *ev,
							       struct dcerpc_binding_handle *h,
							       struct policy_handle _hPrinter /* [in]  */,
							       const char *_pKeyName /* [in] [charset(UTF16),ref] */,
							       uint8_t *_pEnumValues /* [out] [ref,size_is(cbEnumValues)] */,
							       uint32_t _cbEnumValues /* [in]  */,
							       uint32_t *_pcbEnumValues /* [out] [ref] */,
							       uint32_t *_pnEnumValues /* [out] [ref] */);
NTSTATUS dcerpc_winspool_AsyncEnumPrinterDataEx_recv(struct tevent_req *req,
						     TALLOC_CTX *mem_ctx,
						     WERROR *result);
NTSTATUS dcerpc_winspool_AsyncEnumPrinterDataEx(struct dcerpc_binding_handle *h,
						TALLOC_CTX *mem_ctx,
						struct policy_handle _hPrinter /* [in]  */,
						const char *_pKeyName /* [in] [charset(UTF16),ref] */,
						uint8_t *_pEnumValues /* [out] [ref,size_is(cbEnumValues)] */,
						uint32_t _cbEnumValues /* [in]  */,
						uint32_t *_pcbEnumValues /* [out] [ref] */,
						uint32_t *_pnEnumValues /* [out] [ref] */,
						WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncEnumPrinterKey_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncEnumPrinterKey *r);
NTSTATUS dcerpc_winspool_AsyncEnumPrinterKey_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncEnumPrinterKey_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncEnumPrinterKey *r);
struct tevent_req *dcerpc_winspool_AsyncEnumPrinterKey_send(TALLOC_CTX *mem_ctx,
							    struct tevent_context *ev,
							    struct dcerpc_binding_handle *h,
							    struct policy_handle _hPrinter /* [in]  */,
							    const char *_pKeyName /* [in] [charset(UTF16),ref] */,
							    uint16_t *_pSubkey /* [out] [ref,size_is(cbSubkey/2)] */,
							    uint32_t _cbSubkey /* [in]  */,
							    uint32_t *_pcbSubkey /* [out] [ref] */);
NTSTATUS dcerpc_winspool_AsyncEnumPrinterKey_recv(struct tevent_req *req,
						  TALLOC_CTX *mem_ctx,
						  WERROR *result);
NTSTATUS dcerpc_winspool_AsyncEnumPrinterKey(struct dcerpc_binding_handle *h,
					     TALLOC_CTX *mem_ctx,
					     struct policy_handle _hPrinter /* [in]  */,
					     const char *_pKeyName /* [in] [charset(UTF16),ref] */,
					     uint16_t *_pSubkey /* [out] [ref,size_is(cbSubkey/2)] */,
					     uint32_t _cbSubkey /* [in]  */,
					     uint32_t *_pcbSubkey /* [out] [ref] */,
					     WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncDeletePrinterData_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncDeletePrinterData *r);
NTSTATUS dcerpc_winspool_AsyncDeletePrinterData_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncDeletePrinterData_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncDeletePrinterData *r);
struct tevent_req *dcerpc_winspool_AsyncDeletePrinterData_send(TALLOC_CTX *mem_ctx,
							       struct tevent_context *ev,
							       struct dcerpc_binding_handle *h,
							       struct policy_handle _hPrinter /* [in]  */,
							       const char *_pValueName /* [in] [charset(UTF16),ref] */);
NTSTATUS dcerpc_winspool_AsyncDeletePrinterData_recv(struct tevent_req *req,
						     TALLOC_CTX *mem_ctx,
						     WERROR *result);
NTSTATUS dcerpc_winspool_AsyncDeletePrinterData(struct dcerpc_binding_handle *h,
						TALLOC_CTX *mem_ctx,
						struct policy_handle _hPrinter /* [in]  */,
						const char *_pValueName /* [in] [charset(UTF16),ref] */,
						WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncDeletePrinterDataEx_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncDeletePrinterDataEx *r);
NTSTATUS dcerpc_winspool_AsyncDeletePrinterDataEx_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncDeletePrinterDataEx_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncDeletePrinterDataEx *r);
struct tevent_req *dcerpc_winspool_AsyncDeletePrinterDataEx_send(TALLOC_CTX *mem_ctx,
								 struct tevent_context *ev,
								 struct dcerpc_binding_handle *h,
								 struct policy_handle _hPrinter /* [in]  */,
								 const char *_pKeyName /* [in] [charset(UTF16),ref] */,
								 const char *_pValueName /* [in] [charset(UTF16),ref] */);
NTSTATUS dcerpc_winspool_AsyncDeletePrinterDataEx_recv(struct tevent_req *req,
						       TALLOC_CTX *mem_ctx,
						       WERROR *result);
NTSTATUS dcerpc_winspool_AsyncDeletePrinterDataEx(struct dcerpc_binding_handle *h,
						  TALLOC_CTX *mem_ctx,
						  struct policy_handle _hPrinter /* [in]  */,
						  const char *_pKeyName /* [in] [charset(UTF16),ref] */,
						  const char *_pValueName /* [in] [charset(UTF16),ref] */,
						  WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncDeletePrinterKey_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncDeletePrinterKey *r);
NTSTATUS dcerpc_winspool_AsyncDeletePrinterKey_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncDeletePrinterKey_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncDeletePrinterKey *r);
struct tevent_req *dcerpc_winspool_AsyncDeletePrinterKey_send(TALLOC_CTX *mem_ctx,
							      struct tevent_context *ev,
							      struct dcerpc_binding_handle *h,
							      struct policy_handle _hPrinter /* [in]  */,
							      const char *_pKeyName /* [in] [charset(UTF16),ref] */);
NTSTATUS dcerpc_winspool_AsyncDeletePrinterKey_recv(struct tevent_req *req,
						    TALLOC_CTX *mem_ctx,
						    WERROR *result);
NTSTATUS dcerpc_winspool_AsyncDeletePrinterKey(struct dcerpc_binding_handle *h,
					       TALLOC_CTX *mem_ctx,
					       struct policy_handle _hPrinter /* [in]  */,
					       const char *_pKeyName /* [in] [charset(UTF16),ref] */,
					       WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncXcvData_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncXcvData *r);
NTSTATUS dcerpc_winspool_AsyncXcvData_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncXcvData_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncXcvData *r);
struct tevent_req *dcerpc_winspool_AsyncXcvData_send(TALLOC_CTX *mem_ctx,
						     struct tevent_context *ev,
						     struct dcerpc_binding_handle *h,
						     struct policy_handle _hXcv /* [in]  */,
						     const char *_pszDataName /* [in] [charset(UTF16),ref] */,
						     uint8_t *_pInputData /* [in] [ref,size_is(cbInputData)] */,
						     uint32_t _cbInputData /* [in]  */,
						     uint8_t *_pOutputData /* [out] [ref,size_is(cbOutputData)] */,
						     uint32_t _cbOutputData /* [in]  */,
						     uint32_t *_pcbOutputNeeded /* [out] [ref] */,
						     uint32_t *_pdwStatus /* [in,out] [ref] */);
NTSTATUS dcerpc_winspool_AsyncXcvData_recv(struct tevent_req *req,
					   TALLOC_CTX *mem_ctx,
					   WERROR *result);
NTSTATUS dcerpc_winspool_AsyncXcvData(struct dcerpc_binding_handle *h,
				      TALLOC_CTX *mem_ctx,
				      struct policy_handle _hXcv /* [in]  */,
				      const char *_pszDataName /* [in] [charset(UTF16),ref] */,
				      uint8_t *_pInputData /* [in] [ref,size_is(cbInputData)] */,
				      uint32_t _cbInputData /* [in]  */,
				      uint8_t *_pOutputData /* [out] [ref,size_is(cbOutputData)] */,
				      uint32_t _cbOutputData /* [in]  */,
				      uint32_t *_pcbOutputNeeded /* [out] [ref] */,
				      uint32_t *_pdwStatus /* [in,out] [ref] */,
				      WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncSendRecvBidiData_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncSendRecvBidiData *r);
NTSTATUS dcerpc_winspool_AsyncSendRecvBidiData_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncSendRecvBidiData_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncSendRecvBidiData *r);
struct tevent_req *dcerpc_winspool_AsyncSendRecvBidiData_send(TALLOC_CTX *mem_ctx,
							      struct tevent_context *ev,
							      struct dcerpc_binding_handle *h,
							      struct policy_handle _hPrinter /* [in]  */,
							      const char *_pAction /* [in] [charset(UTF16),unique] */,
							      struct RPC_BIDI_REQUEST_CONTAINER *_pReqData /* [in] [ref] */,
							      struct RPC_BIDI_RESPONSE_CONTAINER **_ppRespData /* [out] [ref] */);
NTSTATUS dcerpc_winspool_AsyncSendRecvBidiData_recv(struct tevent_req *req,
						    TALLOC_CTX *mem_ctx,
						    WERROR *result);
NTSTATUS dcerpc_winspool_AsyncSendRecvBidiData(struct dcerpc_binding_handle *h,
					       TALLOC_CTX *mem_ctx,
					       struct policy_handle _hPrinter /* [in]  */,
					       const char *_pAction /* [in] [charset(UTF16),unique] */,
					       struct RPC_BIDI_REQUEST_CONTAINER *_pReqData /* [in] [ref] */,
					       struct RPC_BIDI_RESPONSE_CONTAINER **_ppRespData /* [out] [ref] */,
					       WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncCreatePrinterIC_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncCreatePrinterIC *r);
NTSTATUS dcerpc_winspool_AsyncCreatePrinterIC_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncCreatePrinterIC_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncCreatePrinterIC *r);
struct tevent_req *dcerpc_winspool_AsyncCreatePrinterIC_send(TALLOC_CTX *mem_ctx,
							     struct tevent_context *ev,
							     struct dcerpc_binding_handle *h,
							     struct policy_handle _hPrinter /* [in]  */,
							     struct policy_handle *_pHandle /* [out] [ref] */,
							     struct spoolss_DevmodeContainer *_pDevModeContainer /* [in] [ref] */);
NTSTATUS dcerpc_winspool_AsyncCreatePrinterIC_recv(struct tevent_req *req,
						   TALLOC_CTX *mem_ctx,
						   WERROR *result);
NTSTATUS dcerpc_winspool_AsyncCreatePrinterIC(struct dcerpc_binding_handle *h,
					      TALLOC_CTX *mem_ctx,
					      struct policy_handle _hPrinter /* [in]  */,
					      struct policy_handle *_pHandle /* [out] [ref] */,
					      struct spoolss_DevmodeContainer *_pDevModeContainer /* [in] [ref] */,
					      WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncPlayGdiScriptOnPrinterIC_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncPlayGdiScriptOnPrinterIC *r);
NTSTATUS dcerpc_winspool_AsyncPlayGdiScriptOnPrinterIC_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncPlayGdiScriptOnPrinterIC_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncPlayGdiScriptOnPrinterIC *r);
struct tevent_req *dcerpc_winspool_AsyncPlayGdiScriptOnPrinterIC_send(TALLOC_CTX *mem_ctx,
								      struct tevent_context *ev,
								      struct dcerpc_binding_handle *h,
								      struct policy_handle _hPrinterIC /* [in]  */,
								      uint8_t *_pIn /* [in] [ref,size_is(cIn)] */,
								      uint32_t _cIn /* [in]  */,
								      uint8_t *_pOut /* [out] [ref,size_is(cOut)] */,
								      uint32_t _cOut /* [in]  */,
								      uint32_t _ul /* [in]  */);
NTSTATUS dcerpc_winspool_AsyncPlayGdiScriptOnPrinterIC_recv(struct tevent_req *req,
							    TALLOC_CTX *mem_ctx,
							    WERROR *result);
NTSTATUS dcerpc_winspool_AsyncPlayGdiScriptOnPrinterIC(struct dcerpc_binding_handle *h,
						       TALLOC_CTX *mem_ctx,
						       struct policy_handle _hPrinterIC /* [in]  */,
						       uint8_t *_pIn /* [in] [ref,size_is(cIn)] */,
						       uint32_t _cIn /* [in]  */,
						       uint8_t *_pOut /* [out] [ref,size_is(cOut)] */,
						       uint32_t _cOut /* [in]  */,
						       uint32_t _ul /* [in]  */,
						       WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncDeletePrinterIC_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncDeletePrinterIC *r);
NTSTATUS dcerpc_winspool_AsyncDeletePrinterIC_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncDeletePrinterIC_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncDeletePrinterIC *r);
struct tevent_req *dcerpc_winspool_AsyncDeletePrinterIC_send(TALLOC_CTX *mem_ctx,
							     struct tevent_context *ev,
							     struct dcerpc_binding_handle *h,
							     struct policy_handle *_phPrinterIC /* [in,out] [ref] */);
NTSTATUS dcerpc_winspool_AsyncDeletePrinterIC_recv(struct tevent_req *req,
						   TALLOC_CTX *mem_ctx,
						   WERROR *result);
NTSTATUS dcerpc_winspool_AsyncDeletePrinterIC(struct dcerpc_binding_handle *h,
					      TALLOC_CTX *mem_ctx,
					      struct policy_handle *_phPrinterIC /* [in,out] [ref] */,
					      WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncEnumPrinters_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncEnumPrinters *r);
NTSTATUS dcerpc_winspool_AsyncEnumPrinters_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncEnumPrinters_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncEnumPrinters *r);
struct tevent_req *dcerpc_winspool_AsyncEnumPrinters_send(TALLOC_CTX *mem_ctx,
							  struct tevent_context *ev,
							  struct dcerpc_binding_handle *h,
							  uint32_t _Flags /* [in]  */,
							  const char *_pName /* [in] [charset(UTF16),unique] */,
							  uint32_t _Level /* [in]  */,
							  uint8_t *_pPrinterEnum /* [in,out] [size_is(cbBuf),unique] */,
							  uint32_t _cbBuf /* [in]  */,
							  uint32_t *_pcbNeeded /* [out] [ref] */,
							  uint32_t *_pcReturned /* [out] [ref] */);
NTSTATUS dcerpc_winspool_AsyncEnumPrinters_recv(struct tevent_req *req,
						TALLOC_CTX *mem_ctx,
						WERROR *result);
NTSTATUS dcerpc_winspool_AsyncEnumPrinters(struct dcerpc_binding_handle *h,
					   TALLOC_CTX *mem_ctx,
					   uint32_t _Flags /* [in]  */,
					   const char *_pName /* [in] [charset(UTF16),unique] */,
					   uint32_t _Level /* [in]  */,
					   uint8_t *_pPrinterEnum /* [in,out] [size_is(cbBuf),unique] */,
					   uint32_t _cbBuf /* [in]  */,
					   uint32_t *_pcbNeeded /* [out] [ref] */,
					   uint32_t *_pcReturned /* [out] [ref] */,
					   WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncAddPrinterDriver_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncAddPrinterDriver *r);
NTSTATUS dcerpc_winspool_AsyncAddPrinterDriver_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncAddPrinterDriver_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncAddPrinterDriver *r);
struct tevent_req *dcerpc_winspool_AsyncAddPrinterDriver_send(TALLOC_CTX *mem_ctx,
							      struct tevent_context *ev,
							      struct dcerpc_binding_handle *h,
							      const char *_pName /* [in] [charset(UTF16),unique] */,
							      struct spoolss_AddDriverInfoCtr *_pDriverContainer /* [in] [ref] */,
							      uint32_t _dwFileCopyFlags /* [in]  */);
NTSTATUS dcerpc_winspool_AsyncAddPrinterDriver_recv(struct tevent_req *req,
						    TALLOC_CTX *mem_ctx,
						    WERROR *result);
NTSTATUS dcerpc_winspool_AsyncAddPrinterDriver(struct dcerpc_binding_handle *h,
					       TALLOC_CTX *mem_ctx,
					       const char *_pName /* [in] [charset(UTF16),unique] */,
					       struct spoolss_AddDriverInfoCtr *_pDriverContainer /* [in] [ref] */,
					       uint32_t _dwFileCopyFlags /* [in]  */,
					       WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncEnumPrinterDrivers_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncEnumPrinterDrivers *r);
NTSTATUS dcerpc_winspool_AsyncEnumPrinterDrivers_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncEnumPrinterDrivers_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncEnumPrinterDrivers *r);
struct tevent_req *dcerpc_winspool_AsyncEnumPrinterDrivers_send(TALLOC_CTX *mem_ctx,
								struct tevent_context *ev,
								struct dcerpc_binding_handle *h,
								const char *_pName /* [in] [charset(UTF16),unique] */,
								const char *_pEnvironment /* [in] [charset(UTF16),unique] */,
								uint32_t _Level /* [in]  */,
								uint8_t *_pDrivers /* [in,out] [size_is(cbBuf),unique] */,
								uint32_t _cbBuf /* [in]  */,
								uint32_t *_pcbNeeded /* [out] [ref] */,
								uint32_t *_pcReturned /* [out] [ref] */);
NTSTATUS dcerpc_winspool_AsyncEnumPrinterDrivers_recv(struct tevent_req *req,
						      TALLOC_CTX *mem_ctx,
						      WERROR *result);
NTSTATUS dcerpc_winspool_AsyncEnumPrinterDrivers(struct dcerpc_binding_handle *h,
						 TALLOC_CTX *mem_ctx,
						 const char *_pName /* [in] [charset(UTF16),unique] */,
						 const char *_pEnvironment /* [in] [charset(UTF16),unique] */,
						 uint32_t _Level /* [in]  */,
						 uint8_t *_pDrivers /* [in,out] [size_is(cbBuf),unique] */,
						 uint32_t _cbBuf /* [in]  */,
						 uint32_t *_pcbNeeded /* [out] [ref] */,
						 uint32_t *_pcReturned /* [out] [ref] */,
						 WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncGetPrinterDriverDirectory_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncGetPrinterDriverDirectory *r);
NTSTATUS dcerpc_winspool_AsyncGetPrinterDriverDirectory_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncGetPrinterDriverDirectory_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncGetPrinterDriverDirectory *r);
struct tevent_req *dcerpc_winspool_AsyncGetPrinterDriverDirectory_send(TALLOC_CTX *mem_ctx,
								       struct tevent_context *ev,
								       struct dcerpc_binding_handle *h,
								       const char *_pName /* [in] [charset(UTF16),unique] */,
								       const char *_pEnvironment /* [in] [charset(UTF16),unique] */,
								       uint32_t _Level /* [in]  */,
								       uint8_t *_pDriverDirectory /* [in,out] [size_is(cbBuf),unique] */,
								       uint32_t _cbBuf /* [in]  */,
								       uint32_t *_pcbNeeded /* [out] [ref] */);
NTSTATUS dcerpc_winspool_AsyncGetPrinterDriverDirectory_recv(struct tevent_req *req,
							     TALLOC_CTX *mem_ctx,
							     WERROR *result);
NTSTATUS dcerpc_winspool_AsyncGetPrinterDriverDirectory(struct dcerpc_binding_handle *h,
							TALLOC_CTX *mem_ctx,
							const char *_pName /* [in] [charset(UTF16),unique] */,
							const char *_pEnvironment /* [in] [charset(UTF16),unique] */,
							uint32_t _Level /* [in]  */,
							uint8_t *_pDriverDirectory /* [in,out] [size_is(cbBuf),unique] */,
							uint32_t _cbBuf /* [in]  */,
							uint32_t *_pcbNeeded /* [out] [ref] */,
							WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncDeletePrinterDriver_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncDeletePrinterDriver *r);
NTSTATUS dcerpc_winspool_AsyncDeletePrinterDriver_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncDeletePrinterDriver_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncDeletePrinterDriver *r);
struct tevent_req *dcerpc_winspool_AsyncDeletePrinterDriver_send(TALLOC_CTX *mem_ctx,
								 struct tevent_context *ev,
								 struct dcerpc_binding_handle *h,
								 const char *_pName /* [in] [charset(UTF16),unique] */,
								 const char *_pEnvironment /* [in] [charset(UTF16),ref] */,
								 const char *_pDriverName /* [in] [charset(UTF16),ref] */);
NTSTATUS dcerpc_winspool_AsyncDeletePrinterDriver_recv(struct tevent_req *req,
						       TALLOC_CTX *mem_ctx,
						       WERROR *result);
NTSTATUS dcerpc_winspool_AsyncDeletePrinterDriver(struct dcerpc_binding_handle *h,
						  TALLOC_CTX *mem_ctx,
						  const char *_pName /* [in] [charset(UTF16),unique] */,
						  const char *_pEnvironment /* [in] [charset(UTF16),ref] */,
						  const char *_pDriverName /* [in] [charset(UTF16),ref] */,
						  WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncDeletePrinterDriverEx_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncDeletePrinterDriverEx *r);
NTSTATUS dcerpc_winspool_AsyncDeletePrinterDriverEx_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncDeletePrinterDriverEx_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncDeletePrinterDriverEx *r);
struct tevent_req *dcerpc_winspool_AsyncDeletePrinterDriverEx_send(TALLOC_CTX *mem_ctx,
								   struct tevent_context *ev,
								   struct dcerpc_binding_handle *h,
								   const char *_pName /* [in] [charset(UTF16),unique] */,
								   const char *_pEnvironment /* [in] [charset(UTF16),ref] */,
								   const char *_pDriverName /* [in] [charset(UTF16),ref] */,
								   uint32_t _dwDeleteFlag /* [in]  */,
								   uint32_t _dwVersionNum /* [in]  */);
NTSTATUS dcerpc_winspool_AsyncDeletePrinterDriverEx_recv(struct tevent_req *req,
							 TALLOC_CTX *mem_ctx,
							 WERROR *result);
NTSTATUS dcerpc_winspool_AsyncDeletePrinterDriverEx(struct dcerpc_binding_handle *h,
						    TALLOC_CTX *mem_ctx,
						    const char *_pName /* [in] [charset(UTF16),unique] */,
						    const char *_pEnvironment /* [in] [charset(UTF16),ref] */,
						    const char *_pDriverName /* [in] [charset(UTF16),ref] */,
						    uint32_t _dwDeleteFlag /* [in]  */,
						    uint32_t _dwVersionNum /* [in]  */,
						    WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncAddPrintProcessor_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncAddPrintProcessor *r);
NTSTATUS dcerpc_winspool_AsyncAddPrintProcessor_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncAddPrintProcessor_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncAddPrintProcessor *r);
struct tevent_req *dcerpc_winspool_AsyncAddPrintProcessor_send(TALLOC_CTX *mem_ctx,
							       struct tevent_context *ev,
							       struct dcerpc_binding_handle *h,
							       const char *_pName /* [in] [charset(UTF16),unique] */,
							       const char *_pEnvironment /* [in] [charset(UTF16),ref] */,
							       const char *_pPathName /* [in] [charset(UTF16),ref] */,
							       const char *_pPrintProcessorName /* [in] [charset(UTF16),ref] */);
NTSTATUS dcerpc_winspool_AsyncAddPrintProcessor_recv(struct tevent_req *req,
						     TALLOC_CTX *mem_ctx,
						     WERROR *result);
NTSTATUS dcerpc_winspool_AsyncAddPrintProcessor(struct dcerpc_binding_handle *h,
						TALLOC_CTX *mem_ctx,
						const char *_pName /* [in] [charset(UTF16),unique] */,
						const char *_pEnvironment /* [in] [charset(UTF16),ref] */,
						const char *_pPathName /* [in] [charset(UTF16),ref] */,
						const char *_pPrintProcessorName /* [in] [charset(UTF16),ref] */,
						WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncEnumPrintProcessors_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncEnumPrintProcessors *r);
NTSTATUS dcerpc_winspool_AsyncEnumPrintProcessors_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncEnumPrintProcessors_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncEnumPrintProcessors *r);
struct tevent_req *dcerpc_winspool_AsyncEnumPrintProcessors_send(TALLOC_CTX *mem_ctx,
								 struct tevent_context *ev,
								 struct dcerpc_binding_handle *h,
								 const char *_pName /* [in] [charset(UTF16),unique] */,
								 const char *_pEnvironment /* [in] [charset(UTF16),unique] */,
								 uint32_t _Level /* [in]  */,
								 uint8_t *_pPrintProcessorInfo /* [in,out] [size_is(cbBuf),unique] */,
								 uint32_t _cbBuf /* [in]  */,
								 uint32_t *_pcbNeeded /* [out] [ref] */,
								 uint32_t *_pcReturned /* [out] [ref] */);
NTSTATUS dcerpc_winspool_AsyncEnumPrintProcessors_recv(struct tevent_req *req,
						       TALLOC_CTX *mem_ctx,
						       WERROR *result);
NTSTATUS dcerpc_winspool_AsyncEnumPrintProcessors(struct dcerpc_binding_handle *h,
						  TALLOC_CTX *mem_ctx,
						  const char *_pName /* [in] [charset(UTF16),unique] */,
						  const char *_pEnvironment /* [in] [charset(UTF16),unique] */,
						  uint32_t _Level /* [in]  */,
						  uint8_t *_pPrintProcessorInfo /* [in,out] [size_is(cbBuf),unique] */,
						  uint32_t _cbBuf /* [in]  */,
						  uint32_t *_pcbNeeded /* [out] [ref] */,
						  uint32_t *_pcReturned /* [out] [ref] */,
						  WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncGetPrintProcessorDirectory_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncGetPrintProcessorDirectory *r);
NTSTATUS dcerpc_winspool_AsyncGetPrintProcessorDirectory_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncGetPrintProcessorDirectory_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncGetPrintProcessorDirectory *r);
struct tevent_req *dcerpc_winspool_AsyncGetPrintProcessorDirectory_send(TALLOC_CTX *mem_ctx,
									struct tevent_context *ev,
									struct dcerpc_binding_handle *h,
									const char *_pName /* [in] [charset(UTF16),unique] */,
									const char *_pEnvironment /* [in] [charset(UTF16),unique] */,
									uint32_t _Level /* [in]  */,
									uint8_t *_pPrintProcessorDirectory /* [in,out] [size_is(cbBuf),unique] */,
									uint32_t _cbBuf /* [in]  */,
									uint32_t *_pcbNeeded /* [out] [ref] */);
NTSTATUS dcerpc_winspool_AsyncGetPrintProcessorDirectory_recv(struct tevent_req *req,
							      TALLOC_CTX *mem_ctx,
							      WERROR *result);
NTSTATUS dcerpc_winspool_AsyncGetPrintProcessorDirectory(struct dcerpc_binding_handle *h,
							 TALLOC_CTX *mem_ctx,
							 const char *_pName /* [in] [charset(UTF16),unique] */,
							 const char *_pEnvironment /* [in] [charset(UTF16),unique] */,
							 uint32_t _Level /* [in]  */,
							 uint8_t *_pPrintProcessorDirectory /* [in,out] [size_is(cbBuf),unique] */,
							 uint32_t _cbBuf /* [in]  */,
							 uint32_t *_pcbNeeded /* [out] [ref] */,
							 WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncEnumPorts_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncEnumPorts *r);
NTSTATUS dcerpc_winspool_AsyncEnumPorts_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncEnumPorts_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncEnumPorts *r);
struct tevent_req *dcerpc_winspool_AsyncEnumPorts_send(TALLOC_CTX *mem_ctx,
						       struct tevent_context *ev,
						       struct dcerpc_binding_handle *h,
						       const char *_pName /* [in] [charset(UTF16),unique] */,
						       uint32_t _Level /* [in]  */,
						       uint8_t *_pPort /* [in,out] [size_is(cbBuf),unique] */,
						       uint32_t _cbBuf /* [in]  */,
						       uint32_t *_pcbNeeded /* [out] [ref] */,
						       uint32_t *_pcReturned /* [out] [ref] */);
NTSTATUS dcerpc_winspool_AsyncEnumPorts_recv(struct tevent_req *req,
					     TALLOC_CTX *mem_ctx,
					     WERROR *result);
NTSTATUS dcerpc_winspool_AsyncEnumPorts(struct dcerpc_binding_handle *h,
					TALLOC_CTX *mem_ctx,
					const char *_pName /* [in] [charset(UTF16),unique] */,
					uint32_t _Level /* [in]  */,
					uint8_t *_pPort /* [in,out] [size_is(cbBuf),unique] */,
					uint32_t _cbBuf /* [in]  */,
					uint32_t *_pcbNeeded /* [out] [ref] */,
					uint32_t *_pcReturned /* [out] [ref] */,
					WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncEnumMonitors_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncEnumMonitors *r);
NTSTATUS dcerpc_winspool_AsyncEnumMonitors_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncEnumMonitors_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncEnumMonitors *r);
struct tevent_req *dcerpc_winspool_AsyncEnumMonitors_send(TALLOC_CTX *mem_ctx,
							  struct tevent_context *ev,
							  struct dcerpc_binding_handle *h,
							  const char *_pName /* [in] [charset(UTF16),unique] */,
							  uint32_t _Level /* [in]  */,
							  uint8_t *_pMonitor /* [in,out] [size_is(cbBuf),unique] */,
							  uint32_t _cbBuf /* [in]  */,
							  uint32_t *_pcbNeeded /* [out] [ref] */,
							  uint32_t *_pcReturned /* [out] [ref] */);
NTSTATUS dcerpc_winspool_AsyncEnumMonitors_recv(struct tevent_req *req,
						TALLOC_CTX *mem_ctx,
						WERROR *result);
NTSTATUS dcerpc_winspool_AsyncEnumMonitors(struct dcerpc_binding_handle *h,
					   TALLOC_CTX *mem_ctx,
					   const char *_pName /* [in] [charset(UTF16),unique] */,
					   uint32_t _Level /* [in]  */,
					   uint8_t *_pMonitor /* [in,out] [size_is(cbBuf),unique] */,
					   uint32_t _cbBuf /* [in]  */,
					   uint32_t *_pcbNeeded /* [out] [ref] */,
					   uint32_t *_pcReturned /* [out] [ref] */,
					   WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncAddPort_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncAddPort *r);
NTSTATUS dcerpc_winspool_AsyncAddPort_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncAddPort_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncAddPort *r);
struct tevent_req *dcerpc_winspool_AsyncAddPort_send(TALLOC_CTX *mem_ctx,
						     struct tevent_context *ev,
						     struct dcerpc_binding_handle *h,
						     const char *_pName /* [in] [charset(UTF16),unique] */,
						     struct spoolss_SetPortInfoContainer *_pPortContainer /* [in] [ref] */,
						     struct spoolss_PortVarContainer *_pPortVarContainer /* [in] [ref] */,
						     const char *_pMonitorName /* [in] [charset(UTF16),ref] */);
NTSTATUS dcerpc_winspool_AsyncAddPort_recv(struct tevent_req *req,
					   TALLOC_CTX *mem_ctx,
					   WERROR *result);
NTSTATUS dcerpc_winspool_AsyncAddPort(struct dcerpc_binding_handle *h,
				      TALLOC_CTX *mem_ctx,
				      const char *_pName /* [in] [charset(UTF16),unique] */,
				      struct spoolss_SetPortInfoContainer *_pPortContainer /* [in] [ref] */,
				      struct spoolss_PortVarContainer *_pPortVarContainer /* [in] [ref] */,
				      const char *_pMonitorName /* [in] [charset(UTF16),ref] */,
				      WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncSetPort_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncSetPort *r);
NTSTATUS dcerpc_winspool_AsyncSetPort_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncSetPort_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncSetPort *r);
struct tevent_req *dcerpc_winspool_AsyncSetPort_send(TALLOC_CTX *mem_ctx,
						     struct tevent_context *ev,
						     struct dcerpc_binding_handle *h,
						     const char *_pName /* [in] [charset(UTF16),unique] */,
						     const char *_pPortName /* [in] [charset(UTF16),unique] */,
						     struct spoolss_SetPortInfoContainer *_pPortContainer /* [in] [ref] */);
NTSTATUS dcerpc_winspool_AsyncSetPort_recv(struct tevent_req *req,
					   TALLOC_CTX *mem_ctx,
					   WERROR *result);
NTSTATUS dcerpc_winspool_AsyncSetPort(struct dcerpc_binding_handle *h,
				      TALLOC_CTX *mem_ctx,
				      const char *_pName /* [in] [charset(UTF16),unique] */,
				      const char *_pPortName /* [in] [charset(UTF16),unique] */,
				      struct spoolss_SetPortInfoContainer *_pPortContainer /* [in] [ref] */,
				      WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncAddMonitor_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncAddMonitor *r);
NTSTATUS dcerpc_winspool_AsyncAddMonitor_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncAddMonitor_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncAddMonitor *r);
struct tevent_req *dcerpc_winspool_AsyncAddMonitor_send(TALLOC_CTX *mem_ctx,
							struct tevent_context *ev,
							struct dcerpc_binding_handle *h,
							const char *_Name /* [in] [charset(UTF16),unique] */,
							struct spoolss_MonitorContainer *_pMonitorContainer /* [in] [ref] */);
NTSTATUS dcerpc_winspool_AsyncAddMonitor_recv(struct tevent_req *req,
					      TALLOC_CTX *mem_ctx,
					      WERROR *result);
NTSTATUS dcerpc_winspool_AsyncAddMonitor(struct dcerpc_binding_handle *h,
					 TALLOC_CTX *mem_ctx,
					 const char *_Name /* [in] [charset(UTF16),unique] */,
					 struct spoolss_MonitorContainer *_pMonitorContainer /* [in] [ref] */,
					 WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncDeleteMonitor_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncDeleteMonitor *r);
NTSTATUS dcerpc_winspool_AsyncDeleteMonitor_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncDeleteMonitor_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncDeleteMonitor *r);
struct tevent_req *dcerpc_winspool_AsyncDeleteMonitor_send(TALLOC_CTX *mem_ctx,
							   struct tevent_context *ev,
							   struct dcerpc_binding_handle *h,
							   const char *_Name /* [in] [charset(UTF16),unique] */,
							   const char *_pEnvironment /* [in] [charset(UTF16),unique] */,
							   const char *_pMonitorName /* [in] [charset(UTF16),ref] */);
NTSTATUS dcerpc_winspool_AsyncDeleteMonitor_recv(struct tevent_req *req,
						 TALLOC_CTX *mem_ctx,
						 WERROR *result);
NTSTATUS dcerpc_winspool_AsyncDeleteMonitor(struct dcerpc_binding_handle *h,
					    TALLOC_CTX *mem_ctx,
					    const char *_Name /* [in] [charset(UTF16),unique] */,
					    const char *_pEnvironment /* [in] [charset(UTF16),unique] */,
					    const char *_pMonitorName /* [in] [charset(UTF16),ref] */,
					    WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncDeletePrintProcessor_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncDeletePrintProcessor *r);
NTSTATUS dcerpc_winspool_AsyncDeletePrintProcessor_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncDeletePrintProcessor_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncDeletePrintProcessor *r);
struct tevent_req *dcerpc_winspool_AsyncDeletePrintProcessor_send(TALLOC_CTX *mem_ctx,
								  struct tevent_context *ev,
								  struct dcerpc_binding_handle *h,
								  const char *_Name /* [in] [charset(UTF16),unique] */,
								  const char *_pEnvironment /* [in] [charset(UTF16),unique] */,
								  const char *_pPrintProcessorName /* [in] [charset(UTF16),ref] */);
NTSTATUS dcerpc_winspool_AsyncDeletePrintProcessor_recv(struct tevent_req *req,
							TALLOC_CTX *mem_ctx,
							WERROR *result);
NTSTATUS dcerpc_winspool_AsyncDeletePrintProcessor(struct dcerpc_binding_handle *h,
						   TALLOC_CTX *mem_ctx,
						   const char *_Name /* [in] [charset(UTF16),unique] */,
						   const char *_pEnvironment /* [in] [charset(UTF16),unique] */,
						   const char *_pPrintProcessorName /* [in] [charset(UTF16),ref] */,
						   WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncEnumPrintProcessorDatatypes_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncEnumPrintProcessorDatatypes *r);
NTSTATUS dcerpc_winspool_AsyncEnumPrintProcessorDatatypes_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncEnumPrintProcessorDatatypes_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncEnumPrintProcessorDatatypes *r);
struct tevent_req *dcerpc_winspool_AsyncEnumPrintProcessorDatatypes_send(TALLOC_CTX *mem_ctx,
									 struct tevent_context *ev,
									 struct dcerpc_binding_handle *h,
									 const char *_pName /* [in] [charset(UTF16),unique] */,
									 const char *_pPrintProcessorName /* [in] [charset(UTF16),unique] */,
									 uint32_t _Level /* [in]  */,
									 uint8_t *_pDatatypes /* [in,out] [size_is(cbBuf),unique] */,
									 uint32_t _cbBuf /* [in]  */,
									 uint32_t *_pcbNeeded /* [out] [ref] */,
									 uint32_t *_pcReturned /* [out] [ref] */);
NTSTATUS dcerpc_winspool_AsyncEnumPrintProcessorDatatypes_recv(struct tevent_req *req,
							       TALLOC_CTX *mem_ctx,
							       WERROR *result);
NTSTATUS dcerpc_winspool_AsyncEnumPrintProcessorDatatypes(struct dcerpc_binding_handle *h,
							  TALLOC_CTX *mem_ctx,
							  const char *_pName /* [in] [charset(UTF16),unique] */,
							  const char *_pPrintProcessorName /* [in] [charset(UTF16),unique] */,
							  uint32_t _Level /* [in]  */,
							  uint8_t *_pDatatypes /* [in,out] [size_is(cbBuf),unique] */,
							  uint32_t _cbBuf /* [in]  */,
							  uint32_t *_pcbNeeded /* [out] [ref] */,
							  uint32_t *_pcReturned /* [out] [ref] */,
							  WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncAddPerMachineConnection_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncAddPerMachineConnection *r);
NTSTATUS dcerpc_winspool_AsyncAddPerMachineConnection_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncAddPerMachineConnection_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncAddPerMachineConnection *r);
struct tevent_req *dcerpc_winspool_AsyncAddPerMachineConnection_send(TALLOC_CTX *mem_ctx,
								     struct tevent_context *ev,
								     struct dcerpc_binding_handle *h,
								     const char *_pServer /* [in] [charset(UTF16),unique] */,
								     const char *_pPrinterName /* [in] [charset(UTF16),ref] */,
								     const char *_pPrintServer /* [in] [charset(UTF16),ref] */,
								     const char *_pProvider /* [in] [charset(UTF16),ref] */);
NTSTATUS dcerpc_winspool_AsyncAddPerMachineConnection_recv(struct tevent_req *req,
							   TALLOC_CTX *mem_ctx,
							   WERROR *result);
NTSTATUS dcerpc_winspool_AsyncAddPerMachineConnection(struct dcerpc_binding_handle *h,
						      TALLOC_CTX *mem_ctx,
						      const char *_pServer /* [in] [charset(UTF16),unique] */,
						      const char *_pPrinterName /* [in] [charset(UTF16),ref] */,
						      const char *_pPrintServer /* [in] [charset(UTF16),ref] */,
						      const char *_pProvider /* [in] [charset(UTF16),ref] */,
						      WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncDeletePerMachineConnection_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncDeletePerMachineConnection *r);
NTSTATUS dcerpc_winspool_AsyncDeletePerMachineConnection_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncDeletePerMachineConnection_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncDeletePerMachineConnection *r);
struct tevent_req *dcerpc_winspool_AsyncDeletePerMachineConnection_send(TALLOC_CTX *mem_ctx,
									struct tevent_context *ev,
									struct dcerpc_binding_handle *h,
									const char *_pServer /* [in] [charset(UTF16),unique] */,
									const char *_pPrinterName /* [in] [charset(UTF16),ref] */);
NTSTATUS dcerpc_winspool_AsyncDeletePerMachineConnection_recv(struct tevent_req *req,
							      TALLOC_CTX *mem_ctx,
							      WERROR *result);
NTSTATUS dcerpc_winspool_AsyncDeletePerMachineConnection(struct dcerpc_binding_handle *h,
							 TALLOC_CTX *mem_ctx,
							 const char *_pServer /* [in] [charset(UTF16),unique] */,
							 const char *_pPrinterName /* [in] [charset(UTF16),ref] */,
							 WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncEnumPerMachineConnections_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncEnumPerMachineConnections *r);
NTSTATUS dcerpc_winspool_AsyncEnumPerMachineConnections_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncEnumPerMachineConnections_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncEnumPerMachineConnections *r);
struct tevent_req *dcerpc_winspool_AsyncEnumPerMachineConnections_send(TALLOC_CTX *mem_ctx,
								       struct tevent_context *ev,
								       struct dcerpc_binding_handle *h,
								       const char *_pServer /* [in] [charset(UTF16),unique] */,
								       uint8_t *_pPrinterEnum /* [in,out] [size_is(cbBuf),unique] */,
								       uint32_t _cbBuf /* [in]  */,
								       uint32_t *_pcbNeeded /* [out] [ref] */,
								       uint32_t *_pcReturned /* [out] [ref] */);
NTSTATUS dcerpc_winspool_AsyncEnumPerMachineConnections_recv(struct tevent_req *req,
							     TALLOC_CTX *mem_ctx,
							     WERROR *result);
NTSTATUS dcerpc_winspool_AsyncEnumPerMachineConnections(struct dcerpc_binding_handle *h,
							TALLOC_CTX *mem_ctx,
							const char *_pServer /* [in] [charset(UTF16),unique] */,
							uint8_t *_pPrinterEnum /* [in,out] [size_is(cbBuf),unique] */,
							uint32_t _cbBuf /* [in]  */,
							uint32_t *_pcbNeeded /* [out] [ref] */,
							uint32_t *_pcReturned /* [out] [ref] */,
							WERROR *result);

struct tevent_req *dcerpc_winspool_SyncRegisterForRemoteNotifications_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_SyncRegisterForRemoteNotifications *r);
NTSTATUS dcerpc_winspool_SyncRegisterForRemoteNotifications_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_SyncRegisterForRemoteNotifications_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_SyncRegisterForRemoteNotifications *r);
struct tevent_req *dcerpc_winspool_SyncRegisterForRemoteNotifications_send(TALLOC_CTX *mem_ctx,
									   struct tevent_context *ev,
									   struct dcerpc_binding_handle *h,
									   struct policy_handle _hPrinter /* [in]  */,
									   struct winspool_PrintPropertiesCollection *_pNotifyFilter /* [in] [ref] */,
									   struct policy_handle *_phRpcHandle /* [out] [ref] */);
NTSTATUS dcerpc_winspool_SyncRegisterForRemoteNotifications_recv(struct tevent_req *req,
								 TALLOC_CTX *mem_ctx,
								 HRESULT *result);
NTSTATUS dcerpc_winspool_SyncRegisterForRemoteNotifications(struct dcerpc_binding_handle *h,
							    TALLOC_CTX *mem_ctx,
							    struct policy_handle _hPrinter /* [in]  */,
							    struct winspool_PrintPropertiesCollection *_pNotifyFilter /* [in] [ref] */,
							    struct policy_handle *_phRpcHandle /* [out] [ref] */,
							    HRESULT *result);

struct tevent_req *dcerpc_winspool_SyncUnRegisterForRemoteNotifications_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_SyncUnRegisterForRemoteNotifications *r);
NTSTATUS dcerpc_winspool_SyncUnRegisterForRemoteNotifications_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_SyncUnRegisterForRemoteNotifications_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_SyncUnRegisterForRemoteNotifications *r);
struct tevent_req *dcerpc_winspool_SyncUnRegisterForRemoteNotifications_send(TALLOC_CTX *mem_ctx,
									     struct tevent_context *ev,
									     struct dcerpc_binding_handle *h,
									     struct policy_handle *_phRpcHandle /* [in,out] [ref] */);
NTSTATUS dcerpc_winspool_SyncUnRegisterForRemoteNotifications_recv(struct tevent_req *req,
								   TALLOC_CTX *mem_ctx,
								   HRESULT *result);
NTSTATUS dcerpc_winspool_SyncUnRegisterForRemoteNotifications(struct dcerpc_binding_handle *h,
							      TALLOC_CTX *mem_ctx,
							      struct policy_handle *_phRpcHandle /* [in,out] [ref] */,
							      HRESULT *result);

struct tevent_req *dcerpc_winspool_SyncRefreshRemoteNotifications_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_SyncRefreshRemoteNotifications *r);
NTSTATUS dcerpc_winspool_SyncRefreshRemoteNotifications_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_SyncRefreshRemoteNotifications_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_SyncRefreshRemoteNotifications *r);
struct tevent_req *dcerpc_winspool_SyncRefreshRemoteNotifications_send(TALLOC_CTX *mem_ctx,
								       struct tevent_context *ev,
								       struct dcerpc_binding_handle *h,
								       struct policy_handle _hRpcHandle /* [in]  */,
								       struct winspool_PrintPropertiesCollection *_pNotifyFilter /* [in] [ref] */,
								       struct winspool_PrintPropertiesCollection **_ppNotifyData /* [out] [ref] */);
NTSTATUS dcerpc_winspool_SyncRefreshRemoteNotifications_recv(struct tevent_req *req,
							     TALLOC_CTX *mem_ctx,
							     HRESULT *result);
NTSTATUS dcerpc_winspool_SyncRefreshRemoteNotifications(struct dcerpc_binding_handle *h,
							TALLOC_CTX *mem_ctx,
							struct policy_handle _hRpcHandle /* [in]  */,
							struct winspool_PrintPropertiesCollection *_pNotifyFilter /* [in] [ref] */,
							struct winspool_PrintPropertiesCollection **_ppNotifyData /* [out] [ref] */,
							HRESULT *result);

struct tevent_req *dcerpc_winspool_AsyncGetRemoteNotifications_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncGetRemoteNotifications *r);
NTSTATUS dcerpc_winspool_AsyncGetRemoteNotifications_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncGetRemoteNotifications_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncGetRemoteNotifications *r);
struct tevent_req *dcerpc_winspool_AsyncGetRemoteNotifications_send(TALLOC_CTX *mem_ctx,
								    struct tevent_context *ev,
								    struct dcerpc_binding_handle *h,
								    struct policy_handle _hRpcHandle /* [in]  */,
								    struct winspool_PrintPropertiesCollection **_ppNotifyData /* [out] [ref] */);
NTSTATUS dcerpc_winspool_AsyncGetRemoteNotifications_recv(struct tevent_req *req,
							  TALLOC_CTX *mem_ctx,
							  HRESULT *result);
NTSTATUS dcerpc_winspool_AsyncGetRemoteNotifications(struct dcerpc_binding_handle *h,
						     TALLOC_CTX *mem_ctx,
						     struct policy_handle _hRpcHandle /* [in]  */,
						     struct winspool_PrintPropertiesCollection **_ppNotifyData /* [out] [ref] */,
						     HRESULT *result);

struct tevent_req *dcerpc_winspool_AsyncInstallPrinterDriverFromPackage_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncInstallPrinterDriverFromPackage *r);
NTSTATUS dcerpc_winspool_AsyncInstallPrinterDriverFromPackage_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncInstallPrinterDriverFromPackage_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncInstallPrinterDriverFromPackage *r);
struct tevent_req *dcerpc_winspool_AsyncInstallPrinterDriverFromPackage_send(TALLOC_CTX *mem_ctx,
									     struct tevent_context *ev,
									     struct dcerpc_binding_handle *h,
									     const char *_pszServer /* [in] [charset(UTF16),unique] */,
									     const char *_pszInfPath /* [in] [charset(UTF16),unique] */,
									     const char *_pszDriverName /* [in] [charset(UTF16),ref] */,
									     const char *_pszEnvironment /* [in] [charset(UTF16),ref] */,
									     uint32_t _dwFlags /* [in]  */);
NTSTATUS dcerpc_winspool_AsyncInstallPrinterDriverFromPackage_recv(struct tevent_req *req,
								   TALLOC_CTX *mem_ctx,
								   HRESULT *result);
NTSTATUS dcerpc_winspool_AsyncInstallPrinterDriverFromPackage(struct dcerpc_binding_handle *h,
							      TALLOC_CTX *mem_ctx,
							      const char *_pszServer /* [in] [charset(UTF16),unique] */,
							      const char *_pszInfPath /* [in] [charset(UTF16),unique] */,
							      const char *_pszDriverName /* [in] [charset(UTF16),ref] */,
							      const char *_pszEnvironment /* [in] [charset(UTF16),ref] */,
							      uint32_t _dwFlags /* [in]  */,
							      HRESULT *result);

struct tevent_req *dcerpc_winspool_AsyncUploadPrinterDriverPackage_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncUploadPrinterDriverPackage *r);
NTSTATUS dcerpc_winspool_AsyncUploadPrinterDriverPackage_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncUploadPrinterDriverPackage_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncUploadPrinterDriverPackage *r);
struct tevent_req *dcerpc_winspool_AsyncUploadPrinterDriverPackage_send(TALLOC_CTX *mem_ctx,
									struct tevent_context *ev,
									struct dcerpc_binding_handle *h,
									const char *_pszServer /* [in] [charset(UTF16),unique] */,
									const char *_pszInfPath /* [in] [charset(UTF16),ref] */,
									const char *_pszEnvironment /* [in] [charset(UTF16),ref] */,
									uint32_t _dwFlags /* [in]  */,
									uint16_t *_pszDestInfPath /* [in,out] [size_is(*pcchDestInfPath),unique] */,
									uint32_t *_pcchDestInfPath /* [in,out] [ref] */);
NTSTATUS dcerpc_winspool_AsyncUploadPrinterDriverPackage_recv(struct tevent_req *req,
							      TALLOC_CTX *mem_ctx,
							      HRESULT *result);
NTSTATUS dcerpc_winspool_AsyncUploadPrinterDriverPackage(struct dcerpc_binding_handle *h,
							 TALLOC_CTX *mem_ctx,
							 const char *_pszServer /* [in] [charset(UTF16),unique] */,
							 const char *_pszInfPath /* [in] [charset(UTF16),ref] */,
							 const char *_pszEnvironment /* [in] [charset(UTF16),ref] */,
							 uint32_t _dwFlags /* [in]  */,
							 uint16_t *_pszDestInfPath /* [in,out] [size_is(*pcchDestInfPath),unique] */,
							 uint32_t *_pcchDestInfPath /* [in,out] [ref] */,
							 HRESULT *result);

struct tevent_req *dcerpc_winspool_AsyncGetCorePrinterDrivers_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncGetCorePrinterDrivers *r);
NTSTATUS dcerpc_winspool_AsyncGetCorePrinterDrivers_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncGetCorePrinterDrivers_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncGetCorePrinterDrivers *r);
struct tevent_req *dcerpc_winspool_AsyncGetCorePrinterDrivers_send(TALLOC_CTX *mem_ctx,
								   struct tevent_context *ev,
								   struct dcerpc_binding_handle *h,
								   const char *_pszServer /* [in] [charset(UTF16),unique] */,
								   const char *_pszEnvironment /* [in] [charset(UTF16),ref] */,
								   uint32_t _cchCoreDrivers /* [in]  */,
								   uint16_t *_pszzCoreDriverDependencies /* [in] [ref,size_is(cchCoreDrivers)] */,
								   uint32_t _cCorePrinterDrivers /* [in]  */,
								   struct spoolss_CorePrinterDriver *_pCorePrinterDrivers /* [out] [ref,size_is(cCorePrinterDrivers)] */);
NTSTATUS dcerpc_winspool_AsyncGetCorePrinterDrivers_recv(struct tevent_req *req,
							 TALLOC_CTX *mem_ctx,
							 HRESULT *result);
NTSTATUS dcerpc_winspool_AsyncGetCorePrinterDrivers(struct dcerpc_binding_handle *h,
						    TALLOC_CTX *mem_ctx,
						    const char *_pszServer /* [in] [charset(UTF16),unique] */,
						    const char *_pszEnvironment /* [in] [charset(UTF16),ref] */,
						    uint32_t _cchCoreDrivers /* [in]  */,
						    uint16_t *_pszzCoreDriverDependencies /* [in] [ref,size_is(cchCoreDrivers)] */,
						    uint32_t _cCorePrinterDrivers /* [in]  */,
						    struct spoolss_CorePrinterDriver *_pCorePrinterDrivers /* [out] [ref,size_is(cCorePrinterDrivers)] */,
						    HRESULT *result);

struct tevent_req *dcerpc_winspool_AsyncCorePrinterDriverInstalled_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncCorePrinterDriverInstalled *r);
NTSTATUS dcerpc_winspool_AsyncCorePrinterDriverInstalled_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncCorePrinterDriverInstalled_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncCorePrinterDriverInstalled *r);
struct tevent_req *dcerpc_winspool_AsyncCorePrinterDriverInstalled_send(TALLOC_CTX *mem_ctx,
									struct tevent_context *ev,
									struct dcerpc_binding_handle *h,
									const char *_pszServer /* [in] [charset(UTF16),unique] */,
									const char *_pszEnvironment /* [in] [charset(UTF16),ref] */,
									struct GUID _CoreDriverGUID /* [in]  */,
									NTTIME _ftDriverDate /* [in]  */,
									uint64_t _dwlDriverVersion /* [in]  */,
									int32_t *_pbDriverInstalled /* [out] [ref] */);
NTSTATUS dcerpc_winspool_AsyncCorePrinterDriverInstalled_recv(struct tevent_req *req,
							      TALLOC_CTX *mem_ctx,
							      HRESULT *result);
NTSTATUS dcerpc_winspool_AsyncCorePrinterDriverInstalled(struct dcerpc_binding_handle *h,
							 TALLOC_CTX *mem_ctx,
							 const char *_pszServer /* [in] [charset(UTF16),unique] */,
							 const char *_pszEnvironment /* [in] [charset(UTF16),ref] */,
							 struct GUID _CoreDriverGUID /* [in]  */,
							 NTTIME _ftDriverDate /* [in]  */,
							 uint64_t _dwlDriverVersion /* [in]  */,
							 int32_t *_pbDriverInstalled /* [out] [ref] */,
							 HRESULT *result);

struct tevent_req *dcerpc_winspool_AsyncGetPrinterDriverPackagePath_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncGetPrinterDriverPackagePath *r);
NTSTATUS dcerpc_winspool_AsyncGetPrinterDriverPackagePath_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncGetPrinterDriverPackagePath_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncGetPrinterDriverPackagePath *r);
struct tevent_req *dcerpc_winspool_AsyncGetPrinterDriverPackagePath_send(TALLOC_CTX *mem_ctx,
									 struct tevent_context *ev,
									 struct dcerpc_binding_handle *h,
									 const char *_pszServer /* [in] [charset(UTF16),unique] */,
									 const char *_pszEnvironment /* [in] [charset(UTF16),ref] */,
									 const char *_pszLanguage /* [in] [charset(UTF16),unique] */,
									 const char *_pszPackageID /* [in] [charset(UTF16),ref] */,
									 uint16_t *_pszDriverPackageCab /* [in,out] [size_is(cchDriverPackageCab),unique] */,
									 uint32_t _cchDriverPackageCab /* [in]  */,
									 uint32_t *_pcchRequiredSize /* [out] [ref] */);
NTSTATUS dcerpc_winspool_AsyncGetPrinterDriverPackagePath_recv(struct tevent_req *req,
							       TALLOC_CTX *mem_ctx,
							       HRESULT *result);
NTSTATUS dcerpc_winspool_AsyncGetPrinterDriverPackagePath(struct dcerpc_binding_handle *h,
							  TALLOC_CTX *mem_ctx,
							  const char *_pszServer /* [in] [charset(UTF16),unique] */,
							  const char *_pszEnvironment /* [in] [charset(UTF16),ref] */,
							  const char *_pszLanguage /* [in] [charset(UTF16),unique] */,
							  const char *_pszPackageID /* [in] [charset(UTF16),ref] */,
							  uint16_t *_pszDriverPackageCab /* [in,out] [size_is(cchDriverPackageCab),unique] */,
							  uint32_t _cchDriverPackageCab /* [in]  */,
							  uint32_t *_pcchRequiredSize /* [out] [ref] */,
							  HRESULT *result);

struct tevent_req *dcerpc_winspool_AsyncDeletePrinterDriverPackage_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncDeletePrinterDriverPackage *r);
NTSTATUS dcerpc_winspool_AsyncDeletePrinterDriverPackage_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncDeletePrinterDriverPackage_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncDeletePrinterDriverPackage *r);
struct tevent_req *dcerpc_winspool_AsyncDeletePrinterDriverPackage_send(TALLOC_CTX *mem_ctx,
									struct tevent_context *ev,
									struct dcerpc_binding_handle *h,
									const char *_pszServer /* [in] [charset(UTF16),unique] */,
									const char *_pszInfPath /* [in] [charset(UTF16),ref] */,
									const char *_pszEnvironment /* [in] [charset(UTF16),ref] */);
NTSTATUS dcerpc_winspool_AsyncDeletePrinterDriverPackage_recv(struct tevent_req *req,
							      TALLOC_CTX *mem_ctx,
							      HRESULT *result);
NTSTATUS dcerpc_winspool_AsyncDeletePrinterDriverPackage(struct dcerpc_binding_handle *h,
							 TALLOC_CTX *mem_ctx,
							 const char *_pszServer /* [in] [charset(UTF16),unique] */,
							 const char *_pszInfPath /* [in] [charset(UTF16),ref] */,
							 const char *_pszEnvironment /* [in] [charset(UTF16),ref] */,
							 HRESULT *result);

struct tevent_req *dcerpc_winspool_AsyncReadPrinter_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncReadPrinter *r);
NTSTATUS dcerpc_winspool_AsyncReadPrinter_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncReadPrinter_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncReadPrinter *r);
struct tevent_req *dcerpc_winspool_AsyncReadPrinter_send(TALLOC_CTX *mem_ctx,
							 struct tevent_context *ev,
							 struct dcerpc_binding_handle *h,
							 struct policy_handle _hPrinter /* [in]  */,
							 uint8_t *_pBuf /* [out] [ref,size_is(cbBuf)] */,
							 uint32_t _cbBuf /* [in]  */,
							 uint32_t *_pcNoBytesRead /* [out] [ref] */);
NTSTATUS dcerpc_winspool_AsyncReadPrinter_recv(struct tevent_req *req,
					       TALLOC_CTX *mem_ctx,
					       WERROR *result);
NTSTATUS dcerpc_winspool_AsyncReadPrinter(struct dcerpc_binding_handle *h,
					  TALLOC_CTX *mem_ctx,
					  struct policy_handle _hPrinter /* [in]  */,
					  uint8_t *_pBuf /* [out] [ref,size_is(cbBuf)] */,
					  uint32_t _cbBuf /* [in]  */,
					  uint32_t *_pcNoBytesRead /* [out] [ref] */,
					  WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncResetPrinter_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncResetPrinter *r);
NTSTATUS dcerpc_winspool_AsyncResetPrinter_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncResetPrinter_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncResetPrinter *r);
struct tevent_req *dcerpc_winspool_AsyncResetPrinter_send(TALLOC_CTX *mem_ctx,
							  struct tevent_context *ev,
							  struct dcerpc_binding_handle *h,
							  struct policy_handle _hPrinter /* [in]  */,
							  const char *_pDatatype /* [in] [charset(UTF16),unique] */,
							  struct spoolss_DevmodeContainer *_pDevModeContainer /* [in] [ref] */);
NTSTATUS dcerpc_winspool_AsyncResetPrinter_recv(struct tevent_req *req,
						TALLOC_CTX *mem_ctx,
						WERROR *result);
NTSTATUS dcerpc_winspool_AsyncResetPrinter(struct dcerpc_binding_handle *h,
					   TALLOC_CTX *mem_ctx,
					   struct policy_handle _hPrinter /* [in]  */,
					   const char *_pDatatype /* [in] [charset(UTF16),unique] */,
					   struct spoolss_DevmodeContainer *_pDevModeContainer /* [in] [ref] */,
					   WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncGetJobNamedPropertyValue_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncGetJobNamedPropertyValue *r);
NTSTATUS dcerpc_winspool_AsyncGetJobNamedPropertyValue_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncGetJobNamedPropertyValue_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncGetJobNamedPropertyValue *r);
struct tevent_req *dcerpc_winspool_AsyncGetJobNamedPropertyValue_send(TALLOC_CTX *mem_ctx,
								      struct tevent_context *ev,
								      struct dcerpc_binding_handle *h,
								      struct policy_handle _hPrinter /* [in]  */,
								      uint32_t _JobId /* [in]  */,
								      const char *_pszName /* [in] [charset(UTF16),ref] */,
								      struct spoolss_PrintPropertyValue *_pValue /* [out] [ref] */);
NTSTATUS dcerpc_winspool_AsyncGetJobNamedPropertyValue_recv(struct tevent_req *req,
							    TALLOC_CTX *mem_ctx,
							    WERROR *result);
NTSTATUS dcerpc_winspool_AsyncGetJobNamedPropertyValue(struct dcerpc_binding_handle *h,
						       TALLOC_CTX *mem_ctx,
						       struct policy_handle _hPrinter /* [in]  */,
						       uint32_t _JobId /* [in]  */,
						       const char *_pszName /* [in] [charset(UTF16),ref] */,
						       struct spoolss_PrintPropertyValue *_pValue /* [out] [ref] */,
						       WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncSetJobNamedProperty_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncSetJobNamedProperty *r);
NTSTATUS dcerpc_winspool_AsyncSetJobNamedProperty_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncSetJobNamedProperty_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncSetJobNamedProperty *r);
struct tevent_req *dcerpc_winspool_AsyncSetJobNamedProperty_send(TALLOC_CTX *mem_ctx,
								 struct tevent_context *ev,
								 struct dcerpc_binding_handle *h,
								 struct policy_handle _hPrinter /* [in]  */,
								 uint32_t _JobId /* [in]  */,
								 struct spoolss_PrintNamedProperty *_pProperty /* [in] [ref] */);
NTSTATUS dcerpc_winspool_AsyncSetJobNamedProperty_recv(struct tevent_req *req,
						       TALLOC_CTX *mem_ctx,
						       WERROR *result);
NTSTATUS dcerpc_winspool_AsyncSetJobNamedProperty(struct dcerpc_binding_handle *h,
						  TALLOC_CTX *mem_ctx,
						  struct policy_handle _hPrinter /* [in]  */,
						  uint32_t _JobId /* [in]  */,
						  struct spoolss_PrintNamedProperty *_pProperty /* [in] [ref] */,
						  WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncDeleteJobNamedProperty_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncDeleteJobNamedProperty *r);
NTSTATUS dcerpc_winspool_AsyncDeleteJobNamedProperty_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncDeleteJobNamedProperty_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncDeleteJobNamedProperty *r);
struct tevent_req *dcerpc_winspool_AsyncDeleteJobNamedProperty_send(TALLOC_CTX *mem_ctx,
								    struct tevent_context *ev,
								    struct dcerpc_binding_handle *h,
								    struct policy_handle _hPrinter /* [in]  */,
								    uint32_t _JobId /* [in]  */,
								    const char *_pszName /* [in] [charset(UTF16),ref] */);
NTSTATUS dcerpc_winspool_AsyncDeleteJobNamedProperty_recv(struct tevent_req *req,
							  TALLOC_CTX *mem_ctx,
							  WERROR *result);
NTSTATUS dcerpc_winspool_AsyncDeleteJobNamedProperty(struct dcerpc_binding_handle *h,
						     TALLOC_CTX *mem_ctx,
						     struct policy_handle _hPrinter /* [in]  */,
						     uint32_t _JobId /* [in]  */,
						     const char *_pszName /* [in] [charset(UTF16),ref] */,
						     WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncEnumJobNamedProperties_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncEnumJobNamedProperties *r);
NTSTATUS dcerpc_winspool_AsyncEnumJobNamedProperties_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncEnumJobNamedProperties_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncEnumJobNamedProperties *r);
struct tevent_req *dcerpc_winspool_AsyncEnumJobNamedProperties_send(TALLOC_CTX *mem_ctx,
								    struct tevent_context *ev,
								    struct dcerpc_binding_handle *h,
								    struct policy_handle _hPrinter /* [in]  */,
								    uint32_t _JobId /* [in]  */,
								    uint32_t *_pcProperties /* [out] [ref] */,
								    struct spoolss_PrintNamedProperty **_ppProperties /* [out] [ref,size_is(,*pcProperties)] */);
NTSTATUS dcerpc_winspool_AsyncEnumJobNamedProperties_recv(struct tevent_req *req,
							  TALLOC_CTX *mem_ctx,
							  WERROR *result);
NTSTATUS dcerpc_winspool_AsyncEnumJobNamedProperties(struct dcerpc_binding_handle *h,
						     TALLOC_CTX *mem_ctx,
						     struct policy_handle _hPrinter /* [in]  */,
						     uint32_t _JobId /* [in]  */,
						     uint32_t *_pcProperties /* [out] [ref] */,
						     struct spoolss_PrintNamedProperty **_ppProperties /* [out] [ref,size_is(,*pcProperties)] */,
						     WERROR *result);

struct tevent_req *dcerpc_winspool_AsyncLogJobInfoForBranchOffice_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winspool_AsyncLogJobInfoForBranchOffice *r);
NTSTATUS dcerpc_winspool_AsyncLogJobInfoForBranchOffice_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winspool_AsyncLogJobInfoForBranchOffice_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winspool_AsyncLogJobInfoForBranchOffice *r);
struct tevent_req *dcerpc_winspool_AsyncLogJobInfoForBranchOffice_send(TALLOC_CTX *mem_ctx,
								       struct tevent_context *ev,
								       struct dcerpc_binding_handle *h,
								       struct policy_handle _hPrinter /* [in]  */,
								       struct spoolss_BranchOfficeJobDataContainer *_pBranchOfficeJobDataContainer /* [in] [ref] */);
NTSTATUS dcerpc_winspool_AsyncLogJobInfoForBranchOffice_recv(struct tevent_req *req,
							     TALLOC_CTX *mem_ctx,
							     WERROR *result);
NTSTATUS dcerpc_winspool_AsyncLogJobInfoForBranchOffice(struct dcerpc_binding_handle *h,
							TALLOC_CTX *mem_ctx,
							struct policy_handle _hPrinter /* [in]  */,
							struct spoolss_BranchOfficeJobDataContainer *_pBranchOfficeJobDataContainer /* [in] [ref] */,
							WERROR *result);

#endif /* _HEADER_RPC_iremotewinspool */
