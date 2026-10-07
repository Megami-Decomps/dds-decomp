#ifndef EFF_CHANNEL_H
#define EFF_CHANNEL_H

#include "common.h"
#include "sdf.h"

/* Shared channel parameter block copied into each allocated channel work. */
typedef struct EffChanHead {
    f32 controlPoints[4][4];
    u8 enabled;
    u8 pad41[3];
    u32 count;
    s32 steps;
    s32 spread;
    s32 fadeIn;
    s32 fadeOut;
    f32 jitter[4];
    u8 pad68[0x100];
} EffChanHead;

/* One randomized channel record. */
typedef struct EffChanRecord {
    s32 delay;
    void *param;
} EffChanRecord;

/* Allocated channel owner shared by the event factory and channel consumers. */
typedef struct EffChanWork {
    EffChanHead head;
    EffChanRecord *records;
    s32 *slots;
    SdfMemBlock *buffer;
} EffChanWork;

typedef char EffChanHead_size_must_be_0x168[(sizeof(EffChanHead) == 0x168) ? 1 : -1];
typedef char EffChanRecord_size_must_be_0x8[(sizeof(EffChanRecord) == 0x8) ? 1 : -1];
typedef char EffChanWork_size_must_be_0x174[(sizeof(EffChanWork) == 0x174) ? 1 : -1];
typedef char EffChanWork_records_offset_must_be_0x168[((u32)&((EffChanWork *)0)->records == 0x168) ? 1 : -1];
typedef char EffChanWork_slots_offset_must_be_0x16C[((u32)&((EffChanWork *)0)->slots == 0x16C) ? 1 : -1];
typedef char EffChanWork_buffer_offset_must_be_0x170[((u32)&((EffChanWork *)0)->buffer == 0x170) ? 1 : -1];

#endif
