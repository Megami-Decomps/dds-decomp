#include "common.h"

typedef struct {
    s8 r;
    s8 g;
    s8 b;
    s8 a;
} KwlnFadeColor;

extern u32 D_003BA904;

extern KwlnFadeColor D_003BA920;

extern u16 D_003BA918;

extern u32 D_003BA8F0;

extern u32 D_003BA8F4;

extern s32 D_003BA8F8;

extern u32 D_003BA8DC;

extern s8 D_003BA7FC;

extern s8 D_003BA92B;

extern s8 D_003BA92C;

extern s16 D_003BA92E;

extern s16 D_003BA930;

extern s8 D_0032453B[];

extern f32 D_003245EC[];

extern f32 D_003245E0[];

extern f32 D_00324980[];

extern u8 D_00325748[];

extern u8 D_003BD690[2];

extern s32 D_003BD6A0[2];

extern u16 D_003BD6C0;

extern u16 D_003BD6C2;

extern s32 D_003BD308;
extern s32 D_003BA8D8;
extern u8 D_00324550[];
extern void func_002E8430(void *data, u32 tag);
extern s32 func_00104A98(void);
extern void func_00104B88(void *data, s32 handle);

extern void func_001050B8(void *arg0);

extern void func_00105150(s32 arg0);

extern void func_00105890(void);

extern void func_00108A48(void);

extern void func_002D0E88(s32 arg0);

extern void func_002E1718(void *arg0);

extern void func_002E3BA8(s32 arg0, s32 arg1, s32 arg2);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00102ED8);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00102F98);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00103160);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00103218);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00103400);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00103498);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00103540);

void func_001035F0(void) {
}

INCLUDE_ASM(const s32, "game/code_00102ED8", func_001035F8);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_001037C0);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_001038A0);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00103908);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00103970);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_001039E0);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00103B10);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00104008);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00104068);

void func_00104130(void) {
    D_003BD690[0] = 0;
    D_003BD6A0[0] = 0;
    D_003BD690[1] = 0;
    D_003BD6A0[1] = 0;
    func_002E3BA8(0, 0, 0);
}

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00104168);

u32 func_00104260(void) {
    return 0;
}

void func_00104268(void) {
    func_002E8430(D_00324550, 0x12345678);
}

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00104290);

void func_001045F8(void) {
}

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00104600);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00104678);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00104798);

INCLUDE_RODATA(const s32, "game/code_00102ED8", D_0039E078);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00104810);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_001049A8);

u32 func_00104A10(void) {
    return D_003BA8DC;
}

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00104A18);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00104A98);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00104B88);

s32 func_00104D20(s32 resource) {
    s32 status;

    if (D_003BD308 == 0) {
        return 0;
    }
    status = func_00104A98();
    func_00104B88((void *)resource, D_003BA8D8);
    return status;
}

s32 func_00104D70(void) {
    if (D_003BD308 == 0) {
        return 0;
    }
    if (func_00104A98() == 0) {
        return -1;
    }
    func_00104B88(D_00325748, D_003BA8D8);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00104DB8);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00104E20);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00105010);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_001050B8);

s32 func_00105118(void) {
    if (D_0032453B[0] != 0) {
        return 0;
    }
    func_001050B8(D_00325748);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00105150);

void func_00105320(void) {
    func_002D0E88(1);
    func_002E1718(&D_003245E0);
    func_002E1718(&D_00324980);
    func_00105150(0);
    func_00105150(1);
    func_00105890();
    D_003BA7FC = 0;
    func_00108A48();
}

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00105370);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_001054D0);

void func_001055C0(void) {
    if (D_003BA8F8 != 0) {
        func_002D2CB8(D_003BA8F8);
    }
    D_003BA8F8 = 0;
    D_003BA8F4 = 0;
    D_003BA8F0 = 0;
}

s32 func_001055F8(void) {
    s32 ret = 0;

    if (D_003BA8F8 == 0) {
        return ret;
    }
    D_003BA8F0 = 1;
    return 1;
}

void func_00105618(void) {
    D_003BA8F0 = 0;
}

u32 func_00105620(void) {
    return D_003BA8F0;
}

s32 func_00105628(void) {
    return D_003BA8F8;
}

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00105630);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00105828);

void func_00105888(void) {
    D_003BA918 = 0;
}

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00105890);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_001059F0);

void func_00105A68(void) {
    D_003BA904 &= ~3;
    D_003BA920.r = 0;
    D_003BA920.g = 0;
    D_003BA920.b = 0;
    D_003BA920.a = 0;
}

void func_00105A90(s8 arg0, s8 arg1, s8 arg2, s8 arg3) {
    D_003BA904 &= ~3;
    D_003BA920.r = arg0;
    D_003BA920.g = arg1;
    D_003BA920.b = arg2;
    D_003BA920.a = arg3;
}

void func_00105AB8(KwlnFadeColor **arg0) {
    *arg0 = &D_003BA920;
}

void func_00105AC8(s8 arg0, s8 arg1, s8 arg2) {
    D_003BA920.r = arg0;
    D_003BA920.g = arg1;
    D_003BA920.b = arg2;
}

void func_00105AE0(s8 arg0, s8 arg1, s8 arg2, s32 arg3) {
    D_003BA920.r = arg0;
    D_003BA920.g = arg1;
    D_003BA920.b = arg2;
    D_003BA920.a = -0x80;
    if (arg3 == 0) {
        D_003BA920.a = 0;
        D_003BD6C0 = 0;
        D_003BD6C2 = 0;
        func_00105A68();
        return;
    }
    D_003BD6C2 = arg3;
    D_003BD6C0 = arg3;
    D_003BA904 = (D_003BA904 | 1) & ~2;
}

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00105B48);

void func_00105B98(s8 arg0, s8 arg1, s8 arg2, s32 arg3) {
    D_003BA920.r = arg0;
    D_003BA920.g = arg1;
    D_003BA920.b = arg2;
    D_003BA920.a = 0;
    if (arg3 == 0) {
        D_003BD6C0 = 0;
        D_003BD6C2 = 0;
        D_003BA920.a = -0x80;
        D_003BA904 &= ~3;
        return;
    }
    D_003BD6C2 = arg3;
    D_003BD6C0 = 0;
    D_003BA904 = (D_003BA904 & ~1) | 2;
}

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00105BF8);

u8 func_00105C48(void) {
    return (D_003BA904 & 3) != 0;
}

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00105C58);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00105D00);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00105DD8);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00106088);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_001060C8);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00106160);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_001061E8);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00106240);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00106268);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00106368);

void func_00106488(f32 arg0) {
    D_003245EC[0] = arg0;
}
