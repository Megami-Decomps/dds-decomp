#include "common.h"

extern u32 func_00116D38(u32);

INCLUDE_ASM(const s32, "game/code_00111610", func_00111610);

void func_00111698(s32 arg0, u32 arg1) {
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 8) = arg1;
}

void func_001116A8(s32 arg0, u32 arg1) {
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 0xc) = arg1;
}

void func_001116B8(s32 arg0) {
    s32 temp_v0;
    u32 temp_v1;

    temp_v0 = *(s32 *)(arg0 + 0x18);
    if (*(s32 *)(temp_v0 + 4) != 0) {
        func_00116F08(*(s32 *)(temp_v0 + 4));
    }
    temp_v1 = func_00116D38(*(u32 *)(temp_v0 + 0xc));
    *(u32 *)(temp_v0 + 4) = temp_v1;
}

void func_001116F8(s32 arg0) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v0 = *(s32 *)(arg0 + 0x18);
    temp_v1 = *(s32 *)(temp_v0 + 4);
    if (temp_v1 != 0) {
        func_00116F08(temp_v1);
        *(u32 *)(temp_v0 + 4) = 0;
    }
}

u32 func_00111730(s32 arg0) {
    return *(u32 *)(*(s32 *)(arg0 + 0x18) + 4);
}

void func_00111740(void) {
    func_00111478();
}

INCLUDE_ASM(const s32, "game/code_00111610", func_00111758);

INCLUDE_ASM(const s32, "game/code_00111610", func_001117A8);
