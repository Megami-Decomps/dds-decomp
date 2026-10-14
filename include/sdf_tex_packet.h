#ifndef DDS_SDF_TEX_PACKET_H
#define DDS_SDF_TEX_PACKET_H

#include "common.h"

struct SdfTex;
struct SdfTexBuf;

/* Store the variant-zero resource packet on the texture's primary buffer. */
void sdfTexCreateFirstPacket(struct SdfTex *texture);

/* Build the 0x40-byte texture-state packet from the texture dimensions,
 * resource words, GS format/LOD settings and packet variant. */
void sdfBuildTextureStatePacket(struct SdfTexBuf *packet, s32 width, s32 height,
    u32 primaryResourceWord, s32 pixelFormat, u32 secondaryResourceWord,
    s32 clutFormat, s32 textureColorComponents, s32 maxMipLevel,
    s32 lodParameters, s32 clampMode, s32 variant);

#endif
