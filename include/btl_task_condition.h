#ifndef BTL_TASK_CONDITION_H
#define BTL_TASK_CONDITION_H

#include "common.h"

/* The task scheduler evaluates these predicates for either task phase.
 * Storage stays u8; count/handle/owner/task-kind data is stored separately. */
enum BtlTaskConditionKind {
    BTL_TASK_CONDITION_NEVER = 0,
    BTL_TASK_CONDITION_ALWAYS = 1,
    BTL_TASK_CONDITION_COUNT_REACHED = 2,
    BTL_TASK_CONDITION_HANDLE_PRESENT = 3,
    BTL_TASK_CONDITION_HANDLE_ABSENT = 4,
    BTL_TASK_CONDITION_HANDLE_RUNNING_OR_ABSENT = 5,
    BTL_TASK_CONDITION_OWNER_PRESENT = 6,
    BTL_TASK_CONDITION_OWNER_ABSENT = 7,
    BTL_TASK_CONDITION_OWNER_RUNNING_OR_ABSENT = 8,
    BTL_TASK_CONDITION_KIND_PRESENT = 9,
    BTL_TASK_CONDITION_KIND_ABSENT = 10,
};

/* Tagged scheduler predicate stored at the head of each task record. */
typedef struct TaskCondition {
    u8 kind;
    u8 pad01[7];
    union {
        s32 count;    /* Kind 2: signed threshold for the supplied counter. */
        u64 handle;   /* Kinds 3-5: task handle. */
        u64 owner;    /* Kinds 6-8: owner handle. */
        u16 taskKind; /* Kinds 9-10: registered task kind. */
    } value;
} TaskCondition;

typedef char TaskConditionSizeCheck[(sizeof(TaskCondition) == 0x10) ? 1 : -1];
typedef char TaskConditionKindOffsetCheck[
    ((u32)&((TaskCondition *)0)->kind == 0) ? 1 : -1];
typedef char TaskConditionValueOffsetCheck[
    ((u32)&((TaskCondition *)0)->value == 8) ? 1 : -1];

#endif /* BTL_TASK_CONDITION_H */
