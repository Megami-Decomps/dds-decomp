#include "common.h"

u32 func_001167F8(void) {
    return 1;
}

u32 func_00116800(s32 arg0) {
    return *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x74);
}

INCLUDE_ASM(const s32, "game/code_001167F8", func_00116810);

INCLUDE_ASM(const s32, "game/code_001167F8", func_00116858);

INCLUDE_ASM(const s32, "game/code_001167F8", func_00116870);

void func_00116920(s32 arg0) {
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 100) = *(u32 *)(*(s32 *)(arg0 + 0x18) + 100) & 0xfffffffc
    ;
}

INCLUDE_ASM(const s32, "game/code_001167F8", func_00116938);

INCLUDE_ASM(const s32, "game/code_001167F8", func_00116958);

INCLUDE_ASM(const s32, "game/code_001167F8", func_00116978);

INCLUDE_ASM(const s32, "game/code_001167F8", func_00116A20);

INCLUDE_ASM(const s32, "game/code_001167F8", func_00116A88);
