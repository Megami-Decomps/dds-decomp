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

extern void itfDrawRotatedTexturedRect(u32, u32, u32, u32, u32, u32, u32, u32,
                                      f32, u32, u32, u32, u32);

/* Draw enabled border strips and restore the temporary texture-coordinate changes. */
void func_00306678(s32 x, s32 y, s32 depth, s32 width, s32 height,
                  const u32 *edgeExtents, u8 *edgeColorBytes,
                  u32 *textureCoordinates, u32 *sourceCornerColors, u32 kind,
                  u32 mode, u32 flip, u32 texture, u32 buffer) {
    s32 position[2];
    s32 dimensions[2];
    u32 savedTextureCoordinates[4];
    u32 cornerColors[4];
    s32 i;

    for (i = 0; i < 4; i++) {
        savedTextureCoordinates[i] = textureCoordinates[i];
    }

    if (edgeExtents[0] != 0) {
        position[0] = x;
        position[1] = y - (edgeExtents[0] << 3);
        dimensions[0] = width;
        dimensions[1] = edgeExtents[0] << 3;
        textureCoordinates[3] = textureCoordinates[1] + 1;
        sdfCopyCornerWordsWithEdgeByte(sourceCornerColors, cornerColors,
                                       edgeColorBytes, 0);
        itfDrawRotatedTexturedRect(position[0], position[1], depth,
                                   dimensions[0], dimensions[1],
                                   (u32)textureCoordinates, (u32)cornerColors,
                                   kind, 0.0f, mode, flip,
                                   texture, buffer);
        textureCoordinates[3] = savedTextureCoordinates[3];
    }

    if (edgeExtents[1] != 0) {
        position[0] = x;
        position[1] = y + height;
        dimensions[0] = width;
        dimensions[1] = edgeExtents[1] << 3;
        textureCoordinates[1] = textureCoordinates[3] - 1;
        sdfCopyCornerWordsWithEdgeByte(sourceCornerColors, cornerColors,
                                       edgeColorBytes, 1);
        itfDrawRotatedTexturedRect(position[0], position[1], depth,
                                   dimensions[0], dimensions[1],
                                   (u32)textureCoordinates, (u32)cornerColors,
                                   kind, 0.0f, mode, flip,
                                   texture, buffer);
        textureCoordinates[1] = savedTextureCoordinates[1];
    }

    if (edgeExtents[2] != 0) {
        position[0] = x - (edgeExtents[2] << 4);
        position[1] = y;
        dimensions[0] = edgeExtents[2] << 4;
        dimensions[1] = height;
        textureCoordinates[2] = textureCoordinates[0] + 1;
        sdfCopyCornerWordsWithEdgeByte(sourceCornerColors, cornerColors,
                                       edgeColorBytes, 2);
        itfDrawRotatedTexturedRect(position[0], position[1], depth,
                                   dimensions[0], dimensions[1],
                                   (u32)textureCoordinates, (u32)cornerColors,
                                   kind, 0.0f, mode, flip,
                                   texture, buffer);
        textureCoordinates[2] = savedTextureCoordinates[2];
    }

    if (edgeExtents[3] != 0) {
        position[0] = x + width;
        position[1] = y;
        dimensions[0] = edgeExtents[3] << 4;
        dimensions[1] = height;
        textureCoordinates[0] = textureCoordinates[2] - 1;
        sdfCopyCornerWordsWithEdgeByte(sourceCornerColors, cornerColors,
                                       edgeColorBytes, 3);
        itfDrawRotatedTexturedRect(position[0], position[1], depth,
                                   dimensions[0], dimensions[1],
                                   (u32)textureCoordinates, (u32)cornerColors,
                                   kind, 0.0f, mode, flip,
                                   texture, buffer);
        textureCoordinates[0] = savedTextureCoordinates[0];
    }
}

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
