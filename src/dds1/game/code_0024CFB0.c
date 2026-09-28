#include "common.h"

extern s32 D_003BC410;

extern s32 D_003BC408;

extern u8 D_003BC40C;

extern s8 D_003BC40D;

extern s8 D_003BC414;

extern s8 D_003BC415;

typedef struct {
    s32 unk0;
    s32 unk4;
    s8 data[0];
} UnkBD8A0;

extern UnkBD8A0 D_003BD8A0;

extern u32 func_002EB028(u32, u32 *, u32);

extern s64 func_0024DC08(void);

extern s32 func_00101A70();

extern u32 D_003D8100[];

void func_0024CFB0(s32 arg0) {
    func_0024DBC8();
    func_0024D9D8(*(u32 *)(arg0 + 0x60));
}

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024CFD8);

s32 func_0024D220(void) {
    s32 *temp_v0 = (s32 *)func_00101A70();

    func_00249DD0(temp_v0);
    func_0024B358(0, temp_v0);
    return 1;
}

u32 func_0024D260(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024D268);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024D300);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024D350);

u32 func_0024D398(void) {
    s32 temp_v0;

    temp_v0 = func_00101A70();
    if (*(s32 *)(temp_v0 + 0xdc) == 0) {
        func_0024DED8();
        func_0024DEF8(0, 1);
        func_0024DEF8(1, 0);
    }
    else {
        func_00105B98(0, 0, 0, 0xf);
    }
    return 1;
}

s32 func_0024D400(void) {
    s32 temp_v0 = func_00101A70();

    func_0024B358(1, temp_v0);
    func_00105AE0(0, 0, 0, 0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024D440);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024D500);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024D550);

u32 func_0024D588(void) {
    s32 temp_v0;

    temp_v0 = func_00101A70();
    *(u32 *)(temp_v0 + 0x90) = 0;
    return 1;
}

s32 func_0024D5B0(void) {
    s32 *temp_v0 = (s32 *)func_00101A70();

    func_002495F8(temp_v0);
    func_00249498(temp_v0);
    return 1;
}

u32 func_0024D5F0(void) {
    return 0;
}

u32 func_0024D5F8(void) {
    return 0;
}

u32 func_0024D600(void) {
    return 0;
}

u32 func_0024D608(void) {
    return 0xffffffff;
}

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024D610);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024D670);

void func_0024D6E0(void) {
    func_001068F0(0, 0, 0);
    func_00106D08(0);
    func_00106F40(0);
    func_0018F3B0();
    func_0018F438();
    func_0018F750();
    func_0018F4F0();
    func_0018F6E8();
}

void func_0024D738(void) {
    func_00226C38();
    func_0024D6E0();
}

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024D758);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024D778);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024D7B8);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024D828);

s32 func_0024D880(u8 *arg0, u8 *arg1) {
    u8 temp_A = *arg0;
    u8 temp_B = *arg1;

    if (temp_B < temp_A) {
        return 1;
    }
    return (temp_A < temp_B) ? -1 : 0;
}

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024D8A8);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024D8F8);

void func_0024D990(u32 arg0, u32 *arg1) {
    u32 temp_v0;

    temp_v0 = func_002EB028(arg0, arg1 + 1, 0);
    *arg1 = temp_v0;
}

void func_0024D9C0(u32 *arg0) {
    func_002D0918(*arg0);
}

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024D9D8);

s32 func_0024DA20(s32 arg0) {
    if (D_003BC408 < 0) {
        return 0;
    }
    func_0019C968(D_003BC408, 0, arg0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024DA58);

s32 func_0024DAB8(s32 arg0) {
    if (D_003BC408 < 0) {
        return 0;
    }
    D_003BC410 = arg0;
    D_003BC415 = func_0024DB08();
    return 1;
}

void func_0024DAE8(s32 arg0) {
    if (D_003BC408 >= 0) {
        D_003BC414 = arg0;
    }
}

s8 func_0024DB00(void) {
    return D_003BC414;
}

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024DB08);

s8 func_0024DB40(void) {
    return D_003BC415;
}

u32 func_0024DB48(s32 arg0) {
    u32 temp_v0;

    temp_v0 = 0;
    if (-1 < D_003BC408) {
        func_0019AE58(D_003BC408, 0);
        if (arg0 != 0) {
            func_0019B4A0(D_003BC408);
        }
        func_0019BC98(D_003BC408, 0);
        func_0024DDC0(1);
        D_003BC40C = 0;
        temp_v0 = 1;
    }
    return temp_v0;
}

void func_0024DBB0(void) {
    func_0024DB48(1);
}

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024DBC8);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024DC08);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024DC50);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024DC98);

void func_0024DD78(void) {
    func_0024DC98(1);
}

void func_0024DD90(s32 arg0, s32 arg1) {
    func_0019C838(D_003BC408, arg0, arg1);
}

s8 func_0024DDB8(void) {
    return D_003BC40D;
}

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024DDC0);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024DE30);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024DE78);

void func_0024DED8(s32 arg0) {
    D_003BD8A0.data[arg0] = 0;
}

s32 func_0024DEE8(s32 arg0) {
    return D_003BD8A0.data[arg0] != 0;
}

s32 func_0024DEF8(s32 arg0, s32 arg1) {
    if (arg0 < 0x10) {
    } else {
        return 0;
    }
    D_003D8100[arg0] = arg1;
    return 1;
}

u32 func_0024DF20(s32 arg0) {
    arg0 = (arg0 < 0x10) ? arg0 : 0xf;
    return D_003D8100[arg0];
}

s32 func_0024DF48(void) {
    s32 temp_v0 = func_0010D428(0);

    D_003BD8A0.data[temp_v0] = 1;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024DF78);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024DFC0);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024E010);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024E100);

void func_0024E198(u32 arg0, u32 arg1, u32 arg2, u32 arg3) {
    func_002C0DD8(arg0, arg1, 0, arg2, arg3, 0x30303040, 0x53);
}





INCLUDE_SDATA(const s32, "game/code_0024CFB0", D_003BC408);

INCLUDE_SDATA(const s32, "game/code_0024CFB0", D_003BC40C);

INCLUDE_SDATA(const s32, "game/code_0024CFB0", D_003BC40D);

INCLUDE_SDATA(const s32, "game/code_0024CFB0", D_003BC410);

INCLUDE_SDATA(const s32, "game/code_0024CFB0", D_003BC414);

INCLUDE_SDATA(const s32, "game/code_0024CFB0", D_003BC415);


INCLUDE_SDATA(const s32, "game/code_0024CFB0", D_003BC418);

