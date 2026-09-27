#include "common.h"

extern s64 func_002A2928(void);

extern u64 func_00205018(void);

extern u32 D_00436AD4;
extern u64 func_0010FCA8(void);
extern u32 func_00116810(u64);

extern s32 func_001E3168(void);

extern s32 func_0023A170(u32);

extern s32 func_0022F180(void);

extern u64 func_001D4160(u64, u64, u64);

extern u32 D_004367F8;

extern s32 func_0011A2C8(void);

extern s32 func_001AAA80(u8);

extern s32 func_00101820(u32);

extern s8 D_003B4CF8[13];

extern u32 D_00436640;

extern s32 D_00436644;

extern s32 func_001A0710(u32);

extern u64 D_004366E8;

extern s32 D_004366E4;

extern s32 D_00435DE0;

extern s32 D_00435DF0;

extern s32 D_00435DE4;

extern s32 D_00435DF8;

extern s32 D_00435E38;

extern s32 func_001AA6F8(void);

extern s32 func_001B32F8(u32, u32);

extern u32 D_004367FC;

extern s32 D_00435DD0;

extern u32 D_004367E4;

extern s64 func_00101740(u32);

extern s64 func_001018B0(s64);

extern s64 func_001B88C8(u64);

extern u32 func_00101958(s64);

extern u32 D_004367E8;

extern u32 D_004367E0;

extern u32 D_004367D8;

extern u32 D_004367C8;

extern u32 D_004367C4;

extern u32 D_00438F4C;

extern u32 D_00438F50;

extern u64 func_0019F5E8(s32, s32, u64, u64, u64, u64);

extern u32 func_001CCBB8(void);

extern s32 D_00435DEC;

extern u32 D_00436A1C;

extern u32 D_00436A20;

extern s32 func_0032CD98(void);

extern u32 D_00435CD4;

extern u32 func_00203008(void);

extern s32 func_002A2330(void);

extern s32 func_00342168(u32);

void func_001A5BB8(s32 arg0) {
    for (; arg0 != 0; arg0 = *(s32 *)(arg0 + 0x24)) {
        func_0019D038(arg0);
    }
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A5BF8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A5CE8);

void func_001A5DD8(u32 *arg0, s32 arg1) {
    if (arg1 != 0) {
        *arg0 = 0x280;
        arg0[1] = 0xa10;
    }
    arg0[2] = 0;
    *(u16 *)(arg0 + 3) = 0xffff;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A5E00);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A5E48);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A5E88);

void func_001A5EB8(s32 arg0) {
    s32 temp_v0;
    u32 *puVar2;

    puVar2 = (u32 *)(arg0 + 0x7c);
    temp_v0 = 0x1f;
    do {
        temp_v0 = temp_v0 - 1;
        *puVar2 = 0;
        puVar2 = puVar2 + -1;
    } while (-1 < temp_v0);
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A5EE8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A5F08);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A5F18);

void func_001A5FA0(s32 *arg0) {
    s32 temp_v0;

    temp_v0 = 0x1f;
    do {
        if (*arg0 != 0) {
            func_003297C8(arg0[0x20]);
            *arg0 = 0;
        }
        temp_v0 = temp_v0 - 1;
        arg0 = arg0 + 1;
    } while (-1 < temp_v0);
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A5FF0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A6078);

void func_001A6128(u32 arg0) {
    func_001A6160();
    func_001A6350(arg0);
    func_001A6528(arg0);
    func_001A6B48(arg0);
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A6160);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A6350);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A6528);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A67E0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A68B8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A6A08);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A6AB8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A6B48);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A6BF8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A6DA8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A6E88);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A7120);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A74F0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A7688);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A76C8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A7738);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A7798);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A7878);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A7A08);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A7A98);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A7B00);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00414D50);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00414D60);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A7C08);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A81F0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A85E0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A8748);

void func_001A87E8(void) {
    if (D_00436644 != 0) {
        func_0032BBB0(D_00436644);
        D_00436644 = 0;
    }
    D_00436640 = (D_00436640 + 1) & 3;
    if (D_00436640 != 3) {
        D_00436644 = func_001A0710(*(u32 *)(D_003B4CF8 + D_00436640 * 4));
    }
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A8848);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A8878);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00414EB0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00414EC8);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00414EE0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A8918);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A8938);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A8A70);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A8BD0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A9130);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A9580);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A96E8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A9740);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A9798);

void func_001A9910(void) {
    D_004366E8 = 1;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A9920);

void func_001A9948(void) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v1 = 0xbe0;
    do {
        temp_v0 = temp_v1 + 1;
        func_0023A130(temp_v1);
        temp_v1 = temp_v0;
    } while (temp_v0 < 0xc00);
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A9988);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A9A30);

void func_001A9AA8(void) {
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A9AB0);

void func_001A9B40(void) {
    s64 temp_v0;

    temp_v0 = func_00101820(0x3f9);
    if (temp_v0 != 0) {
        kwlnTaskDestroyWithHierarchy(*(u32 *)(D_004366E4 + 0x2c4), 1);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A9B80);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A9D70);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A9F30);

void func_001AA2E0(void) {
    func_00209CD0();
    func_001CFB48();
    func_00203B90();
}

u8 func_001AA308(void) {
    return D_004366E4 != 0;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AA318);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AA350);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AA398);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AA400);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AA570);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AA6F8);

u16 func_001AA700(s32 arg0) {
    return *(u16 *)(arg0 + 6);
}

u16 func_001AA708(s32 arg0) {
    return *(u16 *)(arg0 + 10);
}

void func_001AA710(void) {
    func_001188F0();
}

void func_001AA728(void) {
    func_001189D0();
}

void func_001AA740(void) {
    func_001197C0();
}

void func_001AA758(void) {
    func_001198C0();
}

void func_001AA770(void) {
    func_001199C0();
}

void func_001AA788(void) {
    func_001199E8();
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AA7A0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AA7F0);

u16 func_001AA840(s32 arg0) {
    return *(u16 *)(arg0 + 0xe) & 0x7fff;
}

void func_001AA850(void) {
    func_00119728();
}

void func_001AA868(void) {
    func_001197A8();
}

void func_001AA880(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x310) = arg1;
}

void func_001AA888(s32 arg0) {
    *(u32 *)(arg0 + 0x310) = 0xffffffff;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AA898);

