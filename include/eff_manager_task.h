#ifndef EFF_MANAGER_TASK_H
#define EFF_MANAGER_TASK_H

#include "kwln.h"

/* The field effect scheduler registers these in its update/destroy slots.
 * Both update callbacks return zero after their work completes. */
s32 effSubmitFieldBlendStateTask(dds3ProcessID_t *task);
s32 dds3EffectManagerRun(dds3ProcessID_t *task);
void effManagerInitializeSubsystems(dds3ProcessID_t *task);

#endif /* EFF_MANAGER_TASK_H */
