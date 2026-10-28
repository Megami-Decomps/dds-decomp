#ifndef FLD_TERMINAL_SCENE_STATE_H
#define FLD_TERMINAL_SCENE_STATE_H

#include "common.h"

typedef s8 FldTerminalTaskState;
typedef s32 FldTerminalScenePollResult;

enum FldTerminalTaskStateValue {
    FLD_TERMINAL_TASK_STATE_IDLE = 0,
    FLD_TERMINAL_TASK_STATE_ACTIVE = 1,
    FLD_TERMINAL_TASK_STATE_RESOURCES_RELEASED = 2,
};

enum FldTerminalScenePollResultValue {
    FLD_TERMINAL_SCENE_POLL_NOT_PENDING = 0,
    FLD_TERMINAL_SCENE_POLL_PENDING = 1,
};

#endif /* FLD_TERMINAL_SCENE_STATE_H */
