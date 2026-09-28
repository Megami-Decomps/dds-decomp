#include "common.h"

extern u32 *D_00438940;

void func_0031BB80(u8 *node);

void func_0031C578(s32 node);

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031B188);

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031B1F8);

void func_0031B268(void) {
    if (D_00438940 != (u32 *)0x0) {
        func_003297C8(*D_00438940);
        D_00438940 = (u32 *)0x0;
    }
}

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031B290);

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031B2E0);

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031B328);

void func_0031B3B0(s32 arg0) {
    *(u16 *)(arg0 + 0x1da) = 0;
}

void func_0031B3B8(s32 arg0) {
    *(u16 *)(arg0 + 0x1da) = 1;
}

void func_0031B3C8(void) {
}

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031B3D0);

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031B4F0);

void func_0031B5F8(s32 *arg0) {
    s32 temp_v0;
    u32 temp_v1;

    temp_v1 = 0;
    temp_v0 = *arg0;
    if (0 < arg0[1]) {
        do {
            memset(temp_v0, 0, 0x20);
            temp_v1 = (temp_v1 + 1) & 0xffff;
            temp_v0 = temp_v0 + 0x20;
        } while ((s32)temp_v1 < arg0[1]);
    }
}

void func_0031B668(s32 *list) {
    u8 *node = (u8 *)list[0];
    s32 index = 0;
    if (list[1] > 0) {
        do {
            func_0031BB80(node);
            node += 0x20;
            index++;
        } while (list[1] > index);
    }
}

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031B6D0);

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031B748);

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031B838);

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031B960);

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031BA28);

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031BB80);

u32 func_0031BBB0(u32 *owner, u32 resource) {
    u32 handle;
    u32 other;
    u32 data = func_00343ED0(resource, &handle, &other);
    *owner = func_002D4138(handle);
    func_002D46A0(*owner);
    func_003297C8(data);
    return *owner;
}

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031BC10);

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031BDE8);

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031BFA0);

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031BFC0);

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031BFE0);

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031C0F8);

void func_0031C1A0(s32 *list) {
    u8 *node = (u8 *)list[0];
    s32 index = 0;
    if (list[1] > 0) {
        do {
            func_0031C578((s32)node);
            node += 0x50;
            index++;
        } while (index < list[1]);
    }
}

void func_0031C208(s32 *list) {
    u8 *node = (u8 *)list[0];
    s16 index = 0;
    if (list[1] > 0) {
        do {
            u32 model = *(u32 *)(node + 0x40);
            node += 0x50;
            if (model != 0) {
                func_002322E8(model);
            }
            index++;
        } while (index < list[1]);
    }
}

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031C280);

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031C348);

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031C3C8);

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031C458);

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031C4A0);

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031C4E8);

void func_0031C578(s32 arg0) {
    *(u32 *)(arg0 + 0x44) = 0;
    **(u32 **)(arg0 + 0x40) = **(u32 **)(arg0 + 0x40) | 1;
}

void func_0031C590(u8 *model, s32 value) {
    value &= 0xFFFF;
    *(u16 *)(model + 0x48) = value;
    *(u16 *)(model + 0x4A) = value;
}

void func_0031C5A0(u8 *model, f32 x, f32 y, f32 z) {
    *(f32 *)(model + 0x30) = x;
    *(f32 *)(model + 0x34) = y;
    *(f32 *)(model + 0x38) = z;
    *(u32 *)(model + 0x3C) = 0;
}

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031C5B8);

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031C5E8);

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031C630);

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031C688);

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031C850);

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031C888);

void func_0031C8A8(void) {
}

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031C8B0);

void func_0031C900(u8 *model, s8 selector) {
    if (selector == 1) {
        **(u32 **)(model + 0x40) &= ~1U;
    } else {
        **(u32 **)(model + 0x40) |= 1U;
    }
}

INCLUDE_SDATA(const s32, "game/code_0031B188", D_00438950);

