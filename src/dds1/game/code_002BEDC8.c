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

typedef struct EffectSlotDescription {
    u8 pad00[0x14];
    u32 textureIndex;
    s32 flags;
    u8 pad1C[0x10];
    s32 presetMode;
    u8 pad30[4];
    u32 rect[4];
    s32 xOffset;
    s32 yOffset;
    u8 pad4C[8];
    s32 colors[4];
    u8 pad64[0x1C];
} EffectSlotDescription;

typedef struct BdWork {
    s32 flags;
    s32 unk04;
    s32 unk08;
    s32 width;
    s32 height;
    u8 pad14[0x10];
    f32 alpha;
    u8 pad28[0x28];
    s32 colors[4];
    u8 pad60[8];
    u32 rect[4];
    u8 pad78[0x1C];
    s32 slotOffset;
    u8 pad98[8];
} BdWork;

typedef struct EffectSlotSet {
    u8 pad00[0x10];
    EffectSlotDescription *descriptions;
    u32 workAllocation;
    BdWork *workEntries;
    u8 pad1C[8];
    u32 *textures;
} EffectSlotSet;

extern void sdfSubmitGsTestOneRegisterPacket();
extern void effSelectPresetAndDispatch(u32, u32, u32, u32, u32, u32, u32, u32);
extern u8 *effUpdateTimedStates(u8 *, u32, u8 *);
extern void func_002BEEA0(s32, s32, s32, s32, s32, u32 *, u32 *,
                        s32 *, s32, s32, s32, s32, u32, s32);
extern void func_002BE8A8(s32, s32, s32, s32, s32, s32 *, s32,
                        s32, f32, s32, s32, u32, s32);

void func_002BF198(s32 x, s32 y, s32 z, s32 color, u32 flags,
                  EffectSlotSet *set, s32 slotIndex, BdWork *draw, s32 buffer) {
    s32 colors[4];
    EffectSlotDescription *description = &set->descriptions[slotIndex];
    BdWork *work;
    u32 texture = set->textures[description->textureIndex];
    s32 flip;
    s32 kind;
    s32 mode;
    u32 i;

    if (texture == 0) {
        return;
    }
    work = set->workEntries;
    work += slotIndex;
    slotIndex += work->slotOffset;
    if (!(flags & 0x20)) {
        sdfSubmitGsTestOneRegisterPacket(0x5101BL, buffer);
    }
    if (!(flags & 1)) {
        x += description->xOffset * 16;
        y += description->yOffset * 8;
    }
    if (flags & 0x10) {
        x -= draw->unk04;
        y -= draw->unk08;
    } else {
        x += draw->unk04;
        y += draw->unk08;
    }
    if (description->flags & 4) {
        flip = 1;
    } else {
        flip = 0;
    }
    mode = 16;
    if (!(flags & 0x40)) {
        mode = description->presetMode;
    }
    kind = description->flags & 3;
    if (description->flags & 8) {
        effSelectPresetAndDispatch(x, y, z, draw->width, draw->height,
                                   color, mode, buffer);
    } else {
        for (i = 0; i < 4; i++) {
            colors[i] = description->colors[i] + draw->colors[i];
        }
        func_002BEEA0(x, y, z, draw->width, draw->height, description->rect,
                     draw->rect, colors, color, kind, mode, flip, texture, buffer);
        func_002BE8A8(x, y, z, draw->width, draw->height, colors, color,
                     kind, draw->alpha, mode, flip, texture, buffer);
    }
    if (flags & 0x80) {
        effUpdateTimedStates((u8 *)set, slotIndex, (u8 *)draw);
    }
}
void func_002BF400(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8) {
    func_002BF198(a0, a1, a2, a6 + 0x14, a3, (EffectSlotSet *)a4, a5, (BdWork *)a6, a7);
}
void func_002BF438(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7) {
    func_002BF198(a0, a1, a2, a3, a4, (EffectSlotSet *)a5, a6,
                  (BdWork *)effGetSlotWorkOrOverride(a5, a6), a7);
}



INCLUDE_ASM(const s32, "game/code_002BEDC8", func_002BF4E0);

INCLUDE_ASM(const s32, "game/code_002BEDC8", func_002BF5D8);
