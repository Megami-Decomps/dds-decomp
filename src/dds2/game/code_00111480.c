#include "sdk_ps2.h"
#include "common.h"
#include "sdf_chip.h"
#include "dds3obj.h"
#include "dds3_basic_array_search.h"
#include "dds3_path.h"

s32 dds3SelectOwnerWorldIndexSlot(u32 kind);

/* Remove every entry for the node from the owner's world slot selected by the node's kind. */
void dds3AreaBasicGetItemArray(dds3Basic_t *worldIndexOwner, dds3Basic_t *node) {
    WorldIndexNode *slot =
        dds3AreaBasicGetWorldIndexNode(worldIndexOwner, dds3SelectOwnerWorldIndexSlot(node->kindTag >> 24));

    dds3SearchDeleteBasicArray((dds3BasicArrayHandle_t *)slot, node, 0);
}

/* Select one of the kind-2 owner's two world index nodes. */
WorldIndexNode *dds3AreaBasicGetWorldIndexNode(dds3Basic_t *worldIndexOwner, s32 slotIndex) {
    WorldIndexNode **slot = worldIndexOwner->data;

    slot += slotIndex;
    return *slot;
}
#include "dds3_world_secondary.h"
/* Copy the slot's world-object words (optionally filtered) into a fresh index node. */
WorldIndexNode *dds3AreaBasicArrangeGetItemArray(dds3Basic_t *worldIndexOwner, s32 slotIndex, s32 (*filter)(dds3Basic_t *)) {
    WorldIndexNode *slot = dds3AreaBasicGetWorldIndexNode(worldIndexOwner, slotIndex);
    WorldIndexNode *result;
    dds3Basic_t *value;

    if (dds3GetBasicArrayCount((dds3BasicArrayHandle_t *)slot) == 0) {
        return NULL;
    }
    result = dds3GetBasicArrayHandle(0);
    dds3SetFirstBasicArray((dds3BasicArrayHandle_t *)slot);
    do {
        value = dds3GetBasicArray((dds3BasicArrayHandle_t *)slot);
        if (filter == NULL || filter(value) != 0) {
            dds3AddBasicArray((dds3BasicArrayHandle_t *)result, 1);
            dds3SetBasicArray((dds3BasicArrayHandle_t *)result, value);
        }
    } while (dds3SetNextBasicArray((dds3BasicArrayHandle_t *)slot) != 0);
    if (dds3GetBasicArrayCount((dds3BasicArrayHandle_t *)result) == 0) {
        dds3FreeBasicArrayHandle(result);
        return NULL;
    }
    return result;
}

dds3Basic_t *dds3GetFirstWorldObjectNodeOfKind2(void) {
    WorldIndexNode *indexObject;
    dds3Basic_t *node;

    indexObject = dds3SceneBasicGetList(dds3GetControlScene(), 2);
    if (indexObject == NULL) {
        return NULL;
    }
    if (dds3GetBasicArrayCount((dds3BasicArrayHandle_t *)indexObject) == 0) {
        return NULL;
    }
    dds3SetFirstBasicArray((dds3BasicArrayHandle_t *)indexObject);
    node = dds3GetBasicArray((dds3BasicArrayHandle_t *)indexObject);
    dds3FreeBasicArrayHandle(indexObject);
    return node;
}


s32 MoverBasicConstructor(dds3Basic_t *obj) {
    obj->data = sdfChipAllocDirect(0x10);
    memset(obj->data, 0, 0x10);
    return 1;
}
