#include "librpc/rpc/dcerpc.h"
#include "bin/default/source4/librpc/gen_ndr/irpc.h"
#ifndef _HEADER_RPC_irpc
#define _HEADER_RPC_irpc

extern const struct ndr_interface_table ndr_table_irpc;

struct tevent_req *dcerpc_irpc_uptime_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct irpc_uptime *r);
NTSTATUS dcerpc_irpc_uptime_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_irpc_uptime_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct irpc_uptime *r);
struct tevent_req *dcerpc_irpc_uptime_send(TALLOC_CTX *mem_ctx,
					   struct tevent_context *ev,
					   struct dcerpc_binding_handle *h,
					   NTTIME *_start_time /* [out] [ref] */);
NTSTATUS dcerpc_irpc_uptime_recv(struct tevent_req *req,
				 TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_irpc_uptime(struct dcerpc_binding_handle *h,
			    TALLOC_CTX *mem_ctx,
			    NTTIME *_start_time /* [out] [ref] */);

struct tevent_req *dcerpc_nbtd_information_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct nbtd_information *r);
NTSTATUS dcerpc_nbtd_information_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_nbtd_information_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct nbtd_information *r);
/*
 * The following functions are skipped because
 * an [out] argument info is not a pointer or array:
 *
 * dcerpc_nbtd_information_send()
 * dcerpc_nbtd_information_recv()
 * dcerpc_nbtd_information()
 */

struct tevent_req *dcerpc_nbtd_getdcname_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct nbtd_getdcname *r);
NTSTATUS dcerpc_nbtd_getdcname_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_nbtd_getdcname_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct nbtd_getdcname *r);
/*
 * The following functions are skipped because
 * an [out] argument dcname is a pointer to type 'string':
 *
 * dcerpc_nbtd_getdcname_send()
 * dcerpc_nbtd_getdcname_recv()
 * dcerpc_nbtd_getdcname()
 */

struct tevent_req *dcerpc_nbtd_proxy_wins_challenge_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct nbtd_proxy_wins_challenge *r);
NTSTATUS dcerpc_nbtd_proxy_wins_challenge_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_nbtd_proxy_wins_challenge_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct nbtd_proxy_wins_challenge *r);
/*
 * The following functions are skipped because
 * an [out] argument num_addrs is not a pointer or array:
 *
 * dcerpc_nbtd_proxy_wins_challenge_send()
 * dcerpc_nbtd_proxy_wins_challenge_recv()
 * dcerpc_nbtd_proxy_wins_challenge()
 */

struct tevent_req *dcerpc_nbtd_proxy_wins_release_demand_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct nbtd_proxy_wins_release_demand *r);
NTSTATUS dcerpc_nbtd_proxy_wins_release_demand_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_nbtd_proxy_wins_release_demand_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct nbtd_proxy_wins_release_demand *r);
struct tevent_req *dcerpc_nbtd_proxy_wins_release_demand_send(TALLOC_CTX *mem_ctx,
							      struct tevent_context *ev,
							      struct dcerpc_binding_handle *h,
							      struct nbt_name _name /* [in]  */,
							      uint32_t _num_addrs /* [in]  */,
							      struct nbtd_proxy_wins_addr *_addrs /* [in]  */);
NTSTATUS dcerpc_nbtd_proxy_wins_release_demand_recv(struct tevent_req *req,
						    TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_nbtd_proxy_wins_release_demand(struct dcerpc_binding_handle *h,
					       TALLOC_CTX *mem_ctx,
					       struct nbt_name _name /* [in]  */,
					       uint32_t _num_addrs /* [in]  */,
					       struct nbtd_proxy_wins_addr *_addrs /* [in]  */);

struct tevent_req *dcerpc_kdc_check_generic_kerberos_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct kdc_check_generic_kerberos *r);
NTSTATUS dcerpc_kdc_check_generic_kerberos_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_kdc_check_generic_kerberos_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct kdc_check_generic_kerberos *r);
/*
 * The following functions are skipped because
 * an [out] argument generic_reply is not a pointer or array:
 *
 * dcerpc_kdc_check_generic_kerberos_send()
 * dcerpc_kdc_check_generic_kerberos_recv()
 * dcerpc_kdc_check_generic_kerberos()
 */

struct tevent_req *dcerpc_smbsrv_information_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct smbsrv_information *r);
NTSTATUS dcerpc_smbsrv_information_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_smbsrv_information_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct smbsrv_information *r);
/*
 * The following functions are skipped because
 * an [out] argument info is not a pointer or array:
 *
 * dcerpc_smbsrv_information_send()
 * dcerpc_smbsrv_information_recv()
 * dcerpc_smbsrv_information()
 */

struct tevent_req *dcerpc_samba_terminate_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct samba_terminate *r);
NTSTATUS dcerpc_samba_terminate_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_samba_terminate_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct samba_terminate *r);
struct tevent_req *dcerpc_samba_terminate_send(TALLOC_CTX *mem_ctx,
					       struct tevent_context *ev,
					       struct dcerpc_binding_handle *h,
					       const char * _reason /* [in] [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM)] */);
