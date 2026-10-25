#ifndef FLD_OBJECT_SLOT_H
#define FLD_OBJECT_SLOT_H

#include "common.h"

struct EffNode;
struct dds3Basic_t;

/* Pair lifecycle request and current-state fields with these transitions. */
#define FLD_OBJECT_ACTIVATION_INACTIVE 0
#define FLD_OBJECT_ACTIVATION_ANIMATING 1
#define FLD_OBJECT_ACTIVATION_WAITING_FOR_ENCOUNTER 2
#define FLD_OBJECT_ACTIVATION_ACTIVE 3

/* One of the 32 field object slots. DDS1 and DDS2 use the same 0x30-byte
 * layout. */
typedef struct FldObjectSlot {
    struct dds3Basic_t *unitObject; /* 0x00: borrowed world node owned by the active scene. */
    s32 id;
    s32 activationState;     /* 0x08 */
    s32 activationRequested; /* 0x0C */
    f32 x;                   /* 0x10: placement position */
    f32 y;
    f32 z;
    f32 yaw;                 /* 0x1C: placement yaw, in degrees */
    s32 activationKind;      /* 0x20: copied from FldActivationEntry.kind. */
    struct EffNode *effectNode; /* 0x24: effect deleted when the slot table resets/releases. */
    s32 effectKindIndex;     /* 0x28: row of DDS1's D_0034C8F0 */
    u8 pad2C[4];
} FldObjectSlot;

typedef char FldObjectSlot_activationState_offset_must_be_0x08[
    ((u32)&((FldObjectSlot *)0)->activationState == 0x08) ? 1 : -1];
typedef char FldObjectSlot_activationRequested_offset_must_be_0x0C[
    ((u32)&((FldObjectSlot *)0)->activationRequested == 0x0C) ? 1 : -1];
typedef char FldObjectSlot_activationKind_offset_must_be_0x20[
    ((u32)&((FldObjectSlot *)0)->activationKind == 0x20) ? 1 : -1];
typedef char FldObjectSlot_unitObject_offset_must_be_0x00[
    ((u32)&((FldObjectSlot *)0)->unitObject == 0x00) ? 1 : -1];
typedef char FldObjectSlot_effectNode_offset_must_be_0x24[
    ((u32)&((FldObjectSlot *)0)->effectNode == 0x24) ? 1 : -1];
typedef char FldObjectSlot_size_must_be_0x30[sizeof(FldObjectSlot) == 0x30 ? 1 : -1];

extern FldObjectSlot fldTakaraSlotTable[32];
extern s32 fldTakaraCount;

/* A field object's effect kind (0x10), by FldObjectSlot.effectKindIndex. */
typedef struct FldEffectKind {
    s32 unk0;
    s32 unk4;
    s32 kind; /* 0x08 */
    s32 unkC;
} FldEffectKind;

typedef char FldEffectKind_size_must_be_0x10[sizeof(FldEffectKind) == 0x10 ? 1 : -1];

#endif /* FLD_OBJECT_SLOT_H */
