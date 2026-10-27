#ifndef PTY_STATUS_QUERY_API_H
#define PTY_STATUS_QUERY_API_H

#include "common.h"

typedef u8 PtyPartyStatusMatchResult;
enum {
    PTY_PARTY_STATUS_MATCH_NOT_FOUND = 0,
    PTY_PARTY_STATUS_MATCH_FOUND = 1,
};

typedef s32 PtyPartyStatusScope;
enum {
    PTY_PARTY_STATUS_SCOPE_ALL_LIVING = 0,
    PTY_PARTY_STATUS_SCOPE_FRONTLINE_ONLY = 1,
};

/* Search occupied, living party entries for any status bit in statusMask. */
PtyPartyStatusMatchResult ptyAnyLivingUnitHasStatus(
    u32 statusMask, PtyPartyStatusScope scope);

#endif /* PTY_STATUS_QUERY_API_H */
