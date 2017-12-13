#include "bin/default/librpc/gen_ndr/ndr_winbind.h"
#ifndef __SRV_WINBIND__
#define __SRV_WINBIND__
void _wbint_Ping(struct pipes_struct *p, struct wbint_Ping *r);
NTSTATUS _wbint_LookupSid(struct pipes_struct *p, struct wbint_LookupSid *r);
NTSTATUS _wbint_LookupSids(struct pipes_struct *p, struct wbint_LookupSids *r);
NTSTATUS _wbint_LookupName(struct pipes_struct *p, struct wbint_LookupName *r);
NTSTATUS _wbint_Sids2UnixIDs(struct pipes_struct *p, struct wbint_Sids2UnixIDs *r);
NTSTATUS _wbint_UnixIDs2Sids(struct pipes_struct *p, struct wbint_UnixIDs2Sids *r);
NTSTATUS _wbint_AllocateUid(struct pipes_struct *p, struct wbint_AllocateUid *r);
NTSTATUS _wbint_AllocateGid(struct pipes_struct *p, struct wbint_AllocateGid *r);
NTSTATUS _wbint_GetNssInfo(struct pipes_struct *p, struct wbint_GetNssInfo *r);
NTSTATUS _wbint_LookupUserAliases(struct pipes_struct *p, struct wbint_LookupUserAliases *r);
NTSTATUS _wbint_LookupUserGroups(struct pipes_struct *p, struct wbint_LookupUserGroups *r);
NTSTATUS _wbint_QuerySequenceNumber(struct pipes_struct *p, struct wbint_QuerySequenceNumber *r);
NTSTATUS _wbint_LookupGroupMembers(struct pipes_struct *p, struct wbint_LookupGroupMembers *r);
NTSTATUS _wbint_QueryGroupList(struct pipes_struct *p, struct wbint_QueryGroupList *r);
NTSTATUS _wbint_QueryUserRidList(struct pipes_struct *p, struct wbint_QueryUserRidList *r);
NTSTATUS _wbint_DsGetDcName(struct pipes_struct *p, struct wbint_DsGetDcName *r);
NTSTATUS _wbint_LookupRids(struct pipes_struct *p, struct wbint_LookupRids *r);
NTSTATUS _wbint_CheckMachineAccount(struct pipes_struct *p, struct wbint_CheckMachineAccount *r);
NTSTATUS _wbint_ChangeMachineAccount(struct pipes_struct *p, struct wbint_ChangeMachineAccount *r);
NTSTATUS _wbint_PingDc(struct pipes_struct *p, struct wbint_PingDc *r);
NTSTATUS _winbind_SamLogon(struct pipes_struct *p, struct winbind_SamLogon *r);
NTSTATUS _winbind_DsrUpdateReadOnlyServerDnsRecords(struct pipes_struct *p, struct winbind_DsrUpdateReadOnlyServerDnsRecords *r);
WERROR _winbind_LogonControl(struct pipes_struct *p, struct winbind_LogonControl *r);
WERROR _winbind_GetForestTrustInformation(struct pipes_struct *p, struct winbind_GetForestTrustInformation *r);
const struct api_struct *winbind_get_pipe_fns(int *n_fns);
#endif /* __SRV_WINBIND__ */
