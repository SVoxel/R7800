#include "librpc/rpc/dcerpc.h"
#include "bin/default/source4/librpc/gen_ndr/winsif.h"
#ifndef _HEADER_RPC_winsif
#define _HEADER_RPC_winsif

extern const struct ndr_interface_table ndr_table_winsif;

struct tevent_req *dcerpc_winsif_WinsRecordAction_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winsif_WinsRecordAction *r);
NTSTATUS dcerpc_winsif_WinsRecordAction_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winsif_WinsRecordAction_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winsif_WinsRecordAction *r);
struct tevent_req *dcerpc_winsif_WinsRecordAction_send(TALLOC_CTX *mem_ctx,
						       struct tevent_context *ev,
						       struct dcerpc_binding_handle *h,
						       struct winsif_RecordAction **_record_action /* [in,out] [ref] */);
NTSTATUS dcerpc_winsif_WinsRecordAction_recv(struct tevent_req *req,
					     TALLOC_CTX *mem_ctx,
					     WERROR *result);
NTSTATUS dcerpc_winsif_WinsRecordAction(struct dcerpc_binding_handle *h,
					TALLOC_CTX *mem_ctx,
					struct winsif_RecordAction **_record_action /* [in,out] [ref] */,
					WERROR *result);

struct tevent_req *dcerpc_winsif_WinsStatus_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winsif_WinsStatus *r);
NTSTATUS dcerpc_winsif_WinsStatus_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winsif_WinsStatus_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winsif_WinsStatus *r);
struct tevent_req *dcerpc_winsif_WinsStatus_send(TALLOC_CTX *mem_ctx,
						 struct tevent_context *ev,
						 struct dcerpc_binding_handle *h,
						 enum winsif_StatusCmd _cmd /* [in]  */,
						 struct winsif_Results *_results /* [in,out] [ref] */);
NTSTATUS dcerpc_winsif_WinsStatus_recv(struct tevent_req *req,
				       TALLOC_CTX *mem_ctx,
				       WERROR *result);
NTSTATUS dcerpc_winsif_WinsStatus(struct dcerpc_binding_handle *h,
				  TALLOC_CTX *mem_ctx,
				  enum winsif_StatusCmd _cmd /* [in]  */,
				  struct winsif_Results *_results /* [in,out] [ref] */,
				  WERROR *result);

struct tevent_req *dcerpc_winsif_WinsTrigger_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winsif_WinsTrigger *r);
NTSTATUS dcerpc_winsif_WinsTrigger_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winsif_WinsTrigger_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winsif_WinsTrigger *r);
struct tevent_req *dcerpc_winsif_WinsTrigger_send(TALLOC_CTX *mem_ctx,
						  struct tevent_context *ev,
						  struct dcerpc_binding_handle *h,
						  struct winsif_Address *_owner_address /* [in] [ref] */,
						  enum winsif_TriggerType _trigger_type /* [in]  */);
NTSTATUS dcerpc_winsif_WinsTrigger_recv(struct tevent_req *req,
					TALLOC_CTX *mem_ctx,
					WERROR *result);
NTSTATUS dcerpc_winsif_WinsTrigger(struct dcerpc_binding_handle *h,
				   TALLOC_CTX *mem_ctx,
				   struct winsif_Address *_owner_address /* [in] [ref] */,
				   enum winsif_TriggerType _trigger_type /* [in]  */,
				   WERROR *result);

struct tevent_req *dcerpc_winsif_WinsDoStaticInit_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winsif_WinsDoStaticInit *r);
NTSTATUS dcerpc_winsif_WinsDoStaticInit_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winsif_WinsDoStaticInit_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winsif_WinsDoStaticInit *r);
struct tevent_req *dcerpc_winsif_WinsDoStaticInit_send(TALLOC_CTX *mem_ctx,
						       struct tevent_context *ev,
						       struct dcerpc_binding_handle *h,
						       const char *_data_file_path /* [in] [charset(UTF16),unique] */,
						       uint32_t _delete_file /* [in]  */);
NTSTATUS dcerpc_winsif_WinsDoStaticInit_recv(struct tevent_req *req,
					     TALLOC_CTX *mem_ctx,
					     WERROR *result);
