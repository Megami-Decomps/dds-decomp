#include "common.h"

extern u32 D_00435D88;

INCLUDE_ASM(const s32, "game/code_0010FB00", func_0010FB00);

INCLUDE_ASM(const s32, "game/code_0010FB00", func_0010FB70);

INCLUDE_ASM(const s32, "game/code_0010FB00", func_0010FBD0);

INCLUDE_ASM(const s32, "game/code_0010FB00", func_0010FC28);

INCLUDE_ASM(const s32, "game/code_0010FB00", func_0010FC68);

void func_0010FCA8(void) {
    D_00435D88 = (D_00435D88 + 1) & 0xffff;
}

void func_0010FCC0(s32 arg0, u32 arg1) {
    if (arg0 != 0) {
        *(u32 *)((s32)arg0 + 8) = arg1;
    }
}

u32 func_0010FCD0(s32 arg0) {
    u32 temp_v0;

    temp_v0 = 0;
    if (arg0 != 0) {
        temp_v0 = *(u32 *)((s32)arg0 + 8);
    }
    return temp_v0;
}

INCLUDE_ASM(const s32, "game/code_0010FB00", func_0010FCE8);

INCLUDE_ASM(const s32, "game/code_0010FB00", func_0010FD58);

u32 func_0010FDF0(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(*(s32 *)(arg0 + 0x18) + 8);
    if (temp_v0 != 0) {
        func_0010FC28(temp_v0);
    }
    return 1;
}

u32 func_0010FE20(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(*(s32 *)(arg0 + 0x18) + 8);
    if (temp_v0 != 0) {
        func_0010FC68(temp_v0);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0010FB00", func_0010FE50);


INCLUDE_SDATA(const s32, "game/code_0010FB00", D_00435D88);


INCLUDE_SDATA(const s32, "game/code_0010FB00", D_00435D8C);

