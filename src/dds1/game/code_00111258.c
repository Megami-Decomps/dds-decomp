#include "sdk_ps2.h"
#include "common.h"
#include "sdf_chip.h"
#include "dds3obj.h"
#include "dds3_path.h"

s32 dds3AreaBasicTypeHasItems(u32 kind);

extern s32 dds3SearchDeleteBasicArray(WorldValueIndices *indexNode, EffWorldNode *targetNode, s32 processAllMatches);

/* Remove every entry for the node from the owner's world slot selected by the node's kind. */
void dds3AreaBasicGetItemArray(EffWorldNode *worldIndexOwner, EffWorldNode *node) {
    WorldIndexNode *slot = dds3AreaBasicReleaseItem(worldIndexOwner, dds3AreaBasicTypeHasItems(((u8 *)node)[0xF]));

    dds3SearchDeleteBasicArray((WorldValueIndices *)slot, node, 0);
}

/* Select one of the kind-2 owner's two world index nodes. */
WorldIndexNode *dds3AreaBasicReleaseItem(EffWorldNode *worldIndexOwner, s32 slotIndex) {
    WorldIndexNode **slot = worldIndexOwner->data;

    slot += slotIndex;
    return *slot;
}
#include "dds3_world_secondary.h"
/* Copy the slot's world-object words (optionally filtered) into a fresh index node. */
WorldIndexNode *dds3AreaBasicArrangeGetItemArray(EffWorldNode *worldIndexOwner, s32 slotIndex, s32 (*filter)(u32)) {
    WorldIndexNode *slot = dds3AreaBasicReleaseItem(worldIndexOwner, slotIndex);
    WorldIndexNode *result;
    u32 word;

    if (dds3GetBasicArrayCount((WorldValueIndices *)slot) == 0) {
        return NULL;
    }
    result = dds3GetBasicArrayHandle(0);
    dds3SetFirstBasicArray((WorldValueIndices *)slot);
    do {
        word = dds3GetBasicArray((WorldValueIndices *)slot);
        if (filter == NULL || filter(word) != 0) {
            dds3AddBasicArray((WorldValueIndices *)result, 1);
            dds3SetBasicArray((WorldValueIndices *)result, word);
        }
    } while (dds3SetNextBasicArray((WorldValueIndices *)slot) != 0);
    if (dds3GetBasicArrayCount((WorldValueIndices *)result) == 0) {
        dds3FreeBasicArrayHandle(result);
        return NULL;
    }
    return result;
}

EffWorldNode *dds3GetFirstWorldObjectNodeOfKind2(void) {
    WorldIndexNode *indexObject;
    EffWorldNode *node;

    indexObject = dds3SceneBasicGetList(dds3GetControlScene(), 2);
    if (indexObject == NULL) {
        return NULL;
    }
    if (dds3GetBasicArrayCount((WorldValueIndices *)indexObject) == 0) {
        return NULL;
    }
    dds3SetFirstBasicArray((WorldValueIndices *)indexObject);
    node = (EffWorldNode *)(u32)dds3GetBasicArray((WorldValueIndices *)indexObject);
    dds3FreeBasicArrayHandle(indexObject);
    return node;
}


s32 MoverBasicConstructor(EffWorldNode *obj) {
    obj->data = sdfChipAllocDirect(0x10);
    memset(obj->data, 0, 0x10);
    return 1;
}
