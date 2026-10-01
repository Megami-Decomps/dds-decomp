#include "common.h"
#include "pcp_vu0.h"

typedef struct {
    u8 pad0[8];
    u32 callbackTarget; /* 0x08: forwarded to both lifecycle helpers */
} WorldEntry;

typedef struct {
    u8 pad0[0x18];
    WorldEntry *entry;
} WorldObject;

extern u32 D_003BA9B8;

extern void dds3BuildVuTransformFromComponents(void *, void *, void *);

void dds3LoadOrBuildObjectMatrix(u8 *arg0) {
    u8 *obj = *(u8 **)(arg0 + 0x1C);
    u32 flags = *(u32 *)(obj + 0xC0);

    if (flags & 2) {
        VU0_LOAD_MATRIX(obj);
    } else {
        *(u32 *)(obj + 0xC0) = flags | 2;
        dds3BuildVuTransformFromComponents(obj + 0x60, obj + 0x50, obj + 0x40);
        VU0_STORE_MATRIX(obj);
    }
}

s32 dds3TestObjectSphereOverlap(u8 *left, u8 *right) {
    f32 length;
    f32 leftLimit;
    f32 rightLimit;

    VU0_LOAD_VF(vf10, left + 0x40);
    VU0_LOAD_VF(vf11, right + 0x40);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LENGTH_VF10(length);
    leftLimit = *(f32 *)(left + 0xC4);
    rightLimit = *(f32 *)(right + 0xC4);
    if (fabsf(length) - (leftLimit + rightLimit) < 0.0f) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0010F8D8", func_0010F9A8);

INCLUDE_ASM(const s32, "game/code_0010F8D8", func_0010FA00);

INCLUDE_ASM(const s32, "game/code_0010F8D8", func_0010FA40);

/* The stored sequence wraps at 16 bits even though its backing word is 32 bits. */
void dds3AdvanceWorldCounter(void) {
    D_003BA9B8 = (D_003BA9B8 + 1) & 0xffff;
}

void dds3SetWorldEntryCallbackTarget(WorldEntry *entry, u32 callbackTarget) {
    if (entry != NULL) {
        entry->callbackTarget = callbackTarget;
    }
}

u32 dds3GetWorldEntryCallbackTarget(WorldEntry *entry) {
    u32 callbackTarget;

    callbackTarget = 0;
    if (entry != NULL) {
        callbackTarget = entry->callbackTarget;
    }
    return callbackTarget;
}

typedef struct WorldNode {
    u32 unk00; u32 unk04; u32 unk08; u32 unk0C; u32 unk10; u32 unk14;
    s16 counter18; s16 counter1A; s16 counter1C; s16 counter1E;
    u32 unk20; u32 unk24;
} WorldNode;

/* Allocate and clear a world node, then attach it as the object's entry.
   The assignment order is load-bearing: ee-gcc hoists the last statement's
   store out of the independent group, so counter1C stays last and the entry
   store follows it. */
s32 func_0010FAC0(WorldObject *object) {
    WorldNode *node;

    node = (WorldNode *)func_002CFEB8(0x28);
    if (node == NULL) {
        return 0;
    }
    node->unk00 = 0;
    node->unk04 = 0;
    node->unk08 = 0;
    node->unk0C = 0;
    node->unk10 = 0;
    node->unk14 = 0;
    node->counter18 = 0;
    node->counter1A = -1;
    node->counter1E = 0;
    node->unk20 = 0;
    node->unk24 = 0;
    node->counter1C = -1;
    object->entry = (WorldEntry *)node;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0010F8D8", func_0010FB30);

u32 func_0010FBC8(WorldObject *obj) {
    s32 callbackTarget;

    callbackTarget = obj->entry->callbackTarget;
    if (callbackTarget != 0) {
        func_0010FA00(callbackTarget);
    }
    return 1;
}

u32 func_0010FBF8(WorldObject *obj) {
    s32 callbackTarget;

    callbackTarget = obj->entry->callbackTarget;
    if (callbackTarget != 0) {
        func_0010FA40(callbackTarget);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0010F8D8", func_0010FC28);

INCLUDE_SDATA(const s32, "game/code_0010F8D8", D_003BA9B8);

INCLUDE_SDATA(const s32, "game/code_0010F8D8", D_003BA9BC);

