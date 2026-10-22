#include "common.h"
#include "sdf_chip.h"
#include "sdf_resource.h"
#include "dds3obj.h"
#include "dds3_world_value.h"
#include "pcp_vu0.h"
#include "fpu.h"
#include "dds3_vu_transform.h"
#include "dds3_world_counter.h"

extern u32 dds3ObjectIdCounter;

extern void dds3DeleteScene(dds3Basic_t *node);

/* Load the cached VU matrix, or rebuild and cache it when flags bit 1 is clear. */
void dds3GetTMPoseSet(dds3Basic_t *object) {
    dds3PoseSet_t *transform = object->inner;
    u32 flags = transform->flags;

    if (flags & OBJECT_TRANSFORM_FLAG_MATRIX_CACHE_VALID) {
        VU0_LOAD_MATRIX(transform->matrix);
    } else {
        transform->flags = flags | OBJECT_TRANSFORM_FLAG_MATRIX_CACHE_VALID;
        dds3MakeMatrix(transform->scale, transform->rotation, transform->position);
        VU0_STORE_MATRIX(transform->matrix);
    }
}

extern float fabsf(float);

s32 dds3ComparePosPoseSet(dds3PoseSet_t *left, dds3PoseSet_t *right) {
    f32 length;
    f32 leftLimit;
    f32 rightLimit;

    VU0_LOAD_VF(vf10, left->position);
    VU0_LOAD_VF(vf11, right->position);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LENGTH_VF10(length);
    leftLimit = left->radius;
    rightLimit = right->radius;
    if (fabsf(length) - (leftLimit + rightLimit) < 0.0f) {
        return 1;
    }
    return 0;
}

/* vu0 routine: whether the distance between the position vectors is inside the second object's radius */
s32 dds3ComparePosPoseSet2(dds3PoseSet_t *left, dds3PoseSet_t *right) {
    f32 length;
    f32 rightLimit;

    VU0_LOAD_VF(vf10, left->position);
    VU0_LOAD_VF(vf11, right->position);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LENGTH_VF10(length);
    rightLimit = right->radius;
    if (ffabsf(length) - rightLimit < 0.0f) {
        return 1;
    }
    return 0;
}



/* Invoke the holder's first / second lifecycle callback when present; the result is 1 when there is nothing to call. */
s32 dds3CallBasicMainProc(dds3Basic_t *holder) {
    s32 result = 1;

    if (holder != NULL) {
        EffWorldOps *table = holder->ops;

        if (table != NULL && table->update != NULL) {
            result = table->update(holder);
        }
    }
    return result;
}

s32 dds3CallBasicDrawProc(dds3Basic_t *holder) {
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
u32 dds3AllocObjectId(void) {
    dds3ObjectIdCounter = (dds3ObjectIdCounter + 1) & 0xffff;
    return dds3ObjectIdCounter;
}

void dds3SetUnitObjectId(dds3Basic_t *node, u32 worldNodeValue) {
    if (node != NULL) {
        node->value = (const char *)worldNodeValue;
    }
}

u32 dds3GetWorldNodeValue(dds3Basic_t *node) {
    u32 worldNodeValue;

    worldNodeValue = 0;
    if (node != NULL) {
        worldNodeValue = (u32)node->value;
    }
    return worldNodeValue;
}


/* Allocate and clear the world node's 0x28-byte WorldInfo payload. The native
   callback stores the payload on the node before initializing its fields. */
s32 worldconstructor(dds3Basic_t *object) {
    WorldInfo *entry;

    entry = (WorldInfo *)sdfChipAllocDirect(0x28);
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

void worlddestructor(dds3Basic_t *object)
{
    WorldInfo *entry = object->data;

    if (entry == NULL) {
        return;
    }
    while (entry->firstNode != NULL) {
        dds3DeleteScene(entry->firstNode);
    }
    while (entry->firstIndex != NULL) {
        dds3FreeBasicArrayHandle(entry->firstIndex);
    }
    if (entry->entryAllocation != NULL) {
        sdfMemFreeDirect(entry->entryAllocation);
    }
    sdfChipFreeDirect(entry);
}

u32 worldmainprocuder(dds3Basic_t *obj) {
    WorldInfo *entry = obj->data;
    dds3Basic_t *callbackTarget;

    callbackTarget = (dds3Basic_t *)entry->primaryObject;
    if (callbackTarget != NULL) {
        dds3CallBasicMainProc(callbackTarget);
    }
    return 1;
}

u32 worlddrawprocuder(dds3Basic_t *obj) {
    WorldInfo *entry = obj->data;
    dds3Basic_t *callbackTarget;

    callbackTarget = (dds3Basic_t *)entry->primaryObject;
    if (callbackTarget != NULL) {
        dds3CallBasicDrawProc(callbackTarget);
    }
    return 1;
}

extern dds3Basic_t *dds3AllocBasic(u32 kind);
extern dds3Basic_t *dds3TheWorld;

dds3Basic_t *dds3AddWorld(s32 capacity) {
    dds3Basic_t *world;
    WorldInfo *info;
    struct sdfMemHandle_t *allocation;
    s32 index;

    if (dds3TheWorld != NULL) {
        return NULL;
    }
    if ((u32)capacity - 0x20u >= 0x7FE0u) {
        return NULL;
    }

    world = dds3AllocBasic(0);
    info = world->data;
    allocation = sdfMemAllocDirect(capacity * 8);
    info->entryAllocation = allocation;
    info->entries = (WorldValueEntry *)sdfMemLock(allocation);
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
    dds3TheWorld = world;
    return world;
}

INCLUDE_SDATA(const s32, "game/code_0010FB00", dds3ObjectIdCounter);

INCLUDE_SDATA(const s32, "game/code_0010FB00", dds3TheWorld);

