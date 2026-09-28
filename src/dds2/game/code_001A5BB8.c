#include "common.h"

extern u32 D_004367F8;

extern s32 dds3FindEntryIndex(void);

extern s32 func_001AAA80(s32);

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

extern u32 func_00101958(s64);

extern u32 D_004367E8;

extern u32 D_004367E0;

extern u32 D_004367D8;

extern u32 D_004367C8;

extern u32 D_004367C4;

extern u32 D_00438F4C;

extern u32 D_00438F50;

extern s8 D_0037F530[];

extern u8 D_003B4DC0[];

extern s32 D_00435E00;

extern void *D_003B4E88[];

typedef struct UiObject {
    u8 unk_00[0x110];
    u32 flags;
    u8 unk_114[0x10];
    u16 index;
    u16 currentValue;
    u16 maximumValue;
    u8 unk_12A[4];
    u16 statusFlags;
} UiObject;

extern s32 D_003B4F70[];

extern s32 D_00435E20;

extern s32 D_004367F4;

extern s32 func_001B88C8(s32);

extern s32 D_003B6928[];

extern s32 D_00435DEC;

extern s32 D_00435E1C;

extern char D_00415158[];

extern s32 func_0020D128(const char *, ...);

extern s8 D_0037F550[];

extern void *D_003B4E40[];

extern s32 D_003B4F78[];

extern s32 D_003B4F74[];

extern f32 D_003B4E28[];
extern s32 func_001AD0A8(s32, s32);
extern s32 mdlFlagTest(s32);
extern u32 func_002C55C0(s32);
extern s8 *D_00435E24;
extern s32 func_001AD1C0(s32, u32);
extern s32 eventCheckValueThreshold(s32, s32);
extern void mdlFlagSet(s32);
extern void mdlFlagClear(s32);

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

void func_001A5EE8(s32 arg0, s32 arg1) {
    if (arg1 == 0) {
        *(u8 *)arg0 = 0;
    }
    *(u16 *)(arg0 + 2) = 0;
    *(u16 *)(arg0 + 6) = 0;
    *(u16 *)(arg0 + 4) = 0x40;
    *(u32 *)(arg0 + 8) = 0;
}

void func_001A5F08(s32 arg0, s16 arg1, s16 arg2, s16 arg3) {
    *(s16 *)(arg0 + 2) = arg1;
    *(s16 *)(arg0 + 4) = arg2;
    *(s16 *)(arg0 + 6) = arg3;
}

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

INCLUDE_ASM(const s32, "game/code_001A5BB8", nbSoundVisitQueuedResources);

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

s32 func_001A8848(void) {
    if (D_0037F530[0] < 0) {
        func_001A87E8();
    }
    return 0;
}

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
        mdlFlagClear(temp_v1);
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

s32 func_001AA6F8(void) {
    return D_004366E4;
}

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

    temp_v0 = dds3FindEntryIndex();
    return D_00435DD0 + temp_v0 * 0x1c4 + 0xa60;
}

s32 func_001AAA80(s32 index) {
    return D_00435DD0 + index * 0x1c4 + 0xa60;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AAAA8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AABC0);

void func_001AABD8(void) {
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AABE0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AAC50);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AB160);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AB510);

s32 getEntryFlagsUnlessDisabled(s32 entry) {
    if ((*(u16 *)entry & 4) != 0) {
        return 0;
    }
    return *(s32 *)(D_00435DEC + *(u16 *)(entry + 4) * 76);
}

void func_001AB8C0(void) {
    func_00119A10();
}

void func_001AB8D8(void) {
    func_00119A78();
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AB8F0);

s32 func_001AB9F0(s32 battler, s32 slot) {
    s32 value = D_00435E24[slot * 16 - 0x1aa4];
    if (func_001AD1C0(battler + 0x120, 0xe4) && (u32)value >= 2) {
        value--;
    }
    return value;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001ABA40);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001ABB10);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001ABCC0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001ABDE8);

