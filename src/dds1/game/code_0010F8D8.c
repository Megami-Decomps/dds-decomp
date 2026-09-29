#include "common.h"

typedef struct {
    u8 pad0[8];
    u32 callbackTarget; /* 0x08: forwarded to both lifecycle helpers */
} WorldEntry;

typedef struct {
    u8 pad0[0x18];
    WorldEntry *entry;
} WorldObject;

extern u32 D_003BA9B8;

INCLUDE_ASM(const s32, "game/code_0010F8D8", func_0010F8D8);

INCLUDE_ASM(const s32, "game/code_0010F8D8", func_0010F948);

INCLUDE_ASM(const s32, "game/code_0010F8D8", func_0010F9A8);

INCLUDE_ASM(const s32, "game/code_0010F8D8", func_0010FA00);

INCLUDE_ASM(const s32, "game/code_0010F8D8", func_0010FA40);

/* The stored sequence wraps at 16 bits even though its backing word is 32 bits. */
void dds3AdvanceWorldCounter(void) {
    D_003BA9B8 = (D_003BA9B8 + 1) & 0xffff;
}

void dds3SetWorldEntryCallbackTarget(WorldEntry *entry, u32 callbackTarget) {
    if (entry != NULL) {
        entry->callbackTarget = callbackTarget;
    }
}

u32 dds3GetWorldEntryCallbackTarget(WorldEntry *entry) {
    u32 callbackTarget;

    callbackTarget = 0;
    if (entry != NULL) {
        callbackTarget = entry->callbackTarget;
    }
    return callbackTarget;
}

INCLUDE_ASM(const s32, "game/code_0010F8D8", func_0010FAC0);

INCLUDE_ASM(const s32, "game/code_0010F8D8", func_0010FB30);

u32 func_0010FBC8(WorldObject *obj) {
    s32 callbackTarget;

    callbackTarget = obj->entry->callbackTarget;
    if (callbackTarget != 0) {
        func_0010FA00(callbackTarget);
    }
    return 1;
}

u32 func_0010FBF8(WorldObject *obj) {
    s32 callbackTarget;

    callbackTarget = obj->entry->callbackTarget;
    if (callbackTarget != 0) {
        func_0010FA40(callbackTarget);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0010F8D8", func_0010FC28);

INCLUDE_SDATA(const s32, "game/code_0010F8D8", D_003BA9B8);

INCLUDE_SDATA(const s32, "game/code_0010F8D8", D_003BA9BC);

