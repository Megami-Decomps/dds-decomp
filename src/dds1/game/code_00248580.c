#include "common.h"

extern s32 func_0027B888(u32);

extern s32 func_00105C48(void);

extern s32 func_0024A6C0(s32);

extern u8 D_003BC3E1;
extern s8 D_003BC3E0;


extern s32 func_0010FD80(void);

extern s32 func_0021F600(u32);

extern s64 func_0024DC08(void);
void func_00248580(s32 arg0) {
    func_002BD7A0(*(u32 *)(arg0 + 100));
    func_002BD7A0(*(u32 *)(arg0 + 0x68));
}

void func_002485B0(s32 arg0) {
    func_002BD870(*(u32 *)(arg0 + 100));
    func_002BD870(*(u32 *)(arg0 + 0x68));
}

INCLUDE_ASM(const s32, "game/code_00248580", func_002485E0);

INCLUDE_ASM(const s32, "game/code_00248580", func_00248658);

INCLUDE_ASM(const s32, "game/code_00248580", func_00248700);

INCLUDE_ASM(const s32, "game/code_00248580", func_00248750);

INCLUDE_RODATA(const s32, "game/code_00248580", D_003AF570);

INCLUDE_RODATA(const s32, "game/code_00248580", D_003AF580);

INCLUDE_RODATA(const s32, "game/code_00248580", D_003AF590);

INCLUDE_RODATA(const s32, "game/code_00248580", D_003AF5A8);

INCLUDE_ASM(const s32, "game/code_00248580", func_00248810);

INCLUDE_ASM(const s32, "game/code_00248580", func_00248BA0);

