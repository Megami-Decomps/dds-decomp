#include "common.h"

extern s32 D_003BD9B0;
extern s32 D_003BD9B4;
extern u8 D_003BD9C0;

void *func_002CFEB8(s32 arg0);
void *sdfClearQuadwords(void *arg0, s32 arg1);
void func_002D3C30(void *arg0, s32 arg1);

void *func_002CFF68(s32 size) {
    return sdfClearQuadwords(func_002CFEB8(size), (size + 15) >> 4);
}

INCLUDE_ASM(const s32, "sdf/sdfChip", func_002CFF98);

void func_002D00B8(s32 value) {
    func_002D3C30(&D_003BD9C0, value);
}

s32 sdfChipIsInRange(s32 address) {
    s32 inRange;

    inRange = 0;
    if (address >= D_003BD9B0) {
        inRange = address < D_003BD9B4;
    }
    return inRange;
}
