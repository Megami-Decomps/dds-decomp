#ifndef FLD_SCENE_READY_API_H
#define FLD_SCENE_READY_API_H

#include "common.h"

u32 fldGetSceneReadyFlag(void);
u32 fldLoadNextGetStatus(void);
u8 fldLoadNextGetStatus2(void);
u32 fldPollAreaResourceLoad(void);
s32 fldLoadNextFile_BtlRet(void);

#endif
