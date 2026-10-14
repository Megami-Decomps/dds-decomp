#ifndef EFF_PCP_DRIFT_EVENT_ROW_H
#define EFF_PCP_DRIFT_EVENT_ROW_H

#include "common.h"

struct EffEventWork;

/* Both PCP drift owners allocate count * 0x18 bytes after their 0x11C-byte
 * work header, initialize these rows, and release each retained event. */
typedef struct EffPCPDriftEventRow {
    struct EffEventWork *event;
    s32 frame;
    f32 position;
    f32 positionStep;
    f32 angle;
    f32 angleStep;
} EffPCPDriftEventRow;

typedef char EffPCPDriftEventRowSizeCheck[
    (sizeof(EffPCPDriftEventRow) == 0x18) ? 1 : -1];
typedef char EffPCPDriftEventRowFrameOffsetCheck[
    ((u32)&((EffPCPDriftEventRow *)0)->frame == 0x04) ? 1 : -1];
typedef char EffPCPDriftEventRowPositionOffsetCheck[
    ((u32)&((EffPCPDriftEventRow *)0)->position == 0x08) ? 1 : -1];
typedef char EffPCPDriftEventRowAngleStepOffsetCheck[
    ((u32)&((EffPCPDriftEventRow *)0)->angleStep == 0x14) ? 1 : -1];

#endif
