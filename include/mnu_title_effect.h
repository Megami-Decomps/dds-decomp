#ifndef MNU_TITLE_EFFECT_H
#define MNU_TITLE_EFFECT_H

#include "common.h"

typedef struct TitleEffectState {
    s32 soundNameIndex;
    s32 frameCounter;
} TitleEffectState;

typedef char TitleEffectState_size_must_be_0x08[
    (sizeof(TitleEffectState) == 0x08 &&
     (u32)&((TitleEffectState *)0)->soundNameIndex == 0x00 &&
     (u32)&((TitleEffectState *)0)->frameCounter == 0x04)
        ? 1 : -1];

#endif /* MNU_TITLE_EFFECT_H */
