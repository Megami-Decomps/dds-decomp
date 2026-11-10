#ifndef DDS3_TASK_STATE_H
#define DDS3_TASK_STATE_H

/* The low flag nibble selects a scheduler queue; upper flag bits are independent. */
#define DDS3_TASK_STATE_MASK 0xF

typedef enum dds3TaskState_t {
    DDS3_TASK_DETACHED = 0,
    DDS3_TASK_DELAYED_START = 1,
    DDS3_TASK_ACTIVE = 2,
    DDS3_TASK_DESTROY_PENDING = 3,
    DDS3_TASK_STATE_LIMIT = 4 /* Exclusive bound for registered states. */
} dds3TaskState_t;

#endif /* DDS3_TASK_STATE_H */
