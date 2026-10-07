#include "common.h"
#include "dds3obj.h"
#include "pcp_vu0.h"


extern u32 dds3WorldCounter;

extern void dds3BuildVuTransformFromComponents(void *, void *, void *);

typedef struct WorldEntry {
    EffWorldNode *worldNodes;
    u32 unk04;
    EffWorldNode *callbackTarget; /* 0x08: forwarded to both lifecycle helpers */
    u32 unk0C;
    void *resource;
    u32 unk14;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    NodeB *worldIndexNodes;
    u32 unk24;
} WorldEntry;
typedef char WorldEntry_size_must_be_0x28[(sizeof(WorldEntry) == 0x28) ? 1 : -1];

typedef struct {
    u8 pad0[0x18];
    WorldEntry *entry;
    ObjectTransform *transform;
} WorldObject;

extern void dds3DestroyWorldNode(EffWorldNode *node);
extern void dds3DestroyWorldIndexNode(NodeB *node);
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

void dds3SetWorldNodeValue(EffWorldNode *node, u32 value) {
    if (node != NULL) {
        node->value = value;
    }
}

u32 dds3GetWorldNodeValue(EffWorldNode *node) {
    u32 value;

    value = 0;
    if (node != NULL) {
        value = node->value;
    }
    return value;
}

extern void *sdfAllocSizeClassBlock(s32 size);

/* Allocate and clear a world node, then attach it as the object's entry.
   The assignment order is load-bearing: ee-gcc hoists the last statement's
   store out of the independent group, so unk1C stays last and the entry
   store follows it. */
s32 dds3AllocateWorldObjectEntry(WorldObject *object) {
    WorldEntry *entry;

    entry = (WorldEntry *)sdfAllocSizeClassBlock(0x28);
    if (entry == NULL) {
        return 0;
    }
    entry->worldNodes = NULL;
    entry->unk04 = 0;
    entry->callbackTarget = NULL;
    entry->unk0C = 0;
    entry->resource = NULL;
    entry->unk14 = 0;
    entry->unk18 = 0;
    entry->unk1A = -1;
    entry->unk1E = 0;
    entry->worldIndexNodes = NULL;
    entry->unk24 = 0;
    entry->unk1C = -1;
    object->entry = entry;
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

u32 dds3DispatchWorldEntryUpdateCallback(WorldObject *obj) {
    EffWorldNode *callbackTarget;

    callbackTarget = obj->entry->callbackTarget;
    if (callbackTarget != NULL) {
        dds3InvokeWorldCallbackFirst(callbackTarget);
    }
    return 1;
}

u32 dds3DispatchWorldEntryDrawCallback(WorldObject *obj) {
    EffWorldNode *callbackTarget;

    callbackTarget = obj->entry->callbackTarget;
    if (callbackTarget != NULL) {
        dds3InvokeWorldCallbackSecond(callbackTarget);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0010FB00", func_0010FE50);

INCLUDE_SDATA(const s32, "game/code_0010FB00", dds3WorldCounter);

INCLUDE_SDATA(const s32, "game/code_0010FB00", dds3ActiveWorld);

