#include "common.h"
#include "dds3obj.h"
#include "pcp_vu0.h"


extern u32 dds3WorldCounter;

extern void dds3BuildVuTransformFromComponents(void *, void *, void *);

typedef struct WorldEntry {
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
    ObjectTransform *transform;
} WorldObject;

extern void dds3DestroyWorldNode(void *node);
extern void dds3DestroyWorldIndexNode(void *node);
extern void sdfReleaseResourceAllocation(void *resource);
extern void sdfReleaseChipBlock(void *block);

/* Load the cached VU matrix, or rebuild and cache it when flags bit 1 is clear. */
void dds3LoadOrBuildObjectMatrix(WorldObject *object) {
    ObjectTransform *transform = object->transform;
    u32 flags = transform->flags;

    if (flags & 2) {
        VU0_LOAD_MATRIX(transform->matrix);
    } else {
        transform->flags = flags | 2;
        dds3BuildVuTransformFromComponents(&transform->scale, &transform->rotation, &transform->position);
        VU0_STORE_MATRIX(transform->matrix);
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



/* Invoke the holder's first / second lifecycle callback when present; the result is 1 when there is nothing to call. */
s32 dds3InvokeWorldCallbackFirst(EffWorldNode *holder) {
    s32 result = 1;

    if (holder != NULL) {
        EffWorldOps *table = holder->ops;

        if (table != NULL && table->update != NULL) {
            result = table->update(holder);
        }
    }
    return result;
}

s32 dds3InvokeWorldCallbackSecond(EffWorldNode *holder) {
    s32 result = 1;

    if (holder != NULL) {
        EffWorldOps *table = holder->ops;

        if (table != NULL && table->draw != NULL) {
            result = table->draw(holder);
        }
    }
    return result;
}

/* Advance and return the 16-bit sequence stored in a 32-bit backing word. */
u32 dds3AdvanceWorldCounter(void) {
    dds3WorldCounter = (dds3WorldCounter + 1) & 0xffff;
    return dds3WorldCounter;
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

extern void *sdfAllocSizeClassBlock(s32 size);

/* Allocate and clear a world node, then attach it as the object's entry.
   The assignment order is load-bearing: ee-gcc hoists the last statement's
   store out of the independent group, so counter1C stays last and the entry
   store follows it. */
s32 dds3AllocateWorldObjectEntry(WorldObject *object) {
    WorldNode *node;

    node = (WorldNode *)sdfAllocSizeClassBlock(0x28);
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
        sdfReleaseResourceAllocation(entry->resource);
    }
    sdfReleaseChipBlock(entry);
}

u32 func_0010FDF0(WorldObject *obj) {
    void *callbackTarget;

    callbackTarget = obj->entry->callbackTarget;
    if (callbackTarget != NULL) {
        dds3InvokeWorldCallbackFirst((EffWorldNode *)callbackTarget);
    }
    return 1;
}

u32 dds3DispatchWorldEntryCallbackTarget(WorldObject *obj) {
    void *callbackTarget;

    callbackTarget = obj->entry->callbackTarget;
    if (callbackTarget != NULL) {
        dds3InvokeWorldCallbackSecond((EffWorldNode *)callbackTarget);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0010FB00", func_0010FE50);

INCLUDE_SDATA(const s32, "game/code_0010FB00", dds3WorldCounter);

INCLUDE_SDATA(const s32, "game/code_0010FB00", dds3ActiveWorld);

