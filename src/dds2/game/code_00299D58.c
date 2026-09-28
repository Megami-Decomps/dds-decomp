#include "common.h"

extern u32 func_0029D790(u32, s32);

extern void func_0026C900(void);

extern s32 func_00101958();

extern void func_002C44E8(s32);

extern void func_002C4038(s32, s32, s32, s32);

extern void func_0029CD60(s32);

INCLUDE_ASM(const s32, "game/code_00299D58", func_00299D58);

INCLUDE_ASM(const s32, "game/code_00299D58", func_00299E30);

INCLUDE_ASM(const s32, "game/code_00299D58", func_00299E90);

u32 func_00299EF0(void) {
    return 1;
}

u32 func_00299EF8(void) {
    return 1;
}

void func_00299F00(s32 input) {
    s32 context = func_00101958();
    func_002C44E8(0x33);
    func_002C4038(context + 8, context + 0x54, 0, input);
}

void func_00299F50(s32 input) {
    s32 context = func_00101958();
    func_0029CD60(context);
    func_002C4038(context + 8, context + 0x54, 1, input);
}

void func_00299FA0(s32 input) {
    s32 context = func_00101958();
    func_002C4038(context + 8, context + 0x54, 2, input);
}

u32 func_00299FD8(void) {
    return 1;
}

u32 func_00299FE0(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00299D58", func_00299FE8);

u32 func_0029A088(u32 arg0) {
    u32 temp_v0;
    s32 temp_v1;

    temp_v1 = (s32)arg0;
    temp_v0 = func_0029D790(**(u32 **)(temp_v1 + 0x9c), temp_v1 + 0x4e8);
    *(u32 *)(temp_v1 + 0x268) = temp_v0;
    func_00299FE8(arg0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00299D58", func_0029A0C8);

u32 func_0029A1E0(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00299D58", func_0029A1E8);

INCLUDE_ASM(const s32, "game/code_00299D58", func_0029A270);

INCLUDE_ASM(const s32, "game/code_00299D58", func_0029A2F8);

INCLUDE_ASM(const s32, "game/code_00299D58", func_0029A400);

INCLUDE_ASM(const s32, "game/code_00299D58", func_0029A588);

void func_0029A5D8(s32 arg0) {
    s32 temp_v0 = func_00101958();

    func_0026C900();
    func_002C4038(temp_v0 + 8, temp_v0 + 0x54, 2, arg0);
}

INCLUDE_ASM(const s32, "game/code_00299D58", func_0029A620);

u32 func_0029A650(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00299D58", func_0029A658);

INCLUDE_ASM(const s32, "game/code_00299D58", func_0029A748);

INCLUDE_ASM(const s32, "game/code_00299D58", func_0029A768);

INCLUDE_ASM(const s32, "game/code_00299D58", func_0029A898);

INCLUDE_SDATA(const s32, "game/code_00299D58", D_004379B0);

