#ifndef EFF_THUNDER_DRAW_PARAMS_H
#define EFF_THUNDER_DRAW_PARAMS_H

#include "common.h"

/* Vertex-draw descriptor used by the thunder history and end-cap packets.
 * Both title providers clear the first 0x2C bytes and keep these globals in
 * 0x30-byte slots; the final four bytes remain opaque. */
typedef struct EffThunderDrawParams {
    s16 primitiveCount; /* 0x00 */
    s16 pointCount;     /* 0x02 */
    u16 flags;          /* 0x04 */
    u8 opaque_06[2];
    u32 color;          /* 0x08 */
    const u32 *primitiveIndices; /* 0x0C */
    u128 *points;       /* 0x10 */
    u8 opaque_14[0xC];
    u32 *colors;        /* 0x20 */
    u8 opaque_24[0xC];
} EffThunderDrawParams;

typedef char EffThunderDrawParams_primitiveCount_at_0x0[((u32)&((EffThunderDrawParams *)0)->primitiveCount == 0x0) ? 1 : -1];
typedef char EffThunderDrawParams_pointCount_at_0x2[((u32)&((EffThunderDrawParams *)0)->pointCount == 0x2) ? 1 : -1];
typedef char EffThunderDrawParams_flags_at_0x4[((u32)&((EffThunderDrawParams *)0)->flags == 0x4) ? 1 : -1];
typedef char EffThunderDrawParams_color_at_0x8[((u32)&((EffThunderDrawParams *)0)->color == 0x8) ? 1 : -1];
typedef char EffThunderDrawParams_primitiveIndices_at_0xC[((u32)&((EffThunderDrawParams *)0)->primitiveIndices == 0xC) ? 1 : -1];
typedef char EffThunderDrawParams_points_at_0x10[((u32)&((EffThunderDrawParams *)0)->points == 0x10) ? 1 : -1];
typedef char EffThunderDrawParams_colors_at_0x20[((u32)&((EffThunderDrawParams *)0)->colors == 0x20) ? 1 : -1];

#endif
