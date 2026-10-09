#ifndef BTL_STAGE_TASK_CLEANUP_H
#define BTL_STAGE_TASK_CLEANUP_H

#include "common.h"

/* These title-specific wrappers forward an opaque stage-task owner and its
 * handled/not-handled result. */
#ifdef VERSION_DDS1
s32 func_00288788(void *request);
#else
s32 func_002C7CE8(void *request);
#endif

#endif /* BTL_STAGE_TASK_CLEANUP_H */
