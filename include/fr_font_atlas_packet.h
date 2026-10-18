#ifndef FR_FONT_ATLAS_PACKET_H
#define FR_FONT_ATLAS_PACKET_H

#include "common.h"
#include "fr_font.h"

/* One buffered atlas preview: TEX0/TEX1/CLAMP, then a four-corner textured strip. */
typedef struct FrFontAtlasCorner {
    FrFontGsWrite uv;
    FrFontGsWrite rgbaq;
    FrFontGsWrite xyz;
} FrFontAtlasCorner;

typedef struct FrFontAtlasDraw {
    u64 gifTag;
    u64 gifRegisters;
    FrFontGsWrite tex0;
    FrFontGsWrite tex1;
    FrFontGsWrite clamp;
    FrFontAtlasCorner corners[4];
} FrFontAtlasDraw;

typedef struct FrFontAtlasPacket {
    FrFontDmaTag dma;
    FrFontAtlasDraw draw;
} FrFontAtlasPacket;

typedef char FrFontAtlasPacketSizeCheck[(sizeof(FrFontAtlasPacket) == 0x110) ? 1 : -1];

#endif
