#ifndef SDK_LIBSDR_H
#define SDK_LIBSDR_H

/* SDK sound/IOP remote library (libsdr). One canonical prototype per function the game calls;
 * signatures come from retail call-site argument setup, the units' former local
 * declarations and PS2 SDK semantics. */

#include "common.h"

#ifndef SDK_SKIP_sceSdRemoteInit
s32 sceSdRemoteInit(void);
#endif
#ifndef SDK_SKIP_sndDispatchIopAudioCommand
s32 sndDispatchIopAudioCommand(s32 command, ...);
#endif

#ifdef VERSION_DDS2
#ifndef SDK_SKIP_sdfInitializeMemoryCardRpcClient
void sdfInitializeMemoryCardRpcClient(void);
#endif
#endif

#endif
