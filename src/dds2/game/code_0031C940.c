#include "common.h"

void func_0031D508(s32 node);

INCLUDE_ASM(const s32, "game/code_0031C940", func_0031C940);

INCLUDE_ASM(const s32, "game/code_0031C940", func_0031CA10);

INCLUDE_ASM(const s32, "game/code_0031C940", func_0031CAE8);

INCLUDE_ASM(const s32, "game/code_0031C940", func_0031CBC8);

INCLUDE_ASM(const s32, "game/code_0031C940", func_0031CDE8);

INCLUDE_ASM(const s32, "game/code_0031C940", func_0031CE60);

INCLUDE_ASM(const s32, "game/code_0031C940", func_0031CF68);

INCLUDE_ASM(const s32, "game/code_0031C940", func_0031CF88);

INCLUDE_ASM(const s32, "game/code_0031C940", func_0031D120);

INCLUDE_ASM(const s32, "game/code_0031C940", func_0031D260);

void func_0031D380(s32 *arg0) {
    s32 temp_v0;
    u32 temp_v1;

    temp_v1 = 0;
    temp_v0 = *arg0;
    if (0 < arg0[1]) {
        do {
            memset(temp_v0, 0, 0x34);
            temp_v1 = (temp_v1 + 1) & 0xffff;
            temp_v0 = temp_v0 + 0x34;
        } while ((s32)temp_v1 < arg0[1]);
    }
}

void func_0031D3F0(s32 *arg0, u32 arg1) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v1 = 0;
    temp_v0 = *arg0;
    if (0 < arg0[1]) {
        do {
            temp_v1 = temp_v1 + 1;
            *(u32 *)(temp_v0 + 0x20) = (*(u32 *)(temp_v0 + 0x20) & 0xfffff807) | ((arg1 & 0xff) << 3);
            temp_v0 = temp_v0 + 0x34;
        } while (temp_v1 < arg0[1]);
    }
}

void func_0031D440(s32 *list) {
    u8 *node = (u8 *)list[0];
    s32 index = 0;
    if (list[1] > 0) {
        do {
            func_0031D508((s32)node);
            node += 0x34;
            index++;
        } while (index < list[1]);
    }
}

s32 func_0031D4A8(s32 *arg0) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v1 = 0;
    temp_v0 = *arg0;
    if (0 < arg0[1]) {
        do {
            if ((*(u32 *)(temp_v0 + 0x20) & 1) == 0) {
                *(u16 *)(temp_v0 + 0x2a) = 10;
                *(u32 *)(temp_v0 + 0x20) = *(u32 *)(temp_v0 + 0x20) | 1;
                *(u16 *)(temp_v0 + 0x28) = 0;
                *(u16 *)(temp_v0 + 0x24) = 0;
                *(u16 *)(temp_v0 + 0x26) = 0;
                return temp_v0;
            }
            temp_v1 = temp_v1 + 1;
            temp_v0 = temp_v0 + 0x34;
        } while (temp_v1 < arg0[1]);
    }
    return 0;
}

void func_0031D508(s32 arg0) {
    *(u32 *)(arg0 + 0x20) = *(u32 *)(arg0 + 0x20) & 0xfffffffe;
}

void func_0031D520(u8 *model, s32 value) {
    value &= 0xFFFF;
    *(u16 *)(model + 0x24) = value;
    *(u16 *)(model + 0x26) = value;
}

void func_0031D530(u8 *model, f32 x, f32 y, f32 z) {
    *(f32 *)(model + 0x10) = x;
    *(f32 *)(model + 0x14) = y;
    *(f32 *)(model + 0x18) = z;
}

void func_0031D540(u8 *model, f32 x, f32 y, f32 z) {
    *(f32 *)(model + 0x0) = x;
    *(f32 *)(model + 0x4) = y;
    *(f32 *)(model + 0x8) = z;
    *(u32 *)(model + 0xC) = 0;
}

INCLUDE_ASM(const s32, "game/code_0031C940", func_0031D558);

INCLUDE_ASM(const s32, "game/code_0031C940", func_0031D680);
