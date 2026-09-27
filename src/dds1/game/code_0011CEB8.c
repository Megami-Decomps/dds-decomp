#include "common.h"

extern u64 func_0010D428(u64);
extern u64 func_0011B140(u64, u64);

INCLUDE_ASM(const s32, "game/code_0011CEB8", func_0011CEB8);

u32 func_0011CEF0(void) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0010D428(0);
    temp_v1 = func_0010D428(1);
    temp_v0 = func_0011B140(temp_v0, temp_v1);
    func_0010D5F0(temp_v0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0011CEB8", func_0011CF38);

INCLUDE_ASM(const s32, "game/code_0011CEB8", func_0011CF88);

void func_0011CFC0(s32 *arg0, s32 arg1, s32 arg2) {
    s32 temp_v0;

    temp_v0 = arg0[1];
    if (temp_v0 == 0) {
        *arg0 = arg1;
    }
    else {
        *(s32 *)(temp_v0 + arg2 + 4) = arg1;
    }
    *(s32 *)(arg1 + arg2) = temp_v0;
    ((s32 *)(arg1 + arg2))[1] = 0;
    arg0[1] = arg1;
}

INCLUDE_ASM(const s32, "game/code_0011CEB8", func_0011CFF0);

INCLUDE_ASM(const s32, "game/code_0011CEB8", func_0011D030);

INCLUDE_ASM(const s32, "game/code_0011CEB8", func_0011D070);

INCLUDE_ASM(const s32, "game/code_0011CEB8", func_0011D0B0);

void func_0011D0E0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 8) = arg1;
}

INCLUDE_ASM(const s32, "game/code_0011CEB8", func_0011D0E8);

INCLUDE_ASM(const s32, "game/code_0011CEB8", func_0011D138);

void func_0011D178(u32 arg0) {
    func_00194920(*(u32 *)((s32)arg0 + 0x10));
    func_002CFF98(arg0);
}

void func_0011D1A8(s32 arg0) {
    func_00195868(*(u32 *)(arg0 + 0x10));
}

INCLUDE_ASM(const s32, "game/code_0011CEB8", func_0011D1C0);

void func_0011D258(s32 arg0, u8 arg1) {
    func_00195470(*(u32 *)(arg0 + 0x10), arg1);
}

void func_0011D278(void) {
    func_002CFF98();
}
