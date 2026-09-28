#include "common.h"

extern s32 func_00101958();

void func_0029AA48(s32 arg0) {
    func_002B7F80(arg0 + 0xad40, 0x20);
}

INCLUDE_ASM(const s32, "game/code_0029AA48", func_0029AA68);

INCLUDE_ASM(const s32, "game/code_0029AA48", func_0029AC20);

INCLUDE_ASM(const s32, "game/code_0029AA48", func_0029AD98);

INCLUDE_ASM(const s32, "game/code_0029AA48", func_0029AE10);

u32 func_0029AE70(void) {
    s8 temp_v0;
    s32 temp_v1;
    u32 *puVar3;
    s8 *pcVar4;
    s32 temp_v2;
    s32 temp_v3;
    s32 temp_v4;

    temp_v1 = func_00101958();
    temp_v4 = 0;
    temp_v2 = 4;
    temp_v3 = (*(s32 **)(temp_v1 + 0x9c))[1] * 3;
    pcVar4 = (s8 *)(**(s32 **)(temp_v1 + 0x9c) + 0x16);
    do {
        temp_v0 = *pcVar4;
        pcVar4 = pcVar4 + 1;
        temp_v2 = temp_v2 - 1;
        temp_v4 = temp_v4 + temp_v0;
    } while (-1 < temp_v2);
    *(u32 *)(temp_v1 + 0x3f0) = 0;
    temp_v2 = 4;
    puVar3 = (u32 *)(temp_v1 + 0x404);
    if (0x1ef - temp_v4 < temp_v3) {
        temp_v3 = 0x1ef - temp_v4;
    }
    *(s32 *)(temp_v1 + 0x3ec) = temp_v3;
    do {
        temp_v2 = temp_v2 - 1;
        *puVar3 = 0;
        puVar3 = puVar3 + -1;
    } while (-1 < temp_v2);
    if (*(s32 *)(temp_v1 + 0xb6e4) != 0) {
        func_002C0CF8(*(u32 *)(temp_v1 + 0xad34), 0);
    }
    return 1;
}

u32 func_0029AF40(void) {
    return 1;
}

void func_0029AF48(s32 arg0) {
    s32 temp_v0;
    u32 *puVar2;

    *(u32 *)(arg0 + 0x3f0) = 0;
    puVar2 = (u32 *)(arg0 + 0x404);
    temp_v0 = 4;
    do {
        temp_v0 = temp_v0 - 1;
        *puVar2 = 0;
        puVar2 = puVar2 + -1;
    } while (-1 < temp_v0);
}

void func_0029AF80(u32 arg0, u32 arg1) {
    func_00314298(arg0, (s32)arg1 + 0x3f4);
    func_00299A38(arg0, arg1);
}
