#include "common.h"

extern s32 D_00435E38;

extern s32 func_0011AE90(void);

extern s32 func_003412A0(u32, u32);

extern u32 D_00435E88;

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011A118);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011A1D0);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011A220);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011A288);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011A2C8);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011A318);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011A328);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011A510);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011A700);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011A788);

void func_0011A7F8(s32 arg0, s32 arg1) {
    *(s32 *)(arg0 + 0x10) = *(s32 *)(arg0 + 0x10) + arg1;
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011A808);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011A938);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011A9C0);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011AA58);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011AB38);

u16 func_0011AE60(s32 arg0) {
    return *(u16 *)(arg0 * 8 + D_00435E38 + 2);
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011AE78);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011AE90);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011AEE0);

u8 func_0011B260(void) {
    s64 temp_v0;

    temp_v0 = func_0011AE90();
    return temp_v0 != 0;
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011B280);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011B2C0);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011B328);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011B4B0);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011B6F0);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011B9A0);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011BC80);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011C0B0);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011C328);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011C340);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011C680);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011C698);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011C6A0);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011C6A8);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011C868);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011C978);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011C998);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011CA28);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011CA88);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011CFE8);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D050);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D0D8);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D130);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D2A0);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D2D8);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D360);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D3B8);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D3E8);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D438);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D4C8);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D558);

u32 func_0011D588(void) {
    return D_00435E88;
}

void func_0011D590(void) {
    func_0010BF48(D_00435E88);
    D_00435E88 = 0;
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D5B8);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D5E0);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D608);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D630);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D658);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D680);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D6A8);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D758);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D808);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D8B0);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D958);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D990);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D9C8);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011DA50);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011DAE0);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011DB20);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011DBA0);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011DC38);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011DCC0);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011DD00);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011DD40);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011DD68);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011DDA0);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011DE08);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011DE58);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011DEA8);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011DEF8);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011DF60);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011DFA0);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011DFE0);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011E018);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011E050);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011E128);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011E160);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011E198);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011E1D0);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011E208);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011E268);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011E2A8);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011E3A0);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011E430);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011E498);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011E528);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011E728);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011E848);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011E930);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011EBC8);

void func_0011EBE0(void) {
}

void func_0011EBE8(void) {
}

void func_0011EBF0(void) {
}

void func_0011EBF8(void) {
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011EC00);
