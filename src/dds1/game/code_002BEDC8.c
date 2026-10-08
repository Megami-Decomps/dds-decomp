#include "common.h"
#include "eff.h"

extern void *effGetSlotWorkOrOverride(EffectSlotSet *, s32);

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


extern void func_002BE8A8(s32, s32, s32, s32, s32, const u32 *, const u32 *, u32,
                          f32, u32, u32, u32, u32);

/* Draw enabled border strips and restore the temporary texture-coordinate changes. */
void func_002BEEA0(s32 x, s32 y, s32 depth, s32 width, s32 height,
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
        func_002BE8A8(position[0], position[1], depth,
                      dimensions[0], dimensions[1],
                      textureCoordinates, cornerColors,
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
        func_002BE8A8(position[0], position[1], depth,
                      dimensions[0], dimensions[1],
                      textureCoordinates, cornerColors,
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
        func_002BE8A8(position[0], position[1], depth,
                      dimensions[0], dimensions[1],
                      textureCoordinates, cornerColors,
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
        func_002BE8A8(position[0], position[1], depth,
                      dimensions[0], dimensions[1],
                      textureCoordinates, cornerColors,
                      kind, 0.0f, mode, flip,
                      texture, buffer);
        textureCoordinates[0] = savedTextureCoordinates[0];
    }
}



extern void sdfSubmitGsTestOneRegisterPacket();
extern void effSelectPresetAndDispatch(u32, u32, u32, u32, u32, u32, u32, u32);
extern EffectSlotSet *effUpdateTimedStates(EffectSlotSet *, u32, void *);


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
        x -= draw->geometry.bounds[0];
        y -= draw->geometry.bounds[1];
    } else {
        x += draw->geometry.bounds[0];
        y += draw->geometry.bounds[1];
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
        effSelectPresetAndDispatch(x, y, z, draw->geometry.bounds[2], draw->geometry.bounds[3],
                                   color, mode, buffer);
    } else {
        for (i = 0; i < 4; i++) {
            colors[i] = description->colors[i] + draw->parameters[i];
        }
        func_002BEEA0(x, y, z, draw->geometry.bounds[2], draw->geometry.bounds[3], description->rect,
                     (u8 *)draw->bounds.texture.rect, (u32 *)colors, (u32 *)color,
                     kind, mode, flip, texture, buffer);
        func_002BE8A8(x, y, z, draw->geometry.bounds[2], draw->geometry.bounds[3],
                     (const u32 *)colors, (const u32 *)color,
                     kind, draw->geometry.angleDegrees, mode, flip, texture, buffer);
    }
    if (flags & 0x80) {
        effUpdateTimedStates(set, slotIndex, draw);
    }
}
void func_002BF400(s32 x, s32 y, s32 z, s32 flags, EffectSlotSet *set, s32 slot, BdWork *entry, s32 layer) {
    effDrawTextureSlot(x, y, z, (s32)entry->geometry.cornerColors, flags, set, slot, entry, layer);
}
void func_002BF438(s32 x, s32 y, s32 z, u32 *palette, s32 flags, EffectSlotSet *set, s32 slot, s32 layer) {
    effDrawTextureSlot(x, y, z, (s32)palette, flags, set, slot,
                  (BdWork *)effGetSlotWorkOrOverride(set, slot), layer);
}



extern u32 uiBlendColors(u32, u32, u32);

void func_002BF4E0(s32 x, s32 y, s32 depth, u32 blend, s32 flags,
                   EffectSlotSet *set, s32 slotIndex, s32 layer) {
    u32 colors[4];
    BdWork *work = effGetSlotWorkOrOverride(set, slotIndex);
    s32 i;

    for (i = 0; i < 4; i++) {
        colors[i] = uiBlendColors(work->geometry.cornerColors[i],
                                  work->geometry.cornerColors[i] & 0xFFFFFF00, blend);
    }
    effDrawTextureSlot(x, y, depth, (s32)colors, (u32)flags, set, slotIndex, work, layer);
}

INCLUDE_ASM(const s32, "game/code_002BEDC8", func_002BF5D8);
