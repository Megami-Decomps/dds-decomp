#include "common.h"

void func_0015D9E0(u32 arg0) {
    func_0015B8B8(*(u32 *)((s32)arg0 + 0xdc));
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "effect/polyManager", func_0015DA10);

void func_0015DA80(float arg0, s32 arg1) {
    *(float *)(arg1 + 0xcc) = *(float *)(arg1 + 0xcc) * arg0;
    *(float *)(arg1 + 0xd0) = *(float *)(arg1 + 0xd0) * arg0;
}

INCLUDE_ASM(const s32, "effect/polyManager", func_0015DAA0);

void func_0015DC48(s32 arg0) {
    func_0015DAA0();
    func_0015B918(*(u32 *)(arg0 + 0xdc));
}

INCLUDE_ASM(const s32, "effect/polyManager", func_0015DC70);

INCLUDE_ASM(const s32, "effect/polyManager", func_0015DDC8);

INCLUDE_ASM(const s32, "effect/polyManager", func_0015DE88);

INCLUDE_ASM(const s32, "effect/polyManager", func_0015DFA8);

void func_0015E0D0(s32 arg0) {
    func_0015B8B8(*(u32 *)(arg0 + 0xf0));
    func_002D0918(*(u32 *)(arg0 + 0xf8));
}

INCLUDE_ASM(const s32, "effect/polyManager", func_0015E100);

INCLUDE_ASM(const s32, "effect/polyManager", func_0015E148);

INCLUDE_ASM(const s32, "effect/polyManager", func_0015E238);

INCLUDE_ASM(const s32, "effect/polyManager", func_0015E3D8);

void func_0015E5A0(float arg0, s32 arg1) {
    *(float *)(arg1 + 200) = *(float *)(arg1 + 200) * arg0;
    *(float *)(arg1 + 0xdc) = *(float *)(arg1 + 0xdc) * arg0;
    *(float *)(arg1 + 0xcc) = *(float *)(arg1 + 0xcc) * arg0;
    *(float *)(arg1 + 0xd0) = *(float *)(arg1 + 0xd0) * arg0;
}

INCLUDE_ASM(const s32, "effect/polyManager", func_0015E5D8);

INCLUDE_ASM(const s32, "effect/polyManager", func_0015E760);

void func_0015E888(s32 arg0) {
    func_0015B8B8(*(u32 *)(arg0 + 0xe0));
    func_002D0918(*(u32 *)(arg0 + 0xe8));
}

INCLUDE_ASM(const s32, "effect/polyManager", func_0015E8B8);

INCLUDE_ASM(const s32, "effect/polyManager", func_0015E900);

INCLUDE_ASM(const s32, "effect/polyManager", func_0015E9A0);

void func_0015EBF8(float arg0, s32 arg1) {
    *(float *)(arg1 + 0xcc) = *(float *)(arg1 + 0xcc) * arg0;
}

INCLUDE_ASM(const s32, "effect/polyManager", func_0015EC08);

INCLUDE_ASM(const s32, "effect/polyManager", func_0015ED90);

void func_0015EEC0(s32 arg0) {
    func_0015B8B8(*(u32 *)(arg0 + 0xf4));
    func_002D0918(*(u32 *)(arg0 + 0xfc));
}

INCLUDE_ASM(const s32, "effect/polyManager", func_0015EEF0);

INCLUDE_ASM(const s32, "effect/polyManager", func_0015EF50);

INCLUDE_ASM(const s32, "effect/polyManager", func_0015F0C0);

void func_0015F2B0(float arg0, s32 arg1) {
    *(float *)(arg1 + 0xcc) = *(float *)(arg1 + 0xcc) * arg0;
    *(float *)(arg1 + 0xd0) = *(float *)(arg1 + 0xd0) * arg0;
}

INCLUDE_ASM(const s32, "effect/polyManager", func_0015F2D0);

void func_0015F468(s32 arg0) {
    u32 temp_v0;
    s32 *piVar2;
    u32 temp_v1;

    temp_v0 = *(u32 *)(arg0 + 0x10);
    temp_v1 = 0;
    *(u32 *)(arg0 + 0x14) = 0xfffffff;
    *(u32 *)(arg0 + 0x6c) = 0;
    *(u32 *)(arg0 + 0x68) = 0;
    piVar2 = *(s32 **)(arg0 + 0xf8);
    if (temp_v0 != 0) {
        do {
            if (*piVar2 != -0xffffff) {
                *piVar2 = 0xffffff0;
            }
            temp_v1 = temp_v1 + 1;
            piVar2 = piVar2 + 5;
        } while (temp_v1 < temp_v0);
    }
}
