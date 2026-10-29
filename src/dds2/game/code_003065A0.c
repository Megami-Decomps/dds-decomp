#include "common.h"
#include "ui_color.h"
#include "eff.h"

extern void *effGetSlotWorkOrOverride(EffectSlotSet *, s32);

extern void effDrawTextureSlot(s32, s32, s32, u32 *, s32, EffectSlotSet *, s32, void *, s32);
extern void effDrawTextureSlotWithBlendedCornerColors(s32, s32, s32, u32, s32, EffectSlotSet *, s32, s32);

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

extern void itfDrawRotatedTexturedRect(s32 x, s32 y, s32 depth, s32 width, s32 height,
                                      const u32 *textureCoordinates, const u32 *cornerColors,
                                      u32 flipFlags, f32 rotation, u32 blendKind, u32 textureMode,
                                      sdfTexHandle_t *texture, u32 surfaceId);

/* Draw enabled border strips and restore the temporary texture-coordinate changes. */
void effDrawTexturedRectBorderStrips(s32 x, s32 y, s32 depth, s32 width, s32 height,
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
                                   textureCoordinates, cornerColors,
                                   kind, 0.0f, mode, flip,
                                   (sdfTexHandle_t *)texture, buffer);
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
                                   textureCoordinates, cornerColors,
                                   kind, 0.0f, mode, flip,
                                   (sdfTexHandle_t *)texture, buffer);
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
                                   textureCoordinates, cornerColors,
                                   kind, 0.0f, mode, flip,
                                   (sdfTexHandle_t *)texture, buffer);
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
                                   textureCoordinates, cornerColors,
                                   kind, 0.0f, mode, flip,
                                   (sdfTexHandle_t *)texture, buffer);
        textureCoordinates[0] = savedTextureCoordinates[0];
    }
}

#ifdef NON_MATCHING
#include "common.h"
#include "ui_color.h"
#include "eff.h"
#include "sdf_gs_register_submit_api.h"

extern void effSelectPresetAndDispatch(u32, u32, u32, u32, u32, u32, u32, u32);
extern EffectSlotSet *effUpdateTimedStates(EffectSlotSet *, u32, void *);
extern void effDrawTexturedRectBorderStrips(s32 x, s32 y, s32 depth, s32 width, s32 height,
                                            const u32 *edgeExtents, u8 *edgeColorBytes,
                                            u32 *textureCoordinates, u32 *sourceCornerColors, u32 kind,
                                            u32 mode, u32 flip, u32 texture, u32 buffer);
/* DDS1's, with its corners' aspect mode before the texture. */
extern void sdfDrawTexturedQuad(s32, s32, s32, s32, s32, const u32 *, const u32 *, u32, f32, u32, u32, u32,
                                sdfTexHandle_t *, u32);

/* Draw an effect slot's texture at its place, through its preset or as a
 * bordered quad, and advance its timed states where asked. DDS1's
 * effDrawTextureSlot (upstream C in game/code_002BEDC8), its quad's aspect
 * mode whether bit 1 of the flags is clear. Upstream: INCLUDE_ASM in
 * game/code_003065A0; the same calls and arguments as the retail
 * routine. As upstream C (DDS1's with that argument) it differs from retail
 * in register allocation only, 74 of 160 words. */