NTSTATUS dcerpc_winsif_WinsDoStaticInit(struct dcerpc_binding_handle *h,
					TALLOC_CTX *mem_ctx,
					const char *_data_file_path /* [in] [charset(UTF16),unique] */,
					uint32_t _delete_file /* [in]  */,
					WERROR *result);

struct tevent_req *dcerpc_winsif_WinsDoScavenging_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winsif_WinsDoScavenging *r);
NTSTATUS dcerpc_winsif_WinsDoScavenging_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winsif_WinsDoScavenging_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winsif_WinsDoScavenging *r);
struct tevent_req *dcerpc_winsif_WinsDoScavenging_send(TALLOC_CTX *mem_ctx,
						       struct tevent_context *ev,
						       struct dcerpc_binding_handle *h);
NTSTATUS dcerpc_winsif_WinsDoScavenging_recv(struct tevent_req *req,
					     TALLOC_CTX *mem_ctx,
					     WERROR *result);
NTSTATUS dcerpc_winsif_WinsDoScavenging(struct dcerpc_binding_handle *h,
					TALLOC_CTX *mem_ctx,
					WERROR *result);

struct tevent_req *dcerpc_winsif_WinsGetDbRecs_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winsif_WinsGetDbRecs *r);
NTSTATUS dcerpc_winsif_WinsGetDbRecs_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winsif_WinsGetDbRecs_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winsif_WinsGetDbRecs *r);
struct tevent_req *dcerpc_winsif_WinsGetDbRecs_send(TALLOC_CTX *mem_ctx,
						    struct tevent_context *ev,
						    struct dcerpc_binding_handle *h,
						    struct winsif_Address *_owner_address /* [in] [unique] */,
						    uint64_t _min_version_number /* [in]  */,
						    uint64_t _max_version_number /* [in]  */,
						    struct winsif_Records *_records /* [out] [ref] */);
NTSTATUS dcerpc_winsif_WinsGetDbRecs_recv(struct tevent_req *req,
					  TALLOC_CTX *mem_ctx,
					  WERROR *result);
NTSTATUS dcerpc_winsif_WinsGetDbRecs(struct dcerpc_binding_handle *h,
				     TALLOC_CTX *mem_ctx,
				     struct winsif_Address *_owner_address /* [in] [unique] */,
				     uint64_t _min_version_number /* [in]  */,
				     uint64_t _max_version_number /* [in]  */,
				     struct winsif_Records *_records /* [out] [ref] */,
				     WERROR *result);

struct tevent_req *dcerpc_winsif_WinsTerm_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winsif_WinsTerm *r);
NTSTATUS dcerpc_winsif_WinsTerm_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winsif_WinsTerm_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winsif_WinsTerm *r);
struct tevent_req *dcerpc_winsif_WinsTerm_send(TALLOC_CTX *mem_ctx,
					       struct tevent_context *ev,
					       struct dcerpc_binding_handle *h,
					       uint16_t _abrupt_termination /* [in]  */);
NTSTATUS dcerpc_winsif_WinsTerm_recv(struct tevent_req *req,
				     TALLOC_CTX *mem_ctx,
				     WERROR *result);
NTSTATUS dcerpc_winsif_WinsTerm(struct dcerpc_binding_handle *h,
				TALLOC_CTX *mem_ctx,
				uint16_t _abrupt_termination /* [in]  */,
				WERROR *result);

struct tevent_req *dcerpc_winsif_WinsBackup_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winsif_WinsBackup *r);
NTSTATUS dcerpc_winsif_WinsBackup_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winsif_WinsBackup_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winsif_WinsBackup *r);
struct tevent_req *dcerpc_winsif_WinsBackup_send(TALLOC_CTX *mem_ctx,
						 struct tevent_context *ev,
						 struct dcerpc_binding_handle *h,
						 const char *_backup_path /* [in] [charset(DOS),ref] */,
						 uint16_t _incremental /* [in]  */);
NTSTATUS dcerpc_winsif_WinsBackup_recv(struct tevent_req *req,
				       TALLOC_CTX *mem_ctx,
				       WERROR *result);
