#include "common.h"

extern u64 func_0010D650(u64);

extern u64 func_0011A318(u64);

u32 func_0010F118(void) {
    u64 temp_v0;

    temp_v0 = func_0010D650(0);
    func_0011A328(temp_v0);
    return 1;
}

u32 func_0010F140(void) {
    func_0011A700();
    return 1;
}

u32 func_0010F160(void) {
    u64 temp_v0;

    temp_v0 = func_0010D650(0);
    temp_v0 = func_0011A318(temp_v0);
    func_0010D818(temp_v0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0010F118", func_0010F190);

INCLUDE_ASM(const s32, "game/code_0010F118", func_0010F490);

INCLUDE_ASM(const s32, "game/code_0010F118", func_0010F518);

INCLUDE_ASM(const s32, "game/code_0010F118", func_0010F640);

INCLUDE_ASM(const s32, "game/code_0010F118", func_0010F718);

INCLUDE_ASM(const s32, "game/code_0010F118", func_0010F798);

void func_0010F810(s32 arg0) {
    s32 temp_v0;

    if (arg0 != 0) {
        temp_v0 = *(s32 *)((s32)arg0 + 0x1c);
        if (temp_v0 != 0) {
            func_00328E48(temp_v0);
            *(u32 *)((s32)arg0 + 0x1c) = 0;
        }
    }
}

void func_0010F850(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0xc0) = *(u32 *)(arg0 + 0xc0) | arg1;
}

void func_0010F860(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0xc0) = *(u32 *)(arg0 + 0xc0) & ~arg1;
}

u8 func_0010F878(s32 arg0, u32 arg1) {
    return (*(u32 *)(arg0 + 0xc0) & arg1) != 0;
}

INCLUDE_ASM(const s32, "game/code_0010F118", func_0010F888);

INCLUDE_ASM(const s32, "game/code_0010F118", func_0010F8B0);

INCLUDE_ASM(const s32, "game/code_0010F118", func_0010F8E8);

INCLUDE_ASM(const s32, "game/code_0010F118", func_0010F8F8);

INCLUDE_ASM(const s32, "game/code_0010F118", func_0010F908);

INCLUDE_ASM(const s32, "game/code_0010F118", func_0010F938);

INCLUDE_ASM(const s32, "game/code_0010F118", func_0010F968);

INCLUDE_ASM(const s32, "game/code_0010F118", func_0010F998);

INCLUDE_ASM(const s32, "game/code_0010F118", func_0010F9B0);

INCLUDE_ASM(const s32, "game/code_0010F118", func_0010F9D0);

INCLUDE_ASM(const s32, "game/code_0010F118", func_0010F9E8);

INCLUDE_ASM(const s32, "game/code_0010F118", func_0010FA20);

INCLUDE_ASM(const s32, "game/code_0010F118", func_0010FA70);
