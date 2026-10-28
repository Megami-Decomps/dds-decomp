#ifndef MNU_ENTRY_USE_STATUS_API_H
#define MNU_ENTRY_USE_STATUS_API_H

#include "common.h"

struct datUnitWork_t;

typedef s32 MnuEntryUseStatus;
enum {
    MNU_ENTRY_USE_COST_UNAFFORDABLE = -1,
    MNU_ENTRY_USE_MARKER_SUPPRESSED = 0,
    MNU_ENTRY_USE_MARK_UNAVAILABLE = 1
};

/* The menu marks entries unavailable for either nonzero result. */
MnuEntryUseStatus mnuGetEntryUseStatus(
    struct datUnitWork_t *actorEntry, u16 commandId);

#endif /* MNU_ENTRY_USE_STATUS_API_H */
