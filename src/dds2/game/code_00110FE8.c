#include "common.h"
#include "sdf_chip.h"
#include "dds3obj.h"
#include "dds3_scene_membership.h"
#include "dds3_area_callback.h"
#include "evt_world.h"

Dds3SceneMembershipResult dds3SceneBasicExistBasicFromType(
    const dds3Basic_t *object, s32 slotIndex, const dds3Basic_t *targetNode) {
    const dds3Basic_t *node;

    if (object == NULL || targetNode == 0) {
        return DDS3_SCENE_MEMBERSHIP_ABSENT;
    }
    node = ((const evtWorldTable_t *)object->workdata)->slots[slotIndex].head;
    while (node != NULL) {
        if (node == targetNode) {
            return DDS3_SCENE_MEMBERSHIP_PRESENT;
        }
        node = node->next;
    }
    return DDS3_SCENE_MEMBERSHIP_ABSENT;
}

Dds3SceneMembershipResult dds3SceneBasicExistBasic(
    const dds3Basic_t *object, const dds3Basic_t *targetNode) {
    const evtWorldSlot_t *slots;
    s32 i;

    if (object == NULL || targetNode == 0) {
        return DDS3_SCENE_MEMBERSHIP_ABSENT;
    }
    slots = ((const evtWorldTable_t *)object->workdata)->slots;
    for (i = 0; i < 0x12; i++) {
        const dds3Basic_t *node = slots[i].head;

        if (node == NULL) {
            continue;
        }
        do {
            if (node == targetNode) {
                return DDS3_SCENE_MEMBERSHIP_PRESENT;
            }
            node = node->next;
        } while (node != 0);
    }
    return DDS3_SCENE_MEMBERSHIP_ABSENT;
}

void dds3SetMapNo(dds3Basic_t *worldNode, Dds3AreaRoomKey areaRoomKey) {
    if (worldNode != NULL) {
        ((evtWorldTable_t *)worldNode->workdata)->areaRoomKey = areaRoomKey;
    }
}

Dds3AreaRoomKey dds3GetMapNo(const dds3Basic_t *worldNode) {
    if (worldNode == NULL) {
        return DDS3_AREA_ROOM_KEY_UNASSIGNED;
    }
    return ((evtWorldTable_t *)worldNode->workdata)->areaRoomKey;
}

dds3Basic_t *dds3SceneBasicSearchNameAndType(dds3Basic_t *object, s32 slotIndex, const char *name) {
    dds3Basic_t *node;
    u8 *s;
    const u8 *nameBytes = (const u8 *)name;
    s32 i;

    if (object == NULL || name == NULL) {
        return NULL;
    }
    node = ((evtWorldTable_t *)object->workdata)->slots[slotIndex].head;
    while (node != NULL) {
        s = ((u8 *)node->name);
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
            } while (nameBytes[i] == ((u8 *)node->name)[i]);
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
    object->workdata = indexNodes;
    for (i = 0; i < 2; i++) {
        indexNodes[i] = dds3GetBasicArrayHandle(0);
    }
    return 1;
}

void AreaBasicDestructor(dds3Basic_t *object) {
    WorldIndexNode **indexNodes;
    u32 i;

    indexNodes = object->workdata;
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
    WorldIndexNode **indexNodes = object->workdata;

    dds3ForEachBasicArray((dds3BasicArrayHandle_t *)indexNodes[0], CB_BasicDrawProcuder);
    return 1;
}
