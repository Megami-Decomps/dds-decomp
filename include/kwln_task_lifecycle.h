#ifndef KWLN_TASK_LIFECYCLE_H
#define KWLN_TASK_LIFECYCLE_H

#include "kwln.h"

s32 kwlnTaskDestroyWithHierarchy(KwlnTask *task, s32 delayTicks);
s32 kwlnTaskDestroyWithHierarchyByName(const char *name, s32 delayTicks);

#endif /* KWLN_TASK_LIFECYCLE_H */
