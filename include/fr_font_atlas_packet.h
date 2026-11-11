#ifndef FR_FONT_ATLAS_PACKET_H
#define FR_FONT_ATLAS_PACKET_H

#include "common.h"
#include "fr_font.h"

/* One buffered atlas preview: TEX0/TEX1/CLAMP, then a four-corner textured strip. */
typedef struct FrFontAtlasCorner {
    frFontGsWrite_t uv;
    frFontGsWrite_t rgbaq;
    frFontGsWrite_t xyz;
} FrFontAtlasCorner;

typedef struct FrFontAtlasDraw {
    u64 gifTag;
    u64 gifRegisters;
    frFontGsWrite_t tex0;
    frFontGsWrite_t tex1;
    frFontGsWrite_t clamp;
    FrFontAtlasCorner corners[4];
} FrFontAtlasDraw;

typedef struct FrFontAtlasPacket {
    frFontDmaTag_t dma;
    FrFontAtlasDraw draw;
} FrFontAtlasPacket;

typedef char FrFontAtlasPacketSizeCheck[(sizeof(FrFontAtlasPacket) == 0x110) ? 1 : -1];

#endif
