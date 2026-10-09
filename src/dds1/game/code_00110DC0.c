#include "common.h"
#include "sdf_chip.h"
#include "dds3obj.h"
#include "evt_world.h"


s32 dds3ExchangeAreaSlot(void *arg);

extern s32 dds3InvokeAreaCallback(void *arg);



const s32 dds3ContainsNodeInObjectChain(EffWorldNode *object, s32 index, EffWorldNode *value) {
    EffWorldNode *node;

    if (object == NULL || value == 0) {
        return 0;
    }
    node = ((EvtWorldTable *)object->data)->slots[index].head;
    while (node != NULL) {
        if (node == value) {
            return 1;
        }
        node = node->next;
    }
    return 0;
}

const s32 dds3ContainsNodeInAnyObjectChain(EffWorldNode *object, EffWorldNode *value) {
    EvtWorldSlot *slots;
    s32 i;

    if (object == NULL || value == 0) {
        return 0;
    }
    slots = ((EvtWorldTable *)object->data)->slots;
    for (i = 0; i < 0x12; i++) {
        EffWorldNode *node = slots[i].head;

        if (node == NULL) {
            continue;
        }
        do {
            if (node == value) {
                return 1;
            }
            node = node->next;
        } while (node != 0);
    }
    return 0;
}

void dds3SetWorldObjectValue(EffWorldNode *object, u32 value) {
    if (object != NULL) {
        ((EvtWorldTable *)object->data)->areaRoomKey = value;
    }
}

s32 dds3GetWorldObjectValue(EffWorldNode *object) {
    if (object == NULL) {
        return -1;
    }
    return ((EvtWorldTable *)object->data)->areaRoomKey;
}

EffWorldNode *dds3FindIndexedObjectChainNodeByName(EffWorldNode *object, s32 index, const u8 *name) {
    EffWorldNode *node;
    u8 *s;
    s32 i;

    if (object == NULL || name == NULL) {
        return NULL;
    }
    node = ((EvtWorldTable *)object->data)->slots[index].head;
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
        if (name[0] == s[0]) {
            do {
                if (name[i] == '\0') {
                    return node;
                }
                i++;
            } while (name[i] == ((u8 *)node->value)[i]);
        }
        node = node->next;
    }
    return NULL;
}

EffWorldNode *dds3FindObjectChainNodeByName(EffWorldNode *object, const u8 *name) {
    EffWorldNode *node;
    s32 i;

    if (object == NULL || name == NULL) {
        return NULL;
    }
    for (i = 0; i < 0x12; i++) {
        node = dds3FindIndexedObjectChainNodeByName(object, i, name);
        if (node != NULL) {
            return node;
        }
    }
    return NULL;
}

s32 dds3CreatePairedWorldIndexNodes(EffWorldNode *object) {
    WorldIndexNode **indexNodes;
    u32 i;

    indexNodes = sdfAllocSizeClassBlock(8);
    object->data = indexNodes;
    for (i = 0; i < 2; i++) {
        indexNodes[i] = dds3AppendWorldIndexNode(0);
    }
    return 1;
}

void dds3DestroyObjectPointerChains(EffWorldNode *object) {
    WorldIndexNode **indexNodes;
    u32 i;

    indexNodes = object->data;
    dds3VisitWorldObjectValues((WorldValueIndices *)indexNodes[0], (s32 (*)(u32))dds3ExchangeAreaSlot);
    dds3VisitWorldObjectValues((WorldValueIndices *)indexNodes[1], (s32 (*)(u32))dds3ExchangeAreaSlot);
    for (i = 0; i < 2; i++) {
        dds3DestroyWorldIndexNode(indexNodes[i]);
    }
    sdfReleaseChipBlock(indexNodes);
}



u32 func_001110D0(void) {
    return 1;
}

s32 ddsDispatchFirstWorldIndexAreas(EffWorldNode *object) {
    WorldIndexNode **indexNodes = object->data;

    dds3VisitWorldObjectValues((WorldValueIndices *)indexNodes[0], (s32 (*)(u32))dds3InvokeAreaCallback);
    return 1;
}
