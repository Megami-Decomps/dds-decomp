#include "common.h"

INCLUDE_ASM(const s32, "game/code_0031A638", func_0031A638);

INCLUDE_ASM(const s32, "game/code_0031A638", func_0031A690);

INCLUDE_ASM(const s32, "game/code_0031A638", func_0031A730);

INCLUDE_ASM(const s32, "game/code_0031A638", func_0031A770);

INCLUDE_ASM(const s32, "game/code_0031A638", func_0031A7F8);

void func_0031A830(u8 *work) {
    s32 progress;
    f32 ratio;

    if (*(s32 *)(work + 0x80) >= 0x65) {
        *(s32 *)(work + 0x80) = 0x64;
    }
    progress = *(s32 *)(work + 0x80);
    ratio = (f32)progress / 100.0f;
    if (*(s16 *)(work + 0x88) != 0) {
        return;
    }
    if (ratio < 0.25f) {
        *(s32 *)(work + 0x84) = 1;
    } else if (ratio < 0.5f) {
        *(s32 *)(work + 0x84) = 2;
    } else if (ratio < 0.75f) {
        *(s32 *)(work + 0x84) = 4;
    } else if (ratio < 1.0f) {
        *(s32 *)(work + 0x84) = 8;
    } else {
        *(s32 *)(work + 0x84) = 0x14;
        *(s16 *)(work + 0x88) = 0x258;
        func_0031B2E0(0x1E00005, 0);
    }
}

INCLUDE_ASM(const s32, "game/code_0031A638", func_0031A920);

INCLUDE_ASM(const s32, "game/code_0031A638", func_0031AA10);
