#include "common.h"

extern u32 D_003BAAB4;

extern s32 func_002E83F8(u32, u32);

extern s32 func_0011A598(void);

extern s32 D_003BAA68;

INCLUDE_ASM(const s32, "game/code_00119900", func_00119900);

INCLUDE_ASM(const s32, "game/code_00119900", func_001199B0);

INCLUDE_ASM(const s32, "game/code_00119900", func_00119A00);

INCLUDE_ASM(const s32, "game/code_00119900", func_00119A68);

INCLUDE_ASM(const s32, "game/code_00119900", func_00119AA8);

INCLUDE_ASM(const s32, "game/code_00119900", func_00119AF8);

INCLUDE_ASM(const s32, "game/code_00119900", func_00119B08);

INCLUDE_ASM(const s32, "game/code_00119900", func_00119CF0);

INCLUDE_ASM(const s32, "game/code_00119900", func_00119E00);

INCLUDE_ASM(const s32, "game/code_00119900", func_00119E88);

void func_00119EF8(s32 arg0, s32 arg1) {
    *(s32 *)(arg0 + 0x10) = *(s32 *)(arg0 + 0x10) + arg1;
}

INCLUDE_ASM(const s32, "game/code_00119900", func_00119F08);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011A038);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011A0C0);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011A158);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011A238);

u16 func_0011A568(s32 arg0) {
    return *(u16 *)(arg0 * 8 + D_003BAA68 + 2);
}

INCLUDE_ASM(const s32, "game/code_00119900", func_0011A580);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011A598);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011A5E8);

u8 func_0011A968(void) {
    s64 temp_v0;

    temp_v0 = func_0011A598();
    return temp_v0 != 0;
}

INCLUDE_ASM(const s32, "game/code_00119900", func_0011A988);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011A9C8);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011AA28);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011AE78);

u32 func_0011B140(void) {
    return 0;
}

u32 func_0011B148(void) {
    return 1;
}

u32 func_0011B150(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00119900", func_0011B158);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011B308);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011B418);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011B438);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011B4C8);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011B528);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011B640);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011B6A8);

void func_0011B7B8(s32 arg0) {
    s32 temp_v0;

    temp_v0 = func_002E83F8(0, 4);
    *(s32 *)(arg0 + 0x194) = 0x12 - temp_v0;
}

INCLUDE_ASM(const s32, "game/code_00119900", func_0011B7F0);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011B878);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011B908);

u32 func_0011B938(void) {
    return D_003BAAB4;
}

void func_0011B940(void) {
    func_0010BD20(D_003BAAB4);
    D_003BAAB4 = 0;
}

INCLUDE_ASM(const s32, "game/code_00119900", func_0011B968);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011B990);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011B9B8);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011B9E0);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011BA08);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011BA30);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011BA58);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011BB08);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011BBB8);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011BC60);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011BD08);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011BD40);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011BD78);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011BE00);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011BE90);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011BED0);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011BF50);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011BFE8);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011C070);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011C0B0);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011C0F0);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011C118);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011C150);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011C1B8);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011C208);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011C258);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011C2A8);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011C310);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011C350);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011C390);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011C3C0);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011C3F0);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011C4C0);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011C4F0);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011C520);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011C550);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011C580);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011C5D8);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011C610);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011C700);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011C790);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011C990);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011CAB0);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011CB90);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011CE18);

void func_0011CE30(void) {
}

void func_0011CE38(void) {
}

void func_0011CE40(void) {
}

void func_0011CE48(void) {
}

INCLUDE_ASM(const s32, "game/code_00119900", func_0011CE50);
