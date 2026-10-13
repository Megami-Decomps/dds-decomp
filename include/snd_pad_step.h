#ifndef SND_PAD_STEP_H
#define SND_PAD_STEP_H

#include "itf.h"

/* Inspector overlay: this view crosses ItfMesBlkA4 into adjacent window state. */
typedef struct SndPadStepper {
    u8 opaque00[4];
    UiSprite *target; /* 0x04 */
    u8 opaque08[0x20];
    s32 index; /* 0x28 */
} SndPadStepper;

typedef char SndPadStepper_target_offset_must_be_0x04[
    ((u32)&((SndPadStepper *)0)->target == 0x04) ? 1 : -1];
typedef char SndPadStepper_index_offset_must_be_0x28[
    ((u32)&((SndPadStepper *)0)->index == 0x28) ? 1 : -1];

#endif
