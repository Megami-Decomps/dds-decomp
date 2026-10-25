#ifndef FLD_NEXT_SCENE_REQUEST_API_H
#define FLD_NEXT_SCENE_REQUEST_API_H

#include "common.h"

typedef s32 FldNextSceneRequestResult;
enum {
    FLD_NEXT_SCENE_REQUEST_NO_REQUEST = 0,
    FLD_NEXT_SCENE_REQUEST_CONSUMED = 1
};

/* The output values are written only when a request is consumed. */
FldNextSceneRequestResult fldConsumeNextSceneRequest(s32 *outCode, s32 *outParameter);

#endif /* FLD_NEXT_SCENE_REQUEST_API_H */
