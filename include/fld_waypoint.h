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

typedef union fldWaypointHeaderValues_t {
    s16 data[12];
    struct {
        s16 floors[6];
        s16 blocks[6];
    } elevator;
} fldWaypointHeaderValues_t;

typedef struct fldWaypointHeader_t {
    s32 area;       /* +0x00: also read as a word by actor-trigger setup. */
    union {
        /* Serialized halfword payload, including its sound/count prefix. */
        s16 payload[14];
        struct {
            s16 sound;      /* +0x04: elevator sound selector. */
            s16 floorCount; /* +0x06: number of usable floor/block pairs. */
            fldWaypointHeaderValues_t body; /* +0x08: six floors, then six block IDs. */
        };
    };
} fldWaypointHeader_t;

typedef struct fldWaypointBlock_t {
    fldWaypointHeader_t headers[FLD_WAYPOINT_HEADER_COUNT];
    doormovetbl_s actors[256];
} fldWaypointBlock_t;

typedef char FldWaypointHeaderPayloadSizeCheck[
    (sizeof(((fldWaypointHeader_t *)0)->payload) == 0x1C) ? 1 : -1];
typedef char FldWaypointHeader_payload_offset_check[
    ((u32)&((fldWaypointHeader_t *)0)->payload == 0x04) ? 1 : -1];
typedef char FldWaypointHeaderSizeCheck[(sizeof(fldWaypointHeader_t) == 0x20) ? 1 : -1];
typedef char FldWaypointHeader_area_offset_check[
    ((u32)&((fldWaypointHeader_t *)0)->area == 0x00) ? 1 : -1];
typedef char FldWaypointHeader_sound_offset_check[
    ((u32)&((fldWaypointHeader_t *)0)->sound == 0x04) ? 1 : -1];
typedef char FldWaypointHeader_floorCount_offset_check[
    ((u32)&((fldWaypointHeader_t *)0)->floorCount == 0x06) ? 1 : -1];
typedef char FldWaypointHeader_floors_offset_check[
    ((u32)&((fldWaypointHeader_t *)0)->body.elevator.floors == 0x08) ? 1 : -1];
typedef char FldWaypointHeader_blocks_offset_check[
    ((u32)&((fldWaypointHeader_t *)0)->body.elevator.blocks == 0x14) ? 1 : -1];
typedef char FldWaypointActorSizeCheck[(sizeof(doormovetbl_s) == 0x6C) ? 1 : -1];
typedef char FldWaypointActorOffsetCheck[((u32)&((fldWaypointBlock_t *)0)->actors == FLD_WAYPOINT_HEADER_COUNT * 0x20) ? 1 : -1];
typedef char FldWaypointBlockSizeCheck[(sizeof(fldWaypointBlock_t) == FLD_WAYPOINT_BLOCK_BYTES) ? 1 : -1];

#endif
