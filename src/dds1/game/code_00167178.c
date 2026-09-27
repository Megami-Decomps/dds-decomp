#include "common.h"

extern u64 func_00163258(u64, u64);

INCLUDE_ASM(const s32, "game/code_00167178", func_00167178);

INCLUDE_ASM(const s32, "game/code_00167178", func_00167350);

u32 func_001673C0(u32 arg0) {
    return arg0;
}

void func_001673C8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x120) = arg1;
}

INCLUDE_ASM(const s32, "game/code_00167178", func_001673D0);

INCLUDE_ASM(const s32, "game/code_00167178", func_00167A78);

INCLUDE_ASM(const s32, "game/code_00167178", func_00167BF8);

void func_00167E78(u64 arg0) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_00163258(arg0, 0);
    temp_v1 = func_00163258(arg0, 1);
    func_00167BF8(temp_v0, temp_v1);
}

INCLUDE_ASM(const s32, "game/code_00167178", func_00167EC0);

INCLUDE_ASM(const s32, "game/code_00167178", func_00168138);

INCLUDE_ASM(const s32, "game/code_00167178", func_001681C0);

INCLUDE_ASM(const s32, "game/code_00167178", func_00169928);

void func_00169938(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x54) = arg1;
}

INCLUDE_ASM(const s32, "game/code_00167178", func_00169940);

void func_00169B40(s32 arg0) {
    func_002DAA68(*(u32 *)(arg0 + 0x20));
    func_002D0918(*(u32 *)(arg0 + 0x24));
}

void func_00169B70(u32 *arg0) {
    arg0[4] = 3;
    *arg0 = 0x80808080;
    arg0[3] = 0;
}

INCLUDE_ASM(const s32, "game/code_00167178", func_00169B90);

INCLUDE_ASM(const s32, "game/code_00167178", func_00169D78);

INCLUDE_ASM(const s32, "game/code_00167178", func_00169E10);

INCLUDE_ASM(const s32, "game/code_00167178", func_0016A088);
