#ifndef BTL_SCENE_FADE_H
#define BTL_SCENE_FADE_H

#include "common.h"

/* The scene-slot fade root is allocated and cleared as 0xD8 bytes.
 * Known slot-count producers yield 0..8. Updates visit rows 0..count and
 * activate the next phase, proving these minimum array extents. Bytes
 * outside those observed ranges remain opaque; no larger capacity is inferred. */
typedef struct SceneSlotFadeWork {
    s16 timer;
    s8 enabled[2];
    s32 bank;
    u8 unk08[4];
    u8 completed;
    u8 unk0D[0x13];
    s8 currentIndex;
    s8 lastIndex;
    s8 phase[10][2];
    u8 unk36[2];
    s32 fade[9][2];
    u8 unk80[0x58];
} SceneSlotFadeWork;

typedef char SceneSlotFadeWork_size[(sizeof(SceneSlotFadeWork) == 0xD8) ? 1 : -1];
typedef char SceneSlotFadeWork_phase_offset[((unsigned long)&((SceneSlotFadeWork *)0)->phase == 0x22) ? 1 : -1];
typedef char SceneSlotFadeWork_fade_offset[((unsigned long)&((SceneSlotFadeWork *)0)->fade == 0x38) ? 1 : -1];

#endif
