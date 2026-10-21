#ifndef SDF_COUNTER_TIMER_H
#define SDF_COUNTER_TIMER_H

#include "common.h"

/* Allocated and cleared as 0x24 bytes by both counter-channel providers. */
typedef struct SdfCounterTimer {
    s32 value;                     /* 0x00 */
    s16 selectionPulseFramesRemaining; /* 0x04 */
    s16 mode;                      /* 0x06 */
    u8 opaque08[4];                /* 0x08 */
    s16 mapTimerPrimary;           /* 0x0C */
    s16 mapTimerSecondary;         /* 0x0E */
    s32 y;                         /* 0x10 */
    s16 startX;                    /* 0x14 */
    s16 startY;                    /* 0x16 */
    s16 targetX;                   /* 0x18 */
    s16 targetY;                   /* 0x1A */
    s16 curX;                      /* 0x1C */
    s16 curY;                      /* 0x1E */
    s32 frames;                    /* 0x20 */
} SdfCounterTimer;

typedef char SdfCounterTimer_size_must_be_0x24[
    (sizeof(SdfCounterTimer) == 0x24) ? 1 : -1
];
typedef char SdfCounterTimer_value_at_0x00[
    ((u32)&((SdfCounterTimer *)0)->value == 0x00) ? 1 : -1
];
typedef char SdfCounterTimer_selectionPulseFramesRemaining_at_0x04[
    ((u32)&((SdfCounterTimer *)0)->selectionPulseFramesRemaining == 0x04) ? 1 : -1
];
typedef char SdfCounterTimer_mode_at_0x06[
    ((u32)&((SdfCounterTimer *)0)->mode == 0x06) ? 1 : -1
];
typedef char SdfCounterTimer_opaque08_at_0x08[
    ((u32)&((SdfCounterTimer *)0)->opaque08 == 0x08) ? 1 : -1
];
typedef char SdfCounterTimer_mapTimerPrimary_at_0x0C[
    ((u32)&((SdfCounterTimer *)0)->mapTimerPrimary == 0x0C) ? 1 : -1
];
typedef char SdfCounterTimer_mapTimerSecondary_at_0x0E[
    ((u32)&((SdfCounterTimer *)0)->mapTimerSecondary == 0x0E) ? 1 : -1
];
typedef char SdfCounterTimer_y_at_0x10[
    ((u32)&((SdfCounterTimer *)0)->y == 0x10) ? 1 : -1
];
typedef char SdfCounterTimer_startX_at_0x14[
    ((u32)&((SdfCounterTimer *)0)->startX == 0x14) ? 1 : -1
];
typedef char SdfCounterTimer_startY_at_0x16[
    ((u32)&((SdfCounterTimer *)0)->startY == 0x16) ? 1 : -1
];
typedef char SdfCounterTimer_targetX_at_0x18[
    ((u32)&((SdfCounterTimer *)0)->targetX == 0x18) ? 1 : -1
];
typedef char SdfCounterTimer_targetY_at_0x1A[
    ((u32)&((SdfCounterTimer *)0)->targetY == 0x1A) ? 1 : -1
];
typedef char SdfCounterTimer_curX_at_0x1C[
    ((u32)&((SdfCounterTimer *)0)->curX == 0x1C) ? 1 : -1
];
typedef char SdfCounterTimer_curY_at_0x1E[
    ((u32)&((SdfCounterTimer *)0)->curY == 0x1E) ? 1 : -1
];
typedef char SdfCounterTimer_frames_at_0x20[
    ((u32)&((SdfCounterTimer *)0)->frames == 0x20) ? 1 : -1
];

#endif /* SDF_COUNTER_TIMER_H */
