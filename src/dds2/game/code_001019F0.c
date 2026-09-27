#include "common.h"

extern u32 D_00435D8C;

extern s32 func_00102790(void);

INCLUDE_ASM(const s32, "game/code_001019F0", func_001019F0);

INCLUDE_ASM(const s32, "game/code_001019F0", func_00101A60);

INCLUDE_ASM(const s32, "game/code_001019F0", func_00101A90);

INCLUDE_ASM(const s32, "game/code_001019F0", func_00101AC0);

INCLUDE_ASM(const s32, "game/code_001019F0", func_00101C50);

INCLUDE_ASM(const s32, "game/code_001019F0", func_00101D30);

INCLUDE_ASM(const s32, "game/code_001019F0", func_001023C8);

u32 func_00102740(void) {
    func_0010FC28(D_00435D8C);
    return 0;
}

u32 func_00102768(void) {
    func_0010FC68(D_00435D8C);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001019F0", func_00102790);

u32 func_001027B8(void) {
    s32 temp_v0;

    temp_v0 = func_00102790();
    return *(u32 *)(temp_v0 + 4);
}

INCLUDE_ASM(const s32, "game/code_001019F0", func_001027D8);

INCLUDE_RODATA(const s32, "game/code_001019F0", D_00411198);
