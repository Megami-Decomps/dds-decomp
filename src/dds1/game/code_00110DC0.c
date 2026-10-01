#include "common.h"

/* The object owns a pointer to its current world value, not the value itself. */
typedef struct WorldObjectPointer {
    u8 pad00[0x18];
    u32 *value;
} WorldObjectPointer;

/* One entry of the chain table value[2] points at; the walk follows next. */
typedef struct WorldChainNode {
    u8 pad00[0x20];
    struct WorldChainNode *next; /* 0x20 */
} WorldChainNode;

typedef struct WorldChainEntry {
    u8 pad00[4];
    WorldChainNode *node; /* 0x04 */
    u8 pad08[4];
} WorldChainEntry; /* 0x0C */

s32 dds3VisitWorldObjectValues(s32 object, s32 (*callback)(void *));

s32 dds3ExchangeAreaSlot(void *arg);

extern s32 dds3InvokeAreaCallback(void *arg);

void dds3DestroyWorldIndexNode(u32 node);

void sdfReleaseChipBlock(void *arg);

const s32 dds3ContainsNodeInObjectChain(WorldObjectPointer *object, s32 index, s32 value) {
    WorldChainNode *node;

    if (object == NULL || value == 0) {
        return 0;
    }
    node = ((WorldChainEntry *)object->value[2])[index].node;
    while (node != NULL) {
        if ((u32 *)node == (u32 *)value) {
            return 1;
        }
        node = node->next;
    }
    return 0;
}

const s32 dds3ContainsNodeInAnyObjectChain(WorldObjectPointer *object, s32 value) {
    u8 *p;
    s32 i;

    if (object == NULL || value == 0) {
        return 0;
    }
    p = (u8 *)object->value[2] + 4;
    for (i = 0; i < 0x12; i++, p += 0xC) {
        WorldChainNode *node = *(WorldChainNode **)p;

        if (node == NULL) {
            continue;
        }
        do {
            if ((u32 *)node == (u32 *)value) {
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

INCLUDE_ASM(const s32, "game/code_00110DC0", func_00110ED0);

u32 *dds3FindObjectChainNodeByName(WorldObjectPointer *object, const u8 *name) {
    u32 *node;
    s32 i;

    if (object == NULL || name == NULL) {
        return NULL;
    }
    for (i = 0; i < 0x12; i++) {
        node = func_00110ED0(object, i, name);
        if (node != NULL) {
            return node;
        }
    }
    return NULL;
}

s32 func_00110FF0(WorldObjectPointer *object) {
    u32 *p;
    u32 i;

    object->value = func_002CFEB8(8);
    p = object->value;
    for (i = 0; i < 2; i++) {
        *p = (u32)dds3AppendWorldIndexNode(0);
        p++;
    }
    return 1;
}

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

extern void *func_002CFEB8(s32 size);

extern void *dds3AppendWorldIndexNode(s32 index);

u32 func_001110D0(void) {
    return 1;
}

s32 func_001110D8(WorldObjectPointer *object) {
    dds3VisitWorldObjectValues(*object->value, dds3InvokeAreaCallback);
    return 1;
}
