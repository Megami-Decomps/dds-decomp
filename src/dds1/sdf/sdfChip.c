#include "common.h"

extern s32 D_003BD9B0;
extern s32 D_003BD9B4;
extern u8 D_003BD9C0;

void *func_002CFEB8(s32 arg0);
void *func_002E7480(void *arg0, s32 arg1);
void func_002D3C30(void *arg0, s32 arg1);

void *func_002CFF68(s32 arg0) {
    return func_002E7480(func_002CFEB8(arg0), (arg0 + 15) >> 4);
}

INCLUDE_ASM(const s32, "sdf/sdfChip", func_002CFF98);

void func_002D00B8(s32 arg0) {
    func_002D3C30(&D_003BD9C0, arg0);
}

s32 func_002D00D8(s32 arg0) {
    s32 ret;

    ret = 0;
    if (arg0 >= D_003BD9B0) {
        ret = arg0 < D_003BD9B4;
    }
    return ret;
}
