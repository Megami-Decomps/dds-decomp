#ifndef DDS_SDF_TEX_PACKET_H
#define DDS_SDF_TEX_PACKET_H

#include "common.h"

struct sdfTexHandle_t;
struct sdfTexPacket_t;

/* Store the variant-zero resource packet on the texture's primary buffer. */
void sdfTexMakeGIFPacket(struct sdfTexHandle_t *texture);

/* Build the 0x40-byte texture-state packet from the texture dimensions,
 * resource words, GS format/LOD settings and packet variant. */
void sdfTexSetPacket(struct sdfTexPacket_t *packet, s32 width, s32 height,
    u32 primaryResourceWord, s32 pixelFormat, u32 secondaryResourceWord,
    s32 clutFormat, s32 textureColorComponents, s32 maxMipLevel,
    s32 lodParameters, s32 clampMode, s32 variant);

#endif