void effDrawTextureSlot(s32 x, s32 y, s32 z, u32 *color, s32 flags, EffectSlotSet *set, s32 slotIndex,
                        void *drawWork, s32 buffer) {
    s32 colors[4];
    EffectSlotDescription *description = &set->descriptions[slotIndex];
    BdWork *draw = drawWork;
    BdWork *work;
    u32 texture = (u32)set->textureReferences[description->textureIndex];
    s32 flip;
    s32 kind;
    s32 mode;
    s32 aspectMode;
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
        x += description->bounds[0] * 16;
        y += description->bounds[1] * 8;
    }
    if (flags & 0x10) {
        x -= draw->geometry.bounds[0];
        y -= draw->geometry.bounds[1];
    } else {
        x += draw->geometry.bounds[0];
        y += draw->geometry.bounds[1];
    }
    flip = (description->flags & 4) != 0;
    mode = 16;
    if (!(flags & 0x40)) {
        mode = description->presetMode;
    }
    aspectMode = (flags & 2) == 0;
    kind = description->flags & 3;
    if (description->flags & 8) {
        effSelectPresetAndDispatch(x, y, z, draw->geometry.bounds[2], draw->geometry.bounds[3], (u32)color, mode,
                                   buffer);
    } else {
        for (i = 0; i < 4; i++) {
            colors[i] = description->colors[i] + draw->parameters[i];
        }
        effDrawTexturedRectBorderStrips(x, y, z, draw->geometry.bounds[2], draw->geometry.bounds[3],
                                        description->rect, (u8 *)draw->bounds.texture.rect, (u32 *)colors, color,
                                        kind, mode, flip, texture, buffer);
        sdfDrawTexturedQuad(x, y, z, draw->geometry.bounds[2], draw->geometry.bounds[3], (const u32 *)colors,
                            color, kind, draw->geometry.angleDegrees, mode, flip, aspectMode, (sdfTexHandle_t *)texture, buffer);
    }
    if (flags & 0x80) {
        effUpdateTimedStates(set, slotIndex, draw);
    }
}
#else
INCLUDE_ASM(const s32, "game/code_003065A0", effDrawTextureSlot);
#endif

void effDrawSpriteWithWorkCornerColors(s32 x, s32 y, s32 z, s32 flags, EffectSlotSet *set, s32 slot, BdWork *entry, s32 layer) {
    effDrawTextureSlot(x, y, z, entry->geometry.cornerColors, flags, set, slot, entry, layer);
}
void effDrawSpriteWithCornerColors(s32 x, s32 y, s32 z, u32 *palette, s32 flags, EffectSlotSet *set, s32 slot, s32 layer) {
    effDrawTextureSlot(x, y, z, palette, flags, set, slot,
                  effGetSlotWorkOrOverride(set, slot), layer);
}



void effDrawTextureSlotWithBlendedCornerColors(s32 x, s32 y, s32 depth, u32 blend, s32 flags,
                   EffectSlotSet *set, s32 slotIndex, s32 layer) {
    u32 colors[4];
    void *payload = effGetSlotWorkOrOverride(set, slotIndex);
    EffectSlotGeometry *geometry = (EffectSlotGeometry *)((u8 *)payload + 4);
    s32 i;

    for (i = 0; i < 4; i++) {
        colors[i] = fclBlendColor(geometry->cornerColors[i],
                                  geometry->cornerColors[i] & 0xFFFFFF00, blend);
    }
    effDrawTextureSlot(x, y, depth, colors, flags, set, slotIndex, payload, layer);
}

void effDrawRepeatedSpriteTiles(s32 x, s32 y, s32 z, s32 length, u32 weight, s32 flags,
                  EffectSlotSet *set, s32 slot, s32 layer) {
    u32 colors[4];
    void *draw;
    s32 tileWidth;
    s32 i;

    draw = effGetSlotWorkOrOverride(set, slot);
    for (i = 0; i < 4; i++) {
        u32 color = ((EffectSlotGeometry *)((u8 *)draw + 4))->cornerColors[i];
        colors[i] = fclBlendColor(color, color & ~0xFF, weight);
    }
    {
        /* Start with the complete tile; draw callbacks may replace the work array. */
        set->workEntries[slot].parameters[2] = 0;
        tileWidth = set->workEntries[slot].sourceWidth * 16;
        set->workEntries[slot].geometry.bounds[2] = tileWidth;
    }
    for (i = 0; i < length / tileWidth; i++) {
        effDrawTextureSlot(x, y, z, colors, flags, set, slot, draw, layer);
        x += tileWidth;
    }
    {
        /* Publish the crop offset, then the visible width, before the final draw. */
        length = tileWidth - length % tileWidth;
        set->workEntries[slot].parameters[2] = -(length >> 4);
        set->workEntries[slot].geometry.bounds[2] = tileWidth - length;
    }
    effDrawTextureSlot(x, y, z, colors, flags, set, slot, draw, layer);
}
