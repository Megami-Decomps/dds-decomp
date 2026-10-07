#ifndef KWLN_SPRITE_H
#define KWLN_SPRITE_H

#include "common.h"

/* Sprite vertex record of a column packet: colour, then two corners of the quad. */
typedef struct KwlnSpriteCorner {
    s32 u, v;
    u8 pad08[8];
    s32 x, y;
    s32 mask;
    s16 flag;
    u8 pad1E[2];
} KwlnSpriteCorner;

typedef struct KwlnSpriteVertex {
    s32 r, g, b, a;
    KwlnSpriteCorner corner[2];
} KwlnSpriteVertex;

typedef char KwlnSpriteCornerSize[(sizeof(KwlnSpriteCorner) == 0x20) ? 1 : -1];
typedef char KwlnSpriteVertexSize[(sizeof(KwlnSpriteVertex) == 0x50) ? 1 : -1];

#endif /* KWLN_SPRITE_H */
