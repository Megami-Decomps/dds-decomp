#ifndef SDK_LIBPAD_H
#define SDK_LIBPAD_H

/* SDK controller library (libpad). One canonical prototype per function the game calls;
 * signatures come from retail call-site argument setup, the units' former local
 * declarations and PS2 SDK semantics. */

#include "common.h"

#ifndef SDK_SKIP_scePadPortOpen
s32 scePadPortOpen(s32 port, s32 slot, void *buffer);
#endif
#ifndef SDK_SKIP_scePadSetActAlign
s32 scePadSetActAlign(s32 port, s32 slot, void *data);
#endif
#ifndef SDK_SKIP_scePadSetMainMode
s32 scePadSetMainMode(s32 port, s32 slot, s32 mode, s32 lock);
#endif

#endif
