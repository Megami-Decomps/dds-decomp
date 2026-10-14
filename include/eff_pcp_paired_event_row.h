#ifndef EFF_PCP_PAIRED_EVENT_ROW_H
#define EFF_PCP_PAIRED_EVENT_ROW_H

#include "common.h"

struct EffThunderFragmentWork;
struct EffEventWork;

/* The paired PCP event owners allocate count * 0x20 bytes after the 0xA4
 * byte work header and initialize/release two event nodes per row. */
typedef struct EffPCPPairedEventRow {
    struct EffThunderFragmentWork *fragment;
    struct EffEventWork *eventA;
    struct EffEventWork *eventB;
    f32 phase;
    f32 radius;
    f32 tilt;
    f32 tiltStep;
    s32 frame;
} EffPCPPairedEventRow;

typedef char EffPCPPairedEventRowSizeCheck[
    (sizeof(EffPCPPairedEventRow) == 0x20) ? 1 : -1];
typedef char EffPCPPairedEventRowEventAOffsetCheck[
    ((u32)&((EffPCPPairedEventRow *)0)->eventA == 0x04) ? 1 : -1];
typedef char EffPCPPairedEventRowRadiusOffsetCheck[
    ((u32)&((EffPCPPairedEventRow *)0)->radius == 0x10) ? 1 : -1];
typedef char EffPCPPairedEventRowFrameOffsetCheck[
    ((u32)&((EffPCPPairedEventRow *)0)->frame == 0x1C) ? 1 : -1];

#endif
