#include "common.h"
#include "pcp_vu0.h"

extern u32 D_00435D88;

extern void dds3BuildVuTransformFromComponents(u8 *, u8 *, u8 *);

typedef struct {
    u8 pad0[8];
    void *callbackTarget; /* 0x08: forwarded to both lifecycle helpers */
} WorldEntry;

typedef struct {
    u8 pad0[0x18];
    WorldEntry *entry;
} WorldObject;

void dds3LoadOrBuildObjectMatrix(u8 *arg0) {
    u8 *obj = *(u8 **)(arg0 + 0x1C);
    u32 flags = *(u32 *)(obj + 0xC0);

    if (flags & 2) {
        VU0_LOAD_MATRIX(obj);
    } else {
        *(u32 *)(obj + 0xC0) = flags | 2;
        dds3BuildVuTransformFromComponents(obj + 0x60, obj + 0x50, obj + 0x40);
        VU0_STORE_MATRIX(obj);
    }
}

INCLUDE_ASM(const s32, "game/code_0010FB00", func_0010FB70);

INCLUDE_ASM(const s32, "game/code_0010FB00", func_0010FBD0);

INCLUDE_ASM(const s32, "game/code_0010FB00", func_0010FC28);

INCLUDE_ASM(const s32, "game/code_0010FB00", func_0010FC68);

/* The stored sequence wraps at 16 bits even though its backing word is 32 bits. */
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

