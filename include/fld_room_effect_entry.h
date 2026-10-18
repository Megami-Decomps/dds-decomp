#ifndef FLD_ROOM_EFFECT_ENTRY_H
#define FLD_ROOM_EFFECT_ENTRY_H

#include "common.h"

/* A room's placed NPC model, in placement order. DDS1 and DDS2 use the same
 * 0x50-byte layout. */
typedef struct FldRoomEffectEntry {
    s32 unk0;
    f32 x;              /* 0x04 */
    f32 y;              /* 0x08 */
    f32 z;              /* 0x0C */
    f32 orientation[4]; /* 0x10 */
    s32 *object;        /* 0x20: room effect object/record. */
    s16 kind;           /* 0x24 */
    s16 kindId;         /* 0x26 */
    f32 size;           /* 0x28 */
    s32 unk2C, unk30, unk34;
    s32 room;           /* 0x38 */
    s16 unk3C, unk3E;
    char name[0x10];    /* 0x40 */
} FldRoomEffectEntry;

typedef char FldRoomEffectEntry_size_check[(sizeof(FldRoomEffectEntry) == 0x50) ? 1 : -1];
typedef char FldRoomEffectEntry_object_offset_check[
    ((u32)&((FldRoomEffectEntry *)0)->object == 0x20) ? 1 : -1];
typedef char FldRoomEffectEntry_room_offset_check[
    ((u32)&((FldRoomEffectEntry *)0)->room == 0x38) ? 1 : -1];

extern FldRoomEffectEntry fldRoomEffectEntries[];
extern s32 fldRoomEffectEntryCount;

#endif /* FLD_ROOM_EFFECT_ENTRY_H */
