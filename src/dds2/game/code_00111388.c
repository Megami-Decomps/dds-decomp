#include "common.h"
#include "dds3obj.h"

typedef struct WorldSlotObject {
    u8 pad00[0x18];
    u32 *slots;
} WorldSlotObject;


extern EffWorldNode *dds3AppendWorldObjectNode();

extern void *sdfAllocSizeClassBlock(s32 size);


s32 func_00111388(u32 kind) {
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

extern u16 dds3GetWorldValueCount(WorldValueIndices *object);
extern u32 dds3ResetObjectValueCursor(WorldValueIndices *object);
extern s32 dds3SeekWorldNode(WorldValueIndices *indexNode, u32 targetWord);
extern void dds3GrowWorldValueChain(WorldValueIndices *object, s32 count);
extern u32 dds3WriteIndexedWorldObjectWord(WorldValueIndices *object, u32 value);
extern u32 dds3ReadIndexedWorldObjectWord(WorldValueIndices *object);
extern u32 dds3AdvanceObjectValueCursor(WorldValueIndices *object);
extern WorldValueIndices *dds3GetWorldSlotValue(u8 *object, s32 index);

void dds3EnsureWorldNodeInSlot(s32 object, u8 *node) {
    WorldValueIndices *slot = dds3GetWorldSlotValue((u8 *)object, func_00111388(node[0xF]));

    dds3ResetObjectValueCursor(slot);
    if (dds3SeekWorldNode(slot, (u32)node) != 1) {
        dds3GrowWorldValueChain(slot, 1);
        dds3WriteIndexedWorldObjectWord(slot, (u32)node);
    }
}

INCLUDE_ASM(const s32, "game/code_00111388", func_00111480);

/* Read a 32-bit value from the object's array of world slots. */
WorldValueIndices *dds3GetWorldSlotValue(u8 *object, s32 index) {
    return (WorldValueIndices *)(u32)(*(s32 *)(*(u8 **)(object + 0x18) + (index << 2)));
}

extern NodeB *dds3AppendWorldIndexNode(s32 initialCount);
extern void dds3DestroyWorldIndexNode(NodeB *node);
extern void *dds3GetWorldSecondaryObject(void);
extern NodeB *dds3CopyWorldListToValueChain(EffWorldNode *object, s32 kind);

/* Copy the slot's world-object words (optionally filtered) into a fresh index node. */
NodeB *dds3CopyFilteredWorldSlot(s32 object, s32 index, s32 (*filter)(u32)) {
    WorldValueIndices *slot = dds3GetWorldSlotValue((u8 *)object, index);
    NodeB *result;
    u32 word;

    if (dds3GetWorldValueCount(slot) == 0) {
        return NULL;
    }
    result = dds3AppendWorldIndexNode(0);
    dds3ResetObjectValueCursor(slot);
    do {
        word = dds3ReadIndexedWorldObjectWord(slot);
        if (filter == NULL || filter(word) != 0) {
            dds3GrowWorldValueChain((WorldValueIndices *)result, 1);
            dds3WriteIndexedWorldObjectWord((WorldValueIndices *)result, word);
        }
    } while (dds3AdvanceObjectValueCursor(slot) != 0);
    if (dds3GetWorldValueCount((WorldValueIndices *)result) == 0) {
        dds3DestroyWorldIndexNode(result);
        return NULL;
    }
    return result;
}

EffWorldNode *dds3GetFirstWorldObjectNodeOfKind2(void) {
    NodeB *indexObject;
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

void dds3ReleaseWorldSlotResource(WorldSlotObject *object) {
    u32 *resource;

    resource = object->slots;
    dds3ReleaseObjectResource();
    dds3ExchangeSlot(*resource, 0, 1);
    sdfReleaseChipBlock(resource);
}
