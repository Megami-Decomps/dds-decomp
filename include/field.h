#ifndef FIELD_H
#define FIELD_H

#include "common.h"

struct dds3Basic_t;

/* Collision faces are 0x24-byte wire records in the field resource.
 * The room-record builders retain the face and stop-table addresses. */
typedef struct fldCollisionFace_t {
    u32 attributes;
    u8 moveFloor;
    u8 sound;
    u16 stop;
    u16 place;
    u8 automap[2];
    u32 vertexIndices[4];
    s16 encounterType;
    s16 encounter;
    s16 special[2];
} fldCollisionFace_t;

/* Both room-record builders construct this complete 0xE4-byte owner.
 * Zone classifiers use the bounding-plane prefix of the same record. */
typedef struct collision_s {
    s16 mode2D;
    s16 numedges;
    f32 normal[4];
    f32 num;
    f32 e_normal[4][4]; /* 0x18 */
    f32 e_num[4]; /* 0x58 */
    f32 min[4]; /* 0x68: min0, min1, max0, max1 */
    f32 point[4][4]; /* 0x78 */
    f32 previousPosition[3]; /* 0xB8: saved actor world position */
    u32 elv;
    u16 blockID;
    u16 hitflag;
    s32 resID;
    struct dds3Basic_t *matter; /* Cached object for this record ID. */
    fldCollisionFace_t *pFace;
    void *pTome;
    s32 hojiID;
    s32 npcID; /* 0xE0: map-slot npc-flag selector; <= 0 bypasses its gate */
} collision_s;

typedef char FldCollisionFaceSizeCheck[(sizeof(fldCollisionFace_t) == 0x24) ? 1 : -1];
typedef char collision_sSizeCheck[(sizeof(collision_s) == 0xE4) ? 1 : -1];
typedef char collision_s_matter_at_D0[
    ((u32)&((collision_s *)0)->matter == 0xD0) ? 1 : -1];

#include "fld.h"
#include "fld_area_work.h"
#include "fld_encount_flags.h"
#include "fld_room_effect_entry.h"

#endif /* FIELD_H */
