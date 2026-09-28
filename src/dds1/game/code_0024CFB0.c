#include "common.h"

extern s32 D_003BC410;

extern s32 func_002CB3B8(u32, u32);

extern u8 D_003AF7A8[];

extern u32 D_003BC4CC;

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

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024E1C8);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024E260);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024E310);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024E3C0);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024E470);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024E5A0);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024E728);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024E8D0);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024EA50);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024EC08);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024EDC0);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024EF68);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024F0D0);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024F210);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024F338);

INCLUDE_RODATA(const s32, "game/code_0024CFB0", D_003AF658);

INCLUDE_RODATA(const s32, "game/code_0024CFB0", D_003AF668);

INCLUDE_RODATA(const s32, "game/code_0024CFB0", D_003AF678);

INCLUDE_RODATA(const s32, "game/code_0024CFB0", D_003AF688);

INCLUDE_RODATA(const s32, "game/code_0024CFB0", D_003AF6A0);

INCLUDE_RODATA(const s32, "game/code_0024CFB0", D_003AF6B0);

INCLUDE_RODATA(const s32, "game/code_0024CFB0", D_003AF6E0);

INCLUDE_RODATA(const s32, "game/code_0024CFB0", D_003AF6F0);

INCLUDE_RODATA(const s32, "game/code_0024CFB0", D_003AF700);

INCLUDE_RODATA(const s32, "game/code_0024CFB0", D_003AF710);

INCLUDE_RODATA(const s32, "game/code_0024CFB0", D_003AF720);

INCLUDE_RODATA(const s32, "game/code_0024CFB0", D_003AF730);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024F4F0);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024F570);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024F5B0);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024F608);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024F6F0);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024F760);

s32 func_0024F7A8(void) {
    if (func_002CB2E0(D_003AF7A8) != 0) {
        return 1;
    }
    D_003BC4CC = 0;
    return 0;
}

void func_0024F7D8(void) {
    func_002CB278(D_003BC4CC);
    D_003BC4CC = 0;
}

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024F800);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024F858);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024F8D8);

u32 func_0024FA18(void) {
    s32 temp_v0;

    temp_v0 = func_002CB3B8(D_003BC4CC, 0);
    return *(u32 *)(*(s32 *)(*(s32 *)(temp_v0 + 0xc) + 0x1c) + 0x70);
}

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024FA48);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024FA88);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024FAC8);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024FB30);

INCLUDE_RODATA(const s32, "game/code_0024CFB0", D_003AF7A8);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024FBB8);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_002501E0);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_00250758);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_00250820);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_002508D8);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_00250978);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_00250A48);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_00250B60);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_00250E88);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_00250F60);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_00251260);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_002512F0);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_002515C8);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_002515F0);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_002517C0);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_00251960);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_002519E8);

INCLUDE_RODATA(const s32, "game/code_0024CFB0", D_003AF810);

INCLUDE_RODATA(const s32, "game/code_0024CFB0", D_003AF830);

INCLUDE_RODATA(const s32, "game/code_0024CFB0", D_003AF840);

INCLUDE_RODATA(const s32, "game/code_0024CFB0", D_003AF850);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_00251A38);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_00251E38);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_00252CE8);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_00252E38);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_00252F88);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_00253018);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_002530D8);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_00253208);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_00253520);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_00253558);

s32 func_00253608(void) {
    s32 temp_v0 = func_002CB3B8(D_003BC4CC, 1);

    if (temp_v0 == 0) {
        return 0;
    }
    return *(s32 *)(*(s32 *)(*(s32 *)(temp_v0 + 0x484) + 8) + 4);
}

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_00253640);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_00253778);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_00253830);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_00253AD0);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_00253C78);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_00253CC8);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_00253CF8);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_00253D40);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_00253E58);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_00254218);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_00254288);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_00254680);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_002546D8);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_00254758);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_00254778);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_00254810);

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_002549F0);