s32 func_001AAA08(s32 arg0) {
    s32 temp_v0;

    if ((*(u32 *)(arg0 + 0x110) & 0x400) == 0) {
        temp_v0 = func_001AAA80(*(u8 *)(arg0 + 0x2e4));
        return temp_v0;
    }
    return arg0 + 0x120;
}

s32 func_001AAA40(void) {
    s32 temp_v0;

    temp_v0 = func_0011A2C8();
    return D_00435DD0 + temp_v0 * 0x1c4 + 0xa60;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AAA80);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AAAA8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AABC0);

void func_001AABD8(void) {
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AABE0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AAC50);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AB160);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AB510);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AB880);

void func_001AB8C0(void) {
    func_00119A10();
}

void func_001AB8D8(void) {
    func_00119A78();
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AB8F0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AB9F0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001ABA40);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001ABB10);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001ABCC0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001ABDE8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001ABF00);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001ABF50);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001ABF68);

s32 func_001ABFD8(s32 arg0, s32 arg1) {
    if (arg0 == 0) {
        return D_00435DE0 + arg1 * 0x270;
    }
    return D_00435DF0 + arg1 * 0x270;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AC020);

s32 func_001AC050(s32 arg0, s32 arg1) {
    if (arg0 == 0) {
        return D_00435DE4 + arg1 * 0x74;
    }
    return D_00435DF8 + arg1 * 0x74;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AC098);

u16 func_001AC0E0(s32 arg0) {
    return *(u16 *)(arg0 * 8 + D_00435E38 + 2);
}

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415158);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AC0F8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AC360);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AC510);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AC648);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AC750);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001ACD10);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001ACF68);

void func_001AD090(void) {
    func_00119F68();
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AD0A8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AD118);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AD1C0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AD200);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AD248);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AD310);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AD5B0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AD698);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AD900);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AD978);

u32 func_001ADA10(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    return *(u32 *)(temp_v0 + 0x278);
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001ADA30);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001ADAD0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001ADB30);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001ADBD0);

void func_001ADC48(u32 arg0) {
    u32 temp_v0;
    u32 temp_v1;

    temp_v1 = 0;
    do {
        temp_v0 = temp_v1 + 1;
        func_001ADDD0(arg0, temp_v1);
        temp_v1 = temp_v0;
    } while (temp_v0 < 7);
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001ADC98);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001ADCD0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001ADD30);

void func_001ADDB8(s32 arg0, s32 arg1, u16 arg2) {
    *(u16 *)(arg1 * 6 + arg0 + 0x2e6) = arg2;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001ADDD0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001ADE00);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001ADE18);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001ADFE0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AE2C0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AE358);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AE3A8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AE678);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AE8C0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AEAB8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AEB20);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AEC18);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AED08);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AED48);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AED78);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004152F8);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415308);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415318);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AED98);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AEEA8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AF060);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AF0B0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AF4A0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AFF38);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B0760);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B08A8);

s32 func_001B08F8(s32 arg0) {
    u16 temp_v0;
    s32 temp_v1;

    temp_v0 = *(u16 *)(arg0 + 0x12e) & 0x7fff;
    temp_v1 = 0;
    if ((temp_v0 == 0x80) || (temp_v0 == 0x400)) {
        temp_v1 = (s32)-(u32)*(u16 *)(arg0 + 0x128) / 5;
    }
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B0940);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B09F0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B0B20);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B0B30);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B0C18);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B0C68);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B0D28);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B0DB0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B1090);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B1168);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B1350);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B1640);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B16B0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B17E8);

void func_001B1F78(void) {
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B1F80);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B20C8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B21E0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B2298);

u32 func_001B2380(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B2388);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B2430);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B24C0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B24F8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B2540);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B25F0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B2620);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B2630);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B27E8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B28C8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B2900);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B2970);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B29A8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B29F0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B2AF8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B2CC0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B2D38);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B2D70);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B2F50);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B3188);

u32 func_001B31E8(s32 arg0) {
    return ((*(s32 *)(arg0 + 0x110) >> 0x1a) ^ 1U) & 1;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B3200);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B32F8);

u8 func_001B33C8(u32 arg0) {
    s64 temp_v0;

    temp_v0 = func_001B32F8(arg0, 0);
    return temp_v0 != 0;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B33E8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B3430);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B3510);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B3588);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B35D0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B3610);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B36B8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B3760);

void func_001B3818(u32 arg0) {
    func_001B3760(arg0, 1);
}

void func_001B3830(u32 arg0) {
    func_001B3760(arg0, 0);
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B3848);

void func_001B3900(u32 arg0) {
    func_001B3848(arg0, 1);
}

void func_001B3918(u32 arg0) {
    func_001B3848(arg0, 0);
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B3930);

void func_001B39E8(u32 arg0) {
    func_001B3930(arg0, 1);
}

void func_001B3A00(u32 arg0) {
    func_001B3930(arg0, 0);
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B3A18);

void func_001B3AF8(u32 arg0, u32 arg1) {
    func_001B3A18(arg0, arg1, 1);
}

void __udivdi3(u32 arg0, u32 arg1) {
    func_001B3A18(arg0, arg1, 0);
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B3B28);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B3BE0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B3DD8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B4040);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004157A0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B4210);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B4560);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B45C8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B4600);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B4738);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B47E0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B4828);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B4918);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B4AA0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B4EB8);

u32 func_001B5268(void) {
    func_001AA6F8();
    return 0xffffffff;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B5288);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B52D8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B5440);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B5510);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B5548);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B5600);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B5688);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B5758);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B57B0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B5810);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B5930);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B5978);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415AF0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415B08);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415B18);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B5A20);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B5D78);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B5E98);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B6180);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B6438);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B6A20);

void func_001B6C80(void) {
    func_00328E48(D_004367FC);
    D_004367FC = 0;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B6CA8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B6FC0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B70B8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B7208);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415B80);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415BA0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415BC0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415BD0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415C00);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415C30);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415C58);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415C88);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415C98);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415CD8);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415CE8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B7238);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B72E8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B7370);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B73E8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B7530);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B75F8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B76F0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B7718);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B7830);

void func_001B7900(void) {
    u32 *puVar1;
    s32 temp_v0;

    temp_v0 = 3;
    puVar1 = (u32 *)(D_00435DD0 + 0x16f00);
    do {
        temp_v0 = temp_v0 - 1;
        *puVar1 = 0;
        puVar1 = puVar1 + 1;
    } while (-1 < temp_v0);
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B7940);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B7A00);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B7A38);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B7AE0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B7B20);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B7BF0);

