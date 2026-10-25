#ifndef SND_PACKED_TRACK_LOAD_STATUS_H
#define SND_PACKED_TRACK_LOAD_STATUS_H

#include "common.h"

/* These are lookup results, not booleans: pending and resident both matter. */
typedef s32 SndPackedTrackLoadStatus;
enum {
    SND_PACKED_TRACK_LOAD_NOT_PRESENT = 0,
    SND_PACKED_TRACK_LOAD_RESIDENT = 1,
    SND_PACKED_TRACK_LOAD_PENDING = 2
};

#endif /* SND_PACKED_TRACK_LOAD_STATUS_H */
