#include "common.h"

INCLUDE_ASM(const s32, "newdata/datCalc", func_00119098);

INCLUDE_ASM(const s32, "newdata/datCalc", func_001190B0);

INCLUDE_ASM(const s32, "newdata/datCalc", func_001191B0);

void func_001192B0(s32 arg0, s32 arg1) {
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

void func_001192D8(s32 arg0, s32 arg1) {
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

INCLUDE_ASM(const s32, "newdata/datCalc", func_00119300);

INCLUDE_ASM(const s32, "newdata/datCalc", func_00119368);

INCLUDE_ASM(const s32, "newdata/datCalc", func_001193A0);

INCLUDE_ASM(const s32, "newdata/datCalc", func_00119448);

INCLUDE_ASM(const s32, "newdata/datCalc", func_00119520);

INCLUDE_ASM(const s32, "newdata/datCalc", func_00119708);

INCLUDE_ASM(const s32, "newdata/datCalc", func_00119728);

INCLUDE_ASM(const s32, "newdata/datCalc", func_00119750);

INCLUDE_ASM(const s32, "newdata/datCalc", func_00119880);

INCLUDE_ASM(const s32, "newdata/datCalc", func_001198B8);

INCLUDE_ASM(const s32, "newdata/datCalc", func_001198E8);
