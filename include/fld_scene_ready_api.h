#ifndef FLD_SCENE_READY_API_H
#define FLD_SCENE_READY_API_H

#include "common.h"

typedef u32 FldSceneReadyState;
enum {
    FLD_SCENE_NOT_READY = 0,
    FLD_SCENE_READY = 1
};

typedef u32 FldNextFileLoadState;
enum {
    FLD_NEXT_FILE_LOAD_FINISHED = 0,
    FLD_NEXT_FILE_LOAD_PENDING = 1
};

typedef u8 FldAreaResourceReadyState;
enum {
    FLD_AREA_RESOURCE_NOT_READY = 0,
    FLD_AREA_RESOURCE_READY = 1
};

FldSceneReadyState fldGetSceneReadyFlag(void);
FldNextFileLoadState fldGetNextFileLoadState(void);
/* READY when the active area request completes or the shared ready flag is
 * set. */
FldAreaResourceReadyState fldIsAreaResourceReady(void);
u32 fldPollAreaResourceLoad(void);

typedef u8 FldBattleReturnAreaLoadResult;
enum {
    FLD_BATTLE_RETURN_AREA_LOAD_NOT_REQUESTED = 0,
    FLD_BATTLE_RETURN_AREA_LOAD_STARTED = 1
};

FldBattleReturnAreaLoadResult fldTryStartPendingBattleReturnAreaLoad(void);

#endif
