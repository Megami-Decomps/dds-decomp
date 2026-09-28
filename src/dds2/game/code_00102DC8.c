#include "common.h"

extern u32 D_00435CAC;

extern u32 D_00435CC0;

extern u32 D_00435CC4;

extern s32 D_00435CC8;

extern u16 D_00435CE8;

extern u32 D_00435CD4;

typedef struct {
    s8 r;
    s8 g;
    s8 b;
    s8 a;
} KwlnFadeColor;

extern KwlnFadeColor D_00435CF0;

extern u16 D_00438DC0;

extern u16 D_00438DC2;

extern f32 D_0037F5EC[];

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00102DC8);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00102E88);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00103050);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00103108);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_001032F0);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00103388);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00103430);

void func_001034E0(void) {
}

INCLUDE_ASM(const s32, "game/code_00102DC8", func_001034E8);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_001036B0);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00103790);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_001037F8);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00103860);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_001038D0);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00103A00);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00103EF8);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00103F58);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00104020);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00104058);

u32 func_00104150(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00104158);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00104180);

void func_001044E8(void) {
}

INCLUDE_ASM(const s32, "game/code_00102DC8", func_001044F0);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00104568);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00104688);

INCLUDE_RODATA(const s32, "game/code_00102DC8", D_004111F8);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00104700);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00104898);

u32 func_00104900(void) {
    return D_00435CAC;
}

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00104908);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_001049B8);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00104AA8);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00104C40);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00104C90);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00104CD8);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00104D40);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00104F30);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00104FD8);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00105038);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00105070);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00105240);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00105290);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_001053F0);

void func_001054E0(void) {
    if (D_00435CC8 != 0) {
        func_0032BB68(D_00435CC8);
    }
    D_00435CC8 = 0;
    D_00435CC4 = 0;
    D_00435CC0 = 0;
}

s32 func_00105518(void) {
    s32 ret = 0;

    if (D_00435CC8 == 0) {
        return ret;
    }
    D_00435CC0 = 1;
    return 1;
}

void func_00105538(void) {
    D_00435CC0 = 0;
}

u32 func_00105540(void) {
    return D_00435CC0;
}

s32 func_00105548(void) {
    return D_00435CC8;
}

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00105550);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00105748);

void func_001057A8(void) {
    D_00435CE8 = 0;
}

INCLUDE_ASM(const s32, "game/code_00102DC8", func_001057B0);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00105910);

void func_00105988(void) {
    D_00435CD4 &= ~3;
    D_00435CF0.r = 0;
    D_00435CF0.g = 0;
    D_00435CF0.b = 0;
    D_00435CF0.a = 0;
}

void func_001059B0(s8 arg0, s8 arg1, s8 arg2, s8 arg3) {
    D_00435CD4 &= ~3;
    D_00435CF0.r = arg0;
    D_00435CF0.g = arg1;
    D_00435CF0.b = arg2;
    D_00435CF0.a = arg3;
}

void func_001059D8(u32 *arg0) {
    *arg0 = &D_00435CF0;
}

void func_001059E8(s8 arg0, s8 arg1, s8 arg2) {
    D_00435CF0.r = arg0;
    D_00435CF0.g = arg1;
    D_00435CF0.b = arg2;
}

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00105A00);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00105A68);

void func_00105AB8(s8 arg0, s8 arg1, s8 arg2, s32 arg3) {
    D_00435CF0.r = arg0;
    D_00435CF0.g = arg1;
    D_00435CF0.b = arg2;
    D_00435CF0.a = 0;
    if (arg3 == 0) {
        D_00438DC0 = 0;
        D_00438DC2 = 0;
        D_00435CF0.a = -0x80;
        D_00435CD4 &= ~3;
        return;
    }
    D_00438DC2 = arg3;
    D_00438DC0 = 0;
    D_00435CD4 = (D_00435CD4 & ~1) | 2;
}

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00105B18);

u8 func_00105B68(void) {
    return (D_00435CD4 & 3) != 0;
}

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00105B78);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00105C20);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00105CF8);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00105FA8);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00105FE8);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00106080);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00106108);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00106160);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00106188);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00106288);

void func_001063A8(f32 arg0) {
    D_0037F5EC[0] = arg0;
}