u32 func_001B7DC0(void) {
    s64 temp_v0;

    temp_v0 = func_001B88C8(10);
    if (temp_v0 != 0) {
        temp_v0 = func_001018B0(temp_v0);
        if (temp_v0 == 0) {
            return 0;
        }
        temp_v0 = func_00101740(D_004367E4);
        if (temp_v0 != 0) {
            return 1;
        }
    }
    return 0;
}

void func_001B7E08(void) {
    u8 *puVar1;
    s64 temp_v0;

    temp_v0 = func_00101740(D_004367E4);
    if (temp_v0 != 0) {
        puVar1 = (u8 *)func_00101958(temp_v0);
        *puVar1 = 2;
    }
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B7E40);

u32 func_001B7FB8(void) {
    s64 temp_v0;

    temp_v0 = func_001B88C8(0xb);
    if (temp_v0 != 0) {
        temp_v0 = func_001018B0(temp_v0);
        if (temp_v0 == 0) {
            return 0;
        }
        temp_v0 = func_00101740(D_004367E8);
        if (temp_v0 != 0) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B8000);

u32 func_001B8038(void) {
    u8 *puVar1;
    s64 temp_v0;

    temp_v0 = func_00101740(D_004367E8);
    if (temp_v0 != 0) {
        puVar1 = (u8 *)func_00101958(temp_v0);
        *puVar1 = 2;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B8078);

void func_001B81B0(void) {
    u8 *puVar1;
    s64 temp_v0;

    temp_v0 = func_00101740(D_004367E0);
    if (temp_v0 != 0) {
        puVar1 = (u8 *)func_00101958(temp_v0);
        *puVar1 = 2;
    }
}

u32 func_001B81E8(void) {
    s64 temp_v0;

    temp_v0 = func_001B88C8(9);
    if (temp_v0 != 0) {
        temp_v0 = func_001018B0(temp_v0);
        if (temp_v0 == 0) {
            return 0;
        }
        temp_v0 = func_00101740(D_004367E0);
        if (temp_v0 != 0) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B8230);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B8278);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B82E8);

u32 func_001B8320(void) {
    s64 temp_v0;

    temp_v0 = func_001B88C8(6);
    if (temp_v0 != 0) {
        temp_v0 = func_001018B0(temp_v0);
        if (temp_v0 == 0) {
            return 0;
        }
        temp_v0 = func_00101740(D_004367D8);
        if (temp_v0 != 0) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B8368);

u32 func_001B8538(void) {
    s64 temp_v0;

    temp_v0 = func_001B88C8(1);
    if (temp_v0 != 0) {
        temp_v0 = func_001018B0(temp_v0);
        if (temp_v0 == 0) {
            return 0;
        }
        temp_v0 = func_00101740(D_004367C8);
        if (temp_v0 != 0) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B8580);

u32 func_001B8740(void) {
    s64 temp_v0;

    temp_v0 = func_001B88C8(0);
    if (temp_v0 != 0) {
        temp_v0 = func_001018B0(temp_v0);
        if (temp_v0 == 0) {
            return 0;
        }
        temp_v0 = func_00101740(D_004367C4);
        if (temp_v0 != 0) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B8788);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B88C8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B88E0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B88F8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B8A80);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B8DD0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415D58);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415D88);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415DA0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415DC8);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415DF0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B8E68);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B9158);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B9A10);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B9A90);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415F80);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415F90);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B9C70);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BA1E8);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415FC0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415FD0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416000);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416030);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416060);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BAB90);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BB078);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004160F0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416100);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416128);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416138);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004162F8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BB1E8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BB550);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BB5C0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BB888);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BB8D0);

u8 func_001BB970(s32 arg0) {
    return (~*(u64 *)(arg0 + 0x110) & 0x201) == 0;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BB988);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BB9A0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BB9E8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BBA60);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BBA80);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BC108);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004163A0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004163B0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004163C0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004163F0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BC138);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416428);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BC618);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416448);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416458);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416468);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BC8A8);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416498);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BCFB0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BD618);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BD6B8);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004164B8);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004164C8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BD6E8);

void func_001BD978(void) {
    func_00328E48(D_00438F4C);
    func_00328E48(D_00438F50);
}

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416520);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BD9A0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416560);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BDF20);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BE9E8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BEBD0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BEEF8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BEF28);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BF3C8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BF600);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BF640);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BF690);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BF8B8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BF908);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004165A0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004165B0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004165C0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BF978);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004165E0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004165F0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416600);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416610);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416620);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BFB58);

void func_001BFFD8(void) {
    func_001AA6F8();
    func_00328E48(D_004367F8);
    D_004367F8 = 0;
    func_001B88E0(4, 0);
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C0008);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C0080);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C0118);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C0158);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C01E8);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416650);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C0240);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C05D0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416680);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C0630);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C0828);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C0D78);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C0DD0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C0E40);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004166F0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C0EF0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416740);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C1300);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C14E8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C1520);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C16B0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416780);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416798);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C1A68);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C1F10);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C2390);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C23C0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C2450);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C26E0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C2A50);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C2A98);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C2EA8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C3168);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C35B8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C35F0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C3750);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C3850);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C3978);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C3A38);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C3BB0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C3D20);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C3D70);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C3DB0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C3EC0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C43F8);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004167E0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416800);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416820);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416830);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416840);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416858);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416870);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C4520);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C4900);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C4C58);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C50A0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416898);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C53A0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C5610);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C5868);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C5D10);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C6010);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C6320);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C6648);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004168C8);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004168D8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C68D0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C6B98);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C7020);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C7760);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C7BA8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C7D48);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C7DB8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C7F10);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C7FF8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C8078);

void func_001C80C0(void) {
}

void func_001C80C8(void) {
}

void func_001C80D0(s32 arg0, s32 arg1, u64 arg2, u64 arg3) {
    u64 temp_v0;

    func_0019B8B0(0x13);
    temp_v0 = func_0019F5E8(arg0 << 4, arg1 << 3, 0, arg2, arg3, 0);
    func_0019D550(temp_v0, 1, 0x53);
    func_0019C5B0(temp_v0);
    func_0019B8B0(0xffffffffffffffff);
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C8158);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C81F8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C82A0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C82D8);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416920);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416938);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416948);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C83D0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C8518);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C8768);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C89A0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C8A28);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C8A80);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C91D8);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004169C0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C92A0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C98E8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C9BE8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C9DC8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C9EA0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001CA390);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004169F0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416A00);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416A10);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001CA490);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001CA760);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001CA790);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001CA7E0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001CA820);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001CA858);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001CA8D8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001CA958);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001CA9C0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001CAA30);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001CAB60);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001CB158);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001CB190);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001CB278);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416BB8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001CB498);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416C98);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416CC8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001CB7A8);

