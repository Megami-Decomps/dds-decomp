#include "common.h"

extern s64 func_0026C768(void);

extern s64 func_002C4038(s32, s32 *, u64, u64);

extern void func_002C42B0(s32, s32);

extern s32 func_00101958();

extern void func_0026C900(void);

extern void func_002686F0(s32);

INCLUDE_ASM(const s32, "game/code_00269978", func_00269978);

void func_00269AF8(u32 arg0, s32 arg1) {
    u32 temp_v0;

    temp_v0 = *(u32 *)(arg1 + 0xcc);
    *(u32 *)(arg1 + 0xcc) = arg0;
    *(u32 *)(arg1 + 0xd0) = temp_v0;
}

INCLUDE_ASM(const s32, "game/code_00269978", func_00269B08);

INCLUDE_ASM(const s32, "game/code_00269978", func_00269B80);

u32 func_00269C48(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00269978", func_00269C50);

INCLUDE_ASM(const s32, "game/code_00269978", func_00269E98);

void func_00269F28(s32 arg0) {
    s32 temp_v0 = func_00101958();

    func_0026C900();
    func_002C4038(temp_v0 + 8, temp_v0 + 0x54, 2, arg0);
}

INCLUDE_ASM(const s32, "game/code_00269978", func_00269F70);

INCLUDE_ASM(const s32, "game/code_00269978", func_00269FC8);

INCLUDE_ASM(const s32, "game/code_00269978", func_0026A048);

void func_0026A138(s32 arg0) {
    s32 temp_v0 = func_00101958();

    func_002C4038(temp_v0 + 8, temp_v0 + 0x54, 1, arg0);
}

void func_0026A170(s32 arg0) {
    s32 temp_v0 = func_00101958();

    func_0026C900();
    func_002C4038(temp_v0 + 8, temp_v0 + 0x54, 2, arg0);
}

INCLUDE_ASM(const s32, "game/code_00269978", func_0026A1B8);

INCLUDE_ASM(const s32, "game/code_00269978", func_0026A258);

INCLUDE_ASM(const s32, "game/code_00269978", func_0026A2E0);

INCLUDE_ASM(const s32, "game/code_00269978", func_0026A3F8);

void func_0026A468(s32 arg0) {
    s32 temp_v0 = func_00101958();

    func_0026C900();
    func_002C4038(temp_v0 + 8, temp_v0 + 0x54, 2, arg0);
}

INCLUDE_ASM(const s32, "game/code_00269978", func_0026A4B0);

INCLUDE_ASM(const s32, "game/code_00269978", func_0026A528);

INCLUDE_ASM(const s32, "game/code_00269978", func_0026A598);

INCLUDE_ASM(const s32, "game/code_00269978", func_0026A728);

INCLUDE_ASM(const s32, "game/code_00269978", func_0026A808);

void func_0026A890(s32 arg0) {
    s32 temp_v0 = func_00101958();

    func_0026C900();
    func_002C4038(temp_v0 + 8, temp_v0 + 0x54, 2, arg0);
}

u32 func_0026A8D8(void) {
    s32 temp_v0;

    temp_v0 = func_00101958();
    func_002B8988(*(u32 *)(temp_v0 + 0x78));
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00269978", func_0026A900);

INCLUDE_ASM(const s32, "game/code_00269978", func_0026A998);

void func_0026AA78(s32 arg0) {
    s32 temp_v0 = func_00101958();

    func_0026C900();
    func_002C4038(temp_v0 + 8, temp_v0 + 0x54, 2, arg0);
}

INCLUDE_ASM(const s32, "game/code_00269978", func_0026AAC0);

INCLUDE_ASM(const s32, "game/code_00269978", func_0026AB38);

INCLUDE_ASM(const s32, "game/code_00269978", func_0026ABB0);

INCLUDE_ASM(const s32, "game/code_00269978", func_0026AC90);

void func_0026AD00(s32 arg0) {
    s32 temp_v0 = func_00101958();

    func_002C4038(temp_v0 + 8, temp_v0 + 0x54, 2, arg0);
}

INCLUDE_ASM(const s32, "game/code_00269978", func_0026AD38);

u32 func_0026ADC0(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00269978", func_0026ADC8);

INCLUDE_ASM(const s32, "game/code_00269978", func_0026AEB0);

void func_0026AF20(s32 arg0) {
    s32 temp_v0 = func_00101958();

    func_0026C900();
    func_002C4038(temp_v0 + 8, temp_v0 + 0x54, 2, arg0);
}

INCLUDE_ASM(const s32, "game/code_00269978", func_0026AF68);

s64 func_0026AFE0(u64 arg0) {
    s32 temp_v0;
    s64 temp_v1;
    s32 *piVar3;

    temp_v0 = func_00101958();
    piVar3 = (s32 *)(temp_v0 + 0x54);
    temp_v1 = func_002C4038(temp_v0 + 8, piVar3, 0, arg0);
    if (temp_v1 == 0) {
        if ((*piVar3 == 0) && (temp_v1 = func_0026C768(), temp_v1 == 0)) {
            func_002C42B0(piVar3, *(u32 *)(temp_v0 + 0x58));
        }
        temp_v1 = 0;
    }
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_00269978", func_0026B068);

void func_0026B0D8(s32 arg0) {
    s32 temp_v0 = func_00101958();

    func_0026C900();
    func_002C4038(temp_v0 + 8, temp_v0 + 0x54, 2, arg0);
}

INCLUDE_ASM(const s32, "game/code_00269978", func_0026B120);

INCLUDE_ASM(const s32, "game/code_00269978", func_0026B1C8);

INCLUDE_ASM(const s32, "game/code_00269978", func_0026B260);

INCLUDE_ASM(const s32, "game/code_00269978", func_0026B358);

void func_0026B3F8(s32 arg0) {
    s32 temp_v0 = func_00101958();

    func_002C4038(temp_v0 + 8, temp_v0 + 0x54, 2, arg0);
}

INCLUDE_ASM(const s32, "game/code_00269978", func_0026B430);

u32 func_0026B4A0(void) {
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_00269978", D_00424FF8);

INCLUDE_ASM(const s32, "game/code_00269978", func_0026B4A8);

void func_0026B5F8(s32 arg0) {
    s32 temp_v0 = func_00101958();

    func_002686F0(temp_v0);
    func_002C4038(temp_v0 + 8, temp_v0 + 0x54, 1, arg0);
}

void func_0026B648(s32 arg0) {
    s32 temp_v0 = func_00101958();

    func_002C4038(temp_v0 + 8, temp_v0 + 0x54, 2, arg0);
}