NTSTATUS dcerpc_winsif_WinsBackup(struct dcerpc_binding_handle *h,
				  TALLOC_CTX *mem_ctx,
				  const char *_backup_path /* [in] [charset(DOS),ref] */,
				  uint16_t _incremental /* [in]  */,
				  WERROR *result);

struct tevent_req *dcerpc_winsif_WinsDelDbRecs_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winsif_WinsDelDbRecs *r);
NTSTATUS dcerpc_winsif_WinsDelDbRecs_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winsif_WinsDelDbRecs_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winsif_WinsDelDbRecs *r);
struct tevent_req *dcerpc_winsif_WinsDelDbRecs_send(TALLOC_CTX *mem_ctx,
						    struct tevent_context *ev,
						    struct dcerpc_binding_handle *h,
						    struct winsif_Address *_owner_address /* [in] [ref] */,
						    uint64_t _min_version_number /* [in]  */,
						    uint64_t _max_version_number /* [in]  */);
NTSTATUS dcerpc_winsif_WinsDelDbRecs_recv(struct tevent_req *req,
					  TALLOC_CTX *mem_ctx,
					  WERROR *result);
NTSTATUS dcerpc_winsif_WinsDelDbRecs(struct dcerpc_binding_handle *h,
				     TALLOC_CTX *mem_ctx,
				     struct winsif_Address *_owner_address /* [in] [ref] */,
				     uint64_t _min_version_number /* [in]  */,
				     uint64_t _max_version_number /* [in]  */,
				     WERROR *result);

struct tevent_req *dcerpc_winsif_WinsPullRange_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winsif_WinsPullRange *r);
NTSTATUS dcerpc_winsif_WinsPullRange_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winsif_WinsPullRange_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winsif_WinsPullRange *r);
struct tevent_req *dcerpc_winsif_WinsPullRange_send(TALLOC_CTX *mem_ctx,
						    struct tevent_context *ev,
						    struct dcerpc_binding_handle *h,
						    struct winsif_Address *_server_address /* [in] [ref] */,
						    struct winsif_Address *_owner_address /* [in] [ref] */,
						    uint64_t _min_version_number /* [in]  */,
						    uint64_t _max_version_number /* [in]  */);
NTSTATUS dcerpc_winsif_WinsPullRange_recv(struct tevent_req *req,
					  TALLOC_CTX *mem_ctx,
					  WERROR *result);
NTSTATUS dcerpc_winsif_WinsPullRange(struct dcerpc_binding_handle *h,
				     TALLOC_CTX *mem_ctx,
				     struct winsif_Address *_server_address /* [in] [ref] */,
				     struct winsif_Address *_owner_address /* [in] [ref] */,
				     uint64_t _min_version_number /* [in]  */,
				     uint64_t _max_version_number /* [in]  */,
				     WERROR *result);

struct tevent_req *dcerpc_winsif_WinsSetPriorityClass_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winsif_WinsSetPriorityClass *r);
NTSTATUS dcerpc_winsif_WinsSetPriorityClass_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winsif_WinsSetPriorityClass_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winsif_WinsSetPriorityClass *r);
struct tevent_req *dcerpc_winsif_WinsSetPriorityClass_send(TALLOC_CTX *mem_ctx,
							   struct tevent_context *ev,
							   struct dcerpc_binding_handle *h,
							   enum winsif_PriorityClass _prioritiy_class /* [in]  */);
NTSTATUS dcerpc_winsif_WinsSetPriorityClass_recv(struct tevent_req *req,
						 TALLOC_CTX *mem_ctx,
						 WERROR *result);
NTSTATUS dcerpc_winsif_WinsSetPriorityClass(struct dcerpc_binding_handle *h,
					    TALLOC_CTX *mem_ctx,
					    enum winsif_PriorityClass _prioritiy_class /* [in]  */,
					    WERROR *result);

