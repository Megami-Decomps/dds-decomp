#ifndef FLD_SPRITE_VERTEX_H
#define FLD_SPRITE_VERTEX_H

#include "common.h"

/* One corner in the 0x50-byte payload of a single-column sprite packet.
 * The producer writes texture coordinates, screen coordinates, mask and flag;
 * the two intervening spans remain opaque. */
typedef struct FldSpriteCorner {
    s32 u;
    s32 v;
    u8 pad08[8];
    s32 x;
    s32 y;
    s32 mask;
    s16 flag;
    u8 pad1E[2];
} FldSpriteCorner;

typedef struct FldSpriteVertex {
    s32 r;
    s32 g;
    s32 b;
    s32 a;
    FldSpriteCorner corner[2];
} FldSpriteVertex;

typedef char FldSpriteCornerSizeCheck[(sizeof(FldSpriteCorner) == 0x20) ? 1 : -1];
typedef char FldSpriteCornerUOffsetCheck[
    ((u32)&((FldSpriteCorner *)0)->u == 0x00) ? 1 : -1];
typedef char FldSpriteCornerVOffsetCheck[
    ((u32)&((FldSpriteCorner *)0)->v == 0x04) ? 1 : -1];
typedef char FldSpriteCornerXOffsetCheck[
    ((u32)&((FldSpriteCorner *)0)->x == 0x10) ? 1 : -1];
typedef char FldSpriteCornerYOffsetCheck[
    ((u32)&((FldSpriteCorner *)0)->y == 0x14) ? 1 : -1];
typedef char FldSpriteCornerMaskOffsetCheck[
    ((u32)&((FldSpriteCorner *)0)->mask == 0x18) ? 1 : -1];
typedef char FldSpriteCornerFlagOffsetCheck[
    ((u32)&((FldSpriteCorner *)0)->flag == 0x1C) ? 1 : -1];
typedef char FldSpriteVertexSizeCheck[(sizeof(FldSpriteVertex) == 0x50) ? 1 : -1];
typedef char FldSpriteVertexColorOffsetCheck[
    ((u32)&((FldSpriteVertex *)0)->r == 0x00) ? 1 : -1];
typedef char FldSpriteVertexAlphaOffsetCheck[
    ((u32)&((FldSpriteVertex *)0)->a == 0x0C) ? 1 : -1];
typedef char FldSpriteVertexCornersOffsetCheck[
    ((u32)&((FldSpriteVertex *)0)->corner == 0x10) ? 1 : -1];

#endif /* FLD_SPRITE_VERTEX_H */
