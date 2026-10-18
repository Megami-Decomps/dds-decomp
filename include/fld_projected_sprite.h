#ifndef FLD_PROJECTED_SPRITE_H
#define FLD_PROJECTED_SPRITE_H

#include "common.h"

/* A map icon's rectangle in its texture, the anchor it rotates about, and
 * the texture slot it draws from. DDS1 and DDS2 share the layout. */
typedef struct {
    s16 u;
    s16 v;
    s16 width;
    s16 height;
    s16 anchorX;
    s16 anchorY;
    s32 textureSlot;
} FldProjectedSprite;

#endif
