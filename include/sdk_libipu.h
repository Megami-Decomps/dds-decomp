#ifndef SDK_LIBIPU_H
#define SDK_LIBIPU_H

/* SDK image processing unit library (libipu). One canonical prototype per function the game calls;
 * signatures come from retail call-site argument setup, the units' former local
 * declarations and PS2 SDK semantics. */

#include "common.h"

#ifndef SDK_SKIP_sceIpuInit
void sceIpuInit(void);
#endif
#ifndef SDK_SKIP_sceIpuStopDMA
void sceIpuStopDMA(void *state);
#endif
#ifndef SDK_SKIP_sceIpuSync
s32 sceIpuSync(s32 mode, s32 timeout);
#endif

#endif
