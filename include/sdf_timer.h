#ifndef SDF_TIMER_H
#define SDF_TIMER_H

#include "common.h"

/* Return the modulo-16-bit elapsed count from the EE timer 0 value. */
extern u32 sdfGetElapsedTimerTicks(u32 previous);

#endif /* SDF_TIMER_H */
