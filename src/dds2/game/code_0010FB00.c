#include "common.h"

extern u32 D_00435D88;

typedef struct {
    u8 pad0[8];
    u32 value;
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

void func_0010FCA8(void) {
    D_00435D88 = (D_00435D88 + 1) & 0xffff;
}

void func_0010FCC0(WorldEntry *entry, u32 value) {
    if (entry != NULL) {
        entry->value = value;
    }
}

u32 func_0010FCD0(WorldEntry *entry) {
    u32 value;

    value = 0;
    if (entry != NULL) {
        value = entry->value;
    }
    return value;
}

INCLUDE_ASM(const s32, "game/code_0010FB00", func_0010FCE8);

INCLUDE_ASM(const s32, "game/code_0010FB00", func_0010FD58);

u32 func_0010FDF0(WorldObject *obj) {
    s32 value;

    value = obj->entry->value;
    if (value != 0) {
        func_0010FC28(value);
    }
    return 1;
}

u32 func_0010FE20(WorldObject *obj) {
    s32 value;

    value = obj->entry->value;
    if (value != 0) {
        func_0010FC68(value);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0010FB00", func_0010FE50);

INCLUDE_SDATA(const s32, "game/code_0010FB00", D_00435D88);

INCLUDE_SDATA(const s32, "game/code_0010FB00", D_00435D8C);

