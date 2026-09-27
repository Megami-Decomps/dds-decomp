#include "common.h"

extern u64 func_002D0A80(void);

INCLUDE_ASM(const s32, "game/code_002D0750", func_002D0750);

INCLUDE_ASM(const s32, "game/code_002D0750", func_002D0900);

INCLUDE_ASM(const s32, "game/code_002D0750", func_002D0918);

void func_002D09B8(void) {
    u64 temp_v0;

    temp_v0 = func_002D0A80();
    func_002D0918(temp_v0);
}

void func_002D09D8(s32 *arg0) {
    s32 temp_v0;

    temp_v0 = *arg0;
    if (temp_v0 != 0) {
        *arg0 = 0;
        func_002D0918(temp_v0);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_002D0750", func_002D0A10);

u32 func_002D0A48(s32 arg0) {
    *(s16 *)(arg0 + 0xe) = *(s16 *)(arg0 + 0xe) + 1;
    return *(u32 *)(arg0 + 8);
}

INCLUDE_ASM(const s32, "game/code_002D0750", func_002D0A60);

INCLUDE_ASM(const s32, "game/code_002D0750", func_002D0A80);

INCLUDE_ASM(const s32, "game/code_002D0750", func_002D0B50);

INCLUDE_ASM(const s32, "game/code_002D0750", func_002D0BF8);

INCLUDE_ASM(const s32, "game/code_002D0750", func_002D0C68);

INCLUDE_ASM(const s32, "game/code_002D0750", func_002D0D70);

INCLUDE_ASM(const s32, "game/code_002D0750", func_002D0E30);
