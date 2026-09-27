#include "common.h"

extern u64 func_00163258(u64, u64);

INCLUDE_ASM(const s32, "game/code_0016F1D0", func_0016F1D0);

void func_0016F420(u64 arg0) {
    u64 temp_v0;

    temp_v0 = func_00163258(arg0, 0);
    func_0016F1D0(temp_v0);
}

void func_0016F440(void) {
    func_0016F1D0();
}

void func_0016F458(s32 arg0) {
    func_001705A0(*(u32 *)(arg0 + 0x7c));
    func_002D0918(*(u32 *)(arg0 + 0x78));
}

INCLUDE_ASM(const s32, "game/code_0016F1D0", func_0016F488);

void func_0016F498(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x60) = arg1;
}

INCLUDE_ASM(const s32, "game/code_0016F1D0", func_0016F4A0);

INCLUDE_ASM(const s32, "game/code_0016F1D0", func_0016F4A8);

INCLUDE_ASM(const s32, "game/code_0016F1D0", func_0016F4D8);

INCLUDE_ASM(const s32, "game/code_0016F1D0", func_0016F5C8);

void func_0016F790(s32 arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x58) + arg1 * 0x10;
    *(float *)(temp_v0 + 8) = *(float *)(temp_v0 + 8) + *(float *)(arg0 + 0x50);
}

INCLUDE_ASM(const s32, "game/code_0016F1D0", func_0016F7B0);

INCLUDE_ASM(const s32, "game/code_0016F1D0", func_0016FB08);

void func_0016FC28(s32 arg0) {
    func_002DAA68(*(u32 *)(arg0 + 0x68));
    func_002D0918(*(u32 *)(arg0 + 0x6c));
}

INCLUDE_ASM(const s32, "game/code_0016F1D0", func_0016FC58);

s32 func_0016FF08(s32 arg0, s32 arg1) {
    return *(s32 *)(arg0 + 0x60) + arg1 * 0x50;
}

s32 func_0016FF20(s32 arg0, s32 arg1) {
    return *(s32 *)(arg0 + 100) + arg1 * 0x14;
}

void func_0016FF38(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x50) = arg1;
}

void func_0016FF40(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x54) = arg1;
}

INCLUDE_ASM(const s32, "game/code_0016F1D0", func_0016FF48);

INCLUDE_ASM(const s32, "game/code_0016F1D0", func_0016FF50);

void func_00170048(s32 arg0) {
    func_002DAA68(*(u32 *)(arg0 + 0x68));
    func_002D0918(*(u32 *)(arg0 + 0x6c));
}

INCLUDE_ASM(const s32, "game/code_0016F1D0", func_00170078);

s32 func_00170220(s32 arg0, s32 arg1) {
    return *(s32 *)(arg0 + 0x60) + arg1 * 0x30;
}

s32 func_00170238(s32 arg0, s32 arg1) {
    return *(s32 *)(arg0 + 100) + arg1 * 0xc;
}

INCLUDE_ASM(const s32, "game/code_0016F1D0", func_00170250);

void func_00170350(s32 arg0) {
    func_002DAA68(*(u32 *)(arg0 + 0x68));
    func_002D0918(*(u32 *)(arg0 + 0x6c));
}

INCLUDE_ASM(const s32, "game/code_0016F1D0", func_00170380);

s32 func_00170538(s32 arg0, s32 arg1) {
    return *(s32 *)(arg0 + 0x60) + arg1 * 0x40;
}

s32 func_00170548(s32 arg0, s32 arg1) {
    return *(s32 *)(arg0 + 100) + arg1 * 0x10;
}

INCLUDE_ASM(const s32, "game/code_0016F1D0", func_00170558);

INCLUDE_ASM(const s32, "game/code_0016F1D0", func_001705A0);

INCLUDE_ASM(const s32, "game/code_0016F1D0", func_001705B8);

s32 func_00170858(s32 arg0, s32 arg1) {
    return *(s32 *)(arg0 + 0x60) + arg1 * 0x50;
}

s32 func_00170870(s32 arg0, s32 arg1) {
    return *(s32 *)(arg0 + 100) + arg1 * 0x14;
}

void func_00170888(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x50) = arg1;
}

void func_00170890(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x54) = arg1;
}

INCLUDE_ASM(const s32, "game/code_0016F1D0", func_00170898);

INCLUDE_ASM(const s32, "game/code_0016F1D0", func_001708A0);
