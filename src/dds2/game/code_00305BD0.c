#include "sdf_gs_alpha_register_api.h"
#include "common.h"
#include "sdf_packet_header.h"
#include "eff_object_node.h"
#include "sdf_trig.h"
#include "sdf_chip.h"
#include "sdf_packet_list.h"
#include "sdf_texture_draw_packet.h"
#include "itf_draw_grid.h"
#include "eff_ref_obj.h"
#include "sdf_heap_api.h"
extern sdfGrOt_t sdfGrOtTable[];

static inline void effSubmitSurfacePacket(sdfGrOt_t *surface, sdfDrawTag_t *list) {
    surface->regFunc(surface, list);
}

void effSelectPresetByKind(u32 mode, u32 value) {
    switch (mode) {
    case 0:
        sdfSubmitGsAlphaOneRegisterPacket(0x44, value);
        return;
    case 1:
        sdfSubmitGsAlphaOneRegisterPacket(0x48, value);
        return;
    case 2:
        sdfSubmitGsAlphaOneRegisterPacket(0x42, value);
        break;
    }
}

typedef struct sdfTexHandle_t sdfTexHandle_t;
typedef struct SdfDrawPacket SdfDrawPacket;

typedef struct EffSpriteUV {
    s32 u0;
    s32 v0;
    s32 u1;
    s32 v1;
} EffSpriteUV;

typedef union EffSpriteColor {
    u32 rgba;
    struct {
        u32 alpha : 8;
        u32 blue : 8;
        u32 green : 8;
        u32 red : 8;
    } channels;
} EffSpriteColor;

#include "sdf_packet_alloc.h"

extern void sdfTexSetFilter(sdfTexHandle_t *, s32, s32);
extern s32 sdf2DGetPrimPacketSize(s32, s32);
extern u8 *sdf2DGetPrimPtr(SdfDrawPacket *packet);
extern void effSelectPresetByKind(u32, u32);
void itfDrawTexturedSpriteRect(s32 x, s32 y, u32 depth, s32 width, s32 height,
                   const EffSpriteUV *uvRect, const EffSpriteColor *color, u32 flipFlags,
                   u32 blendKind, s32 mode, sdfTexHandle_t *texture, s32 surfaceId) {
    u32 textureCoordinates[4];
    void *packet;
    sdfDrawTag_t *packetList;
    u64 *packetWords;
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
    s32 savedCoordinate;

    if (mode == 0) {
        sdfTexSetFilter(texture, 0, 1);
    } else {
        sdfTexSetFilter(texture, 1, 1);
    }
    packet = (void *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(5, 1));
    sdf2DMakePrimPacketHeader(packet, 0x156, 5, 0x43431, 1);
    /* The SDK size helper also skips the packet's two header quadwords. */
    packetWords = (u64 *)sdf2DGetPrimPtr(packet);
    left = x + 0x7000;
    top = y + 0x7900;
    textureCoordinates[0] = uvRect->u0 * 16;
    textureCoordinates[2] = uvRect->u1 * 16;
    textureCoordinates[1] = uvRect->v0 * 16;
    textureCoordinates[3] = uvRect->v1 * 16;
    right = left + width;
    bottom = top + height;
    if (flipFlags & 1) {
        savedCoordinate = left;
        left = right;
        right = savedCoordinate;
    }
    if (flipFlags & 2) {
        savedCoordinate = top;
        top = bottom;
        bottom = savedCoordinate;
    }
    if (color == NULL) {
        packetWords[0] = ((u64)0x80 << 32) | 0x80;
        packetWords[1] = ((u64)0x80 << 32) | 0x80;
    } else {
        u32 rgba = color->rgba;

        packetWords[0] = color->channels.red | ((u64)color->channels.green << 32);
        packetWords[1] = ((rgba >> 8) & 0xFF) | ((u64)(rgba & 0xFF) << 32);
    }
    packetWords[2] = textureCoordinates[0] | ((u64)textureCoordinates[1] << 32);
    packetWords[4] = (u64)(u32)left | ((u64)top << 32);
    packetWords[6] = textureCoordinates[2] | ((u64)textureCoordinates[3] << 32);
    packetWords[8] = (u64)(u32)right | ((u64)bottom << 32);
    packetWords[5] = depth;
    packetWords[9] = depth;
    effSelectPresetByKind(blendKind, surfaceId);
    packetList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(packetList);
    sdf2DLinkTexPacket(packetList, texture, SDF_GS_CONTEXT_1);
    sdfDrawTagLink(packetList, (u32)packet);
    effSubmitSurfacePacket(&sdfGrOtTable[surfaceId], packetList);
    sdfSubmitGsAlphaOneRegisterPacket(0x44, surfaceId);
}


