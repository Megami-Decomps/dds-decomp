#include "common.h"
#include "evt_world.h"

struct NodeB;

s32 dds3InvokeAreaCallback(void *arg);

s32 dds3VisitWorldObjectValues(WorldValueIndices *object, s32 (*callback)(u32));

s32 dds3ContainsNodeInObjectChain(EffWorldNode *object, s32 index, s32 value) {
    EffWorldNode *node;

    if (object == NULL || value == 0) {
        return 0;
    }
    node = ((EvtWorldTable *)object->data)->slots[index].head;
    while (node != NULL) {
        if (node == (EffWorldNode *)value) {
            return 1;
        }
        node = node->next;
    }
    return 0;
}

s32 dds3ContainsNodeInAnyObjectChain(EffWorldNode *object, EffWorldNode *value) {
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
        *(u32 *)object->data = value;
    }
}

s32 dds3GetWorldObjectValue(EffWorldNode *object) {
    if (object == NULL) {
        return -1;
    }
    return *(u32 *)object->data;
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

void *sdfAllocSizeClassBlock(s32 size);

struct NodeB *dds3AppendWorldIndexNode(s32 initialCount);

s32 dds3CreatePairedWorldIndexNodes(EffWorldNode *object) {
    u32 *p;
    u32 i;

    object->data = sdfAllocSizeClassBlock(8);
    p = (u32 *)object->data;
    for (i = 0; i < 2; i++) {
        *p = (u32)dds3AppendWorldIndexNode(0);
        p++;
    }
    return 1;
}

s32 dds3ExchangeAreaSlot(void *arg);

void dds3DestroyWorldIndexNode(struct NodeB *node);

void sdfReleaseChipBlock(void *arg);

void dds3DestroyObjectPointerChains(EffWorldNode *object) {
    u32 *p;
    u32 i;

    p = (u32 *)object->data;
    dds3VisitWorldObjectValues((WorldValueIndices *)(u32)p[0], (s32 (*)(u32))dds3ExchangeAreaSlot);
    dds3VisitWorldObjectValues((WorldValueIndices *)(u32)p[1], (s32 (*)(u32))dds3ExchangeAreaSlot);
    for (i = 0; i < 2; i++) {
        dds3DestroyWorldIndexNode((struct NodeB *)p[i]);
    }
    sdfReleaseChipBlock(p);
}

u32 func_001112F8(void) {
    return 1;
}

s32 ddsDispatchFirstWorldIndexAreas(EffWorldNode *object) {
    dds3VisitWorldObjectValues((WorldValueIndices *)(u32)*(u32 *)object->data, (s32 (*)(u32))dds3InvokeAreaCallback);
    return 1;
}