s8 func_001ABF00(s32 object, s32 index) {
    if (index == 0 && (*(u32 *)(object + 0x110) & 0x400) != 0) {
        return *(s8 *)(D_00435DEC + *(u16 *)(object + 0x124) * 76 + 0x46);
    }
    return *(s8 *)(D_00435E1C + index * 2);
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001ABF50);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001ABF68);

s32 func_001ABFD8(s32 arg0, s32 arg1) {
    if (arg0 == 0) {
        return D_00435DE0 + arg1 * 0x270;
    }
    return D_00435DF0 + arg1 * 0x270;
}

s32 func_001AC020(s32 arg0, s32 arg1) {
    if (arg0 == 0) {
        return (s32)D_003B4DC0;
    }
    return D_00435E00 + arg1 * 24;
}

s32 func_001AC050(s32 arg0, s32 arg1) {
    if (arg0 == 0) {
        return D_00435DE4 + arg1 * 0x74;
    }
    return D_00435DF8 + arg1 * 0x74;
}

s32 func_001AC098(s32 index) {
    u16 item = *(u16 *)(D_00435E38 + index * 8 + 2);
    func_0020D128(D_00415158, index, item);
    return item;
}

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

s32 func_001AD1C0(s32 status, u32 value) {
    if (*(u16 *)status & 0x20) {
        return 0;
    }
    return func_002C55C0(status) == value;
}

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

f32 func_001AED08(void) {
    u16 index = *(u16 *)(func_001AA6F8() + 0x47c);
    if (index > 4) {
        index = 4;
    }
    return D_003B4E28[index];
}

s32 getSoundResourceForIndex(s32 index) {
    s8 resource = *(s8 *)(D_00435E1C + index * 2);
    if (resource < 0) {
        return 0;
    }
    return (s32)D_003B4E40[resource];
}

void *func_001AED78(s32 arg0) {
    return D_003B4E88[*(u16 *)(arg0 + 0x124)];
}

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004152F8);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415308);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415318);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AED98);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AEEA8);

void func_001AF060(void) {
    if (eventCheckValueThreshold(0x53, 1) || eventCheckValueThreshold(0x54, 1)) {
        mdlFlagSet(0xa20);
    } else {
        mdlFlagClear(0xa20);
    }
}

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

f32 func_001B0B20(void) {
    return 1.5f;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B0B30);

u8 func_001B0C18(s32 object, s32 index) {
    if (index == 0) {
        if ((*(u32 *)(object + 0x110) & 0x400) != 0) {
            return *(u8 *)(D_00435DEC + *(u16 *)(object + 0x124) * 76 + 0x48);
        }
        return 12;
    }
    return 12;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B0C68);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B0D28);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B0DB0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B1090);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B1168);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B1350);

s32 func_001B1640(s32 first, s32 second, s32 other, s32 variant, s32 mode) {
    s32 flags;
    if (mode != 1) {
        return 0;
    }
    flags = func_001194E8(other, first + 0x120, second + 0x120);
    if ((*(u16 *)(second + 0x12E) & 8) != 0 && variant == 2) {
        flags |= 8;
    }
    return flags;
}

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

s32 func_001B24C0(UiObject *object) {
    return object->currentValue * 100 / object->maximumValue < 26;
}

s32 func_001B24F8(UiObject *object, s32 delta) {
    s32 value = object->currentValue + delta;
    if (value <= 0) {
        return 1;
    }
    return value * 100 / object->maximumValue < 26;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B2540);

s32 func_001B25F0(UiObject *object) {
    if ((object->flags & 0x400) != 0) {
        if (object->index >= 0x100) {
            return 0;
        }
    }
    return 1;
}

s32 func_001B2620(UiObject *object) {
    if ((object->statusFlags & 0x1000) != 0) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B2630);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B27E8);

s32 func_001B28C8(s32 arg0) {
    u16 temp_v0;

    temp_v0 = *(u16 *)(D_00435E20 + arg0 * 56 + 0x2e);
    return D_003B4F70[temp_v0 * 3];
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B2900);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B2970);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B29A8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B29F0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B2AF8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B2CC0);

