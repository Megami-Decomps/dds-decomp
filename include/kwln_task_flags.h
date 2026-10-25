#ifndef KWLN_TASK_FLAGS_H
#define KWLN_TASK_FLAGS_H

#include "kwln.h"

/* Flag updates preserve the scheduler state and the upper four bits. */
#define KWLN_TASK_MUTABLE_FLAGS_MASK 0x0FFFFFF0

/* Per-task controls consumed by RunProcess in the paired kernel providers. */
#define KWLN_TASK_CLEAR_INPUT_STATE 0x00000010
#define KWLN_TASK_SKIP_UPDATE 0x00000020

typedef enum KwlnTaskFlagScope {
    KWLN_TASK_FLAG_SCOPE_TARGET = 0,
    KWLN_TASK_FLAG_SCOPE_OTHER_QUEUED_TASKS = 1,
    KWLN_TASK_FLAG_SCOPE_SUBTREE = 2,
    KWLN_TASK_FLAG_SCOPE_ALL_QUEUED_TASKS = 3
} KwlnTaskFlagScope;

/* Subtree includes the target; queued scopes visit all three scheduler queues. */
void ChangeProcessFlag(s32 setFlags, dds3ProcessID_t *task, u32 flags, KwlnTaskFlagScope scope);
void dds3SetProcessFlag(dds3ProcessID_t *object, u32 mask, KwlnTaskFlagScope scope);
void dds3ResetProcessFlag(dds3ProcessID_t *object, u32 mask, KwlnTaskFlagScope scope);

#endif /* KWLN_TASK_FLAGS_H */
