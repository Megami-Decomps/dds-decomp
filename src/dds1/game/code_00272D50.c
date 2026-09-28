#include "common.h"

extern s64 func_00273750(void);

extern s32 func_00101A70();

extern s32 D_003BAA00;

extern s64 func_00285670(s32, s32 *, u64, u64);

INCLUDE_ASM(const s32, "game/code_00272D50", func_00272D50);

void func_00273020(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x90c);
    func_0027C430(*(u32 *)(temp_v0 + 8));
    func_0027C430(*(u32 *)(temp_v0 + 0xc));
}

INCLUDE_ASM(const s32, "game/code_00272D50", func_00273050);

INCLUDE_ASM(const s32, "game/code_00272D50", func_002730A0);

void func_00273200(s32 arg0) {
    func_0027C430(*(u32 *)(*(s32 *)(arg0 + 0x90c) + 0x10));
}

INCLUDE_ASM(const s32, "game/code_00272D50", func_00273220);

void func_00273390(u32 arg0) {
    setStaffDisplayMode(2, arg0);
}

void func_002733B0() {
}

INCLUDE_ASM(const s32, "game/code_00272D50", func_002733B8);

s32 freeStaffDisplayResources(void) {
    u8 *context = (u8 *)func_00101A70();
    u32 *resource = *(u32 **)(context + 0x90c);
    func_00273020((s32)context);
    func_002733B0(context);
    func_002D0918(*resource);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00272D50", func_002734C0);

INCLUDE_ASM(const s32, "game/code_00272D50", func_00273670);

INCLUDE_ASM(const s32, "game/code_00272D50", func_00273718);

INCLUDE_ASM(const s32, "game/code_00272D50", func_00273750);

void func_00273838(s32 arg0, s32 arg1) {
    s32 temp_v0;
    s64 temp_v1;

    temp_v0 = *(s32 *)(arg1 + 0x90c);
    temp_v1 = func_00273750();
    if (temp_v1 != 0) {
        *(u32 *)(*(s32 *)(*(s32 *)(*(s32 *)(temp_v0 + 8) + 0x14) + 0x1c) + 0x60) =
                  (u32)*(u8 *)(arg0 + D_003BAA00 + 0x12a0);
        *(s32 *)(temp_v0 + 0x28) = arg0;
    }
    func_00283BF0(arg1 + 0x914, 1);
}

INCLUDE_ASM(const s32, "game/code_00272D50", func_002738A0);

INCLUDE_ASM(const s32, "game/code_00272D50", func_00273A30);
