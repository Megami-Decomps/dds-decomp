#include "common.h"
#include "sdf_chip.h"
#include "dds3obj.h"
#include "dds3_path.h"


s32 func_00111160(u32 kind) {
    s32 result = 0;

    if (kind >= 4) {
        if (kind >= 8) {
            result = kind == 8;
        }
    }
    return result;
}



EffWorldNode *evtSpawnActionObj2(s32 firstValue, s32 secondValue) {
    EffWorldNode *obj = dds3AppendWorldObjectNode(2);

    obj->key = firstValue;
    obj->value = secondValue;
    return obj;
}

void dds3EnsureWorldNodeInSlot(EffWorldNode *object, EffWorldNode *node) {
    WorldIndexNode *slot = dds3GetWorldSlotValue(object, func_00111160(((u8 *)node)[0xF]));

    dds3ResetObjectValueCursor((WorldValueIndices *)slot);
    if (dds3SeekWorldNode((WorldValueIndices *)slot, (u32)node) != 1) {
        dds3GrowWorldValueChain((WorldValueIndices *)slot, 1);
        dds3WriteIndexedWorldObjectWord((WorldValueIndices *)slot, (u32)node);
    }
}

INCLUDE_ASM(const s32, "game/code_00111160", func_00111258);

/* Select one of the kind-2 owner's two world index nodes. */
WorldIndexNode *dds3GetWorldSlotValue(EffWorldNode *object, s32 index) {
    WorldIndexNode **slot = object->data;

    slot += index;
    return *slot;
}

extern void *dds3GetWorldSecondaryObject(void);

/* Copy the slot's world-object words (optionally filtered) into a fresh index node. */
WorldIndexNode *dds3CopyFilteredWorldSlot(EffWorldNode *object, s32 index, s32 (*filter)(u32)) {
    WorldIndexNode *slot = dds3GetWorldSlotValue(object, index);
    WorldIndexNode *result;
    u32 word;

    if (dds3GetWorldValueCount((WorldValueIndices *)slot) == 0) {
        return NULL;
    }
    result = dds3AppendWorldIndexNode(0);
    dds3ResetObjectValueCursor((WorldValueIndices *)slot);
    do {
        word = dds3ReadIndexedWorldObjectWord((WorldValueIndices *)slot);
        if (filter == NULL || filter(word) != 0) {
            dds3GrowWorldValueChain((WorldValueIndices *)result, 1);
            dds3WriteIndexedWorldObjectWord((WorldValueIndices *)result, word);
        }
    } while (dds3AdvanceObjectValueCursor((WorldValueIndices *)slot) != 0);
    if (dds3GetWorldValueCount((WorldValueIndices *)result) == 0) {
        dds3DestroyWorldIndexNode(result);
        return NULL;
    }
    return result;
}

EffWorldNode *dds3GetFirstWorldObjectNodeOfKind2(void) {
    WorldIndexNode *indexObject;
    EffWorldNode *node;

    indexObject = dds3CopyWorldListToValueChain(dds3GetWorldSecondaryObject(), 2);
    if (indexObject == NULL) {
        return NULL;
    }
    if (dds3GetWorldValueCount((WorldValueIndices *)indexObject) == 0) {
        return NULL;
    }
    dds3ResetObjectValueCursor((WorldValueIndices *)indexObject);
    node = (EffWorldNode *)(u32)dds3ReadIndexedWorldObjectWord((WorldValueIndices *)indexObject);
    dds3DestroyWorldIndexNode(indexObject);
    return node;
}



s32 dds3AllocateClearedObjectWork(EffWorldNode *obj) {
    obj->data = sdfAllocSizeClassBlock(0x10);
    memset(obj->data, 0, 0x10);
    return 1;
}

extern void dds3ReleaseObjectResource(EffWorldNode *object);

void dds3ReleaseWorldSlotResource(EffWorldNode *object) {
    Dds3SlotResource *resource;

    resource = object->data;
    dds3ReleaseObjectResource(object);
    dds3ExchangeSlot(resource->target, 0, 1);
    sdfReleaseChipBlock(resource);
}
