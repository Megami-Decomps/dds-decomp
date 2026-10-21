#ifndef SDK_LIBDMA_H
#define SDK_LIBDMA_H

/* SDK DMA library (libdma). One canonical prototype per function the game calls;
 * signatures come from retail call-site argument setup, the units' former local
 * declarations and PS2 SDK semantics. */

#include "common.h"

#ifndef SDK_SKIP_sceDmaGetChan
void *sceDmaGetChan(s32 channel);
#endif
#ifndef SDK_SKIP_sceDmaSend
void sceDmaSend(void *channel, void *packet);
#endif
#ifndef SDK_SKIP_sceDmaSendN
void sceDmaSendN(void *channel, void *address, s32 size);
#endif
#ifndef SDK_SKIP_sceDmaSync
s32 sceDmaSync(void *channel, s32 mode, s32 timeout);
#endif

#endif
