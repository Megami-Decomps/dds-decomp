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
    u8 unk0D[0xB];
    s32 unk18; /* Cleared independently from completed by the scene-slot reset. */
    u8 unk1C[4];
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

/* Both bank buffers are allocated and cleared as 0x274 bytes. Existing
 * order-copy providers write at most eight entries; the independent
 * word range at +0x2C is not part of that destination. Phase-transition
 * consumers index eight state bytes and the final integer slot-value
 * and float percentage pairs. */
typedef struct ActorSlotOrder {
#ifdef VERSION_DDS1
    u8 unk00[2];
    s8 state[8];
    u8 unk0A[2];
#else
    u8 unk00[4];
    s8 state[8];
#endif
    s32 entries[8];
    u8 unk2C[0x40];
    f32 unk6C[8]; /* Scene-slot reset initializes these per-slot values to 30.0f. */
    s32 unk8C[8]; /* Corresponding per-slot values initially set to 130. */
    s8 secondaryState[8]; /* Independent color cycle, states 0 through 8. */
    s32 colorAdjustments[8][4]; /* Signed values clamped to 0..127. */
    u8 unk134[0xC0];
    s32 slotValues[8][2];
    f32 scalePercent[8][2];
} ActorSlotOrder;
typedef char ActorSlotOrder_size[(sizeof(ActorSlotOrder) == 0x274) ? 1 : -1];
typedef char ActorSlotOrder_slotValues_offset[((unsigned long)&((ActorSlotOrder *)0)->slotValues == 0x1F4) ? 1 : -1];
typedef char ActorSlotOrder_scalePercent_offset[((unsigned long)&((ActorSlotOrder *)0)->scalePercent == 0x234) ? 1 : -1];

typedef char ActorSlotOrder_secondaryState_offset[((unsigned long)&((ActorSlotOrder *)0)->secondaryState == 0xAC) ? 1 : -1];
typedef char ActorSlotOrder_colorAdjustments_offset[((unsigned long)&((ActorSlotOrder *)0)->colorAdjustments == 0xB4) ? 1 : -1];

#endif
