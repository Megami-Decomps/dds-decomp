#include "common.h"

typedef struct WorldObjectPointer {
    u8 pad00[0x18];
    u32 *value;
} WorldObjectPointer;

/* One entry of the chain table value[2] points at; the walk follows next. */
typedef struct WorldChainNode {
    u8 pad00[8];
    u8 *name; /* 0x08 */
    u8 pad0C[0x14];
    struct WorldChainNode *next; /* 0x20 */
} WorldChainNode;

typedef struct WorldChainEntry {
    u8 pad00[4];
    WorldChainNode *node; /* 0x04 */
    u8 pad08[4];
} WorldChainEntry; /* 0x0C */

s32 dds3InvokeAreaCallback(void *arg);

s32 dds3VisitWorldObjectValues(s32 object, s32 (*callback)(void *));

s32 dds3ContainsNodeInObjectChain(WorldObjectPointer *object, s32 index, s32 value) {
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

s32 dds3ContainsNodeInAnyObjectChain(WorldObjectPointer *object, s32 value) {
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
            if (node == (u32 *)value) {
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

WorldChainNode *func_001110F8(WorldObjectPointer *object, s32 index, const u8 *name) {
    WorldChainNode *node;
    u8 *s;
    s32 i;

    if (object == NULL || name == NULL) {
        return NULL;
    }
    node = ((WorldChainEntry *)object->value[2])[index].node;
    while (node != NULL) {
        s = node->name;
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
            } while (name[i] == node->name[i]);
        }
        node = node->next;
    }
    return NULL;
}

u32 *dds3FindObjectChainNodeByName(WorldObjectPointer *object, const u8 *name) {
    u32 *node;
    s32 i;

    if (object == NULL || name == NULL) {
        return NULL;
    }
    for (i = 0; i < 0x12; i++) {
        node = func_001110F8(object, i, name);
        if (node != NULL) {
            return node;
        }
    }
    return NULL;
}

void *func_00328D68(s32 size);

void *dds3AppendWorldIndexNode(s32 index);

s32 func_00111218(WorldObjectPointer *object) {
    u32 *p;
    u32 i;

    object->value = func_00328D68(8);
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

s32 func_00111300(WorldObjectPointer *object) {
    dds3VisitWorldObjectValues(*object->value, dds3InvokeAreaCallback);
    return 1;
}
