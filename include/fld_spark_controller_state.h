#ifndef FLD_SPARK_CONTROLLER_STATE_H
#define FLD_SPARK_CONTROLLER_STATE_H

#include "common.h"

typedef s32 FldSparkAnimationState;
enum {
    FLD_SPARK_ANIMATION_READY = 0,
    FLD_SPARK_ANIMATION_RUNNING = 1
};

/* Values for the spark-event prompt, playback, and field-return state machine. */
#define FLD_SPARK_DIALOG_PHASE_IDLE 0
#define FLD_SPARK_DIALOG_PHASE_ENTRY_PROMPT 1
#define FLD_SPARK_DIALOG_PHASE_ARCHIVE_LOADING 2
#define FLD_SPARK_DIALOG_PHASE_TITLE_STREAM_WAIT 3
#define FLD_SPARK_DIALOG_PHASE_CONFIRM_PROMPT 4
#define FLD_SPARK_DIALOG_PHASE_START_ARCHIVE_LOAD 5
#define FLD_SPARK_DIALOG_PHASE_WAIT_ARCHIVE_FILE 6
#define FLD_SPARK_DIALOG_PHASE_ENTER_ENCOUNTER 7
#define FLD_SPARK_DIALOG_PHASE_PLAY_EVENT 8
#define FLD_SPARK_DIALOG_PHASE_RESTORE_FIELD 9
#define FLD_SPARK_DIALOG_PHASE_WAIT_FIELD_FADE 10
#define FLD_SPARK_DIALOG_PHASE_CANCELLED 11
#define FLD_SPARK_DIALOG_PHASE_UNAVAILABLE_MESSAGE 12

/* Shared field-spark phase, timer, and serialized-sequence progress state. */
typedef struct FldSparkControllerState {
    s32 phase;
    s32 countdown;
    s32 terminated;
    FldSparkAnimationState animationState;
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
typedef char FldSparkControllerStateAnimationStateOffsetCheck[
    ((u32)&((FldSparkControllerState *)0)->animationState == 0x0C) ? 1 : -1];
typedef char FldSparkControllerStateSequenceIndexOffsetCheck[
    ((u32)&((FldSparkControllerState *)0)->sequenceIndex == 0x2C) ? 1 : -1];
typedef char FldSparkControllerStateSequenceRemainingOffsetCheck[
    ((u32)&((FldSparkControllerState *)0)->sequenceRemaining == 0x30) ? 1 : -1];

#endif
