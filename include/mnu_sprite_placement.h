#ifndef MNU_SPRITE_PLACEMENT_H
#define MNU_SPRITE_PLACEMENT_H

#include "common.h"

/* Menu sprite placement table entry shared by mantra and currency drawing. */
typedef struct MnuSpritePlacement {
    s16 resourceIndex;
    s16 spriteIndex;
    s16 x;
    s16 y;
} MnuSpritePlacement;

typedef char MnuSpritePlacementSizeCheck[
    (sizeof(MnuSpritePlacement) == 0x08) ? 1 : -1];
typedef char MnuSpritePlacementSpriteIndexOffsetCheck[
    ((u32)&((MnuSpritePlacement *)0)->spriteIndex == 0x02) ? 1 : -1];
typedef char MnuSpritePlacementXOffsetCheck[
    ((u32)&((MnuSpritePlacement *)0)->x == 0x04) ? 1 : -1];
typedef char MnuSpritePlacementYOffsetCheck[
    ((u32)&((MnuSpritePlacement *)0)->y == 0x06) ? 1 : -1];

#endif
