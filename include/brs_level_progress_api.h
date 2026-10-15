#ifndef BRS_LEVEL_PROGRESS_API_H
#define BRS_LEVEL_PROGRESS_API_H

#include "common.h"

u8 brsGetLevelStepForValue(s32 value);
u8 brsGetLevelStepCrossedBy(s32 position, s32 increment);

#endif /* BRS_LEVEL_PROGRESS_API_H */
