#ifndef DDS3_EFFECT_SPRITE_H
#define DDS3_EFFECT_SPRITE_H

#include "common.h"

/* Sprite vertex record of a column packet: colour, then two corners of the quad. */
typedef struct dds3EffectSpriteCorner_t {
    s32 u, v;
    u8 pad08[8];
    s32 x, y;
    s32 mask;
    s16 flag;
    u8 pad1E[2];
} dds3EffectSpriteCorner_t;

typedef struct dds3EffectSpriteVertex_t {
    s32 r, g, b, a;
    dds3EffectSpriteCorner_t corner[2];
} dds3EffectSpriteVertex_t;

typedef char dds3EffectSpriteCorner_size[(sizeof(dds3EffectSpriteCorner_t) == 0x20) ? 1 : -1];
typedef char dds3EffectSpriteVertex_size[(sizeof(dds3EffectSpriteVertex_t) == 0x50) ? 1 : -1];

#endif /* DDS3_EFFECT_SPRITE_H */
