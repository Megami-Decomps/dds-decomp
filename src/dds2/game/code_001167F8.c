#include "common.h"

u32 func_001167F8(void) {
    return 1;
}

u32 func_00116800(s32 arg0) {
    return *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x74);
}

INCLUDE_ASM(const s32, "game/code_001167F8", func_00116810);

void func_00116858(void) {
    func_00110B50();
}

INCLUDE_ASM(const s32, "game/code_001167F8", func_00116870);

void dds3ClearUnitObjectLowFlags(s32 arg0) {
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 100) = *(u32 *)(*(s32 *)(arg0 + 0x18) + 100) & 0xfffffffc
    ;
}

void func_00116938(u32 arg0, s32 arg1) {
    func_0023C750(*(u32 *)(*(s32 *)(arg1 + 0x18) + 8), arg0);
}

void func_00116958(s32 arg0) {
    func_0023C7C0(*(u32 *)(*(s32 *)(arg0 + 0x18) + 8));
}

INCLUDE_ASM(const s32, "game/code_001167F8", func_00116978);

INCLUDE_ASM(const s32, "game/code_001167F8", func_00116A20);

INCLUDE_ASM(const s32, "game/code_001167F8", func_00116A88);
