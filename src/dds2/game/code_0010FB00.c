#include "common.h"
#include "pcp_vu0.h"

extern u32 dds3WorldCounter;

extern void dds3BuildVuTransformFromComponents(u8 *, u8 *, u8 *);

typedef struct {
    void *worldNodes;
    u32 unk04;
    void *callbackTarget; /* 0x08: forwarded to both lifecycle helpers */
    u32 unk0C;
    void *resource;
    u8 pad14[0x0C];
    void *worldIndexNodes;
} WorldEntry;

typedef struct {
    u8 pad0[0x18];
    WorldEntry *entry;
} WorldObject;

extern void dds3DestroyWorldNode(void *node);
extern void dds3DestroyWorldIndexNode(void *node);
extern void func_003297C8(void *resource);
extern void sdfReleaseChipBlock(void *block);

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

INCLUDE_ASM(const s32, "game/code_0010FB00", func_0010FBD0);

INCLUDE_ASM(const s32, "game/code_0010FB00", func_0010FC28);

INCLUDE_ASM(const s32, "game/code_0010FB00", func_0010FC68);

/* The stored sequence wraps at 16 bits even though its backing word is 32 bits. */
void dds3AdvanceWorldCounter(void) {
    dds3WorldCounter = (dds3WorldCounter + 1) & 0xffff;
}

void dds3SetWorldEntryCallbackTarget(WorldEntry *entry, void *callbackTarget) {
    if (entry != NULL) {
        entry->callbackTarget = callbackTarget;
    }
}

void *dds3GetWorldEntryCallbackTarget(WorldEntry *entry) {
    void *callbackTarget;

    callbackTarget = NULL;
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

extern void *func_00328D68(s32 size);

/* Allocate and clear a world node, then attach it as the object's entry.
   The assignment order is load-bearing: ee-gcc hoists the last statement's
   store out of the independent group, so counter1C stays last and the entry
   store follows it. */
s32 dds3AllocateWorldObjectEntry(WorldObject *object) {
    WorldNode *node;

    node = (WorldNode *)func_00328D68(0x28);
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

void func_0010FD58(WorldObject *object)
{
    WorldEntry *entry = object->entry;

    if (entry == NULL) {
        return;
    }
    while (entry->worldNodes != NULL) {
        dds3DestroyWorldNode(entry->worldNodes);
    }
    while (entry->worldIndexNodes != NULL) {
        dds3DestroyWorldIndexNode(entry->worldIndexNodes);
    }
    if (entry->resource != NULL) {
        func_003297C8(entry->resource);
    }
    sdfReleaseChipBlock(entry);
}

u32 func_0010FDF0(WorldObject *obj) {
    void *callbackTarget;

    callbackTarget = obj->entry->callbackTarget;
    if (callbackTarget != NULL) {
        func_0010FC28(callbackTarget);
    }
    return 1;
}

u32 dds3DispatchWorldEntryCallbackTarget(WorldObject *obj) {
    void *callbackTarget;

    callbackTarget = obj->entry->callbackTarget;
    if (callbackTarget != NULL) {
        func_0010FC68(callbackTarget);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0010FB00", func_0010FE50);

INCLUDE_SDATA(const s32, "game/code_0010FB00", dds3WorldCounter);

INCLUDE_SDATA(const s32, "game/code_0010FB00", dds3ActiveWorld);

