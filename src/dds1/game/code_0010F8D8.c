#include "common.h"

extern u32 D_003BA9B8;

INCLUDE_ASM(const s32, "game/code_0010F8D8", func_0010F8D8);

INCLUDE_ASM(const s32, "game/code_0010F8D8", func_0010F948);

INCLUDE_ASM(const s32, "game/code_0010F8D8", func_0010F9A8);

INCLUDE_ASM(const s32, "game/code_0010F8D8", func_0010FA00);

INCLUDE_ASM(const s32, "game/code_0010F8D8", func_0010FA40);

void func_0010FA80(void) {
    D_003BA9B8 = (D_003BA9B8 + 1) & 0xffff;
}

void func_0010FA98(s32 arg0, u32 arg1) {
    if (arg0 != 0) {
        *(u32 *)((s32)arg0 + 8) = arg1;
    }
}

u32 func_0010FAA8(s32 arg0) {
    u32 temp_v0;

    temp_v0 = 0;
    if (arg0 != 0) {
        temp_v0 = *(u32 *)((s32)arg0 + 8);
    }
    return temp_v0;
}

INCLUDE_ASM(const s32, "game/code_0010F8D8", func_0010FAC0);

INCLUDE_ASM(const s32, "game/code_0010F8D8", func_0010FB30);

u32 func_0010FBC8(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(*(s32 *)(arg0 + 0x18) + 8);
    if (temp_v0 != 0) {
        func_0010FA00(temp_v0);
    }
    return 1;
}

u32 func_0010FBF8(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(*(s32 *)(arg0 + 0x18) + 8);
    if (temp_v0 != 0) {
        func_0010FA40(temp_v0);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0010F8D8", func_0010FC28);



INCLUDE_SDATA(const s32, "game/code_0010F8D8", D_003BA9B8);


INCLUDE_SDATA(const s32, "game/code_0010F8D8", D_003BA9BC);

