#include "common.h"

INCLUDE_ASM(const s32, "game/code_00159B48", func_00159B48);

u32 func_00159BB0(void) {
    return 0xf;
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_00159BB8);

INCLUDE_ASM(const s32, "game/code_00159B48", func_00159BD8);

INCLUDE_ASM(const s32, "game/code_00159B48", func_00159BE8);

INCLUDE_ASM(const s32, "game/code_00159B48", func_00159BF0);

void func_00159C00(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_00159C08);

INCLUDE_ASM(const s32, "game/code_00159B48", func_00159C40);

INCLUDE_ASM(const s32, "game/code_00159B48", func_00159CF0);

INCLUDE_ASM(const s32, "game/code_00159B48", func_00159D60);

u16 func_00159D80(s32 arg0) {
    return *(u16 *)(arg0 + 0x2c);
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_00159D88);

INCLUDE_ASM(const s32, "game/code_00159B48", func_00159DC0);

INCLUDE_ASM(const s32, "game/code_00159B48", func_00159DF0);

INCLUDE_ASM(const s32, "game/code_00159B48", func_00159E30);

INCLUDE_ASM(const s32, "game/code_00159B48", func_00159E50);

INCLUDE_ASM(const s32, "game/code_00159B48", func_00159E78);

INCLUDE_ASM(const s32, "game/code_00159B48", func_00159ED8);

INCLUDE_ASM(const s32, "game/code_00159B48", func_00159F38);

INCLUDE_ASM(const s32, "game/code_00159B48", func_00159F60);

INCLUDE_ASM(const s32, "game/code_00159B48", func_00159F80);

INCLUDE_ASM(const s32, "game/code_00159B48", func_00159FA0);

INCLUDE_ASM(const s32, "game/code_00159B48", func_00159FC8);

INCLUDE_ASM(const s32, "game/code_00159B48", func_00159FF8);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015A150);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015A1B8);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015A240);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015A310);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015A348);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015A360);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015A380);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015A3B0);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015A3C8);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015A3F0);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015A4B0);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015AA30);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015ACB0);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015AD18);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015B208);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015B270);

void func_0015B290(void) {
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015B298);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015B318);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015B330);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015B510);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015B5C0);

void func_0015B630(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x158) = *(float *)(arg1 + 0x158) * arg0;
    *(float *)(arg1 + 0x160) = *(float *)(arg1 + 0x160) * arg0;
    *(float *)(arg1 + 0x168) = *(float *)(arg1 + 0x168) * arg0;
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015B680);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015B700);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015B728);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015B988);

void func_0015BC38(s32 arg0) {
    s32 temp_v0;
    u32 temp_v1;

    temp_v1 = 0;
    temp_v0 = *(s32 *)(*(s32 *)(arg0 + 0xf8) + 4);
    if (*(s32 *)(arg0 + 0x20) != 0) {
        do {
            *(u32 *)(temp_v0 + 0x20) = 0xf0000001;
            temp_v1 = temp_v1 + 1;
            temp_v0 = temp_v0 + 0x40;
        } while (temp_v1 < *(u32 *)(arg0 + 0x20));
    }
}

void func_0015BC80(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x158) = *(float *)(arg1 + 0x158) * arg0;
    *(float *)(arg1 + 0x15c) = *(float *)(arg1 + 0x15c) * arg0;
    *(float *)(arg1 + 0x164) = *(float *)(arg1 + 0x164) * arg0;
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015BCD0);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015BD50);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015BD78);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015C020);

void func_0015C288(s32 arg0) {
    s32 temp_v0;
    u32 temp_v1;

    temp_v1 = 0;
    temp_v0 = *(s32 *)(*(s32 *)(arg0 + 0xf8) + 4);
    if (*(s32 *)(arg0 + 0x20) != 0) {
        do {
            *(u32 *)(temp_v0 + 0x20) = 0xf0000001;
            temp_v1 = temp_v1 + 1;
            temp_v0 = temp_v0 + 0x40;
        } while (temp_v1 < *(u32 *)(arg0 + 0x20));
    }
}

void func_0015C2D0(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x15c) = *(float *)(arg1 + 0x15c) * arg0;
    *(float *)(arg1 + 0x164) = *(float *)(arg1 + 0x164) * arg0;
    *(float *)(arg1 + 0x168) = *(float *)(arg1 + 0x168) * arg0;
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015C320);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015C3A0);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015C3C8);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015CBC8);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015CDF0);

void func_0015CE90(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x164) = *(float *)(arg1 + 0x164) * arg0;
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015CEC8);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015CF48);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015CF70);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015D208);

void func_0015D420(s32 arg0) {
    s32 temp_v0;
    u32 temp_v1;

    temp_v1 = 0;
    temp_v0 = *(s32 *)(*(s32 *)(arg0 + 0xf8) + 4);
    if (*(s32 *)(arg0 + 0x20) != 0) {
        do {
            *(u32 *)(temp_v0 + 0x20) = 0xf0000001;
            temp_v1 = temp_v1 + 1;
            temp_v0 = temp_v0 + 0x40;
        } while (temp_v1 < *(u32 *)(arg0 + 0x20));
    }
}

void func_0015D468(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x158) = *(float *)(arg1 + 0x158) * arg0;
    *(float *)(arg1 + 0x160) = *(float *)(arg1 + 0x160) * arg0;
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015D4A8);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015D528);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015D550);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015D8B0);

