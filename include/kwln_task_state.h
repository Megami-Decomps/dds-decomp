#ifndef KWLN_TASK_STATE_H
#define KWLN_TASK_STATE_H

/* The low flag nibble selects a scheduler queue; upper flag bits are independent. */
#define KWLN_TASK_STATE_MASK 0xF

typedef enum KwlnTaskState {
    KWLN_TASK_DETACHED = 0,
    KWLN_TASK_DELAYED_START = 1,
    KWLN_TASK_ACTIVE = 2,
    KWLN_TASK_DESTROY_PENDING = 3,
    KWLN_TASK_STATE_LIMIT = 4 /* Exclusive bound for registered states. */
} KwlnTaskState;

#endif /* KWLN_TASK_STATE_H */
