#include "common.h"

extern void func_002C5720(void);
extern void func_002C5798(void);
extern void func_002C5C70(void);

extern void func_002C3DB0(void);

extern s32 func_002C4A10(void);

extern s32 D_003BD274;

extern u32 D_003BD268;

extern s8 D_003BD270;

extern s32 D_003BD26C;

extern void func_002C3C48(void);

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C3868);

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C38B0);

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C3AC8);

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C3C48);

void func_002C3CD0(u32 arg0) {
    func_002C3C48();
    D_003BD268 = arg0;
}

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C3CF8);

void func_002C3D48(void) {
    if (D_003BD268 != 0) {
        func_002C3C48();
        D_003BD268 = D_003BD268 - 1;
    } else {
        func_002C3C48();
        D_003BD268 = D_003BD26C - 1;
    }
}

s8 func_002C3D88(void) {
    return D_003BD270;
}

void func_002C3D90(void) {
    func_002C3DB0();
}

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C3DB0);

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C3F78);

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C4160);

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C42F0);

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C44D0);

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C45C8);

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C4630);

void func_002C4650(void) {
    func_002C5720();
    func_002C5798();
    func_002C5C70();
}

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C4680);

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C4850);

u32 func_002C49F8(void) {
    return **(u32 **)(*(s32 *)(D_003BD274 + 0x1c) + 0x70);
}

s32 func_002C4A10(void) {
    return *(s16 *)(*(s32 *)(*(s32 *)(D_003BD274 + 0x1c) + 0x70) + 8);
}

float func_002C4A28(void) {
    s32 p;

    p = *(s32 *)(D_003BD274 + 0x30);
    return (float)(*(s32 *)p) / 10.0f;
}

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C4A58);

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C4C88);

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C5338);

void func_002C56C0(void) {
    s32 temp_v0;

    temp_v0 = **(s32 **)(D_003BD274 + 0x30);
    if (temp_v0 < 10) {
        **(s32 **)(D_003BD274 + 0x30) = temp_v0 + 1;
    }
}

void func_002C56E8(void) {
    s32 temp_v0;

    temp_v0 = **(s32 **)(D_003BD274 + 0x30);
    if (temp_v0 != 0) {
        **(s32 **)(D_003BD274 + 0x30) = temp_v0 - 1;
    }
}

void func_002C5708(s32 arg0) {
    s32 ptr;

    ptr = *(s32 *)(D_003BD274 + 0x30);
    *(s16 *)(ptr + 6) = arg0;
    *(s16 *)(ptr + 4) = 8;
}

void func_002C5720(void) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(D_003BD274 + 0x30);
    if (0 < *(s16 *)(temp_v0 + 4)) {
        *(s16 *)(temp_v0 + 4) = *(s16 *)(temp_v0 + 4) - 1;
    }
}

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C5748);

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C5798);

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C57F0);

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C5BF8);

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C5C40);

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C5C70);

void func_002C5D30(void) {
}

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C5D38);


INCLUDE_SDATA(const s32, "game/code_002C3868", D_003BD268);

INCLUDE_SDATA(const s32, "game/code_002C3868", D_003BD26C);

INCLUDE_SDATA(const s32, "game/code_002C3868", D_003BD270);

INCLUDE_SDATA(const s32, "game/code_002C3868", D_003BD271);


INCLUDE_SDATA(const s32, "game/code_002C3868", D_003BD274);

