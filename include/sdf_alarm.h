#ifndef SDF_ALARM_H
#define SDF_ALARM_H

#include "common.h"

/* Sleep the current thread until its alarm fires, truncating delay to 16 bits. */
void sdfSleepWithAlarm(u32 delay);

#endif /* SDF_ALARM_H */