u32 func_001CC018(void) {
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416D00);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001CC020);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001CC438);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001CC7C8);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416D28);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001CC8D0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001CC9C0);

void func_001CCB50(u32 arg0) {
    func_001E8018(*(u32 *)((s32)arg0 + 0x10));
    func_001E8018(*(u32 *)((s32)arg0 + 0xc));
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001CCB88);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001CCBB8);

u32 func_001CCBF8(void) {
    u32 *puVar1;

    puVar1 = (u32 *)func_001CCBB8();
    return *puVar1;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001CCC18);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001CCC38);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001CCD50);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001CCD80);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416D58);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416D88);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001CCEB8);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416DE0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416DF0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416E50);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001CD6E0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001CDD38);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416ED0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001CDEC0);

void func_001CE3E8(s32 arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = arg1 * 0xa0 + *(s32 *)(arg0 + 0x18);
    *(s32 *)(temp_v0 + 0xc) = *(s32 *)(temp_v0 + 0x7c) << 4;
    *(s32 *)(temp_v0 + 0x10) = *(s32 *)(temp_v0 + 0x80) << 3;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001CE418);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001CE548);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001CE5C8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001CE838);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001CF0B0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001CF500);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001CF5D0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001CF630);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001CF688);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001CF720);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001CF800);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001CFA60);

void func_001CFAC0(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    *(u32 *)(temp_v0 + 0x2d8) = 0;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001CFAE0);

void func_001CFB20(void) {
    func_001AA6F8();
    func_001C7DB8(0, 8);
}

void func_001CFB48(void) {
    func_001B7238();
}

u32 func_001CFB60(s32 arg0) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    return *(u32 *)(arg0 * 4 + temp_v0 + 0x4e4);
}

void func_001CFB90(void) {
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001CFB98);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001CFC40);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001CFCA8);

void func_001CFEF8(void) {
}

u32 func_001CFF00(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001CFF08);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001CFFC8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D0020);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D0078);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D0140);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D0710);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D08A8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D0FE0);

void func_001D1120(void) {
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D1128);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D1190);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D1200);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D14B0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D1700);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416EF8);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416F08);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416F18);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416F28);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416FA0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D18D8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D22D8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D2798);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D28D8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D2A20);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D2A78);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D2C10);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D2C40);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D2C80);

void func_001D2CD0(u32 arg0) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    *(u32 *)(temp_v0 + 0x230) = arg0;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D2CF8);

void func_001D2D78(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    func_001D2C80(1);
    *(u32 *)(temp_v0 + 0x230) = 0;
}

void func_001D2DB0(void) {
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D2DB8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D2DF0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D2E68);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D2EC8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D2F40);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D3018);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D3098);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D30E0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D34B8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D3520);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D3690);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D37E8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D3898);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D38F0);

void func_001D3978(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    func_001D3018(temp_v0 + 0x318, 0x14);
    func_001D2F40(temp_v0 + 0x368, 0x2d);
    func_001D2F40(temp_v0 + 0x41c, 0xf);
    func_001D30E0();
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D39C8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D3A78);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D3B38);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D3BA8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D3C00);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D3D40);

void func_001D3E00(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    *(u32 *)(temp_v0 + 0x218) = *(u32 *)(temp_v0 + 0x218) | 0xc;
}

void func_001D3E28(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    *(u32 *)(temp_v0 + 0x218) = *(u32 *)(temp_v0 + 0x218) & 0xfffffffb;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D3E58);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D3E90);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D3ED8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D4020);

u32 func_001D4120(s32 *arg0) {
    u8 temp_v0;

    if (*arg0 == 0) {
        temp_v0 = (u8)arg0[2];
    }
    else {
        if ((*(u32 *)(*arg0 + 8) & 0x40) != 0) {
            return 1;
        }
        temp_v0 = (u8)arg0[2];
    }
    func_001D3520(arg0[1], temp_v0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D4160);

void func_001D4200(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    *(u32 *)(temp_v0 + 0x218) = *(u32 *)(temp_v0 + 0x218) & 0xfffffffb;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D4230);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D4290);

u32 func_001D4328(u32 *arg0) {
    func_001D37E8(*arg0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D4348);

void func_001D43B0(s32 arg0) {
    *(u32 *)(arg0 + 8) = *(u32 *)(arg0 + 8) | 1;
}

void func_001D43C0(s32 arg0) {
    *(u32 *)(arg0 + 8) = *(u32 *)(arg0 + 8) & 0xfffffffe;
}

void func_001D43D8(s32 arg0, s32 arg1) {
    u32 temp_v0;

    temp_v0 = *(u32 *)(arg1 + 0x110);
    *(s32 *)(arg0 + 0x18) = arg1;
    if ((temp_v0 & 0x400) != 0) {
        if (0x17f < *(u16 *)(arg1 + 0x124)) {
            temp_v0 = *(u32 *)(arg0 + 8);
            goto LAB_001c8880;
        }
        *(u16 *)(arg0 + 4) =
                  (u16)*(u8 *)(((u32)*(u16 *)(arg1 + 0x124) * 0x14 -
                                                      (u32)*(u16 *)(arg1 + 0x124)) * 4 + D_00435DEC + 0x15);
    }
    temp_v0 = *(u32 *)(arg0 + 8);
LAB_001c8880:
    *(u32 *)(arg0 + 8) = temp_v0 | 8;
}

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00417278);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00417288);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D4438);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D4590);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D46A8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D4820);

void func_001D48E0(void) {
}

void func_001D48E8(void) {
}

void func_001D48F0(s32 arg0) {
    *(u32 *)(arg0 + 8) = *(u32 *)(arg0 + 8) & 0xfffffdff;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D4908);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D4A30);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D4A48);

void func_001D4B90(s32 arg0) {
    *(u32 *)(arg0 + 8) = *(u32 *)(arg0 + 8) & 0xffffffef;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D4BA8);

void func_001D4C98(void) {
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D4CA0);

void func_001D4FE0(void) {
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D4FE8);

void func_001D5330(u32 arg0) {
    func_001AA6F8();
    *(u32 *)((s32)arg0 + 8) = *(u32 *)((s32)arg0 + 8) & 0xfffffffb;
    func_001CAB60(arg0);
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D5368);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D54B8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D5668);

