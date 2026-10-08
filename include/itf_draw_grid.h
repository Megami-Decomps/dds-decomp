#ifndef ITF_DRAW_GRID_H
#define ITF_DRAW_GRID_H

#include "common.h"

struct EffectSlotSet;

#ifdef VERSION_DDS2
void itfDrawGridWithResolvedSlot(u32 offsetX, u32 offsetY, u32 z,
                                 u32 drawFlags, struct EffectSlotSet *object,
                                 u32 index, u32 surfaceIndex);
#else
void itfDrawGridWithResolvedSlot(s32 offsetX, s32 offsetY, s32 z,
                                 s32 drawFlags, struct EffectSlotSet *object,
                                 s32 index, s32 surfaceIndex);
#endif

#endif
