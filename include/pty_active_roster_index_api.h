#ifndef PTY_ACTIVE_ROSTER_INDEX_API_H
#define PTY_ACTIVE_ROSTER_INDEX_API_H

#include "common.h"
#include "dat_unit_id.h"

typedef s32 PtyActiveRosterIndex;
enum {
    PTY_ACTIVE_ROSTER_INDEX_NOT_FOUND = -1,
};

#define PTY_ACTIVE_ROSTER_INDEX_IS_FOUND(index) ((index) >= 0)
#define PTY_ACTIVE_ROSTER_INDEX_IS_NOT_FOUND(index) ((index) < 0)

/* Find an occupied party-array entry by unit ID, or return NOT_FOUND. */
PtyActiveRosterIndex dds3FindEntryIndex(DatUnitId unitId);

struct BtlUnit;

/* Resolve an active battle actor by its current party-array slot. */
struct BtlUnit *btlFindActiveActorByPartyIndex(PtyActiveRosterIndex partyIndex);

#endif