void func_001D5938(s32 arg0) {
    *(u32 *)(arg0 + 8) = *(u32 *)(arg0 + 8) & 0xffffff7f;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D5950);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D5B38);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D5B70);

void func_001D5BD8(void) {
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D5BE0);

void func_001D5CE8(s32 arg0) {
    func_001B7830();
    func_001DF700(arg0 + 0x20);
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x314) = 0xffffffff;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D5D20);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D5E50);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D5EE8);

void func_001D5FA8(void) {
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D5FB0);

void func_001D7228(void) {
}

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00417348);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00417360);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D7230);

void func_001D8C78(void) {
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001D8C80);

void func_001DA1F8(s32 arg0) {
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x110) = *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x110) | 0x4000
    ;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001DA210);

void func_001DA728(s32 arg0) {
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x110) = *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x110) | 0x4000
    ;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001DA740);

void func_001DABC0(s32 arg0) {
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x110) = *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x110) | 0x4000
    ;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001DABD8);

void func_001DACE0(s32 arg0) {
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x110) = *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x110) | 0x4000
    ;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001DACF8);

void func_001DB048(void) {
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001DB050);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001DB258);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001DB440);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001DB518);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001DB5E0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001DBE20);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001DBE70);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001DC278);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001DC2D8);

void func_001DC538(void) {
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001DC540);

void func_001DC7F0(void) {
}

void func_001DC7F8(u64 arg0) {
    u64 temp_v0;

    temp_v0 = func_001D4160(arg0, 0x1194, 1);
    func_001E1580(temp_v0);
    func_001DCF18(arg0, 0x1b);
}

void func_001DC838(void) {
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001DC840);

void func_001DC888(void) {
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001DC890);

void func_001DC8F8(void) {
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001DC900);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001DC9C0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001DCC48);

void func_001DCD80(void) {
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001DCD88);

void func_001DCE58(s32 arg0) {
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x110) = *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x110) | 0x4000
    ;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001DCE70);

void func_001DCEB0(void) {
}

void func_001DCEB8(void) {
}

void func_001DCEC0(void) {
    func_0022F068();
}

void func_001DCED8(u32 arg0) {
    s64 temp_v0;

    temp_v0 = func_0022F180();
    if (temp_v0 == 0) {
        func_001DCF18(arg0, 6);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001DCF18);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00417508);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00417518);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00417528);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00417538);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00417548);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001DCF58);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001DCFE0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001DD060);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001DD108);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001DD148);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001DD1A8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001DD390);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001DD5E8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001DD628);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001DD6D8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001DD810);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001DD9A8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001DDAD0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001DDB60);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001DF700);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001DF7B8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001DF810);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001DF860);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001DFB08);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001DFBE0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001DFC80);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001DFD58);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001DFDC0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001DFE48);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001DFF48);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001DFFE0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E0010);

u32 func_001E00E8(u32 *arg0) {
    if (0 < (s32)arg0[7]) {
        func_001AA880(*arg0, arg0[7]);
        func_001E2758(*arg0);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E0128);

u32 func_001E0200(u32 *arg0) {
    func_001AA888(*arg0);
    func_001E2758(*arg0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E0238);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E02A8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E05B8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E0640);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E06B0);

u32 func_001E0738(s32 arg0) {
    func_0011A118(*(u16 *)(arg0 + 4), 1);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E0760);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E07E8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E0858);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E08E0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E0950);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E09D8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E0B08);

u32 func_001E0B50(s32 arg0) {
    func_0011A0D0(*(u32 *)(arg0 + 4));
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E0B70);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E0BF8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E0CE0);

void func_001E1100(void) {
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E1108);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E1168);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E11C8);

s32 func_001E1228(void) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v0 = func_001AA6F8();
    temp_v1 = 0;
    for (temp_v0 = *(s32 *)(temp_v0 + 0x254); temp_v0 != 0; temp_v0 = *(s32 *)(temp_v0 + 0x58)) {
        temp_v1 = temp_v1 + 1;
    }
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E1278);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E12C8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E1318);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E1368);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E1468);

u32 func_001E14F8(s32 arg0) {
    return *(u32 *)(arg0 + 0x54);
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E1500);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E1580);

void func_001E15E0(void) {
    D_00436A1C = 0;
    D_00436A20 = 0;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E15F0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E1730);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E17B8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E1800);

u32 func_001E1848(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E1850);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E1898);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E1918);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E1958);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E19C8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E1AD8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E1B80);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E1BB8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E1F98);

void func_001E2058(u32 arg0, u32 arg1, u32 arg2) {
    s64 temp_v0;

    temp_v0 = func_0023A170(0xc0f);
    if (temp_v0 != 0) {
        func_0022CD60(arg1, arg2);
        return;
    }
    func_00231B80(arg1, arg2, 0);
}

void func_001E20B8(u32 arg0, u32 arg1, u32 arg2) {
    s64 temp_v0;

    temp_v0 = func_0023A170(0xc0f);
    if (temp_v0 != 0) {
        func_0022CB68(arg1, arg2);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E2110);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E21A0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E2220);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E2298);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00417940);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E22D8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E2758);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E2B60);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E2C00);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E2C78);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E2CD8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E2DA0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E2E20);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E2E58);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E2EE8);

void func_001E2F18(s32 arg0) {
    if ((*(u32 *)(arg0 + 0x110) & 2) != 0) {
        func_003343E8(*(u32 *)(*(s32 *)(*(s32 *)(arg0 + 0x340) + 0x8c) + 0x1c));
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E2F50);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E2F78);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E2FB0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E3040);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E3088);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E3108);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E3120);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E3168);

void func_001E31F0(u32 arg0) {
    s64 temp_v0;

    temp_v0 = func_001E3168();
    if (temp_v0 == 0) {
        func_00207DC0(arg0);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E3230);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E3320);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E33A8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E33E0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E3448);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E34D8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E34F0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E3560);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E35F8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E3628);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E36A0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E3720);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004179E0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E37A8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E3810);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E38F0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E3C28);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E3CB8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E3E20);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E4028);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E40F0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E4378);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E44A8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E44E0);

u32 func_001E4588(u32 *arg0) {
    func_001E2DA0(*arg0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E45A8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E4618);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E4678);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E4700);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E4908);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E49A8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E4A90);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E4B40);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E4C30);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E4CF8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E4DF0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E4EE0);

u32 func_001E4FA0(u32 *arg0) {
    func_001E2220(*arg0);
    func_001E1F98(*arg0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E4FD8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E5048);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E50E0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E5790);

