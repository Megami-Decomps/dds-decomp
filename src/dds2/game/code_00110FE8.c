#include "common.h"

typedef struct WorldObjectPointer {
    u8 pad00[0x18];
    u32 *value;
} WorldObjectPointer;

s32 dds3InvokeAreaCallback(void *arg);

s32 func_00110720(s32 object, s32 (*callback)(void *));

s32 func_00110FE8(WorldObjectPointer *object, s32 index, s32 value) {
    u32 *node;

    if (object == NULL || value == 0) {
        return 0;
    }
    node = *(u32 **)((u8 *)object->value[2] + index * 12 + 4);
    while (node != NULL) {
        if (node == (u32 *)value) {
            return 1;
        }
        node = *(u32 **)((u8 *)node + 0x20);
    }
    return 0;
}

s32 func_00111050(WorldObjectPointer *object, s32 value) {
    u8 *p;
    s32 i;

    if (object == NULL || value == 0) {
        return 0;
    }
    p = (u8 *)object->value[2] + 4;
    for (i = 0; i < 0x12; i++, p += 0xC) {
        u32 *node = *(u32 **)p;

        if (node == NULL) {
            continue;
        }
        do {
            if (node == (u32 *)value) {
                return 1;
            }
            node = *(u32 **)((u8 *)node + 0x20);
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

INCLUDE_ASM(const s32, "game/code_00110FE8", func_001110F8);

u32 *func_001111A8(WorldObjectPointer *object, const u8 *name) {
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

void func_00328E48(void *arg);

void func_00111278(WorldObjectPointer *object) {
    u32 *p;
    u32 i;

    p = object->value;
    func_00110720(p[0], dds3ExchangeAreaSlot);
    func_00110720(p[1], dds3ExchangeAreaSlot);
    for (i = 0; i < 2; i++) {
        dds3DestroyWorldIndexNode(p[i]);
    }
    func_00328E48(p);
}

u32 func_001112F8(void) {
    return 1;
}

s32 func_00111300(WorldObjectPointer *object) {
    func_00110720(*object->value, dds3InvokeAreaCallback);
    return 1;
}
