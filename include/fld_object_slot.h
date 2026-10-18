#ifndef FLD_OBJECT_SLOT_H
#define FLD_OBJECT_SLOT_H

#include "common.h"

/* One of the 32 field object slots. DDS1 and DDS2 use the same 0x30-byte
 * layout. */
typedef struct FldObjectSlot {
    s32 unk0;
    s32 id;
    s32 activationState;     /* 0x08 */
    s32 activationRequested; /* 0x0C */
    f32 x;                   /* 0x10: placement position */
    f32 y;
    f32 z;
    f32 yaw;                 /* 0x1C: placement yaw, in degrees */
    s32 effectVariant;       /* 0x20 */
    s32 effectNode;          /* 0x24 */
    s32 effectKindIndex;     /* 0x28: row of DDS1's D_0034C8F0 */
    u8 pad2C[4];
} FldObjectSlot;

typedef char FldObjectSlot_activationState_offset_must_be_0x08[
    ((u32)&((FldObjectSlot *)0)->activationState == 0x08) ? 1 : -1];
typedef char FldObjectSlot_size_must_be_0x30[sizeof(FldObjectSlot) == 0x30 ? 1 : -1];

extern FldObjectSlot fldObjectSlots[32];
extern s32 fldObjectSlotCount;

/* A field object's effect kind (0x10), by FldObjectSlot.effectKindIndex. */
typedef struct FldEffectKind {
    s32 unk0;
    s32 unk4;
    s32 kind; /* 0x08 */
    s32 unkC;
} FldEffectKind;

typedef char FldEffectKind_size_must_be_0x10[sizeof(FldEffectKind) == 0x10 ? 1 : -1];

#endif /* FLD_OBJECT_SLOT_H */
