#include "common.h"
#include "evt_world.h"

/* Scalar and paired-index payloads are separate from the world's list table. */
typedef struct WorldObjectPointer {
    u8 pad00[0x18];
    u32 *value;
} WorldObjectPointer;



s32 dds3InvokeAreaCallback(void *arg);

s32 dds3VisitWorldObjectValues(s32 object, s32 (*callback)(void *));

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

s32 dds3ContainsNodeInAnyObjectChain(EffWorldNode *object, s32 value) {
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
            if (node == (EffWorldNode *)value) {
                return 1;
            }
            node = node->next;
        } while (node != 0);
    }
    return 0;
}

void dds3SetWorldObjectValue(WorldObjectPointer *object, u32 value) {
    if (object != NULL) {
        *object->value = value;
    }
}

s32 dds3GetWorldObjectValue(WorldObjectPointer *object) {
    if (object == NULL) {
        return -1;
    }
    return *object->value;
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

u32 *dds3FindObjectChainNodeByName(EffWorldNode *object, const u8 *name) {
    u32 *node;
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

void *dds3AppendWorldIndexNode(s32 index);

s32 dds3CreatePairedWorldIndexNodes(WorldObjectPointer *object) {
    u32 *p;
    u32 i;

    object->value = sdfAllocSizeClassBlock(8);
    p = object->value;
    for (i = 0; i < 2; i++) {
        *p = (u32)dds3AppendWorldIndexNode(0);
        p++;
    }
    return 1;
}

s32 dds3ExchangeAreaSlot(void *arg);

void dds3DestroyWorldIndexNode(u32 node);

void sdfReleaseChipBlock(void *arg);

void dds3DestroyObjectPointerChains(WorldObjectPointer *object) {
    u32 *p;
    u32 i;

    p = object->value;
    dds3VisitWorldObjectValues(p[0], dds3ExchangeAreaSlot);
    dds3VisitWorldObjectValues(p[1], dds3ExchangeAreaSlot);
    for (i = 0; i < 2; i++) {
        dds3DestroyWorldIndexNode(p[i]);
    }
    sdfReleaseChipBlock(p);
}

u32 func_001112F8(void) {
    return 1;
}

s32 ddsDispatchFirstWorldIndexAreas(WorldObjectPointer *object) {
    dds3VisitWorldObjectValues(*object->value, dds3InvokeAreaCallback);
    return 1;
}
