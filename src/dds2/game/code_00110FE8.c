#include "common.h"
#include "sdf_chip.h"
#include "dds3obj.h"
#include "dds3_area_callback.h"
#include "evt_world.h"

s32 dds3SceneBasicExistBasicFromType(dds3Basic_t *object, s32 slotIndex, const dds3Basic_t *targetNode) {
    dds3Basic_t *node;

    if (object == NULL || targetNode == 0) {
        return 0;
    }
    node = ((EvtWorldTable *)object->data)->slots[slotIndex].head;
    while (node != NULL) {
        if (node == targetNode) {
            return 1;
        }
        node = node->next;
    }
    return 0;
}

s32 dds3SceneBasicExistBasic(dds3Basic_t *object, dds3Basic_t *targetNode) {
    EvtWorldSlot *slots;
    s32 i;

    if (object == NULL || targetNode == 0) {
        return 0;
    }
    slots = ((EvtWorldTable *)object->data)->slots;
    for (i = 0; i < 0x12; i++) {
        dds3Basic_t *node = slots[i].head;

        if (node == NULL) {
            continue;
        }
        do {
            if (node == targetNode) {
                return 1;
            }
            node = node->next;
        } while (node != 0);
    }
    return 0;
}

void dds3SetMapNo(dds3Basic_t *worldNode, u32 areaRoomKey) {
    if (worldNode != NULL) {
        ((EvtWorldTable *)worldNode->data)->areaRoomKey = areaRoomKey;
    }
}

s32 dds3GetMapNo(dds3Basic_t *worldNode) {
    if (worldNode == NULL) {
        return -1;
    }
    return ((EvtWorldTable *)worldNode->data)->areaRoomKey;
}

dds3Basic_t *dds3SceneBasicSearchNameAndType(dds3Basic_t *object, s32 slotIndex, const char *name) {
    dds3Basic_t *node;
    u8 *s;
    const u8 *nameBytes = (const u8 *)name;
    s32 i;

    if (object == NULL || name == NULL) {
        return NULL;
    }
    node = ((EvtWorldTable *)object->data)->slots[slotIndex].head;
    while (node != NULL) {
        s = ((u8 *)node->value);
        if (s == NULL) {
            node = node->next;
            continue;
        }
        if (s[0] == '\0') {
            node = node->next;
            continue;
        }
        i = 0;
        if (nameBytes[0] == s[0]) {
            do {
                if (nameBytes[i] == '\0') {
                    return node;
                }
                i++;
            } while (nameBytes[i] == ((u8 *)node->value)[i]);
        }
        node = node->next;
    }
    return NULL;
}

dds3Basic_t *dds3SceneBasicSearchName(dds3Basic_t *object, const char *name) {
    dds3Basic_t *node;
    s32 i;

    if (object == NULL || name == NULL) {
        return NULL;
    }
    for (i = 0; i < 0x12; i++) {
        node = dds3SceneBasicSearchNameAndType(object, i, name);
        if (node != NULL) {
            return node;
        }
    }
    return NULL;
}



s32 AreaBasicConstructor(dds3Basic_t *object) {
    WorldIndexNode **indexNodes;
    u32 i;

    indexNodes = sdfChipAllocDirect(8);
    object->data = indexNodes;
    for (i = 0; i < 2; i++) {
        indexNodes[i] = dds3GetBasicArrayHandle(0);
    }
    return 1;
}

s32 CB_ReleaseObjectBasic(dds3Basic_t *areaObject);



void AreaBasicDestructor(dds3Basic_t *object) {
    WorldIndexNode **indexNodes;
    u32 i;

    indexNodes = object->data;
    dds3ForEachBasicArray((dds3BasicArrayHandle_t *)indexNodes[0], CB_ReleaseObjectBasic);
    dds3ForEachBasicArray((dds3BasicArrayHandle_t *)indexNodes[1], CB_ReleaseObjectBasic);
    for (i = 0; i < 2; i++) {
        dds3FreeBasicArrayHandle(indexNodes[i]);
    }
    sdfChipFreeDirect(indexNodes);
}

u32 AreaBasicMainprocuder(void) {
    return 1;
}

s32 AreaBasicDrawprocuder(dds3Basic_t *object) {
    WorldIndexNode **indexNodes = object->data;

    dds3ForEachBasicArray((dds3BasicArrayHandle_t *)indexNodes[0], CB_BasicDrawProcuder);
    return 1;
}
