#ifndef KWLN_TASK_FLAGS_H
#define KWLN_TASK_FLAGS_H

#include "kwln.h"

/* Flag updates preserve the scheduler state and the upper four bits. */
#define KWLN_TASK_MUTABLE_FLAGS_MASK 0x0FFFFFF0

typedef enum KwlnTaskFlagScope {
    KWLN_TASK_FLAG_SCOPE_TARGET = 0,
    KWLN_TASK_FLAG_SCOPE_OTHER_QUEUED_TASKS = 1,
    KWLN_TASK_FLAG_SCOPE_SUBTREE = 2,
    KWLN_TASK_FLAG_SCOPE_ALL_QUEUED_TASKS = 3
} KwlnTaskFlagScope;

/* Subtree includes the target; queued scopes visit all three scheduler queues. */
void kwlnTaskUpdateFlagsScoped(s32 setFlags, KwlnTask *task, u32 flags, s32 scope);
void dds3SetScopedObjectFlags(KwlnTask *object, u32 mask, s32 scope);
void dds3ClearScopedObjectFlags(KwlnTask *object, u32 mask, s32 scope);

#endif /* KWLN_TASK_FLAGS_H */
