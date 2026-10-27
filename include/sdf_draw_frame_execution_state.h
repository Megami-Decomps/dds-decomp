#ifndef SDF_DRAW_FRAME_EXECUTION_STATE_H
#define SDF_DRAW_FRAME_EXECUTION_STATE_H

#include "common.h"

/* Cross-thread state bracketing sdfGrDrawFrame. sdfDrawSync waits for IDLE
 * before inspecting the active packet slot; keep the shared byte volatile. */
typedef u8 SdfDrawFrameExecutionState;

enum {
    SDF_DRAW_FRAME_IDLE = 0,
    SDF_DRAW_FRAME_IN_PROGRESS = 1,
};

typedef char SdfDrawFrameExecutionState_size_must_be_1[
    (sizeof(SdfDrawFrameExecutionState) == 1) ? 1 : -1];

#endif /* SDF_DRAW_FRAME_EXECUTION_STATE_H */
