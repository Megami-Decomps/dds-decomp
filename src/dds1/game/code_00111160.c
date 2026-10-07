#include "common.h"
#include "eff_transform.h"

struct NodeB;

s32 func_00111160(u32 kind) {
    s32 result = 0;

    if (kind >= 4) {
        if (kind >= 8) {
            result = kind == 8;
        }
    }
    return result;
}


extern EffWorldNode *dds3AppendWorldObjectNode();

EffWorldNode *evtSpawnActionObj2(s32 firstValue, s32 secondValue) {
    EffWorldNode *obj = dds3AppendWorldObjectNode(2);

    obj->key = firstValue;
    obj->value = secondValue;
    return obj;
}

extern s32 dds3GetWorldSlotValue();
extern u32 dds3ResetObjectValueCursor();
extern s32 dds3SeekWorldNode(WorldValueIndices *indexNode, u32 targetWord);
extern void dds3GrowWorldValueChain();
extern u32 dds3WriteIndexedWorldObjectWord();

void dds3EnsureWorldNodeInSlot(s32 object, u8 *node) {
    s32 slot = dds3GetWorldSlotValue(object, func_00111160(node[0xF]));

    dds3ResetObjectValueCursor(slot);
    if (dds3SeekWorldNode((WorldValueIndices *)slot, (u32)node) != 1) {
        dds3GrowWorldValueChain(slot, 1);
        dds3WriteIndexedWorldObjectWord(slot, node);
    }
}

INCLUDE_ASM(const s32, "game/code_00111160", func_00111258);

/* Read a 32-bit value from the object's array of world slots. */
s32 dds3GetWorldSlotValue(u8 *object, s32 index) {
    return *(s32 *)(*(u8 **)(object + 0x18) + (index << 2));
}

extern u32 dds3GetWorldValueCount();
extern struct NodeB *dds3AppendWorldIndexNode(s32 initialCount);
extern u32 dds3ReadIndexedWorldObjectWord();
extern u32 dds3AdvanceObjectValueCursor();
extern void dds3DestroyWorldIndexNode(struct NodeB *node);
extern void *dds3GetWorldSecondaryObject(void);
extern void *dds3CopyWorldListToValueChain(void *object, s32 kind);

/* Copy the slot's world-object words (optionally filtered) into a fresh index node. */
void *dds3CopyFilteredWorldSlot(s32 object, s32 index, s32 (*filter)(u32)) {
    s32 slot = dds3GetWorldSlotValue(object, index);
    struct NodeB *result;
    u32 word;

    if (dds3GetWorldValueCount(slot) == 0) {
        return NULL;
    }
    result = dds3AppendWorldIndexNode(0);
    dds3ResetObjectValueCursor(slot);
    do {
        word = dds3ReadIndexedWorldObjectWord(slot);
        if (filter == NULL || filter(word) != 0) {
            dds3GrowWorldValueChain(result, 1);
            dds3WriteIndexedWorldObjectWord(result, word);
        }
    } while (dds3AdvanceObjectValueCursor(slot) != 0);
    if (dds3GetWorldValueCount(result) == 0) {
        dds3DestroyWorldIndexNode(result);
        return NULL;
    }
    return result;
}

void *dds3GetFirstWorldObjectNodeOfKind2(void) {
    void *indexObject;
    void *node;

    indexObject = dds3CopyWorldListToValueChain(dds3GetWorldSecondaryObject(), 2);
    if (indexObject == NULL) {
        return NULL;
    }
    if (dds3GetWorldValueCount(indexObject) == 0) {
        return NULL;
    }
    dds3ResetObjectValueCursor(indexObject);
    node = (void *)dds3ReadIndexedWorldObjectWord(indexObject);
    dds3DestroyWorldIndexNode((struct NodeB *)indexObject);
    return node;
}

extern void *sdfAllocSizeClassBlock(s32 size);


s32 dds3AllocateClearedObjectWork(EffWorldNode *obj) {
    obj->data = sdfAllocSizeClassBlock(0x10);
    memset(obj->data, 0, 0x10);
    return 1;
}

void dds3ReleaseWorldSlotResource(EffWorldNode *obj) {
    u32 *slot;

    slot = ((u32 *)obj->data);
    dds3ReleaseObjectResource();
    dds3ExchangeSlot(*slot, 0, 1);
    sdfReleaseChipBlock(slot);
}
