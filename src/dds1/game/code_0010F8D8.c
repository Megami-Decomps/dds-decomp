#include "common.h"
#include "dds3obj.h"
#include "pcp_vu0.h"

extern u32 dds3WorldCounter;

extern void dds3BuildVuTransformFromComponents(void *, void *, void *);
extern void dds3DestroyWorldNode(EffWorldNode *node);
extern void dds3DestroyWorldIndexNode(NodeB *node);
extern void sdfReleaseResourceAllocation(void *resource);
extern void sdfReleaseChipBlock(void *block);

/* Load the cached VU matrix, or rebuild and cache it when flags bit 1 is clear. */
void dds3LoadOrBuildObjectMatrix(EffWorldNode *object) {
    ObjectTransform *transform = object->inner;
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

INCLUDE_ASM(const s32, "game/code_0010F8D8", func_0010F9A8);



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

/* Allocate and clear the world node's 0x28-byte WorldInfo payload. The native
   callback stores the payload on the node before initializing its fields. */
s32 dds3AllocateWorldObjectEntry(EffWorldNode *object) {
    WorldInfo *entry;

    entry = (WorldInfo *)sdfAllocSizeClassBlock(0x28);
    if (entry == NULL) {
        return 0;
    }
    object->data = entry;
    entry->firstNode = NULL;
    entry->lastNode = NULL;
    entry->primaryObject = NULL;
    entry->secondaryObject = NULL;
    entry->entryAllocation = NULL;
    entry->entries = NULL;
    entry->entryCapacity = 0;
    entry->freeHeadIndex = -1;
    entry->freeEntryCount = 0;
    entry->firstIndex = NULL;
    entry->lastIndex = NULL;
    entry->freeTailIndex = -1;
    return 1;
}

void dds3DestroyWorldObjectEntry(EffWorldNode *object) {
    WorldInfo *entry = object->data;

    if (entry == NULL) {
        return;
    }
    while (entry->firstNode != NULL) {
        dds3DestroyWorldNode(entry->firstNode);
    }
    while (entry->firstIndex != NULL) {
        dds3DestroyWorldIndexNode(entry->firstIndex);
    }
    if (entry->entryAllocation != NULL) {
        sdfReleaseResourceAllocation(entry->entryAllocation);
    }
    sdfReleaseChipBlock(entry);
}

u32 dds3DispatchWorldEntryUpdateCallback(EffWorldNode *obj) {
    WorldInfo *entry = obj->data;
    EffWorldNode *callbackTarget;

    callbackTarget = (EffWorldNode *)entry->primaryObject;
    if (callbackTarget != NULL) {
        dds3InvokeWorldCallbackFirst(callbackTarget);
    }
    return 1;
}

u32 dds3DispatchWorldEntryDrawCallback(EffWorldNode *obj) {
    WorldInfo *entry = obj->data;
    EffWorldNode *callbackTarget;

    callbackTarget = (EffWorldNode *)entry->primaryObject;
    if (callbackTarget != NULL) {
        dds3InvokeWorldCallbackSecond(callbackTarget);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0010F8D8", func_0010FC28);

INCLUDE_SDATA(const s32, "game/code_0010F8D8", dds3WorldCounter);

INCLUDE_SDATA(const s32, "game/code_0010F8D8", dds3ActiveWorld);

