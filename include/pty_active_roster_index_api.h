#ifndef PTY_ACTIVE_ROSTER_INDEX_API_H
#define PTY_ACTIVE_ROSTER_INDEX_API_H

#include "common.h"

typedef s32 PtyActiveRosterIndex;
enum {
    PTY_ACTIVE_ROSTER_INDEX_NOT_FOUND = -1,
};

#define PTY_ACTIVE_ROSTER_INDEX_IS_FOUND(index) ((index) >= 0)
#define PTY_ACTIVE_ROSTER_INDEX_IS_NOT_FOUND(index) ((index) < 0)

/* Find an occupied party-array entry by unit ID, or return NOT_FOUND. */
PtyActiveRosterIndex dds3FindEntryIndex(s32 unitId);

#endif