void func_001E5870(s32 arg0) {
    if ((*(u32 *)(*(s32 *)(arg0 + 0xc) + 0x110) & 2) != 0) {
        func_0023CB58(*(u32 *)(*(s32 *)(arg0 + 0xc) + 0x340));
        return;
    }
}

u32 func_001E58A0(u32 *arg0) {
    if ((*(u64 *)(arg0[3] + 0x110) & 0x1000000002) == 0x1000000002) {
        func_0023C870(*(u32 *)(arg0[3] + 0x340), arg0[2], *arg0, arg0[1]);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E58F0);

void func_001E59A0(s32 arg0) {
    if ((*(u32 *)(*(s32 *)(arg0 + 0x14) + 0x110) & 2) != 0) {
        func_0023CB58(*(u32 *)(*(s32 *)(arg0 + 0x14) + 0x340));
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E59D0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E5A10);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E5AB0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E5C08);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E5CB0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E5DA8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E5E40);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E5FF8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E6080);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E61A0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E6228);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E6428);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E64B0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E65C0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E6620);

u32 func_001E66B8(void) {
    func_00208F78();
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E66D8);

u32 func_001E6720(void) {
    func_00209078();
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E6740);

u32 func_001E6790(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E6798);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E67E0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E68F8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E6970);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E6B70);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E6BF8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E6E18);

u32 func_001E6E90(s32 *arg0) {
    s32 temp_v0;

    temp_v0 = *arg0;
    *(u32 *)(temp_v0 + 0x110) = *(u32 *)(temp_v0 + 0x110) & 0xffffffef;
    func_001E3448(temp_v0, temp_v0 + 0x40);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E6EC8);

u32 func_001E6F38(u32 *arg0) {
    func_001E3628(*arg0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E6F58);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E6FC8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E7068);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E70F8);

u32 func_001E7188(void) {
    func_001E2CD8();
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E71A8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E71F0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E7248);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E72B0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E7378);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E7438);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E74A0);

u32 func_001E7528(u32 *arg0) {
    func_001E21A0(*arg0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E7548);

u32 func_001E75B8(u32 *arg0) {
    func_001E2220(*arg0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E75D8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E7648);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E7960);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E7B00);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E7B58);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E7C48);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E7D30);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E7DB0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E7DF0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E7E50);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E7EB0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E7F08);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E7F70);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E7FD8);

void func_001E8018(void) {
    func_00328E48();
}

void func_001E8030(s32 arg0, u32 arg1) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 4);
    *(s32 *)(arg0 + 4) = temp_v0 + 1;
    *(u32 *)(temp_v0 * 4 + *(s32 *)(arg0 + 8)) = arg1;
}

void func_001E8050(s32 arg0) {
    *(u32 *)(arg0 + 4) = 0;
}

u32 func_001E8058(s32 arg0) {
    return *(u32 *)(arg0 + 4);
}

u32 func_001E8060(s32 arg0, s32 arg1) {
    return *(u32 *)(arg1 * 4 + *(s32 *)(arg0 + 8));
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E8078);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E80F8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E8128);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E81A8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E8258);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E8428);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E8510);

u32 func_001E8568(void) {
    return 1;
}

u32 func_001E8570(void) {
    return 1;
}

u32 func_001E8578(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E8580);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E8650);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E8708);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E87A0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E8840);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E88A8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E89E0);

u32 func_001E8B08(u32 *arg0) {
    func_001E8258(arg0[3], *arg0, arg0[1], arg0[2], arg0[4]);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E8B40);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E8BE0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E8C20);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E8C88);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E8D00);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E8E08);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E8EC0);

u32 func_001E9008(u32 arg0) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    func_001E8E08(arg0);
    *(u32 *)(temp_v0 + 0x180) = *(u32 *)(temp_v0 + 0x180) | 0x80000;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E9058);

u32 func_001E90C0(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    func_001E9B80(temp_v0 + 0x70);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E90E8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E9130);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E9410);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E9548);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E9598);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E95C8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E95D0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E9660);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E96C8);

u32 func_001E9798(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    return *(u32 *)(temp_v0 + 0x194);
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E97B8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E97C0);

void func_001E9860(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    *(u32 *)(temp_v0 + 0x184) = 0;
    *(u32 *)(temp_v0 + 0x180) = *(u32 *)(temp_v0 + 0x180) | 0x400;
    func_001E8050(*(u32 *)(temp_v0 + 0x1a8));
}

void func_001E9890(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    *(u32 *)(temp_v0 + 0x180) = *(u32 *)(temp_v0 + 0x180) & 0xffffdfff;
}

void func_001E98C0(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    *(u32 *)(temp_v0 + 0x180) = *(u32 *)(temp_v0 + 0x180) | 0x2000;
}

u32 func_001E98E8(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    return ((*(s32 *)(temp_v0 + 0x180) >> 0xd) ^ 1U) & 1;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E9918);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E99C0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E9A18);

s32 func_001E9A68(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    return temp_v0 + 0x70;
}

void func_001E9A88(void) {
    func_00208D58();
}

void func_001E9AA0(void) {
    func_00208DA0();
}

void func_001E9AB8(s32 arg0) {
    func_00208DE8(*(u32 *)(*(s32 *)(*(s32 *)(arg0 + 0x114) + 0x18) + 0x110) & 0x600);
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E9AE0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00417C78);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00417C88);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00417C98);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00417CA8);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00417CB8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E9B80);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E9CD8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E9DD0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001E9F30);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001EA058);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001EA120);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001EA190);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001EA210);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001EA2B8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001EA338);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001EA3B8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001EA3F8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001EA4D8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001EA598);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001EA620);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001EA650);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001EA688);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001EA6F8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001EA748);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001EA7C8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001EA800);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001EA830);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001EA8B0);

u8 func_001EA940(s32 arg0) {
    return *(s32 *)(arg0 + 0x134) == 0x5f;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001EA950);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001EA9D8);

u32 func_001EAA00(void) {
    return 0;
}

u8 func_001EAA08(s32 arg0) {
    return *(s32 *)(arg0 + 0x134) == 0x1a0;
}

void func_001EAA18(void) {
}

void func_001EAA20(void) {
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001EAA28);

void func_001EAC08(void) {
}

void func_001EAC10(u32 arg0) {
    func_001ECBF8(arg0, arg0);
}

void func_001EAC28(void) {
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001EAC30);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001EADC0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001EAE88);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001EB490);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001EB5B0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001EBB88);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001EBD40);

