#include "common.h"
#include "sdf_chip.h"
#include "sdf_resource.h"
#include "dds3obj.h"
#include "pcp_vu0.h"
#include "fpu.h"

extern u32 dds3WorldCounter;

extern void dds3BuildVuTransformFromComponents(void *, void *, void *);
extern void dds3DestroyWorldNode(EffWorldNode *node);

/* Load the cached VU matrix, or rebuild and cache it when flags bit 1 is clear. */
void dds3LoadOrBuildObjectMatrix(EffWorldNode *object) {
    ObjectTransform *transform = object->inner;
    u32 flags = transform->flags;

    if (flags & OBJECT_TRANSFORM_FLAG_MATRIX_CACHE_VALID) {
        VU0_LOAD_MATRIX(transform->matrix);
    } else {
        transform->flags = flags | OBJECT_TRANSFORM_FLAG_MATRIX_CACHE_VALID;
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

/* vu0 routine: whether the distance between the two points at +0x40 is inside the second object's radius at +0xC4 */
s32 func_0010F9A8(u8 *left, u8 *right) {
    f32 length;
    f32 rightLimit;
    VU0_LOAD_VF(vf10, left + 0x40);
    VU0_LOAD_VF(vf11, right + 0x40);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LENGTH_VF10(length);
    rightLimit = *(f32 *)(right + 0xC4);
    return ffabsf(length) - rightLimit < 0.0f;
}



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
        node->value = (const char *)value;
    }
}

u32 dds3GetWorldNodeValue(EffWorldNode *node) {
    u32 value;

    value = 0;
    if (node != NULL) {
        value = (u32)node->value;
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

extern EffWorldNode *dds3CreateWorldNodeForKind(u32 kind);
extern EffWorldNode *dds3ActiveWorld;

EffWorldNode *func_0010FC28(s32 capacity) {
    EffWorldNode *world;
    WorldInfo *info;
    struct SdfMemBlock *allocation;
    s32 index;

    if (dds3ActiveWorld != NULL) {
        return NULL;
    }
    if ((u32)capacity - 0x20u >= 0x7FE0u) {
        return NULL;
    }

    world = dds3CreateWorldNodeForKind(0);
    info = world->data;
    allocation = sdfAllocGeneralBlock(capacity * 8);
    info->entryAllocation = allocation;
    info->entries = (WorldValueEntry *)sdfResourceRetainAddress(allocation);
    info->entryCapacity = capacity;

    for (index = 0; index < capacity; index++) {
        if (index == 0) {
            info->entries[index].previousIndex = -1;
        } else {
            info->entries[index].previousIndex = index - 1;
        }
        if (index == capacity - 1) {
            info->entries[index].nextIndex = -1;
        } else {
            info->entries[index].nextIndex = index + 1;
        }
        info->entries[index].value = 0;
    }
    info->freeHeadIndex = 0;
    info->freeTailIndex = capacity - 1;
    info->freeEntryCount = capacity;
    dds3ActiveWorld = world;
    return world;
}


INCLUDE_SDATA(const s32, "game/code_0010F8D8", dds3WorldCounter);

INCLUDE_SDATA(const s32, "game/code_0010F8D8", dds3ActiveWorld);

