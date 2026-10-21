#ifndef SDK_LIBCDVD_H
#define SDK_LIBCDVD_H

/* SDK CD/DVD library (libcdvd). One canonical prototype per function the game calls;
 * signatures come from retail call-site argument setup, the units' former local
 * declarations and PS2 SDK semantics. */

#include "common.h"
#include "sdf_cd_clock.h"

#ifndef SDK_SKIP_sceCdPowerOff
void sceCdPowerOff(void *result);
#endif
#ifndef SDK_SKIP_sceCdReadClock
s32 sceCdReadClock(SdfCdClock *clock);
#endif
#ifndef SDK_SKIP_sceCdSearchFile
s32 sceCdSearchFile(void *file, s32 path);
#endif
#ifndef SDK_SKIP_sceCdStatus
s32 sceCdStatus(void);
#endif

#endif
