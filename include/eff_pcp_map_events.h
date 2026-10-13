#ifndef EFF_PCP_MAP_EVENTS_H
#define EFF_PCP_MAP_EVENTS_H

#include "common.h"

struct EffEventWork;
struct EffParamWork;
struct SdfMemBlock;
struct SoundMixer;

/* One event per model-map position. Init seeds the entry, then the updater
 * latches its initial position and advances the event through its frame span. */
typedef struct EffPCPMapEventEntry {
    f32 initialPosition[4];
    u8 hasMoved;
    u8 pad11[3];
    s32 age;
    s32 frameLimit;
    struct EffEventWork *event;
} EffPCPMapEventEntry;

/* Serialized model placement and fade parameters copied into the owner. */
typedef struct EffPCPEventParamHead {
    f32 position[4];
    s32 fadeIn;
    s32 fadeOut;
    f32 frameStep;
} EffPCPEventParamHead;

/* The factory allocates this owner as one 0x40-byte size-class block; the
 * per-position entries are a separate count * 0x20 allocation. */
typedef struct EffPCPMapEventWork {
    EffPCPEventParamHead params;
    EffPCPMapEventEntry *entries;
    struct SdfMemBlock *entriesHandle;
    struct SoundMixer *owner;
    struct EffParamWork *modelResource;
    f32 scale;
    s32 frame;
    s32 frameLimit;
    u32 count;
    u32 color;
} EffPCPMapEventWork;

typedef char EffPCPMapEventEntrySizeCheck[sizeof(EffPCPMapEventEntry) == 0x20 ? 1 : -1];
typedef char EffPCPMapEventEntryHasMovedOffsetCheck[((u32)&((EffPCPMapEventEntry *)0)->hasMoved == 0x10) ? 1 : -1];
typedef char EffPCPMapEventEntryAgeOffsetCheck[((u32)&((EffPCPMapEventEntry *)0)->age == 0x14) ? 1 : -1];
typedef char EffPCPMapEventEntryFrameLimitOffsetCheck[((u32)&((EffPCPMapEventEntry *)0)->frameLimit == 0x18) ? 1 : -1];
typedef char EffPCPMapEventEntryEventOffsetCheck[((u32)&((EffPCPMapEventEntry *)0)->event == 0x1C) ? 1 : -1];
typedef char EffPCPEventParamHeadSizeCheck[sizeof(EffPCPEventParamHead) == 0x1C ? 1 : -1];
typedef char EffPCPMapEventWorkSizeCheck[sizeof(EffPCPMapEventWork) == 0x40 ? 1 : -1];
typedef char EffPCPMapEventWorkEntriesOffsetCheck[((u32)&((EffPCPMapEventWork *)0)->entries == 0x1C) ? 1 : -1];
typedef char EffPCPMapEventWorkEntriesHandleOffsetCheck[((u32)&((EffPCPMapEventWork *)0)->entriesHandle == 0x20) ? 1 : -1];
typedef char EffPCPMapEventWorkOwnerOffsetCheck[((u32)&((EffPCPMapEventWork *)0)->owner == 0x24) ? 1 : -1];
typedef char EffPCPMapEventWorkModelResourceOffsetCheck[((u32)&((EffPCPMapEventWork *)0)->modelResource == 0x28) ? 1 : -1];
typedef char EffPCPMapEventWorkFrameOffsetCheck[((u32)&((EffPCPMapEventWork *)0)->frame == 0x30) ? 1 : -1];
typedef char EffPCPMapEventWorkCountOffsetCheck[((u32)&((EffPCPMapEventWork *)0)->count == 0x38) ? 1 : -1];
typedef char EffPCPMapEventWorkColorOffsetCheck[((u32)&((EffPCPMapEventWork *)0)->color == 0x3C) ? 1 : -1];

#endif
