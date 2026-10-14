#ifndef FLD_SCENE_READY_API_H
#define FLD_SCENE_READY_API_H

#include "common.h"

u32 fldGetSceneReadyFlag(void);
u32 fldGetResourceReadyFlag(void);
u32 fldPollAreaResourceLoad(void);

#endif
