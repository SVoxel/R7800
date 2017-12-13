#include "bin/default/librpc/gen_ndr/ndr_epmapper.h"
#ifndef __SRV_EPMAPPER__
#define __SRV_EPMAPPER__
uint32_t _epm_Insert(struct pipes_struct *p, struct epm_Insert *r);
uint32_t _epm_Delete(struct pipes_struct *p, struct epm_Delete *r);
uint32_t _epm_Lookup(struct pipes_struct *p, struct epm_Lookup *r);
uint32_t _epm_Map(struct pipes_struct *p, struct epm_Map *r);
uint32_t _epm_LookupHandleFree(struct pipes_struct *p, struct epm_LookupHandleFree *r);
uint32_t _epm_InqObject(struct pipes_struct *p, struct epm_InqObject *r);
uint32_t _epm_MgmtDelete(struct pipes_struct *p, struct epm_MgmtDelete *r);
uint32_t _epm_MapAuth(struct pipes_struct *p, struct epm_MapAuth *r);
const struct api_struct *epmapper_get_pipe_fns(int *n_fns);
struct rpc_srv_callbacks;
NTSTATUS rpc_epmapper_init(const struct rpc_srv_callbacks *rpc_srv_cb);
NTSTATUS rpc_epmapper_shutdown(void);
#endif /* __SRV_EPMAPPER__ */
