#ifndef FLD_SCENE_READY_API_H
#define FLD_SCENE_READY_API_H

#include "common.h"

typedef u32 FldSceneReadyState;
enum {
    FLD_SCENE_NOT_READY = 0,
    FLD_SCENE_READY = 1
};

FldSceneReadyState fldGetSceneReadyFlag(void);
u32 fldLoadNextGetStatus(void);
u8 fldLoadNextGetStatus2(void);
u32 fldPollAreaResourceLoad(void);
u8 fldTryStartPendingBattleReturnAreaLoad(void);

#endif
