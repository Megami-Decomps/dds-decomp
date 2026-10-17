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

/* Shared viewer work through the pending effect. DDS2 also tracks whether the
 * first model update has run before it starts queued effect/motion work. */
typedef struct StageTestState {
    s32 mode;
    s32 assetRequest;
    struct MdlCtx *model;
    s8 flag;
    u8 pad0D[3];
    StageTestEntry *entries;
    StageTestQueue queue;
    s32 effect;
    s32 pendingEffect;
#if defined(VERSION_DDS2)
    s32 modelUpdateStarted;
#endif
} StageTestState;

typedef char StageTestEntrySizeCheck[sizeof(StageTestEntry) == 0x3C ? 1 : -1];
typedef char StageTestSlotSizeCheck[sizeof(StageTestSlot) == 0x28 ? 1 : -1];
typedef char StageTestQueueSizeCheck[sizeof(StageTestQueue) == 0x54 ? 1 : -1];
typedef char StageTestEntryFrameOffsetCheck[((u32)&((StageTestEntry *)0)->frame) == 0x18 ? 1 : -1];
typedef char StageTestEntryPositionOffsetCheck[((u32)&((StageTestEntry *)0)->position) == 0x1C ? 1 : -1];
typedef char StageTestEntryRotationOffsetCheck[((u32)&((StageTestEntry *)0)->rotation) == 0x2C ? 1 : -1];
typedef char StageTestSlotStateOffsetCheck[((u32)&((StageTestSlot *)0)->state) == 0x18 ? 1 : -1];
typedef char StageTestQueueSlotsOffsetCheck[((u32)&((StageTestQueue *)0)->slot) == 4 ? 1 : -1];
typedef char StageTestStateModeOffsetCheck[((u32)&((StageTestState *)0)->mode) == 0x00 ? 1 : -1];
typedef char StageTestStateAssetRequestOffsetCheck[((u32)&((StageTestState *)0)->assetRequest) == 0x04 ? 1 : -1];
typedef char StageTestStateModelOffsetCheck[((u32)&((StageTestState *)0)->model) == 0x08 ? 1 : -1];
typedef char StageTestStateFlagOffsetCheck[((u32)&((StageTestState *)0)->flag) == 0x0C ? 1 : -1];
typedef char StageTestStatePaddingOffsetCheck[((u32)&((StageTestState *)0)->pad0D) == 0x0D ? 1 : -1];
typedef char StageTestStateEntriesOffsetCheck[((u32)&((StageTestState *)0)->entries) == 0x10 ? 1 : -1];
typedef char StageTestStateQueueOffsetCheck[((u32)&((StageTestState *)0)->queue) == 0x14 ? 1 : -1];
typedef char StageTestStateEffectOffsetCheck[((u32)&((StageTestState *)0)->effect) == 0x68 ? 1 : -1];
typedef char StageTestStatePendingEffectOffsetCheck[((u32)&((StageTestState *)0)->pendingEffect) == 0x6C ? 1 : -1];
#if defined(VERSION_DDS1)
typedef char StageTestStateSizeCheck[sizeof(StageTestState) == 0x70 ? 1 : -1];
#elif defined(VERSION_DDS2)
typedef char StageTestStateModelUpdateStartedOffsetCheck[
    ((u32)&((StageTestState *)0)->modelUpdateStarted) == 0x70 ? 1 : -1];
typedef char StageTestStateSizeCheck[sizeof(StageTestState) == 0x74 ? 1 : -1];
#endif

#endif
