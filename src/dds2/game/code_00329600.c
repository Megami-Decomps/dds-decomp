#include "common.h"

extern u64 func_00329930(void);

INCLUDE_ASM(const s32, "game/code_00329600", func_00329600);

INCLUDE_ASM(const s32, "game/code_00329600", func_003297B0);

INCLUDE_ASM(const s32, "game/code_00329600", func_003297C8);

void func_00329868(void) {
    u64 temp_v0;

    temp_v0 = func_00329930();
    func_003297C8(temp_v0);
}

void sdfReleaseMemorySlot(s32 *arg0) {
    s32 temp_v0;

    temp_v0 = *arg0;
    if (temp_v0 != 0) {
        *arg0 = 0;
        func_003297C8(temp_v0);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_00329600", func_003298C0);

u32 sdfResourceRetainAddress(s32 arg0) {
    *(s16 *)(arg0 + 0xe) = *(s16 *)(arg0 + 0xe) + 1;
    return *(u32 *)(arg0 + 8);
}

INCLUDE_ASM(const s32, "game/code_00329600", func_00329910);

INCLUDE_ASM(const s32, "game/code_00329600", func_00329930);

INCLUDE_ASM(const s32, "game/code_00329600", func_00329A00);

INCLUDE_ASM(const s32, "game/code_00329600", func_00329AA8);

INCLUDE_ASM(const s32, "game/code_00329600", func_00329B18);

INCLUDE_ASM(const s32, "game/code_00329600", func_00329C20);

INCLUDE_ASM(const s32, "game/code_00329600", func_00329CE0);