NTSTATUS dcerpc_samba_terminate_recv(struct tevent_req *req,
				     TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_samba_terminate(struct dcerpc_binding_handle *h,
				TALLOC_CTX *mem_ctx,
				const char * _reason /* [in] [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM)] */);

struct tevent_req *dcerpc_dreplsrv_refresh_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct dreplsrv_refresh *r);
NTSTATUS dcerpc_dreplsrv_refresh_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_dreplsrv_refresh_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct dreplsrv_refresh *r);
struct tevent_req *dcerpc_dreplsrv_refresh_send(TALLOC_CTX *mem_ctx,
						struct tevent_context *ev,
						struct dcerpc_binding_handle *h);
NTSTATUS dcerpc_dreplsrv_refresh_recv(struct tevent_req *req,
				      TALLOC_CTX *mem_ctx,
				      WERROR *result);
NTSTATUS dcerpc_dreplsrv_refresh(struct dcerpc_binding_handle *h,
				 TALLOC_CTX *mem_ctx,
				 WERROR *result);

struct tevent_req *dcerpc_drepl_takeFSMORole_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct drepl_takeFSMORole *r);
NTSTATUS dcerpc_drepl_takeFSMORole_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_drepl_takeFSMORole_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct drepl_takeFSMORole *r);
struct tevent_req *dcerpc_drepl_takeFSMORole_send(TALLOC_CTX *mem_ctx,
						  struct tevent_context *ev,
						  struct dcerpc_binding_handle *h,
						  enum drepl_role_master _role /* [in]  */);
NTSTATUS dcerpc_drepl_takeFSMORole_recv(struct tevent_req *req,
					TALLOC_CTX *mem_ctx,
					WERROR *result);
NTSTATUS dcerpc_drepl_takeFSMORole(struct dcerpc_binding_handle *h,
				   TALLOC_CTX *mem_ctx,
				   enum drepl_role_master _role /* [in]  */,
				   WERROR *result);

struct tevent_req *dcerpc_drepl_trigger_repl_secret_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct drepl_trigger_repl_secret *r);
NTSTATUS dcerpc_drepl_trigger_repl_secret_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_drepl_trigger_repl_secret_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct drepl_trigger_repl_secret *r);
struct tevent_req *dcerpc_drepl_trigger_repl_secret_send(TALLOC_CTX *mem_ctx,
							 struct tevent_context *ev,
							 struct dcerpc_binding_handle *h,
							 const char * _user_dn /* [in] [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM)] */);
NTSTATUS dcerpc_drepl_trigger_repl_secret_recv(struct tevent_req *req,
					       TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_drepl_trigger_repl_secret(struct dcerpc_binding_handle *h,
					  TALLOC_CTX *mem_ctx,
					  const char * _user_dn /* [in] [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM)] */);

struct tevent_req *dcerpc_dnsupdate_RODC_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct dnsupdate_RODC *r);
NTSTATUS dcerpc_dnsupdate_RODC_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_dnsupdate_RODC_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct dnsupdate_RODC *r);
struct tevent_req *dcerpc_dnsupdate_RODC_send(TALLOC_CTX *mem_ctx,
					      struct tevent_context *ev,
					      struct dcerpc_binding_handle *h,
					      struct dom_sid *_dom_sid /* [in] [unique] */,
					      const char *_site_name /* [in] [charset(UTF16),unique] */,
					      uint32_t _dns_ttl /* [in]  */,
					      struct NL_DNS_NAME_INFO_ARRAY *_dns_names /* [in,out] [ref] */);
NTSTATUS dcerpc_dnsupdate_RODC_recv(struct tevent_req *req,
				    TALLOC_CTX *mem_ctx,
				    NTSTATUS *result);
NTSTATUS dcerpc_dnsupdate_RODC(struct dcerpc_binding_handle *h,
			       TALLOC_CTX *mem_ctx,
			       struct dom_sid *_dom_sid /* [in] [unique] */,
			       const char *_site_name /* [in] [charset(UTF16),unique] */,
			       uint32_t _dns_ttl /* [in]  */,
			       struct NL_DNS_NAME_INFO_ARRAY *_dns_names /* [in,out] [ref] */,
			       NTSTATUS *result);

struct tevent_req *dcerpc_dnssrv_reload_dns_zones_r_send(TALLOC_CTX *mem_ctx,
	struct tevent_context *ev,
	struct dcerpc_binding_handle *h,
	struct dnssrv_reload_dns_zones *r);
NTSTATUS dcerpc_dnssrv_reload_dns_zones_r_recv(struct tevent_req *req, TALLOC_CTX *mem_ctx);
NTSTATUS dcerpc_dnssrv_reload_dns_zones_r(struct dcerpc_binding_handle *h, TALLOC_CTX *mem_ctx, struct dnssrv_reload_dns_zones *r);
struct tevent_req *dcerpc_dnssrv_reload_dns_zones_send(TALLOC_CTX *mem_ctx,
						       struct tevent_context *ev,
						       struct dcerpc_binding_handle *h);
NTSTATUS dcerpc_dnssrv_reload_dns_zones_recv(struct tevent_req *req,
					     TALLOC_CTX *mem_ctx,
					     NTSTATUS *result);
NTSTATUS dcerpc_dnssrv_reload_dns_zones(struct dcerpc_binding_handle *h,
					TALLOC_CTX *mem_ctx,
					NTSTATUS *result);

#endif /* _HEADER_RPC_irpc */