struct tevent_req *dcerpc_winsif_WinsResetCounters_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winsif_WinsResetCounters *r);
NTSTATUS dcerpc_winsif_WinsResetCounters_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winsif_WinsResetCounters_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winsif_WinsResetCounters *r);
struct tevent_req *dcerpc_winsif_WinsResetCounters_send(TALLOC_CTX *mem_ctx,
							struct tevent_context *ev,
							struct dcerpc_binding_handle *h);
NTSTATUS dcerpc_winsif_WinsResetCounters_recv(struct tevent_req *req,
					      TALLOC_CTX *mem_ctx,
					      WERROR *result);
NTSTATUS dcerpc_winsif_WinsResetCounters(struct dcerpc_binding_handle *h,
					 TALLOC_CTX *mem_ctx,
					 WERROR *result);

struct tevent_req *dcerpc_winsif_WinsWorkerThreadUpdate_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winsif_WinsWorkerThreadUpdate *r);
NTSTATUS dcerpc_winsif_WinsWorkerThreadUpdate_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winsif_WinsWorkerThreadUpdate_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winsif_WinsWorkerThreadUpdate *r);
struct tevent_req *dcerpc_winsif_WinsWorkerThreadUpdate_send(TALLOC_CTX *mem_ctx,
							     struct tevent_context *ev,
							     struct dcerpc_binding_handle *h,
							     uint32_t _num_of_threads /* [in]  */);
NTSTATUS dcerpc_winsif_WinsWorkerThreadUpdate_recv(struct tevent_req *req,
						   TALLOC_CTX *mem_ctx,
						   WERROR *result);
NTSTATUS dcerpc_winsif_WinsWorkerThreadUpdate(struct dcerpc_binding_handle *h,
					      TALLOC_CTX *mem_ctx,
					      uint32_t _num_of_threads /* [in]  */,
					      WERROR *result);

struct tevent_req *dcerpc_winsif_WinsGetNameAndAdd_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winsif_WinsGetNameAndAdd *r);
NTSTATUS dcerpc_winsif_WinsGetNameAndAdd_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winsif_WinsGetNameAndAdd_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winsif_WinsGetNameAndAdd *r);
struct tevent_req *dcerpc_winsif_WinsGetNameAndAdd_send(TALLOC_CTX *mem_ctx,
							struct tevent_context *ev,
							struct dcerpc_binding_handle *h,
							struct winsif_Address *_server_address /* [out] [ref] */,
							const char *_unc_name /* [out] [charset(DOS),ref,size_is(80)] */);
NTSTATUS dcerpc_winsif_WinsGetNameAndAdd_recv(struct tevent_req *req,
					      TALLOC_CTX *mem_ctx,
					      WERROR *result);
NTSTATUS dcerpc_winsif_WinsGetNameAndAdd(struct dcerpc_binding_handle *h,
					 TALLOC_CTX *mem_ctx,
					 struct winsif_Address *_server_address /* [out] [ref] */,
					 const char *_unc_name /* [out] [charset(DOS),ref,size_is(80)] */,
					 WERROR *result);

struct tevent_req *dcerpc_winsif_WinsGetBrowserNames_Old_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winsif_WinsGetBrowserNames_Old *r);
NTSTATUS dcerpc_winsif_WinsGetBrowserNames_Old_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winsif_WinsGetBrowserNames_Old_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winsif_WinsGetBrowserNames_Old *r);
struct tevent_req *dcerpc_winsif_WinsGetBrowserNames_Old_send(TALLOC_CTX *mem_ctx,
							      struct tevent_context *ev,
							      struct dcerpc_binding_handle *h,
							      struct winsif_BrowserNames *_names /* [out] [ref] */);
NTSTATUS dcerpc_winsif_WinsGetBrowserNames_Old_recv(struct tevent_req *req,
						    TALLOC_CTX *mem_ctx,
						    WERROR *result);
NTSTATUS dcerpc_winsif_WinsGetBrowserNames_Old(struct dcerpc_binding_handle *h,
					       TALLOC_CTX *mem_ctx,
					       struct winsif_BrowserNames *_names /* [out] [ref] */,
					       WERROR *result);

