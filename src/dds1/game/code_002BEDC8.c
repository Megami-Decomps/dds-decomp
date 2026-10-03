#include "common.h"

extern s32 effGetSlotWorkOrOverride(s32, s32);

/* 0x80 leaves the copied corner words unchanged. */
void sdfCopyCornerWordsWithEdgeByte(u32 *source, u32 *destination, u8 *edgeValues, u32 edge) {
    u8 value = edgeValues[edge];

    destination[0] = source[0];
    destination[1] = source[1];
    destination[2] = source[2];
    destination[3] = source[3];
    if (value != 0x80) {
        switch (edge) {
        case 0:
            destination[0] = (destination[0] & ~0xFF) | value;
            destination[1] = (destination[1] & ~0xFF) | value;
            break;
        case 1:
            destination[2] = (destination[2] & ~0xFF) | value;
            destination[3] = (destination[3] & ~0xFF) | value;
            break;
        case 2:
            destination[0] = (destination[0] & ~0xFF) | value;
            destination[2] = (destination[2] & ~0xFF) | value;
            break;
        case 3:
            destination[1] = (destination[1] & ~0xFF) | value;
            destination[3] = (destination[3] & ~0xFF) | value;
            break;
        }
    }
}


INCLUDE_ASM(const s32, "game/code_002BEDC8", func_002BEEA0);

INCLUDE_ASM(const s32, "game/code_002BEDC8", func_002BF198);
void func_002BF400(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8) {
    func_002BF198(a0, a1, a2, a6 + 0x14, a3, a4, a5, a6, a7);
}
void func_002BF438(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7) {
    func_002BF198(a0, a1, a2, a3, a4, a5, a6,
                  effGetSlotWorkOrOverride(a5, a6), a7);
}



INCLUDE_ASM(const s32, "game/code_002BEDC8", func_002BF4E0);

INCLUDE_ASM(const s32, "game/code_002BEDC8", func_002BF5D8);
