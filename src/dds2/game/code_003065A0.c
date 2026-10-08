#include "common.h"
#include "eff.h"

extern void *effGetSlotWorkOrOverride(EffectSlotSet *, s32);

extern void func_00306970(s32, s32, s32, u32 *, s32, EffectSlotSet *, s32, BdWork *, s32);
extern void func_00306CD0(s32, s32, s32, u32, s32, EffectSlotSet *, s32, s32);
extern u32 uiBlendColors(u32, u32, u32);

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

INCLUDE_ASM(const s32, "game/code_003065A0", func_00306678);

INCLUDE_ASM(const s32, "game/code_003065A0", func_00306970);

void func_00306BF0(s32 a0, s32 a1, s32 a2, s32 a3, EffectSlotSet *a4, s32 a5, BdWork *a6, s32 a7, s32 a8) {
    func_00306970(a0, a1, a2, a6->geometry.cornerColors, a3, a4, a5, a6, a7);
}
void func_00306C28(s32 a0, s32 a1, s32 a2, u32 *a3, s32 a4, EffectSlotSet *a5, s32 a6, s32 a7) {
    func_00306970(a0, a1, a2, a3, a4, a5, a6,
                  effGetSlotWorkOrOverride(a5, a6), a7);
}



void func_00306CD0(s32 x, s32 y, s32 depth, u32 blend, s32 flags,
                   EffectSlotSet *set, s32 slotIndex, s32 layer) {
    u32 colors[4];
    BdWork *work = effGetSlotWorkOrOverride(set, slotIndex);
    s32 i;

    for (i = 0; i < 4; i++) {
        colors[i] = uiBlendColors(work->geometry.cornerColors[i],
                                  work->geometry.cornerColors[i] & 0xFFFFFF00, blend);
    }
    func_00306970(x, y, depth, colors, flags, set, slotIndex, work, layer);
}

INCLUDE_ASM(const s32, "game/code_003065A0", func_00306DC8);