void func_00248C38(s32 arg0) {
    if (arg0 != 0) {
        func_00284340();
        func_00284340((s32)arg0 + 0x54);
        func_002CFF98(arg0);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_00248580", func_00248C80);

void func_00248CF8(s32 arg0) {
    s32 node;

    for (node = *(s32 *)(*(s32 *)(arg0 + 0x74) + 0x10); node != 0; node = *(s32 *)(node + 0x58)) {
        func_00248C38(*(u32 *)(node + 0x70));
    }
}

INCLUDE_ASM(const s32, "game/code_00248580", func_00248D40);

void func_00248E18(s32 arg0) {
    func_0027B368(*(u32 *)(arg0 + 0x74));
}

void func_00248E30(s32 arg0) {
    func_00248C38(*(u32 *)(*(s32 *)(*(s32 *)(arg0 + 0x74) + 0x1c) + 0x70));
    func_0027B888(*(u32 *)(arg0 + 0x74));
}

INCLUDE_ASM(const s32, "game/code_00248580", func_00248E68);

INCLUDE_ASM(const s32, "game/code_00248580", func_00249010);

INCLUDE_ASM(const s32, "game/code_00248580", func_00249058);

u8 func_00249198(void) {
    s64 temp_v0;

    temp_v0 = func_0021F600(0x902);
    return temp_v0 == 0;
}

INCLUDE_ASM(const s32, "game/code_00248580", func_002491B8);

INCLUDE_ASM(const s32, "game/code_00248580", func_002492F8);

INCLUDE_ASM(const s32, "game/code_00248580", func_002493B0);

INCLUDE_ASM(const s32, "game/code_00248580", func_00249420);

INCLUDE_ASM(const s32, "game/code_00248580", func_00249498);

INCLUDE_ASM(const s32, "game/code_00248580", func_002495F8);

INCLUDE_ASM(const s32, "game/code_00248580", func_00249668);

INCLUDE_ASM(const s32, "game/code_00248580", func_002496D8);

INCLUDE_ASM(const s32, "game/code_00248580", func_002496F0);

void func_00249770(u32 *arg0) {
    func_00280488(arg0 + 100);
    func_00276320(arg0 + 2);
    func_00271648(arg0 + 2);
    func_002BC618(arg0[1]);
    func_002D0918(*arg0);
}

INCLUDE_ASM(const s32, "game/code_00248580", func_002497C0);

INCLUDE_ASM(const s32, "game/code_00248580", func_00249850);

void func_00249930(s32 arg0) {
    func_0027F6B8(arg0 + 400);
    func_0027FA20(arg0 + 400);
    func_00283038(*(u32 *)(arg0 + 0x820));
    func_00283820(*(u32 *)(arg0 + 0x824));
    func_00285160(*(u32 *)(arg0 + 0x828));
}

void func_00249980(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    func_00285440(arg0, arg1, arg2, *(s32 *)(arg3 + 0x828));
}
INCLUDE_ASM(const s32, "game/code_00248580", func_00249998);


INCLUDE_ASM(const s32, "game/code_00248580", func_00249A60);

void func_00249C08(s8 arg0) {
    s64 temp_v0;

    if (arg0 == '\x01') {
        temp_v0 = func_0010FD80();
        if (temp_v0 != 0) {
            func_00110860(temp_v0, 1);
        }
        D_003BC3E1 = 1;
    }
    else {
        temp_v0 = func_0010FD80();
        if (temp_v0 != 0) {
            func_00110860(temp_v0, 0);
        }
        D_003BC3E1 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00248580", func_00249C78);

INCLUDE_ASM(const s32, "game/code_00248580", func_00249D80);

INCLUDE_ASM(const s32, "game/code_00248580", func_00249DD0);

INCLUDE_RODATA(const s32, "game/code_00248580", D_003AF5E0);

INCLUDE_RODATA(const s32, "game/code_00248580", D_003AF620);

INCLUDE_ASM(const s32, "game/code_00248580", func_00249E20);

INCLUDE_ASM(const s32, "game/code_00248580", func_00249F08);

INCLUDE_ASM(const s32, "game/code_00248580", func_00249FA8);

INCLUDE_ASM(const s32, "game/code_00248580", func_0024A058);

s32 func_0024A0A0(void) {
    s32 state = D_003BC3E0;
    if (state == 1) {
        return 1;
    }
    if (state < 2) {
        return 0;
    }
    if (state == 2) {
        D_003BC3E0 = 0;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00248580", func_0024A0D8);


INCLUDE_ASM(const s32, "game/code_00248580", func_0024A138);

INCLUDE_ASM(const s32, "game/code_00248580", func_0024A170);

s32 func_0024A1A8(void) {
    s32 temp_v0 = func_00105C48();

    if (temp_v0 != 0) {
        return 0;
    }
    return func_0024DC08() == 0;
}

INCLUDE_ASM(const s32, "game/code_00248580", func_0024A1D8);

void func_0024A2B8(s32 arg0) {
    u16 temp_E = *(u16 *)(arg0 + 0xe);
    u16 temp_8 = *(u16 *)(arg0 + 8);
    u16 temp_C = *(u16 *)(arg0 + 0xc);
    u16 temp_M = temp_E & 0xfa2f;

    *(u16 *)(arg0 + 6) = temp_8;
    *(u16 *)(arg0 + 0xa) = temp_C;
    *(u16 *)(arg0 + 0xe) = temp_M;
}

INCLUDE_ASM(const s32, "game/code_00248580", func_0024A2D8);

INCLUDE_ASM(const s32, "game/code_00248580", func_0024A340);

s32 func_0024A478(s32 arg0) {
    s32 value = *(s32 *)(arg0 + 0x9C);
    if (value == 0) {
        return 0;
    }
    return value >= 60 ? 2 : 1;
}

INCLUDE_ASM(const s32, "game/code_00248580", func_0024A4A0);

INCLUDE_ASM(const s32, "game/code_00248580", func_0024A570);

INCLUDE_ASM(const s32, "game/code_00248580", func_0024A610);

INCLUDE_ASM(const s32, "game/code_00248580", func_0024A6C0);

u8 func_0024A6E8(s32 arg0) {
    s32 temp_v0;

    temp_v0 = func_0024A6C0(arg0);
    return *(u8 *)(temp_v0 * 0xa0 + *(s32 *)(*(s32 *)(arg0 + 100) + 0x18) + 0x14);
}

INCLUDE_ASM(const s32, "game/code_00248580", func_0024A728);

INCLUDE_ASM(const s32, "game/code_00248580", func_0024A930);

INCLUDE_ASM(const s32, "game/code_00248580", func_0024AB28);

INCLUDE_ASM(const s32, "game/code_00248580", func_0024AB70);

INCLUDE_ASM(const s32, "game/code_00248580", func_0024ACD8);

INCLUDE_ASM(const s32, "game/code_00248580", func_0024AE18);

INCLUDE_ASM(const s32, "game/code_00248580", func_0024AF58);

INCLUDE_ASM(const s32, "game/code_00248580", func_0024B090);

INCLUDE_ASM(const s32, "game/code_00248580", func_0024B168);

void func_0024B2D0(u32 arg0, s32 arg1) {
    u32 temp_v0;

    temp_v0 = *(u32 *)(arg1 + 0xc4);
    *(u32 *)(arg1 + 0xc4) = arg0;
    *(u32 *)(arg1 + 200) = temp_v0;
}

INCLUDE_ASM(const s32, "game/code_00248580", func_0024B2E0);

INCLUDE_ASM(const s32, "game/code_00248580", func_0024B358);
