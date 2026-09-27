#include "common.h"

INCLUDE_ASM(const s32, "newdata/datCalc", func_001197A8);

INCLUDE_ASM(const s32, "newdata/datCalc", func_001197C0);

INCLUDE_ASM(const s32, "newdata/datCalc", func_001198C0);

void func_001199C0(s32 arg0, s32 arg1) {
    u32 temp_v0;

    temp_v0 = (u32)*(u16 *)(arg0 + 6) + arg1;
    if ((s32)temp_v0 < 0) {
        temp_v0 = 0;
    }
    if ((s32)(u32)*(u16 *)(arg0 + 8) < (s32)temp_v0) {
        temp_v0 = (u32)*(u16 *)(arg0 + 8);
    }
    *(s16 *)(arg0 + 6) = (s16)temp_v0;
}

void func_001199E8(s32 arg0, s32 arg1) {
    u32 temp_v0;

    temp_v0 = (u32)*(u16 *)(arg0 + 10) + arg1;
    if ((s32)temp_v0 < 0) {
        temp_v0 = 0;
    }
    if ((s32)(u32)*(u16 *)(arg0 + 0xc) < (s32)temp_v0) {
        temp_v0 = (u32)*(u16 *)(arg0 + 0xc);
    }
    *(s16 *)(arg0 + 10) = (s16)temp_v0;
}

INCLUDE_ASM(const s32, "newdata/datCalc", func_00119A10);

INCLUDE_ASM(const s32, "newdata/datCalc", func_00119A78);

INCLUDE_ASM(const s32, "newdata/datCalc", func_00119AF8);

INCLUDE_ASM(const s32, "newdata/datCalc", func_00119BA0);

INCLUDE_ASM(const s32, "newdata/datCalc", func_00119C78);

INCLUDE_ASM(const s32, "newdata/datCalc", func_00119F20);

INCLUDE_ASM(const s32, "newdata/datCalc", func_00119F40);

INCLUDE_ASM(const s32, "newdata/datCalc", func_00119F68);

INCLUDE_ASM(const s32, "newdata/datCalc", func_0011A098);

INCLUDE_ASM(const s32, "newdata/datCalc", func_0011A0D0);

INCLUDE_ASM(const s32, "newdata/datCalc", func_0011A100);
