#ifndef SND_TRACK_CALLBACK_H
#define SND_TRACK_CALLBACK_H

#include "common.h"

/* The title task prepares and starts its packed track through the same operation
 * interface used by the menu reset. The callback expansion retains retail's
 * JAL plus epilogue at 002A58A8 when its target becomes a direct call. */
static inline void sndApplyTrackOperation(
        void (*operation)(s32), s32 packedTrackId) {
    operation(packedTrackId);
}

#endif
