#include "common.h"

extern u32 effPcpScatterResCreate(u32);

extern u32 effPcpScatterResAddRef(u32);

INCLUDE_ASM(const s32, "game/code_0017D758", func_0017D758);

INCLUDE_ASM(const s32, "game/code_0017D758", func_0017D770);

void func_0017D778(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x130) = arg1;
}

INCLUDE_ASM(const s32, "game/code_0017D758", func_0017D780);

INCLUDE_ASM(const s32, "game/code_0017D758", func_0017D7A8);

void func_0017D9E0(u32 arg0) {
    s32 temp_v0;

    temp_v0 = (s32)arg0;
    if (*(s32 *)(temp_v0 + 0x7c) != 0) {
        effPcpScatterResRelease(*(s32 *)(temp_v0 + 0x7c));
    }
    func_00333918(*(u32 *)(temp_v0 + 0x74));
    func_003297C8(*(u32 *)(temp_v0 + 0x78));
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "game/code_0017D758", func_0017DA28);

void func_0017DC78(s32 arg0, u32 arg1) {
    u32 temp_v0;

    temp_v0 = effPcpScatterResCreate(arg1);
    *(u32 *)(arg0 + 0x7c) = temp_v0;
}

void func_0017DCA8(s32 arg0, s32 arg1) {
    u32 temp_v0;

    temp_v0 = effPcpScatterResAddRef(*(u32 *)(arg1 + 0x7c));
    *(u32 *)(arg0 + 0x7c) = temp_v0;
}

s32 func_0017DCD8(s32 arg0, s32 arg1) {
    return *(s32 *)(arg0 + 100) + arg1 * *(s32 *)(arg0 + 0x5c) * 0x10;
}

s32 func_0017DCF0(s32 arg0, s32 arg1) {
    return *(s32 *)(arg0 + 0x68) + arg1 * *(s32 *)(arg0 + 0x5c) * 8;
}

u32 func_0017DD08(s32 arg0, s32 arg1) {
    return *(u32 *)(arg1 * 4 + *(s32 *)(arg0 + 0x70));
}

INCLUDE_ASM(const s32, "game/code_0017D758", func_0017DD20);

INCLUDE_ASM(const s32, "game/code_0017D758", func_0017DD50);
