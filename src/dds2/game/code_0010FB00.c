#include "common.h"

extern u32 D_00435D88;

typedef struct {
    u8 pad0[8];
    void *callbackTarget;
} WorldEntry;

typedef struct {
    u8 pad0[0x18];
    WorldEntry *entry;
} WorldObject;

INCLUDE_ASM(const s32, "game/code_0010FB00", func_0010FB00);

INCLUDE_ASM(const s32, "game/code_0010FB00", func_0010FB70);

INCLUDE_ASM(const s32, "game/code_0010FB00", func_0010FBD0);

INCLUDE_ASM(const s32, "game/code_0010FB00", func_0010FC28);

INCLUDE_ASM(const s32, "game/code_0010FB00", func_0010FC68);

void dds3AdvanceWorldCounter(void) {
    D_00435D88 = (D_00435D88 + 1) & 0xffff;
}

void dds3SetWorldEntryCallbackTarget(WorldEntry *entry, void *callbackTarget) {
    if (entry != NULL) {
        entry->callbackTarget = callbackTarget;
    }
}

void *dds3GetWorldEntryCallbackTarget(WorldEntry *entry) {
    void *callbackTarget;

    callbackTarget = NULL;
    if (entry != NULL) {
        callbackTarget = entry->callbackTarget;
    }
    return callbackTarget;
}

INCLUDE_ASM(const s32, "game/code_0010FB00", func_0010FCE8);

INCLUDE_ASM(const s32, "game/code_0010FB00", func_0010FD58);

u32 func_0010FDF0(WorldObject *obj) {
    void *callbackTarget;

    callbackTarget = obj->entry->callbackTarget;
    if (callbackTarget != NULL) {
        func_0010FC28(callbackTarget);
    }
    return 1;
}

u32 func_0010FE20(WorldObject *obj) {
    void *callbackTarget;

    callbackTarget = obj->entry->callbackTarget;
    if (callbackTarget != NULL) {
        func_0010FC68(callbackTarget);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0010FB00", func_0010FE50);

INCLUDE_SDATA(const s32, "game/code_0010FB00", D_00435D88);

INCLUDE_SDATA(const s32, "game/code_0010FB00", D_00435D8C);

