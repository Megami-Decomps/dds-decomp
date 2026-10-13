#ifndef SDF_THREAD_SLEEP_H
#define SDF_THREAD_SLEEP_H

#include "common.h"

/* Record the current thread, lower its priority for sleep, and return the
 * result from the SDK SleepThread call. */
s32 sdfThreadSleepSelf(void);

#endif /* SDF_THREAD_SLEEP_H */
