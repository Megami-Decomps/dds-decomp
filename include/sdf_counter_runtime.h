#ifndef SDF_COUNTER_RUNTIME_H
#define SDF_COUNTER_RUNTIME_H

#include "common.h"
#include "sdf_counter_channel.h"

struct SdfCounterRuntime;
typedef void (*SdfCounterDrawFn)(s32, s32, s32, struct SdfCounterRuntime *, SdfCounterChannel *, s32);
struct SdfCounterTimer;
typedef struct SdfCounterTimer SdfCounterTimer;

/* Shared selection state at +0x0C in the DDS1/DDS2 counter owner. */
typedef struct SdfCounterRuntimeState {
    s32 base;                       /* 0x00 */
    SdfCounterChannel *first;       /* 0x04 */
    SdfCounterChannel *last;        /* 0x08 */
    SdfCounterChannel *selected;    /* 0x0C */
    SdfCounterChannel *channel;     /* 0x10 */
    s32 active;                     /* 0x14 */
    s32 scroll;                     /* 0x18 */
    s32 posX;                       /* 0x1C */
    SdfCounterDrawFn draw;          /* 0x20 */
    SdfCounterTimer *timer;         /* 0x24 */
} SdfCounterRuntimeState;

typedef char SdfCounterRuntimeState_size_must_be_0x28[
    (sizeof(SdfCounterRuntimeState) == 0x28) ? 1 : -1
];
typedef char SdfCounterRuntimeState_base_at_0x00[
    ((u32)&((SdfCounterRuntimeState *)0)->base == 0x00) ? 1 : -1
];
typedef char SdfCounterRuntimeState_first_at_0x04[
    ((u32)&((SdfCounterRuntimeState *)0)->first == 0x04) ? 1 : -1
];
typedef char SdfCounterRuntimeState_last_at_0x08[
    ((u32)&((SdfCounterRuntimeState *)0)->last == 0x08) ? 1 : -1
];
typedef char SdfCounterRuntimeState_selected_at_0x0C[
    ((u32)&((SdfCounterRuntimeState *)0)->selected == 0x0C) ? 1 : -1
];
typedef char SdfCounterRuntimeState_channel_at_0x10[
    ((u32)&((SdfCounterRuntimeState *)0)->channel == 0x10) ? 1 : -1
];
typedef char SdfCounterRuntimeState_active_at_0x14[
    ((u32)&((SdfCounterRuntimeState *)0)->active == 0x14) ? 1 : -1
];
typedef char SdfCounterRuntimeState_scroll_at_0x18[
    ((u32)&((SdfCounterRuntimeState *)0)->scroll == 0x18) ? 1 : -1
];
typedef char SdfCounterRuntimeState_posX_at_0x1C[
    ((u32)&((SdfCounterRuntimeState *)0)->posX == 0x1C) ? 1 : -1
];
typedef char SdfCounterRuntimeState_draw_at_0x20[
    ((u32)&((SdfCounterRuntimeState *)0)->draw == 0x20) ? 1 : -1
];
typedef char SdfCounterRuntimeState_timer_at_0x24[
    ((u32)&((SdfCounterRuntimeState *)0)->timer == 0x24) ? 1 : -1
];

#endif /* SDF_COUNTER_RUNTIME_H */
