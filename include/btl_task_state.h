#ifndef BTL_TASK_STATE_H
#define BTL_TASK_STATE_H

/* Values occupy the existing u16 phase and flag words of the scheduler task. */
enum BtlRuntimeTaskPhase {
    BTL_TASK_PHASE_WAITING = 0,
    BTL_TASK_PHASE_START_DELAY = 1,
    BTL_TASK_PHASE_RUNNING = 2,
    BTL_TASK_PHASE_END_DELAY = 3,
};

enum BtlRuntimeTaskFlags {
    BTL_TASK_FLAG_REGISTERED = 1,
    BTL_TASK_FLAG_DEFERRED = 2,
    BTL_TASK_FLAG_RELEASE_REQUESTED = 4,
    BTL_TASK_FLAG_STARTED = 8,
};

#endif /* BTL_TASK_STATE_H */
