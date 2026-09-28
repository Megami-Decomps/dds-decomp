#include "common.h"

extern u8 D_00368C40[];

extern s32 D_003BAA00;
extern s8 D_003BC39C;


extern s32 func_00101938(u32);

extern s64 func_00241B58(void);
INCLUDE_ASM(const s32, "game/code_00242608", func_00242608);

void func_00242698(void) {
    s64 temp_v0;

    temp_v0 = func_00241B58();
    if (temp_v0 != 0) {
        kwlnTaskDestroyWithHierarchy(temp_v0, 0);
        return;
    }
}

void func_002426D0(void) {
    s64 temp_v0;

    while (temp_v0 = func_00101938(0x3ec), temp_v0 != 0) {
        kwlnTaskDestroyWithHierarchy(temp_v0, 0);
    }
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00242708);

INCLUDE_ASM(const s32, "game/code_00242608", func_00242780);

INCLUDE_ASM(const s32, "game/code_00242608", func_00242950);

INCLUDE_ASM(const s32, "game/code_00242608", func_002429F0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00242BD0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00242C30);

INCLUDE_ASM(const s32, "game/code_00242608", func_00242E28);

INCLUDE_ASM(const s32, "game/code_00242608", func_00242E70);

INCLUDE_ASM(const s32, "game/code_00242608", func_00242F20);

INCLUDE_ASM(const s32, "game/code_00242608", func_00242F78);

INCLUDE_ASM(const s32, "game/code_00242608", func_00243048);

INCLUDE_ASM(const s32, "game/code_00242608", func_002432D0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00243390);

INCLUDE_ASM(const s32, "game/code_00242608", func_00243440);
INCLUDE_ASM(const s32, "game/code_00242608", func_00243460);

INCLUDE_ASM(const s32, "game/code_00242608", func_002434E0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00243558);

INCLUDE_ASM(const s32, "game/code_00242608", func_00243608);

void func_002437E0(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x2034);
    if (temp_v0 != 0) {
        *(u32 *)(temp_v0 + 0x28) = 0;
        while (temp_v0 = *(s32 *)(temp_v0 + 0x7c), temp_v0 != 0) {
            *(u32 *)(temp_v0 + 0x28) = 0;
        }
    }
    *(u32 *)(arg0 + 0x240c) = 0;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00243818);

INCLUDE_ASM(const s32, "game/code_00242608", func_00243928);

void func_00243A18(s32 arg0) {
    func_00243818();
    if (*(s32 *)(arg0 + 0x23cc) == 1) {
        func_00134CD8();
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00243A58);

void func_00243AA8(s32 arg0) {
    func_00194920(*(u32 *)(arg0 + 0x2410));
    *(u32 *)(arg0 + 0x2410) = 0;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00243AD8);

void func_00243B00(s32 arg0) {
    if ((*(s32 *)(arg0 + 0x2430) == 0) || (*(s32 *)(arg0 + 0x2430) == 5)) {
        *(u32 *)(arg0 + 0x2430) = 1;
    }
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00243B28);

INCLUDE_ASM(const s32, "game/code_00242608", func_00243BF0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00243CC8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00243D48);

void func_00243EC8(s32 arg0) {
    func_00243D48(*(u32 *)(arg0 + 0x2438));
}

void func_00243EE0(void) {
}

void func_00243EE8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x243c) = (*(u32 *)(arg0 + 0x243c) & 0xfffffffc) | (arg1 & 3);
}

u32 func_00243F08(s32 arg0) {
    return *(u32 *)(arg0 + 0x243c) & 3;
}

void func_00243F18(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x243c) = (*(u32 *)(arg0 + 0x243c) & 0xfffffff3) | ((arg1 & 3) << 2);
}

u32 func_00243F38(s32 arg0) {
    return (*(u32 *)(arg0 + 0x243c) & 0xc) >> 2;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00243F48);

INCLUDE_ASM(const s32, "game/code_00242608", func_00244058);

INCLUDE_ASM(const s32, "game/code_00242608", func_002440A8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00244110);

INCLUDE_ASM(const s32, "game/code_00242608", func_00244180);

INCLUDE_ASM(const s32, "game/code_00242608", func_002441E8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00244258);

INCLUDE_ASM(const s32, "game/code_00242608", func_002442D0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AF3D0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00244320);

INCLUDE_ASM(const s32, "game/code_00242608", func_00244360);

INCLUDE_ASM(const s32, "game/code_00242608", func_00244380);

INCLUDE_ASM(const s32, "game/code_00242608", func_002443F8);

void func_002444D0(s32 *arg0) {
    arg0[27] = func_002443F8(D_00368C40, 3, arg0);
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00244508);

INCLUDE_ASM(const s32, "game/code_00242608", func_002445E0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00244658);

INCLUDE_ASM(const s32, "game/code_00242608", func_00244740);

INCLUDE_ASM(const s32, "game/code_00242608", func_002447D8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00244848);

s32 func_00244898(void) {
    u16 flags;
    u16 *entry;
    s32 remaining;
    s32 count;

    count = 0;
    remaining = 4;
    entry = (u16 *)(D_003BAA00 + 0xa60);
    do {
        flags = *entry;
        entry += 0xd2;
        remaining--;
        count += flags & 1;
    } while (remaining >= 0);
    return count;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_002448D0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00244970);

INCLUDE_ASM(const s32, "game/code_00242608", func_002449F0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00244AB8);

s32 func_00244AF8(void) {
    s32 state = D_003BC39C;
    if (state == 1) {
        return 1;
    }
    if (state < 2) {
        return 0;
    }
    if (state == 2) {
        D_003BC39C = 0;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00244B30);

INCLUDE_ASM(const s32, "game/code_00242608", func_00244B90);

INCLUDE_ASM(const s32, "game/code_00242608", func_00244BC8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00244C00);

INCLUDE_ASM(const s32, "game/code_00242608", func_00244D10);

INCLUDE_ASM(const s32, "game/code_00242608", func_00244E08);

INCLUDE_ASM(const s32, "game/code_00242608", func_00244FA0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00245068);

INCLUDE_ASM(const s32, "game/code_00242608", func_00245190);

INCLUDE_ASM(const s32, "game/code_00242608", func_00245208);

INCLUDE_ASM(const s32, "game/code_00242608", func_002453C8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00245628);

INCLUDE_ASM(const s32, "game/code_00242608", func_002457E8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00245A40);

INCLUDE_ASM(const s32, "game/code_00242608", func_00245C00);









INCLUDE_RODATA(const s32, "game/code_00242608", D_003AF418);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AF428);

INCLUDE_SDATA(const s32, "game/code_00242608", D_003BC380);

INCLUDE_SDATA(const s32, "game/code_00242608", D_003BC388);

INCLUDE_SDATA(const s32, "game/code_00242608", D_003BC390);

INCLUDE_SDATA(const s32, "game/code_00242608", D_003BC398);

INCLUDE_SDATA(const s32, "game/code_00242608", D_003BC39C);

INCLUDE_SDATA(const s32, "game/code_00242608", D_003BC3A0);


INCLUDE_SDATA(const s32, "game/code_00242608", D_003BC3A8);

