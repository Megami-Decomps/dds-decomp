#ifndef EVT_STAGE_TEST_H
#define EVT_STAGE_TEST_H

#include "common.h"

/* Fixed-size source entry consumed by both stage-test viewers. */
typedef struct StageTestEntry {
    u8 modelId;      /* 0x00 */
    u8 motionIndex;  /* 0x01 */
    u8 column[2][8]; /* 0x02: two per-column motion-index blocks */
    u8 pad12[6];
    f32 frame;       /* 0x18 */
    f32 position[3]; /* 0x1C */
    f32 pad28;
    f32 rotation[3]; /* 0x2C */
    f32 pad38;
} StageTestEntry;

typedef struct StageTestSlot {
    s32 entryIndex;          /* 0x00 */
    s32 assetResource;       /* 0x04: mode-selected model resource */
    s32 modelId;             /* 0x08 */
    s32 assetOption;         /* 0x0C: third argument to mdlRequestAsset */
    s32 initialMotionIndex;  /* 0x10: negative selects entry motionIndex */
    u32 flags;               /* 0x14: bit 0 suppresses fallback motion */
    s32 state;               /* 0x18: idle/queued/playing/fallback state */
    s32 motionIndex;         /* 0x1C */
    s32 blendLeadFrames;     /* 0x20 */
    s32 blendDurationFrames; /* 0x24 */
} StageTestSlot;

typedef struct StageTestQueue {
    u32 flags; /* pending, request-in-progress, and setup-complete bits */
    StageTestSlot slot[2]; /* 0x04 and 0x2C */
} StageTestQueue;

typedef char StageTestEntrySizeCheck[sizeof(StageTestEntry) == 0x3C ? 1 : -1];
typedef char StageTestSlotSizeCheck[sizeof(StageTestSlot) == 0x28 ? 1 : -1];
typedef char StageTestQueueSizeCheck[sizeof(StageTestQueue) == 0x54 ? 1 : -1];
typedef char StageTestEntryFrameOffsetCheck[((u32)&((StageTestEntry *)0)->frame) == 0x18 ? 1 : -1];
typedef char StageTestEntryPositionOffsetCheck[((u32)&((StageTestEntry *)0)->position) == 0x1C ? 1 : -1];
typedef char StageTestEntryRotationOffsetCheck[((u32)&((StageTestEntry *)0)->rotation) == 0x2C ? 1 : -1];
typedef char StageTestSlotStateOffsetCheck[((u32)&((StageTestSlot *)0)->state) == 0x18 ? 1 : -1];
typedef char StageTestQueueSlotsOffsetCheck[((u32)&((StageTestQueue *)0)->slot) == 4 ? 1 : -1];

#endif
