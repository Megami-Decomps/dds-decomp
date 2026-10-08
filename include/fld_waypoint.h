#ifndef FLD_WAYPOINT_H
#define FLD_WAYPOINT_H

#include "fld.h"

/* The loader/copy providers transfer the complete WAP payload: 0x6CA0 in
 * DDS1 and 0x6D00 in DDS2. Their actor tables begin at +0xA0/+0x100,
 * after five/eight 0x20-byte headers, and contain 256 0x6C-byte rows. */
#ifdef VERSION_DDS1
enum { FLD_WAYPOINT_HEADER_COUNT = 5, FLD_WAYPOINT_BLOCK_BYTES = 0x6CA0 };
#else
enum { FLD_WAYPOINT_HEADER_COUNT = 8, FLD_WAYPOINT_BLOCK_BYTES = 0x6D00 };
#endif

typedef struct FldWaypointHeader {
    s32 sequenceArg; /* Actor-trigger sequence construction reads this with LW. */
    s16 unk4;
    s16 count;
    struct {
        s16 data[12];
    } body;
} FldWaypointHeader;

typedef struct FldWaypointBlock {
    FldWaypointHeader headers[FLD_WAYPOINT_HEADER_COUNT];
    FldActorEntry actors[256];
} FldWaypointBlock;

typedef char FldWaypointHeaderSizeCheck[(sizeof(FldWaypointHeader) == 0x20) ? 1 : -1];
typedef char FldWaypointActorSizeCheck[(sizeof(FldActorEntry) == 0x6C) ? 1 : -1];
typedef char FldWaypointActorOffsetCheck[((u32)&((FldWaypointBlock *)0)->actors == FLD_WAYPOINT_HEADER_COUNT * 0x20) ? 1 : -1];
typedef char FldWaypointBlockSizeCheck[(sizeof(FldWaypointBlock) == FLD_WAYPOINT_BLOCK_BYTES) ? 1 : -1];

#endif
