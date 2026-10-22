#ifndef FLD_SPARK_CONTROLLER_STATE_H
#define FLD_SPARK_CONTROLLER_STATE_H

#include "common.h"

/* Shared field-spark phase, timer, and serialized-sequence progress state. */
typedef struct FldSparkControllerState {
    s32 phase;
    s32 countdown;
    s32 terminated;
    s32 mode;
    s32 modeCountdown;
    s32 pendingMode;
    s32 frame;
    s32 pulse;
    s32 dialogPhase;
    s32 dialogTimer;
    s32 entryCount;
    s32 sequenceIndex;
    s32 sequenceRemaining;
} FldSparkControllerState;

typedef char FldSparkControllerStateSizeCheck[(sizeof(FldSparkControllerState) == 0x34) ? 1 : -1];
typedef char FldSparkControllerStateSequenceIndexOffsetCheck[
    ((u32)&((FldSparkControllerState *)0)->sequenceIndex == 0x2C) ? 1 : -1];
typedef char FldSparkControllerStateSequenceRemainingOffsetCheck[
    ((u32)&((FldSparkControllerState *)0)->sequenceRemaining == 0x30) ? 1 : -1];

#endif