void func_001EBE28(void) {
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001EBE30);

void func_001EC190(void) {
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001EC198);

void func_001EC2A0(void) {
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001EC2A8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001EC3E8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001EC418);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001EC5F0);

void func_001EC620(u32 arg0) {
    func_001F5018(arg0, arg0);
}

void func_001EC638(u32 arg0) {
    func_001F5230(arg0, arg0);
}

void func_001EC650(u32 arg0) {
    func_001F2758(arg0, (s32)arg0 + 0x30, (s32)arg0 + 0xc0);
}

void func_001EC670(void) {
    func_001F2AE8();
}

void func_001EC688(u32 arg0) {
    s32 temp_v0;

    temp_v0 = (s32)arg0;
    func_001E8030(*(u32 *)(temp_v0 + 0x138), *(u32 *)(*(s32 *)(temp_v0 + 0x114) + 0x18));
    func_001F2E30(arg0, temp_v0 + 0x30, temp_v0 + 0xc0);
}

void func_001EC6C8(void) {
}

void func_001EC6D0(u32 arg0) {
    if ((*(u32 *)(*(s32 *)(*(s32 *)((s32)arg0 + 0x114) + 0x18) + 0x110) & 0x200) != 0) {
        func_001F25F8(arg0, arg0);
        return;
    }
    if (*(s32 *)((s32)arg0 + 0x128) != 0x10) {
        func_001F2740(arg0, arg0);
        return;
    }
}

void func_001EC728(void) {
}

void func_001EC730(s32 arg0) {
    if (*(s32 *)(arg0 + 0x114) != 0) {
        func_001FFD30(*(s32 *)(arg0 + 0x114));
        return;
    }
}

void func_001EC760(void) {
}

void func_001EC768(u32 arg0) {
    func_001F35C8(arg0, (s32)arg0 + 0x30, (s32)arg0 + 0xc0);
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001EC788);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001EC7E8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001EC828);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001EC868);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001ECBF8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001ECC18);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001ECCB0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001ED008);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001ED300);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001ED380);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001ED610);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001ED6C8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001ED9A0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001EDAF8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001EDC38);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001EDFB8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001EE458);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001EE690);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001EEB78);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001EF030);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001EF668);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001EF948);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001EFA30);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001EFEE8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001F01E8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001F02E0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001F0508);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001F0690);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001F0968);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001F0C80);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001F1120);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001F1290);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001F17C8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001F1B00);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001F1F20);

void func_001F20B0(void) {
    func_001ECC18();
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001F20C8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001F2308);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001F25F8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001F2740);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001F2758);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001F2AE8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001F2E30);

void func_001F3228(u32 arg0) {
    func_001F01E8(arg0, arg0);
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001F3240);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001F34C8);

void func_001F34E0(void) {
    func_001F3240();
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001F34F8);

void func_001F35C0(void) {
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001F35C8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001F3888);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001F3C30);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001F3E48);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001F41F0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001F4D70);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00417D70);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00417E30);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00417EF0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00417F30);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004180B0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004180C0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00418240);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00418250);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00418310);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00418320);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00418330);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00418338);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00418398);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004183D8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001F4E30);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001F4F10);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001F5018);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001F5230);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001F52D0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001F5320);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001F5780);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001F5810);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004184D8);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00418568);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001F5868);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001FA480);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001FB908);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001FBAC0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001FC5E0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001FD400);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001FDCA8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001FDD20);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001FDE60);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001FDF18);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001FE068);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001FE5C0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001FEC00);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001FF0F8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001FF570);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001FF5D8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001FF820);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001FF8F0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001FF930);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001FFA08);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001FFAD8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001FFBB0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001FFC70);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001FFD30);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001FFDD0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001FFF68);

void func_001FFFF8(void) {
    u64 temp_v0;
    s32 temp_v1;
    u32 temp_v2;

    temp_v1 = func_001AA6F8();
    temp_v0 = func_0010FCA8();
    temp_v2 = func_00116810(temp_v0);
    *(u32 *)(temp_v1 + 0x228) = temp_v2;
    D_00436AD4 = 0;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00200038);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00200078);

void func_002000D0(void) {
    s64 temp_v0;
    s32 temp_v1;

    func_0023A9E0();
    do {
        temp_v0 = func_0032CD98();
    } while (temp_v0 != 0);
    func_0023A9A8();
    do {
        temp_v0 = func_0032CD98();
    } while (temp_v0 != 0);
    func_00200078();
    temp_v1 = func_001AA6F8();
    *(u32 *)(temp_v1 + 0x218) = *(u32 *)(temp_v1 + 0x218) & 0xfffffffd;
}

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00418C58);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00200138);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_002001F0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00200290);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00200490);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_002004E8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00200568);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_002005B0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_002005F0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_002006B0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_002006F0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00200730);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_002007B0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_002007E8);

void func_00200808(void) {
    func_002006F0(0);
    func_00114068(1);
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00200828);

void func_002008C0(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    if ((((*(u32 *)(temp_v0 + 0x218) & 0x20000) != 0) && ((*(u32 *)(temp_v0 + 0x21c) & 0x20) == 0)) &&
          ((*(u32 *)(temp_v0 + 0x220) & 0x4000000) == 0)) {
        func_001355D8();
        func_00134910();
        func_00134A18();
    }
    func_002007B0();
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00200930);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_002009E0);

void func_00200AD8(void) {
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00200AE0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00200B30);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00200D00);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00200DA0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00200F28);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00200FB8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00201108);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00201268);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_002014A8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00201520);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00201540);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00201578);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_002015D0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00201650);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_002016B0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00201718);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00201738);

void func_00201770(s32 arg0, s32 arg1) {
    if (*(s32 *)(arg0 + 0xc) < arg1) {
        *(s32 *)(arg0 + 0xc) = arg1;
    }
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00201788);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_002017C0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00201828);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00201B68);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00201BD8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00201C38);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00201C98);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00201EA0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00201F08);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00202050);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_002020B0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00202100);

void func_00202288(s32 *arg0) {
    s32 temp_v0;
    s32 temp_v1;

    if (arg0[1] != 0) {
        func_001686F0(arg0[1]);
    }
    temp_v0 = *arg0;
    temp_v1 = arg0[2];
    *(s32 *)(temp_v0 + 4) = *(s32 *)(temp_v0 + 4) - 1;
    *(s32 *)(temp_v1 + 0x334) = *(s32 *)(temp_v1 + 0x334) - 1;
    func_00201BD8(temp_v0);
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_002022E0);