struct tevent_req *dcerpc_winsif_WinsDeleteWins_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winsif_WinsDeleteWins *r);
NTSTATUS dcerpc_winsif_WinsDeleteWins_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winsif_WinsDeleteWins_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winsif_WinsDeleteWins *r);
struct tevent_req *dcerpc_winsif_WinsDeleteWins_send(TALLOC_CTX *mem_ctx,
						     struct tevent_context *ev,
						     struct dcerpc_binding_handle *h,
						     struct winsif_Address *_owner_address /* [in] [ref] */);
NTSTATUS dcerpc_winsif_WinsDeleteWins_recv(struct tevent_req *req,
					   TALLOC_CTX *mem_ctx,
					   WERROR *result);
NTSTATUS dcerpc_winsif_WinsDeleteWins(struct dcerpc_binding_handle *h,
				      TALLOC_CTX *mem_ctx,
				      struct winsif_Address *_owner_address /* [in] [ref] */,
				      WERROR *result);

struct tevent_req *dcerpc_winsif_WinsSetFlags_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winsif_WinsSetFlags *r);
NTSTATUS dcerpc_winsif_WinsSetFlags_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winsif_WinsSetFlags_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winsif_WinsSetFlags *r);
struct tevent_req *dcerpc_winsif_WinsSetFlags_send(TALLOC_CTX *mem_ctx,
						   struct tevent_context *ev,
						   struct dcerpc_binding_handle *h,
						   uint32_t _flags /* [in]  */);
NTSTATUS dcerpc_winsif_WinsSetFlags_recv(struct tevent_req *req,
					 TALLOC_CTX *mem_ctx,
					 WERROR *result);
NTSTATUS dcerpc_winsif_WinsSetFlags(struct dcerpc_binding_handle *h,
				    TALLOC_CTX *mem_ctx,
				    uint32_t _flags /* [in]  */,
				    WERROR *result);

struct tevent_req *dcerpc_winsif_WinsGetBrowserNames_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winsif_WinsGetBrowserNames *r);
NTSTATUS dcerpc_winsif_WinsGetBrowserNames_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winsif_WinsGetBrowserNames_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winsif_WinsGetBrowserNames *r);
struct tevent_req *dcerpc_winsif_WinsGetBrowserNames_send(TALLOC_CTX *mem_ctx,
							  struct tevent_context *ev,
							  struct dcerpc_binding_handle *h,
							  struct winsif_BindData *_server_handle /* [in] [ref] */,
							  struct winsif_BrowserNames *_names /* [out] [ref] */);
NTSTATUS dcerpc_winsif_WinsGetBrowserNames_recv(struct tevent_req *req,
						TALLOC_CTX *mem_ctx,
						WERROR *result);
NTSTATUS dcerpc_winsif_WinsGetBrowserNames(struct dcerpc_binding_handle *h,
					   TALLOC_CTX *mem_ctx,
					   struct winsif_BindData *_server_handle /* [in] [ref] */,
					   struct winsif_BrowserNames *_names /* [out] [ref] */,
					   WERROR *result);

struct tevent_req *dcerpc_winsif_WinsGetDbRecsByName_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winsif_WinsGetDbRecsByName *r);
NTSTATUS dcerpc_winsif_WinsGetDbRecsByName_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winsif_WinsGetDbRecsByName_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winsif_WinsGetDbRecsByName *r);
struct tevent_req *dcerpc_winsif_WinsGetDbRecsByName_send(TALLOC_CTX *mem_ctx,
							  struct tevent_context *ev,
							  struct dcerpc_binding_handle *h,
							  struct winsif_Address *_owner_address /* [in] [unique] */,
							  uint32_t _search_backward /* [in]  */,
							  struct nbt_name * _name /* [in] [unique] */,
							  uint32_t _name_len /* [in] [range(0,16),value(name?16:0)] */,
							  uint32_t _num_records_desired /* [in]  */,
							  uint32_t _only_statics /* [in]  */,
							  struct winsif_Records *_records /* [out] [ref] */);
NTSTATUS dcerpc_winsif_WinsGetDbRecsByName_recv(struct tevent_req *req,
						TALLOC_CTX *mem_ctx,
						WERROR *result);
