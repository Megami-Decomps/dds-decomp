#ifndef FLD_SCENE_COMMAND_STATE_API_H
#define FLD_SCENE_COMMAND_STATE_API_H

#include "common.h"

typedef s32 FldSceneCommandState;
enum {
    FLD_SCENE_COMMAND_STATE_UNAVAILABLE = -1,
    FLD_SCENE_COMMAND_STATE_AVAILABLE = 0,
    FLD_SCENE_COMMAND_STATE_PENDING = 1
};

FldSceneCommandState fldGetSceneCommandState(void);

#endif /* FLD_SCENE_COMMAND_STATE_API_H */
