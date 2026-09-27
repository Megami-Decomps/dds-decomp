#include "common.h"

void func_00176A28(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x60) = arg1;
}

INCLUDE_ASM(const s32, "game/code_00176A28", func_00176A30);

void func_00176B50(u32 arg0) {
    func_002DAA68(*(u32 *)((s32)arg0 + 0x6c));
    func_00177048(arg0);
    func_002D0918(*(u32 *)((s32)arg0 + 0x70));
}

INCLUDE_ASM(const s32, "game/code_00176A28", func_00176B88);

INCLUDE_ASM(const s32, "game/code_00176A28", func_00176E20);

void func_00177048(s32 arg0) {
    if (*(s32 *)(arg0 + 0x68) != 0) {
        func_002D0918(*(s32 *)(arg0 + 0x68));
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_00176A28", func_00177078);

INCLUDE_ASM(const s32, "game/code_00176A28", func_001770A8);

void func_001770D8(s32 arg0, s32 arg1, u32 arg2) {
    *(u32 *)(arg1 * 0x14 + *(s32 *)(arg0 + 0x40) + 0x10) = arg2;
}

INCLUDE_ASM(const s32, "game/code_00176A28", func_001770F8);

INCLUDE_ASM(const s32, "game/code_00176A28", func_00177120);
