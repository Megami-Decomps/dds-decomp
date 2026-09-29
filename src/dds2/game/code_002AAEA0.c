#include "common.h"

extern s32 func_00101958();

extern s64 func_002C4038(s32, s32 *, u64, u64);

extern s64 func_002D13F0(void);

extern u8 D_003E7034[];

INCLUDE_ASM(const s32, "game/code_002AAEA0", func_002AAEA0);

u32 func_002AAF40(void) {
    s32 context;

    context = func_00101958();
    *(u32 *)(context + 0xb1d0) = 1;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002AAEA0", func_002AAF70);

INCLUDE_ASM(const s32, "game/code_002AAEA0", func_002AB0E0);

INCLUDE_ASM(const s32, "game/code_002AAEA0", func_002AB1B0);

INCLUDE_ASM(const s32, "game/code_002AAEA0", func_002AB1E8);

/* Switch the staff display to the alternate resource at context + 0x60. */
u32 func_002AB240(void) {
    s32 context;

    context = func_00101958();
    func_002BB498(*(u32 *)(context + 0x118), *(u32 *)(context + 0x60), 0, 1);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002AAEA0", func_002AB278);

INCLUDE_ASM(const s32, "game/code_002AAEA0", func_002AB2E8);

INCLUDE_ASM(const s32, "game/code_002AAEA0", func_002AB368);

u32 func_002AB3A0(void) {
    return 1;
}

u32 func_002AB3A8(void) {
    func_002AAEA0();
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002AAEA0", func_002AB3C8);

INCLUDE_ASM(const s32, "game/code_002AAEA0", func_002AB448);

INCLUDE_ASM(const s32, "game/code_002AAEA0", func_002AB518);

u32 func_002AB550(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002AAEA0", func_002AB558);

INCLUDE_ASM(const s32, "game/code_002AAEA0", func_002AB598);

INCLUDE_ASM(const s32, "game/code_002AAEA0", func_002AB650);

INCLUDE_ASM(const s32, "game/code_002AAEA0", func_002AB690);

INCLUDE_SDATA(const s32, "game/code_002AAEA0", D_00437B88);

INCLUDE_SDATA(const s32, "game/code_002AAEA0", D_00437B90);

INCLUDE_SDATA(const s32, "game/code_002AAEA0", D_00437B98);

INCLUDE_SDATA(const s32, "game/code_002AAEA0", D_00437BA0);

INCLUDE_SDATA(const s32, "game/code_002AAEA0", D_00437BA8);

INCLUDE_SDATA(const s32, "game/code_002AAEA0", D_00437BB0);

INCLUDE_SDATA(const s32, "game/code_002AAEA0", D_00437BB8);

INCLUDE_SDATA(const s32, "game/code_002AAEA0", D_00437BC0);

INCLUDE_SDATA(const s32, "game/code_002AAEA0", D_00437BC8);

