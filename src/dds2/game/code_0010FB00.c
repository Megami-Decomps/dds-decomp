#include "common.h"

extern u32 D_00435D88;

typedef struct WorldValue {
    u8 pad00[8];
    u32 value;
} WorldValue;

INCLUDE_ASM(const s32, "game/code_0010FB00", func_0010FB00);

INCLUDE_ASM(const s32, "game/code_0010FB00", func_0010FB70);

INCLUDE_ASM(const s32, "game/code_0010FB00", func_0010FBD0);

INCLUDE_ASM(const s32, "game/code_0010FB00", func_0010FC28);

INCLUDE_ASM(const s32, "game/code_0010FB00", func_0010FC68);

void func_0010FCA8(void) {
    D_00435D88 = (D_00435D88 + 1) & 0xffff;
}

void func_0010FCC0(WorldValue *record, u32 value) {
    if (record != NULL) {
        record->value = value;
    }
}

u32 func_0010FCD0(WorldValue *record) {
    u32 value;

    value = 0;
    if (record != NULL) {
        value = record->value;
    }
    return value;
}

INCLUDE_ASM(const s32, "game/code_0010FB00", func_0010FCE8);

INCLUDE_ASM(const s32, "game/code_0010FB00", func_0010FD58);

u32 func_0010FDF0(s32 object) {
    s32 child;

    child = *(s32 *)(*(s32 *)(object + 0x18) + 8);
    if (child != 0) {
        func_0010FC28(child);
    }
    return 1;
}

u32 func_0010FE20(s32 object) {
    s32 child;

    child = *(s32 *)(*(s32 *)(object + 0x18) + 8);
    if (child != 0) {
        func_0010FC68(child);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0010FB00", func_0010FE50);

INCLUDE_SDATA(const s32, "game/code_0010FB00", D_00435D88);

INCLUDE_SDATA(const s32, "game/code_0010FB00", D_00435D8C);