NTSTATUS dcerpc_winsif_WinsGetDbRecsByName(struct dcerpc_binding_handle *h,
					   TALLOC_CTX *mem_ctx,
					   struct winsif_Address *_owner_address /* [in] [unique] */,
					   uint32_t _search_backward /* [in]  */,
					   struct nbt_name * _name /* [in] [unique] */,
					   uint32_t _name_len /* [in] [range(0,16),value(name?16:0)] */,
					   uint32_t _num_records_desired /* [in]  */,
					   uint32_t _only_statics /* [in]  */,
					   struct winsif_Records *_records /* [out] [ref] */,
					   WERROR *result);

struct tevent_req *dcerpc_winsif_WinsStatusNew_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winsif_WinsStatusNew *r);
NTSTATUS dcerpc_winsif_WinsStatusNew_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winsif_WinsStatusNew_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winsif_WinsStatusNew *r);
struct tevent_req *dcerpc_winsif_WinsStatusNew_send(TALLOC_CTX *mem_ctx,
						    struct tevent_context *ev,
						    struct dcerpc_binding_handle *h,
						    enum winsif_StatusCmd _cmd /* [in]  */,
						    struct winsif_ResultsNew *_results /* [out] [ref] */);
NTSTATUS dcerpc_winsif_WinsStatusNew_recv(struct tevent_req *req,
					  TALLOC_CTX *mem_ctx,
					  WERROR *result);
NTSTATUS dcerpc_winsif_WinsStatusNew(struct dcerpc_binding_handle *h,
				     TALLOC_CTX *mem_ctx,
				     enum winsif_StatusCmd _cmd /* [in]  */,
				     struct winsif_ResultsNew *_results /* [out] [ref] */,
				     WERROR *result);

struct tevent_req *dcerpc_winsif_WinsStatusWHdl_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winsif_WinsStatusWHdl *r);
NTSTATUS dcerpc_winsif_WinsStatusWHdl_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winsif_WinsStatusWHdl_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winsif_WinsStatusWHdl *r);
struct tevent_req *dcerpc_winsif_WinsStatusWHdl_send(TALLOC_CTX *mem_ctx,
						     struct tevent_context *ev,
						     struct dcerpc_binding_handle *h,
						     struct winsif_BindData *_server_handle /* [in] [ref] */,
						     enum winsif_StatusCmd _cmd /* [in]  */,
						     struct winsif_ResultsNew *_results /* [in,out] [ref] */);
NTSTATUS dcerpc_winsif_WinsStatusWHdl_recv(struct tevent_req *req,
					   TALLOC_CTX *mem_ctx,
					   WERROR *result);
NTSTATUS dcerpc_winsif_WinsStatusWHdl(struct dcerpc_binding_handle *h,
				      TALLOC_CTX *mem_ctx,
				      struct winsif_BindData *_server_handle /* [in] [ref] */,
				      enum winsif_StatusCmd _cmd /* [in]  */,
				      struct winsif_ResultsNew *_results /* [in,out] [ref] */,
				      WERROR *result);

struct tevent_req *dcerpc_winsif_WinsDoScanvengingNew_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct winsif_WinsDoScanvengingNew *r);
NTSTATUS dcerpc_winsif_WinsDoScanvengingNew_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_winsif_WinsDoScanvengingNew_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct winsif_WinsDoScanvengingNew *r);
struct tevent_req *dcerpc_winsif_WinsDoScanvengingNew_send(TALLOC_CTX *mem_ctx,
							   struct tevent_context *ev,
							   struct dcerpc_binding_handle *h,
							   struct winsif_ScavengingRequest *_request /* [in] [ref] */);
NTSTATUS dcerpc_winsif_WinsDoScanvengingNew_recv(struct tevent_req *req,
						 TALLOC_CTX *mem_ctx,
						 WERROR *result);
NTSTATUS dcerpc_winsif_WinsDoScanvengingNew(struct dcerpc_binding_handle *h,
					    TALLOC_CTX *mem_ctx,
					    struct winsif_ScavengingRequest *_request /* [in] [ref] */,
					    WERROR *result);

#endif /* _HEADER_RPC_winsif */
