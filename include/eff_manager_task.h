#ifndef EFF_MANAGER_TASK_H
#define EFF_MANAGER_TASK_H

#include "kwln.h"

/* The field effect scheduler registers these in its update/destroy slots.
 * Both update callbacks return zero after their work completes. */
s32 effSubmitFieldBlendStateTask(KwlnTask *task);
s32 effManagerUpdateAndDispatch(KwlnTask *task);
void effManagerInitializeSubsystems(KwlnTask *task);

#endif /* EFF_MANAGER_TASK_H */
