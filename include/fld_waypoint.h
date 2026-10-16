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

typedef union FldWaypointHeaderValues {
    s16 data[12];
    struct {
        s16 floors[6];
        s16 blocks[6];
    } elevator;
} FldWaypointHeaderValues;

typedef struct FldWaypointHeader {
    s32 area;       /* +0x00: also read as a word by actor-trigger setup. */
    s16 sound;      /* +0x04: elevator sound selector. */
    s16 floorCount; /* +0x06: number of usable floor/block pairs. */
    FldWaypointHeaderValues body; /* +0x08: six floors, then six block IDs. */
} FldWaypointHeader;

typedef struct FldWaypointBlock {
    FldWaypointHeader headers[FLD_WAYPOINT_HEADER_COUNT];
    FldActorEntry actors[256];
} FldWaypointBlock;

typedef char FldWaypointHeaderSizeCheck[(sizeof(FldWaypointHeader) == 0x20) ? 1 : -1];
typedef char FldWaypointHeader_area_offset_check[
    ((u32)&((FldWaypointHeader *)0)->area == 0x00) ? 1 : -1];
typedef char FldWaypointHeader_sound_offset_check[
    ((u32)&((FldWaypointHeader *)0)->sound == 0x04) ? 1 : -1];
typedef char FldWaypointHeader_floorCount_offset_check[
    ((u32)&((FldWaypointHeader *)0)->floorCount == 0x06) ? 1 : -1];
typedef char FldWaypointHeader_floors_offset_check[
    ((u32)&((FldWaypointHeader *)0)->body.elevator.floors == 0x08) ? 1 : -1];
typedef char FldWaypointHeader_blocks_offset_check[
    ((u32)&((FldWaypointHeader *)0)->body.elevator.blocks == 0x14) ? 1 : -1];
typedef char FldWaypointActorSizeCheck[(sizeof(FldActorEntry) == 0x6C) ? 1 : -1];
typedef char FldWaypointActorOffsetCheck[((u32)&((FldWaypointBlock *)0)->actors == FLD_WAYPOINT_HEADER_COUNT * 0x20) ? 1 : -1];
typedef char FldWaypointBlockSizeCheck[(sizeof(FldWaypointBlock) == FLD_WAYPOINT_BLOCK_BYTES) ? 1 : -1];

#endif
