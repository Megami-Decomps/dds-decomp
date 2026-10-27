#ifndef FLD_NEXT_SCENE_REQUEST_API_H
#define FLD_NEXT_SCENE_REQUEST_API_H

#include "common.h"

typedef s32 FldNextSceneRequestResult;
enum {
    FLD_NEXT_SCENE_REQUEST_NO_REQUEST = 0,
    FLD_NEXT_SCENE_REQUEST_CONSUMED = 1
};

/* The map code selects a battle lookup row; the entry index selects its entry. */
typedef s32 FldNextSceneRequestMapCode;
typedef s32 FldNextSceneRequestEntryIndex;

/* Outputs are written only when a request is consumed. */
FldNextSceneRequestResult fldConsumeNextSceneRequest(
    FldNextSceneRequestMapCode *outMapCode,
    FldNextSceneRequestEntryIndex *outEntryIndex);

#endif /* FLD_NEXT_SCENE_REQUEST_API_H */
