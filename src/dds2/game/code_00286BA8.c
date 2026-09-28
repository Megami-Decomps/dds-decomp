#include "common.h"

extern u64 func_00312810(u32, u64);

extern u32 D_00437924;

extern u8 D_00426060[];

extern void func_00286F18(s32, s32);

extern u8 D_003CFCC0[];

INCLUDE_ASM(const s32, "game/code_00286BA8", func_00286BA8);

INCLUDE_ASM(const s32, "game/code_00286BA8", func_00286E20);

INCLUDE_ASM(const s32, "game/code_00286BA8", func_00286E98);

INCLUDE_ASM(const s32, "game/code_00286BA8", func_00286F18);

void func_00286F90(void) {
    s32 data = func_00286E98();
    D_00437924 = func_00312620(D_00426060, 0x402, 0x2B12, D_003CFCC0, func_00286F18, data);
}

s32 func_00286FD8(void) {
    if (func_00312738(D_00426060) != 0) {
        return 1;
    }
    D_00437924 = 0;
    return 0;
}

void func_00287008(void) {
    func_003126D0(D_00437924);
    D_00437924 = 0;
}

INCLUDE_ASM(const s32, "game/code_00286BA8", func_00287030);

INCLUDE_RODATA(const s32, "game/code_00286BA8", D_00426060);

INCLUDE_ASM(const s32, "game/code_00286BA8", func_00287078);

INCLUDE_ASM(const s32, "game/code_00286BA8", func_00287600);

INCLUDE_ASM(const s32, "game/code_00286BA8", func_00287638);

INCLUDE_ASM(const s32, "game/code_00286BA8", func_00287670);

u64 func_00287768(void) {
    u64 temp_v0;

    temp_v0 = func_00312810(D_00437924, 0xffffffffffffffff);
    func_00288920(temp_v0);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00286BA8", func_00287798);

INCLUDE_ASM(const s32, "game/code_00286BA8", func_002877E8);

INCLUDE_ASM(const s32, "game/code_00286BA8", func_00287848);

u64 func_00287900(void) {
    u64 temp_v0;

    temp_v0 = func_00312810(D_00437924, 0xffffffffffffffff);
    func_0028B1B0(temp_v0);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00286BA8", func_00287930);

INCLUDE_ASM(const s32, "game/code_00286BA8", func_00287AF8);

INCLUDE_ASM(const s32, "game/code_00286BA8", func_00287C20);

INCLUDE_ASM(const s32, "game/code_00286BA8", func_00288158);

INCLUDE_ASM(const s32, "game/code_00286BA8", func_002882B8);

INCLUDE_ASM(const s32, "game/code_00286BA8", func_002884C0);

INCLUDE_ASM(const s32, "game/code_00286BA8", func_002885E8);

INCLUDE_ASM(const s32, "game/code_00286BA8", func_00288710);

INCLUDE_RODATA(const s32, "game/code_00286BA8", D_00426280);

INCLUDE_ASM(const s32, "game/code_00286BA8", func_00288748);

INCLUDE_ASM(const s32, "game/code_00286BA8", func_00288920);

INCLUDE_ASM(const s32, "game/code_00286BA8", func_00288A70);

INCLUDE_ASM(const s32, "game/code_00286BA8", func_00288BD8);

INCLUDE_ASM(const s32, "game/code_00286BA8", func_00288DD0);

INCLUDE_RODATA(const s32, "game/code_00286BA8", D_004262B0);

INCLUDE_SDATA(const s32, "game/code_00286BA8", D_00437918);

INCLUDE_SDATA(const s32, "game/code_00286BA8", D_00437920);

INCLUDE_SDATA(const s32, "game/code_00286BA8", D_00437924);

INCLUDE_SDATA(const s32, "game/code_00286BA8", D_00437928);


INCLUDE_SDATA(const s32, "game/code_00286BA8", D_0043792C);

