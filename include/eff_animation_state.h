#ifndef EFF_ANIMATION_STATE_H
#define EFF_ANIMATION_STATE_H

#include "common.h"
#include "file_slot.h"

/* Per-animation allocation: positions follow this four-word owner header. */
typedef struct EffAnimationState {
    f32 *positions;        // 0x00
    u32 textureHandle;     // 0x04
    FileSlotTable *record; // 0x08
    struct SdfMemBlock *allocation; // 0x0C
} EffAnimationState;

#endif
