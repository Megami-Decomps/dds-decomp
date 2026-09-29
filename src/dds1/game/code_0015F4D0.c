#include "common.h"
#include "pcp_vu0.h"
#include "eff.h"

extern BillDispatch D_0034E658[];

extern BillDispatch D_0034E654[];

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_0015F4D0);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_0015F520);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_0015F578);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_0015F5B0);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_0015F5E8);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_0015F630);

u16 func_0015F670(s32 arg0) {
    return *(u16 *)(arg0 + 0xb2);
}

void func_0015F678(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_0015F688);

void func_0015F6E8(BillObj *effect, void *value) {
    effect->unk60 = value;
}

void func_0015F6F0(u8 *work, u8 value) {
    if (*(u16 *)(work + 0xB0) == 0) {
        *(u8 *)(work + 0xC0) = value;
    }
    *(u8 *)(work + 0x64) = value;
}

u8 func_0015F708(s32 arg0) {
    return *(u8 *)(arg0 + 100);
}

void func_0015F710(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x1c) = arg1;
}

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_0015F718);

void func_0015F7F0(s32 arg0, s32 arg1, u32 arg2) {
    *(u32 *)(arg1 * 0x14 + *(s32 *)(arg0 + 0x14) + 0x10) = arg2;
}

s32 func_0015F810(s32 arg0) {
    return arg0 + 0x20;
}

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_0015F818);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_0015F890);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_0015F9C8);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_0015FB88);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_0015FC48);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_0015FD98);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_0015FE20);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_00160210);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_001602F8);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_00160690);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_001606C0);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_00160800);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_00160858);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_00160888);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_001608B8);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_00160910);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_00160958);

INCLUDE_RODATA(const s32, "game/code_0015F4D0", D_003A0DB8);

INCLUDE_SDATA(const s32, "game/code_0015F4D0", D_003BB018);

INCLUDE_SDATA(const s32, "game/code_0015F4D0", D_003BB01C);

INCLUDE_SDATA(const s32, "game/code_0015F4D0", D_003BB020);

