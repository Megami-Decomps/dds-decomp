#include "common.h"
#include "eff.h"

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


extern void sdfSubmitGsTestOneRegisterPacket();
extern void effSelectPresetAndDispatch(u32, u32, u32, u32, u32, u32, u32, u32);
extern EffectSlotSet *effUpdateTimedStates(EffectSlotSet *, u32, BdWork *);
extern void func_002BEEA0(s32, s32, s32, s32, s32, u32 *, u32 *,
                        s32 *, s32, s32, s32, s32, u32, s32);
extern void func_002BE8A8(s32, s32, s32, s32, s32, s32 *, s32,
                        s32, f32, s32, s32, u32, s32);

void effDrawTextureSlot(s32 x, s32 y, s32 z, s32 color, u32 flags,
                  EffectSlotSet *set, s32 slotIndex, BdWork *draw, s32 buffer) {
    s32 colors[4];
    EffectSlotDescription *description = &set->descriptions[slotIndex];
    BdWork *work;
    u32 texture = (u32)set->handles[description->textureIndex];
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
        x -= draw->xOffset;
        y -= draw->yOffset;
    } else {
        x += draw->xOffset;
        y += draw->yOffset;
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
            colors[i] = description->colors[i] + draw->parameters[i];
        }
        func_002BEEA0(x, y, z, draw->width, draw->height, description->rect,
                     draw->bounds.texture.rect, colors, color, kind, mode, flip, texture, buffer);
        func_002BE8A8(x, y, z, draw->width, draw->height, colors, color,
                     kind, draw->angleDegrees, mode, flip, texture, buffer);
    }
    if (flags & 0x80) {
        effUpdateTimedStates(set, slotIndex, draw);
    }
}
void func_002BF400(s32 x, s32 y, s32 z, s32 flags, EffectSlotSet *set, s32 slot, BdWork *entry, s32 layer) {
    effDrawTextureSlot(x, y, z, (s32)entry->cornerColors, flags, set, slot, entry, layer);
}
void func_002BF438(s32 x, s32 y, s32 z, u32 *palette, s32 flags, EffectSlotSet *set, s32 slot, s32 layer) {
    effDrawTextureSlot(x, y, z, (s32)palette, flags, set, slot,
                  (BdWork *)effGetSlotWorkOrOverride((s32)set, slot), layer);
}



INCLUDE_ASM(const s32, "game/code_002BEDC8", func_002BF4E0);

INCLUDE_ASM(const s32, "game/code_002BEDC8", func_002BF5D8);