void func_002023A0(s32 *arg0) {
    *(s32 *)(*arg0 + 8) = *(s32 *)(*arg0 + 8) + 1;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_002023B8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00202450);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00202518);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_002025A0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00202648);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_002026E8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00202750);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_002027B8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00202840);

u32 func_002028A8(void) {
    func_001057A8();
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_002028C8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00202908);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00202958);

void func_00202B58(s32 *arg0) {
    s32 temp_v0;
    s32 temp_v1;

    if (arg0[1] != 0) {
        func_001686F0(arg0[1]);
    }
    temp_v0 = *arg0;
    temp_v1 = arg0[2];
    *(s32 *)(temp_v0 + 4) = *(s32 *)(temp_v0 + 4) - 1;
    *(s32 *)(temp_v1 + 0x334) = *(s32 *)(temp_v1 + 0x334) - 1;
    func_00201BD8(temp_v0);
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00202BB0);

u32 func_00202C70(u32 *arg0) {
    func_00105A68(*arg0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00202C90);

u32 func_00202CF8(u8 *arg0) {
    func_00105AB8(*arg0, arg0[1], arg0[2], *(u32 *)(arg0 + 4));
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00202D28);

u32 func_00202DA8(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    *(u32 *)(temp_v0 + 0x218) = *(u32 *)(temp_v0 + 0x218) | 0x40000;
    func_002DCC30();
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00202DE0);

u32 func_00202E28(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    *(u32 *)(temp_v0 + 0x218) = *(u32 *)(temp_v0 + 0x218) & 0xfffbffff;
    func_002DCC68();
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00202E60);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00202EA8);

void func_00202F40(s32 arg0, u16 arg1) {
    func_001684A8(*(u32 *)(arg0 + 0x10), arg1);
}

void func_00202F60(s32 arg0, u16 arg1) {
    func_00168448(*(u32 *)(arg0 + 0x10), arg1);
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00202F80);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00202FA0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00203008);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00203070);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_002030C0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00203138);

void func_00203258(void) {
    func_002DCB58();
}

void func_00203270(void) {
    func_00169608();
    func_002D2CA8();
    D_00435CD4 = D_00435CD4 & 0xdfffffff;
}

void func_002032A8(void) {
    func_002032D8();
    func_002DCCA0();
    func_00169608();
    func_002D2CA8();
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_002032D8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00203318);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00203418);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00203458);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_002034A8);

void func_002037F0(s32 arg0) {
    *(u8 *)(arg0 + 0x10) = 1;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00203800);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00203840);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00203890);

u32 func_00203AB0(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    *(u8 *)(temp_v0 + 0x5b8) = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00203AD8);

u32 func_00203B20(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    *(u8 *)(temp_v0 + 0x5b8) = 1;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00203B48);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00418E58);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00418E70);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00418E88);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00418EA0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00418EB8);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00418ED0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00418EE8);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00418F00);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00418F18);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00418F30);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00418F48);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00418F60);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00418F78);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00418F90);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00418FA8);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00418FC0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00418FD8);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00418FF0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00419008);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00419020);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00419040);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00419060);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00419080);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00419098);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004190B0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004190C8);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004190E0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004190F8);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00419110);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00419128);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00419140);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00419158);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00419170);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00203B90);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00203C78);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00203CD8);

void func_00203D48(s32 arg0, u32 arg1) {
    u32 temp_v0;
    s32 temp_v1;
    u32 *puVar3;

    temp_v1 = func_001AA6F8();
    puVar3 = (u32 *)func_00203008();
    temp_v0 = *puVar3;
    puVar3[5] = arg1;
    *(u32 **)(arg0 * 4 + temp_v1 + 0x4ec) = puVar3;
    *puVar3 = temp_v0 | 10;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00203DA8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00203E18);

void func_00203E90(u32 arg0) {
    func_002D4548(*(u32 *)arg0);
    func_00328E48(arg0);
}

void func_00203EC0(void) {
}

void func_00203EC8(u32 arg0) {
    func_00341E20(arg0, 0x58, 0x3f);
}

void func_00203EE8(u32 arg0) {
    func_00341E20(arg0, 0x319c, 0x3f);
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00203F08);

u8 func_00203FD8(void) {
    s32 temp_v0;

    temp_v0 = func_002A2330();
    return temp_v0 - 2U < 2;
}

void func_00204000(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    if ((*(u32 *)(temp_v0 + 0x218) & 0x10000) != 0) {
        func_002A2388();
        return;
    }
}

void func_00204038(void) {
    func_002A2440();
}

void func_00204050(void) {
    func_002A2440();
    func_00341CD0();
    func_00341CA8();
}

void func_00204078(void) {
    func_00204038();
}

void func_00204090(void) {
    func_00204050();
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_002040A8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_002040E0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00204128);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00204190);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_002041E8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_002042D8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00204358);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_002043C0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00204508);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_002045C0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00204608);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00204678);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_002046A0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_002046C0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004192D8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_002046F0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00204718);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_002047C8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00204820);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00204888);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_002048C8);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004192F8);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00419308);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00419318);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00204958);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00204A88);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00204B00);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00204BD0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00204CC8);

void func_00204D08(void) {
    u64 temp_v0;

    temp_v0 = func_00205018();
    func_001E1580(temp_v0);
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00204D28);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00204D80);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00204E50);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00205018);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_002050D0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_002050F8);

u8 func_00205140(void) {
    s64 temp_v0;

    temp_v0 = func_00342168(0x10000);
    return temp_v0 != 0;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00205160);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_002053C0);

u32 func_00205438(void) {
    u32 temp_v0;
    s64 temp_v1;

    temp_v1 = func_002A2928();
    temp_v0 = 1;
    if (temp_v1 != 0) {
        if (temp_v1 == 2) {
            func_002A2998();
            temp_v0 = 0;
        }
        else {
            temp_v0 = 0;
        }
    }
    return temp_v0;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00205478);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_002054B8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_002055E0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00205660);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00205758);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00205838);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00205890);

u32 func_00205930(void) {
    func_002040A8(0x1c);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00205950);

u32 func_00205990(void) {
    func_00204038();
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_002059B0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_002059F0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00205CC8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00206060);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00206090);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_002060B0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_002061B0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00206370);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00206570);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00206840);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00206970);

void func_00206C10(void) {
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00206C18);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00206EA8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00207268);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00207438);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_002076E0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00207728);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_002077C0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_00207958);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00419540);
