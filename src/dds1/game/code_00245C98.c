#include "common.h"

extern s32 func_00105C48(void);

extern s64 func_0024DC08(void);

extern s64 func_00285670(s32, s32 *, u64, u64);

extern s32 func_00101A70();

s32 func_00245C98(void) {
    s32 temp_v0 = func_00105C48();

    if (temp_v0 != 0) {
        return 0;
    }
    return func_0024DC08() == 0;
}

INCLUDE_ASM(const s32, "game/code_00245C98", func_00245CC8);

INCLUDE_ASM(const s32, "game/code_00245C98", func_00245D08);

s32 func_00245DA0(void) {
    s32 *temp_v0 = (s32 *)func_00101A70();

    if (temp_v0[43] != 1) {
    } else {
        func_002605B0(temp_v0, 4);
    }
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_00245C98", D_003AF418);

INCLUDE_RODATA(const s32, "game/code_00245C98", D_003AF428);

INCLUDE_ASM(const s32, "game/code_00245C98", func_00245DE0);

INCLUDE_ASM(const s32, "game/code_00245C98", func_00246088);

INCLUDE_ASM(const s32, "game/code_00245C98", func_002460D8);

INCLUDE_ASM(const s32, "game/code_00245C98", func_00246120);

s32 func_00246160(void) {
    s32 *temp_v0 = (s32 *)func_00101A70();

    if (temp_v0[43] != 1) {
    } else {
        func_002453C8(temp_v0);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00245C98", func_00246198);

INCLUDE_ASM(const s32, "game/code_00245C98", func_00246220);

INCLUDE_ASM(const s32, "game/code_00245C98", func_00246460);

INCLUDE_ASM(const s32, "game/code_00245C98", func_002464B0);

INCLUDE_ASM(const s32, "game/code_00245C98", func_002464F8);

s32 func_00246538(void) {
    s32 *temp_v0 = (s32 *)func_00101A70();

    if (temp_v0[43] != 1) {
    } else {
        func_002457E8(temp_v0);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00245C98", func_00246570);

INCLUDE_ASM(const s32, "game/code_00245C98", func_002465F8);

INCLUDE_ASM(const s32, "game/code_00245C98", func_00246838);

INCLUDE_ASM(const s32, "game/code_00245C98", func_00246888);

INCLUDE_ASM(const s32, "game/code_00245C98", func_002468D0);

INCLUDE_ASM(const s32, "game/code_00245C98", func_00246910);

INCLUDE_ASM(const s32, "game/code_00245C98", func_00246968);

INCLUDE_ASM(const s32, "game/code_00245C98", func_002469F0);

INCLUDE_ASM(const s32, "game/code_00245C98", func_00246BE8);

INCLUDE_ASM(const s32, "game/code_00245C98", func_00246C38);

u32 func_00246C80(void) {
    s32 temp_v0;

    temp_v0 = func_00101A70();
    *(u32 *)(temp_v0 + 0xa4) = 0;
    func_0027BB28(*(u32 *)(*(s32 *)(temp_v0 + 0x6c) + 0x14));
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00245C98", func_00246CB0);

INCLUDE_ASM(const s32, "game/code_00245C98", func_00246D68);

INCLUDE_ASM(const s32, "game/code_00245C98", func_00246DB8);

s32 func_00246E00(void) {
    s32 *temp_v0 = (s32 *)func_00101A70();

    temp_v0[32] = 1;
    func_00245190(-1, temp_v0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00245C98", func_00246E38);

INCLUDE_ASM(const s32, "game/code_00245C98", func_00246EA0);

INCLUDE_ASM(const s32, "game/code_00245C98", func_002470F8);

INCLUDE_ASM(const s32, "game/code_00245C98", func_00247148);

INCLUDE_ASM(const s32, "game/code_00245C98", func_00247190);

INCLUDE_ASM(const s32, "game/code_00245C98", func_002472D8);

INCLUDE_ASM(const s32, "game/code_00245C98", func_002473D0);

INCLUDE_ASM(const s32, "game/code_00245C98", func_00247420);

INCLUDE_ASM(const s32, "game/code_00245C98", func_00247588);

INCLUDE_ASM(const s32, "game/code_00245C98", func_00247728);

INCLUDE_ASM(const s32, "game/code_00245C98", func_002478B0);

INCLUDE_ASM(const s32, "game/code_00245C98", func_002478F8);

INCLUDE_ASM(const s32, "game/code_00245C98", func_00247970);

s64 func_002479F0(u64 arg0) {
    s32 temp_v0;
    s64 temp_v1;
    s32 *piVar3;

    temp_v0 = func_00101A70();
    piVar3 = (s32 *)(temp_v0 + 0x54);
    temp_v1 = func_00285670(temp_v0 + 8, piVar3, 0, arg0);
    if (temp_v1 == 0) {
        if ((*piVar3 == 0) && (temp_v1 = func_0024DC08(), temp_v1 == 0)) {
            func_002858F8(piVar3, *(u32 *)(temp_v0 + 0x58));
        }
        temp_v1 = 0;
    }
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_00245C98", func_00247A78);

INCLUDE_ASM(const s32, "game/code_00245C98", func_00247CB0);

u32 func_00247CF8(void) {
    func_00220110(0x323);
    func_00105AE0(0, 0, 0, 0xf);
    func_0024DED8(0);
    func_0024DEF8(0, 0);
    func_0024DEF8(1, 1);
    return 1;
}

u32 func_00247D50(void) {
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_00245C98", D_003AF528);

INCLUDE_ASM(const s32, "game/code_00245C98", func_00247D58);

INCLUDE_ASM(const s32, "game/code_00245C98", func_00248088);

INCLUDE_ASM(const s32, "game/code_00245C98", func_002480E8);

INCLUDE_ASM(const s32, "game/code_00245C98", func_00248130);

u32 func_00248170(void) {
    func_00105AE0(0, 0, 0, 0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00245C98", func_002481A0);

INCLUDE_ASM(const s32, "game/code_00245C98", func_00248240);

INCLUDE_ASM(const s32, "game/code_00245C98", func_00248278);

u32 func_002482B0(void) {
    return 1;
}

u32 func_002482B8(void) {
    return 1;
}

u32 func_002482C0(void) {
    return 0;
}

u32 func_002482C8(void) {
    return 0;
}

u32 func_002482D0(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00245C98", func_002482D8);

INCLUDE_ASM(const s32, "game/code_00245C98", func_002483C0);

INCLUDE_ASM(const s32, "game/code_00245C98", func_00248468);

INCLUDE_ASM(const s32, "game/code_00245C98", func_00248508);
