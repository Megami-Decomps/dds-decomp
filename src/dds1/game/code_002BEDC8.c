#include "common.h"

extern s32 effGetSlotWorkOrOverride(s32, s32);

INCLUDE_ASM(const s32, "game/code_002BEDC8", func_002BEDC8);


INCLUDE_ASM(const s32, "game/code_002BEDC8", func_002BEEA0);

INCLUDE_ASM(const s32, "game/code_002BEDC8", func_002BF198);
void func_002BF400(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8) {
    func_002BF198(a0, a1, a2, a6 + 0x14, a3, a4, a5, a6, a7);
}
void func_002BF438(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7) {
    func_002BF198(a0, a1, a2, a3, a4, a5, a6,
                  effGetSlotWorkOrOverride(a5, a6), a7);
}



INCLUDE_ASM(const s32, "game/code_002BEDC8", func_002BF4E0);

INCLUDE_ASM(const s32, "game/code_002BEDC8", func_002BF5D8);
