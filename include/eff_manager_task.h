#ifndef EFF_MANAGER_TASK_H
#define EFF_MANAGER_TASK_H

#include "kwln.h"

/* These updates run each scheduler pass; RETAIN keeps each callback installed. */
dds3TaskUpdateResult_t effSubmitFieldBlendStateTask(dds3ProcessID_t *task);
dds3TaskUpdateResult_t dds3EffectManagerRun(dds3ProcessID_t *task);
void effManagerInitializeSubsystems(dds3ProcessID_t *task);

#endif /* EFF_MANAGER_TASK_H */
