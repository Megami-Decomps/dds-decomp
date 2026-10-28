#ifndef FLD_AUTOMAP_ENTRY_DISCOVERY_STATE_H
#define FLD_AUTOMAP_ENTRY_DISCOVERY_STATE_H

#include "common.h"

/* The saved area-flag bit determines whether an automap entry is revealed. */
typedef s32 FldAutoMapEntryDiscoveryState;
enum {
    FLD_AUTO_MAP_ENTRY_NOT_DISCOVERED = 0,
    FLD_AUTO_MAP_ENTRY_DISCOVERED = 1
};

/* An unmapped area row has the same state as an unset discovery bit. */
FldAutoMapEntryDiscoveryState fldGetFloorFlag(s32 area, s32 floor, s32 bit);

#endif /* FLD_AUTOMAP_ENTRY_DISCOVERY_STATE_H */
