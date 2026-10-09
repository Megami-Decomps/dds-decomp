#ifndef KWLN_TASK_CREATE_H
#define KWLN_TASK_CREATE_H

#include "kwln.h"

KwlnTask *kwlnTaskCreate(const char *name, u32 priority, s32 startDelay,
                         s32 destroyDelay, TaskUpdate update,
                         TaskDestroy destroy, u32 userValue);

#endif /* KWLN_TASK_CREATE_H */
