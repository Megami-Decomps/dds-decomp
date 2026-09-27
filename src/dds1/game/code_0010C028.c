#include "common.h"

extern s32 func_00101A70(void);

void func_0010C028(u32 arg0, u32 arg1) {
    s32 temp_v0;

    temp_v0 = func_00101A70();
    *(u32 *)(temp_v0 + 0xf0) = arg1;
}

u32 func_0010C050(void) {
    s32 temp_v0;

    temp_v0 = func_00101A70();
    return *(u32 *)(temp_v0 + 0xf0);
}

void func_0010C070(u32 arg0) {
    s64 temp_v0;

    temp_v0 = func_00101A70();
    if (temp_v0 != 0) {
        func_0010BD20(temp_v0);
    }
    func_00101A68(arg0, 0);
}

void func_0010C0B0(void) {
    func_0010D380();
}

INCLUDE_ASM(const s32, "game/code_0010C028", func_0010C0C8);

void func_0010C120(s32 arg0, u32 arg1) {
    *(u8 *)(*(s32 *)(arg0 + 0x1c) + arg0 + 0x20) = 0;
    *(u32 *)(*(s32 *)(arg0 + 0x1c) * 4 + arg0 + 0x3c) = arg1;
    *(s32 *)(arg0 + 0x1c) = *(s32 *)(arg0 + 0x1c) + 1;
}

INCLUDE_ASM(const s32, "game/code_0010C028", func_0010C150);

void func_0010C180(s32 arg0, u32 arg1) {
    *(u8 *)(*(s32 *)(arg0 + 0x1c) + arg0 + 0x20) = 5;
    *(u32 *)(*(s32 *)(arg0 + 0x1c) * 4 + arg0 + 0x3c) = arg1;
    *(s32 *)(arg0 + 0x1c) = *(s32 *)(arg0 + 0x1c) + 1;
}

void func_0010C1B0(s32 arg0, u32 arg1) {
    *(u8 *)(*(s32 *)(arg0 + 0x1c) + arg0 + 0x20) = 4;
    *(u32 *)(*(s32 *)(arg0 + 0x1c) * 4 + arg0 + 0x3c) = arg1;
    *(s32 *)(arg0 + 0x1c) = *(s32 *)(arg0 + 0x1c) + 1;
}

INCLUDE_ASM(const s32, "game/code_0010C028", func_0010C1E0);

INCLUDE_ASM(const s32, "game/code_0010C028", func_0010C2B8);

u32 func_0010C3A0(u32 arg0) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v1 = (s32)arg0;
    temp_v0 = *(s32 *)(temp_v1 + 0x18) + 1;
    *(s32 *)(temp_v1 + 0x18) = temp_v0;
    func_0010C120(arg0, *(u32 *)(temp_v0 * 4 + *(s32 *)(temp_v1 + 0xbc)));
    *(s32 *)(temp_v1 + 0x18) = *(s32 *)(temp_v1 + 0x18) + 1;
    return 1;
}
