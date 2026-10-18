#ifndef MNU_MANTRA_SPRITE_DRAW_H
#define MNU_MANTRA_SPRITE_DRAW_H

#include "common.h"

/* DDS1 mantra sprite drawing with placement indices and per-axis scales. */
void mnuDrawScaledSpriteAtCoordinatePlacement(s32 x, s32 y, s32 z, s32 alpha,
                                            s32 placementIndex, s32 flags,
                                            f32 scaleX, f32 scaleY, s32 context);
void mnuDrawScaledFixedSpritePlacement(s32 x, s32 y, s32 z, s32 alpha,
                                     s32 placementIndex, s32 flags,
                                     f32 scaleX, f32 scaleY, s32 context);
void mnuDrawVariantSpriteBySelector(s32 x, s32 y, s32 z, s32 alpha,
                                  s32 placementIndex, s32 selector, s32 flags,
                                  f32 scaleX, f32 scaleY, s32 context);
void mnuDrawVariantSpriteAtCoordinatePlacement(s32 x, s32 y, s32 z, s32 alpha,
                                             s32 groupPlacementIndex,
                                             s32 coordinatePlacementIndex,
                                             s32 selector, s32 flags,
                                             f32 scaleX, f32 scaleY, s32 context);
void mnuDrawScaledVariantSpriteByIndex(s32 x, s32 y, s32 z, s32 alpha,
                                     s32 placementIndex, s32 spriteIndex, s32 flags,
                                     f32 scaleX, f32 scaleY, s32 context);

#endif /* MNU_MANTRA_SPRITE_DRAW_H */