void func_0015DB20(s32 arg0) {
    s32 temp_v0;
    u32 temp_v1;

    temp_v1 = 0;
    temp_v0 = *(s32 *)(*(s32 *)(arg0 + 0xf8) + 4);
    if (*(s32 *)(arg0 + 0x20) != 0) {
        do {
            *(u32 *)(temp_v0 + 0x20) = 0xf0000001;
            temp_v1 = temp_v1 + 1;
            temp_v0 = temp_v0 + 0x40;
        } while (temp_v1 < *(u32 *)(arg0 + 0x20));
    }
}

void func_0015DB68(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x158) = *(float *)(arg1 + 0x158) * arg0;
    *(float *)(arg1 + 0x15c) = *(float *)(arg1 + 0x15c) * arg0;
    *(float *)(arg1 + 0x164) = *(float *)(arg1 + 0x164) * arg0;
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015DBB8);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015DC38);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015DC60);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015DF68);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015E1D0);

void func_0015E240(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x158) = *(float *)(arg1 + 0x158) * arg0;
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015E278);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015E2F8);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015E320);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015E510);

void func_0015E788(s32 arg0) {
    s32 temp_v0;
    u32 temp_v1;

    temp_v1 = 0;
    temp_v0 = *(s32 *)(*(s32 *)(arg0 + 0xf8) + 4);
    if (*(s32 *)(arg0 + 0x20) != 0) {
        do {
            *(u32 *)(temp_v0 + 0x20) = 0xf0000001;
            temp_v1 = temp_v1 + 1;
            temp_v0 = temp_v0 + 0x40;
        } while (temp_v1 < *(u32 *)(arg0 + 0x20));
    }
}

void func_0015E7D0(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x15c) = *(float *)(arg1 + 0x15c) * arg0;
    *(float *)(arg1 + 0x164) = *(float *)(arg1 + 0x164) * arg0;
    *(float *)(arg1 + 0x168) = *(float *)(arg1 + 0x168) * arg0;
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015E820);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015E8A0);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015E8C8);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015EB20);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015ED30);

void func_0015ED78(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x158) = *(float *)(arg1 + 0x158) * arg0;
    *(float *)(arg1 + 0x15c) = *(float *)(arg1 + 0x15c) * arg0;
    *(float *)(arg1 + 0x160) = *(float *)(arg1 + 0x160) * arg0;
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015EDC8);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015EE48);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015EE70);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015F1C0);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015F560);

void func_0015F578(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015F5A0);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015F620);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015F648);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015F748);

void func_0015F7D8(s32 arg0) {
    s32 temp_v0;
    u32 temp_v1;

    temp_v1 = 0;
    temp_v0 = *(s32 *)(*(s32 *)(arg0 + 0xf8) + 4);
    if (*(s32 *)(arg0 + 0x20) != 0) {
        do {
            *(u32 *)(temp_v0 + 0x20) = 0xf0000001;
            temp_v1 = temp_v1 + 1;
            temp_v0 = temp_v0 + 0x40;
        } while (temp_v1 < *(u32 *)(arg0 + 0x20));
    }
}

void func_0015F820(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x15c) = *(float *)(arg1 + 0x15c) * arg0;
    *(float *)(arg1 + 0x164) = *(float *)(arg1 + 0x164) * arg0;
    *(float *)(arg1 + 0x168) = *(float *)(arg1 + 0x168) * arg0;
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015F870);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015F8F0);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015F918);

INCLUDE_ASM(const s32, "game/code_00159B48", func_001600A8);

void func_00160308(s32 arg0) {
    s32 temp_v0;
    u32 temp_v1;

    temp_v1 = 0;
    temp_v0 = *(s32 *)(*(s32 *)(arg0 + 0xf8) + 4);
    if (*(s32 *)(arg0 + 0x20) != 0) {
        do {
            *(u32 *)(temp_v0 + 0x20) = 0xf0000001;
            temp_v1 = temp_v1 + 1;
            temp_v0 = temp_v0 + 0x40;
        } while (temp_v1 < *(u32 *)(arg0 + 0x20));
    }
}

void func_00160350(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x158) = *(float *)(arg1 + 0x158) * arg0;
    *(float *)(arg1 + 0x15c) = *(float *)(arg1 + 0x15c) * arg0;
    *(float *)(arg1 + 0x164) = *(float *)(arg1 + 0x164) * arg0;
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_001603A0);

INCLUDE_ASM(const s32, "game/code_00159B48", func_00160438);

INCLUDE_ASM(const s32, "game/code_00159B48", func_00160470);

INCLUDE_ASM(const s32, "game/code_00159B48", func_00160700);

void func_00160978(s32 arg0) {
    s32 temp_v0;
    u32 temp_v1;

    temp_v1 = 0;
    temp_v0 = *(s32 *)(*(s32 *)(arg0 + 0xf8) + 4);
    if (*(s32 *)(arg0 + 0x20) != 0) {
        do {
            *(u32 *)(temp_v0 + 0x20) = 0xf0000001;
            temp_v1 = temp_v1 + 1;
            temp_v0 = temp_v0 + 0x40;
        } while (temp_v1 < *(u32 *)(arg0 + 0x20));
    }
}

void func_001609C0(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x158) = *(float *)(arg1 + 0x158) * arg0;
    *(float *)(arg1 + 0x160) = *(float *)(arg1 + 0x160) * arg0;
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_00160A00);
