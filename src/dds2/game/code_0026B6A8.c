#include "common.h"

extern s32 func_00101958();

extern void func_002686F0(s32);

extern s64 func_002C4038(s32, s32 *, u64, u64);

extern void func_0026C900(void);

INCLUDE_ASM(const s32, "game/code_0026B6A8", func_0026B6A8);

s32 func_0026B8F0(void) {
    s32 *temp_v0 = (s32 *)func_00101958();

    func_002680E0(temp_v0);
    func_00268128(0, temp_v0);
    return 1;
}

u32 func_0026B930(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0026B6A8", func_0026B938);

void func_0026B9D0(s32 arg0) {
    s32 temp_v0 = func_00101958();

    func_002686F0(temp_v0);
    func_002C4038(temp_v0 + 8, temp_v0 + 0x54, 1, arg0);
}

void func_0026BA20(s32 arg0) {
    s32 temp_v0 = func_00101958();

    func_0026C900();
    func_002C4038(temp_v0 + 8, temp_v0 + 0x54, 2, arg0);
}

u32 func_0026BA68(void) {
    s32 temp_v0;

    temp_v0 = func_00101958();
    if (*(s32 *)(temp_v0 + 0xe4) == 0) {
        kwlnFadeInStart(0, 0, 0, 0xf);
    }
    else {
        kwlnFadeInStart(0, 0, 0, 0xf);
    }
    return 1;
}

s32 func_0026BAB8(void) {
    s32 temp_v0 = func_00101958();

    func_00268128(1, temp_v0);
    kwlnFadeOutStart(0, 0, 0, 0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0026B6A8", func_0026BAF8);

void func_0026BB78(s32 arg0) {
    s32 temp_v0 = func_00101958();

    func_002686F0(temp_v0);
    func_002C4038(temp_v0 + 8, temp_v0 + 0x54, 1, arg0);
}

INCLUDE_ASM(const s32, "game/code_0026B6A8", func_0026BBC8);

u32 func_0026BC00(void) {
    s32 temp_v0;

    temp_v0 = func_00101958();
    *(u32 *)(temp_v0 + 0x98) = 0;
    return 1;
}

s32 func_0026BC28(void) {
    s32 *temp_v0 = (s32 *)func_00101958();

    func_002678C8(temp_v0);
    func_00267768(temp_v0);
    return 1;
}

u32 func_0026BC68(void) {
    return 0;
}

u32 func_0026BC70(void) {
    return 0;
}

u32 func_0026BC78(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0026B6A8", func_0026BC80);

INCLUDE_ASM(const s32, "game/code_0026B6A8", func_0026BD38);

INCLUDE_ASM(const s32, "game/code_0026B6A8", func_0026BD50);

INCLUDE_ASM(const s32, "game/code_0026B6A8", func_0026BE28);

INCLUDE_ASM(const s32, "game/code_0026B6A8", func_0026BEB0);

INCLUDE_ASM(const s32, "game/code_0026B6A8", func_0026BEC0);

u32 func_0026C168(void) {
    return 0xffffffff;
}

INCLUDE_ASM(const s32, "game/code_0026B6A8", func_0026C170);

INCLUDE_RODATA(const s32, "game/code_0026B6A8", D_00425018);