s32 func_001B2D38(s32 arg0) {
    if ((*(u16 *)(arg0 + 0x12e) & 0x40) != 0) {
        return 0;
    }
    return (getEntryFlagsUnlessDisabled(arg0 + 0x120) & 0x40) < 1;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B2D70);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B2F50);

s32 func_001B3188(s32 object) {
    u8 *status = (u8 *)(D_00435DEC + *(u16 *)(object + 0x124) * 76 + 0x3E);
    u32 i;
    for (i = 0; i < 2; i++) {
        if (*status++ != 0) {
            return 1;
        }
    }
    return 0;
}

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

f32 func_001B3588(s32 unused0, s32 unused1, s32 index) {
    s32 category = *(u16 *)(D_00435E20 + index * 56 + 0x1A);
    if (category < 14) {
        if (category >= 12) {
            return 1.0f;
        }
    }
    return 0.0f;
}

f32 func_001B35D0(s32 unused0, s32 unused1, s32 index) {
    return (f32)*(u16 *)(D_00435E20 + index * 56 + 0x22) / 100.0f;
}

s32 func_001B3610(s32 unused, u32 flags, u32 otherFlags, u32 value, s32 index) {
    s32 entry;
    u16 code;
    if (flags & 0x20000) {
        return 0x1194;
    }
    if (flags & 0x40000) {
        return 0x1194;
    }
    if (flags & 0x10000) {
        return value + 100;
    }
    if (flags & 2) {
        return value + 100;
    }
    if (flags & 4) {
        entry = index * 56 + D_00435E20;
        code = *(u16 *)(entry + 0x16);
        if (code != 8 && code != 10 && *(u8 *)(entry + 2) != 2 &&
            *(s32 *)(entry + 0x30) != 4) {
            return value + 100;
        }
    }
    if (otherFlags & 4) {
        return value >> 1;
    }
    if (otherFlags & 2) {
        return value >> 1;
    }
    return value;
}

s32 func_001B36B8(u32 flags, u32 otherFlags, s32 index) {
    s32 entry;
    u16 code;
    if (flags & 0x20000) {
        return 1;
    }
    if (flags & 0x40000) {
        return 1;
    }
    if (flags & 0x10000) {
        return 1;
    }
    if (flags & 2) {
        return 1;
    }
    if (flags & 4) {
        entry = index * 56 + D_00435E20;
        code = *(u16 *)(entry + 0x16);
        if (code != 8 && code != 10 && *(u8 *)(entry + 2) != 2 &&
            *(s32 *)(entry + 0x30) != 4) {
            return 1;
        }
    }
    if (otherFlags & 4) {
        return 3;
    }
    if (otherFlags & 2) {
        return 2;
    }
    return 1;
}

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

s32 func_001B45C8(s32 object) {
    if (func_001AD0A8(object, 0x279)) {
        return 1;
    }
    return mdlFlagTest(0x820) != 0;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B4600);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B4738);

s32 func_001B47E0(s32 object) {
    if ((*(u32 *)(object + 0x110) & 0x400) == 0) {
        return 0;
    }
    return (*(s32 *)(D_00435DEC + *(u16 *)(object + 0x124) * 76) & 0x100) > 0;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B4828);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B4918);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B4AA0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B4EB8);

u32 func_001B5268(void) {
    func_001AA6F8();
    return 0xffffffff;
}

void func_001B5288(UiObject *object, s32 resource) {
    object->flags &= ~0x20;
    object->statusFlags &= ~0x4080;
    *(u32 *)(resource + 0x28) &= ~1;
    *(u32 *)(resource + 0x28) &= ~2;
    if (object->currentValue == 0) {
        object->currentValue = 1;
    }
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B52D8);

