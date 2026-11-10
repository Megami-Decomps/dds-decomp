#ifndef DDS3_TASK_FLAGS_H
#define DDS3_TASK_FLAGS_H

#include "kwln.h"

/* Flag updates preserve the scheduler state and the upper four bits. */
#define DDS3_TASK_MUTABLE_FLAGS_MASK 0x0FFFFFF0

/* Per-task controls consumed by RunProcess in the paired kernel providers. */
#define DDS3_TASK_CLEAR_INPUT_STATE 0x00000010
#define DDS3_TASK_SKIP_UPDATE 0x00000020

typedef enum dds3TaskFlagScope_t {
    DDS3_TASK_FLAG_SCOPE_TARGET = 0,
    DDS3_TASK_FLAG_SCOPE_OTHER_QUEUED_TASKS = 1,
    DDS3_TASK_FLAG_SCOPE_SUBTREE = 2,
    DDS3_TASK_FLAG_SCOPE_ALL_QUEUED_TASKS = 3
} dds3TaskFlagScope_t;

/* The dispatcher clears for CLEAR and sets for any nonzero action. */
typedef s32 dds3TaskFlagUpdateAction_t;
enum {
    DDS3_TASK_FLAG_UPDATE_CLEAR = 0,
    DDS3_TASK_FLAG_UPDATE_SET = 1
};

/* Subtree includes the target; queued scopes visit all three scheduler queues. */
void ChangeProcessFlag(dds3TaskFlagUpdateAction_t action, dds3ProcessID_t *task,
                       u32 flags, dds3TaskFlagScope_t scope);
void dds3SetProcessFlag(dds3ProcessID_t *object, u32 mask, dds3TaskFlagScope_t scope);
void dds3ResetProcessFlag(dds3ProcessID_t *object, u32 mask, dds3TaskFlagScope_t scope);

#endif /* DDS3_TASK_FLAGS_H */
