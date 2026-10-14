#ifndef EFF_PCP_SPAWN_RANGE_EVENT_ROW_H
#define EFF_PCP_SPAWN_RANGE_EVENT_ROW_H

#include "common.h"

struct EffEventWork;

/* Spawn-range owners allocate count * 0x20 bytes after their 0xAC-byte header. */
typedef struct EffPCPSpawnRangeEventRow {
    struct EffEventWork *event;
    s32 frame;
    f32 height;
    f32 heightStep;
    f32 angle;
    f32 angularStep;
    f32 position;
    f32 positionStep;
} EffPCPSpawnRangeEventRow;

typedef char EffPCPSpawnRangeEventRowSizeCheck[
    (sizeof(EffPCPSpawnRangeEventRow) == 0x20) ? 1 : -1];
typedef char EffPCPSpawnRangeEventRowFrameOffsetCheck[
    ((u32)&((EffPCPSpawnRangeEventRow *)0)->frame == 0x04) ? 1 : -1];
typedef char EffPCPSpawnRangeEventRowHeightOffsetCheck[
    ((u32)&((EffPCPSpawnRangeEventRow *)0)->height == 0x08) ? 1 : -1];
typedef char EffPCPSpawnRangeEventRowPositionStepOffsetCheck[
    ((u32)&((EffPCPSpawnRangeEventRow *)0)->positionStep == 0x1C) ? 1 : -1];

#endif