s32 func_001B5440(s32 object, s32 other, s32 offset, s32 index) {
    s32 (*predicate)(s32, s32, s32);
    s32 table;
    predicate = *(s32 (**)(s32, s32, s32))(func_001AA6F8() + 0x6cc);
    if (predicate != 0 && !predicate(object, other, index)) {
        return 0;
    }
    if (index == 0x91) {
        return 0;
    }
    table = func_001ABFD8(*(s32 *)(object + 0xc4), *(s32 *)(object + 0xc8));
    if (*(s16 *)(table + offset * 20 + 0x2c) != 2) {
        return 0;
    }
    if (index != 0 && *(u8 *)(D_00435E20 + index * 56 + 8) != 0) {
        return 0;
    }
    return 1;
}

s32 func_001B5510(s32 object) {
    s32 value;
    if (*(s32 *)(object + 0xdc) != 1) {
        return 1;
    }
    value = *(s32 *)(object + 0xe0);
    if (value == 0x39 || value == 0x126) {
        return 0;
    }
    return 1;
}

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

void func_001B7208(s32 arg0, s16 arg1, s32 arg2, s32 arg3, s32 arg4, f32 arg5) {
    *(u8 *)(arg0 + 0) = 1;
    *(u8 *)(arg0 + 0x28) = arg4 * 8 + 0x18;
    *(u16 *)(arg0 + 2) = arg1;
    *(f32 *)(arg0 + 4) = arg5;
    *(u32 *)(arg0 + 0x18) = arg2;
    *(u32 *)(arg0 + 0x1c) = arg3;
    *(u32 *)(arg0 + 0x24) = 0;
}

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

void func_001B76F0(void) {
    s32 temp_v0;
    s32 buf[4];

    temp_v0 = func_001AA6F8();
    func_001CF720(temp_v0, buf);
}

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

s32 func_001B8000(void) {
    if (func_001B7FB8() == 0) {
        return 0x80;
    }
    return *(s8 *)func_00101958(func_00101740(D_004367E8));
}

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

void func_001B82E8(void) {
    func_001AA6F8();
    func_00101958(func_001B88C8(7));
    *(u8 *)(D_004367F4 + 0x48) = 0;
}

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

s32 func_001B88C8(s32 arg0) {
    s32 temp_v0;

    temp_v0 = D_004367F4 + arg0 * 4;
    return *(s32 *)temp_v0;
}

void func_001B88E0(s32 arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = D_004367F4 + arg0 * 4;
    *(s32 *)temp_v0 = arg1;
}

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

void func_001BBA60(s32 arg0) {
    *(u32 *)(arg0 + 4) = 1;
    *(u32 *)(arg0 + 12) = 0x80;
    *(u32 *)(arg0 + 16) = 0;
    *(u32 *)(arg0 + 0) = 0;
}

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

s32 func_001C0008(void) {
    s32 i;
    s32 *flags;
    if (mdlFlagTest(0xb8f)) {
        return 0;
    }
    flags = (s32 *)(D_00435DD0 + 0x16f00);
    for (i = 0; i < 4; i++) {
        if (flags[i]) {
            return 0;
        }
    }
    return 1;
}

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

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004167E0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416800);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416820);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416830);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436630);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436638);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436640);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436644);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436648);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436650);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436658);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436660);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436668);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436670);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436678);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436680);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436688);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436690);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436698);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004366A0);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004366A8);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004366B0);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004366B4);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004366B8);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004366C0);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004366C8);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004366D0);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004366D8);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004366E0);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004366E4);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004366E8);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004366F0);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004366F8);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436700);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436708);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436710);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436718);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436720);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436728);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436730);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436738);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436740);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436748);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436750);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436758);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436760);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436768);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436770);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436778);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436780);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436788);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436790);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436798);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004367A0);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004367A8);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004367B0);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004367B8);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004367BC);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004367C0);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004367C4);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004367C8);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004367CC);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004367D0);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004367D4);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004367D8);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004367DC);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004367E0);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004367E4);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004367E8);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004367EC);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004367F0);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004367F4);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004367F8);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004367FC);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436800);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436801);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436804);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436808);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436810);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436818);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436820);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436828);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436830);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436838);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436840);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436848);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436850);

