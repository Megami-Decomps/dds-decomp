#include "common.h"

u32 func_00116590(void) {
    return 1;
}

u32 func_00116598(s32 arg0) {
    return *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x74);
}

INCLUDE_ASM(const s32, "game/code_00116590", func_001165A8);

void func_001165F0(void) {
    func_00110928();
}

INCLUDE_ASM(const s32, "game/code_00116590", func_00116608);

void func_001166B8(s32 arg0) {
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 100) = *(u32 *)(*(s32 *)(arg0 + 0x18) + 100) & 0xfffffffc
    ;
}

void func_001166D0(u32 arg0, s32 arg1) {
    func_00221BE0(*(u32 *)(*(s32 *)(arg1 + 0x18) + 8), arg0);
}

void func_001166F0(s32 arg0) {
    func_00221C50(*(u32 *)(*(s32 *)(arg0 + 0x18) + 8));
}

INCLUDE_ASM(const s32, "game/code_00116590", func_00116710);

INCLUDE_ASM(const s32, "game/code_00116590", func_001167B8);

INCLUDE_ASM(const s32, "game/code_00116590", func_00116820);
