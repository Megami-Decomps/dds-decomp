#ifndef FLD_ROOM_EFFECT_ENTRY_H
#define FLD_ROOM_EFFECT_ENTRY_H

#include "common.h"
#include "eff_transform.h"

/* A room's placed NPC model, in placement order. DDS1 and DDS2 use the same
 * 0x50-byte layout. */
typedef struct fldNpc_tag {
    s32 type;           /* 0x00: source placement key; 999 excludes the row from name lookup. */
    f32 x;              /* 0x04 */
    f32 y;              /* 0x08 */
    f32 z;              /* 0x0C */
    f32 rot[4];         /* 0x10 */
    dds3Basic_t *object; /* 0x20: room effect world node. */
    s16 kind;           /* 0x24 */
    s16 kindId;         /* 0x26 */
    f32 size;           /* 0x28 */
    f32 unk2C;          /* 0x2C: float counter inferred from the paired SMT3 cntf layout. */
    s32 unk30, unk34;
    s32 room;           /* 0x38 */
    s16 unk3C;
    s16 lodIndex;       /* 0x3E: selected LOD copied to the spawned model. */
    char name[0x10];    /* 0x40 */
} fldNpcSlot_t;

typedef char FldRoomEffectEntry_size_check[(sizeof(fldNpcSlot_t) == 0x50) ? 1 : -1];
typedef char FldRoomEffectEntry_object_offset_check[
    ((u32)&((fldNpcSlot_t *)0)->object == 0x20) ? 1 : -1];
typedef char FldRoomEffectEntry_room_offset_check[
    ((u32)&((fldNpcSlot_t *)0)->room == 0x38) ? 1 : -1];
typedef char FldRoomEffectEntry_lodIndex_offset_check[
    ((u32)&((fldNpcSlot_t *)0)->lodIndex == 0x3E) ? 1 : -1];

extern fldNpcSlot_t fldModelSlotTable[];
extern s32 fldNpcModelTable;

#endif /* FLD_ROOM_EFFECT_ENTRY_H */
