#ifndef MNU_PROGRESS_LOAD_STATE_H
#define MNU_PROGRESS_LOAD_STATE_H

#include "common.h"

/* The progress host stays pending until its title/staff resources are ready. */
typedef s32 MnuProgressLoadState;
enum {
    MNU_PROGRESS_LOAD_NOT_STARTED = 0,
    MNU_PROGRESS_LOAD_LOADING = 1,
    MNU_PROGRESS_LOAD_READY = 2,
};

/* Pollers return whether startup loading is pending or complete. */
typedef s32 MnuProgressLoadPollResult;
enum {
    MNU_PROGRESS_LOAD_POLL_COMPLETE = 0,
    MNU_PROGRESS_LOAD_POLL_PENDING = 1,
};

#endif /* MNU_PROGRESS_LOAD_STATE_H */