/* Rotate an offset in degrees and add its center; mode selects the screen aspect correction. */
void itfRotateScreenOffset(s32 *outX, s32 *outY, s32 x, s32 y, s32 centerX, s32 centerY, f32 angle, s32 mode) {
    f32 aspect = 1.75f;
    f32 radians = angle * 0.017453293f;
    f32 adjustedY;
    f32 rotatedX;
    f32 rotatedY;
    if (mode == 0) {
        aspect = 2.0f;
    }
    adjustedY = (s32)(y * aspect);
    rotatedX = sdfCos(radians) * x - sdfSin(radians) * adjustedY;
    rotatedY = sdfSin(radians) * x + sdfCos(radians) * adjustedY;
    if (rotatedX > 0.0f) {
        rotatedX += 0.1f;
    } else {
        rotatedX -= 0.1f;
    }
    if (rotatedY > 0.0f) {
        rotatedY += 0.1f;
    } else {
        rotatedY -= 0.1f;
    }
    *outX = (s32)rotatedX;
    *outY = (s32)rotatedY;
    *outX += centerX;
    *outY = (s32)(*outY / aspect + centerY);
}


void sdfDrawTexturedQuad(s32 x, s32 y, u32 depth, s32 width, s32 height,
                   const EffSpriteUV *uvRect, const EffSpriteColor *cornerColors,
                   u32 flipFlags, f32 angle, u32 blendKind, s32 mode, s32 aspectMode,
                   sdfTexHandle_t *texture, s32 surfaceId) {
    u32 textureCoordinates[4];
    /* The four corners stay separate locals: itfRotateScreenOffset writes each
     * coordinate through its own pointer. */
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
    s32 x2;
    s32 y2;
    s32 x3;
    s32 y3;
    SdfDrawPacket *packet;
    sdfDrawTag_t *packetList;
    u64 *packetWords;
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
    s32 halfWidth;
    s32 halfHeight;
    s32 centerX;
    s32 centerY;
    s32 savedCoordinate;

    if (mode == 0) {
        sdfTexSetFilter(texture, 0, 1);
    } else {
        sdfTexSetFilter(texture, 1, 1);
    }
    packet = (SdfDrawPacket *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(3, 4));
    sdf2DMakePrimPacketHeader(packet, 0x15C, 3, 0x531, 4);
    packetWords = (u64 *)sdf2DGetPrimPtr(packet);
    left = x + 0x7000;
    top = y + 0x7900;
    right = left + width - 8;
    bottom = top + height - 4;
    halfWidth = (right - left) / 2;
    halfHeight = (bottom - top) / 2;
    centerX = left + halfWidth;
    centerY = top + halfHeight;
    x0 = left;
    y0 = top;
    x3 = right;
    y3 = bottom;
    if (angle == 0.0f) {
        x1 = right;
        y1 = top;
        x2 = left;
        y2 = bottom;
    } else {
        itfRotateScreenOffset(&x0, &y0, -halfWidth, -halfHeight, centerX, centerY, angle, aspectMode);
        itfRotateScreenOffset(&x1, &y1, halfWidth, -halfHeight, centerX, centerY, angle, aspectMode);
        itfRotateScreenOffset(&x2, &y2, -halfWidth, halfHeight, centerX, centerY, angle, aspectMode);
        itfRotateScreenOffset(&x3, &y3, halfWidth, halfHeight, centerX, centerY, angle, aspectMode);
    }
    textureCoordinates[0] = uvRect->u0 * 16;
    textureCoordinates[1] = uvRect->v0 * 16;
    textureCoordinates[2] = uvRect->u1 * 16;
    textureCoordinates[3] = uvRect->v1 * 16;
    if (flipFlags & 1) {
        savedCoordinate = x0;
        x0 = x1;
        x1 = savedCoordinate;
        savedCoordinate = x2;
        x2 = x3;
        x3 = savedCoordinate;
    }
    if (flipFlags & 2) {
        savedCoordinate = y0;
        y0 = y2;
        y2 = savedCoordinate;
        savedCoordinate = y1;
        y1 = y3;
        y3 = savedCoordinate;
    }
    if (cornerColors == NULL) {
        packetWords[0] = ((u64)0x80 << 32) | 0x80;
        packetWords[1] = ((u64)0x80 << 32) | 0x80;
        packetWords[6] = ((u64)0x80 << 32) | 0x80;
        packetWords[7] = ((u64)0x80 << 32) | 0x80;
        packetWords[12] = ((u64)0x80 << 32) | 0x80;
        packetWords[13] = ((u64)0x80 << 32) | 0x80;
        packetWords[18] = ((u64)0x80 << 32) | 0x80;
        packetWords[19] = ((u64)0x80 << 32) | 0x80;
    } else {
        u32 rgba0 = cornerColors[0].rgba;
        u32 rgba1 = cornerColors[1].rgba;
        u32 rgba2 = cornerColors[2].rgba;
        u32 rgba3 = cornerColors[3].rgba;

        packetWords[0] = cornerColors[0].channels.red | ((u64)cornerColors[0].channels.green << 32);
        packetWords[1] = ((rgba0 >> 8) & 0xFF) | ((u64)(rgba0 & 0xFF) << 32);
        packetWords[6] = cornerColors[1].channels.red | ((u64)cornerColors[1].channels.green << 32);
        packetWords[7] = ((rgba1 >> 8) & 0xFF) | ((u64)(rgba1 & 0xFF) << 32);
        packetWords[12] = cornerColors[2].channels.red | ((u64)cornerColors[2].channels.green << 32);
        packetWords[13] = ((rgba2 >> 8) & 0xFF) | ((u64)(rgba2 & 0xFF) << 32);
        packetWords[18] = cornerColors[3].channels.red | ((u64)cornerColors[3].channels.green << 32);
        packetWords[19] = ((rgba3 >> 8) & 0xFF) | ((u64)(rgba3 & 0xFF) << 32);
    }
    /* Six u64 per vertex: RGBAQ, UV, then XYZ2 (the 0x531 register list). */
    packetWords[2] = textureCoordinates[0] | ((u64)textureCoordinates[1] << 32);
    packetWords[4] = (u64)(u32)x0 | ((u64)(u32)y0 << 32);
    packetWords[5] = depth >> 4;
    packetWords[8] = textureCoordinates[2] | ((u64)textureCoordinates[1] << 32);
    packetWords[10] = (u64)(u32)x1 | ((u64)(u32)y1 << 32);
    packetWords[11] = depth >> 4;
    packetWords[14] = textureCoordinates[0] | ((u64)textureCoordinates[3] << 32);
    packetWords[16] = (u64)(u32)x2 | ((u64)(u32)y2 << 32);
    packetWords[17] = depth >> 4;
    packetWords[20] = textureCoordinates[2] | ((u64)textureCoordinates[3] << 32);
    packetWords[22] = (u64)(u32)x3 | ((u64)(u32)y3 << 32);
    packetWords[23] = depth >> 4;
    effSelectPresetByKind(blendKind, surfaceId);
    packetList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(packetList);
    sdf2DLinkTexPacket(packetList, texture, SDF_GS_CONTEXT_1);
    sdfDrawTagLink(packetList, (u32)packet);
    effSubmitSurfacePacket(&sdfGrOtTable[surfaceId], packetList);
    sdfSubmitGsAlphaOneRegisterPacket(0x44, surfaceId);
}


void itfDrawRotatedTexturedRect(s32 x, s32 y, s32 depth, s32 width, s32 height,
                               const u32 *textureCoordinates, const u32 *cornerColors,
                               u32 flipFlags, f32 rotation, u32 blendKind, u32 textureMode,
                               sdfTexHandle_t *texture, u32 surfaceId) {
    sdfDrawTexturedQuad(x, y, depth, width, height, (const EffSpriteUV *)textureCoordinates,
                  (const EffSpriteColor *)cornerColors, flipFlags, rotation, blendKind, textureMode, 1,
                  texture, surfaceId);
}

extern void uiDrawGradientColorRect(u32, u32, u32, u32, u32, const u32 *, u32);

void effSelectPresetAndDispatch(u32 x, u32 y, u32 z, u32 width,
                                    u32 height, u32 vertexColors, u32 presetMode, u32 presetValue) {
    effSelectPresetByKind(presetMode, presetValue);
    uiDrawGradientColorRect(x, y, z, width, height, (const u32 *)vertexColors, presetValue);
    sdfSubmitGsAlphaOneRegisterPacket(0x44, presetValue);
}

