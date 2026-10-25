#ifndef FLD_SCENE_STATUS_API_H
#define FLD_SCENE_STATUS_API_H

#include "common.h"

typedef enum FldSceneStatus {
    FLD_SCENE_STATUS_DEFAULT = 0,
    FLD_SCENE_STATUS_BATTLE = 1,
    FLD_SCENE_STATUS_EVENT = 2,
    FLD_SCENE_STATUS_ANA = 3,
    FLD_SCENE_STATUS_SISETU = 4,
    FLD_SCENE_STATUS_MINI_TITLE_UNLOCK = 5
} FldSceneStatus;

FldSceneStatus fldGetRestoreMode(void);

#endif
