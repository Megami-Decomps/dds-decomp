#ifndef PTY_PARTY_UNIT_PRESENCE_API_H
#define PTY_PARTY_UNIT_PRESENCE_API_H

#include "common.h"
#include "dat_unit_id.h"

typedef u8 PtyPartyUnitPresenceResult;
enum {
    PTY_PARTY_UNIT_PRESENCE_NOT_FOUND = 0,
    PTY_PARTY_UNIT_PRESENCE_FOUND = 1,
};

/* Search occupied party records by unit ID and report whether one exists. */
PtyPartyUnitPresenceResult ptyIsPartyUnitPresent(DatUnitId unitId);

#endif /* PTY_PARTY_UNIT_PRESENCE_API_H */
